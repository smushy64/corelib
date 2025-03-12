#if !defined(CORE_MATH_OVERLOAD_H)
#define CORE_MATH_OVERLOAD_H
/**
 * @file   overload.h
 * @brief  C11 generic macro overloads.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   November 19, 2024
*/
// IWYU pragma: begin_exports
#include "core/defines.h"
#include "core/types.h"
// IWYU pragma: end_exports

#define radians( theta ) _Generic( (theta),\
    double:  f64_radians,\
    default: f32_radians\
)( theta )
#define degrees( theta ) _Generic( (theta),\
    double:  f64_degrees,\
    default: f32_degrees\
)( theta )
#define sin( angle ) _Generic( (angle),\
    default: f32_sin\
)( angle )
#define cos( angle ) _Generic(\
    default: f32_cos\
)( angle )
#define sincos( angle, out_sine, out_cosine ) _Generic( (angle),\
    default: f32_sincos\
)( angle, out_sine, out_cosine )
#define tan( angle ) _Generic( (angle),\
    default: f32_tan\
)( angle )

#define asin( angle ) _Generic( (angle),\
    default: f32_asin\
)( angle )
#define asin_real( angle ) _Generic( (angle),\
    default: f32_asin_real\
)( angle )
#define acos( angle ) _Generic(\
    default: f32_acos\
)( angle )
#define atan( angle ) _Generic( (angle),\
    default: f32_atan\
)( angle )
#define atan2( y, x ) _Generic( (y),\
    default: f32_atan2\
)( y, x )

#define wrap_degrees( deg ) _Generic( (deg),\
    default: f32_wrap_degrees\
)( deg )
#define wrap_radians( rad ) _Generic( (rad),\
    default: f32_wrap_radians\
)( rad )

#define isnan( x ) _Generic( (x),\
    double:  f64_isnan,\
    default: f32_isnan\
)(x)
#define isinf( x ) _Generic( (x),\
    double:  f64_isinf,\
    default: f32_isinf\
)(x)
#define sqrt( x ) _Generic( (x),\
    default: f32_sqrt\
)(x)
#define inversesqrt( x ) _Generic( (x),\
    default: f32_inversesqrt\
)(x)
#define cbrt( x ) _Generic( (x),\
    default: f32_cbrt\
)(x)
#define ln( x ) _Generic( (x),\
    default: f32_ln\
)(x)
#define log2( x ) _Generic( (x),\
    default: f32_log2\
)(x)
#define log10( x ) _Generic( (x),\
    default: f32_log10\
)(x)
#define pow( base, exp ) _Generic( (exp),\
    double:  f32_pow,\
    default: f32_pow\
)( base, exp )
#define powi( base, exp ) _Generic( (exp),\
    default: f32_powi\
)( base, exp )
#define exp( x ) _Generic( (x),\
    default: f32_exp\
)(x)
#define mod( lhs, rhs ) _Generic( (lhs),\
    default: f32_mod\
)(lhs, rhs)

#define lerp( a, b, t ) _Generic( (a),\
    vec2:    vec2_lerp,\
    vec3:    vec3_lerp,\
    vec4:    vec4_lerp,\
    quat:    quat_lerp,\
    default: f32_lerp\
)(a, b, t)
#define mix( a, b, t ) lerp( a, b, t )
#define inverselerp( a, b, t ) _Generic( (a),\
    default: f32_inverselerp\
)(a, b, t)
#define inversemix( a, b, t ) inverselerp( a, b, t )
#define remap( in_min, in_max, out_min, out_max, v ) _Generic( (in_min),\
    default: f32_remap\
)( in_min, in_max, out_min, out_max, v )
#define step( edge, x ) _Generic( (edge),\
    vec2:    vec2_step,\
    vec3:    vec3_step,\
    vec4:    vec4_step,\
    default: f32_step\
)( edge, x )
#define smoothstep( edge0, edge1, x ) _Generic( (edge0),\
    vec2:    vec2_smoothstep,\
    vec3:    vec3_smoothstep,\
    vec4:    vec4_smoothstep,\
    quat:    quat_smoothstep,\
    default: f32_smoothstep\
)(edge0, edge1, x)
#define smootherstep( edge0, edge1, x ) _Generic( (edge0),\
    vec2:    vec2_smootherstep,\
    vec3:    vec3_smootherstep,\
    vec4:    vec4_smootherstep,\
    quat:    quat_smootherstep,\
    default: f32_smootherstep\
)(edge0, edge1, x)

