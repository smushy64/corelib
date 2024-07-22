#if !defined(CORE_MATH_CONVERSIONS_H)
#define CORE_MATH_CONVERSIONS_H
/**
 * @file   conversions.h
 * @brief  Integral type conversions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 01, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/constants.h"

/// @brief Convert radians to degrees.
/// @param angle Angle (degrees) to convert.
/// @return Angle in radians.
/// @see #F32_TO_RAD
#define to_rad( angle ) ( (angle) * F32_TO_RAD )
/// @brief Convert degrees to radians.
/// @param angle Angle (radians) to convert.
/// @return Angle in degrees.
/// @see #F32_TO_DEG
#define to_deg( angle ) ( (angle) * F32_TO_DEG )

/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f32 normalize_u8( u8 x ) {
    return (f32)x / (f32)U8_MAX;
}
/// @brief Normalize x to -1 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f32 normalize_i8( i8 x ) {
    return x < 0 ? -( (f32)x / (f32)I8_MIN ) : (f32)x / (f32)I8_MAX;
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f32 normalize_u16( u16 x ) {
    return (f32)x / (f32)U16_MAX;
}
/// @brief Normalize x to -1 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f32 normalize_i16( i16 x ) {
    return x < 0 ? -( (f32)x / (f32)I16_MIN ) : (f32)x / (f32)I16_MAX;
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f32 normalize_u32( u32 x ) {
    return (f32)x / (f32)U32_MAX;
}
/// @brief Normalize x to -1 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f32 normalize_i32( i32 x ) {
    return x < 0.0f ? -( (f32)x / (f32)I32_MIN ) : (f32)x / (f32)I32_MAX;
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f64 normalize_u64( u64 x ) {
    return (f64)x / (f64)U64_MAX;
}
/// @brief Normalize x to -1 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline
attr_header f64 normalize_i64( i64 x ) {
    return x < 0 ? -( (f64)x / (f64)I64_MIN ) : (f64)x / (f64)I64_MAX;
}

/// @brief Truncate float to integer.
/// @param x Float to truncate.
/// @return Truncated value.
attr_always_inline
attr_header i32 truncate_f32( f32 x ) {
    return (i32)x;
}
/// @brief Truncate float to integer.
/// @param x Float to truncate.
/// @return Truncated value.
attr_always_inline
attr_header i64 truncate_f64( f64 x ) {
    return (i64)x;
}
/// @brief Floor float to integer.
/// @param x Float to floor.
/// @return Floored value.
attr_always_inline
attr_header i32 floor_f32( f32 x ) {
    return x < 0 ? truncate_f32( x - 1.0f ) : truncate_f32( x );
}
/// @brief Floor float to integer.
/// @param x Float to floor.
/// @return Floored value.
attr_always_inline
attr_header i64 floor_f64( f64 x ) {
    return x < 0 ? truncate_f64( x - 1.0 ) : truncate_f64( x );
}
/// @brief Ceil float to integer.
/// @param x Float to ceil.
/// @return Ceiled value.
attr_always_inline
attr_header i32 ceil_f32( f32 x ) {
    return x < 0 ? truncate_f32( x ) : truncate_f32( x + 1.0f );
}
/// @brief Ceil float to integer.
/// @param x Float to ceil.
/// @return Ceiled value.
attr_always_inline
attr_header i64 ceil_f64( f64 x ) {
    return x < 0 ? truncate_f64( x ) : truncate_f64( x + 1.0 );
}
/// @brief Round float to integer.
/// @param x Float to round.
/// @return Rounded value.
attr_always_inline
attr_header i32 round_f32( f32 x ) {
    return x < 0 ? truncate_f32( x - 0.5f ) : truncate_f32( x + 0.5f );
}
/// @brief Round float to integer.
/// @param x Float to round.
/// @return Rounded value.
attr_always_inline
attr_header i64 round_f64( f64 x ) {
    return x < 0 ? truncate_f64( x - 0.5 ) : truncate_f64( x + 0.5 );
}
/// @brief Swap endianness of unsigned 16-bit integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_always_inline
attr_header u16 u16_endian_swap( u16 x ) {
    return (( x & 0xFF00 ) >> 8) | ((x & 0x00FF) << 8);
}
/// @brief Swap endianness of unsigned 32-bit integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_always_inline
attr_header u32 u32_endian_swap( u32 x ) {
    return
        ((x & 0xFF000000) >> 24u) |
        ((x & 0x00FF0000) >> 8u)  |
        ((x & 0x0000FF00) << 8u)  |
        ((x & 0x000000FF) << 24u);
}
/// @brief Swap endianness of unsigned 64-bit integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_always_inline
attr_header u64 u64_endian_swap( u64 x ) {
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

#endif /* header guard */
