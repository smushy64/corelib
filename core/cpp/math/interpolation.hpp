#if !defined(CORE_CPP_MATH_INTERPOLATION_HPP)
#define CORE_CPP_MATH_INTERPOLATION_HPP
/**
 * @file   interpolation.hpp
 * @brief  C++ interpolation functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/

#if !defined(CORE_MATH_INTERPOLATION_H)
    #include "core/math/interpolation.h"
#endif

/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Result.
attr_header f32 lerp( f32 a, f32 b, f32 t ) {
    return lerpf( a, b, t );
}
/// @brief Inverse lerp.
/// @param a, b Range of interpolation.
/// @param v Value within range a -> b.
/// @return t value of v.
attr_header f32 inv_lerp( f32 a, f32 b, f32 t ) {
    return inv_lerpf( a, b, t );
}
/// @brief Smooth step interpolation.
/// @param a, b Range of interpolation.
/// @param t Where to interpolate to.
/// @return Result.
attr_header f32 smooth_step( f32 a, f32 b, f32 t ) {
    return smooth_stepf( a, b, t );
}
/// @brief Smoother step interpolation.
/// @param a, b Range of interpolation.
/// @param t Where to interpolate to.
/// @return Result.
attr_header f32 smoother_step( f32 a, f32 b, f32 t ) {
    return smoother_stepf( a, b, t );
}

#endif /* header guard */