#define neg( x ) _Generic( (x),\
    vec2:  vec2_neg,\
    vec3:  vec3_neg,\
    vec4:  vec4_neg,\
    ivec2: ivec2_neg,\
    ivec3: ivec3_neg,\
    ivec4: ivec4_neg,\
    quat:  quat_neg\
)(x)
#define add( lhs, rhs ) _Generic( (lhs),\
    vec2:        vec2_add,\
    vec3:        vec3_add,\
    vec4:        vec4_add,\
    ivec2:       ivec2_add,\
    ivec3:       ivec3_add,\
    ivec4:       ivec4_add,\
    quat:        quat_add,\
    mat2:        mat2_add,\
    mat3*:       mat3_add,\
    const mat3*: mat3_add,\
    mat4*:       mat4_add,\
    const mat4*: mat4_add\
)( lhs, rhs )
#define sub( lhs, rhs ) _Generic( (lhs),\
    vec2:        vec2_sub,\
    vec3:        vec3_sub,\
    vec4:        vec4_sub,\
    ivec2:       ivec2_sub,\
    ivec3:       ivec3_sub,\
    ivec4:       ivec4_sub,\
    quat:        quat_sub,\
    mat2:        mat2_sub,\
    mat3*:       mat3_sub,\
    const mat3*: mat3_sub,\
    mat4*:       mat4_sub,\
    const mat4*: mat4_sub\
)( lhs, rhs )
#define mul( lhs, rhs ) _Generic( (lhs),\
    vec2: _Generic( (rhs),\
        vec2:    vec2_mul_vec2,\
        default: vec2_mul\
    ),\
    vec3: _Generic( (rhs),\
        vec3:    vec3_mul_vec3,\
        default: vec3_mul\
    ),\
    vec4: _Generic( (rhs),\
        vec4:    vec4_mul_vec4,\
        default: vec4_mul\
    ),\
    ivec2: _Generic( (rhs),\
        ivec2:   ivec2_mul_ivec2,\
        default: ivec2_mul\
    ),\
    ivec3: _Generic( (rhs),\
        ivec3:   ivec3_mul_ivec3,\
        default: ivec3_mul\
    ),\
    ivec4: _Generic( (rhs),\
        ivec4:   ivec4_mul_ivec4,\
        default: ivec4_mul\
    ),\
    quat: _Generic( (rhs),\
        quat:    quat_mul_quat,\
        vec3:    quat_mul_vec3,\
        default: quat_mul\
    ),\
    mat2: _Generic( (rhs),\
        mat2:    mat2_mul_mat2,\
        default: mat2_mul\
    ),\
    const mat3*: _Generic( (rhs),\
        const mat3*: mat3_mul_mat3,\
        mat3*:       mat3_mul_mat3,\
        default:     mat3_mul\
    ),\
    mat3*: _Generic( (rhs),\
        const mat3*: mat3_mul_mat3,\
        mat3*:       mat3_mul_mat3,\
        default:     mat3_mul\
    ),\
    mat4*: _Generic( (rhs),\
        mat4*:       mat4_mul_mat4,\
        const mat4*: mat4_mul_mat4,\
        vec4:        mat4_mul_vec4,\
        vec3:        mat4_mul_vec3,\
        default:     mat4_mul\
    ),\
    const mat4*: _Generic( (rhs),\
        mat4*:       mat4_mul_mat4,\
        const mat4*: mat4_mul_mat4,\
        vec4:        mat4_mul_vec4,\
        vec3:        mat4_mul_vec3,\
        default:     mat4_mul\
    )\
)( lhs, rhs )
#define hadamard( lhs, rhs ) _Generic( (lhs),\
    vec2:  vec2_mul_vec2,\
    vec3:  vec3_mul_vec3,\
    vec4:  vec4_mul_vec4,\
    ivec2: ivec2_mul_ivec2,\
    ivec3: ivec3_mul_ivec3,\
    ivec4: ivec4_mul_ivec4\
)( lhs, rhs )
#define div( lhs, rhs ) _Generic( (lhs),\
    vec2: _Generic( (rhs),\
        vec2:    vec2_div_vec2,\
        default: vec2_div\
    ),\
    vec3: _Generic( (rhs),\
        vec3:    vec3_div_vec3,\
        default: vec3_div\
    ),\
    vec4: _Generic( (rhs),\
        vec4:    vec4_div_vec4,\
        default: vec4_div\
    ),\
    ivec2: _Generic( (rhs),\
        ivec2:   ivec2_div_ivec2,\
        default: ivec2_div\
    ),\
    ivec3: _Generic( (rhs),\
        ivec3:   ivec3_div_ivec3,\
        default: ivec3_div\
    ),\
    ivec4: _Generic( (rhs),\
        ivec4:   ivec4_div_ivec4,\
        default: ivec4_div\
    ),\
    quat: _Generic( (rhs),\
        quat:    quat_div_quat,\
        vec3:    quat_div_vec3,\
        default: quat_div\
    ),\
    mat2: _Generic( (rhs),\
        mat2:    mat2_div_mat2,\
        default: mat2_div\
    ),\
    mat3*:       mat3_div,\
    const mat3*: mat3_div,\
    mat4*:       mat4_div,\
    const mat4*: mat4_div\
)( lhs, rhs )
#define hadd( x ) _Generic( (x),\
    vec2:  vec2_hadd,\
    vec3:  vec3_hadd,\
    vec4:  vec4_hadd,\
    ivec2: ivec2_hadd,\
    ivec3: ivec3_hadd,\
    ivec4: ivec4_hadd\
)( x )
#define hmul( x ) _Generic( (x),\
    vec2:  vec2_hmul,\
    vec3:  vec3_hmul,\
    vec4:  vec4_hmul,\
    ivec2: ivec2_hmul,\
    ivec3: ivec3_hmul,\
    ivec4: ivec4_hmul\
)( x )
#define dot( lhs, rhs ) _Generic( (lhs),\
    vec2:  vec2_dot,\
    vec3:  vec3_dot,\
    vec4:  vec4_dot,\
    ivec2: ivec2_dot,\
    ivec3: ivec3_dot,\
    ivec4: ivec4_dot,\
    quat:  quat_dot\
)( lhs, rhs )
#define cross( lhs, rhs ) _Generic( (lhs),\
    vec3: vec3_cross\
)( lhs, rhs )
#define max( x, y ) _Generic( (x),\
    vec2:    vec2_max,\
    vec3:    vec3_max,\
    vec4:    vec4_max,\
    default: num_max\
)( x, y )
#define hmax( x ) _Generic( (x),\
    vec2: vec2_hmax,\
    vec3: vec3_hmax,\
    vec4: vec4_hmax\
)( x )
#define min( x, y ) _Generic( (x),\
    vec2:    vec2_min,\
    vec3:    vec3_min,\
    vec4:    vec4_min,\
    default: num_min\
)( x, y )
#define hmin( x ) _Generic( (x),\
    vec2: vec2_hmin,\
    vec3: vec3_hmin,\
    vec4: vec4_hmin\
)( x )
#define clamp( x, min, max ) _Generic( (x),\
    vec2: _Generic( (min),\
        vec2:    vec2_clamp,\
        default: vec2_clamp_length\
    ),\
    vec3: _Generic( (min),\
        vec3:    vec3_clamp,\
        default: vec3_clamp_length\
    ),\
    vec4: _Generic( (min),\
        vec4:    vec4_clamp,\
        default: vec4_clamp_length\
    ),\
    default: num_clamp\
)( x, min, max )
#define length_sqr( x ) _Generic( (x),\
    vec2:  vec2_length_sqr,\
    vec3:  vec3_length_sqr,\
    vec4:  vec4_length_sqr,\
    ivec2: ivec2_length_sqr,\
    ivec3: ivec3_length_sqr,\
    ivec4: ivec4_length_sqr,\
    quat:  quat_length_sqr\
)( x )
#define length( x ) _Generic( (x),\
    vec2:  vec2_length,\
    vec3:  vec3_length,\
    vec4:  vec4_length,\
    ivec2: ivec2_length,\
    ivec3: ivec3_length,\
    ivec4: ivec4_length,\
    quat:  quat_length\
)( x )
#define distance_sqr( a, b ) _Generic( (a),\
    vec2:  vec2_distance_sqr,\
    vec3:  vec3_distance_sqr,\
    vec4:  vec4_distance_sqr,\
    ivec2: ivec2_distance_sqr,\
    ivec3: ivec3_distance_sqr,\
    ivec4: ivec4_distance_sqr\
)( a, b )
#define distance( a, b ) _Generic( (a),\
    vec2:  vec2_distance,\
    vec3:  vec3_distance,\
    vec4:  vec4_distance,\
    ivec2: ivec2_distance,\
    ivec3: ivec3_distance,\
    ivec4: ivec4_distance\
)( a, b )
#define normalize( x ) _Generic( (x),\
    u8:  u8_normalize,\
    u16: u16_normalize,\
    u32: u32_normalize,\
    u64: u64_normalize,\
    i8:  i8_normalize,\
    i16: i16_normalize,\
    i32: i32_normalize,\
    i64: i64_normalize,\
    vec2: vec2_normalize,\
    vec3: vec3_normalize,\
    vec4: vec4_normalize,\
    quat: quat_normalize\
)( x )
#define angle( a, b ) _Generic( (a),\
    vec2: vec2_angle,\
    vec3: vec3_angle\
)( a, b )
#define abs( x ) _Generic( (x),\
    vec2:    vec2_abs,\
    vec3:    vec3_abs,\
    vec4:    vec4_abs,\
    default: num_abs\
)( x )
#define sign( x ) _Generic( (x),\
    vec2:    vec2_sign,\
    vec3:    vec3_sign,\
    vec4:    vec4_sign,\
    default: num_sign\
)( x )
#define trunc( x ) _Generic( (x),\
    vec2:    vec2_trunc,\
    vec3:    vec3_trunc,\
    vec4:    vec4_trunc,\
    double:  f64_trunc,\
    default: f32_trunc\
)( x )
#define floor( x ) _Generic( (x),\
    vec2:    vec2_floor,\
    vec3:    vec3_floor,\
    vec4:    vec4_floor,\
    double:  f64_floor,\
    default: f32_floor\
)( x )
#define ceil( x ) _Generic( (x),\
    vec2:    vec2_ceil,\
    vec3:    vec3_ceil,\
    vec4:    vec4_ceil,\
    double:  f64_ceil,\
    default: f32_ceil\
)( x )
#define round( x ) _Generic( (x),\
    vec2:    vec2_round,\
    vec3:    vec3_round,\
    vec4:    vec4_round,\
    double:  f64_round,\
    default: f32_round\
)( x )
#define fract( x ) _Generic( (x),\
    vec2:    vec2_fract,\
    vec3:    vec3_fract,\
    vec4:    vec4_fract,\
    double:  f64_fract,\
    default: f32_fract\
)( x )
#define reflect( direction, normal ) _Generic( (direction),\
    vec2: vec2_reflect,\
    vec3: vec3_reflect\
)( direction, normal )
#define conjugate( x ) _Generic( (x),\
    quat: quat_conjugate\
)( x )
#define inverse( x ) _Generic( (x),\
    mat4*:       mat4_inverse,\
    const mat4*: mat4_inverse,\
    quat:        quat_inverse\
)( x )
#define inverse_checked( x ) _Generic( (x),\
    mat4*:       mat4_inverse_checked,\
    const mat4*: mat4_inverse_checked,\
    quat:        quat_inverse_checked\
)( x )
#define slerp( a, b, t ) _Generic( (a),\
    vec3: vec3_slerp,\
    quat: quat_slerp\
)( a, b, t )
#define transpose( x ) _Generic( (x),\
    mat2:        mat2_transpose,\
    const mat3*: mat3_transpose,\
    mat3*:       mat3_transpose,\
    const mat4*: mat4_transpose,\
    mat4*:       mat4_transpose\
)( x )
#define determinant( x ) _Generic( (x),\
    mat2:        mat2_determinant,\
    mat3*:       mat3_determinant,\
    const mat3*: mat3_determinant,\
    const mat4*: mat4_determinant,\
    mat4*:       mat4_determinant\
)( x )

#define cmp( a, b ) _Generic( (a),\
    double:  f64_cmp,\
    vec2:    vec2_cmp,\
    vec3:    vec3_cmp,\
    vec4:    vec4_cmp,\
    ivec2:   ivec2_cmp,\
    ivec3:   ivec3_cmp,\
    ivec4:   ivec4_cmp,\
    quat:    quat_cmp,\
    default: f32_cmp\
)( a, b )

#endif /* header guard */
