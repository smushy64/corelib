#if !defined(CORE_CPP_MATH_TRIG_HPP) && defined(__cplusplus)
#define CORE_CPP_MATH_TRIG_HPP
/**
 * @file   trig.hpp
 * @brief  C++ Math: Trigonometric functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 29, 2025
*/
#if !defined(CORE_MATH_TRIG_H)
    #include "core/math/trig.h"
#endif
#include "core/attributes.h"

/// @brief Convert degrees to radians.
/// @param theta Angle in degrees.
/// @return Angle in radians.
attr_always_inline attr_header attr_hot
f32 radians( f32 theta ) {
    return f32_radians( theta );
}
/// @brief Convert degrees to radians.
/// @param theta Angle in degrees.
/// @return Angle in radians.
attr_always_inline attr_header attr_hot
f64 radians( f64 theta ) {
    return f64_radians( theta );
}
/// @brief Convert radians to degrees.
/// @param theta Angle in radians.
/// @return Angle in degrees.
attr_always_inline attr_header attr_hot
f32 degrees( f32 theta ) {
    return f32_degrees( theta );
}
/// @brief Convert radians to degrees.
/// @param theta Angle in radians.
/// @return Angle in degrees.
attr_always_inline attr_header attr_hot
f64 degrees( f64 theta ) {
    return f64_degrees( theta );
}
/// @brief Calculate sine of x.
/// @param x Value to get sine of.
/// @return Sine of angle.
/// @see #sincos() if both sine and cosine of x are needed.
attr_always_inline attr_header attr_hot
f32 sin( f32 x ) {
    return f32_sin( x );
}
/// @brief Calculate cosine of x.
/// @param x Value to get cosine of.
/// @return Cosine of angle.
/// @see #sincos() if both sine and cosine of x are needed.
attr_always_inline attr_header attr_hot
f32 cos( f32 x ) {
    return f32_cos( x );
}
/// @brief Calculate sine and cosine of x simultaneously.
/// @param      x          Value to get sine and cosine of.
/// @param[out] out_sine   Sine of angle.
/// @param[out] out_cosine Cosine of angle.
attr_always_inline attr_header attr_hot
void sincos( f32 x, f32* out_sine, f32* out_cosine ) {
    f32_sincos( x, out_sine, out_cosine );
}
/// @brief Calculate tangent of x.
/// @warning Returns NaN if cosine of angle is zero.
/// @param x Value to get tangent of.
/// @return Tangent of angle.
attr_always_inline attr_header attr_hot
f32 tan( f32 x ) {
    return f32_tan( x );
}

/// @brief Calculate arcsine of x.
/// @param x Value to get arcsine of.
/// @return Arcsine of x.
attr_always_inline attr_header attr_hot
f32 asin( f32 x ) {
    return f32_asin( x );
}
/// @brief Calculate arcsine of x.
///
/// Does not produce NaN when outside valid range.
/// @param x Value to get arcsine of.
/// @return Arcsine of x clamped to -Pi -> Pi.
/// @see #F32_PI
/// @see #asin()
attr_always_inline attr_header attr_hot
f32 asin_real( f32 x ) {
    return f32_asin_real( x );
}
/// @brief Calculate arccosine of x.
/// @param x Value to get arccosine of.
/// @return Arccosine of x.
attr_always_inline attr_header attr_hot
f32 acos( f32 x ) {
    return f32_acos( x );
}
/// @brief Calculate arctangent of x.
/// @param x Value to get arctangent of.
/// @return Arctangent of x.
attr_always_inline attr_header attr_hot
f32 atan( f32 x ) {
    return f32_atan( x );
}
/// @brief Calculate 2 component arctangent of y and x.
/// @param y Value to get arctangent of.
/// @param x Value to get arctangent of.
/// @return Arctangent of y and x.
attr_always_inline attr_header attr_hot
f32 atan2( f32 y, f32 x ) {
    return f32_atan2( y, x );
}

/// @brief Wrap degrees between 0.0 -> 360.0
/// @param degrees Degrees.
/// @return Degrees wrapped between 0.0 -> 360.0.
attr_always_inline attr_header attr_hot
f32 wrap_degrees( f32 degrees ) {
    return f32_wrap_degrees( degrees );
}
/// @brief Wrap radians between -Pi -> Pi
/// @param radians Radians.
/// @return Radians wrapped bnetween -Pi -> Pi.
/// @see #F32_PI
attr_always_inline attr_header attr_hot
f32 wrap_radians( f32 radians ) {
    return f32_wrap_radians( radians );
}

#endif /* header guard */
