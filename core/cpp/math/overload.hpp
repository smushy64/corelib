#if !defined(CORE_CPP_MATH_OVERLOAD_HPP)
#define CORE_CPP_MATH_OVERLOAD_HPP
/**
 * @file   overload.hpp
 * @brief  C++ function overloads.
 * @note
 *
 *         Vector2/3/4, Quaternion and Matrix2/3/4 functions are
 *         defined in their own core/cpp/math headers.
 *
 *         Macros are defined in core/cpp/math/macros.hpp
 *
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   November 20, 2024
*/
// IWYU pragma: begin_exports
#include "core/defines.h"
#include "core/types.h"
#include "core/math/trig.h"
#include "core/math/functions.h"
#include "core/math/interpolation.h"
// IWYU pragma: end_exports

attr_always_inline attr_header
f32 radians( f32 theta ) {
    return f32_radians( theta );
}
attr_always_inline attr_header
f64 radians( f64 theta ) {
    return f64_radians( theta );
}
attr_always_inline attr_header
f32 degrees( f32 theta ) {
    return f32_degrees( theta );
}
attr_always_inline attr_header
f64 degrees( f64 theta ) {
    return f64_degrees( theta );
}
attr_always_inline attr_header
f32 sin( f32 angle ) {
    return f32_sin( angle );
}
attr_always_inline attr_header
f32 cos( f32 angle ) {
    return f32_cos( angle );
}
attr_always_inline attr_header
void sincos( f32 angle, f32* out_sine, f32* out_cosine ) {
    f32_sincos( angle, out_sine, out_cosine );
}
attr_always_inline attr_header
f32 tan( f32 angle ) {
    return f32_tan( angle );
}

attr_always_inline attr_header
f32 asin( f32 angle ) {
    return f32_asin( angle );
}
attr_always_inline attr_header
f32 asin_real( f32 angle ) {
    return f32_asin_real( angle );
}
attr_always_inline attr_header
f32 acos( f32 angle ) {
    return f32_acos( angle );
}
attr_always_inline attr_header
f32 atan( f32 angle ) {
    return f32_atan( angle );
}
attr_always_inline attr_header
f32 atan2( f32 y, f32 x ) {
    return f32_atan2( y, x );
}

attr_always_inline attr_header
f32 wrap_degrees( f32 deg ) {
    return f32_wrap_degrees( deg );
}
attr_always_inline attr_header
f32 wrap_radians( f32 rad ) {
    return f32_wrap_radians( rad );
}

attr_always_inline attr_header
b32 isnan( f32 x ) {
    return f32_isnan( x );
}
attr_always_inline attr_header
b32 isnan( f64 x ) {
    return f64_isnan( x );
}
attr_always_inline attr_header
b32 isinf( f32 x ) {
    return f32_isinf( x );
}
attr_always_inline attr_header
b32 isinf( f64 x ) {
    return f64_isinf( x );
}
attr_always_inline attr_header
f32 sqrt( f32 x ) {
    return f32_sqrt( x );
}
attr_always_inline attr_header
f32 inversesqrt( f32 x ) {
    return f32_inversesqrt( x );
}
attr_always_inline attr_header
f32 nat_log( f32 x ) {
    return f32_nat_log( x );
}
attr_always_inline attr_header
f32 log_2( f32 x ) {
    return f32_log_2( x );
}
attr_always_inline attr_header
f32 log_10( f32 x ) {
    return f32_log_10( x );
}
attr_always_inline attr_header
f32 pow( f32 base, f32 exp ) {
    return f32_power( base, exp );
}
attr_always_inline attr_header
f32 pow( f32 base, i32 exp ) {
    return f32_power_i32( base, exp );
}
attr_always_inline attr_header
f32 e_pow( f32 x ) {
    return f32_e_power( x );
}
attr_always_inline attr_header
f32 mod( f32 lhs, f32 rhs ) {
    return f32_mod( lhs, rhs );
}
attr_always_inline attr_header
f32 lerp( f32 a, f32 b, f32 t ) {
    return f32_lerp( a, b, t );
}
attr_always_inline attr_header
f32 mix( f32 a, f32 b, f32 t ) {
    return f32_mix( a, b, t );
}
attr_always_inline attr_header
f32 inverselerp( f32 a, f32 b, f32 v ) {
    return f32_inverselerp( a, b, v );
}
attr_always_inline attr_header
f32 inversemix( f32 a, f32 b, f32 v ) {
    return f32_inversemix( a, b, v );
}
attr_always_inline attr_header
f32 remap( f32 in_min, f32 in_max, f32 out_min, f32 out_max, f32 v ) {
    return f32_remap( in_min, in_max, out_min, out_max, v );
}
attr_always_inline attr_header
f32 step( f32 edge, f32 x ) {
    return f32_step( edge, x );
}
attr_always_inline attr_header
f32 smoothstep( f32 edge0, f32 edge1, f32 x ) {
    return f32_smoothstep( edge0, edge1, x );
}
attr_always_inline attr_header
f32 smootherstep( f32 edge0, f32 edge1, f32 x ) {
    return f32_smootherstep( edge0, edge1, x );
}

#endif /* header guard */
