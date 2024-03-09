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
        /// @brief X and Y as an array.
        f32 v[2];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief 2 Component 32-bit Floating Point Vector.
    /// @see struct Vector2
    typedef struct Vector2 vec2;
#endif

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
        i32 v[2];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief 2 Component 32-bit Signed Integer Vector.
    /// @see struct IVector2
    typedef struct IVector2 ivec2;
#endif

#if defined(CORE_DOXYGEN)
    /// @brief Construct a Vector2.
    /// @param x, y (f32) Components.
    /// @return Vector2.
    #define v2( x, y )
    /// @brief Construct an IVector2.
    /// @param x, y (i32) Components.
    /// @return IVector2.
    #define iv2( x, y )
#else

#if defined(CORE_CPLUSPLUS)
    #define v2( x, y )\
        Vector2{ .v={ (x), (y) } }
    #define iv2( x, y )\
        IVector2{ .v={ (x), (y) } }

#else /* C++ constructors */
    #define v2( x, y )\
        (struct Vector2){ .v={ (x), (y) } }
    #define iv2( x, y )\
        (struct IVector2){ .v={ (x), (y) } }
#endif /* C constructors */

#endif /* Doxygen */

/// @brief Contruct a new Vector2 with identical components.
/// @param x (f32) Value for components.
/// @return Vector2.
#define v2_set( x )\
    v2( (x), (x) )
/// @brief Contruct a new IVector2 with identical components.
/// @param x (i32) Value for components.
/// @return IVector2.
#define iv2_set( x )\
    iv2( (x), (x) )

/// @brief Vector2 zero constant.
#define VEC2_ZERO  ( v2_set( 0.0f ) )
/// @brief Vector2 one constant.
#define VEC2_ONE   ( v2_set( 1.0f ) )
/// @brief Vector2 left constant.
#define VEC2_LEFT  ( v2( -1.0f,  0.0f ) )
/// @brief Vector2 right constant.
#define VEC2_RIGHT ( v2(  1.0f,  0.0f ) )
/// @brief Vector2 up constant.
#define VEC2_UP    ( v2(  0.0f,  1.0f ) )
/// @brief Vector2 down constant.
#define VEC2_DOWN  ( v2(  0.0f, -1.0f ) )

