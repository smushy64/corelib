#if !defined(CORE_CPP_MATH_VECTOR4_HPP)
#define CORE_CPP_MATH_VECTOR4_HPP
/**
 * @file   vector4.hpp
 * @brief  C++ Vector4.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/

struct Vector4CPP;
struct IVector4CPP;

#if !defined(CORE_MATH_VECTOR4_H)
    #include "core/math/vector4.h"
#endif

struct Vector4CPP : public Vector4 {
    attr_header Vector4CPP() : Vector4{.x=0,.y=0,.z=0,.w=0} {}
    attr_header Vector4CPP( f32 x, f32 y, f32 z, f32 w ) :
        Vector4{.x=x,.y=y,.z=z,.w=w} {}
    attr_header Vector4CPP( const Vector4& v ) : Vector4{v} {}
    attr_header explicit Vector4CPP( f32 s ) : Vector4CPP( s, s, s, s ) {}
    attr_header explicit Vector4CPP( const f32 a[4] ) :
        Vector4CPP( a[0], a[1], a[2], a[3] ) {}
    attr_header explicit Vector4CPP( const IVector4& v ) :
        Vector4CPP( v.x, v.y, v.z, v.w ) {}

    attr_header static Vector4CPP zero(void) {
        return VEC4_ZERO;
    }
    attr_header static Vector4CPP one(void) {
        return VEC4_ONE;
    }

    attr_header static Vector4CPP red(void) {
        return RGBA_RED;
    }
    attr_header static Vector4CPP green(void) {
        return RGBA_GREEN;
    }
    attr_header static Vector4CPP blue(void) {
        return RGBA_BLUE;
    }
    attr_header static Vector4CPP yellow(void) {
        return RGBA_YELLOW;
    }
    attr_header static Vector4CPP magenta(void) {
        return RGBA_MAGENTA;
    }
    attr_header static Vector4CPP cyan(void) {
        return RGBA_CYAN;
    }
    attr_header static Vector4CPP black(void) {
        return RGBA_BLACK;
    }
    attr_header static Vector4CPP white(void) {
        return RGBA_WHITE;
    }
    attr_header static Vector4CPP clear(void) {
        return RGBA_CLEAR;
    }

    attr_header static Vector4CPP from_array( const f32 a[4] ) {
        return v4_from_array( a );
    }

    attr_header void to_array( f32 out_array[4] ) const {
        v4_to_array( *this, out_array );
    }
    attr_header Vector4CPP add( const Vector4CPP& rhs ) const {
        return v4_add( *this, rhs );
    }
    attr_header Vector4CPP sub( const Vector4CPP& rhs ) const {
        return v4_sub( *this, rhs );
    }
    attr_header Vector4CPP mul( f32 rhs ) const {
        return v4_mul( *this, rhs );
    }
    attr_header Vector4CPP div( f32 rhs ) const {
        return v4_div( *this, rhs );
    }
    attr_header Vector4CPP neg(void) const {
        return v4_neg( *this );
    }
    attr_header Vector4CPP rotl(void) const {
        return v4_rotl( *this );
    }
    attr_header Vector4CPP rotr(void) const {
        return v4_rotr( *this );
    }
    attr_header f32 hadd(void) const {
        return v4_hadd( *this );
    }
    attr_header f32 hmul(void) const {
        return v4_hmul( *this );
    }
    attr_header Vector4CPP hadamard( const Vector4CPP& rhs ) const {
        return v4_hadamard( *this, rhs );
    }
    attr_header f32 dot( const Vector4CPP& rhs ) const {
        return v4_dot( *this, rhs );
    }
    attr_header f32 max(void) const {
        return v4_max( *this );
    }
    attr_header f32 min(void) const {
        return v4_min( *this );
    }
    attr_header f32 sqrmag(void) const {
        return v4_sqrmag( *this );
    }
    attr_header f32 mag(void) const {
        return v4_mag( *this );
    }
    attr_header Vector4CPP normalize(void) const {
        return v4_normalize( *this );
    }
    attr_header Vector4CPP clamp_mag( f32 min_, f32 max_ ) const {
        return v4_clamp_mag( *this, min_, max_ );
    }
    attr_header Vector4CPP lerp( const Vector4CPP& b, f32 t ) const {
        return v4_lerp( *this, b, t );
    }
    attr_header Vector4CPP smooth_step( const Vector4CPP& b, f32 t ) const {
        return v4_smooth_step( *this, b, t );
    }
    attr_header Vector4CPP smoother_step( const Vector4CPP& b, f32 t ) const {
        return v4_smoother_step( *this, b, t );
    }
    attr_header b32 cmp( const Vector4CPP& b ) const {
        return v4_cmp( *this, b );
    }

    attr_header f32 operator[]( usize idx ) const {
        return v[idx];
    }
    attr_header f32& operator[]( usize idx ) {
        return v[idx];
    }
    attr_header Vector4CPP& operator+=( const Vector4CPP& rhs ) {
        return *this = add(rhs);
    }
    attr_header Vector4CPP& operator-=( const Vector4CPP& rhs ) {
        return *this = sub(rhs);
    }
    attr_header Vector4CPP& operator*=( f32 rhs ) {
        return *this = mul(rhs);
    }
    attr_header Vector4CPP& operator/=( f32 rhs ) {
        return *this = div(rhs);
    }
    attr_header Vector4CPP operator-(void) {
        return neg();
    }
};
attr_header Vector4 operator+( const Vector4& lhs, const Vector4& rhs ) {
    return v4_add( lhs, rhs );
}
attr_header Vector4 operator-( const Vector4& lhs, const Vector4& rhs ) {
    return v4_sub( lhs, rhs );
}
attr_header Vector4 operator*( const Vector4& lhs, f32 rhs ) {
    return v4_mul( lhs, rhs );
}
attr_header Vector4 operator*( f32 lhs, const Vector4& rhs ) {
    return v4_mul( rhs, lhs );
}
attr_header Vector4 operator/( const Vector4& lhs, f32 rhs ) {
    return v4_div( lhs, rhs );
}
attr_header b32 operator==( const Vector4& a, const Vector4& b ) {
    return v4_cmp( a, b );
}
attr_header b32 operator!=( const Vector4& a, const Vector4& b ) {
    return !(a == b);
}

attr_header Vector4CPP add( const Vector4CPP& lhs, const Vector4CPP& rhs ) {
    return lhs.add(rhs);
}
attr_header Vector4CPP sub( const Vector4CPP& lhs, const Vector4CPP& rhs ) {
    return lhs.sub(rhs);
}
attr_header Vector4CPP mul( const Vector4CPP& lhs, f32 rhs ) {
    return lhs.mul(rhs);
}
attr_header Vector4CPP mul( f32 lhs, const Vector4CPP& rhs ) {
    return rhs.mul(lhs);
}
attr_header Vector4CPP div( const Vector4CPP& lhs, f32 rhs ) {
    return lhs.div(rhs);
}
attr_header b32 cmp( const Vector4CPP& a, const Vector4CPP& b ) {
    return a.cmp( b );
}
attr_header Vector4CPP neg( const Vector4CPP& v ) {
    return v.neg();
}
attr_header f32 hadd( const Vector4CPP& v ) {
    return v.hadd();
}
attr_header f32 hmul( const Vector4CPP& v ) {
    return v.hmul();
}
attr_header Vector4CPP hadamard( const Vector4CPP& lhs, const Vector4CPP& rhs ) {
    return lhs.hadamard(rhs);
}
attr_header f32 dot( const Vector4CPP& lhs, const Vector4CPP& rhs ) {
    return lhs.dot(rhs);
}
attr_header f32 max( const Vector4CPP& v ) {
    return v.max();
}
attr_header f32 min( const Vector4CPP& v ) {
    return v.min();
}
attr_header f32 sqrmag( const Vector4CPP& v ) {
    return v.sqrmag();
}
attr_header f32 mag( const Vector4CPP& v ) {
    return v.mag();
}
attr_header Vector4CPP normalize( const Vector4CPP& v ) {
    return v.normalize();
}
attr_header Vector4CPP clamp_mag( const Vector4CPP& v, f32 min_, f32 max_ ) {
    return v.clamp_mag( min_, max_ );
}
attr_header Vector4CPP lerp( const Vector4CPP& a, const Vector4CPP& b, f32 t ) {
    return a.lerp( b, t );
}
attr_header Vector4CPP smooth_step( const Vector4CPP& a, const Vector4CPP& b, f32 t ) {
    return a.smooth_step( b, t );
}
attr_header Vector4CPP smoother_step(
    const Vector4CPP& a, const Vector4CPP& b, f32 t
) {
    return a.smoother_step( b, t );
}

struct IVector4CPP : public IVector4 {
    attr_header IVector4CPP() : IVector4{ .x=0,.y=0,.z=0,.w=0 } {}
    attr_header IVector4CPP( i32 x, i32 y, i32 z, i32 w ) :
        IVector4{ .x=x,.y=y,.z=z,.w=w } {}
    attr_header IVector4CPP( const IVector4& v ) : IVector4CPP( v.x, v.y, v.z, v.w ) {}
    attr_header explicit IVector4CPP( i32 s ) : IVector4CPP( s, s, s, s ) {}
    attr_header explicit IVector4CPP( const i32 a[4] ) :
        IVector4CPP( a[0], a[1], a[2], a[3] ) {}
    attr_header explicit IVector4CPP( const Vector4& v ) :
        IVector4CPP( v.x, v.y, v.z, v.w ) {}

    attr_header static IVector4CPP zero(void) {
        return IVEC4_ZERO;
    }
    attr_header static IVector4CPP one(void) {
        return IVEC4_ONE;
    }

    attr_header static IVector4CPP from_array( const i32 a[4] ) {
        return IVector4CPP( a );
    }

    attr_header void to_array( i32 out_array[4] ) {
        iv4_to_array( *this, out_array );
    }
    attr_header IVector4CPP add( const IVector4CPP& rhs ) const {
        return iv4_add( *this, rhs );
    }
    attr_header IVector4CPP sub( const IVector4CPP& rhs ) const {
        return iv4_sub( *this, rhs );
    }
    attr_header IVector4CPP mul( i32 rhs ) const {
        return iv4_mul( *this, rhs );
    }
    attr_header IVector4CPP div( i32 rhs ) const {
        return iv4_div( *this, rhs );
    }
    attr_header IVector4CPP neg(void) const {
        return iv4_neg( *this );
    }
    attr_header IVector4CPP rotl(void) const {
        return iv4_rotl( *this );
    }
    attr_header IVector4CPP rotr(void) const {
        return iv4_rotr( *this );
    }
    attr_header i32 hadd(void) const {
        return iv4_hadd( *this );
    }
    attr_header i32 hmul(void) const {
        return iv4_hmul( *this );
    }
    attr_header IVector4CPP hadamard( const IVector4CPP& rhs ) const {
        return iv4_hadamard( *this, rhs );
    }
    attr_header f32 dot( const IVector4CPP& rhs ) const {
        return iv4_dot( *this, rhs );
    }
    attr_header f32 sqrmag(void) const {
        return iv4_sqrmag( *this );
    }
    attr_header f32 mag(void) const {
        return iv4_mag( *this );
    }
    attr_header b32 cmp( const IVector4CPP& b ) const {
        return iv4_cmp( *this, b );
    }

    attr_header i32 operator[]( usize idx ) const {
        return v[idx];
    }
    attr_header i32& operator[]( usize idx ) {
        return v[idx];
    }
    attr_header IVector4CPP& operator+=( const IVector4CPP& rhs ) {
        return *this = add(rhs);
    }
    attr_header IVector4CPP& operator-=( const IVector4CPP& rhs ) {
        return *this = sub(rhs);
    }
    attr_header IVector4CPP& operator*=( i32 rhs ) {
        return *this = mul(rhs);
    }
    attr_header IVector4CPP& operator/=( i32 rhs ) {
        return *this = div(rhs);
    }
    attr_header IVector4CPP operator-(void) {
        return neg();
    }
};
attr_header IVector4 operator+( const IVector4& lhs, const IVector4& rhs ) {
    return iv4_add( lhs, rhs );
}
attr_header IVector4 operator-( const IVector4& lhs, const IVector4& rhs ) {
    return iv4_sub( lhs, rhs );
}
attr_header IVector4 operator*( const IVector4& lhs, i32 rhs ) {
    return iv4_mul( lhs, rhs );
}
attr_header IVector4 operator*( i32 lhs, const IVector4& rhs ) {
    return iv4_mul( rhs, lhs );
}
attr_header IVector4 operator/( const IVector4& lhs, i32 rhs ) {
    return iv4_div( lhs, rhs );
}
attr_header b32 operator==( const IVector4& a, const IVector4& b ) {
    return iv4_cmp( a, b );
}
attr_header b32 operator!=( const IVector4& a, const IVector4& b ) {
    return !(a == b);
}

attr_header IVector4CPP add( const IVector4CPP& lhs, const IVector4CPP& rhs ) {
    return lhs.add(rhs);
}
attr_header IVector4CPP sub( const IVector4CPP& lhs, const IVector4CPP& rhs ) {
    return lhs.sub(rhs);
}
attr_header IVector4CPP mul( const IVector4CPP& lhs, i32 rhs ) {
    return lhs.mul(rhs);
}
attr_header IVector4CPP mul( i32 lhs, const IVector4CPP& rhs ) {
    return rhs.mul(lhs);
}
attr_header IVector4CPP div( const IVector4CPP& lhs, i32 rhs ) {
    return lhs.div(rhs);
}
attr_header b32 cmp( const IVector4CPP& a, const IVector4CPP& b ) {
    return a.cmp( b );
}
attr_header IVector4CPP neg( const IVector4CPP& v ) {
    return v.neg();
}
attr_header i32 hadd( const IVector4CPP& v ) {
    return v.hadd();
}
attr_header i32 hmul( const IVector4CPP& v ) {
    return v.hmul();
}
attr_header IVector4CPP hadamard( const IVector4CPP& lhs, const IVector4CPP& rhs ) {
    return lhs.hadamard(rhs);
}
attr_header f32 dot( const IVector4CPP& lhs, const IVector4CPP& rhs ) {
    return lhs.dot(rhs);
}
attr_header f32 sqrmag( const IVector4CPP& v ) {
    return v.sqrmag();
}
attr_header f32 mag( const IVector4CPP& v ) {
    return v.mag();
}

#endif /* header guard */
