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
        /// @brief X, Y and Z as an array.
        f32 v[3];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief 3 Component 32-bit Floating Point Vector.
    typedef struct Vector3 vec3;
#endif

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
        /// @brief X, Y and Z components as an array.
        i32 v[3];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief 3 Component 32-bit Signed Integer Vector.
    typedef struct IVector3 ivec3;
#endif

#if defined(CORE_DOXYGEN)
    /// @brief Construct a Vector3.
    /// @param x, y, z (f32) Components.
    /// @return Vector3.
    #define v3( x, y, z )
    /// @brief Construct an IVector3.
    /// @param x, y, z (i32) Components.
    /// @return IVector3.
    #define iv3( x, y, z )
#else

#if defined(CORE_CPLUSPLUS)
    #define v3( x, y, z )\
        Vector3{ .v={ (x), (y), (z) } }
    #define iv3( x, y, z )\
        IVector3{ .v={ (x), (y), (z) } }
#else /* C++ constructors */
    #define v3( x, y, z )\
        (struct Vector3){ .v={ (x), (y), (z) } }
    #define iv3( x, y, z )\
        (struct IVector3){ .v={ (x), (y), (z) } }
#endif /* C constructors */

#endif /* Doxygen */

/// @brief Construct a new Vector3 with identical components.
/// @param x (f32) Value for components.
/// @return Vector3.
#define v3_set( x )\
    v3( (x), (x), (x) )
/// @brief Construct a new RGB (Vector3).
/// @param r, g, b (f32) Red, Green and Blue channels.
/// @return RGB (Vector3).
#define rgb( r, g, b ) v3( r, g, b )
/// @brief Construct a new HSL (Vector3).
/// @param h, s, l (f32) Hue, Saturation and Lightness.
/// @return HSL (Vector3).
#define hsl( h, s, l ) v3( wrap_deg( h ), clamp01( s ), clamp01( l ) )
/// @brief Construct a new IVector3 with identical components.
/// @param x (i32) Value for components.
/// @return IVector3.
#define iv3_set( x )\
    iv3( (x), (x), (x) )

/// @brief Vector3 zero constant.
#define VEC3_ZERO    ( v3_set( 0.0f ) )
/// @brief Vector3 one constant.
#define VEC3_ONE     ( v3_set( 1.0f ) )
/// @brief Vector3 left constant.
#define VEC3_LEFT    ( v3( -1.0f,  0.0f,  0.0f ) )
/// @brief Vector3 right constant.
#define VEC3_RIGHT   ( v3(  1.0f,  0.0f,  0.0f ) )
/// @brief Vector3 up constant.
#define VEC3_UP      ( v3(  0.0f,  1.0f,  0.0f ) )
/// @brief Vector3 down constant.
#define VEC3_DOWN    ( v3(  0.0f, -1.0f,  0.0f ) )
/// @brief Vector3 forward constant.
#define VEC3_FORWARD ( v3(  0.0f,  0.0f,  1.0f ) )
/// @brief Vector3 back constant.
#define VEC3_BACK    ( v3(  0.0f,  0.0f, -1.0f ) )

