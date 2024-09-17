/**
 * Description:  Multi-Threading Function Implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 16, 2024
*/
#include "core/prelude.h"
#include "core/thread.h"
#include "core/memory.h"

#include "core/internal/platform.h"

attr_core_api void thread_sleep( u32 ms ) {
    platform_sleep( ms );
}
attr_core_api b32 thread_create(
    ThreadMainFN* main, void* params,
    usize stack_size, ThreadHandle* out_handle
) {
    return platform_thread_create( main, params, stack_size, out_handle );
}
attr_core_api void thread_destroy( ThreadHandle* handle ) {
    platform_thread_destroy( handle );
    memory_zero( handle, sizeof(*handle) );
}
attr_core_api u32 thread_query_id(void) {
    return platform_thread_query_id();
}
attr_core_api void thread_free( ThreadHandle* handle ) {
    platform_thread_free( handle );
    memory_zero( handle, sizeof(*handle) );
}
attr_core_api b32 thread_join_timed(
    ThreadHandle* handle, u32 ms, int* opt_out_exit_code
) {
    return platform_thread_join_timed( handle, ms, opt_out_exit_code );
}
attr_core_api b32 thread_exit_code( ThreadHandle* handle, int* out_exit_code ) {
    return platform_thread_exit_code( handle, out_exit_code );
}

