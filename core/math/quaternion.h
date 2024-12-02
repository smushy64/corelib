#if !defined(CORE_MATH_QUATERNION_H)
#define CORE_MATH_QUATERNION_H
/**
 * @file   quaternion.h
 * @brief  Quaternion definition.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 28, 2024
*/
#include "core/defines.h"
#include "core/types.h"
#include "core/attributes.h"
#include "core/constants.h"

#include "core/math/exponential.h"
#include "core/math/vector3.h"

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

/// @brief %Quaternion rotation.
struct Quaternion {
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
                struct Vector3 xyz;
                /// @brief B, C and D components as #Vector3.
                struct Vector3 bcd;
            };
        };
        /// @brief W, X, Y, and Z components as an array.
        f32 array[4];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief Quaternion rotation.
    typedef struct Quaternion quat;
#endif
/// @brief Angle Axis representation of 3D rotation.
struct AngleAxis_ {
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
        struct Vector3 axis;
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief Angle Axis Rotation
    typedef struct AngleAxis_ AngleAxis;
#endif

#if defined(CORE_DOXYGEN) && !defined(CORE_CPLUSPLUS)

/// @brief Construct a new Quaternion.
/// @param w Real component.
/// @param x, y, z Imaginary components.
/// @return Quaternion.
#define quat( w, x, y, z )

#else /* Doxygen */

#if defined(CORE_CPLUSPLUS)
    #define quat_new( w, x, y, z ) Quaternion{ .array={ w, x, y, z } }
#else
    #define quat_new( w, x, y, z ) (struct Quaternion){ .array={ w, x, y, z } }
    #define quat(...) quat_new(__VA_ARGS__)
#endif

#endif /* Doxygen */

/// @brief Quaternion zero constant.
#define QUAT_ZERO     quat_new( 0.0f, 0.0f, 0.0f, 0.0f )
/// @brief Quaternion identity constant.
#define QUAT_IDENTITY quat_new( 1.0f, 0.0f, 0.0f, 0.0f )

/// @brief Create Quaternion from array.
/// @param array Array, must have at least 4 floats.
/// @return Quaternion.
attr_always_inline attr_header
struct Quaternion quat_from_array( const f32 array[4] ) {
    return quat_new( array[0], array[1], array[2], array[3] );
}
/// @brief Fill array with components from Quaternion.
/// @param v Vector to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 4 floats.
attr_always_inline attr_header
void quat_to_array( struct Quaternion v, f32* out_array ) {
    out_array[0] = v.array[0]; out_array[1] = v.array[1];
    out_array[2] = v.array[2]; out_array[3] = v.array[3];
}
/// @brief Add two quaternions.
/// @param lhs, rhs Quaternions to add.
/// @return Result of addition.
attr_always_inline attr_header
struct Quaternion quat_add(
    struct Quaternion lhs, struct Quaternion rhs
) {
    return quat_new(
        lhs.w + rhs.w, lhs.x + rhs.x,
        lhs.y + rhs.y, lhs.z + rhs.z );
}
/// @brief Subtract quaternion from quaternion.
/// @param lhs, rhs Quaternions to subtract.
/// @return Result of subtraction.
attr_always_inline attr_header
struct Quaternion quat_sub(
    struct Quaternion lhs, struct Quaternion rhs
) {
    return quat_new(
        lhs.w - rhs.w, lhs.x - rhs.x,
        lhs.y - rhs.y, lhs.z - rhs.z );
}
/// @brief Multiply quaternion components.
/// @param lhs Quaternion to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline attr_header
struct Quaternion quat_mul( struct Quaternion lhs, f32 rhs ) {
    return quat_new(
        lhs.w * rhs, lhs.x * rhs,
        lhs.y * rhs, lhs.z * rhs );
}
/// @brief Divide quaternion components.
/// @param lhs Quaternion to multiply.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline attr_header
struct Quaternion quat_div( struct Quaternion lhs, f32 rhs ) {
    return quat_new(
        lhs.w / rhs, lhs.x / rhs,
        lhs.y / rhs, lhs.z / rhs );
}
/// @brief Multiply quaternion by quaternion.
/// @param lhs, rhs Quaternions to multiply.
/// @return Result of quaternion multiplication.
/// @note lhs X rhs and rhs X lhs give different results!
attr_core_api struct Quaternion quat_mul_quat(
    struct Quaternion lhs, struct Quaternion rhs );
/// @brief Multiply Vector3 by Quaternion.
/// @param lhs Quaternion to multiply.
/// @param rhs Vector3 to multiply
/// @return Result of quaternion vector multiplication.
attr_core_api struct Vector3 quat_mul_vec3(
    struct Quaternion lhs, struct Vector3 rhs );
/// @brief Negate quaternion.
/// @param x Quaternion to negate.
/// @return Negated quaternion.
attr_always_inline attr_header
struct Quaternion quat_neg( struct Quaternion x ) {
    return quat_new( -x.w, -x.x, -x.y, -x.z );
}
/// @brief Calculate square magnitude of quaternion.
/// @param q Quaternion to get square magnitude of.
/// @return Square magnitude of quaternion.
attr_always_inline attr_header
f32 quat_length_sqr( struct Quaternion q ) {
    return (q.w * q.w) + (q.x * q.x) + (q.y * q.y) + (q.z * q.z);
}
/// @brief Calculate magnitude of Quaternion.
/// @param q Quaternion to get magnitude of.
/// @return Magnitude of quaternion.
attr_always_inline attr_header
f32 quat_length( struct Quaternion q ) {
    return f32_sqrt( quat_length_sqr( q ) );
}
/// @brief Normalize a Quaternion.
/// @param x Quaternion to normalize.
/// @return Normalized quaternion.
attr_always_inline attr_header
struct Quaternion quat_normalize( struct Quaternion x ) {
    f32 m = quat_length( x );
    if( m == 0.0f ) {
        return QUAT_ZERO;
    } else {
        return quat_div( x, m );
    }
}
/// @brief Inner product of two quaternions.
/// @param lhs, rhs Quaternions to take inner product of.
/// @return Inner product.
attr_always_inline attr_header
f32 quat_dot( struct Quaternion lhs, struct Quaternion rhs ) {
    return (lhs.w * rhs.w) + (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}
/// @brief Calculate conjugate of quaternion.
/// @param q Quaternion.
/// @return Conjugate of quaternion.
attr_always_inline attr_header
struct Quaternion quat_conjugate( struct Quaternion q ) {
    return quat_new( q.w, -q.x, -q.y, -q.z );
}
/// @brief Invert quaternion.
/// @param q Quaternion.
/// @return Inverted quaternion.
attr_always_inline attr_header
struct Quaternion quat_inverse( struct Quaternion q ) {
    return quat_div( quat_conjugate( q ), quat_length_sqr( q ) );
}
/// @brief Invert quaternion.
/// @param      q           Quaternion.
/// @param[out] out_inverse Inverse quaternion.
/// @return True if quaternion could be inverted.
attr_always_inline attr_header
b32 quat_inverse_checked( struct Quaternion q, struct Quaternion* out_inverse ) {
    f32 len_sqr = quat_length_sqr( q );
    if( len_sqr == 0.0f ) {
        return false;
    }
    *out_inverse = quat_div( quat_conjugate( q ), len_sqr );
    return true;
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Interpolated quaternion.
attr_always_inline attr_header
struct Quaternion quat_lerp(
    struct Quaternion a, struct Quaternion b, f32 t
) {
    return quat_add( quat_mul( a, 1.0f - t ), quat_mul( b, t ) );
}
/// @brief Linearly interpolate from a to b.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Interpolated quaternion.
attr_always_inline attr_header
struct Quaternion quat_mix(
    struct Quaternion a, struct Quaternion b, f32 t
) {
    return quat_lerp( a, b, t );
}
/// @brief Spherical interpolation.
/// @param a, b Range to interpolate within.
/// @param t Where to interpolate to.
/// @return Interpolated quaternion.
attr_core_api
struct Quaternion quat_slerp(
    struct Quaternion a, struct Quaternion b, f32 t );
/// @brief Convert AngleAxis to Quaternion.
/// @param angle_axis Angle axis.
/// @return Quaternion representation of angle axis rotation.
attr_core_api struct Quaternion quat_from_angle_axis( struct AngleAxis_ angle_axis );
/// @brief Convert euler angles rotation to quaternion rotation.
/// @param x, y, z Euler angles.
/// @return Euler angle rotation as quaternion.
attr_core_api struct Quaternion quat_from_euler( f32 x, f32 y, f32 z );
/// @brief Convert euler angles rotation to quaternion rotation.
/// @param euler_angles Euler angles.
/// @return Euler angle rotation as quaternion.
attr_always_inline attr_header
struct Quaternion quat_from_euler_vec3( struct Vector3 euler_angles ) {
    return quat_from_euler( euler_angles.x, euler_angles.y, euler_angles.z );
}
/// @brief Convert quaternion rotation to euler angles.
/// @param q Quaternion to convert.
/// @return Quaternion rotation as euler angles.
attr_core_api struct Vector3 quat_to_euler( struct Quaternion q );
/// @brief Convert quaternion rotation to angle axis rotation.
/// @param q Quaternion to convert.
/// @return Quaternion rotation as angle axis rotation.
attr_core_api struct AngleAxis_ quat_to_angle_axis( struct Quaternion q );
/// @brief Compare two quaternions for equality.
/// @param a, b Quaternions to compare.
/// @return True if the square magnitude of a - b is < F32_EPSILON.
/// @see #F32_EPSILON
attr_always_inline attr_header
b32 quat_cmp( struct Quaternion a, struct Quaternion b ) {
    return quat_length_sqr( quat_sub( a, b ) ) < F32_EPSILON;
}

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#if defined(CORE_CPLUSPLUS)
    #if !defined(CORE_CPP_MATH_QUATERNION_HPP)
        #include "core/cpp/math/quaternion.hpp"
    #endif
    typedef QuaternionCPP quat;
    typedef AngleAxisCPP  AngleAxis;
#endif

#endif /* header guard */
