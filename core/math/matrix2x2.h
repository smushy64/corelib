#if !defined(CORE_MATH_MATRIX2X2_H)
#define CORE_MATH_MATRIX2X2_H
/**
 * @file   matrix2x2.h
 * @brief  Matrix2x2 definition.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 28, 2024
*/
#include "core/defines.h"
#include "core/types.h"
#include "core/attributes.h"
#include "core/math/vector2.h"

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

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
        struct Vector2 c[2];
        /// @brief Matrix components as an array.
        f32  v[4];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief Column-major 2x2 Matrix.
    /// @see Matrix2x2
    typedef struct Matrix2x2 mat2;
#endif

#if defined(CORE_DOXYGEN)
    /// @brief Construct a new 2x2 Matrix.
    /// @param m00, m01 First column components.
    /// @param m10, m11 Second column components.
    /// @return Matrix2x2.
    #define m2( m00, m01, m10, m11 )
#else /* Doxygen */

#if defined(CORE_CPLUSPLUS)
    #define m2(\
        m00, m01,\
        m10, m11 )\
        Matrix2x2{ .v={ (m00), (m01), (m10), (m11) } }
#else /* C++ constructor */
    #define m2( m00, m01, m10, m11 )\
        (struct Matrix2x2){ .v={ (m00), (m01), (m10), (m11) } }
#endif /* C constructor */

#endif /* Doxygen */

/// @brief Matrix2x2 zero constant.
#define MAT2_ZERO     m2( 0.0f, 0.0f, 0.0f, 0.0f )
/// @brief Matrix2x2 identity constant.
#define MAT2_IDENTITY m2( 1.0f, 0.0f, 0.0f, 1.0f )

/// @brief Create matrix from array.
/// @param array Array, must have at least 4 floats.
/// @return Matrix with components from array.
attr_core_api struct Matrix2x2 m2_from_array( const f32 array[4] );
/// @brief Fill array with components from matrix.
/// @param m Matrix to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 4 floats.
attr_core_api void m2_to_array( struct Matrix2x2 m, f32* out_array );
/// @brief Component-wise add matrices.
/// @param lhs, rhs Matrices to add.
/// @return Result of addition.
attr_always_inline
attr_header struct Matrix2x2 m2_add(
    struct Matrix2x2 lhs, struct Matrix2x2 rhs
) {
    struct Matrix2x2 result;
    result.col0 = v2_add( lhs.col0, rhs.col0 );
    result.col1 = v2_add( lhs.col1, rhs.col1 );
    return result;
}
/// @brief Component-wise subtract matrices.
/// @param lhs, rhs Matrices to subtract.
/// @return Result of subtraction.
attr_always_inline
attr_header struct Matrix2x2 m2_sub(
    struct Matrix2x2 lhs, struct Matrix2x2 rhs
) {
    struct Matrix2x2 result;
    result.col0 = v2_sub( lhs.col0, rhs.col0 );
    result.col1 = v2_sub( lhs.col1, rhs.col1 );
    return result;
}
/// @brief Multiply matrix components.
/// @param lhs Matrix to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline
attr_header struct Matrix2x2 m2_mul(
    struct Matrix2x2 lhs, f32 rhs
) {
    struct Matrix2x2 result;
    result.col0 = v2_mul( lhs.col0, rhs );
    result.col1 = v2_mul( lhs.col1, rhs );
    return result;
}
/// @brief Divide matrix components.
/// @param lhs Matrix to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline
attr_header struct Matrix2x2 m2_div(
    struct Matrix2x2 lhs, f32 rhs
) {
    struct Matrix2x2 result;
    result.col0 = v2_div( lhs.col0, rhs );
    result.col1 = v2_div( lhs.col1, rhs );
    return result;
}
/// @brief Multiply matrices.
/// @param lhs, rhs Matrices to multiply.
/// @return Result of multiplication
attr_always_inline
attr_header struct Matrix2x2 m2_mul_m2(
    struct Matrix2x2 lhs, struct Matrix2x2 rhs
) {
    return m2(
        (lhs.v[0] * rhs.v[0]) + (lhs.v[2] * rhs.v[1]),
        (lhs.v[1] * rhs.v[0]) + (lhs.v[3] * rhs.v[2]),
        (lhs.v[0] * rhs.v[2]) + (lhs.v[2] * rhs.v[3]),
        (lhs.v[1] * rhs.v[2]) + (lhs.v[3] * rhs.v[3]) );
}
/// @brief Transpose matrix.
/// @param m Matrix to transpose.
/// @return Transposed matrix.
attr_always_inline
attr_header struct Matrix2x2 m2_transpose( struct Matrix2x2 m ) {
    return m2(
        m.v[0], m.v[2],
        m.v[1], m.v[3] );
}
/// @brief Calculate the determinant of matrix.
/// @param m Matrix to calculate determinant of.
/// @return Determinant of matrix.
attr_always_inline
attr_header f32 m2_determinant( struct Matrix2x2 m ) {
    return ( m.v[0] * m.v[3] ) - ( m.v[2] * m.v[1] );
}

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#if defined(CORE_CPLUSPLUS)
    #if !defined(CORE_CPP_MATH_MATRIX2X2_HPP)
        #include "core/cpp/math/matrix2x2.hpp"
    #endif
    typedef Matrix2x2CPP mat2;
#endif

#endif /* header guard */
