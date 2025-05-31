#if !defined(CORE_MATH_OVERLOAD_H) && !defined(__cplusplus)
#define CORE_MATH_OVERLOAD_H
/**
 * @file   overload.h
 * @brief  Math: Overloads for C11.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 29, 2025
*/
// IWYU pragma: begin_keep
#include "core/math/common.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/math/vector4.h"
#include "core/math/matrix2x2.h"
#include "core/math/matrix3x3.h"
#include "core/math/matrix4x4.h"
#include "core/math/quaternion.h"
// IWYU pragma: end_keep

#define sign( x ) _Generic( (x), \
    vec2:    vec2_sign,  \
    ivec2:   ivec2_sign, \
    vec3:    vec3_sign,  \
    ivec3:   ivec3_sign, \
    vec4:    vec4_sign,  \
    ivec4:   ivec4_sign, \
    i8:      i8_sign,    \
    i16:     i16_sign,   \
    i64:     i64_sign,   \
    u8:      u8_sign,    \
    u16:     u16_sign,   \
    u32:     u32_sign,   \
    u64:     u64_sign,   \
    f32:     f32_sign,   \
    f64:     f64_sign,   \
    default: i32_sign    \
)(x)

#define abs( x ) _Generic( (x), \
    vec2:    vec2_abs,  \
    ivec2:   ivec2_abs, \
    vec3:    vec3_abs,  \
    ivec3:   ivec3_abs, \
    vec4:    vec4_abs,  \
    ivec4:   ivec4_abs, \
    i8:      i8_abs,    \
    i16:     i16_abs,   \
    i64:     i64_abs,   \
    u8:      u8_abs,    \
    u16:     u16_abs,   \
    u32:     u32_abs,   \
    u64:     u64_abs,   \
    f32:     f32_abs,   \
    f64:     f64_abs,   \
    default: i32_abs    \
)(x)

#define min( x, ... ) _Generic( (x),        \
    vec2:    vec2_min##__VA_OPT__(_vec2),   \
    vec3:    vec3_min##__VA_OPT__(_vec3),   \
    vec4:    vec4_min##__VA_OPT__(_vec4),   \
    ivec2:   ivec2_min##__VA_OPT__(_ivec2), \
    ivec3:   ivec3_min##__VA_OPT__(_ivec3), \
    ivec4:   ivec4_min##__VA_OPT__(_ivec4), \
    i8:       i8_min,                       \
    i16:     i16_min,                       \
    i64:     i64_min,                       \
    u8:       u8_min,                       \
    u16:     u16_min,                       \
    u32:     u32_min,                       \
    u64:     u64_min,                       \
    f32:     f32_min,                       \
    f64:     f64_min,                       \
    default: i32_min                        \
)( x __VA_OPT__(,) __VA_ARGS__ )

#define max( x, ... ) _Generic( (x),        \
    vec2:    vec2_max##__VA_OPT__(_vec2),   \
    vec3:    vec3_max##__VA_OPT__(_vec3),   \
    vec4:    vec4_max##__VA_OPT__(_vec4),   \
    ivec2:   ivec2_max##__VA_OPT__(_ivec2), \
    ivec3:   ivec3_max##__VA_OPT__(_ivec3), \
    ivec4:   ivec4_max##__VA_OPT__(_ivec4), \
    i8:       i8_max,                       \
    i16:     i16_max,                       \
    i64:     i64_max,                       \
    u8:       u8_max,                       \
    u16:     u16_max,                       \
    u32:     u32_max,                       \
    u64:     u64_max,                       \
    f32:     f32_max,                       \
    f64:     f64_max,                       \
    default: i32_max                        \
)( x __VA_OPT__(,) __VA_ARGS__ )

