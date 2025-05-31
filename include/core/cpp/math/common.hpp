#if !defined(CORE_CPP_MATH_COMMON_HPP) && defined(__cplusplus)
#define CORE_CPP_MATH_COMMON_HPP
/**
 * @file   common.hpp
 * @brief  C++ Math: Common math functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 29, 2025
*/
#if !defined(CORE_MATH_COMMON_H)
    #include "core/math/common.h" // IWYU pragma: export
#endif
#include "core/types.h"
#include "core/attributes.h"

#undef num_sign
#undef num_abs
#undef num_max
#undef num_min
#undef num_clamp
#undef isnan
#undef isinf
#undef endian_swap

/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
template<typename SignedNumber>
attr_always_inline attr_header attr_hot
SignedNumber sign( SignedNumber x ) {
    return (x > 0) - (x < 0);
}
/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
template<typename SignedNumber>
attr_always_inline attr_header attr_hot
SignedNumber abs( SignedNumber x ) {
    return (x < 0) ? -x : x;
}
/// @brief Get larger of two numbers.
/// @param x, y (any number) Numbers to compare.
/// @return Larger number.
template<typename NumberA, typename NumberB>
attr_always_inline attr_header attr_hot
NumberA max( NumberA x, NumberB y ) {
    return (x < y) ? y : x;
}
/// @brief Get smaller of two numbers.
/// @param x, y (any number) Numbers to compare.
/// @return Smaller number.
template<typename NumberA, typename NumberB>
attr_always_inline attr_header attr_hot
NumberA min( NumberA x, NumberB y ) {
    return (x < y) ? x : y;
}
/// @brief Clamp number between a minimum and maximum.
/// @param v   (any number) Value to clamp.
/// @param min (any number) Minimum value, should be less than max.
/// @param max (any number) Maximum value, should be greater than min.
/// @return Value clamped to range [min,max].
template<typename Number, typename NumberA, typename NumberB>
attr_always_inline attr_header attr_hot
Number clamp( Number x, NumberA min, NumberB max ) {
    return ( x < min ? min : ( x > max ? max : x ) );
}

/// @brief Compare two floating point numbers for equality.
/// @param a, b Floats to compare for equality.
/// @return True if a and b are equal.
attr_always_inline attr_header attr_hot
f32 cmp( f32 a, f32 b ) {
    return abs(a - b) <= F32_EPSILON;
}
/// @brief Compare two floating point numbers for equality.
/// @param a, b Floats to compare for equality.
/// @return True if a and b are equal.
attr_always_inline attr_header attr_hot
f64 cmp( f64 a, f64 b ) {
    return abs(a - b) <= F64_EPSILON;
}

/// @brief Normalize x to [0,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header attr_hot
f32 normalize( u8 x ) {
    return u8_normalize( x );
}
/// @brief Normalize x to [0,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header attr_hot
f32 normalize( u16 x ) {
    return u16_normalize( x );
}
/// @brief Normalize x to [0,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header attr_hot
f32 normalize( u32 x ) {
    return u32_normalize( x );
}
/// @brief Normalize x to [0,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header attr_hot
f32 normalize( u64 x ) {
    return u64_normalize( x );
}

/// @brief Normalize x to [-1,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header attr_hot
f32 normalize( i8 x ) {
    return i8_normalize( x );
}
/// @brief Normalize x to [-1,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header attr_hot
f32 normalize( i16 x ) {
    return i16_normalize( x );
}
/// @brief Normalize x to [-1,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header attr_hot
f32 normalize( i32 x ) {
    return i32_normalize( x );
}
/// @brief Normalize x to [-1,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header attr_hot
f32 normalize( i64 x ) {
    return i64_normalize( x );
}

/// @brief Truncate float to integer.
/// @param x Float to truncate.
/// @return Truncated value.
attr_always_inline attr_header attr_hot
i32 trunc( f32 x ) {
    return f32_trunc( x );
}
/// @brief Truncate float to integer.
/// @param x Float to truncate.
/// @return Truncated value.
attr_always_inline attr_header attr_hot
i64 trunc( f64 x ) {
    return f64_trunc( x );
}

/// @brief Floor float to integer.
/// @param x Float to floor.
/// @return Floored value.
attr_always_inline attr_header attr_hot
i32 floor( f32 x ) {
    return f32_floor( x );
}
/// @brief Floor float to integer.
/// @param x Float to floor.
/// @return Floored value.
attr_always_inline attr_header attr_hot
i64 floor( f64 x ) {
    return f64_floor( x );
}

/// @brief Ceil float to integer.
/// @param x Float to ceil.
/// @return Ceiled value.
attr_always_inline attr_header attr_hot
i32 ceil( f32 x ) {
    return f32_ceil( x );
}
/// @brief Ceil float to integer.
/// @param x Float to ceil.
/// @return Ceiled value.
attr_always_inline attr_header attr_hot
i64 ceil( f64 x ) {
    return f64_ceil( x );
}

