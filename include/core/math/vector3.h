#if !defined(CORE_MATH_VECTOR3_H)
#define CORE_MATH_VECTOR3_H
/**
 * @file   vector3.h
 * @brief  Vector3.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 28, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/math/trig.h"
#include "core/math/exponential.h"
#include "core/math/common.h"
#include "core/math/vector2.h"

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
                /// @brief X and Y components -- as Red and Green channels.
                struct {
                    /// @brief X component -- as Red channel.
                    f32 r;
                    /// @brief Y component -- as Green channel.
                    f32 g;
                };
                /// @brief Swizzle X and Y components.
                struct Vector2 xy;
                /// @brief Swizzle X and Y components -- as Red and Green channels.
                struct Vector2 rg;
            };
            /// @brief Z component.
            union {
                /// @brief Z component.
                f32 z;
                /// @brief Z component -- as Blue channel.
                f32 b;
            };
        };
        /// @brief Swizzle Y and Z components.
        struct {
            f32 __unused0;
            /// @brief Swizzle Y and Z components.
            union {
                /// @brief Swizzle Y and Z components.
                struct Vector2 yz;
                /// @brief Swizzle Y and Z components -- as Green and Blue channels.
                struct Vector2 gb;
            };
        };
        /// @brief X, Y and Z components as an array.
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
/// @brief 3 Component 32-bit Unsigned Integer Vector.
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
/// @brief 3 Component 64-bit Floating Point Vector.
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
/// @brief 3 Component 32-bit Boolean Vector.
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

#if defined(__cplusplus)
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define vec3_new( x, y, z )   Vector3 { .array={x, y, z} }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define ivec3_new( x, y, z ) IVector3 { .array={ x, y, z } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define bvec3_new( x, y, z ) BVector3 { .array={ x, y, z } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define uvec3_new( x, y, z ) UVector3 { .array={ x, y, z } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define dvec3_new( x, y, z ) DVector3 { .array={ x, y, z } }
#else
    /// @brief 3 Component 32-bit Floating Point Vector.
    typedef struct Vector3   vec3;
    /// @brief 3 Component 32-bit Integer Vector.
    typedef struct IVector3 ivec3;
    /// @brief 3 Component 32-bit Boolean Vector.
    typedef struct BVector3 bvec3;
    /// @brief 3 Component 32-bit Unsigned Integer Vector.
    typedef struct UVector3 uvec3;
    /// @brief 3 Component 64-bit Floating Point Vector.
    typedef struct DVector3 dvec3;

    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define vec3_new( x, y, z )   (struct Vector3){ .array={ x, y, z } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define ivec3_new( x, y, z ) (struct IVector3){ .array={ x, y, z } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define bvec3_new( x, y, z ) (struct BVector3){ .array={ x, y, z } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define uvec3_new( x, y, z ) (struct UVector3){ .array={ x, y, z } }
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define dvec3_new( x, y, z ) (struct DVector3){ .array={ x, y, z } }

    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define  vec3( x, y, z )  vec3_new( x, y, z )
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define ivec3( x, y, z ) ivec3_new( x, y, z )
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define bvec3( x, y, z ) bvec3_new( x, y, z )
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define uvec3( x, y, z ) uvec3_new( x, y, z )
    /// @brief Create new vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @return Vector.
    #define dvec3( x, y, z ) dvec3_new( x, y, z )
#endif

/// @brief Vector3 zero constant.
#define VEC3_ZERO    ( vec3_new( 0.0f, 0.0f, 0.0f ) )
/// @brief Vector3 one constant.
#define VEC3_ONE     ( vec3_new( 1.0f, 1.0f, 1.0f ) )
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
#define IVEC3_ZERO    ( ivec3_new( 0, 0, 0 ) )
/// @brief IVector3 one constant.
#define IVEC3_ONE     ( ivec3_new( 1, 1, 1 ) )
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
/// @brief IVector3 back constant.
#define IVEC3_BACK    ( ivec3_new(  0,  0, -1 ) )

/// @brief BVector3 zero constant.
#define BVEC3_ZERO ( bvec3_new( false, false, false ) )
/// @brief BVector3 one constant.
#define BVEC3_ONE  ( bvec3_new( true, true, true ) )

/// @brief RGB red constant.
#define RGB_RED     ( vec3_new( 1.0f, 0.0f, 0.0f ) )
/// @brief RGB green constant.
#define RGB_GREEN   ( vec3_new( 0.0f, 1.0f, 0.0f ) )
/// @brief RGB blue constant.
#define RGB_BLUE    ( vec3_new( 0.0f, 0.0f, 1.0f ) )
/// @brief RGB yellow constant.
#define RGB_YELLOW  ( vec3_new( 1.0f, 1.0f, 0.0f ) )
/// @brief RGB magenta constant.
#define RGB_MAGENTA ( vec3_new( 1.0f, 0.0f, 1.0f ) )
/// @brief RGB cyan constant.
#define RGB_CYAN    ( vec3_new( 0.0f, 1.0f, 1.0f ) )
/// @brief RGB black constant.
#define RGB_BLACK   ( vec3_new( 0.0f, 0.0f, 0.0f ) )
/// @brief RGB white constant.
#define RGB_WHITE   ( vec3_new( 1.0f, 1.0f, 1.0f ) )

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
void array_from_vec3( struct Vector3 v, f32* out_array ) {
    out_array[0] = v.array[0]; out_array[1] = v.array[1]; out_array[2] = v.array[2];
}
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
f32 vec3_max( struct Vector3 x ) {
    f32 _0 = x.x < x.y ? x.y : x.x;
    return _0 < x.z ? x.z : _0;
}
/// @brief Component-wise maximum value.
/// @param x, y Vectors.
/// @return Vector with maximum value in components.
attr_always_inline attr_header
struct Vector3 vec3_max_vec3( struct Vector3 x, struct Vector3 y ) {
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
f32 vec3_mid( struct Vector3 x ) {
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
f32 vec3_min( struct Vector3 x ) {
    f32 _0 = x.x < x.y ? x.x : x.y;
    return _0 < x.z ? _0 : x.z;
}
/// @brief Component-wise minimum value.
/// @param x, y Vectors.
/// @return Vector with minimum value in components.
attr_always_inline attr_header
struct Vector3 vec3_min_vec3( struct Vector3 x, struct Vector3 y ) {
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
/// @param d Direction vector to reflect.
/// @param n Normal of the surface to reflect off of.
/// @return Reflected vector.
attr_always_inline attr_header
struct Vector3 vec3_reflect(
    struct Vector3 d, struct Vector3 n
) {
    return vec3_mul(
        vec3_sub( n, d ), 2.0f * vec3_dot( d, n ) );
}
/// @brief Component-wise clamp.
/// @param v          Vector.
/// @param min_, max_ Range.
/// @return Clamped vector.
attr_always_inline attr_header
struct Vector3 vec3_clamp(
    struct Vector3 v, struct Vector3 min_, struct Vector3 max_
) {
    struct Vector3 result;
    result.x = ( (v.x) < (min_.x) ? (min_.x) : ( (v.x) > (max_.x) ? (max_.x) : (v.x) ) );
    result.y = ( (v.y) < (min_.y) ? (min_.y) : ( (v.y) > (max_.y) ? (max_.y) : (v.y) ) );
    result.z = ( (v.z) < (min_.z) ? (min_.z) : ( (v.z) > (max_.z) ? (max_.z) : (v.z) ) );
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
    return vec3_sub( v, vec3_floor(v) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t    Where to interpolate to.
/// @return Vector in range [a,b].
attr_always_inline attr_header
struct Vector3 vec3_lerp(
    struct Vector3 a, struct Vector3 b, f32 t 
) {
    return vec3_new(
        f32_lerp( a.x, b.x, t ),
        f32_lerp( a.y, b.y, t ),
        f32_lerp( a.z, b.z, t ) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t    Where to interpolate to.
/// @return Vector in range [a,b].
attr_always_inline attr_header
struct Vector3 vec3_lerp_vec3(
    struct Vector3 a, struct Vector3 b, struct Vector3 t 
) {
    return vec3_new(
        f32_lerp( a.x, b.x, t.x ),
        f32_lerp( a.y, b.y, t.y ),
        f32_lerp( a.z, b.z, t.z ) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t    Where to interpolate to.
/// @return Vector in range [a,b].
attr_always_inline attr_header
struct Vector3 vec3_mix(
    struct Vector3 a, struct Vector3 b, f32 t 
) {
    return vec3_lerp( a, b, t );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t    Where to interpolate to.
/// @return Vector in range [a,b].
attr_always_inline attr_header
struct Vector3 vec3_mix_vec3(
    struct Vector3 a, struct Vector3 b, struct Vector3 t 
) {
    return vec3_lerp_vec3( a, b, t );
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
struct Vector3 vec3_step_vec3( struct Vector3 edge, struct Vector3 x ) {
    return vec3_new( f32_step( edge.x, x.x ), f32_step( edge.y, x.y ), f32_step( edge.z, x.z ) );
}
/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline attr_header
struct Vector3 vec3_step( f32 edge, struct Vector3 x ) {
    return vec3_step_vec3( vec3_new( edge, edge, edge ), x );
}
/// @brief Smooth step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header
struct Vector3 vec3_smoothstep_vec3(
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
struct Vector3 vec3_smoothstep( f32 edge0, f32 edge1, struct Vector3 x ) {
    return vec3_smoothstep_vec3(
        vec3_new( edge0, edge0, edge0 ),
        vec3_new( edge1, edge1, edge1 ),
        x );
}
/// @brief Smoother step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Vector in range a -> b.
attr_always_inline attr_header
struct Vector3 vec3_smootherstep_vec3(
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
struct Vector3 vec3_smootherstep(
    f32 edge0, f32 edge1, struct Vector3 x
) {
    return vec3_smootherstep_vec3(
        vec3_new( edge0, edge0, edge0 ),
        vec3_new( edge1, edge1, edge1 ),
        x );
}
/// @brief Check if vector components are NaN.
/// @param x Vector.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 vec3_isnan( struct Vector3 x ) {
    return bvec3_new(
        f32_isnan( x.x ),
        f32_isnan( x.y ),
        f32_isnan( x.z ) );
}
/// @brief Check if vector components are infinite.
/// @param x Vector.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 vec3_isinf( struct Vector3 x ) {
    return bvec3_new(
        f32_isinf( x.x ), 
        f32_isinf( x.y ),
        f32_isinf( x.z ) );
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
/// @param angle Value to get arcsine of.
/// @return Arcsine of x clamped to -Pi -> Pi.
/// @see #F32_PI
/// @see #asin()
attr_always_inline attr_header
struct Vector3 vec3_asin( struct Vector3 angle ) {
    return vec3_new( f32_asin_real(angle.x), f32_asin_real(angle.y), f32_asin_real(angle.z) );
}
/// @brief Calculate arccosine of x.
/// @param angle Value to get arccosine of.
/// @return Arccosine of x.
attr_always_inline attr_header
struct Vector3 vec3_acos( struct Vector3 angle ) {
    return vec3_new( f32_acos(angle.x), f32_acos(angle.y), f32_acos(angle.z) );
}
/// @brief Calculate arctangent of x.
/// @param angle Value to get arctangent of.
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
/// @brief Raise 2 to the power of x.
/// @param x Exponent.
/// @return Result.
attr_always_inline attr_header
struct Vector3 vec3_exp2( struct Vector3 x ) {
    return vec3_new(
        f32_exp2( x.x ),
        f32_exp2( x.y ),
        f32_exp2( x.z ) );
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
/// @brief Calculate logarithm base 10.
/// @param x Value to get logarithm base 10 of.
/// @return Logarithm base 10.
attr_always_inline attr_header
struct Vector3 vec3_log10( struct Vector3 x ) {
    return vec3_new(
        f32_log10( x.x ),
        f32_log10( x.y ),
        f32_log10( x.z ) );
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
/// @brief Calculate cube root.
/// @param x Value to get cube root of.
/// @return Cube root.
attr_always_inline attr_header
struct Vector3 vec3_cbrt( struct Vector3 x ) {
    return vec3_new(
        f32_cbrt( x.x ),
        f32_cbrt( x.y ),
        f32_cbrt( x.z ) );
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if the square magnitude of a - b is < F32_EPSILON.
/// @see #F32_EPSILON
attr_always_inline attr_header
b32 vec3_cmp( struct Vector3 a, struct Vector3 b ) {
    return vec3_length_sqr( vec3_sub( a, b ) ) < F32_EPSILON;
}
/// @brief Check if less than, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 vec3_lt( struct Vector3 a, struct Vector3 b ) {
    return bvec3_new(
        a.x < b.x,
        a.y < b.y,
        a.z < b.z );
}
/// @brief Check if greater than, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 vec3_gt( struct Vector3 a, struct Vector3 b ) {
    return bvec3_new(
        a.x > b.x,
        a.y > b.y,
        a.z > b.z );
}
/// @brief Check if less than or equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 vec3_lteq( struct Vector3 a, struct Vector3 b ) {
    return bvec3_new(
        a.x <= b.x,
        a.y <= b.y,
        a.z <= b.z );
}
/// @brief Check if greater than or equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 vec3_gteq( struct Vector3 a, struct Vector3 b ) {
    return bvec3_new( 
        a.x >= b.x,
        a.y >= b.y,
        a.z >= b.z );
}
/// @brief Check if equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 vec3_eq( struct Vector3 a, struct Vector3 b ) {
    return bvec3_new(
        f32_cmp( a.x, b.x ),
        f32_cmp( a.y, b.y ),
        f32_cmp( a.z, b.z ) );
}
/// @brief Check if not equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 vec3_neq( struct Vector3 a, struct Vector3 b ) {
    return bvec3_new(
        !f32_cmp( a.x, b.x ),
        !f32_cmp( a.y, b.y ),
        !f32_cmp( a.z, b.z ) );
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
void array_from_ivec3( struct IVector3 v, i32* out_array ) {
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
/// @param x Vector.
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
/// @brief Get the minimum component in vector.
/// @param x Vector to get minimum of.
/// @return Component with smallest value.
attr_always_inline attr_header
i32 ivec3_min( struct IVector3 x ) {
    i32 _0 = x.x < x.y ? x.x : x.y;
    return _0 < x.z ? _0 : x.z;
}
/// @brief Component-wise minimum value.
/// @param x, y Vectors.
/// @return Vector with minimum value in components.
attr_always_inline attr_header
struct IVector3 ivec3_min_ivec3( struct IVector3 x, struct IVector3 y ) {
    struct IVector3 result;
    result.x = x.x < y.x ? x.x : y.x;
    result.y = x.y < y.y ? x.y : y.y;
    result.z = x.z < y.z ? x.z : y.z;
    return result;
}
/// @brief Get the component in between min and max component.
/// @param x Vector to get mid of.
/// @return Component in between min and max component.
attr_always_inline attr_header
i32 ivec3_mid( struct IVector3 x ) {
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
/// @brief Get the maximum component in vector.
/// @param x Vector to get maximum of.
/// @return Component with largest value.
attr_always_inline attr_header
i32 ivec3_max( struct IVector3 x ) {
    i32 _0 = x.x < x.y ? x.y : x.x;
    return _0 < x.z ? x.z : _0;
}
/// @brief Component-wise maximum value.
/// @param x, y Vectors.
/// @return Vector with maximum value in components.
attr_always_inline attr_header
struct IVector3 ivec3_max_ivec3( struct IVector3 x, struct IVector3 y ) {
    struct IVector3 result;
    result.x = x.x < y.x ? y.x : x.x;
    result.y = x.y < y.y ? y.y : x.y;
    result.z = x.z < y.z ? y.z : x.z;
    return result;
}
/// @brief Component-wise clamp.
/// @param v        Vector.
/// @param min, max Range.
/// @return Clamped vector.
attr_always_inline attr_header
struct IVector3 ivec3_clamp(
    struct IVector3 v, struct IVector3 min, struct IVector3 max
) {
    struct IVector3 result;
    result.x = ( (v.x) < (min.x) ? (min.x) : ( (v.x) > (max.x) ? (max.x) : (v.x) ) );
    result.y = ( (v.y) < (min.y) ? (min.y) : ( (v.y) > (max.y) ? (max.y) : (v.y) ) );
    result.z = ( (v.z) < (min.z) ? (min.z) : ( (v.z) > (max.z) ? (max.z) : (v.z) ) );
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
/// @brief Check if less than, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 ivec3_lt( struct IVector3 a, struct IVector3 b ) {
    return bvec3_new(
        a.x < b.x,
        a.y < b.y,
        a.z < b.z );
}
/// @brief Check if greater than, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 ivec3_gt( struct IVector3 a, struct IVector3 b ) {
    return bvec3_new(
        a.x > b.x,
        a.y > b.y,
        a.z > b.z );
}
/// @brief Check if less than or equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 ivec3_lteq( struct IVector3 a, struct IVector3 b ) {
    return bvec3_new(
        a.x <= b.x,
        a.y <= b.y,
        a.z <= b.z );
}
/// @brief Check if greater than or equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 ivec3_gteq( struct IVector3 a, struct IVector3 b ) {
    return bvec3_new( 
        a.x >= b.x,
        a.y >= b.y,
        a.z >= b.z );
}
/// @brief Check if equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 ivec3_eq( struct IVector3 a, struct IVector3 b ) {
    return bvec3_new(
        a.x == b.x,
        a.y == b.y,
        a.z == b.z );
}
/// @brief Check if not equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 ivec3_neq( struct IVector3 a, struct IVector3 b ) {
    return bvec3_new(
        a.x != b.x,
        a.y != b.y,
        a.z != b.z );
}

/// @brief Check if equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 bvec3_eq( struct BVector3 a, struct BVector3 b ) {
    return bvec3_new( (a.x == b.x), (a.y == b.y), (a.z == b.z) );
}
/// @brief Check if not equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header
struct BVector3 bvec3_neq( struct BVector3 a, struct BVector3 b ) {
    return bvec3_new( !(a.x == b.x), !(a.y == b.y), !(a.z == b.z) );
}
/// @brief Check if any component of vector is true.
/// @param x Vector to check.
/// @return
///     - true  : Any component is true.
///     - false : All components false.
attr_always_inline attr_header
b32 bvec3_any( struct BVector3 x ) {
    return x.x || x.y || x.z;
}
/// @brief Check if all components of vector are true.
/// @param x Vector to check.
/// @return
///     - true  : All components are true.
///     - false : One or more components are false.
attr_always_inline attr_header
b32 bvec3_all( struct BVector3 x ) {
    return x.x && x.y && x.z;
}
/// @brief Not components.
/// @param x Vector to check.
/// @return Booleans notted.
attr_always_inline attr_header
struct BVector3 bvec3_flip( struct BVector3 x ) {
    return bvec3_new( !x.x, !x.y, !x.z );
}

#if !defined(CORE_CPP_MATH_VECTOR3_HPP)
    #include "core/cpp/math/vector3.hpp"
#endif

#endif /* header guard */
