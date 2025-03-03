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

/// @brief Wait for sync object indefinitely.
#define CORE_WAIT_INFINITE 0xFFFFFFFF

/// @brief Max name capacity for named sync object.
#define CORE_NAMED_SYNC_NAME_CAP 250

#if defined(CORE_PLATFORM_WINDOWS)
    #define CORE_PLATFORM_NAMED_SEMAPHORE_SIZE (sizeof(void*))
    #define CORE_PLATFORM_OS_MUTEX_SIZE        (sizeof(void*))
#elif defined(CORE_PLATFORM_POSIX)
    #define CORE_PLATFORM_NAMED_SEMAPHORE_SIZE (sizeof(void*))
    #if !defined(CORE_PLATFORM_OS_MUTEX_SIZE)
        #define CORE_PLATFORM_OS_MUTEX_SIZE (40) // sizeof pthread_mutex_t
    #endif
#endif

/// @brief Unnamed Mutex. Cannot be shared across processes.
/// @details
/// This mutex is implemented in corelib using atomic operations.
/// For OS mutex that can be shared across processes,
/// use NamedMutex.
/// @see NamedMutex.
typedef struct Mutex {
    atomic32 atom;
} Mutex;

/// @brief Unnamed Semaphore. Cannot be shared across processes.
/// @details
/// This semaphore is implemented in corelib using atomic operations.
/// For OS semaphore that can be shared across processes,
/// use NamedSemaphore.
/// @see NamedSemaphore.
typedef struct Semaphore {
    atomic32 atom;
    Mutex    mtx;
} Semaphore;

/// @brief OS Mutex.
/// @details
/// This Mutex is implemented by the OS.
/// That means that it can be opened by other processes.
typedef struct OSMutex {
    u8 opaque[CORE_PLATFORM_OS_MUTEX_SIZE];
} OSMutex;

/// @brief Named Semaphore.
/// @details
/// This Semaphore is implemented by the OS.
/// That means that it can be opened by other processes.
typedef struct NamedSemaphore {
    u8 opaque[CORE_PLATFORM_NAMED_SEMAPHORE_SIZE];
} NamedSemaphore;

/// @brief Open a named semaphore.
/// @details
/// If semaphore doesn't already exist, create one.
/// Otherwise, open existing semaphore.
/// @param[in]  name          Name of semaphore to open. Cannot be longer than CORE_NAMED_SYNC_NAME_CAP.
///                           Actual path that semaphore is opened at is implementation defined.
/// @param      initial_value Value to intialize semaphore with.
/// @param[out] out_sem       Pointer to write openened named semaphore.
/// @return
///     - @c true  : Opened named semaphore successfully.
///     - @c false : Failed to open named semaphore.
attr_core_api
b32 named_semaphore_open(
    const char* name, u32 initial_value, struct NamedSemaphore* out_sem );
/// @brief Close openend named semaphore.
/// @param[in] sem Pointer to named semaphore to close.
attr_core_api
void named_semaphore_close( struct NamedSemaphore* sem );
/// @brief Signal named semaphore.
/// @param[in] sem Pointer to named semaphore to signal.
attr_core_api
void named_semaphore_signal( struct NamedSemaphore* sem );
/// @brief Wait for named semaphore signal.
/// @param[in] sem Pointer to named semaphore.
/// @param     ms  Milliseconds to wait. Use CORE_WAIT_INFINITE to wait indefinitely.
/// @return
///     - @c true  : Named semaphore was signaled before @c ms elapsed.
///     - @c false : Named semaphore timed out.
attr_core_api
b32 named_semaphore_wait_timed( struct NamedSemaphore* sem, u32 ms );
/// @brief Wait for named semaphore signal indefinitely.
/// @param[in] sem Pointer to named semaphore.
attr_header
void named_semaphore_wait( struct NamedSemaphore* sem ) {
    named_semaphore_wait_timed( sem, CORE_WAIT_INFINITE );
}

