#if !defined(CORE_INTERNAL_SSE_INTRINSICS_H)
#define CORE_INTERNAL_SSE_INTRINSICS_H
/**
 * Description:  SSE Intrinsics
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 14, 2024
*/
#include "core/defines.h"
#if !(defined(CORE_ENABLE_SSE_INSTRUCTIONS) && defined(CORE_ARCH_X86))
    #error "SSE intrinsics included in project that does not support them!"
#endif

// IWYU pragma: begin_exports

/// MMX
#include <mmintrin.h>
/// SSE
#include <xmmintrin.h>
/// SSE2
#include <emmintrin.h>
/// SSE3
#include <pmmintrin.h>
/// SSSE3
#include <tmmintrin.h>
/// SSE4.1
#include <smmintrin.h>
/// SSE4.2
#include <nmmintrin.h>

#define sse_blend_mask( a, b, c, d ) ((d) << 3 | (c) << 2 | (b) << 1 | (a))

// IWYU pragma: end_exports

#endif /* header guard */
