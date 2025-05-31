#if !defined(CORE_CPP_MATH_EXPONENTIAL_HPP) && defined(__cplusplus)
#define CORE_CPP_MATH_EXPONENTIAL_HPP
/**
 * @file   exponential.hpp
 * @brief  C++ Math: Exponential functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 29, 2025
*/
#if !defined(CORE_MATH_EXPONENTIAL_H)
    #include "core/math/exponential.h" // IWYU pragma: export
#endif
#include "core/types.h"
#include "core/attributes.h"
#include "core/macros.h"

#undef is_power_of_2

/// @brief Check if integer is a power of 2.
/// @param x Integer to test.
/// @return True if integer is a power of 2.
template<typename Integer>
attr_always_inline attr_header attr_hot
bool is_power_of_2( Integer x ) {
    Integer integer_test = x % 2;
    unused( integer_test );
    return x && ( !(x & (x - 1)) );
}

/// @brief Calculate square root.
/// @param x Value to get square root of.
/// @return Square root.
attr_always_inline attr_header attr_hot
f32 sqrt( f32 x ) {
    return f32_sqrt( x );
}
/// @brief Calculate reciprocal square root.
/// @param x Value to get reciprocal square root of.
/// @return Reciprocal square root.
attr_always_inline attr_header attr_hot
f32 inversesqrt( f32 x ) {
    return f32_inversesqrt( x );
}

/// @brief Calculate cube root.
/// @param x Value to get cube root of.
/// @return Cube root.
attr_always_inline attr_header attr_hot
f32 cbrt( f32 x ) {
    return f32_cbrt( x );
}

/// @brief Calculate natural logarithm.
/// @param x Value to get natural logarithm of.
/// @return Natural logarithm.
attr_always_inline attr_header attr_hot
f32 ln( f32 x ) {
    return f32_ln( x );
}
/// @brief Calculate logarithm base 2.
/// @param x Value to get logarithm base 2 of.
/// @return Logarithm base 2.
attr_always_inline attr_header attr_hot
f32 log2( f32 x ) {
    return f32_log2( x );
}
/// @brief Calculate logarithm base 10.
/// @param x Value to get logarithm base 10 of.
/// @return Logarithm base 10.
attr_always_inline attr_header attr_hot
f32 log10( f32 x ) {
    return f32_log10( x );
}

/// @brief Raise base to the power of integer exponent.
/// @param base Number to raise.
/// @param exp  Power to raise to.
/// @return Result.
attr_always_inline attr_header attr_hot
f32 powi( f32 base, i32 exp ) {
    return f32_powi( base, exp );
}
/// @brief Raise base to the power of exponent.
/// @param base Number to raise.
/// @param exp  Power to raise to.
/// @return Result.
attr_always_inline attr_header attr_hot
f32 pow( f32 base, f32 exp ) {
    return f32_pow( base, exp );
}

/// @brief Raise e to the power of x.
/// @param x Exponent.
/// @return Result.
attr_always_inline attr_header attr_hot
f32 exp( f32 x ) {
    return f32_exp( x );
}
/// @brief Raise 2 to the power of exponent.
/// @param x Exponent.
/// @return 2^x
attr_always_inline attr_header attr_hot
f32 exp2( f32 x ) {
    return f32_exp2( x );
}

#endif /* header guard */