/// @brief IVector2 zero constant.
#define IVEC2_ZERO  ( iv2_set(0) )
/// @brief IVector2 one constant.
#define IVEC2_ONE   ( iv2_set(1) )
/// @brief IVector2 left constant.
#define IVEC2_LEFT  ( iv2( -1,  0 ) )
/// @brief IVector2 right constant.
#define IVEC2_RIGHT ( iv2(  1,  0 ) )
/// @brief IVector2 up constant.
#define IVEC2_UP    ( iv2(  0,  1 ) )
/// @brief IVector2 down constant.
#define IVEC2_DOWN  ( iv2(  0, -1 ) )

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 2 values.
/// @return Vector with components from array.
attr_always_inline
attr_header struct Vector2 v2_from_array( const f32 array[2] ) {
    return v2( array[0], array[1] );
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 2 values.
attr_always_inline
attr_header void v2_to_array( struct Vector2 v, f32* out_array ) {
    out_array[0] = v.x; out_array[1] = v.y;
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline
attr_header struct Vector2 v2_add(
    struct Vector2 lhs, struct Vector2 rhs
) {
    return v2( lhs.x + rhs.x, lhs.y + rhs.y );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline
attr_header struct Vector2 v2_sub(
    struct Vector2 lhs, struct Vector2 rhs
) {
    return v2( lhs.x - rhs.x, lhs.y - rhs.y );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline
attr_header struct Vector2 v2_mul( struct Vector2 lhs, f32 rhs ) {
    return v2( lhs.x * rhs, lhs.y * rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline
attr_header struct Vector2 v2_div( struct Vector2 lhs, f32 rhs ) {
    return v2( lhs.x / rhs, lhs.y / rhs );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline
attr_header struct Vector2 v2_neg( struct Vector2 x ) {
    return v2( -x.x, -x.y );
}
/// @brief Swap X and Y components.
/// @param x Vector whose components are to be swapped.
/// @return Vector with swapped components.
attr_always_inline
attr_header struct Vector2 v2_swap( struct Vector2 x ) {
    return v2( x.y, x.x );
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline
attr_header f32 v2_hadd( struct Vector2 x ) {
    return x.x + x.y;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline
attr_header f32 v2_hmul( struct Vector2 x ) {
    return x.x * x.y;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline
attr_header struct Vector2 v2_hadamard(
    struct Vector2 lhs, struct Vector2 rhs
) {
    return v2( lhs.x * rhs.x, lhs.y * rhs.y );
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline
attr_header f32 v2_dot(
    struct Vector2 lhs, struct Vector2 rhs
) {
    return v2_hadd( v2_hadamard( lhs, rhs ) );
}
/// @brief Calculate aspect ratio of X and Y components.
/// @param x Vector to calculate aspect ratio of.
/// @return Aspect ratio between X and Y components.
attr_always_inline
attr_header f32 v2_aspect_ratio( struct Vector2 x ) {
    return x.x / x.y;
}
/// @brief Get the maximum component in vector.
/// @param x Vector to get maximum of.
/// @return Component with largest value.
attr_always_inline
attr_header f32 v2_max( struct Vector2 x ) {
    return x.x < x.y ? x.y : x.x;
}
/// @brief Get the minimum component in vector.
/// @param x Vector to get minimum of.
/// @return Component with smallest value.
attr_always_inline
attr_header f32 v2_min( struct Vector2 x ) {
    return x.x < x.y ? x.x : x.y;
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline
attr_header f32 v2_sqrmag( struct Vector2 x ) {
    return v2_dot( x, x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_core_api f32 v2_mag( struct Vector2 x );
/// @brief Normalize a Vector.
/// @param x Vector to normalize.
/// @return Normalized vector or zero vector if magnitude == 0.
attr_core_api struct Vector2 v2_normalize( struct Vector2 x );
/// @brief Rotate vector by given angle.
/// @param v Vector to rotate.
/// @param angle Angle to rotate by (in radians).
/// @return Rotated vector.
attr_core_api struct Vector2 v2_rotate( struct Vector2 v, f32 angle );
/// @brief Clamp vector's magnitude to range min -> max.
/// @param v Vector to clamp.
/// @param min_, max_ Range to clamp to, min_ must be < max_.
/// @return Clamped vector.
attr_always_inline
attr_header struct Vector2 v2_clamp_mag(
    const struct Vector2 v, f32 min_, f32 max_
) {
    f32 mag = v2_mag( v );
    if( mag == 0.0f ) {
        return VEC2_ZERO;
    }
    f32 new_mag = ( mag < min_ ? min_ : ( mag > max_ ? max_ : mag ) );
    return v2_mul( v2_div( v, mag ), new_mag );
}
/// @brief Calculate the angle between two vectors.
/// @param a, b Vectors to calculate angle of.
/// @return Angle (in radians) between vectors.
attr_always_inline
attr_header f32 v2_angle( struct Vector2 a, struct Vector2 b ) {
    extern f32 arc_cosine( f32 x );
    return arc_cosine( v2_dot( a, b ) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_core_api struct Vector2 v2_lerp(
    struct Vector2 a, struct Vector2 b, f32 t );
/// @brief Smooth step interpolation.
/// @param a, b Range of interpolation.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_core_api struct Vector2 v2_smooth_step(
    struct Vector2 a, struct Vector2 b, f32 t );
/// @brief Smoother step interpolation.
/// @param a, b Range of interpolation.
/// @param t Where to interpolate to.
/// @return Vector in range a -> b.
attr_core_api struct Vector2 v2_smoother_step(
    struct Vector2 a, struct Vector2 b, f32 t );
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if the square magnitude of a - b is < F32_EPSILON.
/// @see #F32_EPSILON
attr_always_inline
attr_header b32 v2_cmp( struct Vector2 a, struct Vector2 b ) {
    return v2_sqrmag( v2_sub( a, b ) ) < F32_EPSILON;
}

/// @brief Create vector from array.
/// @param[in] array Array, must have at least 2 values.
/// @return Vector with components from array.
attr_always_inline
attr_header struct IVector2 iv2_from_array( const i32 array[2] ) {
    return iv2( array[0], array[1] );
}
/// @brief Fill array with components from vector.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 2 values.
attr_always_inline
attr_header void iv2_to_array( struct IVector2 v, i32* out_array ) {
    out_array[0] = v.x; out_array[1] = v.y;
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline
attr_header struct IVector2 iv2_add(
    struct IVector2 lhs, struct IVector2 rhs
) {
    return iv2( lhs.x + rhs.x, lhs.y + rhs.y );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline
attr_header struct IVector2 iv2_sub(
    struct IVector2 lhs, struct IVector2 rhs
) {
    return iv2( lhs.x - rhs.x, lhs.y - rhs.y );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline
attr_header struct IVector2 iv2_mul( struct IVector2 lhs, i32 rhs ) {
    return iv2( lhs.x * rhs, lhs.y * rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline
attr_header struct IVector2 iv2_div( struct IVector2 lhs, i32 rhs ) {
    return iv2( lhs.x / rhs, lhs.y / rhs );
}
/// @brief Negate components of a Vector2.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline
attr_header struct IVector2 iv2_neg( struct IVector2 x ) {
    return iv2( -x.x, -x.y );
}
/// @brief Swap X and Y components.
/// @param x Vector whose components are to be swapped.
/// @return Vector with swapped components.
attr_always_inline
attr_header struct IVector2 iv2_swap( struct IVector2 x ) {
    return iv2( x.y, x.x );
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline
attr_header i32 iv2_hadd( struct IVector2 x ) {
    return x.x + x.y;
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline
attr_header i32 iv2_hmul( struct IVector2 x ) {
    return x.x * x.y;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
attr_always_inline
attr_header struct IVector2 iv2_hadamard(
    struct IVector2 lhs, struct IVector2 rhs
) {
    return iv2( lhs.x * rhs.x, lhs.y * rhs.y );
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline
attr_header f32 iv2_dot(
    struct IVector2 lhs, struct IVector2 rhs
) {
    struct Vector2 _lhs = v2( (f32)lhs.x, (f32)lhs.y );
    struct Vector2 _rhs = v2( (f32)rhs.x, (f32)rhs.y );

    return v2_dot( _lhs, _rhs );
}
/// @brief Calculate aspect ratio of X and Y components.
/// @param x Vector to calculate aspect ratio of.
/// @return Aspect ratio between X and Y components.
attr_always_inline
attr_header f32 iv2_aspect_ratio( struct IVector2 x ) {
    return (f32)x.x / (f32)x.y;
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline
attr_header f32 iv2_sqrmag( struct IVector2 x ) {
    struct Vector2 _x = v2( (f32)x.x, (f32)x.y );
    return v2_sqrmag( _x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_always_inline
attr_header f32 iv2_mag( struct IVector2 x ) {
    struct Vector2 _x = v2( (f32)x.x, (f32)x.y );
    return v2_mag( _x );
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if components in vectors are equal.
attr_always_inline
attr_header b32 iv2_cmp( struct IVector2 a, struct IVector2 b ) {
    return
        a.x == b.x &&
        a.y == b.y;
}

#if defined(CORE_CPLUSPLUS)

namespace CoreMathInternal {

/// @brief 2 Component 32-bit Floating Point Vector.
/// @see ::Vector2
struct Vector2 : public ::Vector2 {
    /// @brief Construct empty Vector2.
    Vector2() : ::Vector2{ .v= {0.0f, 0.0f} } {}
    /// @brief Construct Vector2.
    /// @param x, y Components.
    Vector2( f32 x, f32 y ) : ::Vector2{ .v={ x, y } } {}
    /// @brief Construct Vector2.
    /// @param s Scalar to copy to both X and Y.
    Vector2( f32 s ) : Vector2( s, s ) {}
    /// @brief Convert POD Vector2 to Vector2.
    /// @param v POD Vector2.
    Vector2( ::Vector2 v ) : Vector2( v.x, v.y ) {}

    /// @brief Index into Vector2's components.
    /// @param index Index of component to get, must be <= 1.
    /// @return Value of component at given index.
    attr_always_inline
    attr_header f32 operator[]( usize index ) const {
        return v[index];
    }
    /// @brief Index into Vector2's components.
    /// @param index Index of component to get, must be <= 1.
    /// @return Reference to value of component at given index.
    attr_always_inline
    attr_header f32& operator[]( usize index ) {
        return v[index];
    }
    /// @brief Add to Vector2.
    /// @param rhs Right hand side.
    attr_always_inline
    attr_header void operator+=( const Vector2 rhs ) {
        *this = v2_add( *this, rhs );
    }
    /// @brief Sub from Vector2.
    /// @param rhs Right hand side.
    attr_always_inline
    attr_header void operator-=( const Vector2 rhs ) {
        *this = v2_sub( *this, rhs );
    }
    /// @brief Multiply Vector2.
    /// @param rhs Right hand side.
    attr_always_inline
    attr_header void operator*=( f32 rhs ) {
        *this = v2_mul( *this, rhs );
    }
    /// @brief Divide Vector2.
    /// @param rhs Right hand side.
    attr_always_inline
    attr_header void operator/=( f32 rhs ) {
        *this = v2_div( *this, rhs );
    }
    /// @brief Negate Vector2.
    /// @return Negated Vector2.
    attr_always_inline
    attr_header Vector2 operator-() const {
        return v2_neg( *this );
    }

}; /* struct CoreMathInternal::Vector2 */

/// @brief 2 Component 32-bit Signed Integer Vector.
/// @see ::IVector2
struct IVector2 : public ::IVector2 {
    /// @brief Construct empty IVector2.
    IVector2() : ::IVector2{ .v= {0, 0} } {}
    /// @brief Construct IVector2.
    /// @param x, y Components.
    IVector2( i32 x, i32 y ) : ::IVector2{ .v={ x, y } } {}
    /// @brief Construct IVector2.
    /// @param s Scalar to copy to both X and Y.
    IVector2( i32 s ) : IVector2( s, s ) {}
    /// @brief Convert POD IVector2 to IVector2.
    /// @param v POD IVector2.
    IVector2( ::IVector2 v ) : IVector2( v.x, v.y ) {}

    /// @brief Index into IVector2's components.
    /// @param index Index of component to get, must be <= 1.
    /// @return Value of component at given index.
    attr_always_inline
    attr_header i32 operator[]( usize index ) const {
        return v[index];
    }
    /// @brief Index into IVector2's components.
    /// @param index Index of component to get, must be <= 1.
    /// @return Reference to value of component at given index.
    attr_always_inline
    attr_header i32& operator[]( usize index ) {
        return v[index];
    }
    /// @brief Add to IVector2.
    /// @param rhs Right hand side.
    attr_always_inline
    attr_header void operator+=( const IVector2 rhs ) {
        *this = iv2_add( *this, rhs );
    }
    /// @brief Sub from IVector2.
    /// @param rhs Right hand side.
    attr_always_inline
    attr_header void operator-=( const IVector2 rhs ) {
        *this = iv2_sub( *this, rhs );
    }
    /// @brief Multiply IVector2.
    /// @param rhs Right hand side.
    attr_always_inline
    attr_header void operator*=( i32 rhs ) {
        *this = iv2_mul( *this, rhs );
    }
    /// @brief Divide IVector2.
    /// @param rhs Right hand side.
    attr_always_inline
    attr_header void operator/=( i32 rhs ) {
        *this = iv2_div( *this, rhs );
    }
    /// @brief Negate IVector2.
    /// @return Negated components.
    attr_always_inline
    attr_header IVector2 operator-() const {
        return iv2_neg( *this );
    }

}; /* struct CoreMathInternal::IVector2 */

}; /* namespace CoreMathInternal */

/// @brief 2 Component 32-bit Floating Point Vector.
/// @see ::Vector2
typedef CoreMathInternal::Vector2 vec2;
/// @brief 2 Component 32-bit Signed Integer Vector.
/// @see ::IVector2
typedef CoreMathInternal::IVector2 ivec2;

/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline
attr_header vec2 operator+( vec2 lhs, vec2 rhs ) {
    return v2_add( lhs, rhs );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline
attr_header vec2 operator-( vec2 lhs, vec2 rhs ) {
    return v2_sub( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline
attr_header vec2 operator*( vec2 lhs, f32 rhs ) {
    return v2_mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Scalar to multiply components by.
/// @param rhs Vector to multiply.
/// @return Result of multiplication.
attr_always_inline
attr_header vec2 operator*( f32 lhs, vec2 rhs ) {
    return v2_mul( rhs, lhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline
attr_header vec2 operator/( vec2 lhs, f32 rhs ) {
    return v2_div( lhs, rhs );
}
/// @brief Negate components of a Vector2.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline
attr_header vec2 operator-( vec2 x ) {
    return v2_neg( x );
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if the square magnitude of a - b is < F32_EPSILON.
/// @see #F32_EPSILON
attr_always_inline
attr_header b32 operator==( vec2 a, vec2 b ) {
    return v2_cmp( a, b );
}
/// @brief Compare two vectors for inequality.
/// @param a, b Vectors to compare.
/// @return False if the square magnitude of a - b is < F32_EPSILON.
/// @see #F32_EPSILON
attr_always_inline
attr_header b32 operator!=( vec2 a, vec2 b ) {
    return !(a == b);
}

/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
attr_always_inline
attr_header ivec2 operator+( ivec2 lhs, ivec2 rhs ) {
    return iv2_add( lhs, rhs );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
attr_always_inline
attr_header ivec2 operator-( ivec2 lhs, ivec2 rhs ) {
    return iv2_sub( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline
attr_header ivec2 operator*( ivec2 lhs, i32 rhs ) {
    return iv2_mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Scalar to multiply components by.
/// @param rhs Vector to multiply.
/// @return Result of multiplication.
attr_always_inline
attr_header ivec2 operator*( i32 lhs, ivec2 rhs ) {
    return iv2_mul( rhs, lhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline
attr_header ivec2 operator/( ivec2 lhs, i32 rhs ) {
    return iv2_div( lhs, rhs );
}
/// @brief Negate components of a Vector2.
/// @param x Vector to negate.
/// @return Result of negation.
attr_always_inline
attr_header ivec2 operator-( ivec2 x ) {
    return iv2_neg( x );
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if components in vectors are equal.
attr_always_inline
attr_header b32 operator==( ivec2 a, ivec2 b ) {
    return iv2_cmp( a, b );
}
/// @brief Compare two vectors for inequality.
/// @param a, b Vectors to compare.
/// @return True if components in vectors are not equal.
attr_always_inline
attr_header b32 operator!=( ivec2 a, ivec2 b ) {
    return !(a == b);
}

#endif /* C++ */

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#endif /* header guard */
