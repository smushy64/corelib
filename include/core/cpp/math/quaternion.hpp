#if !defined(CORE_CPP_MATH_QUATERNION_HPP) && defined(__cplusplus)
#define CORE_CPP_MATH_QUATERNION_HPP
/**
 * @file   quaternion.hpp
 * @brief  C++ Math: Quaternion.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 30, 2025
*/
#if !defined(CORE_MATH_QUATERNION_H)
    #include "core/math/quaternion.h"
#endif

/// @brief %Quaternion rotation.
struct QuaternionCPP {
    /// @brief Union of quaternion components.
    union {
        /// @brief W, X, Y, and Z components.
        struct {
            /// @brief W component.
            union {
                /// @brief W component.
                f32 w;
                /// @brief A component (W component).
                f32 a;
            };
            /// @brief X, Y and Z components.
            union {
                /// @brief X, Y and Z components.
                struct {
                    /// @brief X component.
                    f32 x;
                    /// @brief Y component.
                    f32 y;
                    /// @brief Z component.
                    f32 z;
                };
                /// @brief B, C and D components. (X, Y and Z components).
                struct {
                    /// @brief B component (X component).
                    f32 b;
                    /// @brief C component (Y component).
                    f32 c;
                    /// @brief D component (Z component).
                    f32 d;
                };
                /// @brief X, Y and Z components as #Vector3.
                Vector3CPP xyz;
                /// @brief B, C and D components as #Vector3.
                Vector3CPP bcd;
            };
        };
        /// @brief W, X, Y, and Z components as an array.
        f32 array[4];

        struct Quaternion wxyz;
    };

    /// @brief Create empty quaternion.
    constexpr QuaternionCPP();
    /// @brief Implicitly convert POD quaternion to C++ quaternion.
    constexpr QuaternionCPP( const Quaternion& __pod );
    /// @brief Create quaternion.
    /// @param w W component.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    constexpr explicit QuaternionCPP( f32 w, f32 x, f32 y, f32 z );
    /// @brief Create quaternion.
    /// @param w   W component.
    /// @param xyz X, Y and Z components.
    constexpr explicit QuaternionCPP( f32 w, Vector3CPP xyz );

    /// @brief Implicitly convert C++ quaternion to POD quaternion.
    constexpr operator Quaternion() const;
    /// @brief Index into quaternion.
    /// @param index Index.
    /// @return Component.
    constexpr f32 operator[]( usize index ) const;
    /// @brief Index into quaternion.
    /// @param index Index.
    /// @return Component.
    constexpr f32& operator[]( usize index );

    /// @brief Quaternion zero constant.
    /// @return Value.
    static constexpr QuaternionCPP zero();
    /// @brief Quaternion identity constant.
    /// @return Value.
    static constexpr QuaternionCPP identity();
};
/// @brief Angle Axis representation of 3D rotation.
struct AngleAxisCPP {
    /// @brief Union of Angle Axis and POD.
    union {
        /// @brief Angle and Axis.
        struct {
            /// @brief Angle in radians.
            f32 angle;
            /// @brief Union of axis components.
            union {
                /// @brief Axis X, Y and Z components.
                struct {
                    /// @brief X component of axis.
                    f32 x;
                    /// @brief Y component of axis.
                    f32 y;
                    /// @brief Z component of axis.
                    f32 z;
                };
                /// @brief X, Y and Z components of axis as #Vector3.
                Vector3CPP axis;
                /// @brief X, Y and Z components of axis as #Vector3.
                Vector3CPP xyz;
            };
        };
        __AngleAxisPOD __pod;
    };

    /// @brief Create empty angle axis rotation.
    constexpr AngleAxisCPP();
    /// @brief Implicitly convert POD angle axis to C++ angle axis.
    constexpr AngleAxisCPP( const __AngleAxisPOD& __pod );
    /// @brief Create angle axis.
    /// @param angle Angle in radians.
    /// @param x     Axis X component.
    /// @param y     Axis Y component.
    /// @param z     Axis Z component.
    constexpr explicit AngleAxisCPP( f32 angle, f32 x, f32 y, f32 z );
    /// @brief Create angle axis.
    /// @param angle Angle in radians.
    /// @param axis  Axis.
    constexpr explicit AngleAxisCPP( f32 angle, const Vector3& axis );

    /// @brief Implicitly convert C++ angle axis to POD angle axis.
    constexpr operator __AngleAxisPOD() const;
};

/// @brief %Quaternion rotation.
typedef QuaternionCPP quat;
/// @brief Angle Axis representation of 3D rotation.
typedef AngleAxisCPP  AngleAxis;

constexpr attr_always_inline attr_header attr_hot
QuaternionCPP::QuaternionCPP()
    : w(0.0f), x(0.0f), y(0.0f), z(0.0f) {}
constexpr attr_always_inline attr_header attr_hot
QuaternionCPP::QuaternionCPP( const Quaternion& __pod )
    : w(__pod.w), x(__pod.x), y(__pod.y), z(__pod.z) {}
constexpr attr_always_inline attr_header attr_hot
QuaternionCPP::QuaternionCPP( f32 w, f32 x, f32 y, f32 z )
    : w(w), x(x), y(y), z(z) {}
constexpr attr_always_inline attr_header attr_hot
QuaternionCPP::QuaternionCPP( f32 w, Vector3CPP xyz )
    : w(w), x(xyz.x), y(xyz.y), z(xyz.z) {}

