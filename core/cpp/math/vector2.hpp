#if !defined(CORE_CPP_MATH_VECTOR2_HPP)
#define CORE_CPP_MATH_VECTOR2_HPP
/**
 * @file   vector2.hpp
 * @brief  C++ Vector2.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/
#include "core/cpp/math/swizzler.hpp"

struct Vector2CPP;
struct IVector2CPP;

#if !defined(CORE_MATH_VECTOR2_H)
    #include "core/math/vector2.h"
#endif

struct Vector2CPP {
    union {
        struct { f32 x, y; };
        struct { f32 w, h; };
        struct { f32 u, v; };
        struct Vector2 pod;

        Swizzler<Vector2CPP, f32, 0, 0> xx, uu, ww;
        Swizzler<Vector2CPP, f32, 1, 1> yy, vv, hh;
        Swizzler<Vector2CPP, f32, 1, 0> yx, vu, hw;

        f32 array[2];
    };

    attr_always_inline attr_header constexpr
    Vector2CPP() : x(0), y(0) {}
    attr_always_inline attr_header constexpr
    Vector2CPP( const struct Vector2& v ) : x(v.x), y(v.y) {}
    attr_always_inline attr_header constexpr
    explicit Vector2CPP( const struct IVector2& v ) : x(v.x), y(v.y) {}
    attr_always_inline attr_header constexpr
    explicit Vector2CPP( f32 s ) : x(s), y(s) {}
    attr_always_inline attr_header constexpr
    explicit Vector2CPP( f32 x, f32 y ) : x(x), y(y) {}

    attr_always_inline attr_header constexpr
    operator Vector2() const {
        return *(struct Vector2*)this;
    }

    attr_always_inline attr_header static constexpr
    Vector2CPP zero() {
        return Vector2CPP();
    }
    attr_always_inline attr_header static constexpr
    Vector2CPP one() {
        return Vector2CPP( 1.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector2CPP left() {
        return Vector2CPP( -1.0, 0.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector2CPP right() {
        return Vector2CPP( 1.0, 0.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector2CPP up() {
        return Vector2CPP( 0.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector2CPP down() {
        return Vector2CPP( 0.0, -1.0 );
    }

    attr_always_inline attr_header static
    Vector2CPP from_array( const f32 array[2] ) {
        return *(Vector2CPP*)array;
    }
    attr_always_inline attr_header constexpr
    void to_array( f32 out_array[2] ) const {
        out_array[0] = array[0]; out_array[1] = array[1];
    }

    attr_always_inline attr_header constexpr
    f32 operator[]( usize idx ) const {
        return array[idx];
    }
    attr_always_inline attr_header constexpr
    f32& operator[]( usize idx ) {
        return array[idx];
    }
};
struct IVector2CPP {
    union {
        struct { i32 x, y; };
        struct { i32 w, h; };
        struct IVector2 pod;

        Swizzler<IVector2CPP, i32, 0, 0> xx, ww;
        Swizzler<IVector2CPP, i32, 1, 1> yy, hh;
        Swizzler<IVector2CPP, i32, 1, 0> yx, hw;

        i32 array[2];
    };

    attr_always_inline attr_header constexpr
    IVector2CPP() : x(0), y(0) {}
    attr_always_inline attr_header constexpr
    IVector2CPP( const struct IVector2& v ) : x(v.x), y(v.y) {}
    attr_always_inline attr_header constexpr
    explicit IVector2CPP( const struct Vector2& v ) : x(v.x), y(v.y) {}
    attr_always_inline attr_header constexpr
    explicit IVector2CPP( i32 s ) : x(s), y(s) {}
    attr_always_inline attr_header constexpr
    explicit IVector2CPP( i32 x, i32 y ) : x(x), y(y) {}

    attr_always_inline attr_header constexpr
    operator IVector2() const {
        return *(struct IVector2*)this;
    }

    attr_always_inline attr_header static constexpr
    IVector2CPP zero() {
        return IVector2CPP();
    }
    attr_always_inline attr_header static constexpr
    IVector2CPP one() {
        return IVector2CPP( 1, 1 );
    }
    attr_always_inline attr_header static constexpr
    IVector2CPP left() {
        return IVector2CPP( -1, 0 );
    }
    attr_always_inline attr_header static constexpr
    IVector2CPP right() {
        return IVector2CPP( 1, 0 );
    }
    attr_always_inline attr_header static constexpr
    IVector2CPP up() {
        return IVector2CPP( 0, 1 );
    }
    attr_always_inline attr_header static constexpr
    IVector2CPP down() {
        return IVector2CPP( 0, -1 );
    }

    attr_always_inline attr_header static constexpr
    IVector2CPP from_array( const i32 array[2] ) {
        return *(IVector2CPP*)array;
    }
    attr_always_inline attr_header constexpr
    void to_array( i32 out_array[2] ) const {
        out_array[0] = array[0]; out_array[1] = array[1];
    }

    attr_always_inline attr_header constexpr
    i32 operator[]( usize idx ) const {
        return array[idx];
    }
    attr_always_inline attr_header constexpr
    i32& operator[]( usize idx ) {
        return array[idx];
    }
};
attr_always_inline attr_header
Vector2CPP add(
    Vector2CPP lhs, Vector2CPP rhs
) {
    return vec2_add( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Vector2CPP sub(
    Vector2CPP lhs, Vector2CPP rhs
) {
    return vec2_sub( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Vector2CPP mul( Vector2CPP lhs, f32 rhs ) {
    return vec2_mul( lhs.pod, rhs );
}
attr_always_inline attr_header
Vector2CPP mul( f32 lhs, Vector2CPP rhs ) {
    return vec2_mul( rhs.pod, lhs );
}
attr_always_inline attr_header
Vector2CPP mul(
    Vector2CPP lhs, Vector2CPP rhs
) {
    return vec2_mul_vec2( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Vector2CPP hadamard(
    Vector2CPP lhs, Vector2CPP rhs
) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP div( Vector2CPP lhs, f32 rhs ) {
    return vec2_div( lhs.pod, rhs );
}
attr_always_inline attr_header
Vector2CPP div( Vector2CPP lhs, Vector2CPP rhs ) {
    return vec2_div_vec2( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Vector2CPP neg( Vector2CPP x ) {
    return vec2_neg( x.pod );
}
attr_always_inline attr_header
f32 hadd( Vector2CPP x ) {
    return vec2_hadd( x.pod );
}
attr_always_inline attr_header
f32 hmul( Vector2CPP x ) {
    return vec2_hmul( x.pod );
}
attr_always_inline attr_header
f32 dot(
    Vector2CPP lhs, Vector2CPP rhs
) {
    return vec2_dot( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
f32 aspect_ratio( Vector2CPP x ) {
    return vec2_aspect_ratio( x.pod );
}
attr_always_inline attr_header
f32 hmax( Vector2CPP x ) {
    return vec2_hmax( x.pod );
}
attr_always_inline attr_header
Vector2CPP max( Vector2CPP x, Vector2CPP y ) {
    return vec2_max( x.pod, y.pod );
}
attr_always_inline attr_header
f32 hmin( Vector2CPP x ) {
    return vec2_hmin( x.pod );
}
attr_always_inline attr_header
Vector2CPP min( Vector2CPP x, Vector2CPP y ) {
    return vec2_min( x.pod, y.pod );
}
attr_always_inline attr_header
f32 length_sqr( Vector2CPP x ) {
    return vec2_length_sqr( x.pod );
}
attr_always_inline attr_header
f32 length( Vector2CPP x ) {
    return vec2_length( x.pod );
}
attr_always_inline attr_header
f32 distance_sqr( Vector2CPP a, Vector2CPP b ) {
    return vec2_distance_sqr( a.pod, b.pod );
}
attr_always_inline attr_header
f32 distance( Vector2CPP a, Vector2CPP b ) {
    return vec2_distance( a.pod, b.pod );
}
attr_always_inline attr_header
Vector2CPP normalize( Vector2CPP x ) {
    return vec2_normalize( x.pod );
}
attr_always_inline attr_header
Vector2CPP reflect( Vector2CPP direction, Vector2CPP normal ) {
    return vec2_reflect( direction.pod, normal.pod );
}
attr_always_inline attr_header
Vector2CPP rotate( Vector2CPP v, f32 angle ) {
    return vec2_rotate( v.pod, angle );
}
attr_always_inline attr_header
Vector2CPP clamp(
    Vector2CPP v, Vector2CPP min, Vector2CPP max
) {
    return vec2_clamp( v.pod, min.pod, max.pod );
}
attr_always_inline attr_header
Vector2CPP clamp(
    const Vector2CPP v, f32 min, f32 max
) {
    return vec2_clamp_length( v.pod, min, max );
}
attr_always_inline attr_header
f32 angle( Vector2CPP a, Vector2CPP b ) {
    return vec2_angle( a.pod, b.pod );
}
attr_always_inline attr_header
Vector2CPP abs( Vector2CPP v ) {
    return vec2_abs( v.pod );
}
attr_always_inline attr_header
Vector2CPP sign( Vector2CPP v ) {
    return vec2_sign( v.pod );
}
attr_always_inline attr_header
Vector2CPP trunc( Vector2CPP v ) {
    return vec2_trunc( v.pod );
}
attr_always_inline attr_header
Vector2CPP floor( Vector2CPP v ) {
    return vec2_floor( v.pod );
}
attr_always_inline attr_header
Vector2CPP ceil( Vector2CPP v ) {
    return vec2_ceil( v.pod );
}
attr_always_inline attr_header
Vector2CPP round( Vector2CPP v ) {
    return vec2_round( v.pod );
}
attr_always_inline attr_header
Vector2CPP fract( Vector2CPP v ) {
    return vec2_fract( v.pod );
}
attr_always_inline attr_header
Vector2CPP lerp(
    Vector2CPP a, Vector2CPP b, f32 t 
) {
    return vec2_lerp( a.pod, b.pod, t );
}
attr_always_inline attr_header
Vector2CPP mix(
    Vector2CPP a, Vector2CPP b, f32 t 
) {
    return lerp( a, b, t );
}
attr_always_inline attr_header
Vector2CPP step( Vector2CPP edge, Vector2CPP x ) {
    return vec2_step( edge.pod, x.pod );
}
attr_always_inline attr_header
Vector2CPP step( f32 edge, Vector2CPP x ) {
    return vec2_step_scalar( edge, x.pod );
}
attr_always_inline attr_header
Vector2CPP smoothstep(
    Vector2CPP edge0, Vector2CPP edge1, Vector2CPP x
) {
    return vec2_smoothstep( edge0.pod, edge1.pod, x.pod );
}
attr_always_inline attr_header
Vector2CPP smoothstep(
    f32 edge0, f32 edge1, Vector2CPP x
) {
    return vec2_smoothstep_scalar( edge0, edge1, x );
}
attr_always_inline attr_header
Vector2CPP smootherstep(
    Vector2CPP edge0, Vector2CPP edge1, Vector2CPP x
) {
    return vec2_smootherstep( edge0.pod, edge1.pod, x.pod );
}
attr_always_inline attr_header
Vector2CPP smootherstep(
    f32 edge0, f32 edge1, Vector2CPP x
) {
    return vec2_smootherstep_scalar( edge0, edge1, x );
}
attr_always_inline attr_header
Vector2CPP radians( Vector2CPP degrees ) {
    return vec2_radians( degrees.pod );
}
attr_always_inline attr_header
Vector2CPP degrees( Vector2CPP radians ) {
    return vec2_degrees( radians.pod );
}
attr_always_inline attr_header
Vector2CPP sin( Vector2CPP angle ) {
    return vec2_sin( angle.pod );
}
attr_always_inline attr_header
Vector2CPP cos( Vector2CPP angle ) {
    return vec2_cos( angle.pod );
}
attr_always_inline attr_header
Vector2CPP tan( Vector2CPP angle ) {
    return vec2_tan( angle.pod );
}
attr_always_inline attr_header
Vector2CPP asin( Vector2CPP angle ) {
    return vec2_asin( angle.pod );
}
attr_always_inline attr_header
Vector2CPP acos( Vector2CPP angle ) {
    return vec2_acos( angle.pod );
}
attr_always_inline attr_header
Vector2CPP atan2( Vector2CPP y, Vector2CPP x ) {
    return vec2_atan2( y.pod, x.pod );
}
attr_always_inline attr_header
b32 cmp( Vector2CPP a, Vector2CPP b ) {
    return vec2_cmp( a.pod, b.pod );
}

attr_always_inline attr_header
Vector2CPP operator-( Vector2CPP v ) {
    return neg( v );
}
attr_always_inline attr_header
Vector2CPP& operator+=( Vector2CPP& lhs, Vector2CPP rhs ) {
    return lhs = add( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP& operator-=( Vector2CPP& lhs, Vector2CPP rhs ) {
    return lhs = sub( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP& operator*=( Vector2CPP& lhs, Vector2CPP rhs ) {
    return lhs = mul( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP& operator/=( Vector2CPP& lhs, Vector2CPP rhs ) {
    return lhs = div( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP& operator*=( Vector2CPP& lhs, f32 rhs ) {
    return lhs = mul( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP& operator/=( Vector2CPP& lhs, f32 rhs ) {
    return lhs = div( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP operator+( Vector2CPP lhs, Vector2CPP rhs ) {
    return add( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP operator-( Vector2CPP lhs, Vector2CPP rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP operator*( Vector2CPP lhs, Vector2CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP operator/( Vector2CPP lhs, Vector2CPP rhs ) {
    return div( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP operator*( Vector2CPP lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP operator*( f32 lhs, Vector2CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Vector2CPP operator/( Vector2CPP lhs, f32 rhs ) {
    return div( lhs, rhs );
}
attr_always_inline attr_header
b32 operator==( Vector2CPP a, Vector2CPP b ) {
    return cmp( a, b );
}
attr_always_inline attr_header
b32 operator!=( Vector2CPP a, Vector2CPP b ) {
    return !(a == b);
}

attr_always_inline attr_header
IVector2CPP add( IVector2CPP lhs, IVector2CPP rhs ) {
    return ivec2_add( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector2CPP sub( IVector2CPP lhs, IVector2CPP rhs ) {
    return ivec2_sub( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector2CPP mul( IVector2CPP lhs, IVector2CPP rhs ) {
    return ivec2_mul_ivec2( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector2CPP hadamard( IVector2CPP lhs, IVector2CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP mul( IVector2CPP lhs, i32 rhs ) {
    return ivec2_mul( lhs.pod, rhs );
}
attr_always_inline attr_header
IVector2CPP mul( i32 lhs, IVector2CPP rhs ) {
    return ivec2_mul( rhs.pod, lhs );
}
attr_always_inline attr_header
IVector2CPP div( IVector2CPP lhs, IVector2CPP rhs ) {
    return ivec2_div_ivec2( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector2CPP div( IVector2CPP lhs, i32 rhs ) {
    return ivec2_div( lhs.pod, rhs );
}
attr_always_inline attr_header
IVector2CPP neg( IVector2CPP v ) {
    return ivec2_neg( v.pod );
}
attr_always_inline attr_header
i32 hadd( IVector2CPP v ) {
    return ivec2_hadd( v.pod );
}
attr_always_inline attr_header
i32 hmul( IVector2CPP v ) {
    return ivec2_hmul( v.pod );
}
attr_always_inline attr_header
f32 dot( IVector2CPP lhs, IVector2CPP rhs ) {
    return ivec2_dot( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
f32 aspect_ratio( IVector2CPP v ) {
    return ivec2_aspect_ratio( v.pod );
}
attr_always_inline attr_header
f32 length_sqr( IVector2CPP v ) {
    return ivec2_length_sqr( v.pod );
}
attr_always_inline attr_header
f32 length( IVector2CPP v ) {
    return ivec2_length( v.pod );
}
attr_always_inline attr_header
b32 cmp( IVector2CPP a, IVector2CPP b ) {
    return ivec2_cmp( a.pod, b.pod );
}

attr_always_inline attr_header
IVector2CPP& operator+=( IVector2CPP& lhs, IVector2CPP rhs ) {
    return lhs = add( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP& operator-=( IVector2CPP& lhs, IVector2CPP rhs ) {
    return lhs = sub( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP& operator*=( IVector2CPP& lhs, IVector2CPP rhs ) {
    return lhs = mul( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP& operator*=( IVector2CPP& lhs, i32 rhs ) {
    return lhs = mul( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP& operator/=( IVector2CPP& lhs, IVector2CPP rhs ) {
    return lhs = div( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP& operator/=( IVector2CPP& lhs, i32 rhs ) {
    return lhs = div( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP operator+( IVector2CPP lhs, IVector2CPP rhs ) {
    return add( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP operator-( IVector2CPP lhs, IVector2CPP rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP operator*( IVector2CPP lhs, IVector2CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP operator*( IVector2CPP lhs, i32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP operator*( i32 lhs, IVector2CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP operator/( IVector2CPP lhs, IVector2CPP rhs ) {
    return div( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP operator/( IVector2CPP lhs, i32 rhs ) {
    return div( lhs, rhs );
}
attr_always_inline attr_header
IVector2CPP operator-( IVector2CPP v ) {
    return neg( v );
}
attr_always_inline attr_header
b32 operator==( IVector2CPP a, IVector2CPP b ) {
    return cmp( a, b );
}
attr_always_inline attr_header
b32 operator!=( IVector2CPP a, IVector2CPP b ) {
    return !( a == b );
}

#endif /* header guard */
