/**
 * Description:  Memory function implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 02, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/memory.h"
#include "core/macros.h"

#include "core/internal/platform.h"

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    #include "core/internal/sse.h"
#endif

attr_clink attr_export void*
memcpy( void* attr_restrict dst, const void* attr_restrict src, usize size );

attr_clink attr_export void*
memset( void* dst, int val, usize size );

attr_clink attr_export void*
memmove( void* str1, const void* str2, usize n );

attr_internal void internal_memory_set_word( void* dst, u8 val, usize size ) {

    usize sizeptr = size / sizeof(usize);
    union { u8 bytes[sizeof(usize)]; usize bytesptr; } valptr;

    valptr.bytes[0] = val;
    valptr.bytes[1] = val;
    valptr.bytes[2] = val;
    valptr.bytes[3] = val;
#if defined(CORE_ARCH_64_BIT)
    valptr.bytes[4] = val;
    valptr.bytes[5] = val;
    valptr.bytes[6] = val;
    valptr.bytes[7] = val;
#endif

    for( usize i = 0; i < sizeptr; ++i ) {
        *( ((usize*)dst) + i ) = valptr.bytesptr;
    }

    usize rem = size % sizeof(usize);
    u8* dst_rem = (u8*)( ((usize*)dst) + sizeptr );

    for( usize i = 0; i < rem; ++i ) {
        *(dst_rem + i) = val;
    }
}


attr_internal void internal_memory_copy_word(
    void* attr_restrict dst, const void* attr_restrict src, usize size
) {
    usize* dstptr = (usize*)dst;
    usize* srcptr = (usize*)src;
    usize rem     = size;
    usize sizeptr = rem;
    if( size < sizeof(usize) ) {
        goto internal_memory_copy_bytes;
    }

    sizeptr /= sizeof(usize);
    rem     %= sizeof(usize);

    for( usize i = 0; i < sizeptr; ++i ) {
        *dstptr++ = *srcptr++;
    }

internal_memory_copy_bytes:
    {
        u8* dstbyte = (u8*)dstptr;
        u8* srcbyte = (u8*)srcptr;
        for( usize i = 0; i < rem; ++i ) {
            *dstbyte++ = *srcbyte++;
        }
    }
}

#define INTERNAL_MEMORY_COPY_PREFER_SIMD_SIZE (gibibytes(1) + mebibytes(512))

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)

attr_always_inline inline
attr_internal void internal_memory_copy_sse(
    void* attr_restrict dst, const void* attr_restrict src, usize size
) {
    if( size < INTERNAL_MEMORY_COPY_PREFER_SIMD_SIZE ) {
        internal_memory_copy_word( dst, src, size );
        return;
    }

    // TODO(alicia): stream and stream_load!
    // but those need aligned memory and idk how im gonna align
    // both dst and src and keep them in sync with each other
    // maybe only use streaming intrinsics if alignments match up?

    usize size128 = size / sizeof(__m128);
    usize rem     = size % sizeof(__m128);

    __m128i* dst128 = (__m128i*)dst;
    __m128i* src128 = (__m128i*)src;
    for( usize i = 0; i < size128; ++i ) {
        _mm_storeu_si128( dst128++, _mm_loadu_si128( src128++ ) );
    }

    u8* dstbyte = (u8*)dst128;
    u8* srcbyte = (u8*)src128;
    for( usize i = 0; i < rem; ++i ) {
        *dstbyte++ = *srcbyte++;
    }
}

#endif /* SSE */

attr_core_api void memory_copy(
    void* attr_restrict dst, const void* attr_restrict src, usize size
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    internal_memory_copy_sse( dst, src, size );
#else
    internal_memory_copy_word( dst, src, size );
#endif
}
attr_core_api void memory_move(
    void* dst, const void* src, usize size
) {
    // TODO(alicia): implementation with sse
    (void)memmove( dst, src, size );
}
attr_core_api void memory_set( void* dst, u8 byte, usize size ) {
    // TODO(alicia): implementation with sse
    internal_memory_set_word( dst, byte, size );
}
attr_core_api void memory_set_chunks(
    void* dst, usize chunk_size, const void* chunk, usize count
) {
    // TODO(alicia): SIMD?
    u8* dst_bytes = (u8*)dst;
    for( usize i = 0; i < count; ++i ) {
        memory_copy( dst_bytes, chunk, chunk_size );
        dst_bytes += chunk_size;
    }
}
attr_core_api b32 memory_cmp( const void* a, const void* b, usize size ) {
    // TODO(alicia): SIMD?

    usize sizeptr = size / sizeof(usize);
    for( usize i = 0; i < sizeptr; ++i ) {
        if( *( ((usize*)a) + i ) != *( ((usize*)b) + i ) ) {
            return false;
        }
    }

    usize rem = size % sizeof(usize);
    u8* a_rem = (u8*)( ((usize*)a) + sizeptr );
    u8* b_rem = (u8*)( ((usize*)b) + sizeptr );

    for( usize i = 0; i < rem; ++i ) {
        if( *(a_rem + i) != *(b_rem + i) ) {
            return false;
        }
    }

    return true;
}

attr_core_api void* memory_alloc( usize size ) {
    return platform_heap_alloc( (usize)size );
}
attr_core_api void* memory_realloc(
    void* old_buffer, usize old_size, usize new_size
) {
    return platform_heap_realloc( old_buffer, (usize)old_size, (usize)new_size );
}
attr_core_api void memory_free( void* buffer, usize size ) {
    if( buffer ) {
        platform_heap_free( buffer, (usize)size );
    }
}
attr_core_api void* memory_alloc_aligned( usize size, usize alignment ) {
    void* ptr = memory_alloc( size + alignment + sizeof(void*) );
    if( !ptr ) {
        return NULL;
    }

    void* aligned = memory_align( (u8*)ptr + sizeof(void*), alignment );
    ((void**)aligned)[-1] = ptr;

    return aligned;
}
attr_core_api void* memory_realloc_aligned(
    void* old_buffer, usize old_size,
    usize new_size, usize alignment
) {
    void* base = ((void**)old_buffer)[-1];
    usize off  = (usize)old_buffer - (usize)base;

    void* new_buffer = memory_realloc(
        base, old_size + alignment + sizeof(void*), new_size );
    if( !new_buffer ) {
        return NULL;
    }

    void* aligned = (void*)((u8*)new_buffer + off);
    ((void**)aligned)[-1] = new_buffer;

    return aligned;
}
attr_core_api void memory_free_aligned(
    void* buffer, usize size, usize alignment
) {
    if( !buffer ) {
        return;
    }

    void* base = ((void**)buffer)[-1];
    memory_free( base, size + alignment + sizeof(void*) );
}
attr_core_api usize memory_calculate_aligned_size( usize size, usize alignment ) {
    return size + alignment + sizeof(void*);
}

#undef INTERNAL_MEMORY_COPY_PREFER_SIMD_SIZE