/// @brief IVector3 zero constant.
#define IVEC3_ZERO    ( iv3_set( 0 ) )
/// @brief IVector3 one constant.
#define IVEC3_ONE     ( iv3_set( 1 ) )
/// @brief IVector3 left constant.
#define IVEC3_LEFT    ( iv3( -1,  0,  0 ) )
/// @brief IVector3 right constant.
#define IVEC3_RIGHT   ( iv3(  1,  0,  0 ) )
/// @brief IVector3 up constant.
#define IVEC3_UP      ( iv3(  0,  1,  0 ) )
/// @brief IVector3 down constant.
#define IVEC3_DOWN    ( iv3(  0, -1,  0 ) )
/// @brief IVector3 forward constant.
#define IVEC3_FORWARD ( iv3(  0,  0,  1 ) )
/// @brief IVector3 zero constant.
#define IVEC3_BACK    ( iv3(  0,  0, -1 ) )

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
#define RGB_BLACK   ( v3_set( 0.0f ) )
/// @brief RGB white constant.
#define RGB_WHITE   ( v3_set( 1.0f ) )

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 3 values.
/// @return Vector with components from array.
attr_always_inline
attr_header struct Vector3 v3_from_array( const f32 array[3] ) {
    return v3( array[0], array[1], array[2] );
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 3 values.
attr_always_inline
attr_header void v3_to_array( struct Vector3 v, f32* out_array ) {
    out_array[0] = v.v[0]; out_array[1] = v.v[1]; out_array[2] = v.v[2];
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
attr_always_inline
attr_header struct Vector3 v3_add(
    struct Vector3 lhs, struct Vector3 rhs
) {
    return v3( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline
attr_header struct Vector3 v3_sub(
    struct Vector3 lhs, struct Vector3 rhs
) {
    return v3( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline
attr_header struct Vector3 v3_mul( struct Vector3 lhs, f32 rhs ) {
    return v3( lhs.x * rhs, lhs.y * rhs, lhs.z * rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline
attr_header struct Vector3 v3_div( struct Vector3 lhs, f32 rhs ) {
    return v3( lhs.x / rhs, lhs.y / rhs, lhs.z / rhs );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline
attr_header struct Vector3 v3_neg( struct Vector3 x ) {
    return v3( -x.x, -x.y, -x.z );
}
/// @brief Rotate vector components to the left.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the left.
attr_always_inline
attr_header struct Vector3 v3_rotl( struct Vector3 x ) {
    return v3( x.y, x.z, x.x );
}
/// @brief Rotate vector components to the right.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the right.
attr_always_inline
attr_header struct Vector3 v3_rotr( struct Vector3 x ) {
    return v3( x.z, x.x, x.y );
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline
attr_header f32 v3_hadd( struct Vector3 x ) {
    return x.x + x.y + x.z;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline
attr_header f32 v3_hmul( struct Vector3 x ) {
    return x.x * x.y * x.z;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline
attr_header struct Vector3 v3_hadamard(
    struct Vector3 lhs, struct Vector3 rhs
) {
    return v3( lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z );
}
/// @brief Cross product between two vectors.
/// @param lhs, rhs Vectors to cross.
/// @return Result of cross product.
attr_always_inline
attr_header struct Vector3 v3_cross(
    struct Vector3 lhs, struct Vector3 rhs
) {
    return v3(
        ( lhs.y * rhs.z ) - ( lhs.z * rhs.y ),
        ( lhs.z * rhs.x ) - ( lhs.x * rhs.z ),
        ( lhs.x * rhs.y ) - ( lhs.y * rhs.x ) );
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline
attr_header f32 v3_dot( struct Vector3 lhs, struct Vector3 rhs ) {
    return v3_hadd( v3_hadamard( lhs, rhs ) );
}
/// @brief Get the maximum component in vector.
/// @param x Vector to get maximum of.
/// @return Component with largest value.
attr_always_inline
attr_header f32 v3_max( struct Vector3 x ) {
    f32 _0 = x.x < x.y ? x.y : x.x;
    return _0 < x.z ? x.z : _0;
}
/// @brief Get the component in between min and max component.
/// @param x Vector to get mid of.
/// @return Component in between min and max component.
attr_always_inline
attr_header f32 v3_mid( struct Vector3 x ) {
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
attr_always_inline
attr_header f32 v3_min( struct Vector3 x ) {
    f32 _0 = x.x < x.y ? x.x : x.y;
    return _0 < x.z ? _0 : x.z;
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline
attr_header f32 v3_sqrmag( struct Vector3 x ) {
    return v3_dot( x, x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_core_api f32 v3_mag( struct Vector3 x );
/// @brief Normalize a Vector.
/// @param x Vector to normalize.
/// @return Normalized vector or zero vector if magnitude == 0.
attr_core_api struct Vector3 v3_normalize( struct Vector3 x );
/// @brief Reflect direction vector off surface.
/// @param direction Direction vector to reflect.
/// @param normal Normal of the surface to reflect off of.
/// @return Reflected vector.
attr_always_inline
attr_header struct Vector3 v3_reflect(
    struct Vector3 direction, struct Vector3 normal
) {
    return v3_mul(
        v3_sub( normal, direction ),
        2.0f * v3_dot( direction, normal ) );
}
/// @brief Clamp vector's magnitude to range min -> max.
/// @param v Vector to clamp.
/// @param min_, max_ Range to clamp to, min_ must be < max_.
/// @return Clamped vector.
attr_always_inline
attr_header struct Vector3 v3_clamp_mag(
    struct Vector3 v, f32 min_, f32 max_
) {
    f32 mag = v3_mag( v );
    if( mag == 0.0f ) {
        return VEC3_ZERO;
    }
    f32 new_mag = ( mag < min_ ? min_ : ( mag > max_ ? max_ : mag ) );
    return v3_mul( v3_div( v, mag ), new_mag );
}
/// @brief Calculate the angle between two vectors.
/// @param a, b Vectors to calculate angle of.
/// @return Angle (in radians) between vectors.
attr_core_api f32 v3_angle( struct Vector3 a, struct Vector3 b );
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_core_api struct Vector3 v3_lerp(
    struct Vector3 a, struct Vector3 b, f32 t );
/// @brief Smooth step interpolation.
/// @param a, b Range of interpolation.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_core_api struct Vector3 v3_smooth_step(
    struct Vector3 a, struct Vector3 b, f32 t );
/// @brief Smoother step interpolation.
/// @param a, b Range of interpolation.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_core_api struct Vector3 v3_smoother_step(
    struct Vector3 a, struct Vector3 b, f32 t );
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if the square magnitude of a - b is < F32_EPSILON.
/// @see #F32_EPSILON
attr_always_inline
attr_header b32 v3_cmp( struct Vector3 a, struct Vector3 b ) {
    return v3_sqrmag( v3_sub( a, b ) ) < F32_EPSILON;
}

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 3 values.
/// @return Vector with components from array.
attr_always_inline
attr_header struct IVector3 iv3_from_array( i32 array[3] ) {
    return iv3( array[0], array[1], array[2] );
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 3 values.
attr_always_inline
attr_header void iv3_to_array( struct IVector3 v, i32* out_array ) {
    out_array[0] = v.v[0]; out_array[1] = v.v[1]; out_array[2] = v.v[2];
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline
attr_header struct IVector3 iv3_add(
    struct IVector3 lhs, struct IVector3 rhs
) {
    return iv3( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline
attr_header struct IVector3 iv3_sub(
    struct IVector3 lhs, struct IVector3 rhs
) {
    return iv3( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline
attr_header struct IVector3 iv3_mul( struct IVector3 lhs, i32 rhs ) {
    return iv3( lhs.x * rhs, lhs.y * rhs, lhs.z * rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline
attr_header struct IVector3 iv3_div( struct IVector3 lhs, i32 rhs ) {
    return iv3( lhs.x / rhs, lhs.y / rhs, lhs.z / rhs );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline
attr_header struct IVector3 iv3_neg( struct IVector3 x ) {
    return iv3( -x.x, -x.y, -x.z );
}
/// @brief Rotate vector components to the left.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the left.
attr_always_inline
attr_header struct IVector3 iv3_rotl( struct IVector3 x ) {
    return iv3( x.y, x.z, x.x );
}
/// @brief Rotate vector components to the right.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the right.
attr_always_inline
attr_header struct IVector3 iv3_rotr( struct IVector3 x ) {
    return iv3( x.z, x.x, x.y );
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline
attr_header i32 iv3_hadd( struct IVector3 x ) {
    return x.x + x.y + x.z;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline
attr_header i32 iv3_hmul( struct IVector3 x ) {
    return x.x * x.y * x.z;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline
attr_header struct IVector3 iv3_hadamard(
    struct IVector3 lhs, struct IVector3 rhs
) {
    return iv3( lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z );
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline
attr_header f32 iv3_dot( struct IVector3 lhs, struct IVector3 rhs ) {
    struct Vector3 _lhs = v3( (f32)lhs.x, (f32)lhs.y, (f32)lhs.z );
    struct Vector3 _rhs = v3( (f32)rhs.x, (f32)rhs.y, (f32)rhs.z );

    return v3_dot( _lhs, _rhs );
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline
attr_header f32 iv3_sqrmag( struct IVector3 x ) {
    struct Vector3 _x = v3( (f32)x.x, (f32)x.y, (f32)x.z );
    return v3_sqrmag( _x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_always_inline
attr_header f32 iv3_mag( struct IVector3 x ) {
    struct Vector3 _x = v3( (f32)x.x, (f32)x.y, (f32)x.z );
    return v3_mag( _x );
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if vector components are equal.
attr_always_inline
attr_header b32 iv3_cmp( struct IVector3 a, struct IVector3 b ) {
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