/// @brief Round float to integer.
/// @param x Float to round.
/// @return Rounded value.
attr_always_inline attr_header attr_hot
i32 round( f32 x ) {
    return f32_round( x );
}
/// @brief Round float to integer.
/// @param x Float to round.
/// @return Rounded value.
attr_always_inline attr_header attr_hot
i64 round( f64 x ) {
    return f64_round( x );
}

/// @brief Get fractional part.
/// @param x Float.
/// @return Fractional part.
attr_always_inline attr_header attr_hot
f32 fract( f32 x ) {
    return f32_fract( x );
}
/// @brief Get fractional part.
/// @param x Float.
/// @return Fractional part.
attr_always_inline attr_header attr_hot
f64 fract( f64 x ) {
    return f64_fract( x );
}

/// @brief Swap endianness of unsigned 8-bit integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_always_inline attr_header attr_hot
u8 endian_swap( u8 x ) {
    return u8_endian_swap( x );
}
/// @brief Swap endianness of unsigned 16-bit integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_always_inline attr_header attr_hot
u16 endian_swap( u16 x ) {
    return u16_endian_swap( x );
}
/// @brief Swap endianness of unsigned 32-bit integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_always_inline attr_header attr_hot
u32 endian_swap( u32 x ) {
    return u32_endian_swap( x );
}
/// @brief Swap endianness of unsigned 64-bit integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_always_inline attr_header attr_hot
u64 endian_swap( u64 x ) {
    return u64_endian_swap( x );
}

/// @brief Check if float is NaN.
/// @param x Float to check.
/// @return
///     - @c true  : @c x is NaN.
///     - @c false : @c x is not NaN.
attr_always_inline attr_header attr_hot
bool isnan( f32 x ) {
    return f32_isnan( x );
}
/// @brief Check if float is NaN.
/// @param x Float to check.
/// @return
///     - @c true  : @c x is NaN.
///     - @c false : @c x is not NaN.
attr_always_inline attr_header attr_hot
bool isnan( f64 x ) {
    return f64_isnan( x );
}

/// @brief Check if float is infinite.
/// @param x Float to check.
/// @return
///     - @c true  : @c x is Infinite.
///     - @c false : @c x is not Infinite.
attr_always_inline attr_header attr_hot
bool isinf( f32 x ) {
    return f32_isinf( x );
}
/// @brief Check if float is infinite.
/// @param x Float to check.
/// @return
///     - @c true  : @c x is Infinite.
///     - @c false : @c x is not Infinite.
attr_always_inline attr_header attr_hot
bool isinf( f64 x ) {
    return f64_isinf( x );
}

/// @brief Floating-point modulus.
/// @param lhs Left hand side of modulo operation.
/// @param rhs Right hand side of modulo operation.
/// @return Result.
attr_always_inline attr_header attr_hot
f32 mod( f32 lhs, f32 rhs ) {
    return f32_mod( lhs, rhs );
}

/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Result.
attr_always_inline attr_header attr_hot
f32 lerp( f32 a, f32 b, f32 t ) {
    return f32_lerp( a, b, t );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Result.
attr_always_inline attr_header attr_hot
f32 mix( f32 a, f32 b, f32 t ) {
    return lerp( a, b, t );
}
/// @brief Inverse lerp.
/// @param a, b Range of interpolation.
/// @param v Value within range [a,b].
/// @return t value of v.
attr_always_inline attr_header attr_hot
f32 inverselerp( f32 a, f32 b, f32 v ) {
    return f32_inverselerp( a, b, v );
}
/// @brief Inverse lerp.
/// @param a, b Range of interpolation.
/// @param v Value within range [a,b].
/// @return t value of v.
attr_always_inline attr_header attr_hot
f32 inversemix( f32 a, f32 b, f32 v ) {
    return inverselerp( a, b, v );
}
/// @brief Remap value from range [in_min,in_max] to range [out_min,out_max].
/// @param in_min, in_max Input range.
/// @param out_min, out_max Output range.
/// @param v Value to remap.
/// @return Value remapped from input range to output range.
attr_always_inline attr_header attr_hot
f32 remap( f32 in_min, f32 in_max, f32 out_min, f32 out_max, f32 v ) {
    return f32_remap( in_min, in_max, out_min, out_max, v );
}

/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline attr_header attr_hot
f32 step( f32 edge, f32 x ) {
    return f32_step( edge, x );
}
/// @brief Smooth step interpolation.
/// @param edge0, edge1 Range of interpolation.
/// @param x            Where to interpolate to.
/// @return Result.
attr_always_inline attr_header attr_hot
f32 smoothstep( f32 edge0, f32 edge1, f32 x ) {
    return f32_smoothstep( edge0, edge1, x );
}
/// @brief Smoother step interpolation.
/// @param edge0, edge1 Range of interpolation.
/// @param x            Where to interpolate to.
/// @return Result.
attr_always_inline attr_header attr_hot
f32 smootherstep( f32 edge0, f32 edge1, f32 x ) {
    return f32_smootherstep( edge0, edge1, x );
}

#endif /* header guard */
