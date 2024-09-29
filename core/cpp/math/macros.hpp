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

/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
constexpr attr_header u8 signum( u8 x ) {
    (void)x;
    return 1;
}
/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
constexpr attr_header u16 signum( u16 x ) {
    (void)x;
    return 1;
}
/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
constexpr attr_header u32 signum( u32 x ) {
    (void)x;
    return 1;
}
/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
constexpr attr_header u64 signum( u64 x ) {
    (void)x;
    return 1;
}
/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
constexpr attr_header i8 signum( i8 x ) {
    return (x > 0) - (x < 0);
}
/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
constexpr attr_header i16 signum( i16 x ) {
    return (x > 0) - (x < 0);
}
/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
constexpr attr_header i32 signum( i32 x ) {
    return (x > 0) - (x < 0);
}
/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
constexpr attr_header i64 signum( i64 x ) {
    return (x > 0) - (x < 0);
}
/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
constexpr attr_header f32 signum( f32 x ) {
    return (x > 0.0f) - (x < 0.0f);
}
/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
constexpr attr_header f64 signum( f64 x ) {
    return (x > 0.0) - (x < 0.0);
}

/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
constexpr attr_header u8 absolute( u8 x ) {
    return x;
}
/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
constexpr attr_header u16 absolute( u16 x ) {
    return x;
}
/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
constexpr attr_header u32 absolute( u32 x ) {
    return x;
}
/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
constexpr attr_header u64 absolute( u64 x ) {
    return x;
}
/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
constexpr attr_header i8 absolute( i8 x ) {
    return x < 0 ? -x : x;
}
/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
constexpr attr_header i16 absolute( i16 x ) {
    return x < 0 ? -x : x;
}
/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
constexpr attr_header i32 absolute( i32 x ) {
    return x < 0 ? -x : x;
}
/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
constexpr attr_header i64 absolute( i64 x ) {
    return x < 0 ? -x : x;
}
/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
constexpr attr_header f32 absolute( f32 x ) {
    return x < 0.0f ? -x : x;
}
/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
constexpr attr_header f64 absolute( f64 x ) {
    return x < 0.0 ? -x : x;
}
/// @brief Get larger of two numbers.
/// @param x, y (any number) Numbers to compare.
/// @return Larger number.
template<typename Number>
constexpr attr_header Number max( Number x, Number y ) {
    return x < y ? y : x;
}
/// @brief Get smaller of two numbers.
/// @param x, y (any number) Numbers to compare.
/// @return Smaller number.
template<typename Number>
constexpr attr_header Number min( Number x, Number y ) {
    return x < y ? x : y;
}
/// @brief Clamp number between a minimum and maximum.
/// @param v (any number) Value to clamp.
/// @param min (any number) Minimum value, should be less than max.
/// @param max (any number) Maximum value, should be greater than min.
/// @return Value clamped to range min -> max.
template<typename Number>
constexpr attr_header Number clamp( Number v, Number min, Number max ) {
    return v < min ? min : v > max ? max : v;
}
/// @brief Clamp a floating point number between 0.0->1.0.
/// @param v (float) Value to clamp.
/// @return Value clamped to range 0.0 -> 1.0.
constexpr attr_header f32 clamp01( f32 v ) {
    return clamp( v, 0.0f, 1.0f );
}
/// @brief Clamp a floating point number between 0.0->1.0.
/// @param v (float) Value to clamp.
/// @return Value clamped to range 0.0 -> 1.0.
constexpr attr_header f64 clamp01( f64 v ) {
    return clamp( v, 0.0, 1.0 );
}
/// @brief Compare two floating point numbers for equality.
/// @param a, b Floats to compare for equality.
/// @return True if a and b are equal.
constexpr attr_header b32 fcmp( f32 a, f32 b ) {
    return absolute( a - b ) <= F32_EPSILON;
}
/// @brief Compare two floating point numbers for equality.
/// @param a, b Floats to compare for equality.
/// @return True if a and b are equal.
constexpr attr_header b32 fcmp( f64 a, f64 b ) {
    return absolute( a - b ) <= F64_EPSILON;
}

#endif /* header guard */
