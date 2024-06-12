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

// TODO(alicia): define platform sizes in build system!
#if defined(CORE_PLATFORM_WINDOWS)
    #define CORE_PLATFORM_SEMAPHORE_SIZE (sizeof(void*))
    #define CORE_PLATFORM_MUTEX_SIZE     (sizeof(void*))
#elif defined(CORE_PLATFORM_POSIX)
    #define CORE_PLATFORM_SEMAPHORE_SIZE (sizeof(void*))
    #define CORE_PLATFORM_MUTEX_SIZE     (40) // sizeof glibc pthread_mutex_t
#endif

/// @brief Wait for sync object indefinitely.
#define CORE_WAIT_INFINITE 0xFFFFFFFF

/// @brief Semaphore.
typedef struct Semaphore {
    u8 opaque[CORE_PLATFORM_SEMAPHORE_SIZE];
} Semaphore;
/// @brief Mutex.
typedef struct Mutex {
    u8 opaque[CORE_PLATFORM_MUTEX_SIZE];
} Mutex;

/// @brief Create a semaphore.
/// @param[out] out_sem Pointer to write semaphore to.
/// @return
///     - @c true  : Created semaphore successfully.
///     - @c false : Failed to create semaphore.
attr_core_api b32  semaphore_create( Semaphore* out_sem );
/// @brief Signal a semaphore.
/// @param[in] sem Semaphore to signal.
attr_core_api void semaphore_signal( Semaphore* sem );
/// @brief Wait for semaphore for specified time.
/// @param[in] sem Semaphore to wait on.
/// @param     ms  Milliseconds to wait for. Use CORE_WAIT_INFINITE to wait indefinitely.
/// @return
///     - @c true  : Semaphore was signaled in time.
///     - @c false : Wait for semaphore timed out.
attr_core_api b32 semaphore_wait_timed( Semaphore* sem, u32 ms );
/// @brief Wait indefinitely for semaphore.
/// @param[in] sem Semaphore to wait on.
attr_always_inline
attr_header void semaphore_wait( Semaphore* sem ) {
    (void)semaphore_wait_timed( sem, CORE_WAIT_INFINITE );
}
/// @brief Destroy semaphore object.
/// @param[in] sem Semaphore to destroy.
attr_core_api void semaphore_destroy( Semaphore* sem );

/// @brief Create a mutex.
/// @param[out] out_mut Pointer to write mutex to.
/// @return
///     - @c true  : Created mutex successfully.
///     - @c false : Failed to create mutex.
attr_core_api b32 mutex_create( Mutex* out_mut );
/// @brief Wait for mutex lock for specified time.
/// @param[in] mut Mutex to wait for lock.
/// @param     ms  Milliseconds to wait for. Use CORE_WAIT_INFINITE to wait indefinitely.
/// @return
///     - @c true  : Mutex lock was obtained in time.
///     - @c false : Wait for mutex lock timed out.
attr_core_api b32 mutex_lock_timed( Mutex* mut, u32 ms );
/// @brief Wait for mutex lock indefinitely.
/// @param[in] mut Mutex to wait for lock on.
attr_always_inline
attr_header void mutex_lock( Mutex* mut ) {
    (void)mutex_lock_timed( mut, CORE_WAIT_INFINITE );
}
/// @brief Release lock on mutex.
/// @param[in] mut Mutex to unlock.
attr_core_api void mutex_unlock( Mutex* mut );
/// @brief Destroy mutex.
/// @param[in] mut Mutex to destroy.
attr_core_api void mutex_destroy( Mutex* mut );

#if defined(CORE_COMPILER_MSVC)
    /// @brief Issue a full read write barrier.
    #define read_write_barrier()\
        _ReadWriteBarrier()
    /// @brief Issue a full read barrier.
    #define read_barrier()\
        _ReadBarrier()
    /// @brief Issue a full write barrier.
    #define write_barrier()\
        _WriteBarrier()

    /// @brief Perform an atomic add on signed integer.
    /// @param[out] atom   Atomic integer to add to.
    /// @param      addend Value to add to atom.
    /// @return Previous value of @c atom.
    #define atomic_add32( atom, addend )\
        _InterlockedAdd( (atom), (addend) )
    /// @brief Atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to exchange.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_exchange32( atom, exch )\
        _InterlockedExchange( (atom), (exch) )
    /// @brief Conditionally and atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to compare and exchange.
    /// @param      cmp  Value to compare atom to. If they are equal, @c atom is exchanged with @c exch.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_compare_exchange32( atom, cmp, exch )\
        _InterlockedCompareExchange( (atom), (exch), (cmp) )
    /// @brief Perform an atomic add on signed integer.
    /// @param[out] atom   Atomic integer to add to.
    /// @param      addend Value to add to atom.
    /// @return Previous value of @c atom.
    #define atomic_add64( atom, addend )\
        _InterlockedAdd64( (atom), (addend) )
    /// @brief Atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to exchange.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_exchange64( atom, exch )\
        _InterlockedExchange64( (atom), (exch) )
    /// @brief Conditionally and atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to compare and exchange.
    /// @param      cmp  Value to compare atom to. If they are equal, @c atom is exchanged with @c exch.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_compare_exchange64( atom, cmp, exch )\
        _InterlockedCompareExchange64( (atom), (exch), (cmp) )
