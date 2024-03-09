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
        /// @brief X, Y, Z and W components as an array.
        f32 v[4];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief 4 Component 32-bit Floating Point Vector.
    typedef struct Vector4 vec4;
    /// @brief RGBA 32-bit Floating Point.
    typedef struct Vector4 rgba;
#endif

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
                        /// @brief X and Y components as a #IVector2.
                        struct IVector2 xy;
                    };
                    /// @brief Z component.
                    union {
                        /// @brief Z component.
                        i32 z;
                    };
                };
                /// @brief X, Y and Z components as a #IVector3.
                struct IVector3 xyz;
            };
            /// @brief W component.
            union {
                /// @brief W component.
                i32 w;
            };
        };
        /// @brief X, Y, Z and W components as an array.
        i32 v[4];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief 4 Component 32-bit Signed Integer Vector.
    typedef struct IVector4 ivec4;
#endif

#if defined(CORE_DOXYGEN)
    /// @brief Construct a Vector4.
    /// @param x, y, z, w (f32) Components.
    /// @return Vector4.
    #define v4( x, y, z, w )
    /// @brief Construct an IVector4.
    /// @param x, y, z, w (i32) Components.
    /// @return IVector4.
    #define iv4( x, y, z, w )
#else /* Doxygen */

#if defined(CORE_CPLUSPLUS)
    #define v4( x, y, z, w )\
        Vector4{ .v={ (x), (y), (z), (w) } }
    #define iv4( x, y, z, w )\
        IVector4{ .v={ (x), (y), (z), (w) } }
#else /* C++ constructors */
    #define v4( x, y, z, w )\
        (struct Vector4){ .v={ (x), (y), (z), (w) } }
    #define iv4( x, y, z, w )\
        (struct IVector4){ .v={ (x), (y), (z), (w) } }
#endif /* C constructors */ 

#endif /* Doxygen */

/// @brief Construct a new Vector4 with identical components.
/// @param x (f32) Value for components.
/// @return Vector4.
#define v4_set( x )\
    v4( (x), (x), (x), (x) )
/// @brief Construct a new RGBA (Vector4).
/// @param r, g, b, a (f32) Red, Green, Blue and Alpha channels.
/// @return RGBA (Vector4).
#define rgba( r, g, b, a ) v4( r, g, b, a )
/// @brief Construct a new IVector4 with identical components.
/// @param x (i32) Value for components.
/// @return IVector4.
#define iv4_set( x )\
    iv4( (x), (x), (x), (x) )

/// @brief Vector4 zero constant.
#define VEC4_ZERO ( v4_set( 0.0f ) )
/// @brief Vector4 one constant.
#define VEC4_ONE  ( v4_set( 1.0f ) )