#define clamp( v, min_, max_ ) _Generic( (v), \
    vec2: _Generic( (min_),                   \
        vec2:    vec2_clamp,                  \
        default: vec2_clamp_length            \
    ),                                        \
    ivec2: ivec2_clamp,                       \
    vec3: _Generic( (min_),                   \
        vec3:    vec3_clamp,                  \
        default: vec3_clamp_length            \
    ),                                        \
    ivec3: ivec3_clamp,                       \
    vec4: _Generic( (min_),                   \
        vec4:    vec4_clamp,                  \
        default: vec4_clamp_length            \
    ),                                        \
    ivec4: ivec4_clamp,                       \
    i8:       i8_clamp,                       \
    i16:     i16_clamp,                       \
    i64:     i64_clamp,                       \
    u8:       u8_clamp,                       \
    u16:     u16_clamp,                       \
    u32:     u32_clamp,                       \
    u64:     u64_clamp,                       \
    f32:     f32_clamp,                       \
    f64:     f64_clamp,                       \
    default: i32_clamp                        \
)(v, min_, max_)

#define cmp( a, b ) _Generic( (a), \
    vec2:    vec2_cmp,  \
    ivec2:   ivec2_cmp, \
    vec3:    vec3_cmp,  \
    ivec3:   ivec3_cmp, \
    vec4:    vec4_cmp,  \
    ivec4:   ivec4_cmp, \
    quat:    quat_cmp,  \
    f64:     f64_cmp,   \
    default: f32_cmp    \
)(a, b)

#define normalize( x ) _Generic( (x), \
    vec2:    vec2_normalize,  \
    vec3:    vec3_normalize,  \
    vec4:    vec4_normalize,  \
    quat:    quat_normalize,  \
    u8:      u8_normalize,    \
    u16:     u16_normalize,   \
    u32:     u32_normalize,   \
    u64:     u64_normalize,   \
    i8:      i8_normalize,    \
    i16:     i16_normalize,   \
    i64:     i64_normalize,   \
    default: i32_normalize    \
)(x)

#define trunc( x ) _Generic( (x), \
    vec2:    vec2_trunc,  \
    vec3:    vec3_trunc,  \
    vec4:    vec4_trunc,  \
    f64:     f64_trunc,   \
    default: f32_trunc    \
)(x)

#define floor( x ) _Generic( (x), \
    vec2:    vec2_floor,  \
    vec3:    vec3_floor,  \
    vec4:    vec4_floor,  \
    f64:     f64_floor,   \
    default: f32_floor    \
)(x)

#define ceil( x ) _Generic( (x), \
    vec2:    vec2_ceil,  \
    vec3:    vec3_ceil,  \
    vec4:    vec4_ceil,  \
    f64:     f64_ceil,   \
    default: f32_ceil    \
)(x)

#define round( x ) _Generic( (x), \
    vec2:    vec2_round,  \
    vec3:    vec3_round,  \
    vec4:    vec4_round,  \
    f64:     f64_round,   \
    default: f32_round    \
)(x)

#define fract( x ) _Generic( (x), \
    vec2:    vec2_fract,  \
    vec3:    vec3_fract,  \
    vec4:    vec4_fract,  \
    f64:     f64_fract,   \
    default: f32_fract    \
)(x)

#define lerp( a, b, t ) _Generic( (a), \
    vec2: _Generic( (t),               \
        vec2:    vec2_lerp_vec2,       \
        default: vec2_lerp             \
    ),                                 \
    vec3: _Generic( (t),               \
        vec3:    vec3_lerp_vec3,       \
        default: vec3_lerp             \
    ),                                 \
    vec4: _Generic( (t),               \
        vec4:    vec4_lerp_vec4,       \
        default: vec4_lerp             \
    ),                                 \
    quat:    quat_lerp,                \
    default: f32_lerp                  \
)( a, b, t )

#define mix( a, b, t ) \
    lerp( a, b, t )

#define inverselerp( a, b, v ) \
    f32_inverselerp( a, b, v )

#define inversemix( a, b, v ) \
    inverselerp( a, b, v )

#define remap( in_min, in_max, out_min, out_max, v ) \
    f32_remap( in_min, in_max, out_min, out_max, v )

