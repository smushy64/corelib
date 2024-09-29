#if !defined(CORE_CPP_MATH_QUATERNION_HPP)
#define CORE_CPP_MATH_QUATERNION_HPP
/**
 * @file   quaternion.hpp
 * @brief  C++ Quaternion.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/

struct QuaternionCPP;
struct AngleAxisCPP;

#if !defined(CORE_MATH_QUATERNION_H)
    #include "core/math/quaternion.h"
#endif

struct QuaternionCPP : public Quaternion {
    attr_header QuaternionCPP() : Quaternion{.w=0,.x=0,.y=0,.z=0} {}
    attr_header QuaternionCPP( f32 w, f32 x, f32 y, f32 z ) :
        Quaternion{.w=w,.x=x,.y=y,.z=z} {}
    attr_header QuaternionCPP( const Quaternion& q ) : Quaternion{q} {}
    attr_header explicit QuaternionCPP( f32 real, const Vector3& imaginary ) :
        Quaternion{.w=real,.xyz=imaginary} {}
    attr_header explicit QuaternionCPP( const AngleAxis_& angle_axis ) :
        Quaternion( q_from_angle_axis( angle_axis ) ) {}

    attr_header static QuaternionCPP zero(void) {
        return QUAT_ZERO;
    }
    attr_header static QuaternionCPP identity(void) {
        return QUAT_IDENTITY;
    }
    attr_header static QuaternionCPP from_array( const f32 a[4] ) {
        return q_from_array( a );
    }
    attr_header static QuaternionCPP from_angle_axis(
        const AngleAxis_& angle_axis
    ) {
        return q_from_angle_axis( angle_axis );
    }
    attr_header static QuaternionCPP from_euler( f32 x, f32 y, f32 z ) {
        return q_from_euler( x, y, z );
    }
    attr_header static QuaternionCPP from_euler( const Vector3& euler ) {
        return q_from_euler_v3( euler );
    }

    attr_header void to_array( f32 out_array[4] ) const {
        q_to_array( *this, out_array );
    }
    attr_header Vector3 to_euler(void) const {
        return q_to_euler( *this );
    }
    attr_header AngleAxis_ to_angle_axis(void) const {
        return q_to_angle_axis( *this );
    }

    attr_header QuaternionCPP add( const QuaternionCPP& rhs ) const {
        return q_add( *this, rhs );
    }
    attr_header QuaternionCPP sub( const QuaternionCPP& rhs ) const {
        return q_sub( *this, rhs );
    }
    attr_header QuaternionCPP mul( f32 rhs ) const {
        return q_mul( *this, rhs );
    }
    attr_header QuaternionCPP mul( const QuaternionCPP& rhs ) const {
        return q_mul_q( *this, rhs );
    }
    attr_header Vector3 mul( const Vector3& rhs ) const {
        return q_mul_v3( *this, rhs );
    }
    attr_header QuaternionCPP div( f32 rhs ) const {
        return q_div( *this, rhs );
    }
    attr_header QuaternionCPP neg(void) const {
        return q_neg( *this );
    }
    attr_header f32 sqrmag(void) const {
        return q_sqrmag( *this );
    }
    attr_header f32 mag(void) const {
        return q_mag( *this );
    }
    attr_header QuaternionCPP normalize(void) const {
        return q_normalize( *this );
    }
    attr_header f32 dot( const QuaternionCPP& rhs ) const {
        return q_dot( *this, rhs );
    }
    attr_header QuaternionCPP conjugate(void) const {
        return q_conjugate( *this );
    }
    attr_header QuaternionCPP inverse(void) const {
        return q_inverse( *this );
    }
    attr_header QuaternionCPP lerp( const QuaternionCPP& b, f32 t ) const {
        return q_lerp( *this, b, t );
    }
    attr_header QuaternionCPP slerp( const QuaternionCPP& b, f32 t ) const {
        return q_slerp( *this, b, t );
    }
    attr_header b32 cmp( const QuaternionCPP& b ) const {
        return q_cmp( *this, b );
    }

    attr_header QuaternionCPP& operator+=( const QuaternionCPP& rhs ) {
        return *this = add(rhs);
    }
    attr_header QuaternionCPP& operator-=( const QuaternionCPP& rhs ) {
        return *this = sub(rhs);
    }
    attr_header QuaternionCPP& operator*=( f32 rhs ) {
        return *this = mul(rhs);
    }
    attr_header QuaternionCPP& operator/=( f32 rhs ) {
        return *this = div(rhs);
    }
    attr_header QuaternionCPP operator-(void) const {
        return neg();
    }
};
attr_header QuaternionCPP operator+(
    const QuaternionCPP& lhs, const QuaternionCPP& rhs
) {
    return lhs.add(rhs);
}
attr_header QuaternionCPP operator-(
    const QuaternionCPP& lhs, const QuaternionCPP& rhs
) {
    return lhs.sub(rhs);
}
attr_header QuaternionCPP operator*(
    const QuaternionCPP& lhs, f32 rhs
) {
    return lhs.mul(rhs);
}
attr_header QuaternionCPP operator*(
    f32 lhs, const QuaternionCPP& rhs
) {
    return rhs.mul(lhs);
}
attr_header QuaternionCPP operator*(
    const QuaternionCPP& lhs, const QuaternionCPP& rhs
) {
    return lhs.mul(rhs);
}
attr_header Vector3 operator*(
    const QuaternionCPP& lhs, const Vector3& rhs
) {
    return lhs.mul(rhs);
}
attr_header QuaternionCPP operator/(
    const QuaternionCPP& lhs, f32 rhs
) {
    return lhs.div(rhs);
}
attr_header b32 operator==( const QuaternionCPP& a, const QuaternionCPP& b ) {
    return a.cmp(b);
}
attr_header b32 operator!=( const QuaternionCPP& a, const QuaternionCPP& b ) {
    return !(a == b);
}

attr_header QuaternionCPP add(
    const QuaternionCPP& lhs, const QuaternionCPP& rhs
) {
    return lhs.add(rhs);
}
attr_header QuaternionCPP sub(
    const QuaternionCPP& lhs, const QuaternionCPP& rhs
) {
    return lhs.sub(rhs);
}
attr_header QuaternionCPP mul(
    const QuaternionCPP& lhs, f32 rhs
) {
    return lhs.mul(rhs);
}
attr_header QuaternionCPP mul(
    f32 lhs, const QuaternionCPP& rhs
) {
    return rhs.mul(lhs);
}
attr_header QuaternionCPP mul(
    const QuaternionCPP& lhs, const QuaternionCPP& rhs
) {
    return lhs.mul(rhs);
}
attr_header Vector3 mul(
    const QuaternionCPP& lhs, const Vector3& rhs
) {
    return lhs.mul(rhs);
}
attr_header QuaternionCPP div(
    const QuaternionCPP& lhs, f32 rhs
) {
    return lhs.div(rhs);
}
attr_header b32 cmp( const QuaternionCPP& a, const QuaternionCPP& b ) {
    return a.cmp(b);
}
attr_header f32 sqrmag( const QuaternionCPP& q ) {
    return q.sqrmag();
}
attr_header f32 mag( const QuaternionCPP& q ) {
    return q.mag();
}
attr_header QuaternionCPP normalize( const QuaternionCPP& q ) {
    return q.normalize();
}
attr_header f32 dot( const QuaternionCPP& lhs, const QuaternionCPP& rhs ) {
    return lhs.dot( rhs );
}
attr_header QuaternionCPP conjugate( const QuaternionCPP& q ) {
    return q.conjugate();
}
attr_header QuaternionCPP inverse( const QuaternionCPP& q ) {
    return q.inverse();
}
attr_header QuaternionCPP lerp(
    const QuaternionCPP& a, const QuaternionCPP& b, f32 t 
) {
    return a.lerp(b, t);
}
attr_header QuaternionCPP slerp(
    const QuaternionCPP& a, const QuaternionCPP& b, f32 t 
) {
    return a.slerp(b, t);
}

struct AngleAxisCPP : public AngleAxis_ {
    attr_header AngleAxisCPP() : AngleAxis_{.angle=0,.x=0,.y=0,.z=0} {}
    attr_header AngleAxisCPP( f32 angle, f32 x, f32 y, f32 z ) :
        AngleAxis_{.angle=angle,.x=x,.y=y,.z=z} {}
    attr_header AngleAxisCPP( f32 angle, const Vector3& axis ) :
        AngleAxis_{.angle=angle,.axis=axis} {}

    attr_header QuaternionCPP to_quaternion(void) const {
        return QuaternionCPP( *this );
    }
};

#endif /* header guard */
