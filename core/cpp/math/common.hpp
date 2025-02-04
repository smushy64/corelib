#if !defined(CORE_CPP_MATH_COMMON_HPP)
#define CORE_CPP_MATH_COMMON_HPP
/**
 * @file   common.hpp
 * @brief  Common math functions.
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

attr_always_inline attr_header
constexpr i32 trunc( f32 x ) {
    return (i32)x;
}
attr_always_inline attr_header
constexpr i64 trunc( f64 x ) {
    return (i64)x;
}
attr_always_inline attr_header
constexpr i32 floor( f32 x ) {
    return x < 0.0f ? trunc( x - 1.0f ) : trunc( x );
}
attr_always_inline attr_header
constexpr i64 floor( f64 x ) {
    return x < 0.0 ? trunc( x - 1.0 ) : trunc( x );
}
attr_always_inline attr_header
constexpr i32 ceil( f32 x ) {
    return x < 0.0f ? trunc( x ) : trunc( x + 1.0f );
}
attr_always_inline attr_header
constexpr i64 ceil( f64 x ) {
    return x < 0.0 ? trunc( x ) : trunc( x + 1.0 );
}
attr_always_inline attr_header
constexpr i32 round( f32 x ) {
    return x < 0.0f ? trunc( x - 0.5f ) : trunc( x + 0.5f );
}
attr_always_inline attr_header
constexpr i64 round( f64 x ) {
    return x < 0.0f ? trunc( x - 0.5f ) : trunc( x + 0.5f );
}
/// @brief Get fractional part.
/// @param x Float.
/// @return Fractional part.
attr_always_inline
attr_header f32 fract( f32 x ) {
    return x - (f32)round(x);
}
/// @brief Get fractional part.
/// @param x Float.
/// @return Fractional part.
attr_always_inline
attr_header f64 fract( f64 x ) {
    return x - (f64)round(x);
}

attr_always_inline attr_header
constexpr u8 endian_swap( u8 x ) {
    return x;
}
attr_always_inline attr_header
constexpr u16 endian_swap( u16 x ) {
    return ((x & 0xFF00) >> 8) | ((x & 0x00FF) << 8);
}
attr_always_inline attr_header
constexpr u32 endian_swap( u32 x ) {
    return
        ((x & 0xFF000000) >> 24u) |
        ((x & 0x00FF0000) >> 8u)  |
        ((x & 0x0000FF00) << 8u)  |
        ((x & 0x000000FF) << 24u);
}
attr_always_inline attr_header
constexpr u64 endian_swap( u64 x ) {
    return
        ( (x & 0xFF00000000000000) >> 56ull ) |
        ( (x & 0x00FF000000000000) >> 40ull ) |
        ( (x & 0x0000FF0000000000) >> 24ull ) |
        ( (x & 0x000000FF00000000) >> 8ull  ) |
        ( (x & 0x00000000FF000000) << 8ull  ) |
        ( (x & 0x0000000000FF0000) << 24ull ) |
        ( (x & 0x000000000000FF00) << 40ull ) |
        ( (x & 0x00000000000000FF) << 56ull );
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f32 normalize( u8 x ) {
    return (f32)x / (f32)U8_MAX;
}
/// @brief Normalize x to -1 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f32 normalize( i8 x ) {
    return x < 0 ? -( (f32)x / (f32)I8_MIN ) : (f32)x / (f32)I8_MAX;
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f32  normalize( u16 x ) {
    return (f32)x / (f32)U16_MAX;
}
/// @brief Normalize x to -1 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f32  normalize( i16 x ) {
    return x < 0 ? -( (f32)x / (f32)I16_MIN ) : (f32)x / (f32)I16_MAX;
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f32  normalize( u32 x ) {
    return (f32)x / (f32)U32_MAX;
}
/// @brief Normalize x to -1 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f32  normalize( i32 x ) {
    return x < 0.0f ? -( (f32)x / (f32)I32_MIN ) : (f32)x / (f32)I32_MAX;
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f64  normalize( u64 x ) {
    return (f64)x / (f64)U64_MAX;
}
/// @brief Normalize x to -1 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f64  normalize( i64 x ) {
    return x < 0 ? -( (f64)x / (f64)I64_MIN ) : (f64)x / (f64)I64_MAX;
}

/// @brief Compare two floating point numbers for equality.
/// @param a, b (f32) Floats to compare for equality.
/// @return True if a and b are equal.
#define f32_cmp( a, b ) (abs((a) - (b)) <= F32_EPSILON)
/// @brief Compare two floating point numbers for equality.
/// @param a, b (f32) Floats to compare for equality.
/// @return True if a and b are equal.
#define f64_cmp( a, b ) (abs((a) - (b)) <= F64_EPSILON)

#endif /* header guard */
