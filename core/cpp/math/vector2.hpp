#if !defined(CORE_CPP_MATH_VECTOR2_HPP)
#define CORE_CPP_MATH_VECTOR2_HPP
/**
 * @file   vector2.hpp
 * @brief  C++ Vector2.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/

struct Vector2CPP;
struct IVector2CPP;

#if !defined(CORE_MATH_VECTOR2_H)
    #include "core/math/vector2.h"
#endif

struct Vector2CPP : public Vector2 {
    attr_header Vector2CPP() : Vector2{ .x=0, .y=0 } {}
    attr_header Vector2CPP( f32 x, f32 y ) : Vector2{ .x=x, .y=y } {}
    attr_header Vector2CPP( const Vector2& v ) : Vector2CPP( v.x, v.y ) {}
    attr_header explicit Vector2CPP( f32 s ) : Vector2CPP( s, s ) {}
    attr_header explicit Vector2CPP( const f32 a[2] ) : Vector2CPP( a[0], a[1] ) {}
    attr_header explicit Vector2CPP( const IVector2& v ) : Vector2CPP( v.x, v.y ) {}

    attr_header static Vector2CPP zero() {
        return VEC2_ZERO;
    }
    attr_header static Vector2CPP one() {
        return VEC2_ONE;
    }
    attr_header static Vector2CPP left() {
        return VEC2_LEFT;
    }
    attr_header static Vector2CPP right() {
        return VEC2_RIGHT;
    }
    attr_header static Vector2CPP up() {
        return VEC2_UP;
    }
    attr_header static Vector2CPP down() {
        return VEC2_DOWN;
    }

    attr_header static Vector2CPP from_array( const f32 a[2] ) {
        return Vector2CPP( a );
    }

    attr_header void to_array( f32 out_array[2] ) const {
        out_array[0] = v[0];
        out_array[1] = v[1];
    }
    attr_header Vector2CPP add( const Vector2CPP& rhs ) const {
        return v2_add( *this, rhs );
    }
    attr_header Vector2CPP sub( const Vector2CPP& rhs ) const {
        return v2_sub( *this, rhs );
    }
    attr_header Vector2CPP mul( f32 rhs ) const {
        return v2_mul( *this, rhs );
    }
    attr_header Vector2CPP div( f32 rhs ) const {
        return v2_div( *this, rhs );
    }
    attr_header Vector2CPP neg(void) const {
        return v2_neg( *this );
    }
    attr_header Vector2CPP swap(void) const {
        return v2_swap( *this );
    }
    attr_header f32 hadd(void) const {
        return v2_hadd( *this );
    }
    attr_header f32 hmul(void) const {
        return v2_hmul( *this );
    }
    attr_header Vector2CPP hadamard( const Vector2CPP& rhs ) const {
        return v2_hadamard( *this, rhs );
    }
    attr_header f32 dot( const Vector2CPP& rhs ) const {
        return v2_dot( *this, rhs );
    }
    attr_header f32 aspect_ratio(void) const {
        return v2_aspect_ratio( *this );
    }
    attr_header f32 max(void) const {
        return v2_max( *this );
    }
    attr_header f32 min(void) const {
        return v2_min( *this );
    }
    attr_header f32 sqrmag(void) const {
        return v2_sqrmag( *this );
    }
    attr_header f32 mag(void) const {
        return v2_mag( *this );
    }
    attr_header Vector2CPP normalize(void) const {
        return v2_normalize(*this);
    }
    attr_header Vector2CPP rotate( f32 angle ) const {
        return v2_rotate( *this, angle );
    }
    attr_header Vector2 clamp_mag( f32 min_, f32 max_ ) const {
        return v2_clamp_mag( *this, min_, max_ );
    }
    attr_header f32 angle( const Vector2CPP& b ) const {
        return v2_angle( *this, b );
    }
    attr_header Vector2CPP lerp( const Vector2CPP& b, f32 t ) const {
        return v2_lerp( *this, b, t );
    }
    attr_header Vector2CPP smooth_step( const Vector2CPP& b, f32 t ) const {
        return v2_smooth_step( *this, b, t );
    }
    attr_header Vector2CPP smoother_step( const Vector2CPP& b, f32 t ) const {
        return v2_smoother_step( *this, b, t );
    }
    attr_header b32 cmp( const Vector2CPP& b ) const {
        return v2_cmp( *this, b );
    }

    attr_header f32 operator[]( usize idx ) const {
        return v[idx];
    }
    attr_header f32& operator[]( usize idx ) {
        return v[idx];
    }
    attr_header Vector2CPP& operator+=( const Vector2CPP& rhs ) {
        return *this = add( rhs );
    }
    attr_header Vector2CPP& operator-=( const Vector2CPP& rhs ) {
        return *this = sub( rhs );
    }
    attr_header Vector2CPP& operator*=( f32 rhs ) {
        return *this = mul(rhs);
    }
    attr_header Vector2CPP& operator/=( f32 rhs ) {
        return *this = div(rhs);
    }
    attr_header Vector2CPP operator-(void) const {
        return neg();
    }
};
attr_header Vector2 operator+( const Vector2& lhs, const Vector2& rhs ) {
    return v2_add( lhs, rhs );
}
attr_header Vector2 operator-( const Vector2& lhs, const Vector2& rhs ) {
    return v2_sub( lhs, rhs );
}
attr_header Vector2 operator*( const Vector2& lhs, f32 rhs ) {
    return v2_mul( lhs, rhs );
}
attr_header Vector2 operator*( f32 lhs, const Vector2& rhs ) {
    return v2_mul( rhs, lhs );
}
attr_header Vector2 operator/( const Vector2& lhs, f32 rhs ) {
    return v2_div( lhs, rhs );
}
attr_header b32 operator==( const Vector2& a, const Vector2& b ) {
    return v2_cmp( a, b );
}
attr_header b32 operator!=( const Vector2& a, const Vector2& b ) {
    return !(a == b);
}

attr_header Vector2CPP add( const Vector2CPP& lhs, const Vector2CPP& rhs ) {
    return lhs + rhs;
}
attr_header Vector2CPP sub( const Vector2CPP& lhs, const Vector2CPP& rhs ) {
    return lhs - rhs;
}
attr_header Vector2CPP mul( const Vector2CPP& lhs, f32 rhs ) {
    return lhs * rhs;
}
attr_header Vector2CPP mul( f32 lhs, const Vector2CPP& rhs ) {
    return lhs * rhs;
}
attr_header Vector2CPP div( const Vector2CPP& lhs, f32 rhs ) {
    return lhs / rhs;
}
attr_header Vector2CPP neg( const Vector2CPP& v ) {
    return -v;
}
attr_header b32 cmp( const Vector2CPP& lhs, const Vector2CPP& rhs ) {
    return lhs == rhs;
}
attr_header f32 hadd( const Vector2CPP& v ) {
    return v.hadd();
}
attr_header f32 hmul( const Vector2CPP& v ) {
    return v.hmul();
}
attr_header Vector2CPP hadamard( const Vector2CPP& lhs, const Vector2CPP& rhs ) {
    return lhs.hadamard(rhs);
}
attr_header f32 dot( const Vector2CPP& lhs, const Vector2CPP& rhs ) {
    return lhs.dot(rhs);
}
attr_header f32 max( const Vector2CPP& v ) {
    return v.max();
}
attr_header f32 min( const Vector2CPP& v ) {
    return v.min();
}
attr_header f32 sqrmag( const Vector2CPP& v ) {
    return v.sqrmag();
}
attr_header f32 mag( const Vector2CPP& v ) {
    return v.mag();
}
attr_header Vector2CPP normalize( const Vector2CPP& v ) {
    return v.normalize();
}
attr_header Vector2CPP rotate( const Vector2CPP& v, f32 angle ) {
    return v.rotate( angle );
}
attr_header Vector2CPP lerp( const Vector2CPP& a, const Vector2CPP& b, f32 t ) {
    return a.lerp( b, t );
}
attr_header Vector2CPP smooth_step( const Vector2CPP& a, const Vector2CPP& b, f32 t ) {
    return a.smooth_step( b, t );
}
attr_header Vector2CPP smoother_step(
    const Vector2CPP& a, const Vector2CPP& b, f32 t
) {
    return a.smoother_step( b, t );
}

struct IVector2CPP : public IVector2 {
    attr_header IVector2CPP() : IVector2{ .x=0, .y=0 } {}
    attr_header IVector2CPP( i32 x, i32 y ) : IVector2{ .x=x, .y=y } {}
    attr_header IVector2CPP( const IVector2& v ) : IVector2CPP( v.x, v.y ) {}
    attr_header explicit IVector2CPP( i32 s ) : IVector2CPP( s, s ) {}
    attr_header explicit IVector2CPP( const i32 a[2] ) : IVector2CPP( a[0], a[1] ) {}
    attr_header explicit IVector2CPP( const Vector2& v ) : IVector2CPP( v.x, v.y ) {}

    attr_header static IVector2CPP zero(void) {
        return IVEC2_ZERO;
    }
    attr_header static IVector2CPP one(void) {
        return IVEC2_ONE;
    }
    attr_header static IVector2CPP left(void) {
        return IVEC2_LEFT;
    }
    attr_header static IVector2CPP right(void) {
        return IVEC2_RIGHT;
    }
    attr_header static IVector2CPP up(void) {
        return IVEC2_UP;
    }
    attr_header static IVector2CPP down(void) {
        return IVEC2_DOWN;
    }

    attr_header static IVector2CPP from_array( const i32 a[2] ) {
        return IVector2CPP( a );
    }

    attr_header void to_array( i32 out_array[2] ) const {
        out_array[0] = v[0];
        out_array[1] = v[1];
    }
    attr_header IVector2CPP add( const IVector2CPP& rhs ) const {
        return iv2_add( *this, rhs );
    }
    attr_header IVector2CPP sub( const IVector2CPP& rhs ) const {
        return iv2_sub( *this, rhs );
    }
    attr_header IVector2CPP mul( i32 rhs ) const {
        return iv2_mul( *this, rhs );
    }
    attr_header IVector2CPP div( i32 rhs ) const {
        return iv2_div( *this, rhs );
    }
    attr_header IVector2CPP neg(void) const {
        return iv2_neg( *this );
    }
    attr_header IVector2CPP swap(void) const {
        return iv2_swap( *this );
    }
    attr_header i32 hadd(void) const {
        return iv2_hadd( *this );
    }
    attr_header i32 hmul(void) const {
        return iv2_hmul( *this );
    }
    attr_header IVector2CPP hadamard( const IVector2CPP& rhs ) const {
        return iv2_hadamard( *this, rhs );
    }
    attr_header f32 dot( const IVector2CPP& rhs ) const {
        return iv2_dot( *this, rhs );
    }
    attr_header f32 aspect_ratio(void) const {
        return iv2_aspect_ratio( *this );
    }
    attr_header f32 sqrmag(void) const {
        return iv2_sqrmag( *this );
    }
    attr_header f32 mag(void) const {
        return iv2_mag( *this );
    }
    attr_header b32 cmp( const IVector2CPP& rhs ) const {
        return iv2_cmp( *this, rhs );
    }

    attr_header i32 operator[]( usize idx ) const {
        return v[idx];
    }
    attr_header i32& operator[]( usize idx ) {
        return v[idx];
    }
    attr_header IVector2CPP& operator+=( const IVector2CPP& rhs ) {
        return *this = add( rhs );
    }
    attr_header IVector2CPP& operator-=( const IVector2CPP& rhs ) {
        return *this = sub( rhs );
    }
    attr_header IVector2CPP& operator*=( i32 rhs ) {
        return *this = mul( rhs );
    }
    attr_header IVector2CPP& operator/=( i32 rhs ) {
        return *this = div( rhs );
    }
    attr_header IVector2CPP operator-(void) const {
        return neg();
    }
};
attr_header IVector2 operator+( const IVector2& lhs, const IVector2& rhs ) {
    return iv2_add( lhs, rhs );
}
attr_header IVector2 operator-( const IVector2& lhs, const IVector2& rhs ) {
    return iv2_sub( lhs, rhs );
}
attr_header IVector2 operator*( const IVector2& lhs, i32 rhs ) {
    return iv2_mul( lhs, rhs );
}
attr_header IVector2 operator*( i32 lhs, const IVector2& rhs ) {
    return iv2_mul( rhs, lhs );
}
attr_header IVector2 operator/( const IVector2& lhs, i32 rhs ) {
    return iv2_div( lhs, rhs );
}
attr_header b32 operator==( const IVector2& a, const IVector2& b ) {
    return iv2_cmp( a, b );
}
attr_header b32 operator!=( const IVector2& a, const IVector2& b ) {
    return !(a == b);
}

attr_header IVector2CPP add( const IVector2CPP& lhs, const IVector2CPP& rhs ) {
    return lhs + rhs;
}
attr_header IVector2CPP sub( const IVector2CPP& lhs, const IVector2CPP& rhs ) {
    return lhs - rhs;
}
attr_header IVector2CPP mul( const IVector2CPP& lhs, i32 rhs ) {
    return lhs * rhs;
}
attr_header IVector2CPP mul( i32 lhs, const IVector2CPP& rhs ) {
    return lhs * rhs;
}
attr_header IVector2CPP div( const IVector2CPP& lhs, i32 rhs ) {
    return lhs / rhs;
}
attr_header IVector2CPP neg( const IVector2CPP& v ) {
    return -v;
}
attr_header b32 cmp( const IVector2CPP& lhs, const IVector2CPP& rhs ) {
    return lhs == rhs;
}
attr_header i32 hadd( const IVector2CPP& v ) {
    return v.hadd();
}
attr_header i32 hmul( const IVector2CPP& v ) {
    return v.hmul();
}
attr_header IVector2CPP hadamard( const IVector2CPP& lhs, const IVector2CPP& rhs ) {
    return lhs.hadamard(rhs);
}
attr_header i32 dot( const IVector2CPP& lhs, const IVector2CPP& rhs ) {
    return lhs.dot(rhs);
}
attr_header i32 sqrmag( const IVector2CPP& v ) {
    return v.sqrmag();
}
attr_header i32 mag( const IVector2CPP& v ) {
    return v.mag();
}

#endif /* header guard */