/// @brief Create an os mutex.
/// @param[out] out_mtx Pointer to write opened mutex to.
/// @return
///     - @c true  : Opened os mutex successfully.
///     - @c false : Failed to create os mutex.
attr_core_api
b32 os_mutex_create( struct OSMutex* out_mtx );
/// @brief Destroy an os mutex.
/// @param[in] mtx Pointer to os mutex to destroy.
attr_core_api
void os_mutex_destroy( struct OSMutex* mtx );
/// @brief Unlock os mutex.
/// @param[in] mtx Pointer to os mutex to unlock.
attr_core_api
void os_mutex_unlock( struct OSMutex* mtx );
/// @brief Wait for os mutex lock.
/// @param[in] mtx Pointer to os mutex to lock.
/// @param     ms  Milliseconds to wait for lock. Use CORE_WAIT_INFINITE to wait indefinitely.
/// @return
///     - @c true  : OS mutex was signaled before @c ms elapsed.
///     - @c false : OS mutex timed out.
attr_core_api
b32 os_mutex_lock_timed( struct OSMutex* mtx, u32 ms );
/// @brief Wait for os mutex lock indefinitely.
/// @param[in] mtx Pointer to os mutex to lock.
attr_header
void os_mutex_lock( struct OSMutex* mtx ) {
    os_mutex_lock_timed( mtx, CORE_WAIT_INFINITE );
}

/// @brief Initialize semaphore.
/// @param[out] sem               Pointer to write semaphore to.
/// @param      opt_initial_value (optional) Initial value of semaphore.
attr_core_api
void semaphore_init( struct Semaphore* sem, u32 opt_initial_value );
/// @brief Signal semaphore.
/// @param[in] sem Pointer to semaphore to signal.
attr_core_api
void semaphore_signal( struct Semaphore* sem );
/// @brief Wait for semaphore signal.
/// @param[in] sem Pointer to semaphore to wait for.
/// @param     ms  Milliseconds to wait for signal. Use CORE_WAIT_INFINITE to wait indefinitely. 
/// @return
///     - @c true  : Semaphore was signaled before @c ms elapsed.
///     - @c false : Semaphore timed out.
attr_core_api
b32 semaphore_wait_timed( struct Semaphore* sem, u32 ms );
/// @brief Wait for semaphore signal indefinitely.
/// @param[in] sem Pointer to semaphore to wait for.
attr_core_api
void semaphore_wait( struct Semaphore* sem );

/// @brief Initialize mutex.
/// @param[out] mtx Pointer to write mutex to.
attr_core_api
void mutex_init( struct Mutex* mtx );
/// @brief Unlock mutex.
/// @param[in] mtx Pointer to mutex to unlock.
attr_core_api
void mutex_unlock( struct Mutex* mtx );
/// @brief Wait for mutex lock.
/// @param[in] mtx Pointer to mutex to lock.
/// @param     ms  Milliseconds to wait for. Use CORE_WAIT_INFINITE to wait indefinitely.
/// @return
///     - @c true  : Obtained mutex lock before @c ms elapsed.
///     - @c false : Timed out.
attr_core_api
b32 mutex_lock_timed( struct Mutex* mtx, u32 ms );
/// @brief Wait for mutex lock indefinitely.
/// @param[in] mtx Pointer to mutex to lock.
attr_header
void mutex_lock( struct Mutex* mtx ) {
    mutex_lock_timed( mtx, CORE_WAIT_INFINITE );
}

/// @brief Spinlock the current thread until atom equals sentinel value.
/// @param[in] atom     Pointer to atom to compare sentinel to.
/// @param     sentinel Sentinel value to check for.
/// @param     ms       Milliseconds to spinlock for. Use CORE_WAIT_INFINITE to wait indefinitely.
/// @return
///     - @c true  : Atom equals sentinel value before @c ms elapsed.
///     - @c false : Timed out.
attr_core_api
b32 atomic_spinlock_timed( atomic32* atom, i32 sentinel, u32 ms );
/// @brief Spinlock the current thread until atom equals sentinel value indefinitely.
/// @param[in] atom     Pointer to atom to compare sentinel to.
/// @param     sentinel Sentinel value to check for.
attr_header
void atomic_spinlock( atomic32* atom, i32 sentinel ) {
    atomic_spinlock_timed( atom, sentinel, CORE_WAIT_INFINITE );
}
/// @brief Spinlock the current thread until atom equals sentinel value.
/// @param[in] atom     Pointer to atom to compare sentinel to.
/// @param     sentinel Sentinel value to check for.
/// @param     ms       Milliseconds to spinlock for. Use CORE_WAIT_INFINITE to wait indefinitely.
/// @return
///     - true  : Atom equals sentinel value before @c ms elapsed.
///     - false : Timed out.
attr_core_api
b32 atomic_spinlock_timed64( atomic64* atom, i64 sentinel, u32 ms );
/// @brief Spinlock the current thread until atom equals sentinel value indefinitely.
/// @param[in] atom     Pointer to atom to compare sentinel to.
/// @param     sentinel Sentinel value to check for.
attr_header
void atomic_spinlock64( atomic64* atom, i64 sentinel ) {
    atomic_spinlock_timed64( atom, sentinel, CORE_WAIT_INFINITE );
}

