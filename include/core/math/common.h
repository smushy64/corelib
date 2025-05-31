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
/// @param v    (any number) Value to clamp.
/// @param min_ (any number) Minimum value, should be less than max_.
/// @param max_ (any number) Maximum value, should be greater than min_.
/// @return Value clamped to range [min_,max_].
#define num_clamp( v, min_, max_ )\
    ( (v) < (min_) ? (min_) : ( (v) > (max_) ? (max_) : (v) ) )

/// @brief Compare two floating point numbers for equality.
/// @param a, b (f32) Floats to compare for equality.
/// @return True if a and b are equal.
attr_always_inline attr_header
b32 f32_cmp( f32 a, f32 b ) {
    return (num_abs((a) - (b)) <= F32_EPSILON);
}
/// @brief Compare two floating point numbers for equality.
/// @param a, b (f64) Floats to compare for equality.
/// @return True if a and b are equal.
attr_always_inline attr_header
b32 f64_cmp( f64 a, f64 b ) { 
    return (num_abs((a) - (b)) <= F64_EPSILON);
}

/// @brief Normalize x to [0,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f32 u8_normalize( u8 x ) {
    return (f32)x / (f32)U8_MAX;
}
/// @brief Normalize x to [0,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f32 u16_normalize( u16 x ) {
    return (f32)x / (f32)U16_MAX;
}
/// @brief Normalize x to [0,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f32 u32_normalize( u32 x ) {
    return (f32)x / (f32)U32_MAX;
}
/// @brief Normalize x to [0,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f64 u64_normalize( u64 x ) {
    return (f64)x / (f64)U64_MAX;
}

/// @brief Normalize x to [-1,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f32 i8_normalize( i8 x ) {
    return x < 0 ? -( (f32)x / (f32)I8_MIN ) : (f32)x / (f32)I8_MAX;
}
/// @brief Normalize x to [-1,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f32 i16_normalize( i16 x ) {
    return x < 0 ? -( (f32)x / (f32)I16_MIN ) : (f32)x / (f32)I16_MAX;
}
/// @brief Normalize x to [-1,1] range.
/// @param x Value to normalize.
/// @return Normalized value.
attr_always_inline attr_header
f32 i32_normalize( i32 x ) {
    return x < 0.0f ? -( (f32)x / (f32)I32_MIN ) : (f32)x / (f32)I32_MAX;
}
/// @brief Normalize x to [-1,1] range.
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
/// @param v Value within range [a,b].
/// @return t value of v.
attr_always_inline attr_header
f32 f32_inverselerp( f32 a, f32 b, f32 v ) {
    return ( v - a ) / ( b - a );
}
/// @brief Inverse lerp.
/// @param a, b Range of interpolation.
/// @param v Value within range [a,b].
/// @return t value of v.
attr_always_inline attr_header
f32 f32_inversemix( f32 a, f32 b, f32 v ) {
    return f32_inverselerp( a, b, v );
}
/// @brief Remap value from range [in_min,in_max] to range [out_min,out_max].
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

/// @brief Swap endianness of integer.
/// @param x Integer to swap endianness of.
/// @return Integer with swapped endianness.
#define endian_swap( x ) _Generic( (x), \
    u8:       u8_endian_swap, \
    u16:     u16_endian_swap, \
    u64:     u64_endian_swap, \
    default: u32_endian_swap  \
)(x)

// NOTE(alicia): C overloads for the following functions are defined in core/math/overload:
// - sign()
// - abs()
// - max()
// - min()
// - clamp()
// - cmp()
// - normalize()
// - trunc()
// - floor()
// - ceil()
// - round()
// - fract()
// - mod()
// - lerp/mix()
// - inverselerp/inversemix()
// - remap()
// - step()
// - smoothstep()
// - smootherstep()
// - isnan()
// - isinf()

#if !defined(CORE_CPP_MATH_COMMON_HPP)
    #include "core/cpp/math/common.hpp"
#endif

#if !defined(__cplusplus)

attr_header
i8 i8_max( i8 x, i8 y ) {
    return num_max(x, y);
}
attr_header
i16 i16_max( i16 x, i16 y ) {
    return num_max(x, y);
}
attr_header
i32 i32_max( i32 x, i32 y ) {
    return num_max(x, y);
}
attr_header
i64 i64_max( i64 x, i64 y ) {
    return num_max(x, y);
}
attr_header
u8 u8_max( u8 x, u8 y ) {
    return num_max(x, y);
}
attr_header
u16 u16_max( u16 x, u16 y ) {
    return num_max(x, y);
}
attr_header
u32 u32_max( u32 x, u32 y ) {
    return num_max(x, y);
}
attr_header
u64 u64_max( u64 x, u64 y ) {
    return num_max(x, y);
}
attr_header
f32 f32_max( f32 x, f32 y ) {
    return num_max(x, y);
}
attr_header
f64 f64_max( f64 x, f64 y ) {
    return num_max(x, y);
}

