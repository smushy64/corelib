#if !defined(CORE_SYNC_H)
#define CORE_SYNC_H
/**
 * @file   sync.h
 * @brief  Multi-Threading Sync Functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 16, 2024
*/
#include "core/types.h"
#include "core/attributes.h"

#if defined(CORE_COMPILER_MSVC)
    #pragma intrinsic(_InterlockedExchangeAdd)
#endif

#if defined(CORE_PLATFORM_WINDOWS)
    #define ___CORE_INTERNAL_SEMAPHORE_SIZE ( sizeof(void*) )
    #define ___CORE_INTERNAL_MUTEX_SIZE     ( sizeof(void*) )
#elif defined(CORE_PLATFORM_LINUX)
    #define ___CORE_INTERNAL_SEMAPHORE_SIZE (sizeof(void*))
    #define ___CORE_INTERNAL_MUTEX_SIZE     (40) // sizeof pthread_mutex_t
#else
    #error "Semaphore and Mutex sizes not defined on current platform!"
#endif

/// @brief Opaque %Semaphore Handle.
typedef struct Semaphore {
#if !defined(CORE_DOXYGEN)
    u8 handle[___CORE_INTERNAL_SEMAPHORE_SIZE];
#endif
} Semaphore;
/// @brief Opaque %Mutex Handle.
typedef struct Mutex {
#if !defined(CORE_DOXYGEN)
    u8 handle[___CORE_INTERNAL_MUTEX_SIZE];
#endif
} Mutex;

/// @brief Create a named semaphore.
/// @param[in] opt_name (optional) Name of semaphore.
/// @param[out] out_sem Pointer to receive semaphore.
/// @return True if semaphore created successfully.
attr_core_api b32 semaphore_create_named( const char* opt_name, Semaphore* out_sem );
/// @brief Create a semaphore.
/// @param[out] out_sem Pointer to receive semaphore.
/// @return True if semaphore created successfully.
attr_always_inline
attr_header b32 semaphore_create( Semaphore* out_sem ) {
    return semaphore_create_named( nullptr, out_sem );
}
/// @brief Destroy a semaphore.
///
/// Semaphore can be invalid (nullptr or semaphore wasn't actually created).
/// @param[in] sem Semaphore to destroy.
attr_core_api void semaphore_destroy( Semaphore* sem );
/// @brief Signal a semaphore.
/// @param[in] sem Semaphore to signal.
attr_core_api void semaphore_signal( Semaphore* sem );
/// @brief Wait for semaphore to be signaled for given milliseconds.
/// @note An ms value of #U32_MAX means the function will not return until semaphore is signaled.
/// @param[in] sem Semaphore to wait for.
/// @param ms Milliseconds to wait for, must be less than #U32_MAX.
/// @return True if signaled in time, otherwise false.
attr_core_api b32 semaphore_wait_timed( Semaphore* sem, u32 ms );
/// @brief Wait for semaphore to be signaled indefinitely.
/// @param[in] sem Semaphore to wait for.
attr_always_inline
attr_header void semaphore_wait( Semaphore* sem ) {
    (void)semaphore_wait_timed( sem, 0xFFFFFFFF );
}

/// @brief Create a named mutex.
/// @param[in] opt_name (optional) Mutex name.
/// @param[out] out_mutex Pointer to receive mutex.
/// @return True if mutex was created successfully.
attr_core_api b32 mutex_create_named( const char* opt_name, Mutex* out_mutex );
/// @brief Create a mutex.
/// @param[out] out_mutex Pointer to receive mutex.
/// @return True if mutex was created successfully.
attr_always_inline
attr_header b32 mutex_create( Mutex* out_mutex ) {
    return mutex_create_named( nullptr, out_mutex );
}
/// @brief Destroy a mutex.
/// @param[in] mutex Mutex to destroy.
attr_core_api void mutex_destroy( Mutex* mutex );
/// @brief Attempt to lock a mutex for given milliseconds.
/// @note An ms value of #U32_MAX means the function will not return until mutex is available.
/// @param[in] mutex Mutex to lock.
/// @param ms Milliseconds to wait for lock, must be less than #U32_MAX.
/// @return True if mutex locked within ms, otherwise false.
attr_core_api b32 mutex_lock_timed( Mutex* mutex, u32 ms );
/// @brief Attempt to lock a mutex indefinitely.
/// @param[in] mutex Mutex to lock.
attr_always_inline
attr_header void mutex_lock( Mutex* mutex ) {
    (void)mutex_lock_timed( mutex, 0xFFFFFFFF );
}
/// @brief Unlock a mutex.
/// @param[in] mutex Mutex to unlock.
attr_core_api void mutex_unlock( Mutex* mutex );

