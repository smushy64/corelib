#if !defined(CORE_MATH_COMMON_H)
#define CORE_MATH_COMMON_H
/**
 * @file   common.h
 * @brief  Common math functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   December 02, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/constants.h"

#if defined(CORE_CPLUSPLUS)
    #include "core/cpp/math/common.hpp"
#else /* C++ only functions */
/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
#define num_sign( x ) ( ( (x) > 0 ) - ( (x) < 0 ) )
/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
#define num_abs( x ) ( (x) < 0 ? -(x) : (x) )
/// @brief Get larger of two numbers.
/// @param x, y (any number) Numbers to compare.
/// @return Larger number.
#define num_max( x, y ) ((x) < (y) ? (y) : (x))
/// @brief Get smaller of two numbers.
/// @param x, y (any number) Numbers to compare.
/// @return Smaller number.
#define num_min( x, y ) ((x) < (y) ? (x) : (y))
/// @brief Clamp number between a minimum and maximum.
/// @param v (any number) Value to clamp.
/// @param min_ (any number) Minimum value, should be less than max_.
/// @param max_ (any number) Maximum value, should be greater than min_.
/// @return Value clamped to range min_ -> max_.
#define num_clamp( v, min_, max_ )\
    ( (v) < (min_) ? (min_) : ( (v) > (max_) ? (max_) : (v) ) )

/// @brief Compare two floating point numbers for equality.
/// @param a, b (f32) Floats to compare for equality.
/// @return True if a and b are equal.
#define f32_cmp( a, b ) (num_abs((a) - (b)) <= F32_EPSILON)
/// @brief Compare two floating point numbers for equality.
/// @param a, b (f32) Floats to compare for equality.
/// @return True if a and b are equal.
#define f64_cmp( a, b ) (num_abs((a) - (b)) <= F64_EPSILON)

/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f32 u8_normalize( u8 x ) {
    return (f32)x / (f32)U8_MAX;
}
/// @brief Normalize x to -1 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f32 i8_normalize( i8 x ) {
    return x < 0 ? -( (f32)x / (f32)I8_MIN ) : (f32)x / (f32)I8_MAX;
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f32 u16_normalize( u16 x ) {
    return (f32)x / (f32)U16_MAX;
}
/// @brief Normalize x to -1 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f32 i16_normalize( i16 x ) {
    return x < 0 ? -( (f32)x / (f32)I16_MIN ) : (f32)x / (f32)I16_MAX;
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f32 u32_normalize( u32 x ) {
    return (f32)x / (f32)U32_MAX;
}
/// @brief Normalize x to -1 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f32 i32_normalize( i32 x ) {
    return x < 0.0f ? -( (f32)x / (f32)I32_MIN ) : (f32)x / (f32)I32_MAX;
}
/// @brief Normalize x to 0 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f64 u64_normalize( u64 x ) {
    return (f64)x / (f64)U64_MAX;
}
/// @brief Normalize x to -1 -> 1 range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f64 i64_normalize( i64 x ) {
    return x < 0 ? -( (f64)x / (f64)I64_MIN ) : (f64)x / (f64)I64_MAX;
}

