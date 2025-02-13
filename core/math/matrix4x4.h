#if !defined(CORE_MATH_MATRIX4X4_H)
#define CORE_MATH_MATRIX4X4_H
/**
 * @file   matrix4x4.h
 * @brief  Matrix4x4 definition.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 28, 2024
*/
#include "core/defines.h"
#include "core/types.h"
#include "core/attributes.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/math/vector4.h"
#include "core/math/matrix3x3.h"

// NOTE(alicia): forward declaration

struct Quaternion;

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

/// @brief Column-major 4x4 matrix.
struct Matrix4x4 {
    /// @brief Union of matrix components.
    union {
        /// @brief Columns 0, 1, 2 and 3.
        struct {
            /// @brief Column 0.
            union {
                /// @brief Column 0.
                struct {
                    /// @brief Column 0, Row 0.
                    f32 m00;
                    /// @brief Column 0, Row 1.
                    f32 m01;
                    /// @brief Column 0, Row 2.
                    f32 m02;
                    /// @brief Column 0, Row 3.
                    f32 m03;
                };
                /// @brief Column 0 as a #Vector4.
                struct Vector4 col0;
            };
            /// @brief Column 1.
            union {
                /// @brief Column 1.
                struct {
                    /// @brief Column 1, Row 0.
                    f32 m10;
                    /// @brief Column 1, Row 1.
                    f32 m11;
                    /// @brief Column 1, Row 2.
                    f32 m12;
                    /// @brief Column 1, Row 3.
                    f32 m13;
                };
                /// @brief Column 1 as a #Vector4.
                struct Vector4 col1;
            };
            /// @brief Column 2.
            union {
                /// @brief Column 2.
                struct {
                    /// @brief Column 2, Row 0.
                    f32 m20;
                    /// @brief Column 2, Row 1.
                    f32 m21;
                    /// @brief Column 2, Row 2.
                    f32 m22;
                    /// @brief Column 2, Row 3.
                    f32 m23;
                };
                /// @brief Column 2 as a #Vector4.
                struct Vector4 col2;
            };
            /// @brief Column 3.
            union {
                /// @brief Column 3.
                struct {
                    /// @brief Column 3, Row 0.
                    f32 m30;
                    /// @brief Column 3, Row 1.
                    f32 m31;
                    /// @brief Column 3, Row 2.
                    f32 m32;
                    /// @brief Column 3, Row 3.
                    f32 m33;
                };
                /// @brief Column 3 as a #Vector4.
                struct Vector4 col3;
            };
        };
        /// @brief Columns as an array of vectors.
        struct Vector4 col[4];
        /// @brief Matrix components as an array.
        f32  array[16];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief Column-major 4x4 matrix.
    typedef struct Matrix4x4 mat4x4;
    /// @brief Column-major 4x4 matrix.
    typedef mat4x4 mat4;
#endif

#if defined(CORE_DOXYGEN) && !defined(CORE_CPLUSPLUS)
    /// @brief Construct a new 4x4 Matrix.
    /// @param m00, m01, m02, m03 First column components.
    /// @param m10, m11, m12, m13 Second column components.
    /// @param m20, m21, m22, m23 Third column components.
    /// @param m30, m31, m32, m33 Fourth column components.
    /// @return Matrix4x4.
    #define mat4(\
        m00, m01, m02, m03,\
        m10, m11, m12, m13,\
        m20, m21, m22, m23,\
        m30, m31, m32, m33 )
#else /* Doxygen */

#if defined(CORE_CPLUSPLUS)
    #define mat4_new(\
        m00, m01, m02, m03,\
        m10, m11, m12, m13,\
        m20, m21, m22, m23,\
        m30, m31, m32, m33 )\
        Matrix4x4{ .array={\
            (m00), (m01), (m02), (m03),\
            (m10), (m11), (m12), (m13),\
            (m20), (m21), (m22), (m23),\
            (m30), (m31), (m32), (m33),\
        } }
#else
    #define mat4_new(\
        m00, m01, m02, m03,\
        m10, m11, m12, m13,\
        m20, m21, m22, m23,\
        m30, m31, m32, m33 )\
        (struct Matrix4x4){ .array={\
            (m00), (m01), (m02), (m03),\
            (m10), (m11), (m12), (m13),\
            (m20), (m21), (m22), (m23),\
            (m30), (m31), (m32), (m33),\
        } }
    #define mat4(...) mat4_new(__VA_ARGS__)
#endif

#endif /* Doxygen */