#if defined(CORE_COMPILER_MSVC)
    /// @brief Multi-Threading safe add.
    /// @param[in] addend Pointer to addend
    /// @param value Value to add to addend
    /// @return Previous value of addend.
    #define interlocked_add( addend, value )\
        _InterlockedExchangeAdd( (addend), (value) )
    /// @brief Multi-Threading safe subtract.
    /// @param[in] addend Pointer to addend
    /// @param value Value to add to addend
    /// @return Previous value of addend.
    #define interlocked_sub( addend, value )\
        _InterlockedExchangeAdd( (addend), -(value) )
    /// @brief Multi-Threading safe increment.
    /// @param[in] addend Pointer to addend
    /// @return Previous value of addend.
    #define interlocked_increment( addend )\
        interlocked_add( addend, 1 )
    /// @brief Multi-Threading safe decrement.
    /// @param[in] addend Pointer to addend
    /// @return Previous value of addend.
    #define interlocked_decrement( addend )\
        interlocked_sub( addend, 1 )
    /// @brief Multi-Threading safe exchange.
    /// @param[in] target Pointer to exchange.
    /// @param value Value to exchange with.
    /// @return initial value of target.
    #define interlocked_exchange( target, value )\
        _InterlockedExchange( (target), (value) )
    /// @brief Multi-Threading safe compare and exchange.
    /// @param[in] target Pointer to compare.
    /// @param exchange Value to exchange with.
    /// @param comperand Value to compare with.
    /// @return initial value of target.
    #define interlocked_compare_exchange( target, exchange, comperand )\
        _InterlockedCompareExchange( (target), (exchange), (comperand) )
#else
    /// @brief Multi-Threading safe add.
    /// @param[in] addend Pointer to addend
    /// @param value Value to add to addend
    /// @return Previous value of addend.
    #define interlocked_add( addend, value )\
        __sync_fetch_and_add( ( addend ), ( value ) )
    /// @brief Multi-Threading safe subtract.
    /// @param[in] addend Pointer to addend
    /// @param value Value to add to addend
    /// @return Previous value of addend.
    #define interlocked_sub( addend, value )\
        __sync_fetch_and_sub( ( addend ), ( value ) )
    /// @brief Multi-Threading safe increment.
    /// @param[in] addend Pointer to addend
    /// @return Previous value of addend.
    #define interlocked_increment( addend )\
        __sync_fetch_and_add( ( addend ), 1 )
    /// @brief Multi-Threading safe decrement.
    /// @param[in] addend Pointer to addend
    /// @return Previous value of addend.
    #define interlocked_decrement( addend )\
        __sync_fetch_and_sub( ( addend ), 1 )
    /// @brief Multi-Threading safe exchange.
    /// @param[in] target Pointer to exchange.
    /// @param value Value to exchange with.
    /// @return initial value of target.
    #define interlocked_exchange( target, value )\
        __sync_val_compare_and_swap( (target), *(target), (value) )
    /// @brief Multi-Threading safe compare and exchange.
    /// @param[in] target Pointer to compare.
    /// @param exchange Value to exchange with.
    /// @param comperand Value to compare with.
    /// @return initial value of target.
    #define interlocked_compare_exchange( target, exchange, comperand )\
        __sync_val_compare_and_swap( (target), (comperand), (exchange) )
#endif

#if defined(CORE_DOXYGEN)
    /// @brief Complete all reads and writes before fence.
    #define read_write_fence()
    /// @brief Complete all reads before fence.
    #define read_fence()
    /// @brief Complete all writes before fence.
    #define write_fence()
#endif

#if defined(CORE_COMPILER_MSVC)
    /// @brief Complete all reads and writes before fence.
    #define read_write_fence()\
        _ReadWriteBarrier()
    /// @brief Complete all reads before fence.
    #define read_fence()\
        _ReadBarrier()
    /// @brief Complete all writes before fence.
    #define write_fence()\
        _WriteBarrier()
#else
    #if defined(CORE_ARCH_X86)
        /// @brief Complete all reads and writes before fence.
        #define read_write_fence()\
            __asm__ volatile ("mfence":::"memory")
        /// @brief Complete all reads before fence.
        #define read_fence()\
            __asm__ volatile ("lfence":::"memory")
        /// @brief Complete all writes before fence.
        #define write_fence()\
            __asm__ volatile ("sfence":::"memory")
    #elif defined(CORE_ARCH_ARM)
        /// @brief Complete all reads and writes before fence.
        #define read_write_fence()\
            __asm__ volatile ("dmb":::"memory")
        /// @brief Complete all reads before fence.
        #define read_fence()\
            __asm__ volatile ("dmb":::"memory")
        /// @brief Complete all writes before fence.
        #define write_fence()\
            __asm__ volatile ("dmb st":::"memory")
    #else
        #error "Fences not defined for current architecture!"
    #endif
#endif

#endif /* header guard */