/// @brief Truncate float to integer.
/// @param x Float to truncate.
/// @return Truncated value.
attr_always_inline attr_header
i32 f32_trunc( f32 x ) {
    return (i32)x;
}
/// @brief Truncate float to integer.
/// @param x Float to truncate.
/// @return Truncated value.
attr_always_inline attr_header
i64 f64_trunc( f64 x ) {
    return (i64)x;
}
/// @brief Floor float to integer.
/// @param x Float to floor.
/// @return Floored value.
attr_always_inline attr_header
i32 f32_floor( f32 x ) {
    return x < 0 ? f32_trunc( x - 1.0f ) : f32_trunc( x );
}
/// @brief Floor float to integer.
/// @param x Float to floor.
/// @return Floored value.
attr_always_inline attr_header
i64 f64_floor( f64 x ) {
    return x < 0 ? f64_trunc( x - 1.0 ) : f64_trunc( x );
}
/// @brief Ceil float to integer.
/// @param x Float to ceil.
/// @return Ceiled value.
attr_always_inline attr_header
i32 f32_ceil( f32 x ) {
    return x < 0 ? f32_trunc( x ) : f32_trunc( x + 1.0f );
}
/// @brief Ceil float to integer.
/// @param x Float to ceil.
/// @return Ceiled value.
attr_always_inline attr_header
i64 f64_ceil( f64 x ) {
    return x < 0 ? f64_trunc( x ) : f64_trunc( x + 1.0 );
}
/// @brief Round float to integer.
/// @param x Float to round.
/// @return Rounded value.
attr_always_inline attr_header
i32 f32_round( f32 x ) {
    return x < 0 ? f32_trunc( x - 0.5f ) : f32_trunc( x + 0.5f );
}
/// @brief Round float to integer.
/// @param x Float to round.
/// @return Rounded value.
attr_always_inline attr_header
i64 f64_round( f64 x ) {
    return x < 0 ? f64_trunc( x - 0.5 ) : f64_trunc( x + 0.5 );
}
/// @brief Get fractional part.
/// @param x Float.
/// @return Fractional part.
attr_always_inline attr_header
f32 f32_fract( f32 x ) {
    return x - (f32)f32_floor(x);
}
/// @brief Get fractional part.
/// @param x Float.
/// @return Fractional part.
attr_always_inline attr_header
f64 f64_fract( f64 x ) {
    return x - (f64)f64_floor(x);
}
/// @brief Swap endianness of unsigned 8-bit integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_always_inline attr_header
u8 u8_endian_swap( u8 x ) {
    return x;
}
/// @brief Swap endianness of unsigned 16-bit integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_always_inline attr_header
u16 u16_endian_swap( u16 x ) {
    return (( x & 0xFF00 ) >> 8) | ((x & 0x00FF) << 8);
}
/// @brief Swap endianness of unsigned 32-bit integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_always_inline attr_header
u32 u32_endian_swap( u32 x ) {
    return
        ((x & 0xFF000000) >> 24u) |
        ((x & 0x00FF0000) >> 8u)  |
        ((x & 0x0000FF00) << 8u)  |
        ((x & 0x000000FF) << 24u);
}
/// @brief Swap endianness of unsigned 64-bit integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
attr_always_inline attr_header
u64 u64_endian_swap( u64 x ) {
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
#endif /* C only functions */

/// @brief Check if float is NaN.
/// @param x Float to check.
/// @return
///     - @c true  : @c x is NaN.
///     - @c false : @c x is not NaN.
attr_always_inline attr_header
b32 f32_isnan( f32 x ) {
    u32 bitpattern = *(u32*)&x;

    u32 exp = bitpattern & F32_EXPONENT_MASK;
    u32 man = bitpattern & F32_MANTISSA_MASK;

    return exp == F32_EXPONENT_MASK && man != 0;
}
/// @brief Check if float is NaN.
/// @param x Float to check.
/// @return
///     - @c true  : @c x is NaN.
///     - @c false : @c x is not NaN.
attr_always_inline attr_header
b32 f64_isnan( f64 x ) {
    u64 bitpattern = *(u64*)&x;

    u64 exp = bitpattern & F64_EXPONENT_MASK;
    u64 man = bitpattern & F64_MANTISSA_MASK;

    return exp == F64_EXPONENT_MASK && man != 0;
}
/// @brief Check if float is infinite.
/// @param x Float to check.
/// @return
///     - @c true  : @c x is Infinite.
///     - @c false : @c x is not Infinite.
attr_always_inline attr_header
b32 f32_isinf( f32 x ) {
    u32 bitpattern = *(u32*)&x;

    u32 exp = (bitpattern >> 23) & 0xFF;
    u32 man = bitpattern & 0x7FFFFF;

    return exp == 0xFF && man == 0;
}
/// @brief Check if float is infinite.
/// @param x Float to check.
/// @return
///     - @c true  : @c x is Infinite.
///     - @c false : @c x is not Infinite.
attr_always_inline attr_header
b32 f64_isinf( f64 x ) {
    u64 bitpattern = *(u64*)&x;

    u64 exp = (bitpattern >> 52) & 0x7FF;
    u64 man = bitpattern & 0xFFFFFFFFFFFFF;

    return exp == 0x7FF && man == 0;
}

/// @brief Floating-point modulus.
/// @param lhs Left hand side of modulo operation.
/// @param rhs Right hand side of modulo operation.
/// @return Result.
attr_core_api
f32 f32_mod( f32 lhs, f32 rhs );

/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Result.
attr_always_inline attr_header
f32 f32_lerp( f32 a, f32 b, f32 t ) {
    return ( 1.0f - t ) * a + b * t;
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Result.
attr_always_inline attr_header
f32 f32_mix( f32 a, f32 b, f32 t ) {
    return f32_lerp( a, b, t );
}
/// @brief Inverse lerp.
/// @param a, b Range of interpolation.
/// @param v Value within range a -> b.
/// @return t value of v.
attr_always_inline attr_header
f32 f32_inverselerp( f32 a, f32 b, f32 v ) {
    return ( v - a ) / ( b - a );
}
/// @brief Inverse lerp.
/// @param a, b Range of interpolation.
/// @param v Value within range a -> b.
/// @return t value of v.
attr_always_inline attr_header
f32 f32_inversemix( f32 a, f32 b, f32 v ) {
    return ( v - a ) / ( b - a );
}
/// @brief Remap value from range in_min -> in_max to range out_min -> out_max.
/// @param in_min, in_max Input range.
/// @param out_min, out_max Output range.
/// @param v Value to remap.
/// @return Value remapped from input range to output range.
attr_always_inline attr_header
f32 f32_remap(
    f32 in_min, f32 in_max, f32 out_min, f32 out_max, f32 v
) {
    f32 t = f32_inversemix( in_min, in_max, v );
    return f32_mix( out_min, out_max, t );
}
/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline attr_header
f32 f32_step( f32 edge, f32 x ) {
    return x < edge ? 0.0f : 1.0f;
}
/// @brief Smooth step interpolation.
/// @param edge0, edge1 Range of interpolation.
/// @param x            Where to interpolate to.
/// @return Result.
attr_always_inline attr_header
f32 f32_smoothstep( f32 edge0, f32 edge1, f32 x ) {
    return ( edge1 - edge0 ) * ( 3.0f - x * 2.0f ) * x * x + edge0;
}
/// @brief Smoother step interpolation.
/// @param edge0, edge1 Range of interpolation.
/// @param x            Where to interpolate to.
/// @return Result.
attr_always_inline attr_header
f32 f32_smootherstep( f32 edge0, f32 edge1, f32 x ) {
    return ( edge1 - edge0 ) *
        ( ( x * ( x * 6.0f - 15.0f ) + 10.0f ) * x * x * x ) + edge0;
}


#endif /* header guard */
