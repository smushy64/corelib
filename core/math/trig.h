#if !defined(CORE_MATH_TRIG_H)
#define CORE_MATH_TRIG_H
/**
 * @file   trig.h
 * @brief  Trigonometric functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 01, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/constants.h"

/// @brief Convert degrees to radians.
/// @param theta Angle in degrees.
/// @return Angle in radians.
attr_always_inline attr_header
f32 f32_radians( f32 theta ) {
    return theta * F32_TO_RAD;
}
/// @brief Convert degrees to radians.
/// @param theta Angle in degrees.
/// @return Angle in radians.
attr_always_inline attr_header
f64 f64_radians( f64 theta ) {
    return theta * F64_TO_RAD;
}
/// @brief Convert radians to degrees.
/// @param theta Angle in radians.
/// @return Angle in degrees.
attr_always_inline attr_header
f32 f32_degrees( f32 theta ) {
    return theta * F32_TO_DEG;
}
/// @brief Convert radians to degrees.
/// @param theta Angle in radians.
/// @return Angle in degrees.
attr_always_inline attr_header
f64 f64_degrees( f64 theta ) {
    return theta * F64_TO_DEG;
}
/// @brief Calculate sine of x.
/// @param angle Value to get sine of.
/// @return Sine of angle.
/// @see #sine_cosine() if both sine and cosine of x are needed.
attr_core_api
f32 f32_sin( f32 angle );
/// @brief Calculate cosine of x.
/// @param angle Value to get cosine of.
/// @return Cosine of angle.
/// @see #sine_cosine() if both sine and cosine of x are needed.
attr_core_api
f32 f32_cos( f32 angle );
/// @brief Calculate sine and cosine of x simultaneously.
/// @param      angle      Value to get sine and cosine of.
/// @param[out] out_sine   Sine of angle.
/// @param[out] out_cosine Cosine of angle.
attr_core_api
void f32_sincos( f32 angle, f32* out_sine, f32* out_cosine );
/// @brief Calculate tangent of x.
/// @param angle Value to get tangent of.
/// @return Tangent of angle.
/// @warning Returns NaN if cosine of angle is zero.
attr_always_inline attr_header
f32 f32_tan( f32 angle ) {
    f32 sine, cosine;
    f32_sincos( angle, &sine, &cosine );
    return cosine == 0.0f ? F32_NAN : sine / cosine;
}

/// @brief Calculate arcsine of x.
/// @param x Value to get arcsine of.
/// @return Arcsine of x.
attr_core_api
f32 f32_asin( f32 x );
/// @brief Calculate arcsine of x.
///
/// Does not produce NaN when outside valid range.
/// @param x Value to get arcsine of.
/// @return Arcsine of x clamped to -Pi -> Pi.
/// @see #F32_PI
/// @see #asin()
attr_always_inline attr_header
f32 f32_asin_real( f32 x ) {
    f32 abs  = x < 0 ? -x : x;
    f32 sign = x < 0 ? -1.0f : 1.0f;
    return abs >= 1.0f ?
        F32_HALF_PI * sign : f32_asin( x );
}
/// @brief Calculate arccosine of x.
/// @param x Value to get arccosine of.
/// @return Arccosine of x.
attr_always_inline attr_header
f32 f32_acos( f32 x ) {
    return -f32_asin( x ) + F32_HALF_PI;
}
/// @brief Calculate arctangent of x.
/// @param x Value to get arctangent of.
/// @return Arctangent of x.
attr_core_api
f32 f32_atan( f32 x );
/// @brief Calculate 2 component arctangent of y and x.
/// @param y Value to get arctangent of.
/// @param x Value to get arctangent of.
/// @return Arctangent of y and x.
attr_core_api
f32 f32_atan2( f32 y, f32 x );

/// @brief Wrap degrees between 0.0 -> 360.0
/// @param deg Degrees.
/// @return Degrees wrapped between 0.0 -> 360.0.
attr_core_api
f32 f32_wrap_degrees( f32 deg );
/// @brief Wrap radians between -Pi -> Pi
/// @param rad Radians.
/// @return Radians wrapped bnetween -Pi -> Pi.
/// @see #F32_PI
attr_core_api
f32 f32_wrap_radians( f32 rad );

#endif /* header guard */
