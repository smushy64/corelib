/**
 * Description:  Time implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 13, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/time.h"

#include "core/internal/platform.h"

StreamBytesFN* global_debug_timer_block_stream        = NULL;
void*          global_debug_timer_block_stream_params = NULL;

attr_core_api void time_query_timestamp( TimeStamp* out_timestamp ) {
    platform_query_timestamp( out_timestamp );
}

attr_core_api f64 time_high_resolution_seconds(void) {
    return platform_query_milliseconds() * 1000.0;
}
attr_core_api f64 time_high_resolution_milliseconds(void) {
    return platform_query_milliseconds();
}

attr_core_api void ___internal_get_debug_timer_block_stream(
    StreamBytesFN** out_stream, void** out_params
) {
    *out_stream = global_debug_timer_block_stream;
    *out_params = global_debug_timer_block_stream_params;
}
attr_core_api void ___internal_debug_timer_block_set_stream(
    StreamBytesFN* stream, void* target
) {
    global_debug_timer_block_stream        = stream;
    global_debug_timer_block_stream_params = target;
}

