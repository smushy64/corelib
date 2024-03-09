/**
 * Description:  Multi-Threading Function Implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 16, 2024
*/
#include "core/prelude.h"
#include "core/thread.h"

#include "core/internal/platform.h"

attr_core_api void thread_sleep( u32 ms ) {
    platform_sleep( ms );
}
attr_core_api Thread* thread_create(
    ThreadMainFN* thread_main, void* thread_main_params,
    usize thread_stack_size
) {
    Thread* handle = NULL;
    if( !platform_thread_create(
        thread_main, thread_main_params, thread_stack_size, &handle
    ) ) {
        return NULL;
    }
    return handle;
}
attr_core_api void thread_destroy( Thread* thread ) {
    platform_thread_destroy( thread );
}
attr_core_api b32 thread_exit_code( Thread* thread, int* out_exit_code ) {
    return platform_thread_exit_code( thread, out_exit_code );
}

