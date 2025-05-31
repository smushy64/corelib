#if !defined(CORE_MATH_VECTOR2_H)
#define CORE_MATH_VECTOR2_H
/**
 * @file   vector2.h
 * @brief  Vector2 definition.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 28, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/constants.h"
#include "core/math/trig.h"
#include "core/math/exponential.h"
#include "core/math/common.h"

/// @brief 2 Component 32-bit Floating Point Vector.
struct Vector2 {
    /// @brief Union of vector component aliases.
    union {
        /// @brief X and Y components.
        struct {
            /// @brief X component.
            f32 x;
            /// @brief Y component.
            f32 y;
        };
        /// @brief X and Y components -- as Red and Green channels.
        struct {
            /// @brief X component -- as Red channel.
            f32 r;
            /// @brief Y component -- as Green channel.
            f32 g;
        };
        /// @brief Width and Height (X and Y).
        struct {
            /// @brief Width (X component).
            f32 w;
            /// @brief Height (Y component).
            f32 h;
        };
        /// @brief UV coordinates (X and Y).
        struct {
            /// @brief U (X component).
            f32 u;
            /// @brief V (Y component).
            f32 v;
        };
        /// @brief X and Y as an array.
        f32 array[2];
    };
};
/// @brief 2 Component 32-bit Signed Integer Vector.
struct IVector2 {
    /// @brief Union of vector component aliases.
    union {
        /// @brief X and Y components.
        struct {
            /// @brief X component.
            i32 x;
            /// @brief Y component.
            i32 y;
        };
        /// @brief Width and Height (X and Y).
        struct {
            /// @brief Width (X component).
            i32 w;
            /// @brief Height (Y component).
            i32 h;
        };
        /// @brief X and Y as an array.
        i32 array[2];
    };
};
/// @brief 2 Component 32-bit Unsigned Integer Vector.
struct UVector2 {
    union {
        struct {
            u32 x;
            u32 y;
        };
        u32 array[2];
    };
};
/// @brief 2 Component 64-bit Floating Point Vector.
struct DVector2 {
    union {
        struct {
            f64 x;
            f64 y;
        };
        f64 array[2];
    };
};
/// @brief 2 Component 32-bit Boolean Vector.
struct BVector2 {
    union {
        struct {
            b32 x;
            b32 y;
        };
        b32 array[2];
    };
};

#if defined(__cplusplus)
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define vec2_new( x, y )   Vector2 { .array={x, y} }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define ivec2_new( x, y ) IVector2 { .array={ x, y } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define bvec2_new( x, y ) BVector2 { .array={ x, y } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define uvec2_new( x, y ) UVector2 { .array={ x, y } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define dvec2_new( x, y ) DVector2 { .array={ x, y } }
#else
    /// @brief 2 Component 32-bit Floating Point Vector.
    typedef struct Vector2   vec2;
    /// @brief 2 Component 32-bit Integer Vector.
    typedef struct IVector2 ivec2;
    /// @brief 2 Component 32-bit Boolean Vector.
    typedef struct BVector2 bvec2;
    /// @brief 2 Component 32-bit Unsigned Integer Vector.
    typedef struct UVector2 uvec2;
    /// @brief 2 Component 64-bit Floating Point Vector.
    typedef struct DVector2 dvec2;

    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define vec2_new( x, y )   (struct Vector2){ .array={ x, y } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define ivec2_new( x, y ) (struct IVector2){ .array={ x, y } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define bvec2_new( x, y ) (struct BVector2){ .array={ x, y } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define uvec2_new( x, y ) (struct UVector2){ .array={ x, y } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define dvec2_new( x, y ) (struct DVector2){ .array={ x, y } }

    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define  vec2( x, y )  vec2_new( x, y )
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define ivec2( x, y ) ivec2_new( x, y )
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define bvec2( x, y ) bvec2_new( x, y )
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define uvec2( x, y ) uvec2_new( x, y )
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @return Vector.
    #define dvec2( x, y ) dvec2_new( x, y )
#endif

/// @brief Vector2 zero constant.
#define VEC2_ZERO  ( vec2_new( 0.0f, 0.0f ) )
/// @brief Vector2 one constant.
#define VEC2_ONE   ( vec2_new( 1.0f, 1.0f ) )
/// @brief Vector2 left constant.
#define VEC2_LEFT  ( vec2_new( -1.0f, 0.0f ) )
/// @brief Vector2 right constant.
#define VEC2_RIGHT ( vec2_new(  1.0f, 0.0f ) )
/// @brief Vector2 up constant.
#define VEC2_UP    ( vec2_new(  0.0f, 1.0f ) )
/// @brief Vector2 down constant.
#define VEC2_DOWN  ( vec2_new(  0.0f,-1.0f ) )

/// @brief IVector2 zero constant.
#define IVEC2_ZERO  ( ivec2_new( 0, 0 ) )
/// @brief IVector2 one constant.
#define IVEC2_ONE   ( ivec2_new( 1, 1 ) )
/// @brief IVector2 left constant.
#define IVEC2_LEFT  ( ivec2_new( -1, 0 ) )
/// @brief IVector2 right constant.
#define IVEC2_RIGHT ( ivec2_new(  1, 0 ) )
/// @brief IVector2 up constant.
#define IVEC2_UP    ( ivec2_new(  0, 1 ) )
/// @brief IVector2 down constant.
#define IVEC2_DOWN  ( ivec2_new(  0,-1 ) )

/// @brief BVector2 zero constant.
#define BVEC2_ZERO ( bvec2_new( false, false ) )
/// @brief BVector2 one constant.
#define BVEC2_ONE  ( bvec2_new( true, true ) )

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 2 values.
/// @return Vector with components from array.
attr_always_inline attr_header
struct Vector2 vec2_from_array( const f32 array[2] ) {
    struct Vector2 result;
    result.x = array[0];
    result.y = array[1];
    return result;
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 2 values.
attr_always_inline attr_header
void array_from_vec2( struct Vector2 v, f32* out_array ) {
    out_array[0] = v.x; out_array[1] = v.y;
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline attr_header
struct Vector2 vec2_add(
    struct Vector2 lhs, struct Vector2 rhs
) {
    struct Vector2 result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    return result;
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline attr_header
struct Vector2 vec2_sub(
    struct Vector2 lhs, struct Vector2 rhs
) {
    struct Vector2 result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    return result;
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline attr_header
struct Vector2 vec2_mul( struct Vector2 lhs, f32 rhs ) {
    struct Vector2 result;
    result.x = lhs.x * rhs;
    result.y = lhs.y * rhs;
    return result;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline attr_header
struct Vector2 vec2_mul_vec2(
    struct Vector2 lhs, struct Vector2 rhs
) {
    struct Vector2 result;
    result.x = lhs.x * rhs.x;
    result.y = lhs.y * rhs.y;
    return result;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline attr_header
struct Vector2 vec2_hadamard(
    struct Vector2 lhs, struct Vector2 rhs
) {
    return vec2_mul_vec2( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline attr_header
struct Vector2 vec2_div( struct Vector2 lhs, f32 rhs ) {
    struct Vector2 result;
    result.x = lhs.x / rhs;
    result.y = lhs.y / rhs;
    return result;
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
attr_always_inline attr_header
struct Vector2 vec2_div_vec2( struct Vector2 lhs, struct Vector2 rhs ) {
    struct Vector2 result;
    result.x = lhs.x / rhs.x;
    result.y = lhs.y / rhs.y;
    return result;
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of modulus division.
attr_always_inline attr_header
struct Vector2 vec2_mod( struct Vector2 lhs, f32 rhs ) {
    return vec2_new( f32_mod( lhs.x, rhs ), f32_mod( lhs.y, rhs ) );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of modulus division.
attr_always_inline attr_header
struct Vector2 vec2_mod_vec2( struct Vector2 lhs, struct Vector2 rhs ) {
    return vec2_new( f32_mod( lhs.x, rhs.x ), f32_mod( lhs.y, rhs.y ) );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline attr_header
struct Vector2 vec2_neg( struct Vector2 x ) {
    struct Vector2 result;
    result.x = -x.x;
    result.y = -x.y;
    return result;
}
/// @brief Swap X and Y components.
/// @param x Vector whose components are to be swapped.
/// @return Vector with swapped components.
attr_always_inline attr_header
struct Vector2 vec2_swap( struct Vector2 x ) {
    struct Vector2 result;
    result.x = x.y;
    result.y = x.x;
    return result;
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline attr_header
f32 vec2_hadd( struct Vector2 x ) {
    return x.x + x.y;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline attr_header
f32 vec2_hmul( struct Vector2 x ) {
    return x.x * x.y;
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline attr_header
f32 vec2_dot(
    struct Vector2 lhs, struct Vector2 rhs
) {
    return vec2_hadd( vec2_hadamard( lhs, rhs ) );
}
/// @brief Calculate aspect ratio of X and Y components.
/// @param x Vector to calculate aspect ratio of.
/// @return Aspect ratio between X and Y components.
attr_always_inline attr_header
f32 vec2_aspect_ratio( struct Vector2 x ) {
    return x.x / x.y;
}
/// @brief Get the minimum component in vector.
/// @param x Vector to get minimum of.
/// @return Component with smallest value.
attr_always_inline attr_header
f32 vec2_min( struct Vector2 x ) {
    return x.x < x.y ? x.x : x.y;
}
/// @brief Component-wise minimum value.
/// @param x, y Vectors.
/// @return Vector with minimum value in components.
attr_always_inline attr_header
struct Vector2 vec2_min_vec2( struct Vector2 x, struct Vector2 y ) {
    struct Vector2 result;
    result.x = x.x < y.x ? x.x : y.x;
    result.y = x.y < y.y ? x.y : y.y;
    return result;
}
/// @brief Get the maximum component in vector.
/// @param x Vector to get maximum of.
/// @return Component with largest value.
attr_always_inline attr_header
f32 vec2_max( struct Vector2 x, ... ) {
    return x.x < x.y ? x.y : x.x;
}
/// @brief Component-wise maximum value.
/// @param x, y Vectors.
/// @return Vector with maximum value in components.
attr_always_inline attr_header
struct Vector2 vec2_max_vec2( struct Vector2 x, struct Vector2 y ) {
    struct Vector2 result;
    result.x = x.x < y.x ? y.x : x.x;
    result.y = x.y < y.y ? y.y : x.y;
    return result;
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline attr_header
f32 vec2_length_sqr( struct Vector2 x ) {
    return vec2_dot( x, x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_always_inline attr_header
f32 vec2_length( struct Vector2 x ) {
    return f32_sqrt( vec2_length_sqr( x ) );
}
/// @brief Calculate distance between two points.
/// @param a, b Points to calculate distance of.
/// @return Distance squared.
attr_always_inline attr_header
f32 vec2_distance_sqr( struct Vector2 a, struct Vector2 b ) {
    return vec2_length_sqr( vec2_sub( a, b ) );
}
/// @brief Calculate distance between two points.
/// @param a, b Points to calculate distance of.
/// @return Distance.
attr_always_inline attr_header
f32 vec2_distance( struct Vector2 a, struct Vector2 b ) {
    return vec2_length( vec2_sub( a, b ) );
}
/// @brief Normalize a Vector.
/// @param x Vector to normalize.
/// @return Normalized vector or zero vector if magnitude == 0.
attr_always_inline attr_header
struct Vector2 vec2_normalize( struct Vector2 x ) {
    f32 length_sqr = vec2_length_sqr( x );
    if( length_sqr == 0.0f ) {
        return VEC2_ZERO;
    } else {
        return vec2_div( x, f32_sqrt( length_sqr ) );
    }
}
/// @brief Reflect direction vector off surface.
/// @param d Direction vector to reflect.
/// @param n Normal of the surface to reflect off of.
/// @return Reflected vector.
attr_always_inline attr_header
struct Vector2 vec2_reflect(
    struct Vector2 d, struct Vector2 n
) {
    return vec2_mul( vec2_sub( n, d ), 2.0f * vec2_dot( d, n ) );
}
/// @brief Rotate vector by given angle.
/// @param v     Vector to rotate.
/// @param angle Angle to rotate by (in radians).
/// @return Rotated vector.
attr_always_inline attr_header
struct Vector2 vec2_rotate( struct Vector2 v, f32 angle ) {
    f32 sin = 0.0f, cos = 0.0f;
    f32_sincos( angle, &sin, &cos );
    struct Vector2 a = vec2_new( cos, sin );
    struct Vector2 b = vec2_new( -sin, cos );

    a = vec2_mul( a, v.x );
    b = vec2_mul( b, v.y );

    return vec2_add( a, b );
}
/// @brief Component-wise clamp.
/// @param v        Vector.
/// @param min, max Range.
/// @return Clamped vector.
attr_always_inline attr_header
struct Vector2 vec2_clamp(
    struct Vector2 v, struct Vector2 min, struct Vector2 max
) {
    struct Vector2 result;
    result.x = ( (v.x) < (min.x) ? (min.x) : ( (v.x) > (max.x) ? (max.x) : (v.x) ) );
    result.y = ( (v.y) < (min.y) ? (min.y) : ( (v.y) > (max.y) ? (max.y) : (v.y) ) );
    return result;
}
/// @brief Clamp vector's magnitude to range [min,max].
/// @param v Vector to clamp.
/// @param min_, max_ Range to clamp to, min_ must be < max_.
/// @return Clamped vector.
attr_always_inline attr_header
struct Vector2 vec2_clamp_length(
    const struct Vector2 v, f32 min_, f32 max_
) {
    f32 mag = vec2_length( v );
    if( mag == 0.0f ) {
        return VEC2_ZERO;
    }
    f32 new_length = ( mag < min_ ? min_ : ( mag > max_ ? max_ : mag ) );
    return vec2_mul( vec2_div( v, mag ), new_length );
}
/// @brief Calculate the angle between two vectors.
/// @param a, b Vectors to calculate angle of.
/// @return Angle (in radians) between vectors.
attr_always_inline attr_header
f32 vec2_angle( struct Vector2 a, struct Vector2 b ) {
    return f32_acos( vec2_dot( a, b ) );
}
/// @brief Component-wise abs.
/// @param v Vector.
/// @return Vector with absolute values.
attr_always_inline attr_header
struct Vector2 vec2_abs( struct Vector2 v ) {
    struct Vector2 result;
    result.x = v.x < 0.0f ? -v.x : v.x;
    result.y = v.y < 0.0f ? -v.y : v.y;
    return result;
}
/// @brief Component-wise sign.
/// @param v Vector.
/// @return Vector with sign values.
attr_always_inline attr_header
struct Vector2 vec2_sign( struct Vector2 v ) {
    struct Vector2 result;
    result.x = (v.x > 0.0f) - (f32)(v.x < 0.0f);
    result.y = (v.y > 0.0f) - (f32)(v.y < 0.0f);
    return result;
}
/// @brief Component-wise truncate.
/// @param v Vector to truncate.
/// @return Vector with truncated values.
attr_always_inline attr_header
struct Vector2 vec2_trunc( struct Vector2 v ) {
    struct Vector2 result;
    result.x = (f32)(i32)v.x;
    result.y = (f32)(i32)v.y;
    return result;
}
/// @brief Component-wise floor.
/// @param v Vector to floor.
/// @return Vector with floored values.
attr_always_inline attr_header
struct Vector2 vec2_floor( struct Vector2 v ) {
    struct Vector2 result;
    result.x = (f32)( v.x < 0.0f ? (i32)( v.x - 1.0f ) : (i32)( v.x ) );
    result.y = (f32)( v.y < 0.0f ? (i32)( v.y - 1.0f ) : (i32)( v.y ) );
    return result;
}
/// @brief Component-wise ceil.
/// @param v Vector to ceil.
/// @return Vector with ceiled values.
attr_always_inline attr_header
struct Vector2 vec2_ceil( struct Vector2 v ) {
    struct Vector2 result;
    result.x = (f32)( v.x < 0.0f ? (i32)( v.x ) : (i32)( v.x + 1.0f ) );
    result.y = (f32)( v.y < 0.0f ? (i32)( v.y ) : (i32)( v.y + 1.0f ) );
    return result;
}
/// @brief Component-wise round.
/// @param v Vector to round.
/// @return Vector with rounded values.
attr_always_inline attr_header
struct Vector2 vec2_round( struct Vector2 v ) {
    struct Vector2 result;
    result.x = (f32)( v.x < 0.0f ? (i32)( v.x - 0.5f ) : (i32)( v.x + 0.5f ) );
    result.y = (f32)( v.y < 0.0f ? (i32)( v.y - 0.5f ) : (i32)( v.y + 0.5f ) );
    return result;
}
/// @brief Get fractional part.
/// @param v Vector.
/// @return Vector with fractional part.
attr_always_inline attr_header
struct Vector2 vec2_fract( struct Vector2 v ) {
    return vec2_sub( v, vec2_floor(v) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range [a,b].
attr_always_inline attr_header
struct Vector2 vec2_lerp(
    struct Vector2 a, struct Vector2 b, f32 t
) {
    return vec2_add( vec2_mul( a, 1.0f - t ), vec2_mul( b, t ) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range [a,b].
attr_always_inline attr_header
struct Vector2 vec2_lerp_vec2(
    struct Vector2 a, struct Vector2 b, struct Vector2 t
) {
    return vec2_new( f32_lerp( a.x, b.x, t.x ), f32_lerp( a.y, b.y, t.y ) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range [a,b].
attr_always_inline attr_header
struct Vector2 vec2_mix(
    struct Vector2 a, struct Vector2 b, f32 t
) {
    return vec2_lerp( a, b, t );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range [a,b].
attr_always_inline attr_header
struct Vector2 vec2_mix_vec2(
    struct Vector2 a, struct Vector2 b, struct Vector2 t
) {
    return vec2_lerp_vec2( a, b, t );
}
/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline attr_header
struct Vector2 vec2_step_vec2( struct Vector2 edge, struct Vector2 x ) {
    return vec2_new( f32_step( edge.x, x.x ), f32_step( edge.y, x.y ) );
}
/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline attr_header
struct Vector2 vec2_step( f32 edge, struct Vector2 x ) {
    return vec2_step_vec2( vec2_new( edge, edge ), x );
}
/// @brief Smooth step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header
struct Vector2 vec2_smoothstep(
    f32 edge0, f32 edge1, struct Vector2 x
) {
    return vec2_new(
        f32_smoothstep( edge0, edge1, x.x ),
        f32_smoothstep( edge0, edge1, x.y ) );
}
/// @brief Smooth step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header
struct Vector2 vec2_smoothstep_vec2(
    struct Vector2 edge0, struct Vector2 edge1, struct Vector2 x
) {
    return vec2_new(
        f32_smoothstep( edge0.x, edge1.x, x.x ),
        f32_smoothstep( edge0.y, edge1.y, x.y ) );
}
/// @brief Smoother step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header
struct Vector2 vec2_smootherstep(
    f32 edge0, f32 edge1, struct Vector2 x
) {
    return vec2_new(
        f32_smootherstep( edge0, edge1, x.x ),
        f32_smootherstep( edge0, edge1, x.y ) );
}
/// @brief Smoother step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header
struct Vector2 vec2_smootherstep_vec2(
    struct Vector2 edge0, struct Vector2 edge1, struct Vector2 x
) {
    return vec2_new(
        f32_smootherstep( edge0.x, edge1.x, x.x ),
        f32_smootherstep( edge0.y, edge1.y, x.y ) );
}
/// @brief Check if vector components are NaN.
/// @param x Vector.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 vec2_isnan( struct Vector2 x ) {
    return bvec2_new( f32_isnan( x.x ), f32_isnan( x.y ) );
}
/// @brief Check if vector components are infinite.
/// @param x Vector.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 vec2_isinf( struct Vector2 x ) {
    return bvec2_new( f32_isinf( x.x ), f32_isinf( x.y ) );
}
/// @brief Convert degrees to radians.
/// @param degrees Angles in degrees.
/// @return Angles in radians.
attr_always_inline attr_header
struct Vector2 vec2_radians( struct Vector2 degrees ) {
    return vec2_mul( degrees, F32_TO_RAD );
}
/// @brief Convert radians to degrees.
/// @param radians Angles in radians.
/// @return Angles in degrees.
attr_always_inline attr_header
struct Vector2 vec2_degrees( struct Vector2 radians ) {
    return vec2_mul( radians, F32_TO_DEG );
}
/// @brief Calculate sine of x.
/// @param angle Value to get sine of.
/// @return Sine of angle.
attr_always_inline attr_header
struct Vector2 vec2_sin( struct Vector2 angle ) {
    return vec2_new( f32_sin(angle.x), f32_sin(angle.y) );
}
/// @brief Calculate cosine of x.
/// @param angle Value to get cosine of.
/// @return Cosine of angle.
attr_always_inline attr_header
struct Vector2 vec2_cos( struct Vector2 angle ) {
    return vec2_new( f32_cos(angle.x), f32_cos(angle.y) );
}
/// @brief Calculate tangent of x.
/// @param angle Value to get tangent of.
/// @return Tangent of angle.
/// @warning Returns NaN if cosine of angle is zero.
attr_always_inline attr_header
struct Vector2 vec2_tan( struct Vector2 angle ) {
    return vec2_new( f32_tan(angle.x), f32_tan(angle.y) );
}
/// @brief Calculate sine and cosine of x.
/// @param x Angle to calculate sine and cosine of (in radians).
/// @return Vector with sine and cosine of x.
attr_always_inline attr_header
struct Vector2 sincos_vec2( f32 x ) {
    f32 s, c;
    f32_sincos( x, &s, &c );
    return vec2_new( s, c );
}
/// @brief Calculate arcsine of x.
///
/// Does not produce NaN when outside valid range.
/// @param angle Value to get arcsine of.
/// @return Arcsine of x clamped to [-Pi,Pi].
/// @see #F32_PI
/// @see #asin()
attr_always_inline attr_header
struct Vector2 vec2_asin( struct Vector2 angle ) {
    return vec2_new( f32_asin_real(angle.x), f32_asin_real(angle.y) );
}
/// @brief Calculate arccosine of x.
/// @param angle Value to get arccosine of.
/// @return Arccosine of x.
attr_always_inline attr_header
struct Vector2 vec2_acos( struct Vector2 angle ) {
    return vec2_new( f32_acos(angle.x), f32_acos(angle.y) );
}
/// @brief Calculate arctangent of x.
/// @param angle Value to get arctangent of.
/// @return Arctangent of x.
attr_always_inline attr_header
struct Vector2 vec2_atan( struct Vector2 angle ) {
    return vec2_new( f32_atan(angle.x), f32_atan(angle.y) );
}
/// @brief Calculate 2 component arctangent of y and x.
/// @param y Value to get arctangent of.
/// @param x Value to get arctangent of.
/// @return Arctangent of y and x.
attr_always_inline attr_header
struct Vector2 vec2_atan2( struct Vector2 y, struct Vector2 x ) {
    return vec2_new( f32_atan2(y.x, x.x), f32_atan2(y.y, x.y) );
}
/// @brief Raise base to the power of exponent.
/// @param base Number to raise.
/// @param exp  Power to raise to.
/// @return Result.
attr_always_inline attr_header
struct Vector2 vec2_pow( struct Vector2 base, f32 exp ) {
    return vec2_new( f32_pow( base.x, exp ), f32_pow( base.y, exp ) );
}
/// @brief Raise base to the power of exponent.
/// @param base Number to raise.
/// @param exp  Power to raise to.
/// @return Result.
attr_always_inline attr_header
struct Vector2 vec2_pow_vec2( struct Vector2 base, struct Vector2 exp ) {
    return vec2_new( f32_pow( base.x, exp.x ), f32_pow( base.y, exp.y ) );
}
/// @brief Raise e to the power of x.
/// @param x Exponent.
/// @return Result.
attr_always_inline attr_header
struct Vector2 vec2_exp( struct Vector2 x ) {
    return vec2_new( f32_exp( x.x ), f32_exp( x.y ) );
}
/// @brief Raise 2 to the power of x.
/// @param x Exponent.
/// @return Result.
attr_always_inline attr_header
struct Vector2 vec2_exp2( struct Vector2 x ) {
    return vec2_new( f32_exp2( x.x ), f32_exp2( x.y ) );
}
/// @brief Calculate natural logarithm.
/// @param x Value to get natural logarithm of.
/// @return Natural logarithm.
attr_always_inline attr_header
struct Vector2 vec2_ln( struct Vector2 x ) {
    return vec2_new( f32_ln( x.x ), f32_ln( x.y ) );
}
/// @brief Calculate logarithm base 2.
/// @param x Value to get logarithm base 2 of.
/// @return Logarithm base 2.
attr_always_inline attr_header
struct Vector2 vec2_log2( struct Vector2 x ) {
    return vec2_new( f32_log2( x.x ), f32_log2( x.y ) );
}
/// @brief Calculate logarithm base 10.
/// @param x Value to get logarithm base 10 of.
/// @return Logarithm base 10.
attr_always_inline attr_header
struct Vector2 vec2_log10( struct Vector2 x ) {
    return vec2_new( f32_log10( x.x ), f32_log10( x.y ) );
}
/// @brief Calculate square root.
/// @param x Value to get square root of.
/// @return Square root.
attr_always_inline attr_header
struct Vector2 vec2_sqrt( struct Vector2 x ) {
    return vec2_new( f32_sqrt( x.x ), f32_sqrt( x.y ) );
}
/// @brief Calculate reciprocal square root.
/// @param x Value to get reciprocal square root of.
/// @return Reciprocal square root.
attr_always_inline attr_header
struct Vector2 vec2_inversesqrt( struct Vector2 x ) {
    return vec2_new( f32_inversesqrt( x.x ), f32_inversesqrt( x.y ) );
}
/// @brief Calculate cube root.
/// @param x Value to get cube root of.
/// @return Cube root.
attr_always_inline attr_header
struct Vector2 vec2_cbrt( struct Vector2 x ) {
    return vec2_new( f32_cbrt( x.x ), f32_cbrt( x.y ) );
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if the square magnitude of a - b is < F32_EPSILON.
/// @see #F32_EPSILON
attr_always_inline attr_header
b32 vec2_cmp( struct Vector2 a, struct Vector2 b ) {
    return vec2_length_sqr( vec2_sub( a, b ) ) < F32_EPSILON;
}
/// @brief Check if less than, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 vec2_lt( struct Vector2 a, struct Vector2 b ) {
    return bvec2_new( a.x < b.x, a.y < b.y );
}
/// @brief Check if less than or equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 vec2_lteq( struct Vector2 a, struct Vector2 b ) {
    return bvec2_new( a.x <= b.x, a.y <= b.y );
}
/// @brief Check if greater than, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 vec2_gt( struct Vector2 a, struct Vector2 b ) {
    return bvec2_new( a.x > b.x, a.y > b.y );
}
/// @brief Check if greater than or equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 vec2_gteq( struct Vector2 a, struct Vector2 b ) {
    return bvec2_new( a.x >= b.x, a.y >= b.y );
}
/// @brief Check if equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 vec2_eq( struct Vector2 a, struct Vector2 b ) {
    return bvec2_new( f32_cmp( a.x, b.x ), f32_cmp( a.y, b.y ) );
}
/// @brief Check if not equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 vec2_neq( struct Vector2 a, struct Vector2 b ) {
    return bvec2_new( !f32_cmp( a.x, b.x ), !f32_cmp( a.y, b.y ) );
}

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 2 values.
/// @return Vector with components from array.
attr_always_inline attr_header
struct IVector2 ivec2_from_array( const i32 array[2] ) {
    struct IVector2 result;
    result.x = array[0];
    result.y = array[1];
    return result;
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 2 values.
attr_always_inline attr_header
void array_from_ivec2( struct IVector2 v, i32* out_array ) {
    out_array[0] = v.x; out_array[1] = v.y;
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline attr_header
struct IVector2 ivec2_add(
    struct IVector2 lhs, struct IVector2 rhs
) {
    struct IVector2 result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    return result;
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline attr_header
struct IVector2 ivec2_sub(
    struct IVector2 lhs, struct IVector2 rhs
) {
    struct IVector2 result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    return result;
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline attr_header
struct IVector2 ivec2_mul( struct IVector2 lhs, i32 rhs ) {
    struct IVector2 result;
    result.x = lhs.x * rhs;
    result.y = lhs.y * rhs;
    return result;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline attr_header
struct IVector2 ivec2_mul_ivec2(
    struct IVector2 lhs, struct IVector2 rhs
) {
    struct IVector2 result;
    result.x = lhs.x * rhs.x;
    result.y = lhs.y * rhs.y;
    return result;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline attr_header
struct IVector2 ivec2_hadamard(
    struct IVector2 lhs, struct IVector2 rhs
) {
    return ivec2_mul_ivec2( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline attr_header
struct IVector2 ivec2_div( struct IVector2 lhs, i32 rhs ) {
    struct IVector2 result;
    result.x = lhs.x / rhs;
    result.y = lhs.y / rhs;
    return result;
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
attr_always_inline attr_header
struct IVector2 ivec2_div_ivec2( struct IVector2 lhs, struct IVector2 rhs ) {
    struct IVector2 result;
    result.x = lhs.x / rhs.x;
    result.y = lhs.y / rhs.y;
    return result;
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of modulus division.
attr_always_inline attr_header
struct IVector2 ivec2_mod( struct IVector2 lhs, i32 rhs ) {
    return ivec2_new( lhs.x % rhs, lhs.y % rhs );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of modulus division.
attr_always_inline attr_header
struct IVector2 ivec2_mod_ivec2( struct IVector2 lhs, struct IVector2 rhs ) {
    return ivec2_new( lhs.x % rhs.x, lhs.y % rhs.y );
}
/// @brief Negate components of a Vector2.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline attr_header
struct IVector2 ivec2_neg( struct IVector2 x ) {
    struct IVector2 result;
    result.x = -x.x;
    result.y = -x.y;
    return result;
}
/// @brief Swap X and Y components.
/// @param x Vector whose components are to be swapped.
/// @return Vector with swapped components.
attr_always_inline attr_header
struct IVector2 ivec2_swap( struct IVector2 x ) {
    struct IVector2 result;
    result.x = x.y;
    result.y = x.x;
    return result;
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline attr_header
i32 ivec2_hadd( struct IVector2 x ) {
    return x.x + x.y;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline attr_header
i32 ivec2_hmul( struct IVector2 x ) {
    return x.x * x.y;
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline attr_header
f32 ivec2_dot(
    struct IVector2 lhs, struct IVector2 rhs
) {
    struct Vector2 _lhs = vec2_new( (f32)lhs.x, (f32)lhs.y );
    struct Vector2 _rhs = vec2_new( (f32)rhs.x, (f32)rhs.y );

    return vec2_dot( _lhs, _rhs );
}
/// @brief Calculate aspect ratio of X and Y components.
/// @param x Vector to calculate aspect ratio of.
/// @return Aspect ratio between X and Y components.
attr_always_inline attr_header
f32 ivec2_aspect_ratio( struct IVector2 x ) {
    return (f32)x.x / (f32)x.y;
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline attr_header
f32 ivec2_length_sqr( struct IVector2 x ) {
    struct Vector2 _x = vec2_new( (f32)x.x, (f32)x.y );
    return vec2_length_sqr( _x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_always_inline attr_header
f32 ivec2_length( struct IVector2 x ) {
    struct Vector2 _x = vec2_new( (f32)x.x, (f32)x.y );
    return vec2_length( _x );
}
/// @brief Minimum value of x and y.
/// @param x Vector.
/// @return Smaller component of vector.
attr_always_inline attr_header
i32 ivec2_min( struct IVector2 x ) {
    return x.x < x.y ? x.x : x.y;
}
/// @brief Component-wise minimum value.
/// @param x, y Vectors.
/// @return Vector with minimum value in components.
attr_always_inline attr_header
struct IVector2 ivec2_min_ivec2( struct IVector2 x, struct IVector2 y ) {
    struct IVector2 result;
    result.x = x.x < y.x ? x.x : y.x;
    result.y = x.y < y.y ? x.y : y.y;
    return result;
}
/// @brief Maximum value of x and y.
/// @param x Vector.
/// @return Larger component of vector.
attr_always_inline attr_header
i32 ivec2_max( struct IVector2 x ) {
    return x.x < x.y ? x.y : x.x;
}
/// @brief Component-wise maximum value.
/// @param x, y Vectors.
/// @return Vector with maximum value in components.
attr_always_inline attr_header
struct IVector2 ivec2_max_ivec2( struct IVector2 x, struct IVector2 y ) {
    struct IVector2 result;
    result.x = x.x < y.x ? y.x : x.x;
    result.y = x.y < y.y ? y.y : x.y;
    return result;
}
/// @brief Component-wise clamp.
/// @param v        Vector.
/// @param min, max Range.
/// @return Clamped vector.
attr_always_inline attr_header
struct IVector2 ivec2_clamp(
    struct IVector2 v, struct IVector2 min, struct IVector2 max
) {
    struct IVector2 result;
    result.x = ( (v.x) < (min.x) ? (min.x) : ( (v.x) > (max.x) ? (max.x) : (v.x) ) );
    result.y = ( (v.y) < (min.y) ? (min.y) : ( (v.y) > (max.y) ? (max.y) : (v.y) ) );
    return result;
}
/// @brief Component-wise abs.
/// @param x Vector.
/// @return Vector with absolute values.
attr_always_inline attr_header
struct IVector2 ivec2_abs( struct IVector2 x ) {
    struct IVector2 result;
    result.x = x.x < 0 ? -x.x : x.x;
    result.y = x.y < 0 ? -x.y : x.y;
    return result;
}
/// @brief Component-wise sign.
/// @param v Vector.
/// @return Vector with sign values.
attr_always_inline attr_header
struct IVector2 ivec2_sign( struct IVector2 v ) {
    struct IVector2 result;
    result.x = (v.x > 0) - (v.x < 0);
    result.y = (v.y > 0) - (v.y < 0);
    return result;
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if components in vectors are equal.
attr_always_inline attr_header
b32 ivec2_cmp( struct IVector2 a, struct IVector2 b ) {
    return
        a.x == b.x &&
        a.y == b.y;
}
/// @brief Check if less than, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 ivec2_lt( struct IVector2 a, struct IVector2 b ) {
    return bvec2_new( a.x < b.x, a.y < b.y );
}
/// @brief Check if greater than, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 ivec2_gt( struct IVector2 a, struct IVector2 b ) {
    return bvec2_new( a.x > b.x, a.y > b.y );
}
/// @brief Check if less than or equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 ivec2_lteq( struct IVector2 a, struct IVector2 b ) {
    return bvec2_new( a.x <= b.x, a.y <= b.y );
}
/// @brief Check if greater than or equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 ivec2_gteq( struct IVector2 a, struct IVector2 b ) {
    return bvec2_new( a.x >= b.x, a.y >= b.y );
}
/// @brief Check if equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 ivec2_eq( struct IVector2 a, struct IVector2 b ) {
    return bvec2_new( (a.x == b.x), (a.y == b.y) );
}
/// @brief Check if not equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 ivec2_neq( struct IVector2 a, struct IVector2 b ) {
    return bvec2_new( !(a.x == b.x), !(a.y == b.y) );
}

/// @brief Check if equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 bvec2_eq( struct BVector2 a, struct BVector2 b ) {
    return bvec2_new( (a.x == b.x), (a.y == b.y) );
}
/// @brief Check if not equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector2 bvec2_neq( struct BVector2 a, struct BVector2 b ) {
    return bvec2_new( !(a.x == b.x), !(a.y == b.y) );
}
/// @brief Check if any component of vector is true.
/// @param x Vector to check.
/// @return
///     - true  : Any component is true.
///     - false : All components false.
attr_always_inline attr_header
b32 bvec2_any( struct BVector2 x ) {
    return x.x || x.y;
}
/// @brief Check if all components of vector are true.
/// @param x Vector to check.
/// @return
///     - true  : All components are true.
///     - false : One or more components are false.
attr_always_inline attr_header
b32 bvec2_all( struct BVector2 x ) {
    return x.x && x.y;
}
/// @brief Not components.
/// @param x Vector to check.
/// @return Booleans notted.
attr_always_inline attr_header
struct BVector2 bvec2_flip( struct BVector2 x ) {
    return bvec2_new( !x.x, !x.y );
}

#if !defined(CORE_CPP_MATH_VECTOR2_HPP)
    #include "core/cpp/math/vector2.hpp"
#endif

#endif /* header guard */
