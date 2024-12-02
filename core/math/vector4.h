#if !defined(CORE_MATH_VECTOR4_H)
#define CORE_MATH_VECTOR4_H
/**
 * @file   vector4.h
 * @brief  Vector4 definition.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 28, 2024
*/
#include "core/defines.h"
#include "core/types.h"
#include "core/attributes.h"
#include "core/constants.h"

#include "core/math/trig.h"
#include "core/math/exponential.h"
#include "core/math/common.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

/// @brief 4 Component 32-bit Floating Point Vector.
struct Vector4 {
    /// @brief Union of vector component aliases.
    union {
        /// @brief X, Y, Z and W components.
        struct {
            /// @brief X, Y and Z components.
            union {
                /// @brief X, Y and Z components.
                struct {
                    /// @brief X and Y components.
                    union {
                        /// @brief X and Y components.
                        struct {
                            /// @brief X component.
                            f32 x;
                            /// @brief Y component.
                            f32 y;
                        };
                        /// @brief Red and Green channels.
                        struct {
                            /// @brief Red channel (X component).
                            f32 r;
                            /// @brief Green channel (Y component).
                            f32 g;
                        };
                        /// @brief X and Y components as a #Vector2.
                        struct Vector2 xy;
                        /// @brief Red and Green channels as a #Vector2.
                        struct Vector2 rg;
                    };
                    /// @brief Z component.
                    union {
                        /// @brief Z component.
                        f32 z;
                        /// @brief Blue channel (Z component).
                        f32 b;
                    };
                };
                /// @brief Swizzle yz.
                struct {
                    f32 __unused0;
                    /// @brief Y and Z components as a #Vector2.
                    struct Vector2 yz;
                };
                /// @brief Swizzle gb.
                struct {
                    f32 __unused1;
                    /// @brief Green and Blue channels as a #Vector2.
                    struct Vector2 gb;
                };
                /// @brief X, Y and Z components as a #Vector3.
                struct Vector3 xyz;
                /// @brief Red, Green and Blue components as a #Vector3.
                struct Vector3 rgb;
            };
            /// @brief W component.
            union {
                /// @brief W component.
                f32 w;
                /// @brief Alpha channel (W component).
                f32 a;
            };
        };
        /// @brief Swizzle yzw.
        struct {
            f32 __unused2;
            /// @brief Y, Z and W components as a #Vector3.
            struct Vector3 yzw;
        };
        /// @brief Swizzle gba.
        struct {
            f32 __unused3;
            /// @brief Green, Blue and Alpha channels as a #Vector3.
            struct Vector3 gba;
        };
        /// @brief Swizzle zw.
        struct {
            struct Vector2 __unused4;
            /// @brief Z and W components as a #Vector2.
            struct Vector2 zw;
        };
        /// @brief Swizzle ba.
        struct {
            struct Vector2 __unused5;
            /// @brief Blue and Alpha channels as a #Vector2.
            struct Vector2 ba;
        };
        /// @brief X, Y, Z and W components as an array.
        f32 array[4];
    };
};
/// @brief 4 Component 32-bit Signed Integer Vector.
struct IVector4 {
    /// @brief Union of vector component aliases.
    union {
        /// @brief X, Y, Z and W components.
        struct {
            /// @brief X, Y and Z components.
            union {
                /// @brief X, Y and Z components.
                struct {
                    /// @brief X and Y components.
                    union {
                        /// @brief X and Y components.
                        struct {
                            /// @brief X component.
                            i32 x;
                            /// @brief Y component.
                            i32 y;
                        };
                        /// @brief X and Y components as an #IVector2.
                        struct IVector2 xy;
                    };
                    /// @brief Z component.
                    union {
                        /// @brief Z component.
                        i32 z;
                    };
                };
                /// @brief Swizzle yz.
                struct {
                    i32 __unused0;
                    /// @brief Y and Z components as an #IVector2.
                    struct IVector2 yz;
                };
                /// @brief X, Y and Z components as an #IVector3.
                struct IVector3 xyz;
            };
            /// @brief W component.
            union {
                /// @brief W component.
                i32 w;
            };
        };
        /// @brief Swizzle yzw.
        struct {
            i32 __unused2;
            /// @brief Y, Z and W components as an #IVector3.
            struct IVector3 yzw;
        };
        /// @brief Swizzle zw.
        struct {
            struct IVector2 __unused4;
            /// @brief Z and W components as an #IVector2.
            struct IVector2 zw;
        };
        /// @brief X, Y, Z and W components as an array.
        i32 array[4];
    };
};
struct UVector4 {
    union {
        struct {
            u32 x;
            u32 y;
            u32 z;
            u32 w;
        };
        u32 array[4];
    };
};
struct DVector4 {
    union {
        struct {
            f64 x;
            f64 y;
            f64 z;
            f64 w;
        };
        f64 array[4];
    };
};
struct BVector4 {
    union {
        struct {
            b32 x;
            b32 y;
            b32 z;
            b32 w;
        };
        b32 array[4];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief 4 Component 32-bit Floating Point Vector.
    typedef struct Vector4 vec4;
    /// @brief 4 Component 32-bit Signed Integer Vector.
    typedef struct IVector4 ivec4;
#endif
typedef struct UVector4 uvec4;
typedef struct DVector4 dvec4;
typedef struct BVector4 bvec4;

#if defined(CORE_DOXYGEN) && !defined(CORE_CPLUSPLUS)
    /// @brief Construct a Vector4.
    /// @param x, y, z, w (f32) Components.
    /// @return Vector4.
    #define vec4( x, y, z, w )
    /// @brief Construct an IVector4.
    /// @param x, y, z, w (i32) Components.
    /// @return IVector4.
    #define ivec4( x, y, z, w )
#else /* Doxygen */

#if defined(CORE_CPLUSPLUS)
    #define vec4_new( x, y, z, w )\
        Vector4{ .array={ (x), (y), (z), (w) } }
    #define ivec4_new( x, y, z, w )\
        IVector4{ .array={ (x), (y), (z), (w) } }
#else /* C++ constructors */
    #define vec4( x, y, z, w )\
        (struct Vector4){ .array={ (x), (y), (z), (w) } }
    #define ivec4( x, y, z, w )\
        (struct IVector4){ .array={ (x), (y), (z), (w) } }
    #define vec4_new(...)  vec4(__VA_ARGS__)
    #define ivec4_new(...) ivec4(__VA_ARGS__)
#endif /* C constructors */ 

#endif /* Doxygen */

/// @brief Construct a new Vector4 with identical components.
/// @param x (f32) Value for components.
/// @return Vector4.
#define vec4_set( x )\
    vec4_new( (x), (x), (x), (x) )
/// @brief Construct a new RGBA (Vector4).
/// @param r, g, b, a (f32) Red, Green, Blue and Alpha channels.
/// @return RGBA (Vector4).
#define rgba( r, g, b, a ) vec4_new( r, g, b, a )
/// @brief Construct a new IVector4 with identical components.
/// @param x (i32) Value for components.
/// @return IVector4.
#define ivec4_set( x )\
    ivec4_new( (x), (x), (x), (x) )

/// @brief Vector4 zero constant.
#define VEC4_ZERO ( vec4_set( 0.0f ) )
/// @brief Vector4 one constant.
#define VEC4_ONE  ( vec4_set( 1.0f ) )

/// @brief IVector4 zero constant.
#define IVEC4_ZERO ( ivec4_set( 0 ) )
/// @brief IVector4 one constant.
#define IVEC4_ONE  ( ivec4_set( 1 ) )

/// @brief RGBA red constant.
#define RGBA_RED     ( rgba( 1.0f, 0.0f, 0.0f, 1.0f ) )
/// @brief RGBA green constant.
#define RGBA_GREEN   ( rgba( 0.0f, 1.0f, 0.0f, 1.0f ) )
/// @brief RGBA blue constant.
#define RGBA_BLUE    ( rgba( 0.0f, 0.0f, 1.0f, 1.0f ) )
/// @brief RGBA yellow constant.
#define RGBA_YELLOW  ( rgba( 1.0f, 1.0f, 0.0f, 1.0f ) )
/// @brief RGBA magenta constant.
#define RGBA_MAGENTA ( rgba( 1.0f, 0.0f, 1.0f, 1.0f ) )
/// @brief RGBA cyan constant.
#define RGBA_CYAN    ( rgba( 0.0f, 1.0f, 1.0f, 1.0f ) )
/// @brief RGBA black constant.
#define RGBA_BLACK   ( rgba( 0.0f, 0.0f, 0.0f, 1.0f ) )
/// @brief RGBA white constant.
#define RGBA_WHITE   ( vec4_set( 1.0f ) )
/// @brief RGBA clear constant.
#define RGBA_CLEAR   ( vec4_set( 0.0f ) )

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 4 values.
/// @return Vector with components from array.
attr_always_inline attr_header
struct Vector4 vec4_from_array( const f32 array[4] ) {
    return vec4_new( array[0], array[1], array[2], array[3] );
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 4 values.
attr_always_inline attr_header
void vec4_to_array( struct Vector4 v, f32* out_array ) {
    out_array[0] = v.array[0]; out_array[1] = v.array[1];
    out_array[2] = v.array[2]; out_array[3] = v.array[3];
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline attr_header
struct Vector4 vec4_add(
    struct Vector4 lhs, struct Vector4 rhs
) {
    return vec4_new(
        lhs.x + rhs.x, lhs.y + rhs.y,
        lhs.z + rhs.z, lhs.w + rhs.w );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline attr_header
struct Vector4 vec4_sub(
    struct Vector4 lhs, struct Vector4 rhs
) {
    return vec4_new(
        lhs.x - rhs.x, lhs.y - rhs.y,
        lhs.z - rhs.z, lhs.w - rhs.w );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline attr_header
struct Vector4 vec4_mul( struct Vector4 lhs, f32 rhs ) {
    return vec4_new(
        lhs.x * rhs, lhs.y * rhs,
        lhs.z * rhs, lhs.w * rhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline attr_header
struct Vector4 vec4_mul_vec4(
    struct Vector4 lhs, struct Vector4 rhs
) {
    return vec4_new(
        lhs.x * rhs.x, lhs.y * rhs.y,
        lhs.z * rhs.z, lhs.w * rhs.w );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline attr_header
struct Vector4 vec4_hadamard(
    struct Vector4 lhs, struct Vector4 rhs
) {
    return vec4_mul_vec4( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline attr_header
struct Vector4 vec4_div( struct Vector4 lhs, f32 rhs ) {
    return vec4_new(
        lhs.x / rhs, lhs.y / rhs,
        lhs.z / rhs, lhs.w / rhs );
}
/// @brief Divide vector components.
/// @param lhs,rhs Vectors to divide.
/// @return Result of division.
attr_always_inline attr_header
struct Vector4 vec4_div_vec4( struct Vector4 lhs, struct Vector4 rhs ) {
    return vec4_new(
        lhs.x / rhs.x, lhs.y / rhs.y,
        lhs.z / rhs.z, lhs.w / rhs.w );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline attr_header
struct Vector4 vec4_neg( struct Vector4 x ) {
    return vec4_new( -x.x, -x.y, -x.z, -x.w );
}
/// @brief Rotate vector components to the left.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the left.
attr_always_inline attr_header
struct Vector4 vec4_rotl( struct Vector4 x ) {
    return vec4_new( x.y, x.z, x.w, x.x );
}
/// @brief Rotate vector components to the right.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the right.
attr_always_inline attr_header
struct Vector4 vec4_rotr( struct Vector4 x ) {
    return vec4_new( x.w, x.x, x.y, x.z );
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline attr_header
f32 vec4_hadd( struct Vector4 x ) {
    return x.x + x.y + x.z + x.w;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline attr_header
f32 vec4_hmul( struct Vector4 x ) {
    return x.x * x.y * x.z * x.w;
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline attr_header
f32 vec4_dot( struct Vector4 lhs, struct Vector4 rhs ) {
    return vec4_hadd( vec4_hadamard( lhs, rhs ) );
}
/// @brief Get the maximum component in vector.
/// @param x Vector to get maximum of.
/// @return Component with largest value.
attr_always_inline attr_header
f32 vec4_hmax( struct Vector4 x ) {
    f32 _0 = vec2_hmax( x.xy );
    f32 _1 = vec2_hmax( vec2_new( x.y, x.z ) );
    return _0 < _1 ? _1 : _0;
}
/// @brief Component-wise maximum value.
/// @param x, y Vectors.
/// @return Vector with maximum value in components.
attr_always_inline attr_header
struct Vector4 vec4_max( struct Vector4 x, struct Vector4 y ) {
    struct Vector4 result;
    result.x = x.x < y.x ? y.x : x.x;
    result.y = x.y < y.y ? y.y : x.y;
    result.z = x.z < y.z ? y.z : x.z;
    result.w = x.w < y.w ? y.w : x.w;
    return result;
}
/// @brief Get the minimum component in vector.
/// @param x Vector to get minimum of.
/// @return Component with smallest value.
attr_always_inline attr_header
f32 vec4_hmin( struct Vector4 x ) {
    f32 _0 = vec2_hmin( x.xy );
    f32 _1 = vec2_hmin( vec2_new( x.y, x.z ) );
    return _0 < _1 ? _0 : _1;
}
/// @brief Component-wise minimum value.
/// @param x, y Vectors.
/// @return Vector with minimum value in components.
attr_always_inline attr_header
struct Vector4 vec4_min( struct Vector4 x, struct Vector4 y ) {
    struct Vector4 result;
    result.x = x.x < y.x ? x.x : y.x;
    result.y = x.y < y.y ? x.y : y.y;
    result.z = x.z < y.z ? x.z : y.z;
    result.w = x.w < y.w ? x.w : y.w;
    return result;
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline attr_header
f32 vec4_length_sqr( struct Vector4 x ) {
    return vec4_dot( x, x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_always_inline attr_header
f32 vec4_length( struct Vector4 x ) {
    return f32_sqrt( vec4_length_sqr( x ) );
}
/// @brief Calculate distance between two points.
/// @param a, b Points to calculate distance of.
/// @return Distance squared.
attr_always_inline attr_header
f32 vec4_distance_sqr( struct Vector4 a, struct Vector4 b ) {
    return vec4_length_sqr( vec4_sub( a, b ) );
}
/// @brief Calculate distance between two points.
/// @param a, b Points to calculate distance of.
/// @return Distance.
attr_always_inline attr_header
f32 vec4_distance( struct Vector4 a, struct Vector4 b ) {
    return vec4_length( vec4_sub( a, b ) );
}
/// @brief Normalize a Vector.
/// @param x Vector to normalize.
/// @return Normalized vector or zero vector if magnitude == 0.
attr_always_inline attr_header
struct Vector4 vec4_normalize( struct Vector4 x ) {
    f32 m = vec4_length_sqr( x );
    if( m == 0.0f ) {
        return VEC4_ZERO;
    } else {
        return vec4_div( x, f32_sqrt( m ) );
    }
}
/// @brief Component-wise clamp.
/// @param v        Vector.
/// @param min, max Range.
/// @return Clamped vector.
attr_always_inline attr_header
struct Vector4 vec4_clamp(
    struct Vector4 v, struct Vector4 min, struct Vector4 max
) {
    struct Vector4 result;
    result.x = ( (v.x) < (min.x) ? (min.x) : ( (v.x) > (max.x) ? (max.x) : (v.x) ) );
    result.y = ( (v.y) < (min.y) ? (min.y) : ( (v.y) > (max.y) ? (max.y) : (v.y) ) );
    result.z = ( (v.z) < (min.z) ? (min.z) : ( (v.z) > (max.z) ? (max.z) : (v.z) ) );
    result.w = ( (v.w) < (min.w) ? (min.w) : ( (v.w) > (max.w) ? (max.w) : (v.w) ) );
    return result;
}
/// @brief Clamp vector's magnitude to range min -> max.
/// @param v Vector to clamp.
/// @param min_, max_ Range to clamp to, min_ must be < max_.
/// @return Clamped vector.
attr_always_inline attr_header
struct Vector4 vec4_clamp_length(
    struct Vector4 v, f32 min_, f32 max_
) {
    f32 mag = vec4_length( v );
    if( mag == 0.0f ) {
        return VEC4_ZERO;
    }
    f32 new_length = ( mag < min_ ? min_ : ( mag > max_ ? max_ : mag ) );
    return vec4_mul( vec4_div( v, mag ), new_length );
}
/// @brief Component-wise abs.
/// @param v Vector.
/// @return Vector with absolute values.
attr_always_inline attr_header
struct Vector4 vec4_abs( struct Vector4 v ) {
    struct Vector4 result;
    result.x = v.x < 0.0f ? -v.x : v.x;
    result.y = v.y < 0.0f ? -v.y : v.y;
    result.z = v.z < 0.0f ? -v.z : v.z;
    result.w = v.w < 0.0f ? -v.w : v.w;
    return result;
}
/// @brief Component-wise sign.
/// @param v Vector.
/// @return Vector with sign values.
attr_always_inline attr_header
struct Vector4 vec4_sign( struct Vector4 v ) {
    struct Vector4 result;
    result.x = (v.x > 0.0f) - (f32)(v.x < 0.0f);
    result.y = (v.y > 0.0f) - (f32)(v.y < 0.0f);
    result.z = (v.z > 0.0f) - (f32)(v.z < 0.0f);
    result.w = (v.w > 0.0f) - (f32)(v.w < 0.0f);
    return result;
}
/// @brief Component-wise truncate.
/// @param v Vector to truncate.
/// @return Vector with truncated values.
attr_always_inline attr_header
struct Vector4 vec4_trunc( struct Vector4 v ) {
    struct Vector4 result;
    result.x = (f32)(i32)v.x;
    result.y = (f32)(i32)v.y;
    result.z = (f32)(i32)v.z;
    result.w = (f32)(i32)v.w;
    return result;
}
/// @brief Component-wise floor.
/// @param v Vector to floor.
/// @return Vector with floored values.
attr_always_inline attr_header
struct Vector4 vec4_floor( struct Vector4 v ) {
    struct Vector4 result;
    result.x = (f32)( v.x < 0.0f ? (i32)( v.x - 1.0f ) : (i32)( v.x ) );
    result.y = (f32)( v.y < 0.0f ? (i32)( v.y - 1.0f ) : (i32)( v.y ) );
    result.z = (f32)( v.z < 0.0f ? (i32)( v.z - 1.0f ) : (i32)( v.z ) );
    result.w = (f32)( v.w < 0.0f ? (i32)( v.w - 1.0f ) : (i32)( v.w ) );
    return result;
}
/// @brief Component-wise ceil.
/// @param v Vector to ceil.
/// @return Vector with ceiled values.
attr_always_inline attr_header
struct Vector4 vec4_ceil( struct Vector4 v ) {
    struct Vector4 result;
    result.x = (f32)( v.x < 0.0f ? (i32)( v.x ) : (i32)( v.x + 1.0f ) );
    result.y = (f32)( v.y < 0.0f ? (i32)( v.y ) : (i32)( v.y + 1.0f ) );
    result.z = (f32)( v.z < 0.0f ? (i32)( v.z ) : (i32)( v.z + 1.0f ) );
    result.w = (f32)( v.w < 0.0f ? (i32)( v.w ) : (i32)( v.w + 1.0f ) );
    return result;
}
/// @brief Component-wise round.
/// @param v Vector to round.
/// @return Vector with rounded values.
attr_always_inline attr_header
struct Vector4 vec4_round( struct Vector4 v ) {
    struct Vector4 result;
    result.x = (f32)( v.x < 0.0f ? (i32)( v.x - 0.5f ) : (i32)( v.x + 0.5f ) );
    result.y = (f32)( v.y < 0.0f ? (i32)( v.y - 0.5f ) : (i32)( v.y + 0.5f ) );
    result.z = (f32)( v.z < 0.0f ? (i32)( v.z - 0.5f ) : (i32)( v.z + 0.5f ) );
    result.w = (f32)( v.w < 0.0f ? (i32)( v.w - 0.5f ) : (i32)( v.w + 0.5f ) );
    return result;
}
/// @brief Get fractional part.
/// @param v Vector.
/// @return Vector with fractional part.
attr_always_inline attr_header
struct Vector4 vec4_fract( struct Vector4 v ) {
    return vec4_sub( v, vec4_round(v) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_always_inline attr_header
struct Vector4 vec4_lerp(
    struct Vector4 a, struct Vector4 b, f32 t 
) {
    return vec4_add( vec4_mul( a, 1.0f - t ), vec4_mul( b, t ) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_always_inline attr_header
struct Vector4 vec4_mix(
    struct Vector4 a, struct Vector4 b, f32 t 
) {
    return vec4_lerp( a, b, t );
}
/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline attr_header
struct Vector4 vec4_step( struct Vector4 edge, struct Vector4 x ) {
    return vec4_new(
        f32_step( edge.x, x.x ),
        f32_step( edge.y, x.y ),
        f32_step( edge.z, x.z ),
        f32_step( edge.w, x.w ) );
}
/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline attr_header
struct Vector4 vec4_step_scalar( f32 edge, struct Vector4 x ) {
    return vec4_step( vec4_set( edge ), x );
}
/// @brief Smooth step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header
struct Vector4 vec4_smoothstep(
    struct Vector4 edge0, struct Vector4 edge1, struct Vector4 x
) {
    return vec4_new(
        f32_smoothstep( edge0.x, edge1.x, x.x ),
        f32_smoothstep( edge0.y, edge1.y, x.y ),
        f32_smoothstep( edge0.z, edge1.z, x.z ),
        f32_smoothstep( edge0.w, edge1.w, x.w ) );
}
/// @brief Smooth step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header
struct Vector4 vec4_smoothstep_scalar( f32 edge0, f32 edge1, struct Vector4 x ) {
    return vec4_smoothstep( vec4_set( edge0 ), vec4_set( edge1 ), x );
}
/// @brief Smoother step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Vector in range a -> b.
attr_always_inline attr_header
struct Vector4 vec4_smootherstep(
    struct Vector4 edge0, struct Vector4 edge1, struct Vector4 x
) {
    return vec4_new(
        f32_smootherstep( edge0.x, edge1.x, x.x ),
        f32_smootherstep( edge0.y, edge1.y, x.y ),
        f32_smootherstep( edge0.z, edge1.z, x.z ),
        f32_smootherstep( edge0.w, edge1.w, x.w ) );
}
/// @brief Smoother step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Vector in range a -> b.
attr_always_inline attr_header
struct Vector4 vec4_smootherstep_scalar(
    f32 edge0, f32 edge1, struct Vector4 x
) {
    return vec4_smootherstep( vec4_set( edge0 ), vec4_set( edge1 ), x );
}
/// @brief Convert degrees to radians.
/// @param degrees Angles in degrees.
/// @return Angles in radians.
attr_always_inline attr_header
struct Vector4 vec4_radians( struct Vector4 degrees ) {
    return vec4_new( f32_radians(degrees.x), f32_radians(degrees.y), f32_radians(degrees.z), f32_radians(degrees.w) );
}
/// @brief Convert radians to degrees.
/// @param radians Angles in radians.
/// @return Angles in degrees.
attr_always_inline attr_header
struct Vector4 vec4_degrees( struct Vector4 radians ) {
    return vec4_new( f32_degrees(radians.x), f32_degrees(radians.y ), f32_degrees(radians.z), f32_degrees(radians.w) );
}
/// @brief Calculate sine of x.
/// @param angle Value to get sine of.
/// @return Sine of angle.
attr_always_inline attr_header
struct Vector4 vec4_sin( struct Vector4 angle ) {
    return vec4_new( f32_sin(angle.x), f32_sin(angle.y), f32_sin(angle.z), f32_sin(angle.w) );
}
/// @brief Calculate cosine of x.
/// @param angle Value to get cosine of.
/// @return Cosine of angle.
attr_always_inline attr_header
struct Vector4 vec4_cos( struct Vector4 angle ) {
    return vec4_new( f32_cos(angle.x), f32_cos(angle.y), f32_cos(angle.z), f32_cos(angle.w) );
}
/// @brief Calculate tangent of x.
/// @param angle Value to get tangent of.
/// @return Tangent of angle.
/// @warning Returns NaN if cosine of angle is zero.
attr_always_inline attr_header
struct Vector4 vec4_tan( struct Vector4 angle ) {
    return vec4_new( f32_tan(angle.x), f32_tan(angle.y), f32_tan(angle.z), f32_tan(angle.w) );
}
/// @brief Calculate arcsine of x.
///
/// Does not produce NaN when outside valid range.
/// @param x Value to get arcsine of.
/// @return Arcsine of x clamped to -Pi -> Pi.
/// @see #F32_PI
/// @see #asin()
attr_always_inline attr_header
struct Vector4 vec4_asin( struct Vector4 angle ) {
    return vec4_new( f32_asin_real(angle.x), f32_asin_real(angle.y), f32_asin_real(angle.z), f32_asin_real(angle.w) );
}
/// @brief Calculate arccosine of x.
/// @param x Value to get arccosine of.
/// @return Arccosine of x.
attr_always_inline attr_header
struct Vector4 vec4_acos( struct Vector4 angle ) {
    return vec4_new( f32_acos(angle.x), f32_acos(angle.y), f32_acos(angle.z), f32_acos(angle.w) );
}
/// @brief Calculate arctangent of x.
/// @param x Value to get arctangent of.
/// @return Arctangent of x.
attr_always_inline attr_header
struct Vector4 vec4_atan( struct Vector4 angle ) {
    return vec4_new( f32_atan(angle.x), f32_atan(angle.y), f32_atan(angle.z), f32_atan(angle.w) );
}
/// @brief Calculate 2 component arctangent of y and x.
/// @param y Value to get arctangent of.
/// @param x Value to get arctangent of.
/// @return Arctangent of y and x.
attr_always_inline attr_header
struct Vector4 vec4_atan2( struct Vector4 y, struct Vector4 x ) {
    return vec4_new( f32_atan2(y.x, x.x), f32_atan2(y.y, x.y), f32_atan2(y.z, x.z), f32_atan2(y.w, x.w) );
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if the square magnitude of a - b is < F32_EPSILON.
/// @see #F32_EPSILON
attr_always_inline attr_header
b32 vec4_cmp( struct Vector4 a, struct Vector4 b ) {
    return vec4_length_sqr( vec4_sub( a, b ) ) < F32_EPSILON;
}

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 4 values.
/// @return Vector with components from array.
attr_always_inline attr_header
struct IVector4 ivec4_from_array( const i32 array[4] ) {
    return ivec4_new( array[0], array[1], array[2], array[4] );
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 4 values.
attr_always_inline attr_header
void ivec4_to_array( struct IVector4 v, i32* out_array ) {
    out_array[0] = v.array[0]; out_array[1] = v.array[1];
    out_array[2] = v.array[2]; out_array[3] = v.array[3];
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline attr_header
struct IVector4 ivec4_add(
    struct IVector4 lhs, struct IVector4 rhs
) {
    return ivec4_new(
        lhs.x + rhs.x, lhs.y + rhs.y,
        lhs.z + rhs.z, lhs.w + rhs.w );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline attr_header
struct IVector4 ivec4_sub(
    struct IVector4 lhs, struct IVector4 rhs
) {
    return ivec4_new(
        lhs.x - rhs.x, lhs.y - rhs.y,
        lhs.z - rhs.z, lhs.w - rhs.w );
}
/// @brief Component-wise multiply vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline attr_header
struct IVector4 ivec4_mul_ivec4(
    struct IVector4 lhs, struct IVector4 rhs
) {
    return ivec4_new(
        lhs.x * rhs.x, lhs.y * rhs.y,
        lhs.z * rhs.z, lhs.w * rhs.w );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline attr_header
struct IVector4 ivec4_mul( struct IVector4 lhs, i32 rhs ) {
    return ivec4_new(
        lhs.x * rhs, lhs.y * rhs,
        lhs.z * rhs, lhs.w * rhs );
}
/// @brief Component-wise divide vectors.
/// @param lhs, rhs Vectors to divide.
/// @return Result of division.
attr_always_inline attr_header
struct IVector4 ivec4_div_ivec4(
    struct IVector4 lhs, struct IVector4 rhs
) {
    return ivec4_new(
        lhs.x / rhs.x, lhs.y / rhs.y,
        lhs.z / rhs.z, lhs.w / rhs.w );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline attr_header
struct IVector4 ivec4_div( struct IVector4 lhs, i32 rhs ) {
    return ivec4_new(
        lhs.x / rhs, lhs.y / rhs,
        lhs.z / rhs, lhs.w / rhs );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline attr_header
struct IVector4 ivec4_neg( struct IVector4 x ) {
    return ivec4_new( -x.x, -x.y, -x.z, -x.w );
}
/// @brief Rotate vector components to the left.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the left.
attr_always_inline attr_header
struct IVector4 ivec4_rotl( struct IVector4 x ) {
    return ivec4_new( x.y, x.z, x.w, x.x );
}
/// @brief Rotate vector components to the right.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the right.
attr_always_inline attr_header
struct IVector4 ivec4_rotr( struct IVector4 x ) {
    return ivec4_new( x.w, x.x, x.y, x.z );
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline attr_header
i32 ivec4_hadd( struct IVector4 x ) {
    return x.x + x.y + x.z + x.w;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline attr_header
i32 ivec4_hmul( struct IVector4 x ) {
    return x.x * x.y * x.z * x.w;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline attr_header
struct IVector4 ivec4_hadamard(
    struct IVector4 lhs, struct IVector4 rhs
) {
    return ivec4_new(
        lhs.x * rhs.x, lhs.y * rhs.y,
        lhs.z * rhs.z, lhs.w * rhs.w );
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline attr_header
f32 ivec4_dot( struct IVector4 lhs, struct IVector4 rhs ) {
    struct Vector4 _lhs = vec4_new( (f32)lhs.x, (f32)lhs.y, (f32)lhs.z, (f32)lhs.w );
    struct Vector4 _rhs = vec4_new( (f32)rhs.x, (f32)rhs.y, (f32)rhs.z, (f32)rhs.w );

    return vec4_dot( _lhs, _rhs );
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline attr_header
f32 ivec4_length_sqr( struct IVector4 x ) {
    struct Vector4 _x = vec4_new( (f32)x.x, (f32)x.y, (f32)x.z, (f32)x.w );
    return vec4_length_sqr( _x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_always_inline attr_header
f32 ivec4_length( struct IVector4 x ) {
    struct Vector4 _x = vec4_new( (f32)x.x, (f32)x.y, (f32)x.z, (f32)x.w );
    return vec4_length( _x );
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if vector components are equal.
attr_always_inline attr_header
b32 ivec4_cmp( struct IVector4 a, struct IVector4 b ) {
    return
        a.x == b.x &&
        a.y == b.y &&
        a.z == b.z &&
        a.w == b.w;
}

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#if defined(CORE_CPLUSPLUS)
    #if !defined(CORE_CPP_MATH_VECTOR4_HPP)
        #include "core/cpp/math/vector4.hpp"
    #endif
    typedef Vector4CPP  vec4;
    typedef IVector4CPP ivec4;
#endif

typedef vec4 rgba;

#endif /* header guard */
