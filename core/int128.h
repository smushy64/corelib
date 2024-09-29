#if !defined(CORE_INT128_H)
#define CORE_INT128_H
/**
 * @file   int128.h
 * @brief  128-bit integers.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 29, 2024
*/

#if !defined(CORE_TYPES_H)
    #include "core/types.h"
#endif

/// @brief 128-bit integer.
struct Integer128C {
    /// @brief High bits of 128-bit integer.
    u64 hi;
    /// @brief Low bits of 128-bit integer.
    u64 lo;
};

static inline struct Integer128C int128_add(
    struct Integer128C lhs, struct Integer128C rhs
) {
    struct Integer128C result;
    result.hi = lhs.hi + rhs.hi;
    result.lo = lhs.lo + rhs.lo;
    if( result.lo < lhs.lo ) {
        result.hi++;
    }
    return result;
}
static inline struct Integer128C int128_sub(
    struct Integer128C lhs, struct Integer128C rhs
) {
    struct Integer128C result;
    result.hi = lhs.hi - rhs.hi;
    result.lo = lhs.lo - rhs.lo;
    if( result.lo > lhs.lo ) {
        result.hi--;
    }
    return result;
}
static inline unsigned int int128_cmp( struct Integer128C a, struct Integer128C b ) {
    return a.hi == b.hi && a.lo == b.lo;
}

#if defined(__cplusplus)
    #if !defined(CORE_CPP_INT128_HPP)
        #include "core/cpp/int128.hpp"
    #endif
#endif

#endif /* header guard */
