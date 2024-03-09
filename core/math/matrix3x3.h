#if !defined(CORE_MATH_MATRIX3X3_H)
#define CORE_MATH_MATRIX3X3_H
/**
 * @file   matrix3x3.h
 * @brief  Matrix3x3 definition.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 28, 2024
*/
#include "core/defines.h"
#include "core/types.h"
#include "core/attributes.h"
#include "core/math/vector3.h"

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

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
        struct Vector3 c[3];
        /// @brief Matrix components as an array.
        f32  v[9];
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief Column-major 3x3 matrix.
    /// @see Matrix3x3
    typedef struct Matrix3x3 mat3;
#endif

#if defined(CORE_DOXYGEN)
    /// @brief Construct a new 3x3 Matrix.
    /// @param m00, m01, m02 First column components.
    /// @param m10, m11, m12 Second column components.
    /// @param m20, m21, m22 Third column components.
    /// @return Matrix3x3.
    #define m3( m00, m01, m02, m10, m11, m12, m20, m21, m22 )

#else /* Doxygen */

#if defined(CORE_CPLUSPLUS)
    #define m3(\
        m00, m01, m02,\
        m10, m11, m12,\
        m20, m21, m22 )\
        Matrix3x3{ .v={\
            (m00), (m01), (m02), (m10), (m11), (m12), (m20), (m21), (m22) } }
#else /* C++ constructor */
    #define m3(\
        m00, m01, m02,\
        m10, m11, m12,\
        m20, m21, m22 )\
        (struct Matrix3x3){ .v={\
            (m00), (m01), (m02), (m10), (m11), (m12), (m20), (m21), (m22) } }

#endif /* C constructor */

#endif /* Doxygen */

/// @brief Matrix3x3 zero constant.
#define MAT3_ZERO m3(\
        0.0f, 0.0f, 0.0f,\
        0.0f, 0.0f, 0.0f,\
        0.0f, 0.0f, 0.0f  )
/// @brief Matrix3x3 identity constant.
#define MAT3_IDENTITY m3(\
        1.0f, 0.0f, 0.0f,\
        0.0f, 1.0f, 0.0f,\
        0.0f, 0.0f, 1.0f )

/// @brief Create matrix from array.
/// @param array Array, must have at least 9 floats.
/// @return Matrix with components from array.
attr_core_api struct Matrix3x3 m3_from_array( const f32 array[9] );
/// @brief Fill array with components from matrix.
/// @param m Matrix to pull components from.
/// @param[out] out_array Pointer to array, must be able to hold at least 9 floats.
attr_core_api void m3_to_array( const struct Matrix3x3* m, f32* out_array );
/// @brief Component-wise add matrices.
/// @param lhs, rhs Matrices to add.
/// @return Result of addition.
attr_always_inline
attr_header struct Matrix3x3 m3_add(
    const struct Matrix3x3* lhs, const struct Matrix3x3* rhs
) {
    struct Matrix3x3 result;
    result.col0 = v3_add( lhs->col0, rhs->col0 );
    result.col1 = v3_add( lhs->col1, rhs->col1 );
    result.col2 = v3_add( lhs->col2, rhs->col2 );
    return result;
}
/// @brief Component-wise subtract matrices.
/// @param lhs, rhs Matrices to subtract.
/// @return Result of subtraction.
attr_always_inline
attr_header struct Matrix3x3 m3_sub(
    const struct Matrix3x3* lhs, const struct Matrix3x3* rhs
) {
    struct Matrix3x3 result;
    result.col0 = v3_sub( lhs->col0, rhs->col0 );
    result.col1 = v3_sub( lhs->col1, rhs->col1 );
    result.col2 = v3_sub( lhs->col2, rhs->col2 );
    return result;
}
/// @brief Multiply matrix components.
/// @param lhs Matrix to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
attr_always_inline
attr_header struct Matrix3x3 m3_mul(
    const struct Matrix3x3* lhs, f32 rhs
) {
    struct Matrix3x3 result;
    result.col0 = v3_mul( lhs->col0, rhs );
    result.col1 = v3_mul( lhs->col1, rhs );
    result.col2 = v3_mul( lhs->col2, rhs );
    return result;
}
/// @brief Divide matrix components.
/// @param lhs Matrix to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
attr_always_inline
attr_header struct Matrix3x3 m3_div(
    const struct Matrix3x3* lhs, f32 rhs
) {
    struct Matrix3x3 result;
    result.col0 = v3_div( lhs->col0, rhs );
    result.col1 = v3_div( lhs->col1, rhs );
    result.col2 = v3_div( lhs->col2, rhs );
    return result;
}
/// @brief Multiply matrices.
/// @param lhs, rhs Matrices to multiply.
/// @return Result of multiplication.
attr_header struct Matrix3x3 m3_mul_m3(
    const struct Matrix3x3* lhs, const struct Matrix3x3* rhs
) {
    return m3(
        // column - 0
        ( lhs->v[0] * rhs->v[0] ) +
            ( lhs->v[3] * rhs->v[1] )  +
            ( lhs->v[6] * rhs->v[2] ),
        ( lhs->v[1] * rhs->v[0] ) +
            ( lhs->v[4] * rhs->v[1] )  +
            ( lhs->v[7] * rhs->v[2] ),
        ( lhs->v[2] * rhs->v[0] ) +
            ( lhs->v[5] * rhs->v[1] )  +
            ( lhs->v[8] * rhs->v[2] ),
        // volumn - 1
        ( lhs->v[0] * rhs->v[3] ) +
            ( lhs->v[3] * rhs->v[4] )  +
            ( lhs->v[6] * rhs->v[5] ),
        ( lhs->v[1] * rhs->v[3] ) +
            ( lhs->v[4] * rhs->v[4] )  +
            ( lhs->v[7] * rhs->v[5] ),
        ( lhs->v[2] * rhs->v[3] ) +
            ( lhs->v[5] * rhs->v[4] )  +
            ( lhs->v[8] * rhs->v[5] ),
        // volumn - 2
        ( lhs->v[0] * rhs->v[6] ) +
            ( lhs->v[3] * rhs->v[7] )  +
            ( lhs->v[6] * rhs->v[8] ),
        ( lhs->v[1] * rhs->v[6] ) +
            ( lhs->v[4] * rhs->v[7] )  +
            ( lhs->v[7] * rhs->v[8] ),
        ( lhs->v[2] * rhs->v[6] ) +
            ( lhs->v[5] * rhs->v[7] )  +
            ( lhs->v[8] * rhs->v[8] ) );
}
/// @brief Transpose matrix.
/// @param m Matrix to transpose.
/// @return Transposed matrix.
attr_always_inline
attr_header struct Matrix3x3 m3_transpose( const struct Matrix3x3* m ) {
    return m3(
        m->v[0], m->v[3], m->v[6],
        m->v[1], m->v[4], m->v[7],
        m->v[2], m->v[5], m->v[8] );
}
/// @brief Calculate the determinant of matrix.
/// @param m Matrix to calculate determinant of.
/// @return Determinant of matrix.
attr_always_inline
attr_header f32 m3_determinant( const struct Matrix3x3* m ) {
    return
    ( m->v[0] * ( ( m->v[4] * m->v[8] ) - ( m->v[7] * m->v[5] ) ) ) -
    ( m->v[3] * ( ( m->v[1] * m->v[8] ) - ( m->v[7] * m->v[2] ) ) ) +
    ( m->v[6] * ( ( m->v[1] * m->v[5] ) - ( m->v[4] * m->v[2] ) ) );
}

