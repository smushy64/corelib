#if !defined(CORE_THREAD_H)
#define CORE_THREAD_H
/**
 * @file   thread.h
 * @brief  Create and manage threads.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 16, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/macros.h"

#if !defined(CORE_DEFAULT_THREAD_STACK_SIZE)
    /// @brief Default value for thread stack size.
    #define CORE_DEFAULT_THREAD_STACK_SIZE (mebibytes(1))
#endif

/// @brief Function prototype for thread main function.
/// @param     thread_id ID of the current thread. (0 reserved for main thread)
/// @param[in] params    Parameters of thread main.
/// @return Thread return code.
typedef int ThreadMainFN( u32 thread_id, void* params );

/// @brief Opaque thread handle.
typedef struct ThreadHandle {
#if defined(CORE_PLATFORM_WINDOWS)
    void* opaque;
    void* opaque2;
#else
    struct {
        void*     handle;
        atomic32* atom;
    } opaque;
#endif
} ThreadHandle;
#if defined(CORE_PLATFORM_WINDOWS)
/// @brief Initialize a null thread handle.
/// @return Thread handle.
#define thread_handle_null() struct_literal(ThreadHandle){ .opaque=NULL }
#else
/// @brief Initialize a null thread handle.
/// @return Thread handle.
#define thread_handle_null() struct_literal(ThreadHandle){\
    .opaque={\
        .handle=NULL,\
        .atom=NULL,\
    }\
}
#endif
/// @brief Sleep current thread for given milliseconds.
/// @param ms Milliseconds to sleep for.
attr_core_api void thread_sleep( u32 ms );
/// @brief Yield execution to another thread.
attr_core_api void thread_yield(void);
/// @brief Spawn a new thread.
/// @details
/// Handle obtained must be freed using thread_free() or thread_destroy().
/// @param[in]  main       Main function for thread.
/// @param[in]  params     (optional) Parameters for thread main function.
/// @param      stack_size Size of stack for thread. Use zero for default thread stack size.
/// @param[out] out_handle Pointer to write thread handle to.
/// @return
///     - @c true  : Spawned thread successfully.
///     - @c false : Failed to spawn new thread.
/// @see #CORE_DEFAULT_THREAD_STACK_SIZE
attr_core_api b32 thread_create(
    ThreadMainFN* main, void* params,
    usize stack_size, ThreadHandle* out_handle );
/// @brief Forcibly stop a thread and free handle.
/// @details
/// Stop thread execution and free thread handle.
/// @param[in] handle Handle of thread to destroy.
attr_core_api void thread_destroy( ThreadHandle* handle );
/// @brief Get id of the current thread.
/// @details Main thread returns zero.
/// @return Id of the current thread.
attr_core_api u32 thread_query_id(void);
/// @brief Free a thread handle.
/// @details
/// Thread does not stop execution when this function is called.
/// @param[in] handle Handle to free.
attr_core_api void thread_free( ThreadHandle* handle );
/// @brief Wait for thread to finish execution.
/// @param[in]  handle            Handle of thread to join with.
/// @param      ms                Milliseconds to wait for join. Use CORE_WAIT_INFINITE to wait indefinitely.
/// @param[out] opt_out_exit_code (optional) Pointer to write thread exit code to.
/// @return
///     - @c true  : Thread finished execution before specified time.
///     - @c false : Waiting for thread join timed out.
attr_core_api b32 thread_join_timed( ThreadHandle* handle, u32 ms, int* opt_out_exit_code );
/// @brief Wait for thread to finish execution for infinite time.
/// @param[in]  handle            Handle of thread to join with.
/// @param[out] opt_out_exit_code (optional) Pointer to write thread exit code to.
attr_always_inline
attr_header void thread_join( ThreadHandle* handle, int* opt_out_exit_code ) {
    // ms is the same as CORE_WAIT_INFINITE
    (void)thread_join_timed( handle, 0xFFFFFFFF, opt_out_exit_code );
}
/// @brief Get exit code of thread, if it finished execution.
/// @param[in]  handle        Handle of thread to get exit code from.
/// @param[out] out_exit_code Pointer to write exit code to.
/// @return
///     - @c true  : Thread finished execution. Exit code written to @c out_exit_code.
///     - @c false : Thread is still running.
attr_core_api b32 thread_exit_code( ThreadHandle* handle, int* out_exit_code );

#endif /* header guard */
