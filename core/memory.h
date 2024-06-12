#if !defined(CORE_MEMORY_H)
#define CORE_MEMORY_H
/**
 * @file   memory.h
 * @brief  Memory management.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 02, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/defines.h"

#if defined(CORE_ARCH_64_BIT)
    /// @brief Guaranteed alignment of memory allocated from default allocator.
    #define CORE_MEMORY_ALLOC_ALIGNMENT (16)
#else
    /// @brief Guaranteed alignment of memory allocated from default allocator.
    #define CORE_MEMORY_ALLOC_ALIGNMENT (8)
#endif

/// @brief Copy memory from source buffer to destination buffer.
/// @note src and dst MUST be pointing to non-overlapping buffers!
/// @param[in] dst Destination buffer, must not overlap with src.
/// @param[in] src Source buffer, must not overlap with dst.
/// @param size Number of bytes to copy. Both src and dst must be able to hold this many bytes.
/// @see #memory_move() for copying between overlapping buffers.
attr_core_api void memory_copy(
    void* attr_restrict dst, const void* attr_restrict src, usize size );
/// @brief Copy overlapping memory from source buffer to destination buffer.
/// @param[in] dst Destination buffer.
/// @param[in] src Source buffer.
/// @param size Number of bytes to copy. Both src and dst must be able to hold this many bytes.
attr_core_api void memory_move( void* dst, const void* src, usize size );
/// @brief Set range of bytes in destination buffer to given value.
/// @param[in] dst Destination buffer.
/// @param byte Value to set bytes to.
/// @param size Number of bytes to set. dst must be able to hold this many bytes.
attr_core_api void memory_set( void* dst, u8 byte, usize size );
/// @brief Set range of chunks to given chunk value.
/// @param[in] dst Destination buffer.
/// @param chunk_size Size of chunk.
/// @param[in] chunk Pointer to chunk, must be size of chunk_size.
/// @param count Number of times to copy chunk to dst, dst must be able to hold chunk_size * count.
attr_core_api void memory_set_chunks(
    void* dst, usize chunk_size, const void* chunk, usize count );
/// @brief Compare two memory ranges for equality.
/// @param[in] a, b Pointers to buffers to compare.
/// @param size Number of bytes to compare.
/// @return True if memory ranges are identical.
attr_core_api b32 memory_cmp( const void* a, const void* b, usize size );
/// @brief Set range of bytes in destination buffer to zero.
/// @param[in] dst Destination buffer.
/// @param size Number of bytes to set. dst must be able to hold this many bytes.
attr_always_inline
attr_header void memory_zero( void* dst, usize size ) {
    memory_set( dst, 0, size );
}

/// @brief Align a pointer to given alignment
/// @param[in] src Pointer to align.
/// @param alignment Alignment.
/// @note alignment MUST be power of two.
/// @return Aligned pointer.
#define memory_align( src, alignment )\
    ((void*)((((usize)(src)) + (alignment)) & ~((usize)0x0F)))

/// @brief Allocate memory using default system allocator.
///
/// Memory returned is always zeroed.
/// @note Alignment of memory allocated is guaranteed to be #CORE_MEMORY_ALLOC_ALIGNMENT.
/// @param size Number of bytes to allocate.
/// @return Pointer to start of allocated buffer. Returns null if failed to allocate.
attr_core_api void* memory_alloc( const usize size );
/// @brief Reallocate buffer using default system allocator.
///
/// New memory returned is always zeroed.
/// @param[in] old_buffer Pointer to buffer to reallocate.
/// @param old_size Size of buffer before allocation.
/// @param new_size Desired new size of buffer.
/// @return Pointer to start of new buffer. Returns null if failed to reallocate.
/// @note Alignment of memory allocated is guaranteed to be #CORE_MEMORY_ALLOC_ALIGNMENT.
/// @note To reallocate memory allocated with #memory_alloc_aligned(), use #memory_realloc_aligned().
/// @warning There is no guarantee that the
/// returned pointer points to the same location that @c old_buffer points to!
attr_core_api void* memory_realloc(
    void* old_buffer, usize old_size, usize new_size );
/// @brief Free memory allocated using default system allocator.
/// @note To free memory allocated with #memory_alloc_aligned(), use #memory_free_aligned().
/// @param[in] buffer Pointer to start of buffer, can be null.
/// @param size Size of buffer.
attr_core_api void memory_free( void* buffer, usize size );
/// @brief Allocate memory using default system allocator.
///
/// Memory returned is always zeroed.
/// @note To free aligned memory, use #memory_free_aligned().
/// @param size Number of bytes to allocate.
/// @param alignment Alignment.
/// @note Alignment MUST be a power of 2.
/// @return Pointer to start of allocated buffer. Returns null if failed to allocate.
attr_core_api void* memory_alloc_aligned( usize size, usize alignment );
/// @brief Reallocate buffer using default system allocator.
///
/// New memory returned is always zeroed.
/// @note To free aligned memory, use #memory_free_aligned().
/// @param[in] old_buffer Pointer to buffer to reallocate.
/// @param old_size Size of buffer before allocation.
/// @param new_size Desired new size of buffer.
/// @param alignment Alignment.
/// @note Alignment MUST be a power of 2.
/// @note Alignment MUST match alignment when buffer was allocated.
/// @return Pointer to start of new buffer. Returns null if failed to reallocate.
attr_core_api void* memory_realloc_aligned(
    void* old_buffer, usize old_size,
    usize new_size, usize alignment );
/// @brief Free memory allocated using default system allocator.
/// @param[in] buffer Pointer to start of buffer, can be null.
/// @param size Size of buffer.
/// @param alignment Alignment.
/// @note Alignment MUST be a power of 2.
/// @note Alignment MUST match alignment when buffer was allocated.
attr_core_api void memory_free_aligned(
    void* buffer, const usize size, const usize alignment );
/// @brief Calculate real size of aligned allocation.
/// @param size Unaligned size of allocation.
/// @param alignment Alignment.
/// @return Size of aligned allocation.
/// @note Alignment MUST be a power of 2.
attr_core_api usize memory_calculate_aligned_size( usize size, usize alignment );

#endif /* header guard */
