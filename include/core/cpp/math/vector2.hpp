#if !defined(CORE_CPP_MATH_VECTOR2_HPP) && defined(__cplusplus)
#define CORE_CPP_MATH_VECTOR2_HPP
/**
 * @file   vector2.hpp
 * @brief  C++ Math: Vector2.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 29, 2025
*/
#include "core/cpp/math/swizzler.hpp"

#if !defined(CORE_MATH_VECTOR2_H)
    #include "core/math/vector2.h"
#endif

/// @brief 2 Component 32-bit Unsigned Integer Vector.
typedef UVector2 uvec2;
/// @brief 2 Component 64-bit Floating Point Vector.
typedef DVector2 dvec2;

/// @brief 2 Component 32-bit Floating Point Vector.
struct Vector2CPP {
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
        /// @brief X and Y components as width and height.
        struct {
            /// @brief X component -- width.
            f32 w;
            /// @brief Y component -- height.
            f32 h;
        };
        /// @brief X and Y components as UV coordinates.
        struct {
            /// @brief X component -- U component.
            f32 u;
            /// @brief X component -- V component.
            f32 v;
        };
        Vector2 xy;
        Vector2 rg;
        Vector2 uv;

        /// @brief XX swizzle.
        Swizzler<Vector2CPP, f32, 0, 0> xx, uu, ww, rr;
        /// @brief YY swizzle.
        Swizzler<Vector2CPP, f32, 1, 1> yy, vv, hh, gg;
        /// @brief YX swizzle.
        Swizzler<Vector2CPP, f32, 1, 0> yx, vu, hw, gr;

        /// @brief X and Y components as an array.
        f32 array[2];
    };

    /// @brief Create empty vector.
    constexpr Vector2CPP();
    /// @brief Implicitly convert POD vector to C++ vector.
    constexpr Vector2CPP( const Vector2& __pod );
    /// @brief Create vector.
    /// @param x X component.
    /// @param y Y component.
    constexpr explicit Vector2CPP( f32 x, f32 y );
    /// @brief Create vector.
    /// @param x X and Y components.
    constexpr explicit Vector2CPP( f32 x );
    /// @brief Convert integer vector to float vector.
    constexpr explicit Vector2CPP( const IVector2& iv );
    /// @brief Convert integer vector to float vector.
    constexpr explicit Vector2CPP( const BVector2& bv );

    /// @brief Implicitly convert C++ vector to POD vector.
    constexpr operator Vector2() const;
    /// @brief Index into vector.
    /// @param index Index of component.
    /// @return Component.
    constexpr f32 operator[]( usize index ) const;
    /// @brief Index into vector.
    /// @param index Index of component.
    /// @return Component.
    constexpr f32& operator[]( usize index );

    /// @brief Vector zero constant.
    /// @return Value.
    static constexpr Vector2CPP zero();
    /// @brief Vector one constant.
    /// @return Value.
    static constexpr Vector2CPP one();
    /// @brief Vector up constant.
    /// @return Value.
    static constexpr Vector2CPP up();
    /// @brief Vector right constant.
    /// @return Value.
    static constexpr Vector2CPP right();
    /// @brief Vector down constant.
    /// @return Value.
    static constexpr Vector2CPP down();
    /// @brief Vector left constant.
    /// @return Value.
    static constexpr Vector2CPP left();
};
/// @brief 2 Component 32-bit Integer Vector.
struct IVector2CPP {
    /// @brief Union of vector component aliases.
    union {
        /// @brief X and Y components.
        struct {
            /// @brief X component.
            i32 x;
            /// @brief Y component.
            i32 y;
        };
        /// @brief X and Y components as width and height.
        struct {
            /// @brief X component -- width.
            i32 w;
            /// @brief Y component -- height.
            i32 h;
        };
        IVector2 xy;
        IVector2 wh;

        /// @brief XX swizzle.
        Swizzler<IVector2CPP, i32, 0, 0> xx, ww;
        /// @brief YY swizzle.
        Swizzler<IVector2CPP, i32, 1, 1> yy, hh;
        /// @brief YX swizzle.
        Swizzler<IVector2CPP, i32, 1, 0> yx, hw;

        /// @brief X and Y components as an array.
        i32 array[2];
    };

    /// @brief Create empty vector.
    constexpr IVector2CPP();
    /// @brief Implicitly convert POD vector to C++ vector.
    constexpr IVector2CPP( const IVector2& __pod );
    /// @brief Create vector.
    /// @param x X component.
    /// @param y Y component.
    constexpr explicit IVector2CPP( i32 x, i32 y );
    /// @brief Create vector.
    /// @param x X and Y components.
    constexpr explicit IVector2CPP( i32 x );
    /// @brief Convert float vector to integer vector.
    constexpr explicit IVector2CPP( const Vector2& v );
    /// @brief Convert boolean vector to integer vector.
    constexpr explicit IVector2CPP( const BVector2& bv );

