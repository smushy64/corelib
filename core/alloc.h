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
#include "core/string.h"
#include "core/memory.h"

/// @brief Function prototype for allocator allocation function.
/// @details
/// All allocators must return zeroed memory. (Only newly allocated space)
/// @param[in] ctx          (optional) Allocator context.
/// @param[in] memory       (nullable) Pointer to memory to reallocate. If null, @c old_size is ignored.
/// @param     old_size     Size of @c memory. If @c memory is null, ignored.
/// @param     new_size     New size of @c memory. Must be >= @c old_size.
/// @param     opt_name     (optional) Name of allocator.
/// @param[in] opt_file     (optional) Name of file where function was called.
/// @param     opt_line     (optional) Line where function was called.
/// @param[in] opt_function (optional) Name of function where this function was called.
/// @return
///     - NULL    : Failed to reallocate buffer. @c memory is still valid.
///     - Pointer : Pointer to new buffer. @c memory is no longer valid.
typedef void* AllocatorAllocFN(
    void* ctx, void* memory, usize old_size, usize new_size, struct _StringPOD opt_name,
    const char* opt_file, int opt_line, const char* opt_function );
/// @brief Function prototype for allocator free function.
/// @param[in] ctx          (optional) Allocator context.
/// @param[in] memory       Pointer to memory to free.
/// @param     size         Size of @c memory.
/// @param     opt_name     (optional) Name of allocator.
/// @param[in] opt_file     (optional) Name of file where function was called.
/// @param     opt_line     (optional) Line where function was called.
/// @param[in] opt_function (optional) Name of function where this function was called.
typedef void AllocatorFreeFN(
    void* ctx, void* memory, usize size, struct _StringPOD opt_name,
    const char* opt_file, int opt_line, const char* opt_function );

/// @brief Interface for memory allocators.
/// @note
/// Allocator interfaces are meant to be defined
/// before your interface. As an example:
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
///
/// struct StackAllocator {
///     struct AllocatorInterface interface;
///     usize cap;
///     usize len;
///     void* base;
/// };
///
/// struct StackAllocator stack;
///
/// allocator_alloc( (struct AllocatorInterface*)&stack, 10 );
///
/// // OR
///
/// allocator_alloc( &stack.interface, 10 );
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// That way, when you call allocator_alloc, you can just rcast the
/// allocator or take a pointer to the interface itself. allocator_alloc
/// passes one past the pointer provided as the context.
///
typedef struct AllocatorInterface {

    /// @brief String identifying this allocator.
    struct _StringPOD name;

    /// @brief Function prototype for allocator allocation function.
    /// @details
    /// All allocators must return zeroed memory. (Only newly allocated space)
    /// @param[in] ctx          (optional) Allocator context.
    /// @param[in] memory       (nullable) Pointer to memory to reallocate. If null, @c old_size is ignored.
    /// @param     old_size     Size of @c memory. If @c memory is null, ignored.
    /// @param     new_size     New size of @c memory. Must be >= @c old_size.
    /// @param     opt_name     (optional) Name of allocator.
    /// @param[in] opt_file     (optional) Name of file where function was called.
    /// @param     opt_line     (optional) Line where function was called.
    /// @param[in] opt_function (optional) Name of function where this function was called.
    /// @return
    ///     - NULL    : Failed to reallocate buffer. @c memory is still valid.
    ///     - Pointer : Pointer to new buffer. @c memory is no longer valid.
    AllocatorAllocFN* alloc;
    /// @brief Function prototype for allocator free function.
    /// @param[in] ctx          (optional) Allocator context.
    /// @param[in] memory       Pointer to memory to free.
    /// @param     size         Size of @c memory.
    /// @param     opt_name     (optional) Name of allocator.
    /// @param[in] opt_file     (optional) Name of file where function was called.
    /// @param     opt_line     (optional) Line where function was called.
    /// @param[in] opt_function (optional) Name of function where this function was called.
    AllocatorFreeFN* free;

} AllocatorInterface;

/// @brief Fill in an allocator interface that uses heap allocation functions.
/// @param[out] out_interface Pointer to interface to fill out.
attr_core_api
void allocator_interface_from_heap( AllocatorInterface* out_interface );

