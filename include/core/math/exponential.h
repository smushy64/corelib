#if !defined(CORE_MATH_EXPONENTIAL_H)
#define CORE_MATH_EXPONENTIAL_H
/**
 * @file   exponential.h
 * @brief  Exponential functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   December 02, 2024
*/
#include "core/types.h"
#include "core/attributes.h"

/// @brief Check if integer is a power of 2.
/// @param x Integer to test.
/// @return True if integer is a power of 2.
#define is_power_of_2( x ) ( (x) && (!((x) & ((x) - 1))) )

/// @brief Calculate square root.
/// @param x Value to get square root of.
/// @return Square root.
attr_core_api
f32 f32_sqrt( f32 x );
/// @brief Calculate reciprocal square root.
/// @param x Value to get reciprocal square root of.
/// @return Reciprocal square root.
attr_core_api
f32 f32_inversesqrt( f32 x );

/// @brief Calculate cube root.
/// @param x Value to get cube root of.
/// @return Cube root.
attr_core_api
f32 f32_cbrt( f32 x );

/// @brief Calculate natural logarithm.
/// @param x Value to get natural logarithm of.
/// @return Natural logarithm.
attr_core_api
f32 f32_ln( f32 x );
/// @brief Calculate logarithm base 2.
/// @param x Value to get logarithm base 2 of.
/// @return Logarithm base 2.
attr_core_api
f32 f32_log2( f32 x );
/// @brief Calculate logarithm base 10.
/// @param x Value to get logarithm base 10 of.
/// @return Logarithm base 10.
attr_core_api
f32 f32_log10( f32 x );

/// @brief Raise base to the power of integer exponent.
/// @param base Number to raise.
/// @param exp  Power to raise to.
/// @return Result.
attr_core_api
f32 f32_powi( f32 base, i32 exp );
/// @brief Raise e to the power of x.
/// @param x Exponent.
/// @return Result.
attr_core_api
f32 f32_exp( f32 x );
/// @brief Raise base to the power of exponent.
/// @param base Number to raise.
/// @param exp  Power to raise to.
/// @return Result.
attr_always_inline attr_header attr_hot
f32 f32_pow( f32 base, f32 exp ) {
    return f32_exp( f32_ln( base ) * exp );
}
/// @brief Raise 2 to the power of exponent.
/// @param x Exponent.
/// @return 2^x
attr_always_inline attr_header attr_hot
f32 f32_exp2( f32 x ) {
    return f32_pow( 2.0f, x );
}

#if !defined(CORE_CPP_MATH_EXPONENTIAL_HPP)
    #include "core/cpp/math/exponential.hpp"
#endif

#endif /* header guard */
