#if !defined(CORE_MATH_VECTOR3_H)
#define CORE_MATH_VECTOR3_H
/**
 * @file   vector3.h
 * @brief  Vector3 definition.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 28, 2024
*/
#include "core/defines.h"
#include "core/types.h"
#include "core/attributes.h"

#include "core/math/trig.h"
#include "core/math/exponential.h"
#include "core/math/common.h"
#include "core/math/vector2.h"

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

/// @brief 3 Component 32-bit Floating Point Vector.
struct Vector3 {
    /// @brief Union of vector component aliases.
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
                /// @brief Hue and Saturation.
                struct {
                    /// @brief Hue (X component).
                    f32 h;
                    /// @brief Saturation (X component).
                    f32 s;
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
                /// @brief Lightness (Z component).
                f32 l;
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
        /// @brief X, Y and Z as an array.
        f32 array[3];
    };
};
/// @brief 3 Component 32-bit Signed Integer Vector.
struct IVector3 {
    /// @brief Union of vector component aliases.
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
            i32 z;
        };
        /// @brief Swizzle yz.
        struct {
            i32 __unused0;
            /// @brief Y and Z components as an #IVector2.
            struct IVector2 yz;
        };
        /// @brief X, Y and Z components as an array.
        i32 array[3];
    };
};
struct UVector3 {
    union {
        struct {
            u32 x;
            u32 y;
            u32 z;
        };
        u32 array[3];
    };
};
struct DVector3 {
    union {
        struct {
            f64 x;
            f64 y;
            f64 z;
        };
        f64 array[3];
    };
};
struct BVector3 {
    union {
        struct {
            b32 x;
            b32 y;
            b32 z;
        };
        b32 array[3];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief 3 Component 32-bit Signed Integer Vector.
    typedef struct IVector3 ivec3;
    /// @brief 3 Component 32-bit Floating Point Vector.
    typedef struct Vector3 vec3;
#endif
typedef struct UVector3 uvec3;
typedef struct DVector3 dvec3;
typedef struct BVector3 bvec3;

#if defined(CORE_DOXYGEN) && !defined(CORE_CPLUSPLUS)
    /// @brief Construct a Vector3.
    /// @param x, y, z (f32) Components.
    /// @return Vector3.
    #define vec3_new( x, y, z )
    /// @brief Construct an IVector3.
    /// @param x, y, z (i32) Components.
    /// @return IVector3.
    #define ivec3_new( x, y, z )
#else

#if defined(CORE_CPLUSPLUS)
    #define vec3_new( x, y, z )\
        Vector3{ .array={ (x), (y), (z) } }
    #define ivec3_new( x, y, z )\
        IVector3{ .array={ (x), (y), (z) } }
#else /* C++ constructors */
    #define vec3( x, y, z )\
        (struct Vector3){ .array={ (x), (y), (z) } }
    #define ivec3( x, y, z )\
        (struct IVector3){ .array={ (x), (y), (z) } }
    #define vec3_new(...)  vec3(__VA_ARGS__)
    #define ivec3_new(...) ivec3(__VA_ARGS__)
#endif /* C constructors */

#endif /* Doxygen */

/// @brief Construct a new Vector3 with identical components.
/// @param x (f32) Value for components.
/// @return Vector3.
#define vec3_set( x )\
    vec3_new( (x), (x), (x) )
/// @brief Construct a new RGB (Vector3).
/// @param r, g, b (f32) Red, Green and Blue channels.
/// @return RGB (Vector3).
#define rgb( r, g, b ) vec3_new( r, g, b )
/// @brief Construct a new HSL (Vector3).
/// @param h, s, l (f32) Hue, Saturation and Lightness.
/// @return HSL (Vector3).
#define hsl( h, s, l ) vec3_new( wrap_deg( h ), clamp01( s ), clamp01( l ) )
/// @brief Construct a new IVector3 with identical components.
/// @param x (i32) Value for components.
/// @return IVector3.
#define ivec3_set( x )\
    ivec3_new( (x), (x), (x) )

/// @brief Vector3 zero constant.
#define VEC3_ZERO    ( vec3_set( 0.0f ) )
/// @brief Vector3 one constant.
#define VEC3_ONE     ( vec3_set( 1.0f ) )
/// @brief Vector3 left constant.
#define VEC3_LEFT    ( vec3_new( -1.0f,  0.0f,  0.0f ) )
/// @brief Vector3 right constant.
#define VEC3_RIGHT   ( vec3_new(  1.0f,  0.0f,  0.0f ) )
/// @brief Vector3 up constant.
#define VEC3_UP      ( vec3_new(  0.0f,  1.0f,  0.0f ) )
/// @brief Vector3 down constant.
#define VEC3_DOWN    ( vec3_new(  0.0f, -1.0f,  0.0f ) )
/// @brief Vector3 forward constant.
#define VEC3_FORWARD ( vec3_new(  0.0f,  0.0f,  1.0f ) )
/// @brief Vector3 back constant.
#define VEC3_BACK    ( vec3_new(  0.0f,  0.0f, -1.0f ) )

/// @brief IVector3 zero constant.
#define IVEC3_ZERO    ( ivec3_set( 0 ) )
/// @brief IVector3 one constant.
#define IVEC3_ONE     ( ivec3_set( 1 ) )
/// @brief IVector3 left constant.
#define IVEC3_LEFT    ( ivec3_new( -1,  0,  0 ) )
/// @brief IVector3 right constant.
#define IVEC3_RIGHT   ( ivec3_new(  1,  0,  0 ) )
/// @brief IVector3 up constant.
#define IVEC3_UP      ( ivec3_new(  0,  1,  0 ) )
/// @brief IVector3 down constant.
#define IVEC3_DOWN    ( ivec3_new(  0, -1,  0 ) )
/// @brief IVector3 forward constant.
#define IVEC3_FORWARD ( ivec3_new(  0,  0,  1 ) )
/// @brief IVector3 zero constant.
#define IVEC3_BACK    ( ivec3_new(  0,  0, -1 ) )

/// @brief RGB red constant.
#define RGB_RED     ( rgb( 1.0f, 0.0f, 0.0f ) )
/// @brief RGB green constant.
#define RGB_GREEN   ( rgb( 0.0f, 1.0f, 0.0f ) )
/// @brief RGB blue constant.
#define RGB_BLUE    ( rgb( 0.0f, 0.0f, 1.0f ) )
/// @brief RGB yellow constant.
#define RGB_YELLOW  ( rgb( 1.0f, 1.0f, 0.0f ) )
/// @brief RGB magenta constant.
#define RGB_MAGENTA ( rgb( 1.0f, 0.0f, 1.0f ) )
/// @brief RGB cyan constant.
#define RGB_CYAN    ( rgb( 0.0f, 1.0f, 1.0f ) )
/// @brief RGB black constant.
#define RGB_BLACK   ( vec3_set( 0.0f ) )
/// @brief RGB white constant.
#define RGB_WHITE   ( vec3_set( 1.0f ) )

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 3 values.
/// @return Vector with components from array.
attr_always_inline attr_header
struct Vector3 vec3_from_array( const f32 array[3] ) {
    return vec3_new( array[0], array[1], array[2] );
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 3 values.
attr_always_inline attr_header
void vec3_to_array( struct Vector3 v, f32* out_array ) {
    out_array[0] = v.array[0]; out_array[1] = v.array[1]; out_array[2] = v.array[2];
}
/// Convert RGB color to HSL color.
/// @param _rgb RGB color to convert.
/// @return HSL color.
attr_core_api struct Vector3 rgb_to_hsl( struct Vector3 _rgb );
/// Convert HSL color to RGB color.
/// @param _hsl HSL color to convert.
/// @return RGB color.
attr_core_api struct Vector3 hsl_to_rgb( struct Vector3 _hsl );
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline attr_header
struct Vector3 vec3_add(
    struct Vector3 lhs, struct Vector3 rhs
) {
    return vec3_new( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline attr_header
struct Vector3 vec3_sub(
    struct Vector3 lhs, struct Vector3 rhs
) {
    return vec3_new( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline attr_header
f32 vec3_hadd( struct Vector3 x ) {
    return x.x + x.y + x.z;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline attr_header
f32 vec3_hmul( struct Vector3 x ) {
    return x.x * x.y * x.z;
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline attr_header
struct Vector3 vec3_mul( struct Vector3 lhs, f32 rhs ) {
    return vec3_new( lhs.x * rhs, lhs.y * rhs, lhs.z * rhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline attr_header
struct Vector3 vec3_mul_vec3(
    struct Vector3 lhs, struct Vector3 rhs
) {
    return vec3_new( lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline attr_header
struct Vector3 vec3_hadamard(
    struct Vector3 lhs, struct Vector3 rhs
) {
    return vec3_mul_vec3( lhs, rhs );
}
/// @brief Cross product between two vectors.
/// @param lhs, rhs Vectors to cross.
/// @return Result of cross product.
attr_always_inline attr_header
struct Vector3 vec3_cross(
    struct Vector3 lhs, struct Vector3 rhs
) {
    return vec3_new(
        ( lhs.y * rhs.z ) - ( lhs.z * rhs.y ),
        ( lhs.z * rhs.x ) - ( lhs.x * rhs.z ),
        ( lhs.x * rhs.y ) - ( lhs.y * rhs.x ) );
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline attr_header
f32 vec3_dot( struct Vector3 lhs, struct Vector3 rhs ) {
    return vec3_hadd( vec3_hadamard( lhs, rhs ) );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline attr_header
struct Vector3 vec3_div( struct Vector3 lhs, f32 rhs ) {
    return vec3_new( lhs.x / rhs, lhs.y / rhs, lhs.z / rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline attr_header
struct Vector3 vec3_div_vec3( struct Vector3 lhs, struct Vector3 rhs ) {
    return vec3_new( lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of modulus division.
attr_always_inline attr_header
struct Vector3 vec3_mod( struct Vector3 lhs, f32 rhs ) {
    return vec3_new(
        f32_mod( lhs.x, rhs ),
        f32_mod( lhs.y, rhs ),
        f32_mod( lhs.z, rhs ) );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of modulus division.
attr_always_inline attr_header
struct Vector3 vec3_mod_vec3( struct Vector3 lhs, struct Vector3 rhs ) {
    return vec3_new(
        f32_mod( lhs.x, rhs.x ),
        f32_mod( lhs.y, rhs.y ),
        f32_mod( lhs.z, rhs.z ) );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline attr_header
struct Vector3 vec3_neg( struct Vector3 x ) {
    return vec3_new( -x.x, -x.y, -x.z );
}
/// @brief Rotate vector components to the left.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the left.
attr_always_inline attr_header
struct Vector3 vec3_rotl( struct Vector3 x ) {
    return vec3_new( x.y, x.z, x.x );
}
/// @brief Rotate vector components to the right.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the right.
attr_always_inline attr_header
struct Vector3 vec3_rotr( struct Vector3 x ) {
    return vec3_new( x.z, x.x, x.y );
}
/// @brief Get the maximum component in vector.
/// @param x Vector to get maximum of.
/// @return Component with largest value.
attr_always_inline attr_header
f32 vec3_hmax( struct Vector3 x ) {
    f32 _0 = x.x < x.y ? x.y : x.x;
    return _0 < x.z ? x.z : _0;
}
/// @brief Component-wise maximum value.
/// @param x, y Vectors.
/// @return Vector with maximum value in components.
attr_always_inline attr_header
struct Vector3 vec3_max( struct Vector3 x, struct Vector3 y ) {
    struct Vector3 result;
    result.x = x.x < y.x ? y.x : x.x;
    result.y = x.y < y.y ? y.y : x.y;
    result.z = x.z < y.z ? y.z : x.z;
    return result;
}
/// @brief Get the component in between min and max component.
/// @param x Vector to get mid of.
/// @return Component in between min and max component.
attr_always_inline attr_header
f32 vec3_hmid( struct Vector3 x ) {
    if( x.x < x.y ) {
        if( x.y < x.z ) {
            return x.z;
        } else {
            return x.y;
        }
    } else {
        if( x.x < x.z ) {
            return x.z;
        } else {
            return x.x;
        }
    }
}
/// @brief Get the minimum component in vector.
/// @param x Vector to get minimum of.
/// @return Component with smallest value.
attr_always_inline attr_header
f32 vec3_hmin( struct Vector3 x ) {
    f32 _0 = x.x < x.y ? x.x : x.y;
    return _0 < x.z ? _0 : x.z;
}
/// @brief Component-wise minimum value.
/// @param x, y Vectors.
/// @return Vector with minimum value in components.
attr_always_inline attr_header
struct Vector3 vec3_min( struct Vector3 x, struct Vector3 y ) {
    struct Vector3 result;
    result.x = x.x < y.x ? x.x : y.x;
    result.y = x.y < y.y ? x.y : y.y;
    result.z = x.z < y.z ? x.z : y.z;
    return result;
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline attr_header
f32 vec3_length_sqr( struct Vector3 x ) {
    return vec3_dot( x, x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_always_inline attr_header
f32 vec3_length( struct Vector3 x ) {
    return f32_sqrt( vec3_length_sqr( x ) );
}
/// @brief Component-wise minimum value.
/// @param x, y Vectors.
/// @return Vector with minimum value in components.
attr_always_inline attr_header
struct IVector3 ivec3_min( struct IVector3 x, struct IVector3 y ) {
    struct IVector3 result;
    result.x = x.x < y.x ? x.x : y.x;
    result.y = x.y < y.y ? x.y : y.y;
    result.z = x.z < y.z ? x.z : y.z;
    return result;
}
/// @brief Component-wise maximum value.
/// @param x, y Vectors.
/// @return Vector with maximum value in components.
attr_always_inline attr_header
struct IVector3 ivec3_max( struct IVector3 x, struct IVector3 y ) {
    struct IVector3 result;
    result.x = x.x < y.x ? y.x : x.x;
    result.y = x.y < y.y ? y.y : x.y;
    result.z = x.z < y.z ? y.z : x.z;
    return result;
}
/// @brief Calculate distance between two points.
/// @param a, b Points to calculate distance of.
/// @return Distance squared.
attr_always_inline attr_header
f32 vec3_distance_sqr( struct Vector3 a, struct Vector3 b ) {
    return vec3_length_sqr( vec3_sub( a, b ) );
}
/// @brief Calculate distance between two points.
/// @param a, b Points to calculate distance of.
/// @return Distance.
attr_always_inline attr_header
f32 vec3_distance( struct Vector3 a, struct Vector3 b ) {
    return vec3_length( vec3_sub( a, b ) );
}
/// @brief Normalize a Vector.
/// @param x Vector to normalize.
/// @return Normalized vector or zero vector if magnitude == 0.
attr_always_inline attr_header
struct Vector3 vec3_normalize( struct Vector3 x ) {
    f32 m = vec3_length_sqr( x );
    if( m == 0.0f ) {
        return VEC3_ZERO;
    } else {
        return vec3_div( x, f32_sqrt( m ) );
    }
}
/// @brief Reflect direction vector off surface.
/// @param direction Direction vector to reflect.
/// @param normal    Normal of the surface to reflect off of.
/// @return Reflected vector.
attr_always_inline attr_header
struct Vector3 vec3_reflect(
    struct Vector3 direction, struct Vector3 normal
) {
    return vec3_mul(
        vec3_sub( normal, direction ),
        2.0f * vec3_dot( direction, normal ) );
}
/// @brief Component-wise clamp.
/// @param v        Vector.
/// @param min, max Range.
/// @return Clamped vector.
attr_always_inline attr_header
struct Vector3 vec3_clamp(
    struct Vector3 v, struct Vector3 min, struct Vector3 max
) {
    struct Vector3 result;
    result.x = ( (v.x) < (min.x) ? (min.x) : ( (v.x) > (max.x) ? (max.x) : (v.x) ) );
    result.y = ( (v.y) < (min.y) ? (min.y) : ( (v.y) > (max.y) ? (max.y) : (v.y) ) );
    result.z = ( (v.z) < (min.z) ? (min.z) : ( (v.z) > (max.z) ? (max.z) : (v.z) ) );
    return result;
}
/// @brief Clamp vector's magnitude to range min -> max.
/// @param v Vector to clamp.
/// @param min_, max_ Range to clamp to, min_ must be < max_.
/// @return Clamped vector.
attr_always_inline attr_header
struct Vector3 vec3_clamp_length(
    struct Vector3 v, f32 min_, f32 max_
) {
    f32 mag = vec3_length( v );
    if( mag == 0.0f ) {
        return VEC3_ZERO;
    }
    f32 new_length = ( mag < min_ ? min_ : ( mag > max_ ? max_ : mag ) );
    return vec3_mul( vec3_div( v, mag ), new_length );
}
/// @brief Calculate the angle between two vectors.
/// @param a, b Vectors to calculate angle of.
/// @return Angle (in radians) between vectors.
attr_always_inline attr_header
f32 vec3_angle( struct Vector3 a, struct Vector3 b ) {
    return f32_acos( vec3_dot( a, b ) );
}
/// @brief Component-wise abs.
/// @param v Vector.
/// @return Vector with absolute values.
attr_always_inline attr_header
struct Vector3 vec3_abs( struct Vector3 v ) {
    struct Vector3 result;
    result.x = v.x < 0.0f ? -v.x : v.x;
    result.y = v.y < 0.0f ? -v.y : v.y;
    result.z = v.z < 0.0f ? -v.z : v.z;
    return result;
}
/// @brief Component-wise sign.
/// @param v Vector.
/// @return Vector with sign values.
attr_always_inline attr_header
struct Vector3 vec3_sign( struct Vector3 v ) {
    struct Vector3 result;
    result.x = (v.x > 0.0f) - (f32)(v.x < 0.0f);
    result.y = (v.y > 0.0f) - (f32)(v.y < 0.0f);
    result.z = (v.z > 0.0f) - (f32)(v.z < 0.0f);
    return result;
}
/// @brief Component-wise truncate.
/// @param v Vector to truncate.
/// @return Vector with truncated values.
attr_always_inline attr_header
struct Vector3 vec3_trunc( struct Vector3 v ) {
    struct Vector3 result;
    result.x = (f32)(i32)v.x;
    result.y = (f32)(i32)v.y;
    result.z = (f32)(i32)v.z;
    return result;
}
/// @brief Component-wise floor.
/// @param v Vector to floor.
/// @return Vector with floored values.
attr_always_inline attr_header
struct Vector3 vec3_floor( struct Vector3 v ) {
    struct Vector3 result;
    result.x = (f32)( v.x < 0.0f ? (i32)( v.x - 1.0f ) : (i32)( v.x ) );
    result.y = (f32)( v.y < 0.0f ? (i32)( v.y - 1.0f ) : (i32)( v.y ) );
    result.z = (f32)( v.z < 0.0f ? (i32)( v.z - 1.0f ) : (i32)( v.z ) );
    return result;
}
/// @brief Component-wise ceil.
/// @param v Vector to ceil.
/// @return Vector with ceiled values.
attr_always_inline attr_header
struct Vector3 vec3_ceil( struct Vector3 v ) {
    struct Vector3 result;
    result.x = (f32)( v.x < 0.0f ? (i32)( v.x ) : (i32)( v.x + 1.0f ) );
    result.y = (f32)( v.y < 0.0f ? (i32)( v.y ) : (i32)( v.y + 1.0f ) );
    result.z = (f32)( v.z < 0.0f ? (i32)( v.z ) : (i32)( v.z + 1.0f ) );
    return result;
}
/// @brief Component-wise round.
/// @param v Vector to round.
/// @return Vector with rounded values.
attr_always_inline attr_header
struct Vector3 vec3_round( struct Vector3 v ) {
    struct Vector3 result;
    result.x = (f32)( v.x < 0.0f ? (i32)( v.x - 0.5f ) : (i32)( v.x + 0.5f ) );
    result.y = (f32)( v.y < 0.0f ? (i32)( v.y - 0.5f ) : (i32)( v.y + 0.5f ) );
    result.z = (f32)( v.z < 0.0f ? (i32)( v.z - 0.5f ) : (i32)( v.z + 0.5f ) );
    return result;
}
/// @brief Get fractional part.
/// @param v Vector.
/// @return Vector with fractional part.
attr_always_inline attr_header
struct Vector3 vec3_fract( struct Vector3 v ) {
    return vec3_sub( v, vec3_round(v) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_always_inline attr_header
struct Vector3 vec3_lerp(
    struct Vector3 a, struct Vector3 b, f32 t 
) {
    return vec3_add( vec3_mul( a, 1.0f - t ), vec3_mul( b, t ) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_always_inline attr_header
struct Vector3 vec3_mix(
    struct Vector3 a, struct Vector3 b, f32 t 
) {
    return vec3_lerp( a, b, t );
}
/// @brief Spherical interpolation from a to b.
/// @param a, b Interpolation.
/// @param t    Step of interpolation.
/// @return Spherical interpolation.
attr_core_api
struct Vector3 vec3_slerp( struct Vector3 a, struct Vector3 b, f32 t );
/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline attr_header
struct Vector3 vec3_step( struct Vector3 edge, struct Vector3 x ) {
    return vec3_new( f32_step( edge.x, x.x ), f32_step( edge.y, x.y ), f32_step( edge.z, x.z ) );
}
/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline attr_header
struct Vector3 vec3_step_scalar( f32 edge, struct Vector3 x ) {
    return vec3_step( vec3_set( edge ), x );
}
/// @brief Smooth step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header
struct Vector3 vec3_smoothstep(
    struct Vector3 edge0, struct Vector3 edge1, struct Vector3 x
) {
    return vec3_new(
        f32_smoothstep( edge0.x, edge1.x, x.x ),
        f32_smoothstep( edge0.y, edge1.y, x.y ),
        f32_smoothstep( edge0.z, edge1.z, x.z ) );
}
/// @brief Smooth step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header
struct Vector3 vec3_smoothstep_scalar( f32 edge0, f32 edge1, struct Vector3 x ) {
    return vec3_smoothstep( vec3_set( edge0 ), vec3_set( edge1 ), x );
}
/// @brief Smoother step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Vector in range a -> b.
attr_always_inline attr_header
struct Vector3 vec3_smootherstep(
    struct Vector3 edge0, struct Vector3 edge1, struct Vector3 x
) {
    return vec3_new(
        f32_smootherstep( edge0.x, edge1.x, x.x ),
        f32_smootherstep( edge0.y, edge1.y, x.y ),
        f32_smootherstep( edge0.z, edge1.z, x.z ) );
}
/// @brief Smoother step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Vector in range a -> b.
attr_always_inline attr_header
struct Vector3 vec3_smootherstep_scalar(
    f32 edge0, f32 edge1, struct Vector3 x
) {
    return vec3_smootherstep( vec3_set( edge0 ), vec3_set( edge1 ), x );
}
/// @brief Convert degrees to radians.
/// @param degrees Angles in degrees.
/// @return Angles in radians.
attr_always_inline attr_header
struct Vector3 vec3_radians( struct Vector3 degrees ) {
    return vec3_mul( degrees, F32_TO_RAD );
}
/// @brief Convert radians to degrees.
/// @param radians Angles in radians.
/// @return Angles in degrees.
attr_always_inline attr_header
struct Vector3 vec3_degrees( struct Vector3 radians ) {
    return vec3_mul( radians, F32_TO_DEG );
}
/// @brief Calculate sine of x.
/// @param angle Value to get sine of.
/// @return Sine of angle.
attr_always_inline attr_header
struct Vector3 vec3_sin( struct Vector3 angle ) {
    return vec3_new( f32_sin(angle.x), f32_sin(angle.y), f32_sin(angle.z) );
}
/// @brief Calculate cosine of x.
/// @param angle Value to get cosine of.
/// @return Cosine of angle.
attr_always_inline attr_header
struct Vector3 vec3_cos( struct Vector3 angle ) {
    return vec3_new( f32_cos(angle.x), f32_cos(angle.y), f32_cos(angle.z) );
}
/// @brief Calculate tangent of x.
/// @param angle Value to get tangent of.
/// @return Tangent of angle.
/// @warning Returns NaN if cosine of angle is zero.
attr_always_inline attr_header
struct Vector3 vec3_tan( struct Vector3 angle ) {
    return vec3_new( f32_tan(angle.x), f32_tan(angle.y), f32_tan(angle.z) );
}
/// @brief Calculate arcsine of x.
///
/// Does not produce NaN when outside valid range.
/// @param x Value to get arcsine of.
/// @return Arcsine of x clamped to -Pi -> Pi.
/// @see #F32_PI
/// @see #asin()
attr_always_inline attr_header
struct Vector3 vec3_asin( struct Vector3 angle ) {
    return vec3_new( f32_asin_real(angle.x), f32_asin_real(angle.y), f32_asin_real(angle.z) );
}
/// @brief Calculate arccosine of x.
/// @param x Value to get arccosine of.
/// @return Arccosine of x.
attr_always_inline attr_header
struct Vector3 vec3_acos( struct Vector3 angle ) {
    return vec3_new( f32_acos(angle.x), f32_acos(angle.y), f32_acos(angle.z) );
}
/// @brief Calculate arctangent of x.
/// @param x Value to get arctangent of.
/// @return Arctangent of x.
attr_always_inline attr_header
struct Vector3 vec3_atan( struct Vector3 angle ) {
    return vec3_new( f32_atan(angle.x), f32_atan(angle.y), f32_atan(angle.z) );
}
/// @brief Calculate 2 component arctangent of y and x.
/// @param y Value to get arctangent of.
/// @param x Value to get arctangent of.
/// @return Arctangent of y and x.
attr_always_inline attr_header
struct Vector3 vec3_atan2( struct Vector3 y, struct Vector3 x ) {
    return vec3_new( f32_atan2(y.x, x.x), f32_atan2(y.y, x.y), f32_atan2(y.z, x.z) );
}
/// @brief Raise base to the power of exponent.
/// @param base Number to raise.
/// @param exp Power to raise to.
/// @return Result.
attr_always_inline attr_header
struct Vector3 vec3_pow( struct Vector3 base, struct Vector3 exp ) {
    return vec3_new(
        f32_pow( base.x, exp.x ),
        f32_pow( base.y, exp.y ),
        f32_pow( base.z, exp.z ) );
}
/// @brief Raise e to the power of x.
/// @param x Exponent.
/// @return Result.
attr_always_inline attr_header
struct Vector3 vec3_exp( struct Vector3 x ) {
    return vec3_new(
        f32_exp( x.x ),
        f32_exp( x.y ),
        f32_exp( x.z ) );
}
/// @brief Calculate natural logarithm.
/// @param x Value to get natural logarithm of.
/// @return Natural logarithm.
attr_always_inline attr_header
struct Vector3 vec3_ln( struct Vector3 x ) {
    return vec3_new(
        f32_ln( x.x ),
        f32_ln( x.y ),
        f32_ln( x.z ) );
}
/// @brief Calculate logarithm base 2.
/// @param x Value to get logarithm base 2 of.
/// @return Logarithm base 2.
attr_always_inline attr_header
struct Vector3 vec3_log2( struct Vector3 x ) {
    return vec3_new(
        f32_log2( x.x ),
        f32_log2( x.y ),
        f32_log2( x.z ) );
}
/// @brief Calculate square root.
/// @param x Value to get square root of.
/// @return Square root.
attr_always_inline attr_header
struct Vector3 vec3_sqrt( struct Vector3 x ) {
    return vec3_new(
        f32_sqrt( x.x ),
        f32_sqrt( x.y ),
        f32_sqrt( x.z ) );
}
/// @brief Calculate reciprocal square root.
/// @param x Value to get reciprocal square root of.
/// @return Reciprocal square root.
attr_always_inline attr_header
struct Vector3 vec3_inversesqrt( struct Vector3 x ) {
    return vec3_new(
        f32_inversesqrt( x.x ),
        f32_inversesqrt( x.y ),
        f32_inversesqrt( x.z ) );
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if the square magnitude of a - b is < F32_EPSILON.
/// @see #F32_EPSILON
attr_always_inline attr_header
b32 vec3_cmp( struct Vector3 a, struct Vector3 b ) {
    return vec3_length_sqr( vec3_sub( a, b ) ) < F32_EPSILON;
}

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 3 values.
/// @return Vector with components from array.
attr_always_inline attr_header
struct IVector3 ivec3_from_array( i32 array[3] ) {
    return ivec3_new( array[0], array[1], array[2] );
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 3 values.
attr_always_inline attr_header
void ivec3_to_array( struct IVector3 v, i32* out_array ) {
    out_array[0] = v.array[0]; out_array[1] = v.array[1]; out_array[2] = v.array[2];
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline attr_header
struct IVector3 ivec3_add(
    struct IVector3 lhs, struct IVector3 rhs
) {
    return ivec3_new( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline attr_header
struct IVector3 ivec3_sub(
    struct IVector3 lhs, struct IVector3 rhs
) {
    return ivec3_new( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}
/// @brief Component-wise multiply vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline attr_header
struct IVector3 ivec3_mul_ivec3(
    struct IVector3 lhs, struct IVector3 rhs
) {
    return ivec3_new( lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline attr_header
struct IVector3 ivec3_mul( struct IVector3 lhs, i32 rhs ) {
    return ivec3_new( lhs.x * rhs, lhs.y * rhs, lhs.z * rhs );
}
/// @brief Component-wise divide vectors.
/// @param lhs, rhs Vectors to divide.
/// @return Result of division.
attr_always_inline attr_header
struct IVector3 ivec3_div_ivec3(
    struct IVector3 lhs, struct IVector3 rhs
) {
    return ivec3_new( lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline attr_header
struct IVector3 ivec3_div( struct IVector3 lhs, i32 rhs ) {
    return ivec3_new( lhs.x / rhs, lhs.y / rhs, lhs.z / rhs );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of modulus division.
attr_always_inline attr_header
struct IVector3 ivec3_mod( struct IVector3 lhs, i32 rhs ) {
    return ivec3_new(
        lhs.x % rhs,
        lhs.y % rhs,
        lhs.z % rhs );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of modulus division.
attr_always_inline attr_header
struct IVector3 ivec3_mod_ivec3( struct IVector3 lhs, struct IVector3 rhs ) {
    return ivec3_new(
        lhs.x % rhs.x,
        lhs.y % rhs.y,
        lhs.z % rhs.z );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline attr_header
struct IVector3 ivec3_neg( struct IVector3 x ) {
    return ivec3_new( -x.x, -x.y, -x.z );
}
/// @brief Rotate vector components to the left.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the left.
attr_always_inline attr_header
struct IVector3 ivec3_rotl( struct IVector3 x ) {
    return ivec3_new( x.y, x.z, x.x );
}
/// @brief Rotate vector components to the right.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the right.
attr_always_inline attr_header
struct IVector3 ivec3_rotr( struct IVector3 x ) {
    return ivec3_new( x.z, x.x, x.y );
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline attr_header
i32 ivec3_hadd( struct IVector3 x ) {
    return x.x + x.y + x.z;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline attr_header
i32 ivec3_hmul( struct IVector3 x ) {
    return x.x * x.y * x.z;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline attr_header
struct IVector3 ivec3_hadamard(
    struct IVector3 lhs, struct IVector3 rhs
) {
    return ivec3_new( lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z );
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline attr_header
f32 ivec3_dot( struct IVector3 lhs, struct IVector3 rhs ) {
    struct Vector3 _lhs = vec3_new( (f32)lhs.x, (f32)lhs.y, (f32)lhs.z );
    struct Vector3 _rhs = vec3_new( (f32)rhs.x, (f32)rhs.y, (f32)rhs.z );

    return vec3_dot( _lhs, _rhs );
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline attr_header
f32 ivec3_length_sqr( struct IVector3 x ) {
    struct Vector3 _x = vec3_new( (f32)x.x, (f32)x.y, (f32)x.z );
    return vec3_length_sqr( _x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_always_inline attr_header
f32 ivec3_length( struct IVector3 x ) {
    struct Vector3 _x = vec3_new( (f32)x.x, (f32)x.y, (f32)x.z );
    return vec3_length( _x );
}
/// @brief Component-wise abs.
/// @param v Vector.
/// @return Vector with absolute values.
attr_always_inline attr_header
struct IVector3 ivec3_abs( struct IVector3 x ) {
    struct IVector3 result;
    result.x = x.x < 0 ? -x.x : x.x;
    result.y = x.y < 0 ? -x.y : x.y;
    result.z = x.z < 0 ? -x.z : x.z;
    return result;
}
/// @brief Component-wise sign.
/// @param v Vector.
/// @return Vector with sign values.
attr_always_inline attr_header
struct IVector3 ivec3_sign( struct IVector3 v ) {
    struct IVector3 result;
    result.x = (v.x > 0) - (v.x < 0);
    result.y = (v.y > 0) - (v.y < 0);
    result.z = (v.z > 0) - (v.z < 0);
    return result;
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if vector components are equal.
attr_always_inline attr_header
b32 ivec3_cmp( struct IVector3 a, struct IVector3 b ) {
    return
        a.x == b.x &&
        a.y == b.y &&
        a.z == b.z;
}

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#if defined(CORE_CPLUSPLUS)
    #if !defined(CORE_CPP_MATH_VECTOR3_HPP)
        #include "core/cpp/math/vector3.hpp"
    #endif
    typedef Vector3CPP  vec3;
    typedef IVector3CPP ivec3;
#endif

typedef vec3 rgb;
typedef vec3 hsl;

#endif /* header guard */
