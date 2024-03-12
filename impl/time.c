/**
 * Description:  Time implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 13, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/time.h"

#include "core/internal/platform.h"
#include "core/sync.h"
#include "core/macros.h"

attr_core_api void time_query_timestamp( TimeStamp* out_timestamp ) {
    platform_query_timestamp( out_timestamp );
}
attr_core_api CoreTicks time_query_ticks(void) {
    return platform_query_ticks();
}
attr_core_api CoreTicks time_ticks_diff( CoreTicks lhs, CoreTicks rhs ) {
    return platform_ticks_diff( lhs, rhs );
}
attr_core_api f64 time_ticks_to_seconds( CoreTicks ticks ) {
    return platform_ticks_to_seconds( ticks );
}

attr_global CoreTicks global_start_ticks;
attr_global CoreTicks global_ticks;
attr_global u64 global_update_count        = 0;
attr_global f64 global_delta_sec           = 1.0 / 60.0;
attr_global volatile f64 global_time_scale = 1.0;

attr_core_api void time_global_timekeeping_initialize(void) {
    global_start_ticks = time_query_ticks();
}
attr_core_api void time_global_timekeeping_update(void) {
    f64 prev_elapsed = time_global_elapsed_sec();
    global_ticks     = time_query_ticks();
    f64 curr_elapsed = time_global_elapsed_sec();

    global_delta_sec = curr_elapsed - prev_elapsed;
    global_update_count++;
}
attr_core_api f64 time_global_elapsed_sec(void) {
    return time_ticks_to_seconds( time_ticks_diff( global_ticks, global_start_ticks ) );
}
attr_core_api u64  time_global_query_update_count(void) {
    return global_update_count;
}
attr_core_api f64  time_global_delta_sec(void) {
    return global_delta_sec * global_time_scale;
}
attr_core_api f64  time_global_unscaled_delta_sec(void) {
    return global_delta_sec;
}
attr_core_api void time_global_set_time_scale( f64 time_scale ) {
    // TODO(alicia): fix these shennanigans
#if defined(CORE_COMPILER_MSVC)
    _InterlockedExchange64( (volatile __int64*)&global_time_scale, rcast( __int64, &time_scale ) );
#else
    interlocked_exchange( (volatile u64*)&global_time_scale, rcast( u64, &time_scale ) );
#endif
}
attr_core_api f64  time_global_query_time_scale(void) {
    return global_time_scale;
}

