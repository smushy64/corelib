#if !defined(CORE_MATH_INTERPOLATION_H)
#define CORE_MATH_INTERPOLATION_H
/**
 * @file   interpolation.h
 * @brief  Interpolation functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 01, 2024
*/
#include "core/types.h"
#include "core/attributes.h"

/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Result.
attr_always_inline
attr_header f32 lerpf( f32 a, f32 b, f32 t ) {
    return ( 1.0f - t ) * a + b * t;
}
/// @brief Inverse lerp.
/// @param a, b Range of interpolation.
/// @param v Value within range a -> b.
/// @return t value of v.
attr_always_inline
attr_header f32 inv_lerp( f32 a, f32 b, f32 v ) {
    return ( v - a ) / ( b - a );
}
/// @brief Remap value from range in_min -> in_max to range out_min -> out_max.
/// @param in_min, in_max Input range.
/// @param out_min, out_max Output range.
/// @param v Value to remap.
/// @return Value remapped from input range to output range.
attr_always_inline
attr_header f32 remap(
    f32 in_min, f32 in_max, f32 out_min, f32 out_max, f32 v
) {
    f32 t = inv_lerp( in_min, in_max, v );
    return lerpf( out_min, out_max, t );
}
/// @brief Smooth step interpolation.
/// @param a, b Range of interpolation.
/// @param t Where to interpolate to.
/// @return Result.
attr_always_inline
attr_header f32 smooth_stepf( f32 a, f32 b, f32 t ) {
    return ( b - a ) * ( 3.0f - t * 2.0f ) * t * t + a;
}
/// @brief Smoother step interpolation.
/// @param a, b Range of interpolation.
/// @param t Where to interpolate to.
/// @return Result.
attr_always_inline
attr_header f32 smoother_stepf( f32 a, f32 b, f32 t ) {
    return ( b - a ) *
        ( ( t * ( t * 6.0f - 15.0f ) + 10.0f ) * t * t * t ) + a;
}

#endif /* header guard */