#if defined(CORE_COMPILER_MSVC)
    /// @brief Issue a full read write barrier.
    #define read_write_barrier() \
        _ReadWriteBarrier()
    /// @brief Issue a full read barrier.
    #define read_barrier() \
        _ReadBarrier()
    /// @brief Issue a full write barrier.
    #define write_barrier() \
        _WriteBarrier()

    /// @brief Perform an atomic add on signed integer.
    /// @param[out] atom   Atomic integer to add to.
    /// @param      addend Value to add to atom.
    /// @return Previous value of @c atom.
    #define atomic_add32( atom, addend ) \
        _InterlockedExchangeAdd( (atom), (addend) )
    /// @brief Atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to exchange.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_exchange32( atom, exch ) \
        _InterlockedExchange( (atom), (exch) )
    /// @brief Conditionally and atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to compare and exchange.
    /// @param      cmp  Value to compare atom to. If they are equal, @c atom is exchanged with @c exch.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_compare_exchange32( atom, cmp, exch ) \
        _InterlockedCompareExchange( (atom), (exch), (cmp) )
    /// @brief Perform an atomic add on signed integer.
    /// @param[out] atom   Atomic integer to add to.
    /// @param      addend Value to add to atom.
    /// @return Previous value of @c atom.
    #define atomic_add64( atom, addend ) \
        _InterlockedExchangeAdd64( (atom), (addend) )
    /// @brief Atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to exchange.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_exchange64( atom, exch ) \
        _InterlockedExchange64( (atom), (exch) )
    /// @brief Conditionally and atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to compare and exchange.
    /// @param      cmp  Value to compare atom to. If they are equal, @c atom is exchanged with @c exch.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_compare_exchange64( atom, cmp, exch ) \
        _InterlockedCompareExchange64( (atom), (exch), (cmp) )
#else
    #if defined(CORE_ARCH_X86)

        /// @brief Issue a full read write barrier.
        #define read_write_barrier() \
            __asm__ volatile ("mfence":::"memory")
        /// @brief Issue a full read barrier.
        #define read_barrier() \
            __asm__ volatile ("lfence":::"memory")
        /// @brief Issue a full write barrier.
        #define write_barrier() \
            __asm__ volatile ("sfence":::"memory")

    #elif defined(CORE_ARCH_ARM)

        /// @brief Issue a full read write barrier.
         #define read_write_barrier() \
            __asm__ volatile ("dmb":::"memory")
        /// @brief Issue a full read barrier.
        #define read_barrier() \
            __asm__ volatile ("dmb":::"memory")
        /// @brief Issue a full write barrier.
        #define write_barrier() \
            __asm__ volatile ("dmb st":::"memory")

    #else

        /// @brief Issue a full read write barrier.
        #define read_write_barrier() \
            __sync_synchronize()
        /// @brief Issue a full read barrier.
        #define read_barrier() \
            read_write_barrier()
        /// @brief Issue a full write barrier.
        #define write_barrier() \
            read_write_barrier()

    #endif

    /// @brief Perform an atomic add on signed integer.
    /// @param[out] atom   Atomic integer to add to.
    /// @param      addend Value to add to atom.
    /// @return Previous value of @c atom.
    #define atomic_add32( atom, addend ) \
        __sync_fetch_and_add( atom, addend )
    /// @brief Atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to exchange.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_exchange32( atom, exch ) \
        __sync_val_compare_and_swap( (atom), *(atom), (exch) )
    /// @brief Conditionally and atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to compare and exchange.
    /// @param      cmp  Value to compare atom to. If they are equal, @c atom is exchanged with @c exch.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_compare_exchange32( atom, cmp, exch ) \
        __sync_val_compare_and_swap( (atom), (cmp), (exch) )
    /// @brief Perform an atomic add on signed integer.
    /// @param[out] atom   Atomic integer to add to.
    /// @param      addend Value to add to atom.
    /// @return Previous value of @c atom.
    #define atomic_add64( atom, addend ) \
        __sync_fetch_and_add( atom, addend )
    /// @brief Atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to exchange.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_exchange64( atom, exch ) \
        __sync_val_compare_and_swap( (atom), *(atom), (exch) )
    /// @brief Conditionally and atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to compare and exchange.
    /// @param      cmp  Value to compare atom to. If they are equal, @c atom is exchanged with @c exch.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_compare_exchange64( atom, cmp, exch ) \
        __sync_val_compare_and_swap( (atom), (cmp), (exch) )