/// @brief Call allocator interface allocate function.
/// @param[in] allocator Pointer to allocator interface.
/// @param     size      Size of allocation.
/// @return
///     - NULL    : Failed to allocate buffer.
///     - Pointer : Pointer to new buffer.
#define allocator_alloc( allocator, size ) \
    (allocator)->alloc( \
        (void*)((allocator) + 1), NULL, 0, size, (allocator)->name, \
        __FILE__, __LINE__, __func__ )

/// @brief Call allocator interface reallocate function.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] memory    Pointer to start of buffer.
/// @param     old_size  Size of @c memory.
/// @param     new_size  New size of @c memory.
/// @return
///     - NULL    : Failed to reallocate buffer.
///     - Pointer : Pointer to new buffer.
#define allocator_realloc( allocator, memory, old_size, new_size ) \
    (allocator)->alloc( \
        (void*)((allocator) + 1), memory, old_size, new_size, (allocator)->name, \
        __FILE__, __LINE__, __func__ )

/// @brief Call allocator interface free function.
/// @param[in] allocator Pointer to allocator interface.
/// @parma[in] memory    Pointer to start of buffer to free.
/// @param     size      Size of @c memory.
#define allocator_free( allocator, memory, size ) \
    (allocator)->free( \
        (void*)((allocator) + 1), memory, size, (allocator)->name, \
        __FILE__, __LINE__, __func__ )

/// @brief Call allocator interface allocate function.
/// @param[in] allocator Pointer to allocator interface.
/// @param     size      Size of allocation.
/// @param     alignment Alignment of memory.
/// @return
///     - NULL    : Failed to allocate buffer.
///     - Pointer : Pointer to new buffer.
#define allocator_alloc_aligned( allocator, size, alignment ) \
    __allocator_interface_alloc_aligned__( \
        (allocator), NULL, 0, size, alignment, __FILE__, __LINE__, __func__ )

/// @brief Call allocator interface reallocate function.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] memory    Pointer to start of buffer.
/// @param     old_size  Size of @c memory.
/// @param     new_size  New size of @c memory.
/// @param     alignment Alignment of memory.
/// @return
///     - NULL    : Failed to reallocate buffer.
///     - Pointer : Pointer to new buffer.
#define allocator_realloc_aligned( allocator, memory, old_size, new_size, alignment ) \
    __allocator_interface_alloc_aligned__( \
        (allocator), memory, old_size, new_size, alignment, __FILE__, __LINE__, __func__ )

/// @brief Call allocator interface free function.
/// @param[in] allocator Pointer to allocator interface.
/// @parma[in] memory    Pointer to start of buffer to free.
/// @param     size      Size of @c memory.
/// @param     alignment Alignment of memory.
#define allocator_free_aligned( allocator, memory, size, alignment ) \
    __allocator_interface_free_aligned__( \
        (allocator), memory, size, alignment, __FILE__, __LINE__, __func__ )

attr_always_inline attr_header attr_optimized
void* __allocator_interface_alloc_aligned__(
    AllocatorInterface* allocator, void* memory,
    usize old_size, usize new_size, usize alignment,
    const char* file, int line, const char* function 
) {
    if( memory ) {
        void* base = ((void**)memory)[-1];
        usize off  = (usize)memory - (usize)base;

        void* new_buffer = allocator->alloc(
            (void*)(allocator + 1), base,
            old_size + alignment + sizeof(void*),
            new_size + alignment + sizeof(void*),
            allocator->name, file, line, function );

        if( !new_buffer ) {
            return NULL;
        }

        void* aligned = (void*)((u8*)new_buffer + off);
        ((void**)aligned)[-1] = new_buffer;

        return aligned;
    } else {
        void* ptr = allocator->alloc(
            (void*)(allocator + 1), NULL, 0,
            new_size, allocator->name, file, line, function );
        if( !ptr ) {
            return NULL;
        }

        void* aligned = memory_align( (u8*)ptr + sizeof(void*), alignment );
        ((void**)aligned)[-1] = ptr;

        return aligned;
    }
}
attr_always_inline attr_header attr_optimized
void __allocator_interface_free_aligned__(
    AllocatorInterface* allocator, void* memory,
    usize size, usize alignment,
    const char* file, int line, const char* function
) {
    if( !memory ) {
        return;
    }
    void* base = ((void**)memory)[-1];
    allocator->free(
        (void*)(allocator + 1), base,
        size + alignment + sizeof(void*),
        allocator->name, file, line, function );
}

#endif /* header guard */
