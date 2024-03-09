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

/// @brief Function prototype for thread main function.
/// @param thread_id ID of the current thread. (0 reserved for main thread)
/// @param[in] params Parameters of thread main.
/// @return Thread return code.
typedef int ThreadMainFN( u32 thread_id, void* params );

/// @brief Opaque handle to thread.
typedef void Thread;

#if !defined(CORE_DEFAULT_THREAD_STACK_SIZE)
    /// @brief Default value for thread stack size.
    #define CORE_DEFAULT_THREAD_STACK_SIZE (mebibytes(1))
#endif

/// @brief Sleep current thread for given milliseconds.
/// @param ms Milliseconds to sleep for.
attr_core_api void thread_sleep( u32 ms );

/// @brief Create a new thread.
/// @param thread_main Main function for thread.
/// @param[in] thread_main_params Pointer to thread main parameters.
/// @param thread_stack_size Size of stack for created thread.
/// @see #CORE_DEFAULT_THREAD_STACK_SIZE for default value for stack_size.
/// @return Opaque handle to thread. NULL if thread creation failed.
attr_core_api Thread* thread_create(
    ThreadMainFN* thread_main, void* thread_main_params,
    usize thread_stack_size );
/// @brief Forcibly stop a thread.
/// @param[in] thread Thread to destroy.
/// @warning Only use this if program needs to be forcibly shutdown.
attr_core_api void thread_destroy( Thread* thread );
/// @brief Check if thread has terminated and if it has, retrieve exit code.
/// @param[in] thread Thread handle.
/// @param[out] out_exit_code Pointer to write exit code to.
/// Only written to if function returns true.
/// @return True if thread has exited, false if it's still running.
attr_core_api b32 thread_exit_code( Thread* thread, int* out_exit_code );

#endif /* header guard */
