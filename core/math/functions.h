#if !defined(CORE_MATH_FUNCTIONS_H)
#define CORE_MATH_FUNCTIONS_H
/**
 * @file   functions.h
 * @brief  Math functions for integral types.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 28, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/constants.h"

/// @brief Check if integer is a power of 2.
/// @param x Integer to test.
/// @return True if integer is a power of 2.
#define is_power_of_2( x ) ( (x) && (!((x) & ((x) - 1))) )

/// @brief Check if float is NaN.
/// @param x Float to check.
/// @return
///     - @c true  : @c x is NaN.
///     - @c false : @c x is not NaN.
attr_always_inline
attr_header b32 is_nan( f32 x ) {
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
attr_always_inline
attr_header b32 is_nan64( f64 x ) {
    u64 bitpattern = *(u64*)&x;

    u64 exp = bitpattern & F64_EXPONENT_MASK;
    u64 man = bitpattern & F64_MANTISSA_MASK;

    return exp == F64_EXPONENT_MASK && man != 0;
}

/// @brief Calculate square root.
/// @param x Value to get square root of.
/// @return Square root.
attr_core_api f32 square_root( f32 x );
/// @brief Calculate reciprocal square root.
/// @param x Value to get reciprocal square root of.
/// @return Reciprocal square root.
attr_core_api f32 inv_square_root( f32 x );

/// @brief Calculate natural logarithm.
/// @param x Value to get natural logarithm of.
/// @return Natural logarithm.
attr_core_api f32 nat_log( f32 x );
/// @brief Calculate logarithm base 2.
/// @param x Value to get logarithm base 2 of.
/// @return Logarithm base 2.
attr_core_api f32 log_2( f32 x );
/// @brief Calculate logarithm base 10.
/// @param x Value to get logarithm base 10 of.
/// @return Logarithm base 10.
attr_core_api f32 log_10( f32 x );

/// @brief Raise base to the power of integer exponent.
/// @param base Number to raise.
/// @param exp Power to raise to.
/// @return Result.
attr_core_api f32 poweri( f32 base, i32 exp );
/// @brief Raise e to the power of x.
/// @param x Exponent.
/// @return Result.
attr_core_api f32 e_power( f32 x );
/// @brief Raise base to the power of exponent.
/// @param base Number to raise.
/// @param exp Power to raise to.
/// @return Result.
attr_always_inline
attr_header f32 power( f32 base, f32 exp ) {
    return e_power( nat_log( base ) * exp );
}

/// @brief Floating-point modulus.
/// @param lhs Left hand side of modulo operation.
/// @param rhs Right hand side of modulo operation.
/// @return Result.
attr_core_api f32 mod( f32 lhs, f32 rhs );

#endif /* header guard */
