#if !defined(CORE_ALLOC_H)
#define CORE_ALLOC_H
/**
 * @file   alloc.h
 * @brief  Memory allocators.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   April 04, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/macros.h"
#include "core/slice.h"

/// @brief Function prototype for allocator alloc function.
/// @details
/// All allocators must returned zeroed memory.
/// @param     size      Size of allocation.
/// @param     alignment Alignment of allocation. Some allocators may not support alignment. Must be a power of 2.
/// @param[in] ctx       (optional) Allocator context.
/// @return
///     - NULL    : Failed to allocate memory from allocator.
///     - Pointer : Pointer to allocated memory.
typedef void* AllocatorAllocFN( usize size, usize alignment, void* ctx );
/// @brief Function prototype for allocator realloc function.
/// @details
/// All allocators must return zeroed memory. (Only newly allocated space)
/// @param[in] memory    Pointer to memory to reallocate.
/// @param     old_size  Size of @c memory.
/// @param     new_size  Reallocation size of @c memory. Must be >= @c old_size.
/// @param     alignment Alignment of allocation. Must match original alignment. Some allocators may not support alignment. Must be a power of 2.
/// @param[in] ctx       (optional) Allocator context.
/// @return
///     - NULL    : Failed to reallocate buffer. @c memory is still valid.
///     - Pointer : Pointer to new buffer. @c memory is no longer valid.
typedef void* AllocatorReallocFN(
    void* memory, usize old_size, usize new_size, usize alignment, void* ctx );
/// @brief Function prototype for allocator free function.
/// @param[in] memory    Pointer to memory to free.
/// @param     size      Size of @c memory.
/// @param     alignment Alignment of @c memory.
/// @param[in] ctx       (optional) Allocator context.
typedef void AllocatorFreeFN( void* memory, usize size, usize alignment, void* ctx );

/// @brief Interface for memory allocators.
typedef struct AllocatorInterface {
    /// @brief Function for allocating memory from this allocator.
    /// @details
    /// All allocators must returned zeroed memory.
    /// @param     size      Size of allocation.
    /// @param     alignment Alignment of allocation. Some allocators may not support alignment. Must be a power of 2.
    /// @param[in] ctx       (optional) Allocator context.
    /// @return
    ///     - NULL    : Failed to allocate memory from allocator.
    ///     - Pointer : Pointer to allocated memory.
    AllocatorAllocFN*   alloc;
    /// @brief Function for reallocating memory from this allocator.
    /// @details
    /// All allocators must return zeroed memory. (Only newly allocated space)
    /// @param[in] memory    Pointer to memory to reallocate.
    /// @param     old_size  Size of @c memory.
    /// @param     new_size  Reallocation size of @c memory. Must be >= @c old_size.
    /// @param     alignment Alignment of allocation. Must match original alignment. Some allocators may not support alignment. Must be a power of 2.
    /// @param[in] ctx       (optional) Allocator context.
    /// @return
    ///     - NULL    : Failed to reallocate buffer. @c memory is still valid.
    ///     - Pointer : Pointer to new buffer. @c memory is no longer valid.
    AllocatorReallocFN* realloc;
    /// @brief Function for freeing memory from this allocator.
    /// @param[in] memory    Pointer to memory to free.
    /// @param     size      Size of @c memory.
    /// @param     alignment Alignment of @c memory.
    /// @param[in] ctx       (optional) Allocator context.
    AllocatorFreeFN*    free;
    /// @brief Pointer to allocator context.
    /// @details
    /// Some allocators (such as heap allocator) don't require a context.
    void*               ctx;

    /// @brief String identifying this allocator.
    struct ByteSlice name;
} AllocatorInterface;
/// @brief Create a new allocator interface.
/// @param[in] _alloc   (AllocatorAllocFN*)   Function for allocating memory.
/// @param[in] _realloc (AllocatorReallocFN*) Function for reallocating memory.
/// @param[in] _free    (AllocatorFreeFN*)    Function for freeing memory.
/// @param[in] opt_ctx  (void*,optional)      Pointer to allocator context.
/// @return Allocator interface.
#define allocator_interface_new( _alloc, _realloc, _free, opt_ctx )\
    struct_literal( AllocatorInterface ) {\
        .alloc=_alloc, .realloc=_realloc, .free=_free, .ctx=opt_ctx }
/// @brief Create allocator interface for default heap allocator.
/// @return Allocator interface.
#define allocator_interface_from_heap()\
    allocator_interface_new( ___internal_memory_alloc_,\
        ___internal_memory_realloc_, ___internal_memory_free_, 0 )

/// @brief Stack allocator context.
typedef struct StackAllocator {
    /// @brief If allocator should use atomic operations.
    bsize is_atomic;
    /// @brief Total size of allocator memory.
    usize size;
    /// @brief Stack pointer.
    volatile usize at;
    /// @brief Pointer to start of stack buffer.
    void* start;
} StackAllocator;
/// @brief Create allocator interface from StackAllocator.
/// @param[in] stack (StackAllocator*) Pointer to stack allocator.
/// @return Allocator interface.
#define allocator_interface_from_stack( stack )\
    allocator_interface_new( stack_allocator_alloc,\
        stack_allocator_realloc, stack_allocator_free, stack )
/// @brief Create new stack allocator.
/// @param     size   Total size of stack allocator buffer.
/// @param[in] buf    Pointer to start of stack allocator buffer.
/// @param     atomic If allocator should use atomic operations when allocating.
/// @return Stack allocator.
attr_always_inline
attr_header StackAllocator stack_allocator_new( usize size, void* buf, b32 atomic ) {
    StackAllocator stack;
    stack.size      = size;
    stack.at        = 0;
    stack.start     = buf;
    stack.is_atomic = atomic;
    return stack;
}
/// @brief Push new memory to the top of stack allocator.
/// @param[in] stack Pointer to stack allocator.
/// @param     size  Size of allocation.
/// @param     alignment Desired alignment of memory. Zero means no particular alignment. Must be a power of 2.
/// @return
///     - NULL    : Stack allocator does not have space for allocation.
///     - Pointer : Pointer to start of allocation.
attr_core_api void* stack_allocator_push(
    StackAllocator* stack, usize size, usize alignment );
/// @brief Pop bytes from top of stack allocator.
/// @warning This function does not check if memory being popped should be popped.
/// @param[in] stack     Pointer to stack allocator.
/// @param     size      Size of allocation to pop.
/// @param     alignment Alignment of memory being popped. Zero means no particular alignment. Must be a power of 2.
attr_core_api void stack_allocator_pop(
    StackAllocator* stack, usize size, usize alignment );
/// @brief Clear stack. Resets stack pointer and zeroes memory.
/// @param[in] stack Pointer to stack to clear.
attr_core_api void stack_allocator_clear( StackAllocator* stack );

/// @brief Stack allocator alloc function for allocator interface.
/// @param     size           Size of allocation.
/// @param     alignment      Alignment of allocation. Must be a power of 2.
/// @param[in] StackAllocator Pointer to stack.
/// @return
///     - NULL    : Failed to allocate memory from stack.
///     - Pointer : Pointer to allocated memory.
attr_core_api void* stack_allocator_alloc(
    usize size, usize alignment, void* StackAllocator );
/// @brief Stack allocator realloc function for allocator interface.
/// @warning
/// This function probably shouldn't be used as stack allocator
/// doesn't really support reallocating.
/// @param[in] memory         Pointer to memory to reallocate.
/// @param     old_size       Size of @c memory.
/// @param     new_size       Reallocation size of @c memory. Must be >= @c old_size.
/// @param     alignment      Alignment of allocation. Must match original alignment. Must be a power of 2.
/// @param[in] StackAllocator Pointer to stack.
/// @return
///     - NULL    : Failed to reallocate buffer. @c memory is still valid.
///     - Pointer : Pointer to new buffer. @c memory is no longer valid.
attr_core_api void* stack_allocator_realloc(
    void* memory, usize old_size, usize new_size,
    usize alignment, void* StackAllocator );
/// @brief Stack allocator free function for allocator interface.
/// @warning
/// This function probably shouldn't be used as stack allocator
/// doesn't really support freeing.
/// Memory is popped from top of stack but that may not be where @c memory points to.
/// @param[in] memory         Pointer to memory to free.
/// @param     size           Size of @c memory.
/// @param     alignment      Alignment of @c memory.
/// @param[in] StackAllocator Pointer to stack.
attr_core_api void  stack_allocator_free(
    void* memory, usize size, usize alignment, void* StackAllocator );

attr_core_api void* ___internal_memory_alloc_(
    usize size, usize alignment, void* ctx );
attr_core_api void* ___internal_memory_realloc_(
    void* memory, usize old_size, usize new_size, usize alignment, void* ctx );
attr_core_api void ___internal_memory_free_(
    void* memory, usize size, usize alignment, void* ctx );

#endif /* header guard */
