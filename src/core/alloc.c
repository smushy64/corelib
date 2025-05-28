/**
 * @file   alloc.c
 * @brief  Memory allocator implementation.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   April 04, 2024
*/
#include "core/prelude.h"
#include "core/alloc.h"
#include "core/memory.h"

void* allocator_heap_alloc(
    AllocatorInterface* allocator, void* memory, usize old_size, usize new_size,
    const char* opt_file, int opt_line, const char* opt_function
) {
    unused(allocator, opt_file, opt_line, opt_function);

    if( memory ) {
        void* result = memory_realloc( memory, old_size, new_size );
        return result;
    }

    return memory_alloc( new_size );
}

void allocator_heap_free(
    AllocatorInterface* allocator, void* memory, usize size,
    const char* opt_file, int opt_line, const char* opt_function
) {
    unused(allocator, opt_file, opt_line, opt_function);

    if( !memory ) {
        return;
    }
    memory_free( memory, size );
}

attr_core_api
void allocator_interface_from_heap( AllocatorInterface* out_interface ) {
    out_interface->alloc = allocator_heap_alloc;
    out_interface->free  = allocator_heap_free;
    out_interface->name  = string_text( "Heap" );
}

