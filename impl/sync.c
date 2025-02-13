/**
 * Description:  Multi-Threading Sync Implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 16, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/sync.h"
#include "core/thread.h"
#include "core/time.h"

#include "core/memory.h" // IWYU pragma: keep

#include "core/internal/platform/thread.h"

attr_core_api
b32 named_semaphore_open(
    const char* name, u32 initial_value, struct NamedSemaphore* out_sem
) {
    return platform_semaphore_create( name, initial_value, out_sem );
}
attr_core_api
void named_semaphore_signal( struct NamedSemaphore* sem ) {
    platform_semaphore_signal( sem );
}
attr_core_api
b32 named_semaphore_wait_timed( struct NamedSemaphore* sem, u32 ms ) {
    return platform_semaphore_wait( sem, ms );
}
attr_core_api
void named_semaphore_close( struct NamedSemaphore* sem ) {
    platform_semaphore_destroy( sem );
}

attr_core_api
b32 os_mutex_open( struct OSMutex* out_mtx ) {
    return platform_mutex_create( out_mtx );
}
attr_core_api
b32 os_mutex_lock_timed( struct OSMutex* mtx, u32 ms ) {
    return platform_mutex_lock( mtx, ms );
}
attr_core_api
void os_mutex_unlock( struct OSMutex* mtx ) {
    platform_mutex_unlock( mtx );
}
attr_core_api
void os_mutex_destroy( struct OSMutex* mtx ) {
    platform_mutex_destroy( mtx );
}

attr_core_api
void semaphore_init( struct Semaphore* sem, u32 init ) {
    sem->atom = init;
    mutex_init( &sem->mtx );
}
attr_core_api
void semaphore_signal( struct Semaphore* sem ) {
    mutex_lock( &sem->mtx );
    atomic_increment32( &sem->atom );
    mutex_unlock( &sem->mtx );
}
attr_core_api
void semaphore_wait( struct Semaphore* sem ) {
    for( ;; ) {
        mutex_lock( &sem->mtx );
        if( sem->atom > 0 ) {
            atomic_decrement32( &sem->atom );
            mutex_unlock( &sem->mtx );
            return;
        }
        mutex_unlock( &sem->mtx );
    }
}
attr_core_api
b32 semaphore_wait_timed( struct Semaphore* sem, u32 ms ) {
    if( ms == CORE_WAIT_INFINITE ) {
        semaphore_wait( sem );
        return true;
    }

    f64 start = timer_milliseconds();
    u32 rem   = ms;
    for( ;; ) {
        if( !mutex_lock_timed( &sem->mtx, rem ) ) {
            return false;
        }

        if( sem->atom > 0 ) {
            atomic_decrement32( &sem->atom );
            mutex_unlock( &sem->mtx );
            return true;
        }

        mutex_unlock( &sem->mtx );
        f64 current = timer_milliseconds();
        f64 elapsed = current - start;

        u32 elapsed_ms = (u32)elapsed;

        if( elapsed_ms > ms ) {
            return false;
        }

        rem -= elapsed_ms;
    }
}

attr_core_api
void mutex_init( struct Mutex* mtx ) {
    mtx->atom = 0;
}
attr_core_api
void mutex_unlock( struct Mutex* mtx ) {
    atomic_exchange32( &mtx->atom, 0 );
}
attr_core_api
b32 mutex_lock_timed( struct Mutex* mtx, u32 ms ) {
    if( ms == CORE_WAIT_INFINITE ) {
        atomic_spinlock( &mtx->atom, 0 );
    } else {
        if( !atomic_spinlock_timed( &mtx->atom, 0, ms ) ) {
            return false;
        }
    }

    atomic_exchange32( &mtx->atom, 1 );
    return true;
}

attr_internal
void internal_atomic_spinlock( atomic32* atom, i32 sentinel ) {
    for( ;; ) {
        atomic_compare_exchange32( atom, sentinel, sentinel );
        if( *atom == sentinel ) {
            break;
        }

        read_write_barrier();
        thread_yield();
    }
}

attr_internal
void internal_atomic_spinlock64( atomic64* atom, i64 sentinel ) {
    for( ;; ) {
        atomic_compare_exchange64( atom, sentinel, sentinel );
        if( *atom == sentinel ) {
            break;
        }

        read_write_barrier();
        thread_yield();
    }
}

attr_core_api
b32 atomic_spinlock_timed( atomic32* atom, i32 sentinel, u32 ms ) {
    if( ms == CORE_WAIT_INFINITE ) {
        internal_atomic_spinlock( atom, sentinel );
        return true;
    }

    f64 start = timer_milliseconds();
    for( ;; ) {
        f64 current = timer_milliseconds();
        f64 elapsed = current - start;
        if( elapsed >= (f64)ms ) {
            return false;
        }

        read_write_barrier();

        atomic_compare_exchange32( atom, sentinel, sentinel );
        if( *atom == sentinel ) {
            break;
        }

        read_write_barrier();
        thread_yield();
    }

    return true;
}
attr_core_api
b32 atomic_spinlock_timed64( atomic64* atom, i64 sentinel, u32 ms ) {
    if( ms == CORE_WAIT_INFINITE ) {
        internal_atomic_spinlock64( atom, sentinel );
        return true;
    }

    f64 start = timer_milliseconds();
    for( ;; ) {
        f64 current = timer_milliseconds();
        f64 elapsed = current - start;
        if( elapsed >= (f64)ms ) {
            return false;
        }

        read_write_barrier();

        atomic_compare_exchange64( atom, sentinel, sentinel );
        if( *atom == sentinel ) {
            break;
        }

        read_write_barrier();
        thread_yield();
    }

    return true;
}

