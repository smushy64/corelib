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

/// @brief Calculate sine of x.
/// @param x Value to get sine of.
/// @return Sine of x.
/// @see #sine_cosine() if both sine and cosine of x are needed.
attr_core_api f32 sine( f32 x );
/// @brief Calculate cosine of x.
/// @param x Value to get cosine of.
/// @return Cosine of x.
/// @see #sine_cosine() if both sine and cosine of x are needed.
attr_core_api f32 cosine( f32 x );
/// @brief Calculate sine and cosine of x simultaneously.
/// @param x Value to get sine and cosine of.
/// @param[out] out_sine Sine of x.
/// @param[out] out_cosine Cosine of x.
attr_core_api void sine_cosine( f32 x, f32* out_sine, f32* out_cosine );
/// @brief Calculate tangent of x.
/// @param x Value to get tangent of.
/// @return Tangent of x.
/// @warning Returns NAN if cosine of x is zero.
attr_always_inline
attr_header f32 tangent( f32 x ) {
    f32 sin, cos;
    sine_cosine( x, &sin, &cos );
    return cos == 0.0f ? F32_NAN : sin / cos;
}

/// @brief Calculate arcsine of x.
/// @param x Value to get arcsine of.
/// @return Arcsine of x.
attr_core_api f32 arc_sine( f32 x );
/// @brief Calculate arcsine of x.
///
/// Does not produce NaN when outside valid range.
/// @param x Value to get arcsine of.
/// @return Arcsine of x clamped to -Pi -> Pi.
/// @see #F32_PI
/// @see #arc_sine()
attr_always_inline
attr_header f32 arc_sine_real( f32 x ) {
    f32 abs  = x < 0 ? -x : x;
    f32 sign = x < 0 ? -1.0f : 1.0f;
    return abs >= 1.0f ?
        F32_HALF_PI * sign : arc_sine( x );
}
/// @brief Calculate arccosine of x.
/// @param x Value to get arccosine of.
/// @return Arccosine of x.
attr_always_inline
attr_header f32 arc_cosine( f32 x ) {
    return -arc_sine( x ) + F32_HALF_PI;
}
/// @brief Calculate arctangent of x.
/// @param x Value to get arctangent of.
/// @return Arctangent of x.
attr_core_api f32 arc_tangent( f32 x );
/// @brief Calculate 2 component arctangent of y and x.
/// @param y Value to get arctangent of.
/// @param x Value to get arctangent of.
/// @return Arctangent of y and x.
attr_core_api f32 arc_tangent2( f32 y, f32 x );

/// @brief Wrap degrees between 0.0 -> 360.0
/// @param deg Degrees.
/// @return Degrees wrapped between 0.0 -> 360.0.
attr_core_api f32 wrap_deg( f32 deg );
/// @brief Wrap radians between -Pi -> Pi
/// @param rad Radians.
/// @return Radians wrapped bnetween -Pi -> Pi.
/// @see #F32_PI
attr_core_api f32 wrap_rad( f32 rad );

#endif /* header guard */
