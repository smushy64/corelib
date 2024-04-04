/**
 * @file   alloc.c
 * @brief  Memory allocator implementation.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   April 04, 2024
*/
#include "core/prelude.h"
#include "core/alloc.h"
#include "core/memory.h"
#include "core/sync.h"

attr_internal usize internal_alloc_aligned_size( usize size, usize alignment ) {
    return size + alignment + 16;
}

attr_core_api void* alloc_stack_push( AllocStack* stack, usize size ) {
    if( (stack->current + size) >= stack->size || !size ) {
        return NULL;
    }
    void* pointer   = stack->buffer + stack->current;
    stack->current += size;
    return pointer;
}
attr_core_api b32 alloc_stack_push_offset(
    AllocStack* stack, usize size, usize* out_offset
) {
    if( (stack->current + size) >= stack->size || !size ) {
        return false;
    }

    *out_offset     = stack->current;
    stack->current += size;
    return true;
}
attr_core_api void* alloc_stack_push_atomic( AllocStack* stack, usize size ) {
    if( (stack->current + size) >= stack->size || !size ) {
        return NULL;
    }
    usize offset = interlocked_add( &stack->current, size );
    return stack->buffer + offset;
}
attr_core_api b32 alloc_stack_push_offset_atomic(
    AllocStack* stack, usize size, usize* out_offset
) {
    if( (stack->current + size) >= stack->size || !size ) {
        return false;
    }
    *out_offset = interlocked_add( &stack->current, size );
    return true;
}
attr_core_api void* alloc_stack_push_aligned(
    AllocStack* stack, usize size, usize alignment
) {
    if( !size ) {
        return NULL;
    }
    usize aligned_size = internal_alloc_aligned_size( size, alignment );
    void* ptr = alloc_stack_push( stack, aligned_size );
    if( !ptr ) {
        return NULL;
    }
    return memory_align( ptr, alignment );
}
attr_core_api b32 alloc_stack_push_aligned_offset(
    AllocStack* stack, usize size, usize alignment, usize* out_offset
) {
    if( !size ) {
        return false;
    }
    usize aligned_size = internal_alloc_aligned_size( size, alignment );
    usize offset = 0;
    if( !alloc_stack_push_offset( stack, aligned_size, &offset ) ) {
        return false;
    }
    *out_offset = (usize)memory_align( offset, alignment );
    return true;
}
attr_core_api void* alloc_stack_push_aligned_atomic(
    AllocStack* stack, usize size, usize alignment
) {
    if( !size ) {
        return false;
    }
    usize aligned_size = internal_alloc_aligned_size( size, alignment );
    void* ptr = alloc_stack_push_atomic( stack, aligned_size );
    if( !ptr ) {
        return NULL;
    }
    return memory_align( ptr, alignment );
}
attr_core_api b32 alloc_stack_push_aligned_offset_atomic(
    AllocStack* stack, usize size, usize alignment, usize* out_offset
) {
    if( !size ) {
        return false;
    }
    usize aligned_size = internal_alloc_aligned_size( size, alignment );
    usize offset = 0;
    if( !alloc_stack_push_offset_atomic( stack, aligned_size, &offset ) ) {
        return false;
    }
    *out_offset = (usize)memory_align( offset, alignment );
    return true;
}
attr_core_api void  alloc_stack_pop( AllocStack* stack, usize size ) {
    if( size > stack->current ) {
        return;
    }
    stack->current -= size;
}
attr_core_api void  alloc_stack_pop_atomic( AllocStack* stack, usize size ) {
    if( size > stack->current ) {
        return;
    }
    interlocked_sub( &stack->current, size );
}
attr_core_api void  alloc_stack_pop_aligned(
    AllocStack* stack, usize size, usize alignment
) {
    usize aligned_size = internal_alloc_aligned_size( size, alignment );
    alloc_stack_pop( stack, aligned_size );
}
attr_core_api void alloc_stack_pop_aligned_atomic(
    AllocStack* stack, usize size, usize alignment
) {
    usize aligned_size = internal_alloc_aligned_size( size, alignment );
    alloc_stack_pop_atomic( stack, aligned_size );
}

