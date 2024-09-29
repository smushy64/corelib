#if !defined(CORE_MATH_MACROS_H)
#define CORE_MATH_MACROS_H
/**
 * @file   macros.h
 * @brief  Helper math macros.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 01, 2024
*/
#include "core/defines.h" // IWYU pragma: keep

#if defined(CORE_CPLUSPLUS)
    #include "core/cpp/math/macros.hpp"
#else
/// @brief Get the sign of a number.
/// @param x (any signed number) Number to get sign of.
/// @return 1 if positive, -1 if negative.
#define signum( x ) ( ( (x) > 0 ) - ( (x) < 0 ) )
/// @brief Get the absolute value of a signed number.
/// @param x (any signed number) Number to get absolute value of.
/// @return Absolute value of x.
#define absolute( x ) ( (x) < 0 ? -(x) : (x) )
/// @brief Get larger of two numbers.
/// @param x, y (any number) Numbers to compare.
/// @return Larger number.
#define max( x, y ) ((x) < (y) ? (y) : (x))
/// @brief Get smaller of two numbers.
/// @param x, y (any number) Numbers to compare.
/// @return Smaller number.
#define min( x, y ) ((x) < (y) ? (x) : (y))
/// @brief Clamp number between a minimum and maximum.
/// @param v (any number) Value to clamp.
/// @param min_ (any number) Minimum value, should be less than max_.
/// @param max_ (any number) Maximum value, should be greater than min_.
/// @return Value clamped to range min_ -> max_.
#define clamp( v, min_, max_ )\
    ( (v) < (min_) ? (min_) : ( (v) > (max_) ? (max_) : (v) ) )
/// @brief Clamp a floating point number between 0.0->1.0.
/// @param v (float) Value to clamp.
/// @return Value clamped to range 0.0 -> 1.0.
#define clamp01( v )\
    clamp( v, 0.0, 1.0 )

/// @brief Compare two floating point numbers for equality.
/// @param a, b (f32) Floats to compare for equality.
/// @return True if a and b are equal.
#define fcmp( a, b ) (absolute((a) - (b)) <= F32_EPSILON)
#endif /* C implementation */

#endif /* header guard */
