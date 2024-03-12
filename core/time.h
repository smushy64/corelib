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

/// @brief Time stamp.
///
/// Useful for low-resolution time keeping.
typedef struct CoreTimeStamp {
    /// @brief Current year.
    u32 year;
    /// @brief Current month. Range 1 -> 12.
    u32 month;
    /// @brief Current day. Range 1 -> 31.
    u32 day;
    /// @brief Current hour. Range 0 -> 23.
    u32 hour;
    /// @brief Current minute. Range 0 -> 59.
    u32 minute;
    /// @brief Current second. Range 0 -> 59.
    u32 second;
} TimeStamp;

#if defined(CORE_PLATFORM_WINDOWS)
    /// @brief System ticks.
    typedef i64 CoreTicks;
#else
    /// @brief System ticks.
    typedef struct {
        u64 sec;
        u64 nsec;
    } CoreTicks;
#endif

/// @brief Query ticks from system.
///
/// Useful for high resolution time keeping.
/// @return Ticks.
attr_core_api CoreTicks time_query_ticks(void);
/// @brief Calculate difference between ticks.
/// @param lhs, rhs Ticks to get difference of.
/// @return Difference between @c lhs and @c rhs.
attr_core_api CoreTicks time_ticks_diff( CoreTicks lhs, CoreTicks rhs );
/// @brief Convert ticks to seconds.
/// @param ticks Ticks from system.
/// @return f64 Seconds.
attr_core_api f64 time_ticks_to_seconds( CoreTicks ticks );

#if defined(CORE_CPLUSPLUS)

#include "core/macros.h" // IWYU pragma: keep
#include "core/fs.h"     // IWYU pragma: keep
#include "core/fmt.h"
#include "core/print.h"

// NOTE(alicia): forward declaration

attr_core_api u64 time_query_ticks_per_second(void);
attr_core_api u64 time_query_ticks(void);

/// @brief Time a block of code.
///
/// When block ends, streams time between construction and
/// destruction to a streaming target.
///
/// @see core/stream.h
/// @see #fmt()
/// @see #time_query_ticks()
/// @see #time_query_ticks_per_second()
struct TimerBlock {
    /// @brief Name of block. (optional)
    const char* opt_block_name;
    /// @brief Streaming function for formatting message.
    StreamFormatFN* stream;
    /// @brief Target to stream to.
    void* target;
    /// @brief Ticks when #TimerBlock was created.
    CoreTicks start_ticks;

    /// @brief Constructor.
    /// @param[in] opt_block_name (optional) Name of time block. Gets streamed at construction and destruction.
    /// @param[in] stream Streaming function.
    /// @param[in] target (optional) Target to stream to.
    TimerBlock( const char* opt_block_name, StreamFormatFN* stream, void* target )
    : opt_block_name(opt_block_name), stream(stream), target(target) {
        start_ticks = time_query_ticks();
    }
    /// @brief Destructor
    ///
    /// Queries ticks and streams time between construction and destruction to #target.
    ~TimerBlock() {
        CoreTicks end_ticks = time_query_ticks();
        f64 time_sec = time_ticks_to_seconds( time_ticks_diff( end_ticks, start_ticks ) );
        f64 time_ms  = time_sec * 1000.0;

        if( opt_block_name ) {
            fmt( stream, target, str_cyan( "[TIMER] " ) "{cc}: {f64}sec ({f64}ms)\n",
                opt_block_name, time_sec, time_ms );
        } else {
            fmt( stream, target, str_cyan( "[TIMER]" ) ": {f64}sec ({f64}ms)\n",
                time_sec, time_ms );
        }
    }
};

#if defined(CORE_ENABLE_DEBUG_TIMER_BLOCK)
    /// @brief Start a debug timer for the current block.
    ///
    /// When code block ends, prints to stdout how long the block took to execute.
    /// @note Only enabled if CORE_ENABLE_DEBUG_TIMER_BLOCK is defined.
    /// @param name (optional) String literal name of timer block.
    #define debug_timer_block( name )\
        auto unique_id(_timer_block_) = TimerBlock( name, stream_console, stdout_handle() )
