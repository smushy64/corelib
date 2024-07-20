#if !defined(CORE_TIME_H)
#define CORE_TIME_H
/**
 * @file   time.h
 * @brief  Time keeping and profiling.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 13, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/stream.h"

/// @brief Same as time_t.
typedef usize TimePosix;
/// @brief Unsigned int representing a year.
typedef u32 TimeYear;
/// @brief Unsigned int representing a month (1-12).
typedef u32 TimeMonth;
/// @brief Unsigned int representing a day of the month (1-31).
typedef u32 TimeDay;
/// @brief Unsigned int representing an hour (0-23).
typedef u32 TimeHour;
/// @brief Unsigned int representing a minute (0-59).
typedef u32 TimeMinute;
/// @brief Unsigned int representing a second (0-59).
typedef u32 TimeSecond;

/// @brief Time split up into year, month, day, hour, minute and second.
typedef struct TimeSplit {
    /// @brief Year.
    TimeYear year;
    /// @brief Month. Range: 1 to 12.
    TimeMonth month;
    /// @brief Day. Range: 1 to 31.
    TimeDay day;
    /// @brief Hour. Range 0 to 23.
    TimeHour hour;
    /// @brief Minute. Range 0 to 59.
    TimeMinute minute;
    /// @brief Seconds. Range 0 to 59.
    TimeSecond second;
} TimeSplit;

/// @brief Time split month of January.
#define TIME_MONTH_JANUARY   (1)
/// @brief Time split month of February.
#define TIME_MONTH_FEBRUARY  (2)
/// @brief Time split month of March.
#define TIME_MONTH_MARCH     (3)
/// @brief Time split month of April.
#define TIME_MONTH_APRIL     (4)
/// @brief Time split month of May.
#define TIME_MONTH_MAY       (5)
/// @brief Time split month of June.
#define TIME_MONTH_JUNE      (6)
/// @brief Time split month of July.
#define TIME_MONTH_JULY      (7)
/// @brief Time split month of August.
#define TIME_MONTH_AUGUST    (8)
/// @brief Time split month of September.
#define TIME_MONTH_SEPTEMBER (9)
/// @brief Time split month of October.
#define TIME_MONTH_OCTOBER   (10)
/// @brief Time split month of November.
#define TIME_MONTH_NOVEMBER  (11)
/// @brief Time split month of December.
#define TIME_MONTH_DECEMBER  (12)

/// @brief Sunday.
#define TIME_DAY_WEEK_SUNDAY    (0)
/// @brief Monday.
#define TIME_DAY_WEEK_MONDAY    (1)
/// @brief Tuesday.
#define TIME_DAY_WEEK_TUESDAY   (2)
/// @brief Wednesday.
#define TIME_DAY_WEEK_WEDNESDAY (3)
/// @brief Thursday.
#define TIME_DAY_WEEK_THURSDAY  (4)
/// @brief Friday.
#define TIME_DAY_WEEK_FRIDAY    (5)
/// @brief Saturday.
#define TIME_DAY_WEEK_SATURDAY  (6)

/// @brief Number of seconds in a day.
#define TIME_SECONDS_IN_DAY       (86400ull)
/// @brief Number of seconds in a year.
#define TIME_SECONDS_IN_YEAR      (TIME_SECONDS_IN_DAY * 365ull)
/// @brief Number of seconds in a leap year.
#define TIME_SECONDS_IN_LEAP_YEAR (TIME_SECONDS_IN_DAY * 366ull)

/// @brief Get POSIX time.
/// @details
/// Time is always in UTC.
/// @return Seconds since 00:00:00 UTC January 1, 1970.
attr_core_api TimePosix time_posix(void);
/// @brief Get split local time.
/// @details
/// Time is always in the system's current timezone.
/// @return Split time.
/// @see #TimeSplit
attr_core_api TimeSplit time_split(void);
/// @brief Stream formatted time split.
/// @param[in] stream         Pointer to streaming function.
/// @param[in] target         Pointer to streaming target.
/// @param[in] ts             Pointer to time split.
/// @param     padding        Padding to add to formatted string.
/// @param     opt_format_len (optional) Length of time format string.
/// @param[in] opt_format     (optional) Pointer to start of format string.
/// @see FORMAT_TIME.md
/// @return Number of characters that could not be streamed to target.
attr_core_api usize stream_fmt_time(
    StreamBytesFN* stream, void* target, const struct TimeSplit* ts,
    int padding, usize opt_format_len, const char* opt_format );
/// @brief Convert 24 hour to 12 hour.
/// @param      hr24     Hour in 24-hour.
/// @param[out] out_hr12 Hour in 12-hour.
/// @return
///     - @c true  : Time is AM.
///     - @c false : Time is PM.
attr_core_api b32 time_hour_24_to_12( TimeHour hr24, TimeHour* out_hr12 );
/// @brief Convert day of the month to day of the week.
/// @param year      Year.
/// @param month     Month (1-12).
/// @param day_month Day of the month (1-31).
/// @return Day of the week (0-6 with 0=Sunday).
attr_core_api u32 time_day_month_to_day_week( u32 year, u32 month, u32 day_month );
/// @brief Convert day of month to day of year.
/// @param year      Year.
/// @param month     Month (1-12).
/// @param day_month Day of the month (1-31).
/// @return Day of the year.
attr_core_api u32 time_day_month_to_day_year( u32 year, u32 month, u32 day_month );
/// @brief Check if year is leap year.
/// @param year Year.
/// @return
///     - @c true  : @c year is a leap year.
///     - @c false : @c year is not a leap year.
attr_core_api b32 time_year_is_leap( u32 year );

/// @brief Get high resolution time in milliseconds.
/// @return Time in milliseconds.
attr_core_api f64 timer_milliseconds(void);
/// @brief Get high resolution time in seconds.
/// @return Time in seconds.
attr_core_api f64 timer_seconds(void);

#if defined(CORE_ENABLE_DEBUG_TIMER_BLOCK)
    /// @brief Create a debug timer block.
    /// @param name   (C identifier)     Name of timer block.
    /// @param code   (C code)           Block of code to execute.
    /// @param format (string literal)   String formatting for debug message.
    /// @param ...    (format arguments) Format arguments for debug message.
    #define debug_timer_block( name, code, format, ... ) do {\
        f64 debug_timer_block_start = timer_milliseconds();\
        { code };\
        f64 debug_timer_block_end = timer_milliseconds();\
        println(\
            format "{f,.2}ms",\
            debug_timer_block_end - debug_timer_block_start,\
            ##__VA_ARGS__ );\
    } while(0)
#else
    /// @brief Create a debug timer block.
    /// @param name   (C identifier)                Name of timer block.
    /// @param code   (C code)                      Block of code to execute.
    /// @param ...    (format and format arguments) Format arguments for debug message.
    #define debug_timer_block( name, code, ... )\
        { code }
#endif

/// @brief Convert month to short string.
/// @param      month       Month to convert.
/// @param[out] opt_out_len (optional) Pointer to write length of string.
/// @return Month short name read-only string.
attr_header const cstr* time_month_to_string_short(
    TimeMonth month, usize* opt_out_len
) {
    #define res( literal )\
        if( opt_out_len ) *opt_out_len = sizeof(literal) - 1; return literal

    switch( month ) {
        case TIME_MONTH_JANUARY   : res("Jan");
        case TIME_MONTH_FEBRUARY  : res("Feb");
        case TIME_MONTH_MARCH     : res("Mar");
        case TIME_MONTH_APRIL     : res("Apr");
        case TIME_MONTH_MAY       : res("May");
        case TIME_MONTH_JUNE      : res("Jun");
        case TIME_MONTH_JULY      : res("Jul");
        case TIME_MONTH_AUGUST    : res("Aug");
        case TIME_MONTH_SEPTEMBER : res("Sep");
        case TIME_MONTH_OCTOBER   : res("Oct");
        case TIME_MONTH_NOVEMBER  : res("Nov");
        case TIME_MONTH_DECEMBER  : res("Dec");
        default: res("INV");
    }

    #undef res
}
/// @brief Convert month to string.
/// @param      month       Month to convert.
/// @param[out] opt_out_len (optional) Pointer to write length of string.
/// @return Month name read-only string.
attr_header const cstr* time_month_to_string( TimeMonth month, usize* opt_out_len ) {
    #define res( literal )\
        if( opt_out_len ) *opt_out_len = sizeof(literal) - 1; return literal

    switch( month ) {
        case TIME_MONTH_JANUARY   : res("Januart");
        case TIME_MONTH_FEBRUARY  : res("February");
        case TIME_MONTH_MARCH     : res("March");
        case TIME_MONTH_APRIL     : res("April");
        case TIME_MONTH_MAY       : res("May");
        case TIME_MONTH_JUNE      : res("June");
        case TIME_MONTH_JULY      : res("July");
        case TIME_MONTH_AUGUST    : res("August");
        case TIME_MONTH_SEPTEMBER : res("September");
        case TIME_MONTH_OCTOBER   : res("October");
        case TIME_MONTH_NOVEMBER  : res("November");
        case TIME_MONTH_DECEMBER  : res("December");
        default: res("INVALID");
    }
    #undef res
}
/// @brief Convert day of the week to string.
/// @param      day_week    Day of the week to convert (0-6, 0 == Sunday).
/// @param[out] opt_out_len (optional) Pointer to write length of string.
/// @return Day name read-only string.
attr_header const char* time_day_week_to_string( u32 day_week, usize* opt_out_len ) {
    #define res( literal )\
        if( opt_out_len ) *opt_out_len = sizeof(literal) - 1; return literal
    switch( day_week ) {
        case TIME_DAY_WEEK_SUNDAY    : res("Sunday");
        case TIME_DAY_WEEK_MONDAY    : res("Monday");
        case TIME_DAY_WEEK_TUESDAY   : res("Tuesday");
        case TIME_DAY_WEEK_WEDNESDAY : res("Wednesday");
        case TIME_DAY_WEEK_THURSDAY  : res("Thursday");
        case TIME_DAY_WEEK_FRIDAY    : res("Friday");
        case TIME_DAY_WEEK_SATURDAY  : res("Saturday");
        default: res("INVALID");
    }
    #undef res
}

#endif /* header guard */
