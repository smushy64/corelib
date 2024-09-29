#if !defined(CORE_CPP_MATH_VECTOR3_HPP)
#define CORE_CPP_MATH_VECTOR3_HPP
/**
 * @file   vector3.hpp
 * @brief  C++ Vector3.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/

struct Vector3CPP;
struct IVector3CPP;

#if !defined(CORE_MATH_VECTOR3_H)
    #include "core/math/vector3.h"
#endif

struct Vector3CPP : public Vector3 {
    attr_header Vector3CPP() : Vector3{ .x=0, .y=0, .z=0 } {}
    attr_header Vector3CPP( f32 x, f32 y, f32 z ) : Vector3{ .x=x,.y=y,.z=z } {}
    attr_header Vector3CPP( const Vector3& v ) : Vector3{v} {}
    attr_header explicit Vector3CPP( f32 s ) : Vector3CPP( s, s, s ) {}
    attr_header explicit Vector3CPP( const f32 a[3] ) :
        Vector3CPP( a[0], a[1], a[2] ) {}
    attr_header explicit Vector3CPP( const IVector3& v ) :
        Vector3CPP( v.x, v.y, v.z ) {}

    attr_header static Vector3CPP zero(void) {
        return VEC3_ZERO;
    }
    attr_header static Vector3CPP one(void) {
        return VEC3_ONE;
    }
    attr_header static Vector3CPP left(void) {
        return VEC3_LEFT;
    }
    attr_header static Vector3CPP right(void) {
        return VEC3_RIGHT;
    }
    attr_header static Vector3CPP up(void) {
        return VEC3_UP;
    }
    attr_header static Vector3CPP down(void) {
        return VEC3_DOWN;
    }
    attr_header static Vector3CPP forward(void) {
        return VEC3_FORWARD;
    }
    attr_header static Vector3CPP back(void) {
        return VEC3_BACK;
    }

    attr_header static Vector3CPP red(void) {
        return RGB_RED;
    }
    attr_header static Vector3CPP green(void) {
        return RGB_GREEN;
    }
    attr_header static Vector3CPP blue(void) {
        return RGB_BLUE;
    }
    attr_header static Vector3CPP yellow(void) {
        return RGB_YELLOW;
    }
    attr_header static Vector3CPP magenta(void) {
        return RGB_MAGENTA;
    }
    attr_header static Vector3CPP cyan(void) {
        return RGB_CYAN;
    }
    attr_header static Vector3CPP black(void) {
        return RGB_BLACK;
    }
    attr_header static Vector3CPP white(void) {
        return RGB_WHITE;
    }

    attr_header static Vector3CPP from_array( const f32 array[3] ) {
        return v3_from_array( array );
    }
    attr_header static Vector3CPP hsl_from_rgb( const Vector3& _rgb ) {
        return rgb_to_hsl( _rgb );
    }
    attr_header static Vector3CPP rgb_from_hsl( const Vector3& _hsl ) {
        return hsl_to_rgb( _hsl );
    }

    attr_header void to_array( f32 out_array[3] ) {
        out_array[0] = v[0];
        out_array[1] = v[1];
        out_array[2] = v[2];
    }
    attr_header Vector3CPP to_hsl(void) const {
        return rgb_to_hsl( *this );
    }
    attr_header Vector3CPP to_rgb(void) const {
        return hsl_to_rgb( *this );
    }

    attr_header Vector3CPP add( const Vector3CPP& rhs ) const {
        return v3_add( *this, rhs );
    }
    attr_header Vector3CPP sub( const Vector3CPP& rhs ) const {
        return v3_sub( *this, rhs );
    }
    attr_header Vector3CPP mul( f32 rhs ) const {
        return v3_mul( *this, rhs );
    }
    attr_header Vector3CPP div( f32 rhs ) const {
        return v3_div( *this, rhs );
    }
    attr_header Vector3CPP neg(void) const {
        return v3_neg( *this );
    }
    attr_header Vector3CPP rotl(void) const {
        return v3_rotl( *this );
    }
    attr_header Vector3CPP rotr(void) const {
        return v3_rotr( *this );
    }
    attr_header f32 hadd(void) const {
        return v3_hadd( *this );
    }
    attr_header f32 hmul(void) const {
        return v3_hmul( *this );
    }
    attr_header Vector3CPP hadamard( const Vector3CPP& rhs ) const {
        return v3_hadamard( *this, rhs );
    }
    attr_header Vector3CPP cross( const Vector3CPP& rhs ) const {
        return v3_cross( *this, rhs );
    }
    attr_header f32 dot( const Vector3CPP& rhs ) const {
        return v3_dot( *this, rhs );
    }
    attr_header f32 max(void) const {
        return v3_max( *this );
    }
    attr_header f32 mid(void) const {
        return v3_mid( *this );
    }
    attr_header f32 min(void) const {
        return v3_min( *this );
    }
    attr_header f32 sqrmag(void) const {
        return v3_sqrmag( *this );
    }
    attr_header f32 mag(void) const {
        return v3_mag( *this );
    }
    attr_header Vector3CPP normalize(void) const {
        return v3_normalize( *this );
    }
    attr_header Vector3CPP reflect( const Vector3CPP& normal ) const {
        return v3_reflect( *this, normal );
    }
    attr_header Vector3CPP clamp_mag( f32 min_, f32 max_ ) const {
        return v3_clamp_mag( *this, min_, max_ );
    }
    attr_header f32 angle( const Vector3CPP& b ) const {
        return v3_angle( *this, b );
    }
    attr_header Vector3CPP lerp( const Vector3CPP& b, f32 t ) const {
        return v3_lerp( *this, b, t );
    }
    attr_header Vector3CPP smooth_step( const Vector3CPP& b, f32 t ) const {
        return v3_smooth_step( *this, b, t );
    }
    attr_header Vector3CPP smoother_step( const Vector3CPP& b, f32 t ) const {
        return v3_smoother_step( *this, b, t );
    }
    attr_header b32 cmp( const Vector3CPP& b ) const {
        return v3_cmp( *this, b );
    }

    attr_header f32 operator[]( usize idx ) const {
        return v[idx];
    }
    attr_header f32& operator[]( usize idx ) {
        return v[idx];
    }
    attr_header Vector3CPP& operator+=( const Vector3CPP& rhs ) {
        return *this = add(rhs);
    }
    attr_header Vector3CPP& operator-=( const Vector3CPP& rhs ) {
        return *this = sub(rhs);
    }
    attr_header Vector3CPP& operator*=( f32 rhs ) {
        return *this = mul(rhs);
    }
    attr_header Vector3CPP& operator/=( f32 rhs ) {
        return *this = div(rhs);
    }
    attr_header Vector3CPP operator-(void) {
        return neg();
    }
};
attr_header Vector3 operator+( const Vector3& lhs, const Vector3& rhs ) {
    return v3_add( lhs, rhs );
}
attr_header Vector3 operator-( const Vector3& lhs, const Vector3& rhs ) {
    return v3_sub( lhs, rhs );
}
attr_header Vector3 operator*( const Vector3& lhs, f32 rhs ) {
    return v3_mul( lhs, rhs );
}
attr_header Vector3 operator*( f32 lhs, const Vector3& rhs ) {
    return v3_mul( rhs, lhs );
}
attr_header Vector3 operator/( const Vector3& lhs, f32 rhs ) {
    return v3_div( lhs, rhs );
}
attr_header b32 operator==( const Vector3& a, const Vector3& b ) {
    return v3_cmp( a, b );
}
attr_header b32 operator!=( const Vector3& a, const Vector3& b ) {
    return !(a == b);
}

attr_header Vector3CPP add( const Vector3CPP& lhs, const Vector3CPP& rhs ) {
    return lhs.add(rhs);
}
attr_header Vector3CPP sub( const Vector3CPP& lhs, const Vector3CPP& rhs ) {
    return lhs.sub(rhs);
}
attr_header Vector3CPP mul( const Vector3CPP& lhs, f32 rhs ) {
    return lhs.mul(rhs);
}
attr_header Vector3CPP mul( f32 lhs, const Vector3CPP& rhs ) {
    return rhs.mul(lhs);
}
attr_header Vector3CPP div( const Vector3CPP& lhs, f32 rhs ) {
    return lhs.div(rhs);
}
attr_header Vector3CPP neg( const Vector3CPP& v ) {
    return v.neg();
}
attr_header f32 hadd( const Vector3CPP& v ) {
    return v.hadd();
}
attr_header f32 hmul( const Vector3CPP& v ) {
    return v.hmul();
}
attr_header Vector3CPP hadamard( const Vector3CPP& lhs, const Vector3CPP& rhs ) {
    return lhs.hadamard(rhs);
}
attr_header Vector3CPP cross( const Vector3CPP& lhs, const Vector3CPP& rhs ) {
    return lhs.cross(rhs);
}
attr_header f32 dot( const Vector3CPP& lhs, const Vector3CPP& rhs ) {
    return lhs.dot(rhs);
}
attr_header f32 max( const Vector3CPP& v ) {
    return v.max();
}
attr_header f32 mid( const Vector3CPP& v ) {
    return v.mid();
}
attr_header f32 min( const Vector3CPP& v ) {
    return v.min();
}
attr_header f32 sqrmag( const Vector3CPP& v ) {
    return v.sqrmag();
}
attr_header f32 mag( const Vector3CPP& v ) {
    return v.mag();
}
attr_header Vector3CPP normalize( const Vector3CPP& v ) {
    return v.normalize();
}
attr_header Vector3CPP reflect(
    const Vector3CPP& direction, const Vector3CPP& normal
) {
    return direction.reflect( normal );
}
attr_header Vector3CPP clamp_mag( const Vector3CPP& v, f32 min_, f32 max_ ) {
    return v.clamp_mag( min_, max_ );
}
attr_header f32 angle( const Vector3CPP& a, const Vector3CPP& b ) {
    return a.angle( b );
}
attr_header Vector3CPP lerp( const Vector3CPP& a, const Vector3CPP& b, f32 t ) {
    return a.lerp( b, t );
}
attr_header Vector3CPP smooth_step( const Vector3CPP& a, const Vector3CPP& b, f32 t ) {
    return a.smooth_step( b, t );
}
attr_header Vector3CPP smoother_step(
    const Vector3CPP& a, const Vector3CPP& b, f32 t
) {
    return a.smoother_step( b, t );
}

struct IVector3CPP : public IVector3 {
    attr_header IVector3CPP() : IVector3{ .x=0, .y=0, .z=0 } {}
    attr_header IVector3CPP( i32 x, i32 y, i32 z ) : IVector3{ .x=x,.y=y,.z=z } {}
    attr_header IVector3CPP( const IVector3& v ) : IVector3{v} {}
    attr_header explicit IVector3CPP( i32 s ) : IVector3CPP( s, s, s ) {}
    attr_header explicit IVector3CPP( const i32 a[3] ) :
        IVector3CPP( a[0], a[1], a[2] ) {}
    attr_header explicit IVector3CPP( const Vector3& v ) :
        IVector3CPP( v.x, v.y, v.z ) {}

    attr_header static IVector3CPP zero(void) {
        return IVEC3_ZERO;
    }
    attr_header static IVector3CPP one(void) {
        return IVEC3_ONE;
    }
    attr_header static IVector3CPP left(void) {
        return IVEC3_LEFT;
    }
    attr_header static IVector3CPP right(void) {
        return IVEC3_RIGHT;
    }
    attr_header static IVector3CPP up(void) {
        return IVEC3_UP;
    }
    attr_header static IVector3CPP down(void) {
        return IVEC3_DOWN;
    }
    attr_header static IVector3CPP forward(void) {
        return IVEC3_FORWARD;
    }
    attr_header static IVector3CPP back(void) {
        return IVEC3_BACK;
    }

    attr_header static IVector3CPP from_array( const i32 a[3] ) {
        return IVector3CPP( a );
    }

    attr_header void to_array( i32 out_array[3] ) {
        iv3_to_array( *this, out_array );
    }
    attr_header IVector3CPP add( const IVector3CPP& rhs ) const {
        return iv3_add( *this, rhs );
    }
    attr_header IVector3CPP sub( const IVector3CPP& rhs ) const {
        return iv3_sub( *this, rhs );
    }
    attr_header IVector3CPP mul( i32 rhs ) const {
        return iv3_mul( *this, rhs );
    }
    attr_header IVector3CPP div( i32 rhs ) const {
        return iv3_div( *this, rhs );
    }
    attr_header IVector3CPP neg(void) const {
        return iv3_neg( *this );
    }
    attr_header IVector3CPP rotl(void) const {
        return iv3_rotl( *this );
    }
    attr_header IVector3CPP rotr(void) const {
        return iv3_rotr( *this );
    }
    attr_header i32 hadd(void) const {
        return iv3_hadd( *this );
    }
    attr_header i32 hmul(void) const {
        return iv3_hmul( *this );
    }
    attr_header IVector3CPP hadamard( const IVector3CPP& rhs ) const {
        return iv3_hadamard( *this, rhs );
    }
    attr_header f32 dot( const IVector3CPP& rhs ) const {
        return iv3_dot( *this, rhs );
    }
    attr_header f32 sqrmag(void) const {
        return iv3_sqrmag( *this );
    }
    attr_header f32 mag(void) const {
        return iv3_mag( *this );
    }
    attr_header b32 cmp( const IVector3CPP& b ) const {
        return iv3_cmp( *this, b );
    }

    attr_header i32 operator[]( usize idx ) const {
        return v[idx];
    }
    attr_header i32& operator[]( usize idx ) {
        return v[idx];
    }
    attr_header IVector3CPP& operator+=( const IVector3CPP& rhs ) {
        return *this = add(rhs);
    }
    attr_header IVector3CPP& operator-=( const IVector3CPP& rhs ) {
        return *this = sub(rhs);
    }
    attr_header IVector3CPP& operator*=( i32 rhs ) {
        return *this = mul(rhs);
    }
    attr_header IVector3CPP& operator/=( i32 rhs ) {
        return *this = div(rhs);
    }
    attr_header IVector3CPP operator-(void) const {
        return neg();
    }
};
attr_header IVector3 operator+( const IVector3& lhs, const IVector3& rhs ) {
    return iv3_add( lhs, rhs );
}
attr_header IVector3 operator-( const IVector3& lhs, const IVector3& rhs ) {
    return iv3_sub( lhs, rhs );
}
attr_header IVector3 operator*( const IVector3& lhs, i32 rhs ) {
    return iv3_mul( lhs, rhs );
}
attr_header IVector3 operator*( i32 lhs, const IVector3& rhs ) {
    return iv3_mul( rhs, lhs );
}
attr_header IVector3 operator/( const IVector3& lhs, i32 rhs ) {
    return iv3_div( lhs, rhs );
}
attr_header b32 operator==( const IVector3& a, const IVector3& b ) {
    return iv3_cmp( a, b );
}
attr_header b32 operator!=( const IVector3& a, const IVector3& b ) {
    return !(a == b);
}

attr_header IVector3CPP add( const IVector3CPP& lhs, const IVector3CPP& rhs ) {
    return lhs.add(rhs);
}
attr_header IVector3CPP sub( const IVector3CPP& lhs, const IVector3CPP& rhs ) {
    return lhs.sub(rhs);
}
attr_header IVector3CPP mul( const IVector3CPP& lhs, i32 rhs ) {
    return lhs.mul(rhs);
}
attr_header IVector3CPP mul( i32 lhs, const IVector3CPP& rhs ) {
    return rhs.mul(lhs);
}
attr_header IVector3CPP div( const IVector3CPP& lhs, i32 rhs ) {
    return lhs.div(rhs);
}
attr_header IVector3CPP neg( const IVector3CPP& v ) {
    return v.neg();
}
attr_header b32 cmp( const IVector3CPP& a, const IVector3CPP& b ) {
    return a.cmp(b);
}
attr_header i32 hadd( const IVector3CPP& v ) {
    return v.hadd();
}
attr_header i32 hmul( const IVector3CPP& v ) {
    return v.hmul();
}
attr_header IVector3CPP hadamard( const IVector3CPP& lhs, const IVector3CPP& rhs ) {
    return lhs.hadamard( rhs );
}
attr_header f32 dot( const IVector3CPP& lhs, const IVector3CPP& rhs ) {
    return lhs.dot(rhs);
}
attr_header f32 sqrmag( const IVector3CPP& v ) {
    return v.sqrmag();
}
attr_header f32 mag( const IVector3CPP& v ) {
    return v.mag();
}

#endif /* header guard */