/// @brief Matrix4x4 zero constant.
#define MAT4_ZERO mat4_new(\
        0.0f, 0.0f, 0.0f, 0.0f,\
        0.0f, 0.0f, 0.0f, 0.0f,\
        0.0f, 0.0f, 0.0f, 0.0f,\
        0.0f, 0.0f, 0.0f, 0.0f  )
/// @brief Matrix4x4 identity constant.
#define MAT4_IDENTITY mat4_new(\
        1.0f, 0.0f, 0.0f, 0.0f,\
        0.0f, 1.0f, 0.0f, 0.0f,\
        0.0f, 0.0f, 1.0f, 0.0f,\
        0.0f, 0.0f, 0.0f, 1.0f  )

/// @brief Create matrix from array.
/// @param array Array, must have at least 16 floats.
/// @return Matrix with components from array.
attr_core_api
struct Matrix4x4 mat4_from_array( const f32 array[16] );
/// @brief Fill array with components from matrix.
/// @param m Matrix to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 16 floats.
attr_core_api
void mat4_to_array( const struct Matrix4x4* m, f32* out_array );
/// @brief Component-wise add matrices.
/// @param lhs, rhs Matrices to add.
/// @return Result of addition.
attr_core_api
struct Matrix4x4 mat4_add(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs );
/// @brief Component-wise subtract matrices.
/// @param lhs, rhs Matrices to subtract.
/// @return Result of subtraction.
attr_core_api
struct Matrix4x4 mat4_sub(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs );
/// @brief Multiply matrix components.
/// @param lhs Matrix to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_core_api
struct Matrix4x4 mat4_mul(
    const struct Matrix4x4* lhs, f32 rhs );
/// @brief Divide matrix components.
/// @param lhs Matrix to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_core_api
struct Matrix4x4 mat4_div(
    const struct Matrix4x4* lhs, f32 rhs );
/// @brief Multiply matrices.
/// @param lhs, rhs Matrices to multiply.
/// @return Result of multiplication.
attr_core_api
struct Matrix4x4 mat4_mul_mat4(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs );
/// @brief Multiply vector by matrix.
/// @param lhs Matrix to multiply.
/// @param rhs Vector4 to multiply.
/// @return Result of multiplication.
attr_core_api
struct Vector4 mat4_mul_vec4(
    const struct Matrix4x4* lhs, struct Vector4 rhs );
/// @brief Multiply vector by matrix.
/// @param lhs Matrix to multiply.
/// @param rhs Vector3 to multiply.
/// @return Result of multiplication.
attr_header struct Vector3 mat4_mul_vec3(
    const struct Matrix4x4* lhs, struct Vector3 rhs
) {
    struct Vector4 rhs_v4;
    rhs_v4.xyz = rhs;
    rhs_v4.w   = 1.0f;
    return mat4_mul_vec4( lhs, rhs_v4 ).xyz;
}
/// @brief Transpose matrix.
/// @param m Matrix to transpose.
/// @return Transposed matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_transpose( const struct Matrix4x4* m ) {
    return mat4_new(
        m->array[0], m->array[4], m->array[ 8], m->array[12],
        m->array[1], m->array[5], m->array[ 9], m->array[13],
        m->array[2], m->array[6], m->array[10], m->array[14],
        m->array[3], m->array[7], m->array[11], m->array[15] );
}
/// @brief Calculate the determinant of matrix.
/// @param m Matrix to calculate determinant of.
/// @return Determinant of matrix.
attr_core_api
f32 mat4_determinant( const struct Matrix4x4* m );
/// @brief Get the submatrix at given coordinates.
/// @param m Matrix to get submatrix of.
/// @param column, row Coordinates of submatrix.
/// @return Submatrix at given coordinates.
attr_core_api
struct Matrix3x3 mat4_submatrix(
    const struct Matrix4x4* m, u32 column, u32 row );
