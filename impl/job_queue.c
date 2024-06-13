/**
 * Description:  Multi-Threaded Job Queue Implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 18, 2024
*/
#include "core/prelude.h"
#include "core/job_queue.h"
#include "core/sync.h"
#include "core/thread.h"
#include "core/memory.h"
#include "core/internal/logging.h"

#define INTERNAL_JOB_QUEUE_MAX_ENTRY_COUNT (64)

struct JobEntry {
    JobMainFN* main;
    void*      params;
};
struct InternalJobQueue {
    Semaphore sem_wake_up;
    Semaphore sem_entry_complete;
    volatile i32 front;
    volatile i32 back;
    volatile u32 len;
    volatile b32 signal_end;
    volatile u32 max_entries;
    struct JobEntry entries[];
};
struct InternalThreadArray {
    usize        len;
    ThreadHandle threads[];
};

attr_always_inline inline
attr_internal usize internal_job_queue_size( u32 entry_count ) {
    return sizeof(struct InternalJobQueue) + (sizeof(struct JobEntry) * entry_count);
}
attr_always_inline inline
attr_internal usize internal_thread_array_size( u32 len ) {
    return sizeof(struct InternalThreadArray) + (sizeof(ThreadHandle*) * len);
}
attr_always_inline inline
attr_internal usize internal_memory_requirement( u32 thread_count, u32 entry_count ) {
    return
        internal_job_queue_size( entry_count ) +
        internal_thread_array_size( thread_count );
}
attr_core_api b32 job_queue_query_memory_requirement(
    u32 thread_count, u32 max_entry_count, usize* out_size
) {
    if(
        !thread_count ||
        (max_entry_count < thread_count) ||
        (max_entry_count >= I32_MAX)
    ) {
        return false;
    }
    *out_size = internal_memory_requirement( thread_count, max_entry_count );
    return true;
}

attr_internal b32 internal_job_dequeue(
    struct InternalJobQueue* q, struct JobEntry* out_entry
) {
    if( !q->len ) {
        return false;
    }
    read_write_barrier();

    i32 front = atomic_increment32( &q->front );
    atomic_decrement32( &q->len );

    read_write_barrier();
    front = (front + 1) % q->max_entries;

    *out_entry = q->entries[front];
    return true;
}

attr_unused
attr_internal int internal_job_queue_main( u32 thread_id, void* params ) {
    struct InternalJobQueue* q = rcast_ref( struct InternalJobQueue, params );

    loop() {
        struct JobEntry entry;
        memory_zero( &entry, sizeof(entry) );

        semaphore_wait( &q->sem_wake_up );
        read_write_barrier();

        if( q->signal_end ) {
            break;
        }

        if( internal_job_dequeue( q, &entry ) ) {
            entry.main( thread_id, entry.params );
            read_write_barrier();
            semaphore_signal( &q->sem_entry_complete );
        }
    }

    read_write_barrier();
    if( q->max_entries ) {
        atomic_decrement32( &q->max_entries );
    }
    return 0;
}

attr_core_api JobQueue* job_queue_create(
    u32* in_out_thread_count, u32 max_entry_count, usize stack_size,
    usize buffer_size, void* buffer
) {
    u32 thread_count = *in_out_thread_count;
    usize memory_requirement = 0;
    if( !job_queue_query_memory_requirement(
        thread_count, max_entry_count, &memory_requirement
    ) ) {
        core_error(
            "thread_count or max_entry_count are invalid! "
            "thread_count: {u32} max_entry_count: {u32}",
            thread_count, max_entry_count );
        return NULL;
    }
    if( memory_requirement > buffer_size ) {
        core_error(
            "memory provided is not enough for the requested job queue! "
            "memory provided: {usize} memory required: {usize}",
            buffer_size, memory_requirement );
        return NULL;
    }

    struct InternalJobQueue* q = rcast_ref( struct InternalJobQueue, buffer );
    q->max_entries = max_entry_count;
    q->front       = -1;
    q->back        = 0;

    if( !semaphore_create( &q->sem_wake_up ) ) {
        core_error( "job queue failed to create required semaphore!" );
        return false;
    }
    if( !semaphore_create( &q->sem_entry_complete ) ) {
        core_error( "job queue failed to create required semaphore!" );
        semaphore_destroy( &q->sem_wake_up );
        return false;
    }

    struct InternalThreadArray* array =
        (struct InternalThreadArray*)
        ((u8*)buffer + internal_job_queue_size( max_entry_count ));
    read_write_barrier();

    for( u32 i = 0; i < thread_count; ++i ) {
        if( !thread_create(
            internal_job_queue_main, q, stack_size,
            array->threads + i
        ) ) {
            break;
        }
        array->len++;
    }
    if( array->len ) {
        if( array->len != thread_count ) {
            core_warn(
                "job queue created successfully but without desired thread count! "
                "desired thread count: {u32} created thread count: {usize}",
                thread_count, array->len );
            *in_out_thread_count = array->len;
        }
    } else {
        core_error( "job queue failed to create any threads!" );
        semaphore_destroy( &q->sem_wake_up );
        semaphore_destroy( &q->sem_entry_complete );
        return false;
    }

    return buffer;
}
attr_core_api void job_queue_destroy( JobQueue* queue ) {
    struct InternalJobQueue* q = rcast_ref( struct InternalJobQueue, queue );
    job_queue_wait( queue );

    u32 max_entries = q->max_entries;
    read_write_barrier();

    atomic_increment32( &q->signal_end );
    read_write_barrier();

    while( q->max_entries ) {
        semaphore_signal( &q->sem_wake_up );
    }
    read_write_barrier();

    semaphore_destroy( &q->sem_wake_up );
    semaphore_destroy( &q->sem_entry_complete );

    read_write_barrier();
    usize job_queue_size = internal_job_queue_size( max_entries );
    struct InternalThreadArray* array =
        (struct InternalThreadArray*)((u8*)queue + job_queue_size);

    for( usize i = 0; i < array->len; ++i ) {
        thread_free( array->threads + i );
    }

    usize buffer_size = job_queue_size + internal_thread_array_size( array->len );
    memory_zero( queue, buffer_size );
}
attr_core_api b32 job_queue_enqueue( JobQueue* queue, JobMainFN* job, void* params ) {
    struct InternalJobQueue* q = rcast_ref( struct InternalJobQueue, queue );
    if( q->back == q->front ) {
        return false;
    }

    i32 back = atomic_increment32( &q->back );
    atomic_increment32( &q->len );

    read_write_barrier();
    back = back % q->max_entries;

    struct JobEntry entry;
    entry.main       = job;
    entry.params     = params;
    q->entries[back] = entry;

    read_write_barrier();
    semaphore_signal( &q->sem_wake_up );

    return true;
}
attr_core_api b32 job_queue_wait_timed( JobQueue* queue, u32 ms ) {
    struct InternalJobQueue* q = rcast_ref( struct InternalJobQueue, queue );
    if( ms == U32_MAX ) {
        while( q->len ) {
            thread_sleep(1);
        }
        return true;
    } else for( u32 i = 0; i < ms; ++i ) {
        if( !q->len ) {
            return true;
        }
        thread_sleep(1);
    }

    return false;
}

#undef INTERNAL_JOB_QUEUE_MAX_ENTRY_COUNT

