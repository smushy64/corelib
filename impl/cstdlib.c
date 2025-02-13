/**
 * @file   cstdlib.c
 * @brief  C Standard Library replacement functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 18, 2024
*/
#include "core/attributes.h"
#include "core/types.h"

#if !defined(CORE_ENABLE_STDLIB)

#if defined(CORE_COMPILER_MSVC)
    #pragma function(memcpy)
#else
attr_c_call attr_export
#endif
void* memcpy(
    void* attr_restrict dst, const void* attr_restrict src, usize size
) {
    for( usize i = 0; i < size; ++i ) {
        ((u8*)dst)[i] = ((u8*)src)[i];
    }
    return dst;
}
#if defined(CORE_COMPILER_MSVC)
    #pragma function(memset)
#else
attr_c_call attr_export
#endif
void* memset( void* dst, int val, usize size ) {
    for( usize i = 0; i < size; ++i ) {
        *((i8*)dst + i) = val;
    }
    return dst;
}
#if defined(CORE_COMPILER_MSVC)
    #pragma function(memmove)
#else
attr_c_call attr_export
#endif
void* memmove( void* str1, const void* str2, usize n ) {
    if( !n ) {
        return str1;
    }
    if( str1 < str2 ) {
        return memcpy( str1, str2, n );
    }
    u8* a = str1;
    const u8* b = str2;

    for( usize i = n; i-- > 0; ) {
        a[i] = b[i];
    }

    return str1;
}

#endif
