/**
 * Description:  String formatting implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 14, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/stream.h" // IWYU pragma: keep
#include "core/fmt.h"
#include "core/memory.h"
#include "core/string.h"
#include "core/path.h"
#include "core/print.h"

#include "core/time.h"
#include "core/macros.h"
#include "core/constants.h"
#include "core/math.h"

attr_global char FMT_DIGITS_BINARY[2]   = { '0', '1' };
attr_global char FMT_DIGITS_DECIMAL[10] = {
    '0', '1', '2',
    '3', '4', '5',
    '6', '7', '8',
    '9'
};
attr_global char FMT_DIGITS_HEXADECIMAL_UPPER[16] = {
    '0', '1', '2',
    '3', '4', '5',
    '6', '7', '8',
    '9', 'A', 'B',
    'C', 'D', 'E',
    'F'
};
attr_global char FMT_DIGITS_HEXADECIMAL_LOWER[16] = {
    '0', '1', '2',
    '3', '4', '5',
    '6', '7', '8',
    '9', 'a', 'b',
    'c', 'd', 'e',
    'f'
};
#define BASE_BINARY      (2)
#define BASE_DECIMAL     (10)
#define BASE_HEXADECIMAL (16)
#define NUMBER_FORMAT_BUFFER_SIZE (128)

union FmtValue {
    const void*  ptr;
    TimeSplit    ts;
    b32          b32;
    int          i;
    unsigned int u;
    u8           u8;
    u16          u16;
    u32          u32;
    u64          u64;
    i8           i8;
    i16          i16;
    i32          i32;
    i64          i64;
    isize        isize;
    usize        usize;
    String       s;
    Path         p;
    const char*  cc;
    vec2         v2;
    vec3         v3;
    vec4         v4;
    c32          c;
    f32           f;
    f32         f32;
    f64         f64;
};

enum FmtIntWidth {
    FMT_INT_WIDTH_NORMAL,
    FMT_INT_WIDTH_SEPARATE,
    FMT_INT_WIDTH_FULL,
};

attr_internal usize stream_repeat(
    StreamBytesFN* stream, void* target, u32 n, char c
) {
    usize res = 0;
    for( u32 i = 0; i < n; ++i ) {
        res += stream( target, 1, &c );
    }
    return res;
}
attr_internal usize stream_padded(
    StreamBytesFN* stream, void* target,
    int padding, char padding_c, usize len, const char* message
) {
    b32 right_pad = padding < 0;
    u32 upadding  = num_abs( padding );
    if( len > upadding ) {
        upadding = 0;
    } else {
        upadding -= len;
    }

    usize res = 0;

    if( right_pad ) {
        res += stream( target, len, message );
        res += stream_repeat( stream, target, upadding, padding_c );
    } else {
        res += stream_repeat( stream, target, upadding, padding_c );
        res += stream( target, len, message );
    }

    return res;
}

attr_core_api usize stream_fmt_bool(
    StreamBytesFN* stream, void* target,
    int padding, u32 count, const b32* booleans,
    struct BoolFormatArguments* args
) {
    if( !count ) {
        return 0;
    }

    usize res = 0;
    if( count > 1 ) {
        res += stream( target, 2, "{ " );
    }

    for( u32 i = 0; i < count; ++i ) {
        String message = args->binary ?
            ( booleans[i] ? string_text( "1" ) : string_text( "0" ) ) :
            ( booleans[i] ? string_text( "true" ) : string_text( "false" ) );

        res += stream_padded(
            stream, target, padding, ' ', message.len, message.cbuf );

        if( i + 1 != count ) {
            res += stream( target, 2, ", " );
        }
    }

    if( count > 1 ) {
        res += stream( target, 2, " }" );
    }

    return res;
}
attr_core_api usize stream_fmt_char(
    StreamBytesFN* stream, void* target,
    int pad, u32 count, const char* characters,
    struct CharFormatArguments* args
) {
    if( !count ) {
        return 0;
    }
    usize res = 0;

    if( count > 1 ) {
        res += stream( target, 2, "{ " );
    }

    // TODO(alicia): handle utf8 properly!
    for( u32 i = 0; i < count; ++i ) {
        char c = characters[i];
        switch( args->casing ) {
            case FMT_CASING_AS_IS: break;
            case FMT_CASING_UPPER: {
                c = ascii_to_upper( c );
            } break;
            case FMT_CASING_LOWER: {
                c = ascii_to_lower( c );
            } break;
        }

        if( args->repeat ) {
            b32 right_pad = pad < 0;
            u32 upadding  = num_abs( pad );
            if( args->repeat > upadding ) {
                upadding = 0;
            } else {
                upadding -= args->repeat;
            }

            if( right_pad ) {
                res += stream_repeat( stream, target, args->repeat, c );
                res += stream_repeat( stream, target, upadding, ' ' );
            } else {
                res += stream_repeat( stream, target, upadding, ' ' );
                res += stream_repeat( stream, target, args->repeat, c );
            }
        } else {
            res += stream_padded(
                stream, target, pad, ' ', 1, &c );
        }
        if( i + 1 != count ) {
            res += stream( target, 2, ", " );
        }
    }

    if( count > 1 ) {
        res += stream( target, 2, " }" );
    }

    return res;
}
attr_internal usize internal_stream_fmt_path(
    StreamBytesFN* stream, void* target,
    Path path, struct StringFormatArguments* args
) {
    usize result = 0;
    if( args->flags & FMT_STRING_PATH_CANONICALIZE ) {
        result += path_stream_canonicalize_utf8( stream, target, path );
    } else {
        result += path_stream_convert_to_utf8( stream, target, path );
    }
    return result;
}
attr_internal usize internal_stream_fmt_string(
    StreamBytesFN* stream, void* target,
    String string, struct StringFormatArguments* args
) {
    FormatStringFlags casing = (args->flags & FMT_STRING_CASING_MASK) >> 4;
    usize result = 0;
    switch( casing ) {
        case FMT_STRING_CASING_UPPER_BIT: {
            result += string_stream_to_upper( stream, target, string );
        } break;
        case FMT_STRING_CASING_LOWER_BIT: {
            result += string_stream_to_lower( stream, target, string );
        } break;
        default: {
            result += stream( target, string.len, string._void );
        } break;
    }
    return result;
}
attr_core_api usize stream_fmt_string(
    StreamBytesFN* stream, void* target,
    int pad, usize string_len, const void* string,
    struct StringFormatArguments* args
) {
    if( !string_len ) {
        return 0;
    }
    usize res = 0;

    b32 right_pad = pad < 0;
    u32 upadding  = num_abs( pad );
    if( string_len > upadding ) {
        upadding = 0;
    } else {
        upadding -= string_len;
    }

    if( !right_pad ) {
        res += stream_repeat( stream, target, upadding, ' ' );
    }

    if( args->flags & FMT_STRING_IS_PATH ) {
        res += internal_stream_fmt_path(
            stream, target, path_new( string_len, (PathCharacter*)string ), args );
    } else {
        res += internal_stream_fmt_string(
            stream, target, string_new( string_len, string ), args );
    }

    if( right_pad ) {
        res += stream_repeat( stream, target, upadding, ' ' );
    }

    return res;
}
attr_internal f64 internal_float_index(
    int bitdepth, const void* floats, usize index
) {
    f64 res = 0.0;
    switch( bitdepth ) {
        case 32: {
            res = *((f32*)floats + index);
        } break;
        case 64: {
            res = *((f64*)floats + index);
        } break;
        default: unreachable();
    }
    return res;
}
attr_internal void internal_float_fmt(
    f64 value, b32 separate, int precision, StringBuf* buf );
attr_internal void internal_memory_fmt(
    f64 value, int precision, b32 kibi, StringBuf* buf );
attr_core_api usize stream_fmt_float(
    StreamBytesFN* stream, void* target,
    int pad, u32 count, const void* floats,
    struct FloatFormatArguments* args
) {
    if( !count ) {
        return 0;
    }
    usize res = 0;

    if( count > 1 ) {
        res += stream( target, 2, "{ " );
    }

    char _buf[NUMBER_FORMAT_BUFFER_SIZE];
    memory_zero( _buf, sizeof(_buf) );

    StringBuf buf = string_buf_new( sizeof(_buf), _buf );

    int precision = args->precision;
    if( precision < 0 ) {
        precision = 0;
    } else if( precision > FMT_FLOAT_MAX_PRECISION ) {
        precision = FMT_FLOAT_MAX_PRECISION;
    }

    int bitdepth   = bitfield_check( args->flags, FMT_FLOAT_F64 ) ? 64 : 32;
    char padding_c = bitfield_check( args->flags, FMT_FLOAT_ZERO_PAD ) ? '0' : ' ';
    b32 memory     = bitfield_check( args->flags, FMT_FLOAT_MEMORY ) != 0;

    b32 kibi             = bitfield_check( args->flags, FMT_FLOAT_MEMORY_KIBI );
    u32 normalized_count = count;
    int vector_counter   = 0;
    int max_counter      = 0;

    b32 seperate = bitfield_check( args->flags, FMT_FLOAT_SEPARATE );

    switch( args->flags & FMT_FLOAT_VECTOR_MASK ) {
        case FMT_FLOAT_VECTOR2: {
            normalized_count *= 2;
            max_counter       = 2;
            memory = false;
            kibi   = false;
        } break;
        case FMT_FLOAT_VECTOR3: {
            normalized_count *= 3;
            max_counter       = 3;
            memory = false;
            kibi   = false;
        } break;
        case FMT_FLOAT_VECTOR4: {
            normalized_count *= 4;
            max_counter       = 4;
            memory = false;
            kibi   = false;
        } break;
        default: {
            vector_counter = -1;
        } break;
    }

    for( u32 i = 0; i < normalized_count; ++i ) {
        if( vector_counter >= 0 ) {
            if( !vector_counter ) {
                res += stream( target, 2, "{ " );
            }
        }
        f64 value = internal_float_index( bitdepth, floats, i );

        if( memory ) {
            internal_memory_fmt( value, precision, kibi, &buf );
        } else {
            internal_float_fmt( value, seperate, precision, &buf );
        }

        res += stream_padded(
            stream, target, pad, padding_c, buf.len, buf.cbuf );

        if( vector_counter >= 0 ) {
            vector_counter++;
            if( vector_counter >= max_counter ) {
                res += stream( target, 2, " }" );
                vector_counter = 0;
            }
        }

        if( i + 1 < normalized_count ) {
            res += stream( target, 2, ", " );
        }
        string_buf_clear( &buf );
    }

    if( count > 1 ) {
        res += stream( target, 2, " }" );
    }
    return res;
}
attr_internal u64 internal_int_index(
    b32 is_signed, int bitdepth, const void* integers, usize index
) {
    if( is_signed ) {
        i64 res = 0;
        switch( bitdepth ) {
            case 8: {
                res = *((i8*)integers + index);
            } break;
            case 16: {
                res = *((i16*)integers + index);
            } break;
            case 32: {
                res = *((i32*)integers + index);
            } break;
            case 64: {
                res = *((i64*)integers + index);
            } break;
        }
        return rcast( u64, &res );
    } else {
        switch( bitdepth ) {
            case 8: {
                return *((u8*)integers + index);
            } break;
            case 16: {
                return *((u16*)integers + index);
            } break;
            case 32: {
                return *((u32*)integers + index);
            } break;
            case 64: {
                return *((u64*)integers + index);
            } break;
        }
    }
    unreachable();
}
attr_internal void internal_int_fmt(
    u64 value, b32 is_signed, int bitdepth, int base,
    enum FmtIntWidth width, StringBuf* buf );
attr_core_api usize stream_fmt_int(
    StreamBytesFN* stream, void* target,
    int pad, u32 count, const void* integers,
    struct IntFormatArguments* args
) {
    if( !count ) {
        return 0;
    }
    usize res = 0;

    if( count > 1 ) {
        res += stream( target, 2, "{ " );
    }

    char _buf[NUMBER_FORMAT_BUFFER_SIZE];
    memory_zero( _buf, sizeof(_buf) );

    StringBuf buf = string_buf_new( sizeof(_buf), _buf );

    int bitdepth = 8;
    switch( args->flags & FMT_INT_BITDEPTH_MASK ) {
        case FMT_INT_BITDEPTH_16: {
            bitdepth = 16;
        } break;
        case FMT_INT_BITDEPTH_32: {
            bitdepth = 32;
        } break;
        case FMT_INT_BITDEPTH_64: {
            bitdepth = 64;
        } break;
        case FMT_INT_BITDEPTH_PTR: {
            bitdepth = sizeof(usize) * 8;
        } break;
        default: break;
    }
    char padding_c = ' ';
    if( bitfield_check( args->flags, FMT_INT_ZERO_PAD ) ) {
        if( !bitfield_check( args->flags, FMT_INT_SEPARATE ) ) {
            switch( args->flags & FMT_INT_BASE_MASK ) {
                case 0: {
                    padding_c = '0';
                } break;
                default: break;
            }
        }
    }
    b32 memory = bitfield_check( args->flags, FMT_INT_MEMORY );

    b32 kibi             = bitfield_check( args->flags, FMT_INT_MEMORY_KIBI );
    u32 normalized_count = count;
    int vector_counter   = 0;
    int max_counter      = 0;
    b32 is_signed        = bitfield_check( args->flags, FMT_INT_SIGNED );

    enum FmtIntWidth width = FMT_INT_WIDTH_NORMAL;
    if( bitfield_check( args->flags, FMT_INT_SEPARATE ) ) {
        width = FMT_INT_WIDTH_SEPARATE;
    } else if( bitfield_check( args->flags, FMT_INT_FULL_WIDTH ) ) {
        width = FMT_INT_WIDTH_FULL;
    }
    int base = args->flags & FMT_INT_BASE_MASK;

    switch( args->flags & FMT_INT_VECTOR_MASK ) {
        case FMT_INT_VECTOR2: {
            normalized_count *= 2;
            max_counter       = 2;
            memory = false;
            kibi   = false;
        } break;
        case FMT_INT_VECTOR3: {
            normalized_count *= 3;
            max_counter       = 3;
            memory = false;
            kibi   = false;
        } break;
        case FMT_INT_VECTOR4: {
            normalized_count *= 4;
            max_counter       = 4;
            memory = false;
            kibi   = false;
        } break;
        default: {
            vector_counter = -1;
        } break;
    }

    for( u32 i = 0; i < normalized_count; ++i ) {
        if( vector_counter >= 0 ) {
            if( !vector_counter ) {
                res += stream( target, 2, "{ " );
            }
        }
        u64 value = internal_int_index( is_signed, bitdepth, integers, i );

        if( memory ) {
            internal_memory_fmt( (f64)value, 2, kibi, &buf );
        } else {
            internal_int_fmt(
                value, is_signed, bitdepth,
                base, width, &buf );
        }

        res += stream_padded(
            stream, target, pad, padding_c, buf.len, buf.cbuf );

        if( vector_counter >= 0 ) {
            vector_counter++;
            if( vector_counter >= max_counter ) {
                res += stream( target, 2, " }" );
                vector_counter = 0;
            }
        }

        if( i + 1 < normalized_count ) {
            res += stream( target, 2, ", " );
        }
        string_buf_clear( &buf );
    }

    if( count > 1 ) {
        res += stream( target, 2, " }" );
    }
    return res;

}
attr_core_api usize stream_fmt_args(
    StreamBytesFN* stream, void* target, FormatArguments* args
) {
    switch( args->type ) {
        case FT_BOOL: {
            return stream_fmt_bool(
                stream, target, args->padding,
                args->count, args->data, &args->boolean );
        } break;
        case FT_CHAR: {
            return stream_fmt_char(
                stream, target, args->padding,
                args->count, args->data, &args->character );
        } break;
        case FT_STRING: {
            return stream_fmt_string(
                stream, target, args->padding,
                args->count, args->data, &args->string );
        } break;
        case FT_FLOAT: {
            return stream_fmt_float(
                stream, target, args->padding,
                args->count, args->data, &args->floating );
        } break;
        case FT_INT: {
            return stream_fmt_int(
                stream, target, args->padding,
                args->count, args->data, &args->integer );
        } break;
        case FT_TIME: {
            return stream_fmt_time(
                stream, target, args->data,
                args->padding, args->time.fmt_len,
                args->time.fmt );
        } break;
    }
    unreachable();
}
attr_internal b32 internal_fmt_parse_args(
    String text, FormatArguments* args, union FmtValue* out_val, va_list va );
attr_core_api usize stream_fmt(
    StreamBytesFN* stream, void* target,
    usize format_len, const char* format, ...
) {
    va_list va;
    va_start( va, format );
    usize res = stream_fmt_va( stream, target, format_len, format, va );
    va_end( va );
    return res;
}
attr_core_api usize stream_fmt_va(
    StreamBytesFN* stream, void* target,
    usize format_len, const char* format_cc, va_list va
) {
    String format = string_new( format_len, format_cc );

    usize res = 0;
    FormatArguments args;
    union FmtValue  val;
    memory_zero( &args, sizeof(args) );
    memory_zero( &val, sizeof(val) );
    while( !string_is_empty( format ) ) {
        usize open = 0;
        if( string_find( format, '{', &open ) ) {
            String args_text = string_advance_by( format, open );

            res   += stream( target, args_text.cbuf - format.cbuf, format.cbuf );
            format = string_advance_by( format, args_text.cbuf - format.cbuf );

            if( args_text.len >= 2 ) {
                if( args_text.cbuf[1] == '{' ) {
                    char c = '{';
                    res   += stream( target, 1, &c );
                    format = string_advance_by( format, 2 );
                    continue;
                }
            }

            usize close = 0;
            if( !string_find( args_text, '}', &close ) ) {
                res += stream( target, args_text.len, args_text.cbuf );
                break;
            }

            args_text.len = close;
            args_text     = string_advance( args_text );
            format        = string_advance_by( format, close + 1 );

            /* va_list va2; */
            /* va_copy( va2, va ); */
            if( internal_fmt_parse_args( args_text, &args, &val, va ) ) {
                if( !args.data ) {
                    args.data = &val;
                }
                res += stream_fmt_args( stream, target, &args );
                memory_zero( &args, sizeof(args) );
                memory_zero( &val, sizeof(val) );
            }
            /* va_end( va2 ); */
        } else {
            res += stream( target, format.len, format.cbuf );
            break;
        }
    }
    return res;
}

