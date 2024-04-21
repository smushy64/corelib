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

/// @brief Query high resolution clock seconds.
/// @return Seconds.
attr_core_api f64 time_high_resolution_seconds(void);
/// @brief Query high resolution clock milliseconds.
/// @return Milliseconds.
attr_core_api f64 time_high_resolution_milliseconds(void);
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

#if defined(CORE_ENABLE_DEBUG_TIMER_BLOCK)
    // IWYU pragma: begin_keep
    #include "core/stream.h"
    #include "core/fmt.h"
    #include "core/fs.h"
    // IWYU pragma: end_keep

    attr_core_api void ___internal_get_debug_timer_block_stream(
        StreamBytesFN** out_stream, void** out_params );
    attr_core_api void ___internal_debug_timer_block_set_stream(
        StreamBytesFN* stream, void* target );

    /// @brief Time a block of code.
    /// @details
    /// Checks how many milliseconds it took to run block of code.
    /// Prints result to stdout or if streaming function is defined
    /// with debug_timer_block_set_stream(), outputs result to stream.
    /// @note Only enabled in builds compiled with CORE_ENABLE_DEBUG_TIMER_BLOCK defined.
    /// @param name (valid C-identifier) Name of timer block.
    /// @param code (block of code) Code to time.
    #define debug_timer_block( name, code ) do {\
        f64 ___##name##_timer_block_time_begin = time_high_resolution_milliseconds();\
        { code }\
        f64 ___##name##_timer_block_time_end   = time_high_resolution_milliseconds();\
        f64 ___##name##_timer_block_ms =\
            ___##name##_timer_block_time_end - ___##name##_timer_block_time_begin;\
        StreamBytesFN* ___##name##_timer_block_stream = NULL;\
        void*          ___##name##_timer_block_params = NULL;\
        ___internal_get_debug_timer_block_stream(\
            &___##name##_timer_block_stream, &___##name##_timer_block_params );\
        if( !___##name##_timer_block_stream ) {\
            ___##name##_timer_block_stream = stream_console;\
            ___##name##_timer_block_params = stdout_handle();\
        }\
        fmt( ___##name##_timer_block_stream,\
            ___##name##_timer_block_params,\
            "TIMER BLOCK '" #name "': {f}ms\n", ___##name##_timer_block_ms );\
    } while(0)
    /// @brief Set streaming function and target for debug timer blocks.
    /// @note Only enabled in builds compiled with CORE_ENABLE_DEBUG_TIMER_BLOCK defined.
    /// @param stream (StreamBytesFN*) Streaming function.
    /// @param target (void*) Target to stream to.
    #define debug_timer_block_set_stream( stream, target )\
        ___internal_debug_timer_block_set_stream( stream, target )

#else /* CORE_ENABLE_DEBUG_TIMER_BLOCK */
    /// @brief Time a block of code.
    /// @details
    /// Checks how many milliseconds it took to run block of code.
    /// Prints result to stdout or if streaming function is defined
    /// with debug_timer_block_set_stream(), outputs result to stream.
    /// @note Only enabled in builds compiled with CORE_ENABLE_DEBUG_TIMER_BLOCK defined.
    /// @param name (valid C-identifier) Name of timer block.
    /// @param code (block of code) Code to time.
    #define debug_timer_block( name, code ) code
    /// @brief Set streaming function and target for debug timer blocks.
    /// @note Only enabled in builds compiled with CORE_ENABLE_DEBUG_TIMER_BLOCK defined.
    /// @param stream (StreamBytesFN*) Streaming function.
    /// @param target (void*) Target to stream to.
    #define debug_timer_block_set_stream(...) unused( __VA_ARGS__ )
#endif /* NOT CORE_ENABLE_DEBUG_TIMER_BLOCK */ 

#endif /* header guard */