#define step( edge, x ) _Generic( (edge), \
    vec2: vec2_step_vec2,      \
    vec3: vec3_step_vec3,      \
    vec4: vec4_step_vec4,      \
    f32: _Generic( (x),        \
        vec2:    vec2_step,    \
        vec3:    vec3_step,    \
        vec4:    vec4_step,    \
        default: f32_step      \
    )                          \
)( edge, x )

#define smoothstep( edge0, edge1, x ) _Generic( (edge0), \
    vec2: vec2_smoothstep_vec2,   \
    vec3: vec3_smoothstep_vec3,   \
    vec4: vec4_smoothstep_vec4,   \
    f32: _Generic( (x),           \
        vec2:    vec2_smoothstep, \
        vec3:    vec3_smoothstep, \
        vec4:    vec4_smoothstep, \
        default: f32_smoothstep   \
    )                             \
)( edge0, edge1, x )

#define smootherstep( edge0, edge1, x ) _Generic( (edge0), \
    vec2: vec2_smootherstep_vec2,   \
    vec3: vec3_smootherstep_vec3,   \
    vec4: vec4_smootherstep_vec4,   \
    f32: _Generic( (x),             \
        vec2:    vec2_smootherstep, \
        vec3:    vec3_smootherstep, \
        vec4:    vec4_smootherstep, \
        default: f32_smootherstep   \
    )                               \
)( edge0, edge1, x )

#define add( lhs, rhs ) _Generic( (lhs), \
    vec2:         vec2_add, \
    ivec2:       ivec2_add, \
    vec3:         vec3_add, \
    ivec3:       ivec3_add, \
    vec4:         vec4_add, \
    ivec4:       ivec4_add, \
    mat2:         mat2_add, \
    const mat3*:  mat3_add, \
    const mat4*:  mat4_add, \
    quat:         quat_add  \
)( lhs, rhs )

#define sub( lhs, rhs ) _Generic( (lhs), \
    vec2:         vec2_sub, \
    ivec2:       ivec2_sub, \
    vec3:         vec3_sub, \
    ivec3:       ivec3_sub, \
    vec4:         vec4_sub, \
    ivec4:       ivec4_sub, \
    mat2:         mat2_sub, \
    const mat3*:  mat3_sub, \
    const mat4*:  mat4_sub, \
    quat:         quat_sub  \
)( lhs, rhs )

#define mul( lhs, rhs ) _Generic( (lhs), \
    vec2: _Generic( (rhs),              \
        vec2:    vec2_mul_vec2,         \
        default: vec2_mul               \
    ),                                  \
    ivec2: _Generic( (rhs),             \
        ivec2:   ivec2_mul_ivec2,       \
        default: ivec2_mul              \
    ),                                  \
    vec3: _Generic( (rhs),              \
        vec3:    vec3_mul_vec3,         \
        default: vec3_mul               \
    ),                                  \
    ivec3: _Generic( (rhs),             \
        ivec3:   ivec3_mul_ivec3,       \
        default: ivec3_mul              \
    ),                                  \
    vec4: _Generic( (rhs),              \
        vec4:    vec4_mul_vec4,         \
        default: vec4_mul               \
    ),                                  \
    ivec4: _Generic( (rhs),             \
        ivec4:   ivec4_mul_ivec4,       \
        default: ivec4_mul              \
    ),                                  \
    mat2: _Generic( (rhs),              \
        mat2:    mat2_mul_mat2,         \
        vec2:    mat2_mul_vec2,         \
        default: mat2_mul               \
    ),                                  \
    const mat3*: _Generic( (rhs),       \
        const mat3*: mat3_mul_mat3,     \
        vec3:        mat3_mul_vec3,     \
        default:     mat3_mul           \
    ),                                  \
    const mat4*: _Generic( (rhs),       \
        const mat4*: mat4_mul_mat4,     \
        vec4:        mat4_mul_vec4,     \
        vec3:        mat4_mul_vec3,     \
        default:     mat4_mul           \
    ),                                  \
    quat: _Generic( (rhs),              \
        quat: quat_mul_quat,            \
        vec3: quat_mul_vec3,            \
        default: quat_mul               \
    )                                   \
)( lhs, rhs )