constexpr attr_always_inline attr_header attr_hot
QuaternionCPP::operator Quaternion() const {
    return *(Quaternion*)this;
}
constexpr attr_always_inline attr_header attr_hot
f32 QuaternionCPP::operator[]( usize index ) const {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
f32& QuaternionCPP::operator[]( usize index ) {
    return this->array[index];
}

constexpr attr_always_inline attr_header attr_hot
QuaternionCPP QuaternionCPP::zero() {
    return QUAT_ZERO;
}
constexpr attr_always_inline attr_header attr_hot
QuaternionCPP QuaternionCPP::identity() {
    return QUAT_IDENTITY;
}

attr_always_inline attr_header attr_hot
quat add( quat lhs, quat rhs ) {
    return quat_add( lhs, rhs );
}
attr_always_inline attr_header attr_hot
quat sub( quat lhs, quat rhs ) {
    return quat_sub( lhs, rhs );
}
attr_always_inline attr_header attr_hot
quat mul( quat lhs, f32 rhs ) {
    return quat_mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
quat mul( f32 lhs, quat rhs ) {
    return quat_mul( rhs, lhs );
}
attr_always_inline attr_header attr_hot
quat mul( quat lhs, quat rhs ) {
    return quat_mul_quat( lhs, rhs );
}
attr_always_inline attr_header attr_hot
vec3 mul( quat lhs, vec3 rhs ) {
    return quat_mul_vec3( lhs, rhs );
}
attr_always_inline attr_header attr_hot
quat div( quat lhs, f32 rhs ) {
    return quat_div( lhs, rhs );
}
attr_always_inline attr_header attr_hot
quat neg( quat x ) {
    return quat_neg( x );
}

attr_always_inline attr_header attr_hot
quat operator+( quat lhs, quat rhs ) {
    return add( lhs, rhs );
}
attr_always_inline attr_header attr_hot
quat& operator+=( quat& lhs, quat rhs ) {
    return lhs = lhs + rhs;
}
attr_always_inline attr_header attr_hot
quat operator-( quat lhs, quat rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline attr_header attr_hot
quat& operator-=( quat& lhs, quat rhs ) {
    return lhs = lhs - rhs;
}
attr_always_inline attr_header attr_hot
quat operator*( quat lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
quat operator*( f32 lhs, quat rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
quat operator*( quat lhs, quat rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
vec3 operator*( quat lhs, vec3 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
quat& operator*=( quat& lhs, f32 rhs ) {
    return lhs = lhs * rhs;
}
attr_always_inline attr_header attr_hot
quat operator/( quat lhs, f32 rhs ) {
    return div( lhs, rhs );
}
attr_always_inline attr_header attr_hot
quat& operator/=( quat& lhs, f32 rhs ) {
    return lhs = lhs / rhs;
}
attr_always_inline attr_header attr_hot
quat operator-( quat x ) {
    return neg(x);
}

attr_always_inline attr_header attr_hot
f32 length_sqr( quat x ) {
    return quat_length_sqr( x );
}
attr_always_inline attr_header attr_hot
f32 length( quat x ) {
    return quat_length( x );
}
attr_always_inline attr_header attr_hot
quat normalize( quat x ) {
    return quat_normalize( x );
}
attr_always_inline attr_header attr_hot
f32 dot( quat lhs, quat rhs ) {
    return quat_dot( lhs, rhs );
}
attr_always_inline attr_header attr_hot
quat conjugate( quat x ) {
    return quat_conjugate( x );
}
attr_always_inline attr_header attr_hot
bool inverse_checked( quat x, quat* out_inverse ) {
    return quat_inverse_checked( x, (Quaternion*)out_inverse );
}
attr_always_inline attr_header attr_hot
quat inverse( quat x ) {
    return quat_inverse( x );
}
attr_always_inline attr_header attr_hot
quat lerp( quat a, quat b, f32 t ) {
    return quat_lerp( a, b, t );
}
attr_always_inline attr_header attr_hot
quat mix( quat a, quat b, f32 t ) {
    return lerp( a, b, t );
}
attr_always_inline attr_header attr_hot
quat slerp( quat a, quat b, f32 t ) {
    return quat_slerp( a, b, t );
}
attr_always_inline attr_header attr_hot
quat quat_from_euler( vec3 euler ) {
    return quat_from_euler_vec3( euler );
}
attr_always_inline attr_header attr_hot
quat quat_from_euler( f32 x, vec2 yz ) {
    return quat_from_euler( vec3( x, yz ) );
}
attr_always_inline attr_header attr_hot
quat quat_from_euler( vec2 xy, f32 z ) {
    return quat_from_euler( vec3( xy, z ) );
}
attr_always_inline attr_header attr_hot
bool cmp( quat a, quat b ) {
    return quat_cmp( a, b );
}

constexpr attr_always_inline attr_header attr_hot
AngleAxisCPP::AngleAxisCPP()
    : angle(0.0f), axis(vec3(0.0f)) {}
constexpr attr_always_inline attr_header attr_hot
AngleAxisCPP::AngleAxisCPP( const __AngleAxisPOD& __pod )
    : __pod(__pod) {}
constexpr attr_always_inline attr_header attr_hot
AngleAxisCPP::AngleAxisCPP( f32 angle, f32 x, f32 y, f32 z )
    : angle(angle), x(x), y(y), z(z) {}
constexpr attr_always_inline attr_header attr_hot
AngleAxisCPP::AngleAxisCPP( f32 angle, const Vector3& axis )
    : angle(angle), axis(axis) {}

constexpr attr_always_inline attr_header attr_hot
AngleAxisCPP::operator __AngleAxisPOD() const {
    return *(__AngleAxisPOD*)this;
}

#endif /* header guard */
