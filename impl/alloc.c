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

attr_core_api void* stack_allocator_push(
    StackAllocator* stack, usize size, usize alignment
) {
    usize _size = size;
    if( alignment ) {
        _size += alignment + sizeof(void*);
    }
    if( stack->at + _size > stack->size ) {
        return NULL;
    }
    usize offset = 0;
    if( stack->is_atomic ) {
        offset = atomic_add_ptrsize( (atomic_size*)&stack->at, _size );
    } else {
        offset = stack->at;
        stack->at += _size;
    }

    read_write_barrier();

    u8* ptr = (u8*)stack->start + offset;

    if( alignment ) {
        void* aligned = memory_align( ptr + sizeof(void*), alignment );
        ((void**)aligned)[-1] = ptr;
        return aligned;
    } else {
        return ptr;
    }
}
attr_core_api void stack_allocator_pop(
    StackAllocator* stack, usize size, usize alignment
) {
    isize _size = size;
    if( alignment ) {
        _size += alignment + sizeof(void*);
    }
    if( stack->is_atomic ) {
        atomic_add_ptrsize( (atomic_size*)&stack->at, -_size );
    } else {
        stack->at -= _size;
    }
    memory_free( (u8*)stack->start + stack->at, _size );
}
attr_core_api void stack_allocator_clear( StackAllocator* stack ) {
    usize at = 0;
    if( stack->is_atomic ) {
        at = atomic_exchange_ptrsize( (atomic_size*)&stack->at, 0 );
    } else {
        at        = stack->at;
        stack->at = 0;
    }
    memory_zero( stack->start, at );
}

attr_core_api void* stack_allocator_alloc( usize size, usize alignment, void* ctx ) {
    StackAllocator* stack = ctx;
    return stack_allocator_push( stack, size, alignment );
}
attr_core_api void* stack_allocator_realloc(
    void* memory, usize old_size, usize new_size, usize alignment, void* ctx
) {
    // TODO(alicia): make this better. it's no good
    unused(memory);
    StackAllocator* stack = ctx;
    stack_allocator_pop( stack, old_size, alignment );
    return stack_allocator_push( stack, new_size, alignment );
}
attr_core_api void  stack_allocator_free(
    void* memory, usize size, usize alignment, void* ctx
) {
    unused(memory);
    StackAllocator* stack = ctx;
    stack_allocator_pop( stack, size, alignment );
}

attr_core_api void* ___internal_memory_alloc_(
    usize size, usize alignment, void* ctx
) {
    unused( ctx );
    if( alignment ) {
        return memory_alloc_aligned( size, alignment );
    } else {
        return memory_alloc( size );
    }
}
attr_core_api void* ___internal_memory_realloc_(
    void* memory, usize old_size, usize new_size, usize alignment, void* ctx
) {
    unused(ctx);
    if( alignment ) {
        return memory_realloc_aligned( memory, old_size, new_size, alignment );
    } else {
        return memory_realloc( memory, old_size, new_size );
    }
}
attr_core_api void ___internal_memory_free_(
    void* memory, usize size, usize alignment, void* ctx
) {
    unused(ctx);
    if( alignment ) {
        memory_free_aligned( memory, size, alignment );
    } else {
        memory_free( memory, size );
    }
}