#else
    #if defined(CORE_ARCH_X86)
        /// @brief Issue a full read write barrier.
        #define read_write_barrier()\
            __asm__ volatile ("mfence":::"memory")
        /// @brief Issue a full read barrier.
        #define read_barrier()\
            __asm__ volatile ("lfence":::"memory")
        /// @brief Issue a full write barrier.
        #define write_barrier()\
            __asm__ volatile ("sfence":::"memory")
    #elif defined(CORE_ARCH_ARM)
        /// @brief Issue a full read write barrier.
         #define read_write_barrier()\
            __asm__ volatile ("dmb":::"memory")
        /// @brief Issue a full read barrier.
        #define read_barrier()\
            __asm__ volatile ("dmb":::"memory")
        /// @brief Issue a full write barrier.
        #define write_barrier()\
            __asm__ volatile ("dmb st":::"memory")
    #else
        /// @brief Issue a full read write barrier.
        #define read_write_barrier()
            __sync_synchronize()
        /// @brief Issue a full read barrier.
        #define read_barrier()  read_write_barrier()
        /// @brief Issue a full write barrier.
        #define write_barrier() read_write_barrier()
    #endif

    /// @brief Perform an atomic add on signed integer.
    /// @param[out] atom   Atomic integer to add to.
    /// @param      addend Value to add to atom.
    /// @return Previous value of @c atom.
    #define atomic_add32( atom, addend )\
        __sync_fetch_and_add( atom, addend )
    /// @brief Atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to exchange.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_exchange32( atom, exch )\
        __sync_val_compare_and_swap( (atom), *(atom), (exch) )
    /// @brief Conditionally and atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to compare and exchange.
    /// @param      cmp  Value to compare atom to. If they are equal, @c atom is exchanged with @c exch.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_compare_exchange32( atom, cmp, exch )\
        __sync_val_compare_and_swap( (atom), (cmp), (exch) )
    /// @brief Perform an atomic add on signed integer.
    /// @param[out] atom   Atomic integer to add to.
    /// @param      addend Value to add to atom.
    /// @return Previous value of @c atom.
    #define atomic_add64( atom, addend )\
        __sync_fetch_and_add( atom, addend )
    /// @brief Atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to exchange.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_exchange64( atom, exch )\
        __sync_val_compare_and_swap( (atom), *(atom), (exch) )
    /// @brief Conditionally and atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to compare and exchange.
    /// @param      cmp  Value to compare atom to. If they are equal, @c atom is exchanged with @c exch.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_compare_exchange64( atom, cmp, exch )\
        __sync_val_compare_and_swap( (atom), (cmp), (exch) )
#endif

/// @brief Perform an atomic increment on signed integer.
/// @param[out] atom Atomic integer to add one to.
/// @return Previous value of @c atom.
#define atomic_increment32( atom ) atomic_add32( atom, 1 )
/// @brief Perform an atomic decrement on signed integer.
/// @param[out] atom Atomic integer to subtract one from.
/// @return Previous value of @c atom.
#define atomic_decrement32( atom ) atomic_add32( atom, -1 )

/// @brief Perform an atomic increment on signed integer.
/// @param[out] atom Atomic integer to add one to.
/// @return Previous value of @c atom.
#define atomic_increment64( atom ) atomic_add64( atom, 1 )
/// @brief Perform an atomic decrement on signed integer.
/// @param[out] atom Atomic integer to subtract one from.
/// @return Previous value of @c atom.
#define atomic_decrement64( atom ) atomic_add64( atom, -1 )

#if defined(CORE_ARCH_64_BIT)
    /// @brief Perform an atomic add on signed integer.
    /// @param[out] atom   Atomic integer to add to.
    /// @param      addend Value to add to atom.
    /// @return Previous value of @c atom.
    #define atomic_add_size( atom, addend ) atomic_add64( atom, addend )
    /// @brief Perform an atomic increment on signed integer.
    /// @param[out] atom Atomic integer to add one to.
    /// @return Previous value of @c atom.
    #define atomic_increment_size( atom ) atomic_increment64( atom )
    /// @brief Perform an atomic decrement on signed integer.
    /// @param[out] atom Atomic integer to subtract one from.
    /// @return Previous value of @c atom.
    #define atomic_decrement_size( atom ) atomic_decrement64( atom )
    /// @brief Atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to exchange.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_exchange_size( atom, exch ) atomic_exchange64( atom, exch )
    /// @brief Conditionally and atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to compare and exchange.
    /// @param      cmp  Value to compare atom to. If they are equal, @c atom is exchanged with @c exch.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_compare_exchange_size( atom, cmp, exch )\
        atomic_compare_exchange64( atom, cmp, exch )
#else
    /// @brief Perform an atomic add on signed integer.
    /// @param[out] atom   Atomic integer to add to.
    /// @param      addend Value to add to atom.
    /// @return Previous value of @c atom.
    #define atomic_add_size( atom, addend ) atomic_add32( atom, addend )
    /// @brief Perform an atomic increment on signed integer.
    /// @param[out] atom Atomic integer to add one to.
    /// @return Previous value of @c atom.
    #define atomic_increment_size( atom ) atomic_increment32( atom )
    /// @brief Perform an atomic decrement on signed integer.
    /// @param[out] atom Atomic integer to subtract one from.
    /// @return Previous value of @c atom.
    #define atomic_decrement_size( atom ) atomic_decrement32( atom )
    /// @brief Atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to exchange.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_exchange_size( atom, exch ) atomic_exchange32( atom, exch )
    /// @brief Conditionally and atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to compare and exchange.
    /// @param      cmp  Value to compare atom to. If they are equal, @c atom is exchanged with @c exch.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_compare_exchange_size( atom, cmp, exch )\
        atomic_compare_exchange32( atom, cmp, exch )
#endif

#endif /* header guard */