    /// @brief Implicitly convert C++ vector to POD vector.
    constexpr operator IVector2() const;
    /// @brief Index into vector.
    /// @param index Index of component.
    /// @return Component.
    constexpr i32  operator[]( usize index ) const;
    /// @brief Index into vector.
    /// @param index Index of component.
    /// @return Component.
    constexpr i32& operator[]( usize index );

    /// @brief Vector zero constant.
    /// @return Value.
    static constexpr IVector2CPP zero();
    /// @brief Vector one constant.
    /// @return Value.
    static constexpr IVector2CPP one();
    /// @brief Vector up constant.
    /// @return Value.
    static constexpr IVector2CPP up();
    /// @brief Vector right constant.
    /// @return Value.
    static constexpr IVector2CPP right();
    /// @brief Vector down constant.
    /// @return Value.
    static constexpr IVector2CPP down();
    /// @brief Vector left constant.
    /// @return Value.
    static constexpr IVector2CPP left();
};
/// @brief 2 Component 32-bit Boolean Vector.
struct BVector2CPP {
    /// @brief Union of vector component aliases.
    union {
        /// @brief X and Y components.
        struct {
            /// @brief X component.
            b32 x;
            /// @brief Y component.
            b32 y;
        };
        BVector2 xy;

        /// @brief XX swizzle.
        Swizzler<BVector2CPP, b32, 0, 0> xx;
        /// @brief YY swizzle.
        Swizzler<BVector2CPP, b32, 1, 1> yy;
        /// @brief YX swizzle.
        Swizzler<BVector2CPP, b32, 1, 0> yx;

        /// @brief X and Y components as an array.
        b32 array[2];
    };

    /// @brief Create empty vector.
    constexpr BVector2CPP();
    /// @brief Implicitly convert POD vector to C++ vector.
    constexpr BVector2CPP( const BVector2& __pod );
    /// @brief Create vector.
    /// @param x X component.
    /// @param y Y component.
    constexpr explicit BVector2CPP( b32 x, b32 y );
    /// @brief Create vector.
    /// @param x X and Y components.
    constexpr explicit BVector2CPP( b32 x );
    /// @brief Convert float vector to boolean vector.
    constexpr explicit BVector2CPP( const Vector2& v );
    /// @brief Convert integer vector to boolean vector.
    constexpr explicit BVector2CPP( const IVector2& iv );

    /// @brief Implicitly convert C++ vector to POD vector.
    constexpr operator BVector2() const;
    /// @brief Index into vector.
    /// @param index Index of component.
    /// @return Component.
    constexpr b32  operator[]( usize index ) const;
    /// @brief Index into vector.
    /// @param index Index of component.
    /// @return Component.
    constexpr b32& operator[]( usize index );

    /// @brief Vector zero constant.
    /// @return Value.
    static constexpr BVector2CPP zero();
    /// @brief Vector one constant.
    /// @return Value.
    static constexpr BVector2CPP one();
};

/// @brief 2 Component 32-bit Floating Point Vector.
typedef Vector2CPP   vec2;
/// @brief 2 Component 32-bit Integer Vector.
typedef IVector2CPP ivec2;
/// @brief 2 Component 32-bit Boolean Vector.
typedef BVector2CPP bvec2;

constexpr attr_always_inline attr_header attr_hot
Vector2CPP::Vector2CPP()
    : x(0.0f), y(0.0f) {}
constexpr attr_always_inline attr_header attr_hot
Vector2CPP::Vector2CPP( const Vector2& __pod )
    : x(__pod.x), y(__pod.y) {}
constexpr attr_always_inline attr_header attr_hot
Vector2CPP::Vector2CPP( f32 x, f32 y )
    : x(x), y(y) {}
constexpr attr_always_inline attr_header attr_hot
Vector2CPP::Vector2CPP( f32 x )
    : x(x), y(x) {}
constexpr attr_always_inline attr_header attr_hot
Vector2CPP::Vector2CPP( const IVector2& iv )
    : x(iv.x), y(iv.y) {}
constexpr attr_always_inline attr_header attr_hot
Vector2CPP::Vector2CPP( const BVector2& bv )
    : x(bv.x ? 1.0f : 0.0f), y(bv.y ? 1.0f : 0.0f) {}