#define div( lhs, rhs ) _Generic( (lhs), \
    vec2: _Generic( (rhs),        \
        vec2:    vec2_div_vec2,   \
        default: vec2_div         \
    ),                            \
    ivec2: _Generic( (rhs),       \
        vec2:    ivec2_div_ivec2, \
        default: ivec2_div        \
    ),                            \
    vec3: _Generic( (rhs),        \
        vec3:    vec3_div_vec3,   \
        default: vec3_div         \
    ),                            \
    ivec3: _Generic( (rhs),       \
        vec3:    ivec3_div_ivec3, \
        default: ivec3_div        \
    ),                            \
    vec4: _Generic( (rhs),        \
        vec4:    vec4_div_vec4,   \
        default: vec4_div         \
    ),                            \
    ivec4: _Generic( (rhs),       \
        vec4:    ivec4_div_ivec4, \
        default: ivec4_div        \
    ),                            \
    mat2:         mat2_div,       \
    const mat3*:  mat3_div,       \
    const mat4*:  mat4_div,       \
    quat:         quat_div        \
)( lhs, rhs )

#define mod( lhs, rhs ) _Generic( (lhs), \
    vec2: _Generic( (rhs),        \
        vec2:    vec2_mod_vec2,   \
        default: vec2_mod         \
    ),                            \
    ivec2: _Generic( (rhs),       \
        ivec2:   ivec2_mod_ivec2, \
        default: ivec2_mod        \
    ),                            \
    vec3: _Generic( (rhs),        \
        vec3:    vec3_mod_vec3,   \
        default: vec3_mod         \
    ),                            \
    ivec3: _Generic( (rhs),       \
        ivec3:   ivec3_mod_ivec3, \
        default: ivec3_mod        \
    ),                            \
    vec4: _Generic( (rhs),        \
        vec4:    vec4_mod_vec4,   \
        default: vec4_mod         \
    ),                            \
    ivec4: _Generic( (rhs),       \
        ivec4:   ivec4_mod_ivec4, \
        default: ivec4_mod        \
    ),                            \
    default: f32_mod              \
)( lhs, rhs )

#define neg( x ) _Generic( (x), \
    vec2:   vec2_neg, \
    ivec2: ivec2_neg, \
    vec3:   vec3_neg, \
    ivec3: ivec3_neg, \
    vec4:   vec4_neg, \
    ivec4: ivec4_neg, \
    quat:   quat_neg  \
)( x )

#define isnan( x ) _Generic( (x), \
    vec2:    vec2_isnan, \
    vec3:    vec3_isnan, \
    vec4:    vec4_isnan, \
    f64:     f64_isnan,  \
    default: f32_isnan   \
)(x)

#define isinf( x ) _Generic( (x), \
    vec2:    vec2_isinf, \
    vec3:    vec3_isinf, \
    vec4:    vec4_isinf, \
    f64:     f64_isinf,  \
    default: f32_isinf   \
)(x)

#define swap( x ) _Generic( (x), \
    vec2:  vec2_swap, \
    ivec2: ivec2_swap \
)(x)

#define hadd( x ) _Generic( (x), \
     vec2:  vec2_hadd, \
    ivec2: ivec2_hadd, \
     vec3:  vec3_hadd, \
    ivec3: ivec3_hadd, \
     vec4:  vec4_hadd, \
    ivec4: ivec4_hadd  \
)(x)

#define hmul( x ) _Generic( (x), \
     vec2:  vec2_hmul, \
    ivec2: ivec2_hmul, \
     vec3:  vec3_hmul, \
    ivec3: ivec3_hmul, \
     vec4:  vec4_hmul, \
    ivec4: ivec4_hmul  \
)(x)

#define dot( lhs, rhs ) _Generic( (lhs), \
     vec2:  vec2_dot, \
    ivec2: ivec2_dot, \
     vec3:  vec3_dot, \
    ivec3: ivec3_dot, \
     vec4:  vec4_dot, \
    ivec4: ivec4_dot, \
     quat:  quat_dot  \
)( lhs, rhs )