attr_internal u32 internal_int_max_digits( int bitdepth, int base ) {
    switch( base ) {
        case FMT_INT_BINARY: {
            return bitdepth;
        } break;
        case FMT_INT_HEX_LOWER:
        case FMT_INT_HEX_UPPER: {
            return bitdepth / 4;
        } break;
        case 0: {
            switch( bitdepth ) {
                case 8:  return 3;
                case 16: return 5;
                case 32: return 10;
                case 64: return 20;
            }
        } break;
    }
    unreachable();
}
attr_internal void internal_int_0(
    enum FmtIntWidth width, int bitdepth, int base, StringBuf* buf
) {
    switch( base ) {
        case FMT_INT_BINARY: {
            string_buf_try_push( buf, '0' );
            string_buf_try_push( buf, 'b' );
            switch( width ) {
                case FMT_INT_WIDTH_NORMAL: {
                    string_buf_try_push( buf, '0' );
                } break;
                case FMT_INT_WIDTH_SEPARATE: {
                    for( int i = 0; i < bitdepth; ++i ) {
                        string_buf_try_push( buf, '0' );
                        if( i && !((i + 1) % 8) && (i + 1) != bitdepth ) {
                            string_buf_try_push( buf, '\'' );
                        }
                    }
                } break;
                case FMT_INT_WIDTH_FULL: {
                    for( int i = 0; i < bitdepth; ++i ) {
                        string_buf_try_push( buf, '0' );
                    }
                } break;
            }
        } break;
        case FMT_INT_HEX_LOWER:
        case FMT_INT_HEX_UPPER: {
            string_buf_try_push( buf, '0' );
            string_buf_try_push( buf, 'x' );
            switch( width ) {
                case FMT_INT_WIDTH_NORMAL: {
                    string_buf_try_push( buf, '0' );
                } break;
                case FMT_INT_WIDTH_SEPARATE: {
                    int max = bitdepth / 4;
                    for( int i = 0; i < max; ++i ) {
                        string_buf_try_push( buf, '0' );
                        if( i && !((i + 1) % 4) && (i + 1) != max ) {
                            string_buf_try_push( buf, '\'' );
                        }
                    }
                } break;
                case FMT_INT_WIDTH_FULL: {
                    int max = bitdepth / 4;
                    for( int i = 0; i < max; ++i ) {
                        string_buf_try_push( buf, '0' );
                    }
                } break;
            }
        } break;
        case 0: {
            switch( width ) {
                case FMT_INT_WIDTH_NORMAL: {
                    string_buf_try_push( buf, '0' );
                } break;
                case FMT_INT_WIDTH_SEPARATE: {
                    string_buf_try_push( buf, '0' );
                } break;
                case FMT_INT_WIDTH_FULL: {
                    u32 max = internal_int_max_digits( bitdepth, base );
                    for( u32 i = 0; i < max; ++i ) {
                        string_buf_try_push( buf, '0' );
                    }
                } break;
            }
        } break;
        default: unreachable();
    }
}
attr_internal void internal_int_fmt(
    u64 value, b32 is_signed, int bitdepth, int base,
    enum FmtIntWidth width, StringBuf* in_buf
) {
    if( !value ) {
        internal_int_0( width, bitdepth, base, in_buf );
        return;
    }

    b32 neg = false;
    u64 abs = value;

    u32   max_digit_count = internal_int_max_digits( bitdepth, base );
    char* digits          = NULL;
    u64   base_num        = 0;
    char  sep             = 0;
    u32   sep_at          = 0;
    switch( base ) {
        // base 10
        case 0: {
            if( is_signed ) {
                i64 signed_value = rcast( i64, &value );
                if( signed_value < 0 ) {
                    neg = true;
                    abs = num_abs( signed_value );
                }
            }
            base_num = 10;
            digits   = FMT_DIGITS_DECIMAL;
            sep      = ',';
            sep_at   = 3;
        } break;
        case FMT_INT_BINARY: {
            base_num = 2;
            digits   = FMT_DIGITS_BINARY;
            sep      = '\'';
            sep_at   = 8;

            string_buf_try_append( in_buf, string_text( "0b" ) );
        } break;
        case FMT_INT_HEX_UPPER:
        case FMT_INT_HEX_LOWER: {
            base_num = 16;
            digits   = (base == FMT_INT_HEX_UPPER) ?
                FMT_DIGITS_HEXADECIMAL_UPPER : FMT_DIGITS_HEXADECIMAL_LOWER;
            sep      = '\'';
            sep_at   = 4;

            string_buf_try_append( in_buf, string_text( "0x" ) );
        } break;
        default: unreachable();
    }

    if( neg ) {
        string_buf_try_push( in_buf, '-' );
    }
    u32 rev_start = in_buf->len;

    for( u32 i = 0; i < max_digit_count; ++i ) {
        char digit = digits[abs % base_num];

        string_buf_try_push( in_buf, digit );

        abs /= base_num;
        switch( width ) {
            case FMT_INT_WIDTH_NORMAL: {
                if( !abs ) {
                    goto internal_int_fmt_end;
                }
            } break;
            case FMT_INT_WIDTH_SEPARATE: {
                if( !base && !abs ) {
                    goto internal_int_fmt_end;
                }
                if( i && !((i + 1) % sep_at) && (i + 1) != max_digit_count ) {
                    string_buf_try_push( in_buf, sep );
                }
            } break;
            case FMT_INT_WIDTH_FULL: break;
        }
    }

internal_int_fmt_end:

    string_mut_reverse( string_advance_by( in_buf->slice, rev_start ) );
}
attr_internal void internal_float_fmt(
    f64 value, b32 seperate, int precision, StringBuf* buf 
) {
    // TODO(alicia): Grisu3
    if( f64_isnan( value ) ) {
        string_buf_try_append( buf, string_text( "NaN" ) );
        return;
    } else if( value == F64_POS_INFINITY ) {
        string_buf_try_append( buf, string_text( "INF" ) );
        return;
    } else if( value == F64_NEG_INFINITY ) {
        string_buf_try_append( buf, string_text( "-INF" ) );
        return;
    }

    i64 whole = (i64)value;
    f64 fract = num_abs( value ) - (f64)num_abs(whole);

    internal_int_fmt(
        rcast( u64, &whole ),
        true,
        64,
        0, // BASE_DECIMAL
        seperate ? FMT_INT_WIDTH_SEPARATE : FMT_INT_WIDTH_NORMAL,
        buf );
    if( fract <= 0.0000001 ) {
        if( precision ) {
            string_buf_try_push( buf, '.' );
            for( int i = 0; i < precision; ++i ) {
                string_buf_try_push( buf, '0' );
            }
        }
        return;
    }

    if( !precision ) {
        return;
    }

    string_buf_try_push( buf, '.' );
    for( int i = 0; i < precision; ++i ) {
        fract *= 10.0;
        u64  idx   = (u64)fract % 10;
        char digit = FMT_DIGITS_DECIMAL[idx];
        string_buf_try_push( buf, digit );
    }
}
attr_internal void internal_memory_fmt(
    f64 value, int precision, b32 kibi, StringBuf* buf
) {
    #define BYTES 0
    #define KB    1
    #define MB    2
    #define GB    3
    #define TB    4
    f64 f       = value;
    int storage = BYTES;
    f64 comp    = kibi ? 1024.0 : 1000.0;

    if( f >= comp ) {
        storage = KB;
        f /= comp;
        if( f >= comp ) {
            storage = MB;
            f /= comp;
            if( f >= comp ) {
                storage = GB;
                f /= comp;
                if( f >= comp ) {
                    storage = TB;
                    f /= comp;
                }
            }
        }
    }

    internal_float_fmt( f, true, precision, buf );
    String storage_text = string_text( " B" );
    switch( storage ) {
        case KB: {
            storage_text = kibi ? string_text( " KiB" ) : string_text( " KB" );
        } break;
        case MB: {
            storage_text = kibi ? string_text( " MiB" ) : string_text( " MB" );
        } break;
        case GB: {
            storage_text = kibi ? string_text( " GiB" ) : string_text( " GB" );
        } break;
        case TB: {
            storage_text = kibi ? string_text( " TiB" ) : string_text( " TB" );
        } break;
        default: break;
    }
    string_buf_try_append( buf, storage_text );

    #undef BYTES
    #undef KB   
    #undef MB   
    #undef GB   
    #undef TB   
}

