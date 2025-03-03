#if !defined(CORE_INTERNAL_PLATFORM_THREAD_H) && defined(CORE_ENABLE_INTERNAL)
#define CORE_INTERNAL_PLATFORM_THREAD_H
/**
 * @file   thread.h
 * @brief  Platform thread functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 06, 2025
*/
#include "core/types.h"
#include "core/thread.h"

struct NamedSemaphore;
struct OSMutex;

b32 platform_semaphore_create(
    const char* name, u32 initial_value, struct NamedSemaphore* out_sem );
void platform_semaphore_destroy( struct NamedSemaphore* sem );
void platform_semaphore_signal( struct NamedSemaphore* sem );
b32 platform_semaphore_wait( struct NamedSemaphore* sem, u32 ms );

b32 platform_mutex_create( struct OSMutex* out_mutex );
void platform_mutex_destroy( struct OSMutex* mutex );
b32 platform_mutex_lock( struct OSMutex* mutex, u32 ms );
void platform_mutex_unlock( struct OSMutex* mutex );

void platform_sleep( u32 ms );

void platform_yield(void);

b32 platform_thread_create(
    ThreadMainFN* main, void* params,
    usize stack_size, ThreadHandle* out_handle );
void platform_thread_destroy( ThreadHandle* handle );
u32 platform_thread_query_id(void);
void platform_thread_free( ThreadHandle* handle );
b32 platform_thread_join_timed( ThreadHandle* handle, u32 ms, int* opt_out_exit_code );
b32 platform_thread_exit_code( ThreadHandle* handle, int* out_exit_code );

#endif /* header guard */
