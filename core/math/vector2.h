#if !defined(CORE_MATH_VECTOR2_H)
#define CORE_MATH_VECTOR2_H
/**
 * @file   vector2.h
 * @brief  Vector2 definition.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 28, 2024
*/
#include "core/defines.h"
#include "core/types.h"
#include "core/attributes.h"
#include "core/constants.h"
#include "core/macros.h" // IWYU pragma: export

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

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
struct UVector2 {
    union {
        struct {
            u32 x;
            u32 y;
        };
        u32 array[2];
    };
};
struct DVector2 {
    union {
        struct {
            f64 x;
            f64 y;
        };
        f64 array[2];
    };
};
struct BVector2 {
    union {
        struct {
            b32 x;
            b32 y;
        };
        b32 array[2];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief 2 Component 32-bit Floating Point Vector.
    /// @see struct Vector2
    typedef struct Vector2 vec2;
    /// @brief 2 Component 32-bit Signed Integer Vector.
    /// @see struct IVector2
    typedef struct IVector2 ivec2;
#endif
typedef struct UVector2 uvec2;
typedef struct DVector2 dvec2;
typedef struct BVector2 bvec2;

#if defined(CORE_DOXYGEN) && !defined(CORE_CPLUSPLUS)
    /// @brief Construct a Vector2.
    /// @param x, y (f32) Components.
    /// @return Vector2.
    #define vec2( x, y )
    /// @brief Construct an IVector2.
    /// @param x, y (i32) Components.
    /// @return IVector2.
    #define ivec2( x, y )
#else

#if defined(CORE_CPLUSPLUS)
    #define vec2_new( _x, _y )\
        Vector2{ .array={ (_x), (_y) } }
    #define ivec2_new( _x, _y )\
        IVector2{ .array={ (_x), (_y) } }
#else /* C++ constructors */
    #define vec2( x, y )\
        (struct Vector2){ .array={ (x), (y) } }
    #define ivec2( x, y )\
        (struct IVector2){ .array={ (x), (y) } }
    #define vec2_new(...)  vec2(__VA_ARGS__)
    #define ivec2_new(...) ivec2(__VA_ARGS__)
#endif /* C constructors */

#endif /* Doxygen */

/// @brief Contruct a new Vector2 with identical components.
/// @param s (f32) Value for components.
/// @return Vector2.
#define vec2_set( s )\
    vec2_new( s, s )
/// @brief Contruct a new IVector2 with identical components.
/// @param x (i32) Value for components.
/// @return IVector2.
#define ivec2_set( s )\
    ivec2_new( s, s )

/// @brief Vector2 zero constant.
#define VEC2_ZERO  ( vec2_set( 0.0f ) )
/// @brief Vector2 one constant.
#define VEC2_ONE   ( vec2_set( 1.0f ) )
/// @brief Vector2 left constant.
#define VEC2_LEFT  ( vec2_new( -1.0f, 0.0f ) )
/// @brief Vector2 right constant.
#define VEC2_RIGHT ( vec2_new(  1.0f, 0.0f ) )
/// @brief Vector2 up constant.
#define VEC2_UP    ( vec2_new(  0.0f, 1.0f ) )
/// @brief Vector2 down constant.
#define VEC2_DOWN  ( vec2_new(  0.0f,-1.0f ) )

/// @brief IVector2 zero constant.
#define IVEC2_ZERO  ( ivec2_set(0) )
/// @brief IVector2 one constant.
#define IVEC2_ONE   ( ivec2_set(1) )
/// @brief IVector2 left constant.
#define IVEC2_LEFT  ( ivec2_new( -1, 0 ) )
/// @brief IVector2 right constant.
#define IVEC2_RIGHT ( ivec2_new(  1, 0 ) )
/// @brief IVector2 up constant.
#define IVEC2_UP    ( ivec2_new(  0, 1 ) )
/// @brief IVector2 down constant.
#define IVEC2_DOWN  ( ivec2_new(  0,-1 ) )

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 2 values.
/// @return Vector with components from array.
attr_always_inline
attr_header struct Vector2 vec2_from_array( const f32 array[2] ) {
    struct Vector2 result;
    result.x = array[0];
    result.y = array[1];
    return result;
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 2 values.
attr_always_inline
attr_header void vec2_to_array( struct Vector2 v, f32* out_array ) {
    out_array[0] = v.x; out_array[1] = v.y;
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline
attr_header struct Vector2 vec2_add(
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
attr_always_inline
attr_header struct Vector2 vec2_sub(
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
attr_always_inline
attr_header struct Vector2 vec2_mul( struct Vector2 lhs, f32 rhs ) {
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
attr_always_inline
attr_header struct Vector2 vec2_mul_vec2(
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
attr_always_inline
attr_header struct Vector2 vec2_hadamard(
    struct Vector2 lhs, struct Vector2 rhs
) {
    return vec2_mul_vec2( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline
attr_header struct Vector2 vec2_div( struct Vector2 lhs, f32 rhs ) {
    struct Vector2 result;
    result.x = lhs.x / rhs;
    result.y = lhs.y / rhs;
    return result;
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
attr_always_inline
attr_header struct Vector2 vec2_div_vec2( struct Vector2 lhs, struct Vector2 rhs ) {
    struct Vector2 result;
    result.x = lhs.x / rhs.x;
    result.y = lhs.y / rhs.y;
    return result;
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline
attr_header struct Vector2 vec2_neg( struct Vector2 x ) {
    struct Vector2 result;
    result.x = -x.x;
    result.y = -x.y;
    return result;
}
/// @brief Swap X and Y components.
/// @param x Vector whose components are to be swapped.
/// @return Vector with swapped components.
attr_always_inline
attr_header struct Vector2 vec2_swap( struct Vector2 x ) {
    struct Vector2 result;
    result.x = x.y;
    result.y = x.x;
    return result;
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline
attr_header f32 vec2_hadd( struct Vector2 x ) {
    return x.x + x.y;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline
attr_header f32 vec2_hmul( struct Vector2 x ) {
    return x.x * x.y;
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline
attr_header f32 vec2_dot(
    struct Vector2 lhs, struct Vector2 rhs
) {
    return vec2_hadd( vec2_hadamard( lhs, rhs ) );
}
/// @brief Calculate aspect ratio of X and Y components.
/// @param x Vector to calculate aspect ratio of.
/// @return Aspect ratio between X and Y components.
attr_always_inline
attr_header f32 vec2_aspect_ratio( struct Vector2 x ) {
    return x.x / x.y;
}
/// @brief Get the maximum component in vector.
/// @param x Vector to get maximum of.
/// @return Component with largest value.
attr_always_inline
attr_header f32 vec2_hmax( struct Vector2 x ) {
    return x.x < x.y ? x.y : x.x;
}
/// @brief Component-wise maximum value.
/// @param x, y Vectors.
/// @return Vector with maximum value in components.
attr_always_inline
attr_header struct Vector2 vec2_max( struct Vector2 x, struct Vector2 y ) {
    struct Vector2 result;
    result.x = x.x < y.x ? y.x : x.x;
    result.y = x.y < y.y ? y.y : x.y;
    return result;
}
/// @brief Get the minimum component in vector.
/// @param x Vector to get minimum of.
/// @return Component with smallest value.
attr_always_inline
attr_header f32 vec2_hmin( struct Vector2 x ) {
    return x.x < x.y ? x.x : x.y;
}
/// @brief Component-wise minimum value.
/// @param x, y Vectors.
/// @return Vector with minimum value in components.
attr_always_inline
attr_header struct Vector2 vec2_min( struct Vector2 x, struct Vector2 y ) {
    struct Vector2 result;
    result.x = x.x < y.x ? x.x : y.x;
    result.y = x.y < y.y ? x.y : y.y;
    return result;
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline
attr_header f32 vec2_length_sqr( struct Vector2 x ) {
    return vec2_dot( x, x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_core_api f32 vec2_length( struct Vector2 x );
/// @brief Calculate distance between two points.
/// @param a, b Points to calculate distance of.
/// @return Distance squared.
attr_always_inline
attr_header f32 vec2_distance_sqr( struct Vector2 a, struct Vector2 b ) {
    return vec2_length_sqr( vec2_sub( a, b ) );
}
/// @brief Calculate distance between two points.
/// @param a, b Points to calculate distance of.
/// @return Distance.
attr_always_inline
attr_header f32 vec2_distance( struct Vector2 a, struct Vector2 b ) {
    return vec2_length( vec2_sub( a, b ) );
}
/// @brief Normalize a Vector.
/// @param x Vector to normalize.
/// @return Normalized vector or zero vector if magnitude == 0.
attr_core_api struct Vector2 vec2_normalize( struct Vector2 x );
/// @brief Reflect direction vector off surface.
/// @param direction Direction vector to reflect.
/// @param normal    Normal of the surface to reflect off of.
/// @return Reflected vector.
attr_always_inline
attr_header struct Vector2 vec2_reflect(
    struct Vector2 direction, struct Vector2 normal
) {
    return vec2_mul( vec2_sub( normal, direction ), 2.0f * vec2_dot( direction, normal ) );
}
/// @brief Rotate vector by given angle.
/// @param v Vector to rotate.
/// @param angle Angle to rotate by (in radians).
/// @return Rotated vector.
attr_core_api struct Vector2 vec2_rotate( struct Vector2 v, f32 angle );
/// @brief Component-wise clamp.
/// @param v        Vector.
/// @param min, max Range.
/// @return Clamped vector.
attr_always_inline
attr_header struct Vector2 vec2_clamp(
    struct Vector2 v, struct Vector2 min, struct Vector2 max
) {
    struct Vector2 result;
    result.x = ( (v.x) < (min.x) ? (min.x) : ( (v.x) > (max.x) ? (max.x) : (v.x) ) );
    result.y = ( (v.y) < (min.y) ? (min.y) : ( (v.y) > (max.y) ? (max.y) : (v.y) ) );
    return result;
}
/// @brief Clamp vector's magnitude to range min -> max.
/// @param v Vector to clamp.
/// @param min_, max_ Range to clamp to, min_ must be < max_.
/// @return Clamped vector.
attr_always_inline
attr_header struct Vector2 vec2_clamp_length(
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
attr_core_api f32 vec2_angle( struct Vector2 a, struct Vector2 b );
/// @brief Component-wise abs.
/// @param v Vector.
/// @return Vector with absolute values.
attr_always_inline
attr_header struct Vector2 vec2_abs( struct Vector2 v ) {
    struct Vector2 result;
    result.x = v.x < 0.0f ? -v.x : v.x;
    result.y = v.y < 0.0f ? -v.y : v.y;
    return result;
}
/// @brief Component-wise sign.
/// @param v Vector.
/// @return Vector with sign values.
attr_always_inline
attr_header struct Vector2 vec2_sign( struct Vector2 v ) {
    struct Vector2 result;
    result.x = (v.x > 0.0f) - (f32)(v.x < 0.0f);
    result.y = (v.y > 0.0f) - (f32)(v.y < 0.0f);
    return result;
}
/// @brief Component-wise truncate.
/// @param v Vector to truncate.
/// @return Vector with truncated values.
attr_always_inline
attr_header struct Vector2 vec2_trunc( struct Vector2 v ) {
    struct Vector2 result;
    result.x = (f32)(i32)v.x;
    result.y = (f32)(i32)v.y;
    return result;
}
/// @brief Component-wise floor.
/// @param v Vector to floor.
/// @return Vector with floored values.
attr_always_inline
attr_header struct Vector2 vec2_floor( struct Vector2 v ) {
    struct Vector2 result;
    result.x = (f32)( v.x < 0.0f ? (i32)( v.x - 1.0f ) : (i32)( v.x ) );
    result.y = (f32)( v.y < 0.0f ? (i32)( v.y - 1.0f ) : (i32)( v.y ) );
    return result;
}
/// @brief Component-wise ceil.
/// @param v Vector to ceil.
/// @return Vector with ceiled values.
attr_always_inline
attr_header struct Vector2 vec2_ceil( struct Vector2 v ) {
    struct Vector2 result;
    result.x = (f32)( v.x < 0.0f ? (i32)( v.x ) : (i32)( v.x + 1.0f ) );
    result.y = (f32)( v.y < 0.0f ? (i32)( v.y ) : (i32)( v.y + 1.0f ) );
    return result;
}
/// @brief Component-wise round.
/// @param v Vector to round.
/// @return Vector with rounded values.
attr_always_inline
attr_header struct Vector2 vec2_round( struct Vector2 v ) {
    struct Vector2 result;
    result.x = (f32)( v.x < 0.0f ? (i32)( v.x - 0.5f ) : (i32)( v.x + 0.5f ) );
    result.y = (f32)( v.y < 0.0f ? (i32)( v.y - 0.5f ) : (i32)( v.y + 0.5f ) );
    return result;
}
/// @brief Get fractional part.
/// @param v Vector.
/// @return Vector with fractional part.
attr_always_inline
attr_header struct Vector2 vec2_fract( struct Vector2 v ) {
    return vec2_sub( v, vec2_round(v) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_core_api struct Vector2 vec2_lerp(
    struct Vector2 a, struct Vector2 b, f32 t );
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
#define vec2_mix vec2_lerp
/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline
attr_header struct Vector2 vec2_step( struct Vector2 edge, struct Vector2 x ) {
    struct Vector2 result;
    result.x = x.x < edge.x ? 0.0 : 1.0;
    result.y = x.y < edge.y ? 0.0 : 1.0;
    return result;
}
/// @brief Smooth step interpolation.
/// @param a, b Range of interpolation.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_core_api struct Vector2 vec2_smoothstep(
    struct Vector2 a, struct Vector2 b, f32 t );
/// @brief Smoother step interpolation.
/// @param a, b Range of interpolation.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_core_api struct Vector2 vec2_smootherstep(
    struct Vector2 a, struct Vector2 b, f32 t );
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if the square magnitude of a - b is < F32_EPSILON.
/// @see #F32_EPSILON
attr_always_inline
attr_header b32 vec2_cmp( struct Vector2 a, struct Vector2 b ) {
    return vec2_length_sqr( vec2_sub( a, b ) ) < F32_EPSILON;
}

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 2 values.
/// @return Vector with components from array.
attr_always_inline
attr_header struct IVector2 ivec2_from_array( const i32 array[2] ) {
    struct IVector2 result;
    result.x = array[0];
    result.y = array[1];
    return result;
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 2 values.
attr_always_inline
attr_header void ivec2_to_array( struct IVector2 v, i32* out_array ) {
    out_array[0] = v.x; out_array[1] = v.y;
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline
attr_header struct IVector2 ivec2_add(
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
attr_always_inline
attr_header struct IVector2 ivec2_sub(
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
attr_always_inline
attr_header struct IVector2 ivec2_mul( struct IVector2 lhs, i32 rhs ) {
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
attr_always_inline
attr_header struct IVector2 ivec2_mul_ivec2(
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
attr_always_inline
attr_header struct IVector2 ivec2_hadamard(
    struct IVector2 lhs, struct IVector2 rhs
) {
    return ivec2_mul_ivec2( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline
attr_header struct IVector2 ivec2_div( struct IVector2 lhs, i32 rhs ) {
    struct IVector2 result;
    result.x = lhs.x / rhs;
    result.y = lhs.y / rhs;
    return result;
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
attr_always_inline
attr_header struct IVector2 ivec2_div_ivec2( struct IVector2 lhs, struct IVector2 rhs ) {
    struct IVector2 result;
    result.x = lhs.x / rhs.x;
    result.y = lhs.y / rhs.y;
    return result;
}
/// @brief Negate components of a Vector2.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline
attr_header struct IVector2 ivec2_neg( struct IVector2 x ) {
    struct IVector2 result;
    result.x = -x.x;
    result.y = -x.y;
    return result;
}
/// @brief Swap X and Y components.
/// @param x Vector whose components are to be swapped.
/// @return Vector with swapped components.
attr_always_inline
attr_header struct IVector2 ivec2_swap( struct IVector2 x ) {
    struct IVector2 result;
    result.x = x.y;
    result.y = x.x;
    return result;
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline
attr_header i32 ivec2_hadd( struct IVector2 x ) {
    return x.x + x.y;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline
attr_header i32 ivec2_hmul( struct IVector2 x ) {
    return x.x * x.y;
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline
attr_header f32 ivec2_dot(
    struct IVector2 lhs, struct IVector2 rhs
) {
    struct Vector2 _lhs = vec2_new( (f32)lhs.x, (f32)lhs.y );
    struct Vector2 _rhs = vec2_new( (f32)rhs.x, (f32)rhs.y );

    return vec2_dot( _lhs, _rhs );
}
/// @brief Calculate aspect ratio of X and Y components.
/// @param x Vector to calculate aspect ratio of.
/// @return Aspect ratio between X and Y components.
attr_always_inline
attr_header f32 ivec2_aspect_ratio( struct IVector2 x ) {
    return (f32)x.x / (f32)x.y;
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline
attr_header f32 ivec2_length_sqr( struct IVector2 x ) {
    struct Vector2 _x = vec2_new( (f32)x.x, (f32)x.y );
    return vec2_length_sqr( _x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_always_inline
attr_header f32 ivec2_length( struct IVector2 x ) {
    struct Vector2 _x = vec2_new( (f32)x.x, (f32)x.y );
    return vec2_length( _x );
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if components in vectors are equal.
attr_always_inline
attr_header b32 ivec2_cmp( struct IVector2 a, struct IVector2 b ) {
    return
        a.x == b.x &&
        a.y == b.y;
}

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#if defined(CORE_CPLUSPLUS)
    #if !defined(CORE_CPP_MATH_VECTOR2_HPP)
        #include "core/cpp/math/vector2.hpp"
    #endif
    typedef Vector2CPP  vec2;
    typedef IVector2CPP ivec2;
#endif

#endif /* header guard */
