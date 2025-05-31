#if !defined(CORE_MATH_MATRIX2X2_H)
#define CORE_MATH_MATRIX2X2_H
/**
 * @file   matrix2x2.h
 * @brief  Matrix2x2.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 28, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/math/vector2.h"

/// @brief Column-major 2x2 matrix.
struct Matrix2x2 {
    /// @brief Union of matrix component aliases.
    union {
        /// @brief Columns 0 and 1.
        struct {
            /// @brief Column 0.
            union {
                /// @brief Column 0.
                struct {
                    /// @brief Column 0, Row 0.
                    f32 m00;
                    /// @brief Column 0, Row 1.
                    f32 m01;
                };
                /// @brief Column 0 as a #Vector2.
                struct Vector2 col0;
            };
            /// @brief Column 1.
            union {
                /// @brief Column 1.
                struct {
                    /// @brief Column 1, Row 0.
                    f32 m10;
                    /// @brief Column 1, Row 1.
                    f32 m11;
                };
                /// @brief Column 1 as a #Vector2.
                struct Vector2 col1;
            };
        };
        /// @brief Columns as an array of vectors.
        struct Vector2 col[2];
        /// @brief Matrix components as an array.
        f32  array[4];
    };
};

#if defined(__cplusplus)
    /// @brief Create new Matrix.
    /// @param m00 Column 0, Row 0
    /// @param m01 Column 0, Row 1
    /// @param m10 Column 1, Row 0
    /// @param m11 Column 1, Row 1
    /// @return Matrix.
    #define mat2_new( m00, m01, m10, m11 ) Matrix2x2{ .array={ m00, m01, m10, m11 } }
#else
    /// @brief Column-major 2x2 matrix.
    typedef struct Matrix2x2 mat2x2;
    /// @brief Column-major 2x2 matrix.
    typedef mat2x2 mat2;

    /// @brief Create new Matrix.
    /// @param m00 Column 0, Row 0
    /// @param m01 Column 0, Row 1
    /// @param m10 Column 1, Row 0
    /// @param m11 Column 1, Row 1
    /// @return Matrix.
    #define mat2_new( m00, m01, m10, m11 ) (struct Matrix2x2){ .array={ m00, m01, m10, m11 } }
    /// @brief Create new Matrix.
    /// @param m00 Column 0, Row 0
    /// @param m01 Column 0, Row 1
    /// @param m10 Column 1, Row 0
    /// @param m11 Column 1, Row 1
    /// @return Matrix.
    #define mat2( m00, m01, m10, m11 )     mat2_new( m00, m01, m10, m11 )
#endif

/// @brief Matrix2x2 zero constant.
#define MAT2_ZERO     mat2_new( 0.0f, 0.0f, 0.0f, 0.0f )
/// @brief Matrix2x2 identity constant.
#define MAT2_IDENTITY mat2_new( 1.0f, 0.0f, 0.0f, 1.0f )

/// @brief Create matrix from array.
/// @param array Array, must have at least 4 floats.
/// @return Matrix with components from array.
attr_core_api
struct Matrix2x2 mat2_from_array( const f32 array[4] );
/// @brief Fill array with components from matrix.
/// @param      m         Matrix to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 4 floats.
attr_core_api
void array_from_mat2( struct Matrix2x2 m, f32* out_array );
/// @brief Component-wise add matrices.
/// @param lhs, rhs Matrices to add.
/// @return Result of addition.
attr_always_inline attr_header
struct Matrix2x2 mat2_add(
    struct Matrix2x2 lhs, struct Matrix2x2 rhs
) {
    struct Matrix2x2 result;
    result.col0 = vec2_add( lhs.col0, rhs.col0 );
    result.col1 = vec2_add( lhs.col1, rhs.col1 );
    return result;
}
/// @brief Component-wise subtract matrices.
/// @param lhs, rhs Matrices to subtract.
/// @return Result of subtraction.
attr_always_inline attr_header
struct Matrix2x2 mat2_sub(
    struct Matrix2x2 lhs, struct Matrix2x2 rhs
) {
    struct Matrix2x2 result;
    result.col0 = vec2_sub( lhs.col0, rhs.col0 );
    result.col1 = vec2_sub( lhs.col1, rhs.col1 );
    return result;
}
/// @brief Multiply matrix components.
/// @param lhs Matrix to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline attr_header
struct Matrix2x2 mat2_mul(
    struct Matrix2x2 lhs, f32 rhs
) {
    struct Matrix2x2 result;
    result.col0 = vec2_mul( lhs.col0, rhs );
    result.col1 = vec2_mul( lhs.col1, rhs );
    return result;
}
/// @brief Divide matrix components.
/// @brief Multiply matrices.
/// @param lhs, rhs Matrices to multiply.
/// @return Result of multiplication
attr_always_inline attr_header
struct Matrix2x2 mat2_mul_mat2(
    struct Matrix2x2 lhs, struct Matrix2x2 rhs
) {
    return mat2_new(
        (lhs.array[0] * rhs.array[0]) + (lhs.array[2] * rhs.array[1]),
        (lhs.array[1] * rhs.array[0]) + (lhs.array[3] * rhs.array[2]),
        (lhs.array[0] * rhs.array[2]) + (lhs.array[2] * rhs.array[3]),
        (lhs.array[1] * rhs.array[2]) + (lhs.array[3] * rhs.array[3]) );
}
/// @brief Multiply.
/// @param lhs Matrix.
/// @param rhs Vector.
/// @return Result.
attr_always_inline attr_header
struct Vector2 mat2_mul_vec2( struct Matrix2x2 lhs, struct Vector2 rhs ) {
    struct Vector2 result;
    result.x = lhs.array[0] * rhs.x + lhs.array[2] * rhs.y;
    result.y = lhs.array[1] * rhs.x + lhs.array[3] * rhs.y;
    return result;
}
/// @param lhs Matrix to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline attr_header
struct Matrix2x2 mat2_div(
    struct Matrix2x2 lhs, f32 rhs
) {
    struct Matrix2x2 result;
    result.col0 = vec2_div( lhs.col0, rhs );
    result.col1 = vec2_div( lhs.col1, rhs );
    return result;
}
/// @brief Transpose matrix.
/// @param m Matrix to transpose.
/// @return Transposed matrix.
attr_always_inline attr_header
struct Matrix2x2 mat2_transpose( struct Matrix2x2 m ) {
    return mat2_new(
        m.array[0], m.array[2],
        m.array[1], m.array[3] );
}
/// @brief Calculate the determinant of matrix.
/// @param m Matrix to calculate determinant of.
/// @return Determinant of matrix.
attr_always_inline attr_header
f32 mat2_determinant( struct Matrix2x2 m ) {
    return ( m.array[0] * m.array[3] ) - ( m.array[2] * m.array[1] );
}

#if !defined(CORE_CPP_MATH_MATRIX2X2_HPP)
    #include "core/cpp/math/matrix2x2.hpp"
#endif

#endif /* header guard */
