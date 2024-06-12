/**
 * Description:  Multi-Threading Sync Implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 16, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/sync.h"

#include "core/string.h"
#include "core/memory.h" // IWYU pragma: keep

#include "core/internal/platform.h"

atomic32 global_obj_num = 0;

attr_core_api b32 semaphore_create( Semaphore* out_sem ) {
    string_buf_create_from_stack( name, 128 );
    string_buf_try_fmt( &name, "sem{u}{0}", atomic_increment32( &global_obj_num ) );

    return platform_semaphore_create( name.cc, out_sem );
}
attr_core_api void semaphore_signal( Semaphore* sem ) {
    platform_semaphore_signal( sem );
}
attr_core_api b32 semaphore_wait_timed( Semaphore* sem, u32 ms ) {
    return platform_semaphore_wait( sem, ms );
}
attr_core_api void semaphore_destroy( Semaphore* sem ) {
    platform_semaphore_destroy( sem );
}

attr_core_api b32 mutex_create( Mutex* out_mut ) {
    string_buf_create_from_stack( name, 128 );
    string_buf_try_fmt( &name, "mut{u}{0}", atomic_increment32( &global_obj_num ) );

    return platform_mutex_create( name.cc, out_mut );
}
attr_core_api b32 mutex_lock_timed( Mutex* mut, u32 ms ) {
    return platform_mutex_lock( mut, ms );
}
attr_core_api void mutex_unlock( Mutex* mut ) {
    platform_mutex_unlock( mut );
}
attr_core_api void mutex_destroy( Mutex* mut ) {
    platform_mutex_destroy( mut );
}