/// @brief IVector4 zero constant.
#define IVEC4_ZERO ( iv4_set( 0 ) )
/// @brief IVector4 one constant.
#define IVEC4_ONE  ( iv4_set( 1 ) )

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
#define RGBA_WHITE   ( v4_set( 1.0f ) )
/// @brief RGBA clear constant.
#define RGBA_CLEAR   ( v4_set( 0.0f ) )

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 4 values.
/// @return Vector with components from array.
attr_always_inline
attr_header struct Vector4 v4_from_array( const f32 array[4] ) {
    return v4( array[0], array[1], array[2], array[3] );
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 4 values.
attr_always_inline
attr_header void v4_to_array( struct Vector4 v, f32* out_array ) {
    out_array[0] = v.v[0]; out_array[1] = v.v[1];
    out_array[2] = v.v[2]; out_array[3] = v.v[3];
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline
attr_header struct Vector4 v4_add(
    struct Vector4 lhs, struct Vector4 rhs
) {
    return v4(
        lhs.x + rhs.x, lhs.y + rhs.y,
        lhs.z + rhs.z, lhs.w + rhs.w );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline
attr_header struct Vector4 v4_sub(
    struct Vector4 lhs, struct Vector4 rhs
) {
    return v4(
        lhs.x - rhs.x, lhs.y - rhs.y,
        lhs.z - rhs.z, lhs.w - rhs.w );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline
attr_header struct Vector4 v4_mul( struct Vector4 lhs, f32 rhs ) {
    return v4(
        lhs.x * rhs, lhs.y * rhs,
        lhs.z * rhs, lhs.w * rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline
attr_header struct Vector4 v4_div( struct Vector4 lhs, f32 rhs ) {
    return v4(
        lhs.x / rhs, lhs.y / rhs,
        lhs.z / rhs, lhs.w / rhs );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline
attr_header struct Vector4 v4_neg( struct Vector4 x ) {
    return v4( -x.x, -x.y, -x.z, -x.w );
}
/// @brief Rotate vector components to the left.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the left.
attr_always_inline
attr_header struct Vector4 v4_rotl( struct Vector4 x ) {
    return v4( x.y, x.z, x.w, x.x );
}
/// @brief Rotate vector components to the right.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the right.
attr_always_inline
attr_header struct Vector4 v4_rotr( struct Vector4 x ) {
    return v4( x.w, x.x, x.y, x.z );
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline
attr_header f32 v4_hadd( struct Vector4 x ) {
    return x.x + x.y + x.z + x.w;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline
attr_header f32 v4_hmul( struct Vector4 x ) {
    return x.x * x.y * x.z * x.w;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline
attr_header struct Vector4 v4_hadamard(
    struct Vector4 lhs, struct Vector4 rhs
) {
    return v4(
        lhs.x * rhs.x, lhs.y * rhs.y,
        lhs.z * rhs.z, lhs.w * rhs.w );
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline
attr_header f32 v4_dot( struct Vector4 lhs, struct Vector4 rhs ) {
    return v4_hadd( v4_hadamard( lhs, rhs ) );
}
/// @brief Get the maximum component in vector.
/// @param x Vector to get maximum of.
/// @return Component with largest value.
attr_always_inline
attr_header f32 v4_max( struct Vector4 x ) {
    f32 _0 = v2_max( x.xy );
    f32 _1 = v2_max( v2( x.y, x.z ) );
    return _0 < _1 ? _1 : _0;
}
/// @brief Get the minimum component in vector.
/// @param x Vector to get minimum of.
/// @return Component with smallest value.
attr_always_inline
attr_header f32 v4_min( struct Vector4 x ) {
    f32 _0 = v2_max( x.xy );
    f32 _1 = v2_max( v2( x.y, x.z ) );
    return _0 < _1 ? _0 : _1;
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline
attr_header f32 v4_sqrmag( struct Vector4 x ) {
    return v4_dot( x, x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_core_api f32 v4_mag( struct Vector4 x );
/// @brief Normalize a Vector.
/// @param x Vector to normalize.
/// @return Normalized vector or zero vector if magnitude == 0.
attr_core_api struct Vector4 v4_normalize( struct Vector4 x );
/// @brief Clamp vector's magnitude to range min -> max.
/// @param v Vector to clamp.
/// @param min_, max_ Range to clamp to, min_ must be < max_.
/// @return Clamped vector.
attr_always_inline
attr_header struct Vector4 v4_clamp_mag(
    struct Vector4 v, f32 min_, f32 max_
) {
    f32 mag = v4_mag( v );
    if( mag == 0.0f ) {
        return VEC4_ZERO;
    }
    f32 new_mag = ( mag < min_ ? min_ : ( mag > max_ ? max_ : mag ) );
    return v4_mul( v4_div( v, mag ), new_mag );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_core_api struct Vector4 v4_lerp(
    struct Vector4 a, struct Vector4 b, f32 t );
/// @brief Smooth step interpolation.
/// @param a, b Range of interpolation.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_core_api struct Vector4 v4_smooth_step(
    struct Vector4 a, struct Vector4 b, f32 t );
/// @brief Smoother step interpolation.
/// @param a, b Range of interpolation.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_core_api struct Vector4 v4_smoother_step(
    struct Vector4 a, struct Vector4 b, f32 t );
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if the square magnitude of a - b is < F32_EPSILON.
/// @see #F32_EPSILON
attr_always_inline
attr_header b32 v4_cmp( struct Vector4 a, struct Vector4 b ) {
    return v4_sqrmag( v4_sub( a, b ) ) < F32_EPSILON;
}

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 4 values.
/// @return Vector with components from array.
attr_always_inline
attr_header struct IVector4 iv4_from_array( const i32 array[4] ) {
    return iv4( array[0], array[1], array[2], array[4] );
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 4 values.
attr_always_inline
attr_header void iv4_to_array( struct IVector4 v, i32* out_array ) {
    out_array[0] = v.v[0]; out_array[1] = v.v[1];
    out_array[2] = v.v[2]; out_array[3] = v.v[3];
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline
attr_header struct IVector4 iv4_add(
    struct IVector4 lhs, struct IVector4 rhs
) {
    return iv4(
        lhs.x + rhs.x, lhs.y + rhs.y,
        lhs.z + rhs.z, lhs.w + rhs.w );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline
attr_header struct IVector4 iv4_sub(
    struct IVector4 lhs, struct IVector4 rhs
) {
    return iv4(
        lhs.x - rhs.x, lhs.y - rhs.y,
        lhs.z - rhs.z, lhs.w - rhs.w );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline
attr_header struct IVector4 iv4_mul( struct IVector4 lhs, i32 rhs ) {
    return iv4(
        lhs.x * rhs, lhs.y * rhs,
        lhs.z * rhs, lhs.w * rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline
attr_header struct IVector4 iv4_div( struct IVector4 lhs, i32 rhs ) {
    return iv4(
        lhs.x / rhs, lhs.y / rhs,
        lhs.z / rhs, lhs.w / rhs );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline
attr_header struct IVector4 iv4_neg( struct IVector4 x ) {
    return iv4( -x.x, -x.y, -x.z, -x.w );
}
/// @brief Rotate vector components to the left.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the left.
attr_always_inline
attr_header struct IVector4 iv4_rotl( struct IVector4 x ) {
    return iv4( x.y, x.z, x.w, x.x );
}
/// @brief Rotate vector components to the right.
/// @param x Vector to rotate components of.
/// @return Vector with components rotated to the right.
attr_always_inline
attr_header struct IVector4 iv4_rotr( struct IVector4 x ) {
    return iv4( x.w, x.x, x.y, x.z );
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline
attr_header i32 iv4_hadd( struct IVector4 x ) {
    return x.x + x.y + x.z + x.w;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline
attr_header i32 iv4_hmul( struct IVector4 x ) {
    return x.x * x.y * x.z * x.w;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline
attr_header struct IVector4 iv4_hadamard(
    struct IVector4 lhs, struct IVector4 rhs
) {
    return iv4(
        lhs.x * rhs.x, lhs.y * rhs.y,
        lhs.z * rhs.z, lhs.w * rhs.w );
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline
attr_header f32 iv4_dot( struct IVector4 lhs, struct IVector4 rhs ) {
    struct Vector4 _lhs = v4( (f32)lhs.x, (f32)lhs.y, (f32)lhs.z, (f32)lhs.w );
    struct Vector4 _rhs = v4( (f32)rhs.x, (f32)rhs.y, (f32)rhs.z, (f32)rhs.w );

    return v4_dot( _lhs, _rhs );
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline
attr_header f32 iv4_sqrmag( struct IVector4 x ) {
    struct Vector4 _x = v4( (f32)x.x, (f32)x.y, (f32)x.z, (f32)x.w );
    return v4_sqrmag( _x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_always_inline
attr_header f32 iv4_mag( struct IVector4 x ) {
    struct Vector4 _x = v4( (f32)x.x, (f32)x.y, (f32)x.z, (f32)x.w );
    return v4_mag( _x );
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if vector components are equal.
attr_always_inline
attr_header b32 iv4_cmp( struct IVector4 a, struct IVector4 b ) {
    return
        a.x == b.x &&
        a.y == b.y &&
        a.z == b.z &&
        a.w == b.w;
}

#if defined(CORE_CPLUSPLUS)

// TODO(alicia): DOCUMENT C++

namespace CoreMathInternal {

struct Vector4 : public ::Vector4 {
    Vector4() : ::Vector4{ .v= {0.0f, 0.0f, 0.0f, 0.0f} } {}
    Vector4( f32 x, f32 y, f32 z, f32 w ) : ::Vector4{ .v={ x, y, z, w } } {}
    Vector4( f32 s ) : Vector4( s, s, s, s ) {}
    Vector4( ::Vector4 v ) : Vector4( v.x, v.y, v.z, v.w ) {}

    attr_always_inline
    attr_header f32 operator[]( usize index ) const {
        return v[index];
    }
    attr_always_inline
    attr_header f32& operator[]( usize index ) {
        return v[index];
    }
    attr_always_inline
    attr_header void operator+=( const Vector4 rhs ) {
        *this = v4_add( *this, rhs );
    }
    attr_always_inline
    attr_header void operator-=( Vector4 rhs ) {
        *this = v4_sub( *this, rhs );
    }
    attr_always_inline
    attr_header void operator*=( f32 rhs ) {
        *this = v4_mul( *this, rhs );
    }
    attr_always_inline
    attr_header void operator/=( f32 rhs ) {
        *this = v4_div( *this, rhs );
    }
    attr_always_inline
    attr_header Vector4 operator-() const {
        return v4_neg( *this );
    }
}; /* struct CoreMathInternal::Vector4 */

struct IVector4 : public ::IVector4 {
    IVector4() : ::IVector4{ .v= {0, 0, 0, 0} } {}
    IVector4( i32 x, i32 y, i32 z, i32 w ) : ::IVector4{ .v={ x, y, z, w } } {}
    IVector4( i32 s ) : IVector4( s, s, s, s ) {}
    IVector4( ::IVector4 v ) : IVector4( v.x, v.y, v.z, v.w ) {}

    attr_always_inline
    attr_header i32 operator[]( usize index ) const {
        return v[index];
    }
    attr_always_inline
    attr_header i32& operator[]( usize index ) {
        return v[index];
    }
    attr_always_inline
    attr_header void operator+=( const IVector4 rhs ) {
        *this = iv4_add( *this, rhs );
    }
    attr_always_inline
    attr_header void operator-=( const IVector4 rhs ) {
        *this = iv4_sub( *this, rhs );
    }
    attr_always_inline
    attr_header void operator*=( i32 rhs ) {
        *this = iv4_mul( *this, rhs );
    }
    attr_always_inline
    attr_header void operator/=( i32 rhs ) {
        *this = iv4_div( *this, rhs );
    }
    attr_always_inline
    attr_header IVector4 operator-() const {
        return iv4_neg( *this );
    }
}; /* struct CoreMathInternal::IVector4 */

}; /* namespace CoreMathInternal */

typedef CoreMathInternal::Vector4 vec4;
typedef CoreMathInternal::Vector4 rgba;
typedef CoreMathInternal::IVector4 ivec4;

attr_always_inline
attr_header vec4 operator+( const vec4 lhs, const vec4 rhs ) {
    return v4_add( lhs, rhs );
}
attr_always_inline
attr_header vec4 operator-( const vec4 lhs, const vec4 rhs ) {
    return v4_sub( lhs, rhs );
}
attr_always_inline
attr_header vec4 operator*( const vec4 lhs, f32 rhs ) {
    return v4_mul( lhs, rhs );
}
attr_always_inline
attr_header vec4 operator*( f32 lhs, const vec4 rhs ) {
    return v4_mul( rhs, lhs );
}
attr_always_inline
attr_header vec4 operator/( const vec4 lhs, f32 rhs ) {
    return v4_div( lhs, rhs );
}
attr_always_inline
attr_header vec4 operator-( const vec4 x ) {
    return v4_neg( x );
}
attr_always_inline
attr_header b32 operator==( const vec4 a, const vec4 b ) {
    return v4_cmp( a, b );
}
attr_always_inline
attr_header b32 operator!=( const vec4 a, const vec4 b ) {
    return !(a == b);
}

attr_always_inline
attr_header ivec4 operator+( const ivec4 lhs, const ivec4 rhs ) {
    return iv4_add( lhs, rhs );
}
attr_always_inline
attr_header ivec4 operator-( const ivec4 lhs, const ivec4 rhs ) {
    return iv4_sub( lhs, rhs );
}
attr_always_inline
attr_header ivec4 operator*( const ivec4 lhs, i32 rhs ) {
    return iv4_mul( lhs, rhs );
}
attr_always_inline
attr_header ivec4 operator*( i32 lhs, const ivec4 rhs ) {
    return iv4_mul( rhs, lhs );
}
attr_always_inline
attr_header ivec4 operator/( const ivec4 lhs, i32 rhs ) {
    return iv4_div( lhs, rhs );
}
attr_always_inline
attr_header ivec4 operator-( const ivec4 x ) {
    return iv4_neg( x );
}
attr_always_inline
attr_header b32 operator==( const ivec4 a, const ivec4 b ) {
    return iv4_cmp( a, b );
}
attr_always_inline
attr_header b32 operator!=( const ivec4 a, const ivec4 b ) {
    return !(a == b);
}


#endif /* C++ */

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#endif /* header guard */