/// @brief Get the determinant of the submatrix of given coordinates.
/// @param m Matrix to get minor of.
/// @param column, row Coordinates of matrix minor.
/// @return Determinant of submatrix at given coordinates.
attr_always_inline attr_header
f32 mat4_minor( const struct Matrix4x4* m, u32 column, u32 row ) {
    struct Matrix3x3 submatrix = mat4_submatrix( m, column, row );
    return mat3_determinant( &submatrix );
}
/// @brief Get the cofactor of the minor of given coordinates.
/// @param m Matrix to get cofactor of.
/// @param column, row Coordinates of cofactor.
/// @return Cofactor.
attr_core_api
f32 mat4_cofactor( const struct Matrix4x4* m, u32 column, u32 row );
/// @brief Create a matrix of all the cofactors of the given matrix.
/// @param m Matrix to create cofactor matrix of.
/// @return Cofactor matrix.
attr_core_api
struct Matrix4x4 mat4_cofactor_matrix( const struct Matrix4x4* m );
/// @brief Get the adjoint matrix of given matrix.
/// @param m Matrix to get adjoint of.
/// @return Adjoint matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_adjoint( const struct Matrix4x4* m ) {
    struct Matrix4x4 cofactor = mat4_cofactor_matrix( m );
    return mat4_transpose( &cofactor );
}
/// @brief Attempt to get the inverse of given matrix.
/// @param m Matrix to get inverse of.
/// @param[out] out_inverse Inverse matrix.
/// @return False if the determinant of given matrix was zero (division by zero).
attr_core_api
b32 mat4_inverse_checked(
    const struct Matrix4x4* m, struct Matrix4x4* out_inverse );
/// @brief Get the inverse of given matrix without checking for zero determinant.
/// @param m Matrix to get inverse of.
/// @return Inverse matrix.
/// @warning Resulting matrix could be all NaN!
attr_core_api
struct Matrix4x4 mat4_inverse( const struct Matrix4x4* m );
/// @brief Attempt to construct normal matrix from matrix.
/// @param m Matrix to construct normal matrix from.
/// @param[out] out_normal Normal matrix.
/// @return False if the determinant of given matrix was zero (division by zero).
attr_core_api
b32 mat4_normal_matrix_checked(
    const struct Matrix4x4* m, struct Matrix3x3* out_normal );
/// @brief Construct normal matrix from matrix without checking for zero determinant.
/// @param m Matrix to construct normal matrix from.
/// @return Normal matrix.
attr_core_api
struct Matrix3x3 mat4_normal_matrix( const struct Matrix4x4* m );
/// @brief Construct a view matrix.
/// @param position Position of camera.
/// @param target Position that camera is targetting.
/// @param up Up direction.
/// @return View matrix.
attr_core_api
struct Matrix4x4 mat4_view(
    struct Vector3 position, struct Vector3 target, struct Vector3 up );
/// @brief Construct a view Matrix for 2d rendering.
/// @param position Position of camera.
/// @param up Up direction.
/// @return View matrix for 2d rendering.
attr_always_inline attr_header
struct Matrix4x4 mat4_view_2d(
    struct Vector2 position, struct Vector2 up
) {
    struct Vector3 target3d   = vec3( position.x, position.y, 0.0f );
    struct Vector3 position3d = target3d;
    position3d.z = 1.0f;
    struct Vector3 up3d = vec3( up.x, up.y, 0.0f );
    return mat4_view( position3d, target3d, up3d );
}
/// @brief Construct an orthographic projection matrix.
/// @param left, right Left and Right bounds of projection.
/// @param bottom, top Bottom and Top bounds of projection.
/// @param clip_near, clip_far Near and Far bounds of projection.
/// @return Orthographic projection matrix.
attr_core_api
struct Matrix4x4 mat4_ortho(
    f32 left, f32 right,
    f32 bottom, f32 top,
    f32 clip_near, f32 clip_far );
/// @brief Construct a perspective projection matrix.
/// @param field_of_view FOV in radians.
/// @param aspect_ratio Aspect ratio of render target.
/// @param clip_near, clip_far Near and Far bounds of projection.
/// @return Perspective projection matrix.
attr_core_api
struct Matrix4x4 mat4_perspective(
    f32 field_of_view, f32 aspect_ratio, f32 clip_near, f32 clip_far );