#if defined(CORE_CPLUSPLUS)

// TODO(alicia): DOCUMENT C++

namespace CoreMathInternal {

struct Matrix3x3 : public ::Matrix3x3 {
    Matrix3x3() : ::Matrix3x3{ .v= {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    } } {}
    Matrix3x3(
        f32 m00, f32 m01, f32 m02,
        f32 m10, f32 m11, f32 m12,
        f32 m20, f32 m21, f32 m22
    ) : ::Matrix3x3{ .v={
            m00, m01, m02,
            m10, m11, m12,
            m20, m21, m22
        } } {}
    Matrix3x3( ::Matrix3x3 m ) : Matrix3x3(
        m.v[0], m.v[1], m.v[2],
        m.v[3], m.v[4], m.v[5],
        m.v[6], m.v[7], m.v[8]
    ) {}

    attr_always_inline
    attr_header vec3 operator[]( usize index ) const {
        return c[index];
    }
    attr_always_inline
    attr_header vec3& operator[]( usize index ) {
        return *(vec3*)(c + index);
    }
    attr_always_inline
    attr_header void operator+=( const Matrix3x3* rhs ) {
        *this = m3_add( this, rhs );
    }
    attr_always_inline
    attr_header void operator-=( const Matrix3x3* rhs ) {
        *this = m3_sub( this, rhs );
    }
    attr_always_inline
    attr_header void operator*=( f32 rhs ) {
        *this = m3_mul( this, rhs );
    }
    attr_always_inline
    attr_header void operator/=( f32 rhs ) {
        *this = m3_div( this, rhs );
    }

}; /* struct Matrix3x3 */

} /* namespace CoreMathInternal */

typedef CoreMathInternal::Matrix3x3 mat3;

attr_always_inline
attr_header mat3 operator+( const mat3& lhs, const mat3& rhs ) {
    return m3_add( &lhs, &rhs );
}
attr_always_inline
attr_header mat3 operator-( const mat3& lhs, const mat3& rhs ) {
    return m3_sub( &lhs, &rhs );
}
attr_always_inline
attr_header mat3 operator*( const mat3& lhs, f32 rhs ) {
    return m3_mul( &lhs, rhs );
}
attr_always_inline
attr_header mat3 operator*( f32 lhs, const mat3& rhs ) {
    return rhs * lhs;
}
attr_always_inline
attr_header mat3 operator*( const mat3& lhs, const mat3& rhs ) {
    return m3_mul_m3( &lhs, &rhs );
}
attr_always_inline
attr_header mat3 operator/( const mat3& lhs, f32 rhs ) {
    return m3_div( &lhs, rhs );
}

#endif /* C++ */

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#endif /* header guard */