#define aspect_ratio( x ) _Generic( (x), \
     vec2:  vec2_aspect_ratio, \
    ivec2: ivec2_aspect_ratio  \
)(x)

#define length_sqr( x ) _Generic( (x), \
     vec2:  vec2_length_sqr, \
    ivec2: ivec2_length_sqr, \
     vec3:  vec3_length_sqr, \
    ivec3: ivec3_length_sqr, \
     vec4:  vec4_length_sqr, \
    ivec4: ivec4_length_sqr, \
     quat:  quat_length_sqr  \
)( x )

#define length( x ) _Generic( (x), \
     vec2:  vec2_length, \
    ivec2: ivec2_length, \
     vec3:  vec3_length, \
    ivec3: ivec3_length, \
     vec4:  vec4_length, \
    ivec4: ivec4_length, \
     quat:  quat_length  \
)( x )

#define distance_sqr( a, b ) _Generic( (a), \
    vec2: vec2_distance_sqr, \
    vec3: vec3_distance_sqr, \
    vec4: vec4_distance_sqr  \
)( a, b )

#define distance( a, b ) _Generic( (a), \
    vec2: vec2_distance, \
    vec3: vec3_distance, \
    vec4: vec4_distance  \
)( a, b )

#define reflect( d, n ) _Generic( (d), \
    vec2: vec2_reflect, \
    vec3: vec3_reflect  \
)( d, n )

#define rotate( v, angle ) \
    vec2_rotate( v, angle )

#define angle( a, b ) _Generic( (a), \
    vec2: vec2_angle, \
    vec3: vec3_angle  \
)( a, b )

#define lt( a, b ) _Generic( (a), \
     vec2:  vec2_lt, \
    ivec2: ivec2_lt, \
     vec3:  vec3_lt, \
    ivec3: ivec3_lt, \
     vec4:  vec4_lt, \
    ivec4: ivec4_lt  \
)( a, b )

#define gt( a, b ) _Generic( (a), \
     vec2:  vec2_gt, \
    ivec2: ivec2_gt, \
     vec3:  vec3_gt, \
    ivec3: ivec3_gt, \
     vec4:  vec4_gt, \
    ivec4: ivec4_gt  \
)( a, b )

#define lteq( a, b ) _Generic( (a), \
     vec2:  vec2_lteq, \
    ivec2: ivec2_lteq, \
     vec3:  vec3_lteq, \
    ivec3: ivec3_lteq, \
     vec4:  vec4_lteq, \
    ivec4: ivec4_lteq  \
)( a, b )

#define gteq( a, b ) _Generic( (a), \
     vec2:  vec2_gteq, \
    ivec2: ivec2_gteq, \
     vec3:  vec3_gteq, \
    ivec3: ivec3_gteq, \
     vec4:  vec4_gteq, \
    ivec4: ivec4_gteq  \
)( a, b )

#define eq( a, b ) _Generic( (a), \
     vec2:  vec2_eq, \
    ivec2: ivec2_eq, \
     vec3:  vec3_eq, \
    ivec3: ivec3_eq, \
     vec4:  vec4_eq, \
    ivec4: ivec4_eq  \
)( a, b )

#define neq( a, b ) _Generic( (a), \
     vec2:  vec2_neq, \
    ivec2: ivec2_neq, \
     vec3:  vec3_neq, \
    ivec3: ivec3_neq, \
     vec4:  vec4_neq, \
    ivec4: ivec4_neq  \
)( a, b )

#define cross( lhs, rhs ) \
    vec3_cross( lhs, rhs )

#define rotl( x ) _Generic( (x), \
     vec3:  vec3_rotl, \
    ivec3: ivec3_rotl, \
     vec4:  vec4_rotl, \
    ivec4: ivec4_rotl  \
)( x )

#define rotr( x ) _Generic( (x), \
     vec3:  vec3_rotr, \
    ivec3: ivec3_rotr, \
     vec4:  vec4_rotr, \
    ivec4: ivec4_rotr  \
)( x )

