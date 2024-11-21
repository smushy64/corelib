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
attr_header f32 f32_lerp( f32 a, f32 b, f32 t ) {
    return ( 1.0f - t ) * a + b * t;
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Result.
attr_always_inline
attr_header f32 f32_mix( f32 a, f32 b, f32 t ) {
    return ( 1.0f - t ) * a + b * t;
}
/// @brief Inverse lerp.
/// @param a, b Range of interpolation.
/// @param v Value within range a -> b.
/// @return t value of v.
attr_always_inline
attr_header f32 f32_inverselerp( f32 a, f32 b, f32 v ) {
    return ( v - a ) / ( b - a );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Result.
attr_always_inline
attr_header f32 f32_inversemix( f32 a, f32 b, f32 v ) {
    return ( v - a ) / ( b - a );
}
/// @brief Remap value from range in_min -> in_max to range out_min -> out_max.
/// @param in_min, in_max Input range.
/// @param out_min, out_max Output range.
/// @param v Value to remap.
/// @return Value remapped from input range to output range.
attr_always_inline
attr_header f32 f32_remap(
    f32 in_min, f32 in_max, f32 out_min, f32 out_max, f32 v
) {
    f32 t = f32_inversemix( in_min, in_max, v );
    return f32_mix( out_min, out_max, t );
}
/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline
attr_header f32 f32_step( f32 edge, f32 x ) {
    return x < edge ? 0.0f : 1.0f;
}
/// @brief Smooth step interpolation.
/// @param edge0, edge1 Range of interpolation.
/// @param x            Where to interpolate to.
/// @return Result.
attr_always_inline
attr_header f32 f32_smoothstep( f32 edge0, f32 edge1, f32 x ) {
    return ( edge1 - edge0 ) * ( 3.0f - x * 2.0f ) * x * x + edge0;
}
/// @brief Smoother step interpolation.
/// @param edge0, edge1 Range of interpolation.
/// @param x            Where to interpolate to.
/// @return Result.
attr_always_inline
attr_header f32 f32_smootherstep( f32 edge0, f32 edge1, f32 x ) {
    return ( edge1 - edge0 ) *
        ( ( x * ( x * 6.0f - 15.0f ) + 10.0f ) * x * x * x ) + edge0;
}

#endif /* header guard */
