/**
 * Description:  Time implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 13, 2024
*/
#include "core/prelude.h"
#include "core/time.h"
#include "core/string.h"
#include "core/fmt.h"
#include "core/memory.h"
#include "core/internal/platform.h"
#include "core/math/common.h"

attr_global u8 global_day_month_table[] = {
    31,// jan
    28,// feb
    31,// mar
    30,// apr
    31,// may
    30,// jun
    31,// jul
    31,// aug
    30,// sep
    31,// oct
    30,// nov
    31 // dec
};

attr_core_api TimePosix time_posix(void) {
    return platform_time_posix();
}
attr_core_api TimeSplit time_split(void) {
    return platform_time_split();
}
attr_core_api b32 time_hour_24_to_12( TimeHour hr24, TimeHour* out_hr12 ) {
    TimeHour hr12 = hr24 % 12;
    b32      am   = hr24 < 12;

    *out_hr12 = hr12 ? hr12 : 12;
    return am;
}
attr_core_api f64 timer_milliseconds(void) {
    return platform_timer_milliseconds();
}
attr_core_api f64 timer_seconds(void) {
    return platform_timer_seconds();
}
attr_core_api u32 time_day_month_to_day_week( u32 year, u32 month, u32 day ) {
    return (day += month < 3 ? year-- : year - 2, 23 * month / 9 + day + 4 + year / 4 - year / 100 + year / 400 ) % 7;
}
attr_core_api u32 time_day_month_to_day_year( u32 year, u32 month, u32 day_month ) {
    u32 m_idx = num_clamp( month, 1, 12 ) - 1;
    u32 days  = 0;
    b32 leap  = time_year_is_leap( year );

    for( u32 i = 0; i < m_idx; ++i ) {
        days += global_day_month_table[i];
        if( i == 1 && leap ) {
            days++;
        }
    }
    return days + day_month;
}
attr_core_api b32 time_year_is_leap( u32 year ) {
    if( !(year % 400) ) {
        return true;
    } else if( !(year % 100) ) {
        return false;
    } else if( !(year % 4) ) {
        return true;
    } else {
        return false;
    }
}

#define DEFAULT_FORMAT string_text( "[mm]/[ddd]/[yyyy] [h]:[m]:[s] [hm]" )

attr_internal usize internal_time_dummy_stream(
    void* _, usize count, const void* __
) {
    unused(_,__);
    return count;
}

