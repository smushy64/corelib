#if !defined(CORE_CPP_MATH_CONVERSIONS_HPP)
#define CORE_CPP_MATH_CONVERSIONS_HPP
/**
 * @file   conversions.hpp
 * @brief  Conversions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/constants.h"

attr_header
constexpr f32 radians( f32 x ) {
    return x * F32_TO_RAD;
}
attr_header
constexpr f64 radians( f64 x ) {
    return x * F64_TO_RAD;
}
attr_header
constexpr f32 degrees( f32 x ) {
    return x * F32_TO_DEG;
}
attr_header
constexpr f64 degrees( f64 x ) {
    return x * F64_TO_DEG;
}

attr_header
constexpr i32 trunc( f32 x ) {
    return (i32)x;
}
attr_header
constexpr i64 trunc( f64 x ) {
    return (i64)x;
}
attr_header
constexpr i32 floor( f32 x ) {
    return x < 0.0f ? trunc( x - 1.0f ) : trunc( x );
}
attr_header
constexpr i64 floor( f64 x ) {
    return x < 0.0 ? trunc( x - 1.0 ) : trunc( x );
}
attr_header
constexpr i32 ceil( f32 x ) {
    return x < 0.0f ? trunc( x ) : trunc( x + 1.0f );
}
attr_header
constexpr i64 ceil( f64 x ) {
    return x < 0.0 ? trunc( x ) : trunc( x + 1.0 );
}
attr_header
constexpr i32 round( f32 x ) {
    return x < 0.0f ? trunc( x - 0.5f ) : trunc( x + 0.5f );
}
attr_header
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

attr_header
constexpr u8 endian_swap( u8 x ) {
    return x;
}
attr_header
constexpr u16 endian_swap( u16 x ) {
    return ((x & 0xFF00) >> 8) | ((x & 0x00FF) << 8);
}
attr_header
constexpr u32 endian_swap( u32 x ) {
    return
        ((x & 0xFF000000) >> 24u) |
        ((x & 0x00FF0000) >> 8u)  |
        ((x & 0x0000FF00) << 8u)  |
        ((x & 0x000000FF) << 24u);
}
attr_header
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

#endif /* header guard */
