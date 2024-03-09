/**
 * Description:  Multi-Threading Sync Implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 16, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/sync.h"

#include "core/string.h"
#include "core/memory.h"

#include "core/internal/platform.h"

attr_global volatile u32 global_running_counter = 0;

attr_core_api b32 semaphore_create_named( const char* opt_name, Semaphore* out_sem ) {
    char tmp[64];
    memory_zero( tmp, sizeof(tmp) );
    const char* name = opt_name;

    if( !opt_name ) {
        StringBuf buf = string_buf_new( sizeof(tmp), tmp );
        string_buf_fmt(
            &buf, "s_{u}{0}", interlocked_increment( &global_running_counter ) );
        name = tmp;
    }

    return platform_semaphore_create( name, out_sem );
}
attr_core_api void semaphore_destroy( Semaphore* sem ) {
    platform_semaphore_destroy( sem );
    memory_zero( sem, sizeof(*sem) );
}
attr_core_api void semaphore_signal( Semaphore* sem ) {
    platform_semaphore_signal( sem );
}
attr_core_api b32 semaphore_wait_timed( Semaphore* sem, u32 ms ) {
    return platform_semaphore_wait( sem, ms );
}

attr_core_api b32 mutex_create_named( const char* opt_name, Mutex* out_mutex ) {
    char tmp[64];
    memory_zero( tmp, sizeof(tmp) );
    const char* name = opt_name;

    if( !opt_name ) {
        StringBuf buf = string_buf_new( sizeof(tmp), tmp );
        string_buf_fmt(
            &buf, "m_{u}{0}", interlocked_increment( &global_running_counter ) );
        name = tmp;
    }

    return platform_mutex_create( name, out_mutex );
}
attr_core_api void mutex_destroy( Mutex* mutex ) {
    platform_mutex_destroy( mutex );
    memory_zero( mutex, sizeof(*mutex) );
}
attr_core_api b32 mutex_lock_timed( Mutex* mutex, u32 ms ) {
    return platform_mutex_lock( mutex, ms );
}
attr_core_api void mutex_unlock( Mutex* mutex ) {
    platform_mutex_unlock( mutex );
}