constexpr attr_always_inline attr_header attr_hot
Vector2CPP::operator Vector2() const {
    return *(Vector2*)this;
}
constexpr attr_always_inline attr_header attr_hot
f32 Vector2CPP::operator[]( usize index ) const {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
f32& Vector2CPP::operator[]( usize index ) {
    return this->array[index];
}

constexpr attr_always_inline attr_header attr_hot
Vector2CPP Vector2CPP::zero() {
    return VEC2_ZERO;
}
constexpr attr_always_inline attr_header attr_hot
Vector2CPP Vector2CPP::one() {
    return VEC2_ONE;
}
constexpr attr_always_inline attr_header attr_hot
Vector2CPP Vector2CPP::up() {
    return VEC2_UP;
}
constexpr attr_always_inline attr_header attr_hot
Vector2CPP Vector2CPP::right() {
    return VEC2_RIGHT;
}
constexpr attr_always_inline attr_header attr_hot
Vector2CPP Vector2CPP::down() {
    return VEC2_DOWN;
}
constexpr attr_always_inline attr_header attr_hot
Vector2CPP Vector2CPP::left() {
    return VEC2_LEFT;
}

/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
constexpr attr_always_inline attr_header attr_hot
vec2 add( vec2 lhs, vec2 rhs ) {
    return vec2_add( lhs, rhs );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
constexpr attr_always_inline attr_header attr_hot
vec2 sub( vec2 lhs, vec2 rhs ) {
    return vec2_sub( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec2 mul( vec2 lhs, f32 rhs ) {
    return vec2_mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Scalar to multiply components by.
/// @param rhs Vector to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec2 mul( f32 lhs, vec2 rhs ) {
    return vec2_mul( rhs, lhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec2 mul( vec2 lhs, vec2 rhs ) {
    return vec2_mul_vec2( lhs, rhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec2 hadamard( vec2 lhs, vec2 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec2 div( vec2 lhs, f32 rhs ) {
    return vec2_div( lhs, rhs );
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec2 div( vec2 lhs, vec2 rhs ) {
    return vec2_div_vec2( lhs, rhs );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of modulus division.
constexpr attr_always_inline attr_header attr_hot
vec2 mod( vec2 lhs, f32 rhs ) {
    return vec2_mod( lhs, rhs );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of modulus division.
constexpr attr_always_inline attr_header attr_hot
vec2 mod( vec2 lhs, vec2 rhs ) {
    return vec2_mod_vec2( lhs, rhs );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
constexpr attr_always_inline attr_header attr_hot
vec2 neg( vec2 x ) {
    return vec2_neg( x );
}

/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
constexpr attr_always_inline attr_header attr_hot
vec2 operator+( vec2 lhs, vec2 rhs ) {
    return add( lhs, rhs );
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
constexpr attr_always_inline attr_header attr_hot
vec2& operator+=( vec2& lhs, vec2 rhs ) {
    return lhs = lhs + rhs;
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
constexpr attr_always_inline attr_header attr_hot
vec2 operator-( vec2 lhs, vec2 rhs ) {
    return sub( lhs, rhs );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
constexpr attr_always_inline attr_header attr_hot
vec2& operator-=( vec2& lhs, vec2 rhs ) {
    return lhs = lhs - rhs;
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec2 operator*( vec2 lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Scalar to multiply components by.
/// @param rhs Vector to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec2 operator*( f32 lhs, vec2 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec2 operator*( vec2 lhs, vec2 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec2& operator*=( vec2& lhs, f32 rhs ) {
    return lhs = lhs * rhs;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec2& operator*=( vec2& lhs, vec2 rhs ) {
    return lhs = lhs * rhs;
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec2 operator/( vec2 lhs, f32 rhs ) {
    return div( lhs, rhs );
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec2 operator/( vec2 lhs, vec2 rhs ) {
    return div( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec2& operator/=( vec2& lhs, f32 rhs ) {
    return lhs = lhs * rhs;
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec2& operator/=( vec2& lhs, vec2 rhs ) {
    return lhs = lhs * rhs;
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
constexpr attr_always_inline attr_header attr_hot
vec2 operator-( vec2 x ) {
    return neg(x);
}

/// @brief Swap X and Y components.
/// @param x Vector whose components are to be swapped.
/// @return Vector with swapped components.
attr_always_inline attr_header attr_hot
vec2 swap( vec2 x ) {
    return x.yx;
}
/// @brief Horizontally add components of vector.
/// @param x Vector whose components are to be added.
/// @return Result of addition.
attr_always_inline attr_header attr_hot
f32 hadd( vec2 x ) {
    return vec2_hadd( x );
}
/// @brief Horizontally multply components of vector.
/// @param x Vector whose components are to be multiplied.
/// @return Result of multiplication.
attr_always_inline attr_header attr_hot
f32 hmul( vec2 x ) {
    return vec2_hmul( x );
}
/// @brief Inner product of two vectors.
/// @param lhs, rhs Vectors to take inner product of.
/// @return Inner product.
attr_always_inline attr_header attr_hot
f32 dot( vec2 lhs, vec2 rhs ) {
    return vec2_dot( lhs, rhs );
}
/// @brief Calculate aspect ratio of X and Y components.
/// @param x Vector to calculate aspect ratio of.
/// @return Aspect ratio between X and Y components.
attr_always_inline attr_header attr_hot
f32 aspect_ratio( vec2 x ) {
    return vec2_aspect_ratio( x );
}
/// @brief Get the minimum component in vector.
/// @param x Vector to get minimum of.
/// @return Component with smallest value.
attr_always_inline attr_header attr_hot
f32 min( vec2 x ) {
    return vec2_min( x );
}
/// @brief Component-wise minimum value.
/// @param x, y Vectors.
/// @return Vector with minimum value in components.
attr_always_inline attr_header attr_hot
vec2 min( vec2 x, vec2 y ) {
    return vec2_min_vec2( x, y );
}
/// @brief Get the maximum component in vector.
/// @param x Vector to get maximum of.
/// @return Component with largest value.
attr_always_inline attr_header attr_hot
f32 max( vec2 x ) {
    return vec2_max( x );
}
/// @brief Component-wise maximum value.
/// @param x, y Vectors.
/// @return Vector with maximum value in components.
attr_always_inline attr_header attr_hot
vec2 max( vec2 x, vec2 y ) {
    return vec2_max_vec2( x, y );
}
/// @brief Component-wise clamp.
/// @param v        Vector.
/// @param min, max Range.
/// @return Clamped vector.
attr_always_inline attr_header attr_hot
vec2 clamp( vec2 v, vec2 min, vec2 max ) {
    return vec2_clamp( v, min, max );
}
/// @brief Clamp vector's magnitude to range [min,max].
/// @param v Vector to clamp.
/// @param min, max Range to clamp to, min must be < max.
/// @return Clamped vector.
attr_always_inline attr_header attr_hot
vec2 clamp( vec2 v, f32 min, f32 max ) {
    return vec2_clamp_length( v, min, max );
}
/// @brief Calculate the square magnitude of Vector.
/// @param x Vector to get square magnitude of.
/// @return Square magnitude.
attr_always_inline attr_header attr_hot
f32 length_sqr( vec2 x ) {
    return vec2_length_sqr( x );
}
/// @brief Calculate magnitude of Vector.
/// @param x Vector to get magnitude of.
/// @return Magnitude.
attr_always_inline attr_header attr_hot
f32 length( vec2 x ) {
    return vec2_length( x );
}
/// @brief Calculate distance between two points.
/// @param a, b Points to calculate distance of.
/// @return Distance squared.
attr_always_inline attr_header attr_hot
f32 distance_sqr( vec2 a, vec2 b ) {
    return vec2_distance_sqr( a, b );
}
/// @brief Calculate distance between two points.
/// @param a, b Points to calculate distance of.
/// @return Distance.
attr_always_inline attr_header attr_hot
f32 distance( vec2 a, vec2 b ) {
    return vec2_distance( a, b );
}
/// @brief Normalize a Vector.
/// @param x Vector to normalize.
/// @return Normalized vector or zero vector if magnitude == 0.
attr_always_inline attr_header attr_hot
vec2 normalize( vec2 x ) {
    return vec2_normalize( x );
}
/// @brief Reflect direction vector off surface.
/// @param d Direction vector to reflect.
/// @param n Normal of the surface to reflect off of.
/// @return Reflected vector.
attr_always_inline attr_header attr_hot
vec2 reflect( vec2 d, vec2 n ) {
    return vec2_reflect( d, n );
}
/// @brief Rotate vector by given angle.
/// @param v     Vector to rotate.
/// @param angle Angle to rotate by (in radians).
/// @return Rotated vector.
attr_always_inline attr_header attr_hot
vec2 rotate( vec2 v, f32 angle ) {
    return vec2_rotate( v, angle );
}
/// @brief Calculate the angle between two vectors.
/// @param a, b Vectors to calculate angle of.
/// @return Angle (in radians) between vectors.
attr_always_inline attr_header attr_hot
f32 angle( vec2 a, vec2 b ) {
    return vec2_angle( a, b );
}
/// @brief Component-wise abs.
/// @param v Vector.
/// @return Vector with absolute values.
attr_always_inline attr_header attr_hot
vec2 abs( vec2 v ) {
    return vec2_abs( v );
}
/// @brief Component-wise sign.
/// @param v Vector.
/// @return Vector with sign values.
attr_always_inline attr_header attr_hot
vec2 sign( vec2 v ) {
    return vec2_sign( v );
}
/// @brief Component-wise truncate.
/// @param v Vector to truncate.
/// @return Vector with truncated values.
attr_always_inline attr_header attr_hot
vec2 trunc( vec2 v ) {
    return vec2_trunc( v );
}
/// @brief Component-wise floor.
/// @param v Vector to floor.
/// @return Vector with floored values.
attr_always_inline attr_header attr_hot
vec2 floor( vec2 v ) {
    return vec2_floor( v );
}
/// @brief Component-wise ceil.
/// @param v Vector to ceil.
/// @return Vector with ceiled values.
attr_always_inline attr_header attr_hot
vec2 ceil( vec2 v ) {
    return vec2_ceil( v );
}
/// @brief Component-wise round.
/// @param v Vector to round.
/// @return Vector with rounded values.
attr_always_inline attr_header attr_hot
vec2 round( vec2 v ) {
    return vec2_round( v );
}
/// @brief Get fractional part.
/// @param v Vector.
/// @return Vector with fractional part.
attr_always_inline attr_header attr_hot
vec2 fract( vec2 v ) {
    return vec2_fract( v );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range [a,b].
attr_always_inline attr_header attr_hot
vec2 lerp( vec2 a, vec2 b, f32 t ) {
    return vec2_lerp( a, b, t );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range [a,b].
attr_always_inline attr_header attr_hot
vec2 lerp( vec2 a, vec2 b, vec2 t ) {
    return vec2_lerp_vec2( a, b, t );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range [a,b].
attr_always_inline attr_header attr_hot
vec2 mix( vec2 a, vec2 b, f32 t ) {
    return lerp( a, b, t );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Vector in range [a,b].
attr_always_inline attr_header attr_hot
vec2 mix( vec2 a, vec2 b, vec2 t ) {
    return lerp( a, b, t );
}
/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline attr_header attr_hot
vec2 step( vec2 edge, vec2 x ) {
    return vec2_step_vec2( edge, x );
}
/// @brief Step function.
/// @param edge Value to compare @c x to.
/// @param x    Value.
/// @return 0 if x < edge, otherwise 1.
attr_always_inline attr_header attr_hot
vec2 step( f32 edge, vec2 x ) {
    return vec2_step( edge, x );
}
/// @brief Smooth step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header attr_hot
vec2 smoothstep( vec2 edge0, vec2 edge1, vec2 x ) {
    return vec2_smoothstep_vec2( edge0, edge1, x );
}
/// @brief Smooth step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header attr_hot
vec2 smoothstep( f32 edge0, f32 edge1, vec2 x ) {
    return vec2_smoothstep( edge0, edge1, x );
}
/// @brief Smoother step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header attr_hot
vec2 smootherstep( vec2 edge0, vec2 edge1, vec2 x ) {
    return vec2_smootherstep_vec2( edge0, edge1, x );
}
/// @brief Smoother step interpolation.
/// @param edge0, edge1 Edges to interpolate between.
/// @param x            Value.
/// @return Result.
attr_always_inline attr_header attr_hot
vec2 smootherstep( f32 edge0, f32 edge1, vec2 x ) {
    return vec2_smootherstep( edge0, edge1, x );
}
/// @brief Check if vector components are NaN.
/// @param x Vector.
/// @return Component-wise result.
attr_always_inline attr_header attr_hot
bvec2 isnan( vec2 x ) {
    return vec2_isnan( x );
}
/// @brief Check if vector components are infinite.
/// @param x Vector.
/// @return Component-wise result.
attr_always_inline attr_header attr_hot
bvec2 isinf( vec2 x ) {
    return vec2_isinf( x );
}
/// @brief Convert degrees to radians.
/// @param degrees Angles in degrees.
/// @return Angles in radians.
attr_always_inline attr_header attr_hot
vec2 radians( vec2 degrees ) {
    return vec2_radians( degrees );
}
/// @brief Convert radians to degrees.
/// @param radians Angles in radians.
/// @return Angles in degrees.
attr_always_inline attr_header attr_hot
vec2 degrees( vec2 radians ) {
    return vec2_degrees( radians );
}
/// @brief Calculate sine of x.
/// @param angle Value to get sine of.
/// @return Sine of angle.
attr_always_inline attr_header attr_hot
vec2 sin( vec2 angle ) {
    return vec2_sin( angle );
}
/// @brief Calculate cosine of x.
/// @param angle Value to get cosine of.
/// @return Cosine of angle.
attr_always_inline attr_header attr_hot
vec2 cos( vec2 angle ) {
    return vec2_cos( angle );
}
/// @brief Calculate tangent of x.
/// @param angle Value to get tangent of.
/// @return Tangent of angle.
/// @warning Returns NaN if cosine of angle is zero.
attr_always_inline attr_header attr_hot
vec2 tan( vec2 angle ) {
    return vec2_tan( angle );
}
/// @brief Calculate arcsine of x.
///
/// Does not produce NaN when outside valid range.
/// @param angle Value to get arcsine of.
/// @return Arcsine of x clamped to [-Pi,Pi].
/// @see #F32_PI
/// @see #asin()
attr_always_inline attr_header attr_hot
vec2 asin( vec2 angle ) {
    return vec2_asin( angle );
}
/// @brief Calculate arccosine of x.
/// @param angle Value to get arccosine of.
/// @return Arccosine of x.
attr_always_inline attr_header attr_hot
vec2 acos( vec2 angle ) {
    return vec2_acos( angle );
}
/// @brief Calculate arctangent of x.
/// @param angle Value to get arctangent of.
/// @return Arctangent of x.
attr_always_inline attr_header attr_hot
vec2 atan( vec2 angle ) {
    return vec2_atan( angle );
}
/// @brief Calculate 2 component arctangent of y and x.
/// @param y Value to get arctangent of.
/// @param x Value to get arctangent of.
/// @return Arctangent of y and x.
attr_always_inline attr_header attr_hot
vec2 atan2( vec2 y, vec2 x ) {
    return vec2_atan2( y, x );
}
/// @brief Raise base to the power of exponent.
/// @param base Number to raise.
/// @param exp Power to raise to.
/// @return Result.
attr_always_inline attr_header attr_hot
vec2 pow( vec2 base, f32 exp ) {
    return vec2_pow( base, exp );
}
/// @brief Raise base to the power of exponent.
/// @param base Number to raise.
/// @param exp  Power to raise to.
/// @return Result.
attr_always_inline attr_header attr_hot
vec2 pow( vec2 base, vec2 exp ) {
    return vec2_pow_vec2( base, exp );
}
/// @brief Raise e to the power of x.
/// @param x Exponent.
/// @return Result.
attr_always_inline attr_header attr_hot
vec2 exp( vec2 x ) {
    return vec2_exp( x );
}
/// @brief Raise 2 to the power of x.
/// @param x Exponent.
/// @return Result.
attr_always_inline attr_header attr_hot
vec2 exp2( vec2 x ) {
    return vec2_exp2( x );
}
/// @brief Calculate natural logarithm.
/// @param x Value to get natural logarithm of.
/// @return Natural logarithm.
attr_always_inline attr_header attr_hot
vec2 ln( vec2 x ) {
    return vec2_ln( x );
}
/// @brief Calculate logarithm base 2.
/// @param x Value to get logarithm base 2 of.
/// @return Logarithm base 2.
attr_always_inline attr_header attr_hot
vec2 log2( vec2 x ) {
    return vec2_log2( x );
}
/// @brief Calculate logarithm base 10.
/// @param x Value to get logarithm base 10 of.
/// @return Logarithm base 10.
attr_always_inline attr_header attr_hot
vec2 log10( vec2 x ) {
    return vec2_log10( x );
}
/// @brief Calculate square root.
/// @param x Value to get square root of.
/// @return Square root.
attr_always_inline attr_header attr_hot
vec2 sqrt( vec2 x ) {
    return vec2_sqrt( x );
}
/// @brief Calculate reciprocal square root.
/// @param x Value to get reciprocal square root of.
/// @return Reciprocal square root.
attr_always_inline attr_header attr_hot
vec2 inversesqrt( vec2 x ) {
    return vec2_inversesqrt( x );
}
/// @brief Calculate cube root.
/// @param x Value to get cube root of.
/// @return Cube root.
attr_always_inline attr_header attr_hot
vec2 cbrt( vec2 x ) {
    return vec2_cbrt( x );
}
/// @brief Compare two vectors for equality.
/// @param a, b Vectors to compare.
/// @return True if the square magnitude of a - b is < F32_EPSILON.
/// @see #F32_EPSILON
attr_always_inline attr_header attr_hot
bool cmp( vec2 a, vec2 b ) {
    return vec2_cmp( a, b );
}
/// @brief Check if less than, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header attr_hot
bvec2 lt( vec2 a, vec2 b ) {
    return vec2_lt( a, b );
}
/// @brief Check if less than or equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header attr_hot
bvec2 lteq( vec2 a, vec2 b ) {
    return vec2_lteq( a, b );
}
/// @brief Check if greater than, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header attr_hot
bvec2 gt( vec2 a, vec2 b ) {
    return vec2_gt( a, b );
}
/// @brief Check if greater than or equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header attr_hot
bvec2 gteq( vec2 a, vec2 b ) {
    return vec2_gteq( a, b );
}
/// @brief Check if equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header attr_hot
bvec2 eq( vec2 a, vec2 b ) {
    return vec2_eq( a, b );
}
/// @brief Check if not equals, component-wise.
/// @param a, b Vectors.
/// @return Component-wise result.
attr_always_inline attr_header attr_hot
bvec2 neq( vec2 a, vec2 b ) {
    return vec2_neq( a, b );
}

constexpr attr_always_inline attr_header attr_hot
IVector2CPP::IVector2CPP()
    : x(0), y(0) {}
constexpr attr_always_inline attr_header attr_hot
IVector2CPP::IVector2CPP( const IVector2& __pod )
    : x(__pod.x), y(__pod.y) {}
constexpr attr_always_inline attr_header attr_hot
IVector2CPP::IVector2CPP( i32 x, i32 y )
    : x(x), y(y) {}
constexpr attr_always_inline attr_header attr_hot
IVector2CPP::IVector2CPP( i32 x )
    : x(x), y(x) {}
constexpr attr_always_inline attr_header attr_hot
IVector2CPP::IVector2CPP( const Vector2& v )
    : x(trunc(v.x)), y(trunc(v.y)) {}
constexpr attr_always_inline attr_header attr_hot
IVector2CPP::IVector2CPP( const BVector2& bv )
    : x(bv.x ? 1 : 0), y(bv.y ? 1 : 0) {}

constexpr attr_always_inline attr_header attr_hot
IVector2CPP::operator IVector2() const {
    return *(IVector2*)this;
}
constexpr attr_always_inline attr_header attr_hot
i32 IVector2CPP::operator[]( usize index ) const {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
i32& IVector2CPP::operator[]( usize index ) {
    return this->array[index];
}

constexpr attr_always_inline attr_header attr_hot
IVector2CPP IVector2CPP::zero() {
    return IVEC2_ZERO;
}
constexpr attr_always_inline attr_header attr_hot
IVector2CPP IVector2CPP::one() {
    return IVEC2_ONE;
}
constexpr attr_always_inline attr_header attr_hot
IVector2CPP IVector2CPP::up() {
    return IVEC2_UP;
}
constexpr attr_always_inline attr_header attr_hot
IVector2CPP IVector2CPP::right() {
    return IVEC2_RIGHT;
}
constexpr attr_always_inline attr_header attr_hot
IVector2CPP IVector2CPP::down() {
    return IVEC2_DOWN;
}
constexpr attr_always_inline attr_header attr_hot
IVector2CPP IVector2CPP::left() {
    return IVEC2_LEFT;
}

constexpr attr_always_inline attr_header attr_hot
ivec2 add( ivec2 lhs, ivec2 rhs ) {
    return ivec2_add( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 sub( ivec2 lhs, ivec2 rhs ) {
    return ivec2_sub( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 mul( ivec2 lhs, i32 rhs ) {
    return ivec2_mul( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 mul( i32 lhs, ivec2 rhs ) {
    return mul( rhs, lhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 mul( ivec2 lhs, ivec2 rhs ) {
    return ivec2_mul_ivec2( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 hadamard( ivec2 lhs, ivec2 rhs ) {
    return ivec2_hadamard( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 div( ivec2 lhs, i32 rhs ) {
    return ivec2_div( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 div( ivec2 lhs, ivec2 rhs ) {
    return ivec2_div_ivec2( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 mod( ivec2 lhs, i32 rhs ) {
    return ivec2_mod( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 mod( ivec2 lhs, ivec2 rhs ) {
    return ivec2_mod_ivec2( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 neg( ivec2 x ) {
    return ivec2_neg( x );
}

constexpr attr_always_inline attr_header attr_hot
ivec2 operator+( ivec2 lhs, ivec2 rhs ) {
    return add( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2& operator+=( ivec2& lhs, ivec2 rhs ) {
    return lhs = lhs + rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec2 operator-( ivec2 lhs, ivec2 rhs ) {
    return sub( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2& operator-=( ivec2& lhs, ivec2 rhs ) {
    return lhs = lhs - rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec2 operator*( ivec2 lhs, i32 rhs ) {
    return mul( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 operator*( i32 lhs, ivec2 rhs ) {
    return mul( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 operator*( ivec2 lhs, ivec2 rhs ) {
    return mul( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2& operator*=( ivec2& lhs, i32 rhs ) {
    return lhs = lhs * rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec2& operator*=( ivec2& lhs, ivec2 rhs ) {
    return lhs = lhs * rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec2 operator/( ivec2 lhs, i32 rhs ) {
    return div( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 operator/( ivec2 lhs, ivec2 rhs ) {
    return div( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2& operator/=( ivec2& lhs, i32 rhs ) {
    return lhs = lhs / rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec2& operator/=( ivec2& lhs, ivec2 rhs ) {
    return lhs = lhs / rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec2 operator%( ivec2 lhs, i32 rhs ) {
    return mod( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2 operator%( ivec2 lhs, ivec2 rhs ) {
    return mod( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec2& operator%=( ivec2& lhs, i32 rhs ) {
    return lhs = lhs % rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec2& operator%=( ivec2& lhs, ivec2 rhs ) {
    return lhs = lhs % rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec2 operator-( ivec2 x ) {
    return neg( x );
}

attr_always_inline attr_header attr_hot
ivec2 swap( ivec2 x ) {
    return ivec2_swap( x );
}
attr_always_inline attr_header attr_hot
i32 hadd( ivec2 x ) {
    return ivec2_hadd( x );
}
attr_always_inline attr_header attr_hot
i32 hmul( ivec2 x ) {
    return ivec2_hmul( x );
}
attr_always_inline attr_header attr_hot
f32 dot( ivec2 lhs, ivec2 rhs ) {
    return ivec2_dot( lhs, rhs );
}
attr_always_inline attr_header attr_hot
f32 aspect_ratio( ivec2 x ) {
    return ivec2_aspect_ratio( x );
}
attr_always_inline attr_header attr_hot
f32 length_sqr( ivec2 x ) {
    return ivec2_length_sqr( x );
}
attr_always_inline attr_header attr_hot
f32 length( ivec2 x ) {
    return ivec2_length( x );
}
attr_always_inline attr_header attr_hot
i32 min( ivec2 x ) {
    return ivec2_min( x );
}
attr_always_inline attr_header attr_hot
ivec2 min( ivec2 x, ivec2 y ) {
    return ivec2_min_ivec2( x, y );
}
attr_always_inline attr_header attr_hot
i32 max( ivec2 x ) {
    return ivec2_max( x );
}
attr_always_inline attr_header attr_hot
ivec2 max( ivec2 x, ivec2 y ) {
    return ivec2_max_ivec2( x, y );
}
attr_always_inline attr_header attr_hot
ivec2 clamp( ivec2 v, ivec2 min, ivec2 max ) {
    return ivec2_clamp( v, min, max );
}
attr_always_inline attr_header attr_hot
ivec2 abs( ivec2 x ) {
    return ivec2_abs( x );
}
attr_always_inline attr_header attr_hot
ivec2 sign( ivec2 x ) {
    return ivec2_sign( x );
}
attr_always_inline attr_header attr_hot
bool cmp( ivec2 a, ivec2 b ) {
    return ivec2_cmp( a, b );
}
attr_always_inline attr_header attr_hot
bvec2 lt( ivec2 a, ivec2 b ) {
    return ivec2_lt( a, b );
}
attr_always_inline attr_header attr_hot
bvec2 gt( ivec2 a, ivec2 b ) {
    return ivec2_gt( a, b );
}
attr_always_inline attr_header attr_hot
bvec2 lteq( ivec2 a, ivec2 b ) {
    return ivec2_lteq( a, b );
}
attr_always_inline attr_header attr_hot
bvec2 gteq( ivec2 a, ivec2 b ) {
    return ivec2_gteq( a, b );
}
attr_always_inline attr_header attr_hot
bvec2 eq( ivec2 a, ivec2 b ) {
    return ivec2_eq( a, b );
}
attr_always_inline attr_header attr_hot
bvec2 neq( ivec2 a, ivec2 b ) {
    return ivec2_neq( a, b );
}

constexpr attr_always_inline attr_header attr_hot
BVector2CPP::BVector2CPP()
    : x(false), y(false) {}
constexpr attr_always_inline attr_header attr_hot
BVector2CPP::BVector2CPP( const BVector2& __pod )
    : x(__pod.x), y(__pod.y) {}
constexpr attr_always_inline attr_header attr_hot
BVector2CPP::BVector2CPP( b32 x, b32 y )
    : x(x), y(y) {}
constexpr attr_always_inline attr_header attr_hot
BVector2CPP::BVector2CPP( b32 x )
    : x(x), y(x) {}
constexpr attr_always_inline attr_header attr_hot
BVector2CPP::BVector2CPP( const Vector2& v )
    : x(v.x == 0.0f ? false : true), y(v.y == 0.0f ? false : true) {}
constexpr attr_always_inline attr_header attr_hot
BVector2CPP::BVector2CPP( const IVector2& iv )
    : x(iv.x ? true : false), y(iv.y ? true : false) {}

constexpr attr_always_inline attr_header attr_hot
BVector2CPP::operator BVector2() const {
    return *(BVector2*)this;
}
constexpr attr_always_inline attr_header attr_hot
b32 BVector2CPP::operator[]( usize index ) const {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
b32& BVector2CPP::operator[]( usize index ) {
    return this->array[index];
}

constexpr attr_always_inline attr_header attr_hot
BVector2CPP BVector2CPP::zero() {
    return BVector2CPP( false );
}
constexpr attr_always_inline attr_header attr_hot
BVector2CPP BVector2CPP::one() {
    return BVector2CPP( true );
}

attr_always_inline attr_header attr_hot
bvec2 eq( bvec2 a, bvec2 b ) {
    return bvec2_eq( a, b );
}
attr_always_inline attr_header attr_hot
bvec2 neq( bvec2 a, bvec2 b ) {
    return bvec2_neq( a, b );
}
attr_always_inline attr_header attr_hot
bool any( bvec2 x ) {
    return bvec2_any( x );
}
attr_always_inline attr_header attr_hot
bool all( bvec2 x ) {
    return bvec2_all( x );
}
attr_always_inline attr_header attr_hot
bvec2 flip( bvec2 x ) {
    return bvec2_flip( x );
}

#endif /* header guard */
