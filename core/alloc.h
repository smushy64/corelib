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

/// @brief Stack allocator.
typedef struct AllocStack {
    /// @brief Total size of allocator memory. (not including AllocStack size)
    usize size;
    /// @brief Current position of stack.
    usize current;
    /// @brief Start of allocator buffer.
    u8    buffer[];
} AllocStack;

/// @brief Query stack allocator memory requirement for given size.
/// @param size Desired size of stack allocator.
/// @return Memory requirement for stack allocator.
attr_always_inline
attr_header usize alloc_stack_query_memory_requirement( usize size ) {
    return size + sizeof(AllocStack);
}
/// @brief Push new item to top of stack allocator.
/// @param[in] stack Stack allocator.
/// @param size Size of item to push.
/// @return Pointer to item in allocator.
/// NULL if stack is not large enough to accept item.
attr_core_api void* alloc_stack_push( AllocStack* stack, usize size );
/// @brief Push new item to top of stack allocator.
/// @param[in] stack Stack allocator.
/// @param size Size of item to push.
/// @param[out] out_offset Pointer to write relative offset of item to.
/// @return True if stack has enough space for item size.
attr_core_api b32 alloc_stack_push_offset(
    AllocStack* stack, usize size, usize* out_offset );
/// @brief Push new item to top of stack allocator.
/// @details
/// Uses atomic operations to calculate item pointer.
/// @param[in] stack Stack allocator.
/// @param size Size of item to push.
/// @return Pointer to item in allocator.
/// NULL if stack is not large enough to accept item.
attr_core_api void* alloc_stack_push_atomic( AllocStack* stack, usize size );
/// @brief Push new item to top of stack allocator.
/// @details
/// Uses atomic operations to calculate item offset.
/// @param[in] stack Stack allocator.
/// @param size Size of item to push.
/// @param[out] out_offset Pointer to write relative offset of item to.
/// @return True if stack has enough space for item size.
attr_core_api b32 alloc_stack_push_offset_atomic(
    AllocStack* stack, usize size, usize* out_offset );
/// @brief Push new item to top of stack allocator.
/// @param[in] stack Stack allocator.
/// @param size Size of item to push.
/// @param alignment Desired alignment of item.
/// @return Pointer to item in allocator.
/// NULL if stack is not large enough to accept item + alignment.
/// @note @c alignment MUST be a power of 2.
attr_core_api void* alloc_stack_push_aligned(
    AllocStack* stack, usize size, usize alignment );
/// @brief Push new item to top of stack allocator.
/// @param[in] stack Stack allocator.
/// @param size Size of item to push.
/// @param alignment Desired alingment of item.
/// @param[out] out_offset Pointer to write relative offset of item to.
/// @return True if stack has enough space for item size + alignment.
/// @note @c alignment MUST be a power of 2.
attr_core_api b32 alloc_stack_push_aligned_offset(
    AllocStack* stack, usize size, usize alignment, usize* out_offset );
/// @brief Push new item to top of stack allocator.
/// @details
/// Uses atomic operations to calculate item pointer.
/// @param[in] stack Stack allocator.
/// @param size Size of item to push.
/// @param alignment Desired alignment of item.
/// @return Pointer to item in allocator.
/// NULL if stack is not large enough to accept item + alignment.
/// @note @c alignment MUST be a power of 2.
attr_core_api void* alloc_stack_push_aligned_atomic(
    AllocStack* stack, usize size, usize alignment );
/// @brief Push new item to top of stack allocator.
/// @details
/// Uses atomic operations to calculate item pointer.
/// @param[in] stack Stack allocator.
/// @param size Size of item to push.
/// @param alignment Desired alingment of item.
/// @param[out] out_offset Pointer to write relative offset of item to.
/// @return True if stack has enough space for item size + alignment.
/// @note @c alignment MUST be a power of 2.
attr_core_api b32 alloc_stack_push_aligned_offset_atomic(
    AllocStack* stack, usize size, usize alignment, usize* out_offset );
/// @brief Convert stack offset to pointer.
/// @details
/// Use relative offsets instead of pointers for stack allocators
/// that need to be reallocated based on use.
/// @param[in] stack Stack allocator.
/// @param offset Offset of item in stack.
/// @return Pointer to item or NULL if it's out of bounds.
attr_always_inline
attr_header void* alloc_stack_pointer_from_offset(
    AllocStack* stack, usize offset
) {
    if( offset >= stack->current ) {
        return NULL;
    }
    return stack->buffer + offset;
}
/// @brief Pop item from the top of the stack.
/// @param[in] stack Stack allocator.
/// @param size Size of item to pop.
attr_core_api void  alloc_stack_pop( AllocStack* stack, usize size );
/// @brief Pop item from the top of the stack.
/// @details
/// Uses atomic operations to pop item.
/// @param[in] stack Stack allocator.
/// @param size Size of item to pop.
attr_core_api void  alloc_stack_pop_atomic( AllocStack* stack, usize size );
/// @brief Pop aligned item from the top of the stack.
/// @param[in] stack Stack allocator.
/// @param size Size of item to pop.
/// @param alignment Alignment of item in stack.
attr_core_api void  alloc_stack_pop_aligned(
    AllocStack* stack, usize size, usize alignment );
/// @brief Pop aligned item from the top of the stack.
/// @details
/// Uses atomic operations to pop item.
/// @param[in] stack Stack allocator.
/// @param size Size of item to pop.
/// @param alignment Alignment of item in stack.
attr_core_api void alloc_stack_pop_aligned_atomic(
    AllocStack* stack, usize size, usize alignment );
/// @brief Reset stack pointer to start of allocator.
/// @param[in] stack Stack allocator.
attr_always_inline
attr_header void alloc_stack_clear( AllocStack* stack ) {
    stack->current = 0;
}
/// @brief Reset stack pointer and clear data from allocator buffer.
/// @param[in] stack Stack allocator.
attr_core_api void alloc_stack_zero( AllocStack* stack );
/// @brief Query remaining amount of bytes in stack allocator.
/// @param[in] stack Stack allocator to query.
/// @return Remaining bytes in stack.
attr_always_inline
attr_header usize alloc_stack_query_remaining_size( AllocStack* stack ) {
    return stack->size - stack->current;
}
/// @brief After reallocating stack buffer, rebase pointer and set new size.
/// @param new_size New size of stack allocator.
/// @param ptr New pointer to stack allocator.
/// @return Stack allocator with size updated and base pointer rebased.
attr_always_inline
attr_header AllocStack* alloc_stack_rebase( usize new_size, void* ptr ) {
    AllocStack* res = ptr;
    res->size       = new_size;
    return res;
}

#endif /* header guard */
