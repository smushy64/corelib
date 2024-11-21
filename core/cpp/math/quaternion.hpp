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

struct AngleAxisCPP {
    union {
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
            };
        };
        struct AngleAxis_ pod;
    };

    attr_always_inline attr_header
    AngleAxisCPP() : angle(0), axis() {};
    attr_always_inline attr_header
    AngleAxisCPP( f32 angle, f32 x, f32 y, f32 z ) :
        angle(angle), axis(x, y, z) {};
    attr_always_inline attr_header
    AngleAxisCPP( f32 angle, Vector3CPP axis ) : angle(angle), axis(axis) {};
    attr_always_inline attr_header
    AngleAxisCPP( const struct AngleAxis_& aa ) : AngleAxisCPP( aa.angle, aa.axis ) {}

    attr_always_inline attr_header
    operator AngleAxis_() const {
        return *(struct AngleAxis_*)this;
    }
};

struct QuaternionCPP {
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

        struct Quaternion pod;
    };

    attr_always_inline attr_header
    QuaternionCPP() : w(0), x(0), y(0), z(0) {}
    attr_always_inline attr_header
    QuaternionCPP( const struct Quaternion& q ) : QuaternionCPP( q.w, q.x, q.y, q.z ) {}
    attr_always_inline attr_header
    explicit QuaternionCPP( f32 w, f32 x, f32 y, f32 z ) : w(w), x(x), y(y), z(z) {}
    attr_always_inline attr_header
    explicit QuaternionCPP( f32 real, Vector3CPP imaginary ) :
        QuaternionCPP( real, imaginary.x, imaginary.y, imaginary.z ) {}

    attr_always_inline attr_header
    operator Quaternion() const {
        return *(struct Quaternion*)this;
    }

    attr_always_inline attr_header static
    QuaternionCPP zero() {
        return QuaternionCPP();
    }
    attr_always_inline attr_header static
    QuaternionCPP identity() {
        return QuaternionCPP( 1.0, 0.0, 0.0, 0.0 );
    }

    attr_always_inline attr_header static
    QuaternionCPP from_array( const f32 array[4] ) {
        return *(QuaternionCPP*)array;
    }
    attr_always_inline attr_header static
    QuaternionCPP from_angle_axis( AngleAxisCPP angle_axis ) {
        return quat_from_angle_axis( angle_axis.pod );
    }
    attr_always_inline attr_header static
    QuaternionCPP from_angle_axis( f32 angle, f32 x, f32 y, f32 z ) {
        return from_angle_axis( AngleAxisCPP( angle, x, y, z ) );
    }
    attr_always_inline attr_header static
    QuaternionCPP from_angle_axis( f32 angle, Vector3CPP axis ) {
        return from_angle_axis( AngleAxisCPP( angle, axis ) );
    }
    attr_always_inline attr_header static
    QuaternionCPP from_euler( f32 x, f32 y, f32 z ) {
        return quat_from_euler( x, y, z );
    }
    attr_always_inline attr_header static
    QuaternionCPP from_euler( Vector3CPP euler ) {
        return from_euler( euler.x, euler.y, euler.z );
    }

    attr_always_inline attr_header
    void to_array( f32 out_array[4] ) const {
        out_array[0] = array[0];
        out_array[1] = array[1];
        out_array[2] = array[2];
        out_array[3] = array[3];
    }
    attr_always_inline attr_header
    Vector3CPP to_euler() const {
        return quat_to_euler( pod );
    }
    attr_always_inline attr_header
    AngleAxisCPP to_angle_axis() const {
        return quat_to_angle_axis( pod );
    }

    attr_always_inline attr_header
    f32 operator[]( usize idx ) const {
        return array[idx];
    }
    attr_always_inline attr_header
    f32& operator[]( usize idx ) {
        return array[idx];
    }
};
attr_always_inline
attr_header QuaternionCPP add( QuaternionCPP lhs, QuaternionCPP rhs ) {
    return quat_add( lhs.pod, rhs.pod );
}
attr_always_inline
attr_header QuaternionCPP sub( QuaternionCPP lhs, QuaternionCPP rhs ) {
    return quat_sub( lhs.pod, rhs.pod );
}
attr_always_inline
attr_header QuaternionCPP mul( QuaternionCPP lhs, f32 rhs ) {
    return quat_mul( lhs.pod, rhs );
}
attr_always_inline
attr_header QuaternionCPP mul( f32 lhs, QuaternionCPP rhs ) {
    return quat_mul( rhs.pod, lhs );
}
attr_always_inline
attr_header QuaternionCPP mul( QuaternionCPP lhs, QuaternionCPP rhs ) {
    return quat_mul_quat( lhs.pod, rhs.pod );
}
attr_always_inline
attr_header Vector3CPP mul( QuaternionCPP lhs, Vector3CPP rhs ) {
    return quat_mul_vec3( lhs.pod, rhs.pod );
}
attr_always_inline
attr_header QuaternionCPP div( QuaternionCPP lhs, f32 rhs ) {
    return quat_div( lhs.pod, rhs );
}
attr_always_inline
attr_header QuaternionCPP neg( QuaternionCPP q ) {
    return quat_neg( q.pod );
}
attr_always_inline
attr_header f32 length_sqr( QuaternionCPP q ) {
    return quat_length_sqr( q.pod );
}
attr_always_inline
attr_header f32 length( QuaternionCPP q ) {
    return quat_length( q.pod );
}
attr_always_inline
attr_header QuaternionCPP normalize( QuaternionCPP q ) {
    return quat_normalize( q.pod );
}
attr_always_inline
attr_header f32 dot( QuaternionCPP lhs, QuaternionCPP rhs ) {
    return quat_dot( lhs.pod, rhs.pod );
}
attr_always_inline
attr_header QuaternionCPP conjugate( QuaternionCPP q ) {
    return quat_conjugate( q.pod );
}
attr_always_inline
attr_header QuaternionCPP inverse( QuaternionCPP q ) {
    return quat_inverse( q.pod );
}
attr_always_inline
attr_header b32 inverse_checked( QuaternionCPP q, QuaternionCPP& out_inverse ) {
    return quat_inverse_checked( q.pod, &out_inverse.pod );
}
attr_always_inline
attr_header QuaternionCPP lerp( QuaternionCPP a, QuaternionCPP b, f32 t ) {
    return quat_lerp( a.pod, b.pod, t );
}
attr_always_inline
attr_header QuaternionCPP mix( QuaternionCPP a, QuaternionCPP b, f32 t ) {
    return lerp( a, b, t );
}
attr_always_inline
attr_header QuaternionCPP slerp( QuaternionCPP a, QuaternionCPP b, f32 t ) {
    return quat_slerp( a.pod, b.pod, t );
}
attr_always_inline
attr_header b32 cmp( QuaternionCPP a, QuaternionCPP b ) {
    return quat_cmp( a.pod, b.pod );
}
attr_always_inline
attr_header QuaternionCPP& operator+=( QuaternionCPP& lhs, QuaternionCPP rhs ) {
    return lhs = add( lhs, rhs );
}
attr_always_inline
attr_header QuaternionCPP& operator-=( QuaternionCPP& lhs, QuaternionCPP rhs ) {
    return lhs = sub( lhs, rhs );
}
attr_always_inline
attr_header QuaternionCPP& operator*=( QuaternionCPP& lhs, f32 rhs ) {
    return lhs = mul( lhs, rhs );
}
attr_always_inline
attr_header QuaternionCPP& operator/=( QuaternionCPP& lhs, f32 rhs ) {
    return lhs = div( lhs, rhs );
}
attr_always_inline
attr_header QuaternionCPP operator-( QuaternionCPP q ) {
    return neg( q );
}
attr_always_inline
attr_header QuaternionCPP operator+( QuaternionCPP lhs, QuaternionCPP rhs ) {
    return add( lhs, rhs );
}
attr_always_inline
attr_header QuaternionCPP operator-( QuaternionCPP lhs, QuaternionCPP rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline
attr_header QuaternionCPP operator*( QuaternionCPP lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline
attr_header QuaternionCPP operator*( f32 lhs, QuaternionCPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline
attr_header QuaternionCPP operator*( QuaternionCPP lhs, QuaternionCPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline
attr_header Vector3CPP operator*( QuaternionCPP lhs, Vector3CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline
attr_header QuaternionCPP operator/( QuaternionCPP lhs, f32 rhs ) {
    return div( lhs, rhs );
}
attr_always_inline
attr_header b32 operator==( QuaternionCPP a, QuaternionCPP b ) {
    return cmp( a, b );
}
attr_always_inline
attr_header b32 operator!=( QuaternionCPP a, QuaternionCPP b ) {
    return !(a == b);
}

#endif /* header guard */
