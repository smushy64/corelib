#if !defined(CORE_CPP_MATH_MACROS_HPP)
#define CORE_CPP_MATH_MACROS_HPP
/**
 * @file   macros.hpp
 * @brief  Helper math macros.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/constants.h"

template<typename T>
attr_always_inline attr_header
constexpr T sign( T x ) {
    return (x > (T)0) - (x < (T)0);
}
template<typename T>
attr_always_inline attr_header
constexpr T abs( T x ) {
    return ( (x) < (T)0 ? -(x) : (x) );
}
template<typename T>
attr_always_inline attr_header
constexpr T max( T x, T y ) {
    return ((x) < (y) ? (y) : (x));
}
template<typename T>
attr_always_inline attr_header
constexpr T min( T x, T y ) {
    return ((x) < (y) ? (x) : (y));
}
template<typename T>
attr_always_inline attr_header
constexpr T clamp( T v, T min, T max ) {
    return ( (v) < (min) ? (min) : ( (v) > (max) ? (max) : (v) ) );
}
attr_always_inline attr_header
constexpr f32 clamp( f32 v ) {
    return clamp( v, 0.0f, 1.0f );
}
attr_always_inline attr_header
constexpr f64 clamp( f64 v ) {
    return clamp( v, 0.0, 1.0 );
}
attr_always_inline attr_header
constexpr b32 fcmp( f32 a, f32 b ) {
    return abs( a - b ) <= F32_EPSILON;
}
attr_always_inline attr_header
constexpr b32 fcmp( f64 a, f64 b ) {
    return abs( a - b ) <= F64_EPSILON;
}

#endif /* header guard */