#else
    /// @brief Start a debug timer for the current block.
    ///
    /// When code block ends, prints to stdout how long the block took to execute.
    #define debug_timer_block( ... )
#endif

#endif /* C++ Time Block */

/// @brief Query time stamp from the system.
/// @param[out] out_timestamp Pointer to TimeStamp struct.
attr_core_api void time_query_timestamp( TimeStamp* out_timestamp );

/// @brief Convert month to string.
/// @param month Month to convert. Must be in range 1 -> 12.
/// @param[out] opt_out_len (optional) Pointer to usize to receive string length.
/// @return UTF-8 null-terminated string.
attr_always_inline
attr_header const char* time_month_to_string( u32 month, usize* opt_out_len ) {
    #define result( str ) {\
        if( opt_out_len ) {\
            *opt_out_len = sizeof(str) - 1;\
        }\
        return str;\
    }\

    switch( month ) {
        case 1:  result("January");
        case 2:  result("February");
        case 3:  result("March");
        case 4:  result("April");
        case 5:  result("May");
        case 6:  result("June");
        case 7:  result("July");
        case 8:  result("August");
        case 9:  result("September");
        case 10: result("October");
        case 11: result("November");
        case 12: result("December");
        default: result("Invalid");
    }
    #undef result
}
/// @brief Convert 24 hour to 12 hour.
/// @param hr24 24 Hour in range 0 -> 23.
/// @param[out] out_hr12 12 Hour in range 1 -> 12.
/// @return True if hour is AM, false if it's PM.
attr_always_inline
attr_header b32 time_24hr_to_12hr( u32 hr24, u32* out_hr12 ) {
    u32 hr12  = hr24 % 12;
    hr12      = hr12 ? hr12 : 12;
    *out_hr12 = hr12;

    return hr24 < 12;
}

/// @brief Initialize global time keeping.
///
/// Required to use following global time keeping functions.
attr_core_api void time_global_timekeeping_initialize(void);
/// @brief Update global time keeping.
attr_core_api void time_global_timekeeping_update(void);
/// @brief Query how many times global time keeping has been updated.
/// @return Number of times global timekeeping has been updated. (or, how many frames have been rendered).
attr_core_api u64 time_global_query_update_count(void);
/// @brief Query how many seconds have elapsed since global time keeping was initialized.
/// @return Elapsed seconds since start of global time keeping.
attr_core_api f64 time_global_elapsed_sec(void);
/// @brief Query how many seconds have elapsed since the last time
/// global time keeping was updated.
/// @note This value is multiplied with global time scale.
/// @return Delta time since last update (or last frame).
attr_core_api f64 time_global_delta_sec(void);
/// @brief Query how many seconds have elapsed since the last time
/// global time keeping was updated.
/// @note This value is NOT multiplied with global time scale.
/// @return Raw delta time since last update (or last frame).
attr_core_api f64 time_global_unscaled_delta_sec(void);
/// @brief Set global time scale.
///
/// This value is multiplied with delta time.
/// @param time_scale New time scale.
attr_core_api void time_global_set_time_scale( f64 time_scale );
/// @brief Query global time scale.
/// @return Global time scale.
attr_core_api f64 time_global_query_time_scale(void);

#if defined(CORE_ENABLE_TIME_GLOBAL_ALIAS)
    /// @brief Query time elapsed since start of the program.
    /// @return Time elapsed since start of program in seconds.
    #define elapsed_time()        time_global_elapsed_sec()
    /// @brief Query scaled time elapsed since the last frame or update.
    /// @return Time elapsed since last frame/update in seconds.
    #define delta_time()          time_global_delta_sec()
    /// @brief Query time elapsed since the last frame or update.
    /// @return Time elapsed since last frame/update in seconds.
    #define unscaled_delta_time() time_global_unscaled_delta_sec()
    /// @brief Query time scale.
    /// @return Time scale.
    #define time_scale()          time_global_query_time_scale()
    /// @brief Set time scale.
    /// @param scale New time scale.
    #define time_set_scale(scale) time_global_set_time_scale( scale )
    /// @brief Query how many updates/frames there have been since start of the program.
    /// @return Number of updates/frames since start of the program.
    #define time_frames()         time_global_query_update_count()
#endif

#endif /* header guard */