attr_core_api usize stream_fmt_time(
    StreamBytesFN* stream, void* target, const struct TimeSplit* ts,
    int padding, usize opt_format_len, const char* opt_format
) {
    String format = string_new( opt_format_len, opt_format );
    if( !format.len || !format.cc ) {
        format = DEFAULT_FORMAT;
    }

    u32 upadding = num_abs( padding );
    if( padding ) {
        usize final_len = stream_fmt_time(
            internal_time_dummy_stream, target, ts,
            0, opt_format_len, opt_format );
        if( final_len <= upadding ) {
            upadding -= final_len;
        }
    }

    usize res = 0;

    char pad = ' ';
    if( padding > 0 ) {
        for( u32 i = 0; i < upadding; ++i ) {
            res += stream( target, 1, &pad );
        }
    }

    struct IntFormatArguments iargs;
    iargs.flags = FMT_INT_BITDEPTH_32 | FMT_INT_ZERO_PAD;
    while( !string_is_empty( format ) ) {
        usize open = 0;
        if( string_find( format, '[', &open ) ) {
            String arg = string_advance_by( format, open + 1 ); {
                usize to_arg = arg.cc - format.cc;
                res += stream( target, to_arg ? to_arg - 1 : 0, format.cc );
            }

            format = arg;

            if( arg.cc[0] == '[' ) {
                char c = '[';
                res   += stream( target, 1, &c );
                format = string_advance( format );
                continue;
            }

            usize close = 0;
            if( !string_find( arg, ']', &close ) ) {
                break;
            }

            arg.len = close;
            format  = string_advance_by( format, close + 1 );

            switch( arg.cc[0] ) {
                case 'y': {
                    switch( arg.len ) {
                        case 2: {
                            if( !memory_cmp( arg.cc, "yy", 2 ) ) {
                                return res;
                            }
                            u32 year = ts->year % 100;
                            res += stream_fmt_int(
                                stream, target, 2, 1, &year, &iargs );
                        } break;
                        case 4: {
                            if( !memory_cmp( arg.cc, "yyyy", 4 ) ) {
                                return false;
                            }
                            res += stream_fmt_int(
                                stream, target, 2, 1, &ts->year, &iargs );
                        } break;
                        default: return res;
                    }
                } break;
                case 'm': {
                    switch( arg.len ) {
                        case 1: {
                            res += stream_fmt_int(
                                stream, target, 2, 1, &ts->minute, &iargs );
                        } break;
                        case 2: {
                            if( !memory_cmp( arg.cc, "mm", 2 ) ) {
                                return res;
                            }
                            res += stream_fmt_int(
                                stream, target, 2, 1, &ts->month, &iargs );
                        } break;
                        case 3: {
                            if( !memory_cmp( arg.cc, "mmm", 3 ) ) {
                                return res;
                            }
                            String short_name;
                            short_name.cc = time_month_to_string_short(
                                ts->month, &short_name.len );

                            res += stream( target, short_name.len, short_name.cc );
                        } break;
                        case 4: {
                            if( !memory_cmp( arg.cc, "mmmm", 4 ) ) {
                                return res;
                            }
                            String name;
                            name.cc = time_month_to_string(
                                ts->month, &name.len );

                            res += stream( target, name.len, name.cc );
                        } break;
                        default: return res;
                    }
                } break;
                case 'd': {
                    switch( arg.len ) {
                        case 1: {
                            u32 day = time_day_month_to_day_week(
                                ts->year, ts->month, ts->day ) + 1;
                            res += stream_fmt_int(
                                stream, target, 2, 1, &day, &iargs );
                        } break;
                        case 2: {
                            if( !memory_cmp( arg.cc, "dd", 2 ) ) {
                                return res;
                            }
                            u32 day_num = time_day_month_to_day_week(
                                ts->year, ts->month, ts->day );
                            String day;
                            day.cc = time_day_week_to_string( day_num, &day.len );
                            res   += stream( target, day.len, day.cc );
                        } break;
                        case 3: {
                            if( !memory_cmp( arg.cc, "ddd", 3 ) ) {
                                return res;
                            }

                            res += stream_fmt_int(
                                stream, target, 2, 1, &ts->day, &iargs );
                        } break;
                        case 4: {
                            if( !memory_cmp( arg.cc, "dddd", 4 ) ) {
                                return res;
                            }

                            u32 day = time_day_month_to_day_year(
                                ts->year, ts->month, ts->day );
                            res += stream_fmt_int( stream, target, 3, 1, &day, &iargs );
                        } break;
                        default: return res;
                    }
                } break;
                case 'h': {
                    if( arg.len == 1 ) {
                        u32 hr12 = U32_MAX;
                        time_hour_24_to_12( ts->hour, &hr12 );
                        res += stream_fmt_int( stream, target, 2, 1, &hr12, &iargs );
                    } else {
                        if( string_cmp( arg, string_text( "hh" ) ) ) {
                            res += stream_fmt_int(
                                stream, target, 2, 1, &ts->hour, &iargs );
                        } else if( string_cmp( arg, string_text( "hm" ) ) ) {
                            b32 am = ts->hour < 12;
                            const char* str = am ? "AM" : "PM";
                            res += stream( target, 2, str );
                        } else if( string_cmp( arg, string_text( "hml" ) ) ) {
                            b32 am = ts->hour < 12;
                            const char* str = am ? "am" : "pm";
                            res += stream( target, 2, str );
                        } else {
                            return res;
                        }
                    }
                } break;
                case 's': {
                    if( arg.len == 1 ) {
                        res += stream_fmt_int(
                            stream, target, 2, 1, &ts->second, &iargs );
                    } else {
                        return res;
                    }
                } break;
                default: return res;
            }

        } else {
            res += stream( target, format.len, format.cc );
            break;
        }
    }

    if( padding < 0 ) {
        for( u32 i = 0; i < upadding; ++i ) {
            res += stream( target, 1, &pad );
        }
    }

    return res;
}

#undef DEFAULT_FORMAT