#define mid( x ) _Generic( (x), \
     vec3:  vec3_mid, \
    ivec3: ivec3_mid  \
)( x )

#define slerp( a, b, t ) _Generic( (a), \
    vec3: vec3_slerp, \
    quat: quat_slerp  \
)( a, b, t )

#define conjugate( x ) \
    quat_conjugate( x )

#define inverse_checked( x, out_inverse ) _Generic( (x), \
    quat:        quat_inverse_checked, \
    const mat4*: mat4_inverse_checked  \
)( x, out_inverse )

#define inverse( x ) _Generic( (x), \
    quat:        quat_inverse, \
    const mat4*: mat4_inverse  \
)( x )

#define transpose( x ) _Generic( (x), \
          mat2:  mat2_transpose, \
    const mat3*: mat3_transpose, \
    const mat4*: mat4_transpose  \
)( x )

#define determinant( x ) _Generic( (x), \
          mat2:  mat2_determinant, \
    const mat3*: mat3_determinant, \
    const mat4*: mat4_determinant  \
)( x )

#define submatrix( x, column, row ) \
    mat4_submatrix( x, column, row )

#define minor( x, column, row ) \
    mat4_minor( x, column, row )

#define cofactor( x, column, row ) \
    mat4_cofactor( x, column, row )

#define cofactor_matrix( x ) \
    mat4_cofactor_matrix( x )

#define adjoint( x ) \
    mat4_adjoint( x )

#define normal_matrix_checked( x, out_normal ) \
    mat4_normal_matrix_checked( x, out_normal )

#define normal_matrix( x ) \
    mat4_normal_matrix( x )

#define view( position, target, up ) \
    mat4_view( position, target, up )

#define view_2d( position, target ) \
    mat4_view_2d( position, target )

#define ortho( left, right, bottom, top, ... ) \
    mat4_ortho( left, right, bottom, top, ##__VA_ARGS__, 0.0001f, 1000.0f )

#define perspective( field_of_view, aspect_ratio, ... ) \
    mat4_perspective( field_of_view, aspect_ratio, ##__VA_ARGS__, 0.0001f, 1000.0f )

#define translation( x, ... ) _Generic( (x), \
    vec3:    mat4_translation_vec3, \
    default: mat4_translation       \
)( x __VA_OPT__(,) __VA_ARGS__ )

#define translation_2d( x, ... ) _Generic( (x), \
    vec2:    mat4_translation_2d_vec2, \
    default: mat4_translation_2d       \
)( x __VA_OPT__(,) __VA_ARGS__ )

#define rotation_pitch( x ) \
    mat4_rotation_pitch( x )

#define rotation_yaw( x ) \
    mat4_rotation_yaw( x )

#define rotation_roll( x ) \
    mat4_rotation_roll( x )

#define rotation_euler( x, ... ) _Generic( (x), \
    vec3:    mat4_rotation_euler_vec3, \
    default: mat4_rotation_euler       \
)( x __VA_OPT__(,) __VA_ARGS__ )

#define rotation( q ) \
    mat4_rotation( q )

#define rotation_2d( rotation ) \
    mat4_rotation_2d( rotation )

#define scale( x, ... ) _Generic( (x), \
    vec3:    mat4_scale_vec3, \
    default: mat4_scale \
)( x __VA_OPT__(,) __VA_ARGS__ )

#define scale_2d( x, ... ) _Generic( (x), \
    vec2:    mat4_scale_2d_vec2, \
    default: mat4_scale_2d \
)( x __VA_OPT__(,) __VA_ARGS__ )

#define transform( translation, rotation, scale ) _Generic( (rotation), \
    quat: mat4_transform,       \
    vec3: mat4_transform_euler  \
)( translation, rotation, scale )

#define transform_2d( translation, rotation, scale ) \
    mat4_transform_2d( translation, rotation, scale )

#define radians( degrees ) _Generic( (degrees), \
    vec2:    vec2_radians, \
    vec3:    vec3_radians, \
    vec4:    vec4_radians, \
    f64:     f64_radians,  \
    default: f32_radians   \
)( degrees )

