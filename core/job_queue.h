#if !defined(CORE_JOB_QUEUE_H)
#define CORE_JOB_QUEUE_H
/**
 * @file   job_queue.h
 * @brief  Multi-Threading Job %Queue.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 18, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/constants.h"

/// @brief Opaque handle to a job queue.
///
/// Points to the same memory as provided in #job_queue_create().
typedef void JobQueue;
/// @brief Function prototype for the main function of a job.
/// @param thread_id ID of the current thread (0 == main thread)
/// @param[in] params Pointer to parameters of job.
typedef void JobMainFN( u32 thread_id, void* params );

/// @brief Query memory requirement for a job queue with given threads and entries.
/// @param thread_count Number of threads that job queue will spawn, must >= 1.
/// @param max_entry_count Number of entries job queue will accept, must be >= thread_count and < #I32_MAX.
/// @param[out] out_memory_requirement Pointer to receive memory requirement.
/// @return True if values for thread_count and max_entry_count are valid.
attr_core_api
b32 job_queue_query_memory_requirement(
    u32 thread_count, u32 max_entry_count, usize* out_memory_requirement );

/// @brief Create a job queue.
/// @param[in,out] in_out_thread_count Number of threads desired,
/// if threads are successfully spawned but not the desired number of threads,
/// the number of threads actually spawned are written here, must be >= 1.
/// @param max_entry_count Maximum number of entries queue can hold, must be >= *in_out_thread_count and < I32_MAX.
/// @param stack_size Size of stack for threads spawned.
/// @param buffer_size Size of provided buffer, must be >= than result of #job_queue_query_memory_requirement().
/// @param[in] buffer Pointer to buffer to hold JobQueue.
/// @see #job_queue_query_memory_requirement()
/// @return Returns null on failure, otherwise it's the same pointer as the provided buffer.
attr_core_api
JobQueue* job_queue_create(
    u32* in_out_thread_count, u32 max_entry_count,
    usize stack_size, usize buffer_size, void* buffer );
/// @brief Destroy a job queue.
///
/// Signals job threads that they should exit and waits for all threads to exit.
/// After threads have exited, zeroes out queue buffer.
/// @param[in] queue Pointer to job queue.
attr_core_api
void job_queue_destroy( JobQueue* queue );
/// @brief Enqueue a new job into job queue.
/// @param[in] queue Pointer to job queue.
/// @param job Pointer to job's main function.
/// @param[in] params (optional) Optional parameters for job function.
/// @return Returns true if job queue had enough space for job, otherwise false.
attr_core_api
b32 job_queue_enqueue( JobQueue* queue, JobMainFN* job, void* params );
/// @brief Wait for all jobs in job queue to finish within given milliseconds.
/// @param[in] queue Pointer to job queue.
/// @param ms Milliseconds to wait for jobs to finish, must be less than #U32_MAX.
/// @return True if jobs finished within given milliseconds.
attr_core_api
b32 job_queue_wait_timed( JobQueue* queue, u32 ms );
/// @brief Wait for all jobs in job queue to finish.
/// @param[in] queue Pointer to job queue.
attr_always_inline attr_header
void job_queue_wait( JobQueue* queue ) {
    (void)job_queue_wait_timed( queue, U32_MAX );
}

#endif /* header guard */
