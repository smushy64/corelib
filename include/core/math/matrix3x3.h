#if !defined(CORE_MATH_MATRIX3X3_H)
#define CORE_MATH_MATRIX3X3_H
/**
 * @file   matrix3x3.h
 * @brief  Matrix3x3 definition.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 28, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/math/vector3.h"

/// @brief Column-major 3x3 matrix.
struct Matrix3x3 {
    /// @brief Union of matrix components.
    union {
        /// @brief Columns 0, 1 and 2.
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
                };
                /// @brief Column 0 as a #Vector3.
                struct Vector3 col0;
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
                };
                /// @brief Column 1 as a #Vector3.
                struct Vector3 col1;
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
                };
                /// @brief Column 2 as a #Vector3.
                struct Vector3 col2;
            };
        };
        /// @brief Columns as an array of vectors.
        struct Vector3 col[3];
        /// @brief Matrix components as an array.
        f32  array[9];
    };
};

#if defined(__cplusplus)
    /// @brief Create matrix.
    /// @param m00, m01, m02 First column components.
    /// @param m10, m11, m12 Second column components.
    /// @param m20, m21, m22 Third column components.
    /// @return Matrix.
    #define mat3_new(            \
        m00, m01, m02,           \
        m10, m11, m12,           \
        m20, m21, m22 )          \
        Matrix3x3 { .array={     \
            (m00), (m01), (m02), \
            (m10), (m11), (m12), \
            (m20), (m21), (m22)  \
        } }
#else
    /// @brief Column-major 3x3 matrix.
    typedef struct Matrix3x3 mat3x3;
    /// @brief Column-major 3x3 matrix.
    typedef struct Matrix3x3 mat3;

    /// @brief Create matrix.
    /// @param m00, m01, m02 First column components.
    /// @param m10, m11, m12 Second column components.
    /// @param m20, m21, m22 Third column components.
    /// @return Matrix.
    #define mat3_new(                \
        m00, m01, m02,               \
        m10, m11, m12,               \
        m20, m21, m22 )              \
        (struct Matrix3x3){ .array={ \
            (m00), (m01), (m02),     \
            (m10), (m11), (m12),     \
            (m20), (m21), (m22)      \
        } }

    /// @brief Create matrix.
    /// @param m00, m01, m02 First column components.
    /// @param m10, m11, m12 Second column components.
    /// @param m20, m21, m22 Third column components.
    /// @return Matrix.
    #define mat3(           \
        m00, m01, m02,      \
        m10, m11, m12,      \
        m20, m21, m22 )     \
        mat3_new(           \
            m00, m01, m02,  \
            m10, m11, m12,  \
            m20, m21, m22 )
#endif

/// @brief Matrix3x3 zero constant.
#define MAT3_ZERO mat3_new(  \
        0.0f, 0.0f, 0.0f,    \
        0.0f, 0.0f, 0.0f,    \
        0.0f, 0.0f, 0.0f  )
/// @brief Matrix3x3 identity constant.
#define MAT3_IDENTITY mat3_new(  \
        1.0f, 0.0f, 0.0f,        \
        0.0f, 1.0f, 0.0f,        \
        0.0f, 0.0f, 1.0f )

/// @brief Create matrix from array.
/// @param array Array, must have at least 9 floats.
/// @return Matrix with components from array.
attr_core_api
struct Matrix3x3 mat3_from_array( const f32 array[9] );
/// @brief Fill array with components from matrix.
/// @param m Matrix to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 9 floats.
attr_core_api
void array_from_mat3( const struct Matrix3x3* m, f32* out_array );
/// @brief Component-wise add matrices.
/// @param lhs, rhs Matrices to add.
/// @return Result of addition.
attr_always_inline attr_header
struct Matrix3x3 mat3_add(
    const struct Matrix3x3* lhs, const struct Matrix3x3* rhs
) {
    struct Matrix3x3 result;
    result.col0 = vec3_add( lhs->col0, rhs->col0 );
    result.col1 = vec3_add( lhs->col1, rhs->col1 );
    result.col2 = vec3_add( lhs->col2, rhs->col2 );
    return result;
}
/// @brief Component-wise subtract matrices.
/// @param lhs, rhs Matrices to subtract.
/// @return Result of subtraction.
attr_always_inline attr_header
struct Matrix3x3 mat3_sub(
    const struct Matrix3x3* lhs, const struct Matrix3x3* rhs
) {
    struct Matrix3x3 result;
    result.col0 = vec3_sub( lhs->col0, rhs->col0 );
    result.col1 = vec3_sub( lhs->col1, rhs->col1 );
    result.col2 = vec3_sub( lhs->col2, rhs->col2 );
    return result;
}
/// @brief Multiply matrix components.
/// @param lhs Matrix to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline attr_header
struct Matrix3x3 mat3_mul(
    const struct Matrix3x3* lhs, f32 rhs
) {
    struct Matrix3x3 result;
    result.col0 = vec3_mul( lhs->col0, rhs );
    result.col1 = vec3_mul( lhs->col1, rhs );
    result.col2 = vec3_mul( lhs->col2, rhs );
    return result;
}
/// @brief Multiply matrices.
/// @param lhs, rhs Matrices to multiply.
/// @return Result of multiplication.
attr_header
struct Matrix3x3 mat3_mul_mat3(
    const struct Matrix3x3* lhs, const struct Matrix3x3* rhs
) {
    return mat3_new(
        // column - 0
        ( lhs->array[0] * rhs->array[0] ) +
            ( lhs->array[3] * rhs->array[1] )  +
            ( lhs->array[6] * rhs->array[2] ),
        ( lhs->array[1] * rhs->array[0] ) +
            ( lhs->array[4] * rhs->array[1] )  +
            ( lhs->array[7] * rhs->array[2] ),
        ( lhs->array[2] * rhs->array[0] ) +
            ( lhs->array[5] * rhs->array[1] )  +
            ( lhs->array[8] * rhs->array[2] ),
        // volumn - 1
        ( lhs->array[0] * rhs->array[3] ) +
            ( lhs->array[3] * rhs->array[4] )  +
            ( lhs->array[6] * rhs->array[5] ),
        ( lhs->array[1] * rhs->array[3] ) +
            ( lhs->array[4] * rhs->array[4] )  +
            ( lhs->array[7] * rhs->array[5] ),
        ( lhs->array[2] * rhs->array[3] ) +
            ( lhs->array[5] * rhs->array[4] )  +
            ( lhs->array[8] * rhs->array[5] ),
        // volumn - 2
        ( lhs->array[0] * rhs->array[6] ) +
            ( lhs->array[3] * rhs->array[7] )  +
            ( lhs->array[6] * rhs->array[8] ),
        ( lhs->array[1] * rhs->array[6] ) +
            ( lhs->array[4] * rhs->array[7] )  +
            ( lhs->array[7] * rhs->array[8] ),
        ( lhs->array[2] * rhs->array[6] ) +
            ( lhs->array[5] * rhs->array[7] )  +
            ( lhs->array[8] * rhs->array[8] ) );
}
/// @brief Multiply matrices.
/// @param lhs, rhs Matrices to multiply.
/// @return Result of multiplication.
attr_header
struct Vector3 mat3_mul_vec3( const struct Matrix3x3* m, struct Vector3 v ) {
    struct Vector3 result;
    result.x = m->array[0] * v.x + m->array[3] * v.y + m->array[6] * v.z;
    result.y = m->array[1] * v.x + m->array[4] * v.y + m->array[7] * v.z;
    result.z = m->array[2] * v.x + m->array[5] * v.y + m->array[8] * v.z;
    return result;
}
/// @brief Divide matrix components.
/// @param lhs Matrix to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline attr_header
struct Matrix3x3 mat3_div(
    const struct Matrix3x3* lhs, f32 rhs
) {
    struct Matrix3x3 result;
    result.col0 = vec3_div( lhs->col0, rhs );
    result.col1 = vec3_div( lhs->col1, rhs );
    result.col2 = vec3_div( lhs->col2, rhs );
    return result;
}
/// @brief Transpose matrix.
/// @param m Matrix to transpose.
/// @return Transposed matrix.
attr_always_inline attr_header
struct Matrix3x3 mat3_transpose( const struct Matrix3x3* m ) {
    return mat3_new(
        m->array[0], m->array[3], m->array[6],
        m->array[1], m->array[4], m->array[7],
        m->array[2], m->array[5], m->array[8] );
}
/// @brief Calculate the determinant of matrix.
/// @param m Matrix to calculate determinant of.
/// @return Determinant of matrix.
attr_always_inline attr_header
f32 mat3_determinant( const struct Matrix3x3* m ) {
    return
    ( m->array[0] * ( ( m->array[4] * m->array[8] ) - ( m->array[7] * m->array[5] ) ) ) -
    ( m->array[3] * ( ( m->array[1] * m->array[8] ) - ( m->array[7] * m->array[2] ) ) ) +
    ( m->array[6] * ( ( m->array[1] * m->array[5] ) - ( m->array[4] * m->array[2] ) ) );
}

#if !defined(CORE_CPP_MATH_MATRIX3X3_HPP)
    #include "core/cpp/math/matrix3x3.hpp"
#endif

#endif /* header guard */