#endif

/// @brief Perform an atomic increment on signed integer.
/// @param[out] atom Atomic integer to add one to.
/// @return Previous value of @c atom.
#define atomic_increment32( atom ) \
    atomic_add32( atom, 1 )
/// @brief Perform an atomic decrement on signed integer.
/// @param[out] atom Atomic integer to subtract one from.
/// @return Previous value of @c atom.
#define atomic_decrement32( atom ) \
    atomic_add32( atom, -1 )

/// @brief Perform an atomic increment on signed integer.
/// @param[out] atom Atomic integer to add one to.
/// @return Previous value of @c atom.
#define atomic_increment64( atom ) \
    atomic_add64( atom, 1 )
/// @brief Perform an atomic decrement on signed integer.
/// @param[out] atom Atomic integer to subtract one from.
/// @return Previous value of @c atom.
#define atomic_decrement64( atom ) \
    atomic_add64( atom, -1 )

#if defined(CORE_ARCH_64_BIT)
    /// @brief Perform an atomic add on signed integer.
    /// @param[out] atom   Atomic integer to add to.
    /// @param      addend Value to add to atom.
    /// @return Previous value of @c atom.
    #define atomic_add_ptrsize( atom, addend ) \
        atomic_add64( atom, addend )
    /// @brief Perform an atomic increment on signed integer.
    /// @param[out] atom Atomic integer to add one to.
    /// @return Previous value of @c atom.
    #define atomic_increment_ptrsize( atom ) \
        atomic_increment64( atom )
    /// @brief Perform an atomic decrement on signed integer.
    /// @param[out] atom Atomic integer to subtract one from.
    /// @return Previous value of @c atom.
    #define atomic_decrement_ptrsize( atom ) \
        atomic_decrement64( atom )
    /// @brief Atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to exchange.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_exchange_ptrsize( atom, exch ) \
        atomic_exchange64( atom, exch )
    /// @brief Conditionally and atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to compare and exchange.
    /// @param      cmp  Value to compare atom to. If they are equal, @c atom is exchanged with @c exch.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_compare_exchange_ptrsize( atom, cmp, exch ) \
        atomic_compare_exchange64( atom, cmp, exch )
#else
    /// @brief Perform an atomic add on signed integer.
    /// @param[out] atom   Atomic integer to add to.
    /// @param      addend Value to add to atom.
    /// @return Previous value of @c atom.
    #define atomic_add_ptrsize( atom, addend ) \
        atomic_add32( atom, addend )
    /// @brief Perform an atomic increment on signed integer.
    /// @param[out] atom Atomic integer to add one to.
    /// @return Previous value of @c atom.
    #define atomic_increment_ptrsize( atom ) \
        atomic_increment32( atom )
    /// @brief Perform an atomic decrement on signed integer.
    /// @param[out] atom Atomic integer to subtract one from.
    /// @return Previous value of @c atom.
    #define atomic_decrement_ptrsize( atom ) \
        atomic_decrement32( atom )
    /// @brief Atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to exchange.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_exchange_ptrsize( atom, exch ) \
        atomic_exchange32( atom, exch )
    /// @brief Conditionally and atomically exchange value of atom with new value.
    /// @param[out] atom Atomic integer to compare and exchange.
    /// @param      cmp  Value to compare atom to. If they are equal, @c atom is exchanged with @c exch.
    /// @param      exch Value to exchange atom with.
    /// @return Previous value of @c atom.
    #define atomic_compare_exchange_ptrsize( atom, cmp, exch ) \
        atomic_compare_exchange32( atom, cmp, exch )
#endif

#endif /* header guard */