/// @brief Construct a translation matrix.
/// @param x, y, z Translation coordinates.
/// @return Translation matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_translation( f32 x, f32 y, f32 z ) {
    return mat4_new(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
           x,    y,    z, 1.0f );
}
/// @brief Construct a translation matrix.
/// @param translation Translation coordinates.
/// @return Translation matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_translation_vec3( struct Vector3 translation ) {
    return mat4_translation( translation.x, translation.y, translation.z );
}
/// @brief Construct a 2d translation matrix.
/// @param x, y Translation coordinates.
/// @return Translation matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_translation_2d( f32 x, f32 y ) {
    return mat4_translation( x, y, 0.0f );
}
/// @brief Construct a 2d translation matrix.
/// @param translation Translation coordinates.
/// @return Translation matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_translation_vec2( struct Vector2 translation ) {
    return mat4_translation_2d( translation.x, translation.y );
}
/// @brief Construct a rotation matrix for pitch rotation.
/// @param pitch Rotation in radians.
/// @return Rotation matrix.
attr_core_api
struct Matrix4x4 mat4_rotation_pitch( f32 pitch );
/// @brief Construct a rotation matrix for yaw rotation.
/// @param yaw Rotation in radians.
/// @return Rotation matrix.
attr_core_api
struct Matrix4x4 mat4_rotation_yaw( f32 yaw );
/// @brief Construct a rotation matrix for roll rotation.
/// @param roll Rotation in radians.
/// @return Rotation matrix.
attr_core_api
struct Matrix4x4 mat4_rotation_roll( f32 roll );
/// @brief Construct a rotation matrix using euler angles.
/// @param pitch, yaw, roll Rotation in radians.
/// @return Rotation matrix.
attr_core_api
struct Matrix4x4 mat4_rotation_euler( f32 pitch, f32 yaw, f32 roll );
/// @brief Construct a rotation matrix using euler angles.
/// @param euler Rotation in radians.
/// @return Rotation matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_rotation_euler_vec3( struct Vector3 euler ) {
    return mat4_rotation_euler( euler.x, euler.y, euler.z );
}
/// @brief Construct a rotation matrix from quaternion rotation.
/// @param rotation Rotation.
/// @return Rotation matrix.
attr_core_api
struct Matrix4x4 mat4_rotation( struct Quaternion rotation );
/// @brief Construct a rotation matrix for 2d rotation.
/// @param rotation Rotation in radians.
/// @return Rotation matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_rotation_2d( f32 rotation ) {
    return mat4_rotation_roll( -rotation );
}
/// @brief Construct a scale matrix.
/// @param width, height, length Dimensions.
/// @return Scale matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_scale( f32 width, f32 height, f32 length ) {
    return mat4_new(
        width,   0.0f,   0.0f, 0.0f,
         0.0f, height,   0.0f, 0.0f,
         0.0f,   0.0f, length, 0.0f,
         0.0f,   0.0f,   0.0f, 1.0f );
}
/// @brief Construct a scale matrix.
/// @param dimensions Dimensions.
/// @return Scale matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_scale_vec3( struct Vector3 dimensions ) {
    return mat4_scale( dimensions.x, dimensions.y, dimensions.z );
}
/// @brief Construct a 2d scale matrix.
/// @param width, height Dimensions.
/// @return Scale matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_scale_2d( f32 width, f32 height ) {
    return mat4_scale( width, height, 0.0f );
}
/// @brief Construct a 2d scale matrix.
/// @param dimensions Dimensions.
/// @return Scale matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_scale_vec2( struct Vector2 dimensions ) {
    return mat4_scale_2d( dimensions.x, dimensions.y );
}
/// @brief Construct a transform matrix.
/// @param translation Translation.
/// @param rotation Rotation.
/// @param scale Scale.
/// @return Transform matrix.
attr_core_api
struct Matrix4x4 mat4_transform(
    struct Vector3 translation, struct Quaternion rotation,
    struct Vector3 scale );
/// @brief Construct a transform matrix.
/// @param translation Translation.
/// @param rotation Euler rotation.
/// @param scale Scale.
/// @return Transform matrix.
attr_core_api
struct Matrix4x4 mat4_transform_euler(
    struct Vector3 translation, struct Vector3 rotation,
    struct Vector3 scale );
/// @brief Construct a 2d transform matrix.
/// @param translation Translation.
/// @param rotation Rotation.
/// @param scale Scale.
/// @return Transform matrix.
attr_always_inline attr_header
struct Matrix4x4 mat4_transform_2d(
    struct Vector2 translation, f32 rotation, struct Vector2 scale
) {
    struct Vector3 translation3d = vec3( translation.x, translation.y, 0.0f );
    struct Vector3 rotation3d    = vec3( 0.0f, 0.0f, -rotation );
    struct Vector3 scale3d       = vec3( scale.x, scale.y, 0.0f );
    return mat4_transform_euler( translation3d, rotation3d, scale3d );
}

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#if defined(CORE_CPLUSPLUS)
    #if !defined(CORE_CPP_MATH_MATRIX4X4_HPP)
        #include "core/cpp/math/matrix4x4.hpp"
    #endif
    typedef Matrix4x4CPP mat4x4;
    typedef mat4x4 mat4;
#endif

#endif /* header guard */