attr_header
i8 i8_min( i8 x, i8 y ) {
    return num_min(x, y);
}
attr_header
i16 i16_min( i16 x, i16 y ) {
    return num_min(x, y);
}
attr_header
i32 i32_min( i32 x, i32 y ) {
    return num_min(x, y);
}
attr_header
i64 i64_min( i64 x, i64 y ) {
    return num_min(x, y);
}
attr_header
u8 u8_min( u8 x, u8 y ) {
    return num_min(x, y);
}
attr_header
u16 u16_min( u16 x, u16 y ) {
    return num_min(x, y);
}
attr_header
u32 u32_min( u32 x, u32 y ) {
    return num_min(x, y);
}
attr_header
u64 u64_min( u64 x, u64 y ) {
    return num_min(x, y);
}
attr_header
f32 f32_min( f32 x, f32 y ) {
    return num_min(x, y);
}
attr_header
f64 f64_min( f64 x, f64 y ) {
    return num_min(x, y);
}

attr_header
i8 i8_clamp( i8 v, i8 min_, i8 max_ ) {
    return num_clamp( v, min_, max_ );
}
attr_header
i16 i16_clamp( i16 v, i16 min_, i16 max_ ) {
    return num_clamp( v, min_, max_ );
}
attr_header
i32 i32_clamp( i32 v, i32 min_, i32 max_ ) {
    return num_clamp( v, min_, max_ );
}
attr_header
i64 i64_clamp( i64 v, i64 min_, i64 max_ ) {
    return num_clamp( v, min_, max_ );
}
attr_header
u8 u8_clamp( u8 v, u8 min_, u8 max_ ) {
    return num_clamp( v, min_, max_ );
}
attr_header
u16 u16_clamp( u16 v, u16 min_, u16 max_ ) {
    return num_clamp( v, min_, max_ );
}
attr_header
u32 u32_clamp( u32 v, u32 min_, u32 max_ ) {
    return num_clamp( v, min_, max_ );
}
attr_header
u64 u64_clamp( u64 v, u64 min_, u64 max_ ) {
    return num_clamp( v, min_, max_ );
}
attr_header
f32 f32_clamp( f32 v, f32 min_, f32 max_ ) {
    return num_clamp( v, min_, max_ );
}
attr_header
f64 f64_clamp( f64 v, f64 min_, f64 max_ ) {
    return num_clamp( v, min_, max_ );
}

attr_header
i8 i8_sign( i8 x ) {
    return num_sign(x);
}
attr_header
i16 i16_sign( i16 x ) {
    return num_sign(x);
}
attr_header
i32 i32_sign( i32 x ) {
    return num_sign(x);
}
attr_header
i64 i64_sign( i64 x ) {
    return num_sign(x);
}
attr_header
u8 u8_sign( u8 x ) {
    (void)(x);
    return 1;
}
attr_header
u16 u16_sign( u16 x ) {
    (void)(x);
    return 1;
}
attr_header
u32 u32_sign( u32 x ) {
    (void)(x);
    return 1;
}
attr_header
u64 u64_sign( u64 x ) {
    (void)(x);
    return 1;
}
attr_header
f32 f32_sign( f32 x ) {
    return num_sign(x);
}
attr_header
f64 f64_sign( f64 x ) {
    return num_sign(x);
}

attr_header
i8 i8_abs( i8 x ) {
    return num_abs(x);
}
attr_header
i16 i16_abs( i16 x ) {
    return num_abs(x);
}
attr_header
i32 i32_abs( i32 x ) {
    return num_abs(x);
}
attr_header
i64 i64_abs( i64 x ) {
    return num_abs(x);
}
attr_header
u8 u8_abs( u8 x ) {
    return x;
}
attr_header
u16 u16_abs( u16 x ) {
    return x;
}
attr_header
u32 u32_abs( u32 x ) {
    return x;
}
attr_header
u64 u64_abs( u64 x ) {
    return x;
}
attr_header
f32 f32_abs( f32 x ) {
    return num_abs(x);
}
attr_header
f64 f64_abs( f64 x ) {
    return num_abs(x);
}

#endif

#endif /* header guard */