#define degrees( radians ) _Generic( (radians), \
    vec2:    vec2_degrees, \
    vec3:    vec3_degrees, \
    vec4:    vec4_degrees, \
    f64:     f64_degrees,  \
    default: f32_degrees   \
)( radians )

#define sin( x ) _Generic( (x), \
    vec2:    vec2_sin, \
    vec3:    vec3_sin, \
    vec4:    vec4_sin, \
    default: f32_sin   \
)( x )

#define cos( x ) _Generic( (x), \
    vec2:    vec2_cos, \
    vec3:    vec3_cos, \
    vec4:    vec4_cos, \
    default: f32_cos   \
)( x )

#define tan( x ) _Generic( (x), \
    vec2:    vec2_tan, \
    vec3:    vec3_tan, \
    vec4:    vec4_tan, \
    default: f32_tan   \
)( x )

#define asin( x ) _Generic( (x), \
    vec2:    vec2_asin, \
    vec3:    vec3_asin, \
    vec4:    vec4_asin, \
    default: f32_asin   \
)( x )

#define acos( x ) _Generic( (x), \
    vec2:    vec2_acos, \
    vec3:    vec3_acos, \
    vec4:    vec4_acos, \
    default: f32_acos   \
)( x )

#define atan( x ) _Generic( (x), \
    vec2:    vec2_atan, \
    vec3:    vec3_atan, \
    vec4:    vec4_atan, \
    default: f32_atan   \
)( x )

#define atan2( y, x ) _Generic( (x), \
    vec2:    vec2_atan2, \
    vec3:    vec3_atan2, \
    vec4:    vec4_atan2, \
    default: f32_atan2   \
)( y, x )

#define sqrt( x ) _Generic( (x), \
    vec2:    vec2_sqrt, \
    vec3:    vec3_sqrt, \
    vec4:    vec4_sqrt, \
    default: f32_sqrt   \
)( x )

#define inversesqrt( x ) _Generic( (x), \
    vec2:    vec2_inversesqrt, \
    vec3:    vec3_inversesqrt, \
    vec4:    vec4_inversesqrt, \
    default: f32_inversesqrt   \
)( x )

#define cbrt( x ) _Generic( (x), \
    vec2:    vec2_cbrt, \
    vec3:    vec3_cbrt, \
    vec4:    vec4_cbrt, \
    default: f32_cbrt   \
)( x )

#define ln( x ) _Generic( (x), \
    vec2:    vec2_ln, \
    vec3:    vec3_ln, \
    vec4:    vec4_ln, \
    default: f32_ln   \
)( x )

#define log2( x ) _Generic( (x), \
    vec2:    vec2_log2, \
    vec3:    vec3_log2, \
    vec4:    vec4_log2, \
    default: f32_log2   \
)( x )

#define log10( x ) _Generic( (x), \
    vec2:    vec2_log10, \
    vec3:    vec3_log10, \
    vec4:    vec4_log10, \
    default: f32_log10   \
)( x )

#define powi( base, exp ) \
    f32_powi( base, exp )

#define pow( base, exp ) _Generic( (base), \
    vec2:    vec2_pow, \
    vec3:    vec3_pow, \
    vec4:    vec4_pow, \
    default: f32_pow   \
)( base, exp )

#define exp( x ) _Generic( (x), \
    vec2:    vec2_exp, \
    vec3:    vec3_exp, \
    vec4:    vec4_exp, \
    default: f32_exp   \
)( x )

#define exp2( x ) _Generic( (x), \
    vec2:    vec2_exp2, \
    vec3:    vec3_exp2, \
    vec4:    vec4_exp2, \
    default: f32_exp2   \
)( x )

#define sincos( x, out_sine, out_cosine ) \
    f32_sincos( x, out_sine, out_cosine )

#define asin_real( x ) \
    f32_asin_real( x )

#define wrap_degrees( degrees ) \
    f32_wrap_degrees( degrees )

#define wrap_radians( radians ) \
    f32_wrap_radians( radians )

#endif /* header guard */