#undef NUMBER_FORMAT_BUFFER_SIZE
#undef BASE_BINARY     
#undef BASE_DECIMAL    
#undef BASE_HEXADECIMAL

attr_internal b32 internal_fmt_parse_format_type(
    String spec, FormatArguments* out_args
) {
    if( string_is_empty( spec ) ) {
        return false;
    }
    switch( spec.cbuf[0] ) {
        case 'b': {
            if( spec.len > 1 ) {
                return false;
            }
            out_args->type = FT_BOOL;
        } return true;
        case 'c': {
            switch( spec.len ) {
                case 1: {
                    out_args->type = FT_CHAR;
                } return true;
                case 2: {
                    if( !string_cmp( spec, string_text( "cc" ) ) ) {
                        return false;
                    }
                    out_args->type = FT_STRING;
                } return true;
                default: break;
            }
        } break;
        case 'f': {
            switch( spec.len ) {
                case 1: {
                    out_args->type = FT_FLOAT;
                } return true;
                case 3: {
                    if( string_cmp( spec, string_text( "f32" ) ) ) {
                        out_args->type = FT_FLOAT;
                    } else if( string_cmp( spec, string_text( "f64" ) ) ) {
                        out_args->floating.flags |= FMT_FLOAT_F64;
                        out_args->type            = FT_FLOAT;
                    } else {
                        return false;
                    }
                } return true;
                default: break;
            }
        } break;
        case 'v': {
            switch( spec.len ) {
                case 2: {
                    switch( spec.cbuf[1] ) {
                        case '2': {
                            out_args->floating.flags |= FMT_FLOAT_VECTOR2;
                            out_args->type            = FT_FLOAT;
                        } break;
                        case '3': {
                            out_args->floating.flags |= FMT_FLOAT_VECTOR3;
                            out_args->type            = FT_FLOAT;
                        } break;
                        case '4': {
                            out_args->floating.flags |= FMT_FLOAT_VECTOR4;
                            out_args->type            = FT_FLOAT;
                        } break;
                        default: return false;
                    }
                } return true;
                default: break;
            }
        } break;
        case 'i':
        case 'u': {
            if( spec.cbuf[0] == 'i' ) {
                out_args->integer.flags |= FMT_INT_SIGNED;
            }
            switch( spec.len ) {
                case 1: {
                    out_args->integer.flags |= FMT_INT_BITDEPTH_32;
                    out_args->type           = FT_INT;
                } return true;
                case 2: {
                    if( spec.cbuf[1] != '8' ) {
                        return false;
                    }
                    out_args->integer.flags |= FMT_INT_BITDEPTH_8;
                    out_args->type           = FT_INT;
                } return true;
                case 3: {
                    String bitdepth = string_advance( spec );
                    if( string_cmp( bitdepth, string_text( "32" ) ) ) {
                        out_args->integer.flags |= FMT_INT_BITDEPTH_32;
                        out_args->type           = FT_INT;
                    } else if( string_cmp( bitdepth, string_text( "64" ) ) ) {
                        out_args->integer.flags |= FMT_INT_BITDEPTH_64;
                        out_args->type           = FT_INT;
                    } else if( string_cmp( bitdepth, string_text( "16" ) ) ) {
                        out_args->integer.flags |= FMT_INT_BITDEPTH_16;
                        out_args->type           = FT_INT;
                    } else {
                        return false;
                    }
                } return true;
                case 5: {
                    if( !string_cmp(
                        string_advance( spec ), string_text( "size" )
                    ) ) {
                        return false;
                    }

                    out_args->integer.flags |= FMT_INT_BITDEPTH_PTR;
                    out_args->type           = FT_INT;
                } return true;
                default: return false;
            }
        } break;
        case 's': {
            if( spec.len > 1 ) {
                return false;
            }
            out_args->type = FT_STRING;
        } return true;
        case 'p': {
            if( spec.len > 1 ) {
                return false;
            }
            out_args->type = FT_STRING;
        } return true;
        case 't': {
            if( spec.len > 1 ) {
                return false;
            }
            out_args->type = FT_TIME;
        } return true;
    }
    return false;
}
attr_internal b32 internal_fmt_parse_args(
    String text, FormatArguments* args, union FmtValue* out_val, va_list va
) {

    #define skip()\
        goto internal_fmt_parse_args_skip

    String rem  = text;
    String spec = string_empty(); {
        usize  comma = 0;
        if( string_find( rem, ',', &comma ) ) {
            spec     = rem;
            spec.len = comma;
        } else {
            spec = rem;
        }
    }

    if( !internal_fmt_parse_format_type( spec, args ) ) {
        return false;
    }

    if( spec.cbuf[0] == 'p' ) {
        args->string.flags |= FMT_STRING_IS_PATH;
    }

    rem = string_advance_by( rem, spec.len + 1 );

    b32 pointer         = false;
    b32 count_by_value  = false;
    b32 repeat_by_value = false;

    // initialize arguments
    switch( args->type ) {
        case FT_CHAR: {
            args->character.repeat = 1;
        } break;
        case FT_STRING: {
            if( spec.cbuf[0] == 'c' ) {
                pointer = true;
            }
        } break;
        case FT_BOOL: {
            args->boolean.binary = false;
        } break;
        case FT_FLOAT: {
            args->floating.precision = 6;
        } break;
        case FT_INT:
        case FT_TIME: break;
    }

    while( !string_is_empty( rem ) ) {
        String arg = rem;
        usize comma = 0;
        if( string_find( rem, ',', &comma ) ) {
            arg.len = comma;
        }

        if( string_is_empty( arg ) ) {
            return false;
        }

        // pass by pointer
        switch( args->type ) {
            case FT_INT: case FT_FLOAT: case FT_STRING: case FT_CHAR:
            case FT_BOOL: {
                if( arg.cbuf[0] == '*' ) {
                    if( !(args->type == FT_STRING && spec.cbuf[0] == 's') ) {
                        pointer = true;
                    }
                    String arg_num = string_advance( arg );

                    if( arg_num.len ) {
                        if( arg_num.cbuf[0] == '_' && arg_num.len == 1 ) {
                            count_by_value = true;
                        } else {
                            u64 count = 0;
                            if( !string_parse_uint( arg_num, &count ) ) {
                                return false;
                            }

                            args->count = count;
                        }
                    } else {
                        args->count = 1;
                    }

                    skip();
                }
            } break;
            case FT_TIME:
                break;
        }
        // string and char arguments
        switch( args->type ) {
            case FT_CHAR:
            case FT_STRING: {
                if( string_cmp( arg, string_text( "u" ) ) ) {
                    if( args->type == FT_CHAR ) {
                        args->character.casing = FMT_CASING_UPPER;
                    } else {
                        args->string.flags |= FMT_STRING_CASING_UPPER;
                    }
                    skip();
                } else if( string_cmp( arg, string_text( "l" ) ) ) {
                    if( args->type == FT_CHAR ) {
                        args->character.casing = FMT_CASING_LOWER;
                    } else {
                        args->string.flags |= FMT_STRING_CASING_LOWER;
                    }
                    skip();
                }
            } break;
            case FT_BOOL: case FT_FLOAT: case FT_INT: case FT_TIME:
                break;
        }

        // padding
        switch( args->type ) {
            case FT_INT: case FT_BOOL: case FT_CHAR: case FT_STRING:
            case FT_TIME: {
                if( arg.cbuf[0] == '-' || ascii_is_numeric( arg.cbuf[0] ) ) {
                    i64 padding = 0;
                    if( !string_parse_int( arg, &padding ) ) {
                        return false;
                    }

                    args->padding = padding;

                    b32 start_zero = (arg.cbuf[0] == '0');
                    if( args->type == FT_INT && start_zero ) {
                        args->integer.flags |= FMT_INT_ZERO_PAD;
                    }
                    skip();
                }
            } break;
            case FT_FLOAT: 
                break;
        }

        // padding and precision for float
        switch( args->type ) {
            case FT_FLOAT: {
                if(
                    arg.cbuf[0] == '-' ||
                    arg.cbuf[0] == '.' ||
                    ascii_is_numeric( arg.cbuf[0] )
                ) {
                    String prec = string_empty();
                    if( arg.cbuf[0] != '.' ) {
                        i64 padding = 0;
                        if( !string_parse_int( arg, &padding ) ) {
                            return false;
                        }
                        args->padding = padding;

                        b32 start_zero = (arg.cbuf[0] == '0');
                        args->floating.flags |= start_zero ? FMT_FLOAT_ZERO_PAD : 0;
                    }

                    usize dot_pos = 0;
                    if( string_find( arg, '.', &dot_pos ) ) {
                        prec = string_advance_by( arg, dot_pos + 1 );
                    }

                    if( prec.len ) {
                        u64 precision = 0;
                        if( !string_parse_uint( prec, &precision ) ) {
                            return false;
                        }
                        args->floating.precision = num_clamp( (int)precision, 0, 6 );
                    }

                    skip();
                }
            } break;
            case FT_BOOL: case FT_CHAR: case FT_STRING: case FT_INT: case FT_TIME:
                break;
        }

        // int and float
        switch( args->type ) {
            case FT_FLOAT:
            case FT_INT: {
                switch( arg.cbuf[0] ) {
                    case 'm': {
                        b32 mem = false;
                        b32 mib = false;
                        switch( arg.len ) {
                            case 1: {
                                mem = true;
                            } break;
                            case 3: {
                                if( string_cmp( arg, string_text( "mib" ) ) ) {
                                    mem = true;
                                    mib = true;
                                }
                            } break;
                        }
                        if( mem ) {
                            if( args->type == FT_INT ) {
                                args->integer.flags |= FMT_INT_MEMORY;
                                args->integer.flags |=
                                    mib ? FMT_INT_MEMORY_KIBI : 0;
                            } else {
                                args->floating.flags |= FMT_FLOAT_MEMORY;
                                args->floating.flags |=
                                    mib ? FMT_FLOAT_MEMORY_KIBI : 0;
                            }
                            skip();
                        }
                    } break;
                    case 's': {
                        if( arg.len == 1 ) {
                            if( args->type == FT_INT ) {
                                args->integer.flags &= ~FMT_INT_FULL_WIDTH;
                                args->integer.flags |= FMT_INT_SEPARATE;
                            } else {
                                args->floating.flags |= FMT_FLOAT_SEPARATE;
                            }
                            skip();
                        }
                    } break;
                    default: break;
                }
            } break;
            case FT_BOOL: case FT_CHAR: case FT_STRING: case FT_TIME:
                break;
        }

        // int arguments only.
        switch( args->type ) {
            case FT_INT: {
                switch( arg.cbuf[0] ) {
                    case 'x': {
                        if( arg.len == 2 ) {
                            switch( arg.cbuf[1] ) {
                                case 'u': {
                                    args->integer.flags &=
                                        ~(FMT_INT_BINARY | FMT_INT_HEX_LOWER);
                                    args->integer.flags |=
                                        FMT_INT_HEX_UPPER;
                                    skip();
                                } break;
                                case 'l': {
                                    args->integer.flags &=
                                        ~(FMT_INT_BINARY | FMT_INT_HEX_UPPER);
                                    args->integer.flags |=
                                        FMT_INT_HEX_LOWER;
                                    skip();
                                } break;
                                default: break;
                            }
                        }
                    } break;
                    case 'b': {
                        if( arg.len == 1 ) {
                            args->integer.flags &=
                                ~(FMT_INT_HEX_UPPER | FMT_INT_HEX_LOWER);
                            args->integer.flags |= FMT_INT_BINARY;
                            skip();
                        }
                    } break;
                    case 'f': {
                        if( arg.len == 1 ) {
                            args->integer.flags &= ~FMT_INT_SEPARATE;
                            args->integer.flags |= FMT_INT_FULL_WIDTH;
                            skip();
                        }
                    } break;
                    default: break;
                }
            } break;
            case FT_BOOL: case FT_CHAR: case FT_STRING: case FT_FLOAT: case FT_TIME:
                break;
        }

        // time arguments only.
        switch( args->type ) {
            case FT_TIME: {
                if( !args->time.fmt ) {
                    if( string_cmp( arg, string_text( "*" ) ) ) {
                        pointer = true;
                        skip();
                    } else {
                        String inline_fmt = arg;
                        if( inline_fmt.len > 2 ) {
                            if(
                                inline_fmt.cbuf[0] == '\'' &&
                                inline_fmt.cbuf[inline_fmt.len - 1] == '\''
                            ) {
                                inline_fmt =
                                    string_advance( string_trim( inline_fmt, 1 ) );
                                args->time.fmt_len = inline_fmt.len;
                                args->time.fmt     = inline_fmt.cbuf;
                                skip();
                            }
                        }
                    }
                }
            } break;
            case FT_BOOL: case FT_CHAR: case FT_STRING: case FT_FLOAT: case FT_INT:
                break;
        }

        // boolean arguments only.
        switch( args->type ) {
            case FT_BOOL: {
                if( string_cmp( arg, string_text( "b" ) ) ) {
                    args->boolean.binary = true;
                    skip();
                }
            } break;
            case FT_CHAR: case FT_STRING: case FT_FLOAT: case FT_INT: case FT_TIME:
                break;
        }

        // path arguments only.
        switch( args->type ) {
            case FT_STRING: {
                if( spec.cbuf[0] == 'p' ) {
                    if( arg.len == 1 ) {
                        switch( arg.cbuf[0] ) {
                            case 'p': {
                                args->string.flags |= FMT_STRING_PATH_REPLACE_SEPARATORS;
                                skip();
                            } break;
                            case 'c': {
                                args->string.flags |= FMT_STRING_PATH_CANONICALIZE;
                                skip();
                            } break;
                        }
                    }
                }
            } break;
            case FT_CHAR: case FT_FLOAT: case FT_INT: case FT_TIME: case FT_BOOL:
                break;
        }

        // char arguments only.
        switch( args->type ) {
            case FT_CHAR: {
                if( arg.cbuf[0] == 'r' ) {
                    String arg_num = string_advance( arg );
                    b32 parse_int  = false;
                    switch( arg_num.len ) {
                        case 0: {
                            args->character.repeat = 2;
                        } break;
                        case 1: {
                            if( arg_num.cbuf[0] == '_' ) {
                                repeat_by_value = true;
                            } else {
                                parse_int = true;
                            }
                        } break;
                        default: {
                            parse_int = true;
                        } break;
                    }
                    if( parse_int ) {
                        u64 repeat = 0;
                        if( !string_parse_uint( arg_num, &repeat ) ) {
                            return false;
                        }
                        args->character.repeat = repeat;
                    }

                    skip();
                }
            } break;
            case FT_BOOL: case FT_STRING: case FT_FLOAT: case FT_INT: case FT_TIME:
                break;
        }

        // argument is unrecognized.
        return false;

internal_fmt_parse_args_skip:
        rem = string_advance_by( rem, arg.len + 1 );
    }

    if( count_by_value ) {
        if( !repeat_by_value ) {
            args->count = va_arg( va, u32 );
        }
    }

    if( pointer && args->type != FT_TIME ) {
        args->data = va_arg( va, const void* );
        if( args->type == FT_STRING ) {
            usize strlen = cstr_len( (const cstr*)args->data );
            if( !args->count || args->count > strlen ) {
                args->count = strlen;
            }
        }
    } else {
        if( args->type != FT_STRING ) {
            args->count = 1;
        }
        switch( args->type ) {
            case FT_BOOL: {
                out_val->b32 = va_arg( va, b32 );
            } break;
            case FT_CHAR: {
                if( repeat_by_value ) {
                    args->character.repeat = va_arg( va, u32 );
                }
                out_val->c  = va_arg( va, u32 );
            } break;
            case FT_STRING: {
                String str = va_arg( va, String );

                if( args->count <= 0 || args->count > str.len ) {
                    args->count = str.len;
                }

                args->data = str.cbuf;
            } break;
            case FT_FLOAT: {
                switch( args->floating.flags & FMT_FLOAT_VECTOR_MASK ) {
                    case FMT_FLOAT_VECTOR2: {
                        out_val->v2 = va_arg( va, vec2 );
                    } break;
                    case FMT_FLOAT_VECTOR3: {
                        out_val->v3 = va_arg( va, vec3 );
                    } break;
                    case FMT_FLOAT_VECTOR4: {
                        out_val->v4 = va_arg( va, vec4 );
                    } break;
                    case 0: {
                        if( bitfield_check( args->floating.flags, FMT_FLOAT_F64 ) ) {
                            out_val->f64 = va_arg( va, f64 );
                        } else {
                            out_val->f32 = va_arg( va, f64 );
                        }
                    } break;
                }
            } break;
            case FT_INT: {
                switch( args->integer.flags & FMT_INT_VECTOR_MASK ) {
                    case FMT_INT_VECTOR2: {
                        out_val->v2 = va_arg( va, vec2 );
                    } break;
                    case FMT_INT_VECTOR3: {
                        out_val->v3 = va_arg( va, vec3 );
                    } break;
                    case FMT_INT_VECTOR4: {
                        out_val->v4 = va_arg( va, vec4 );
                    } break;
                    default: {
                        switch( args->integer.flags & FMT_INT_BITDEPTH_MASK ) {
#if !defined(CORE_ARCH_64_BIT)
                            case FMT_INT_BITDEPTH_PTR:
#endif
                            case FMT_INT_BITDEPTH_32: {
                                out_val->u32 = va_arg( va, u32 );
                            } break;
                            case FMT_INT_BITDEPTH_16: {
                                if( args->integer.flags & FMT_INT_SIGNED ) {
                                    out_val->i16 = va_arg( va, i32 );
                                } else {
                                    out_val->u16 = va_arg( va, u32 );
                                }
                            } break;
                            case FMT_INT_BITDEPTH_8: {
                                if( args->integer.flags & FMT_INT_SIGNED ) {
                                    out_val->i8 = va_arg( va, i32 );
                                } else {
                                    out_val->u8 = va_arg( va, u32 );
                                }
                            } break;
#if defined(CORE_ARCH_64_BIT)
                            case FMT_INT_BITDEPTH_PTR:
#endif
                            case FMT_INT_BITDEPTH_64: {
                                out_val->u64 = va_arg( va, u64 );
                            } break;
                        }
                    } break;
                }
            } break;
            case FT_TIME: {
                if( pointer ) {
                    args->time.fmt     = va_arg( va, char* );
                    args->time.fmt_len = cstr_len( args->time.fmt );
                }
                out_val->ts = va_arg( va, TimeSplit );
            } break;
        }
    }

    return true;

    #undef skip
}

