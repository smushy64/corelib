#if !defined(CORE_CPP_MATH_CONVERSIONS_HPP)
#define CORE_CPP_MATH_CONVERSIONS_HPP
/**
 * @file   conversions.hpp
 * @brief  Conversions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/

#if !defined(CORE_MATH_CONVERSIONS_H)
    #include "core/math/conversions.h"
#endif

/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_header f32 normalize( u8 x ) {
    return normalize_u8( x );
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_header f32 normalize( u16 x ) {
    return normalize_u16( x );
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_header f32 normalize( u32 x ) {
    return normalize_u32( x );
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_header f64 normalize( u64 x ) {
    return normalize_u64( x );
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_header f32 normalize( i8 x ) {
    return normalize_i8( x );
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_header f32 normalize( i16 x ) {
    return normalize_i16( x );
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_header f32 normalize( i32 x ) {
    return normalize_i32( x );
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_header f64 normalize( i64 x ) {
    return normalize_i64( x );
}

/// @brief Truncate float to integer.
/// @param x Float to truncate.
/// @return Truncated value.
attr_header i32 truncate( f32 x ) {
    return truncate_f32( x );
}
/// @brief Truncate float to integer.
/// @param x Float to truncate.
/// @return Truncated value.
attr_header i64 truncate( f64 x ) {
    return truncate_f64( x );
}
/// @brief Floor float to integer.
/// @param x Float to floor.
/// @return Floored value.
attr_header i32 floor( f32 x ) {
    return floor_f32( x );
}
/// @brief Floor float to integer.
/// @param x Float to floor.
/// @return Floored value.
attr_header i64 floor( f64 x ) {
    return floor_f64( x );
}
/// @brief Ceil float to integer.
/// @param x Float to ceil.
/// @return Ceiled value.
attr_header i32 ceil( f32 x ) {
    return ceil_f32( x );
}
/// @brief Ceil float to integer.
/// @param x Float to ceil.
/// @return Ceiled value.
attr_header i64 ceil( f64 x ) {
    return ceil_f64( x );
}
/// @brief Round float to integer.
/// @param x Float to round.
/// @return Rounded value.
attr_header i32 round( f32 x ) {
    return round_f32( x );
}
/// @brief Round float to integer.
/// @param x Float to round.
/// @return Rounded value.
attr_header i64 round( f64 x ) {
    return round_f64( x );
}
/// @brief Swap endianness of integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_header u16 endian_swap( u16 x ) {
    return u16_endian_swap( x );
}
/// @brief Swap endianness of integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_header u32 endian_swap( u32 x ) {
    return u32_endian_swap( x );
}
/// @brief Swap endianness of integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_header u64 endian_swap( u64 x ) {
    return u64_endian_swap( x );
}
/// @brief Swap endianness of integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_header i16 endian_swap( i16 x ) {
    return i16_endian_swap( x );
}
/// @brief Swap endianness of integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_header i32 endian_swap( i32 x ) {
    return i32_endian_swap( x );
}
/// @brief Swap endianness of integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_header i64 endian_swap( i64 x ) {
    return i64_endian_swap( x );
}

#endif /* header guard */
