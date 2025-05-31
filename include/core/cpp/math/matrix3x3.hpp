#if !defined(CORE_CPP_MATH_MATRIX3X3_HPP) && defined(__cplusplus)
#define CORE_CPP_MATH_MATRIX3X3_HPP
/**
 * @file   matrix3x3.hpp
 * @brief  C++ Math: Matrix3x3.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 30, 2025
*/
#if !defined(CORE_MATH_MATRIX3X3_H)
    #include "core/math/matrix3x3.h"
#endif

struct Matrix3x3CPP {
    union {
        struct {
            union {
                struct { f32 m00, m01, m02; };
                Vector3CPP col0;
            };
            union {
                struct { f32 m10, m11, m12; };
                Vector3CPP col1;
            };
            union {
                struct { f32 m20, m21, m22; };
                Vector3CPP col2;
            };
        };
        struct Matrix3x3 __pod;

        Vector3CPP col[3];
        f32        array[9];
    };

    constexpr Matrix3x3CPP();
    constexpr Matrix3x3CPP( const Matrix3x3& __pod );
    constexpr explicit Matrix3x3CPP(
        f32 m00, f32 m01, f32 m02,
        f32 m10, f32 m11, f32 m12,
        f32 m20, f32 m21, f32 m22 );
    constexpr explicit Matrix3x3CPP(
        Vector3CPP col0, Vector3CPP col1, Vector3CPP col2 );

    constexpr operator Matrix3x3() const;
    constexpr Vector3CPP operator[]( usize index ) const;
    constexpr Vector3CPP& operator[]( usize index );

    static constexpr Matrix3x3CPP zero();
    static constexpr Matrix3x3CPP identity();
};

typedef Matrix3x3CPP mat3x3;
typedef Matrix3x3CPP mat3;

constexpr attr_always_inline attr_header attr_hot
Matrix3x3CPP::Matrix3x3CPP()
    : m00(0.0f), m01(0.0f), m02(0.0f),
      m10(0.0f), m11(0.0f), m12(0.0f),
      m20(0.0f), m21(0.0f), m22(0.0f) {}
constexpr attr_always_inline attr_header attr_hot
Matrix3x3CPP::Matrix3x3CPP( const Matrix3x3& __pod )
    : __pod(__pod) {}
constexpr attr_always_inline attr_header attr_hot
Matrix3x3CPP::Matrix3x3CPP(
    f32 m00, f32 m01, f32 m02,
    f32 m10, f32 m11, f32 m12,
    f32 m20, f32 m21, f32 m22
) : m00(m00), m01(m01), m02(m02),
    m10(m10), m11(m11), m12(m12),
    m20(m20), m21(m21), m22(m22) {}
constexpr attr_always_inline attr_header attr_hot
Matrix3x3CPP::Matrix3x3CPP(
    Vector3CPP col0, Vector3CPP col1, Vector3CPP col2 )
    : col0(col0), col1(col1), col2(col2) {}

constexpr attr_always_inline attr_header attr_hot
Matrix3x3CPP::operator Matrix3x3() const {
    return *(Matrix3x3*)this;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Matrix3x3CPP::operator[]( usize index ) const {
    return this->col[index];
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP& Matrix3x3CPP::operator[]( usize index ) {
    return this->col[index];
}

constexpr attr_always_inline attr_header attr_hot
Matrix3x3CPP Matrix3x3CPP::zero() {
    return MAT3_ZERO;
}
constexpr attr_always_inline attr_header attr_hot
Matrix3x3CPP Matrix3x3CPP::identity() {
    return MAT3_IDENTITY;
}

attr_always_inline attr_header attr_hot
mat3 add( const mat3& lhs, const mat3& rhs ) {
    return mat3_add( &lhs.__pod, &rhs.__pod );
}
attr_always_inline attr_header attr_hot
mat3 sub( const mat3& lhs, const mat3& rhs ) {
    return mat3_sub( &lhs.__pod, &rhs.__pod );
}
attr_always_inline attr_header attr_hot
mat3 mul( const mat3& lhs, f32 rhs ) {
    return mat3_mul( &lhs.__pod, rhs );
}
attr_always_inline attr_header attr_hot
mat3 mul( f32 lhs, const mat3& rhs ) {
    return mat3_mul( &rhs.__pod, lhs );
}
attr_always_inline attr_header attr_hot
mat3 mul( const mat3& lhs, const mat3& rhs ) {
    return mat3_mul_mat3( &lhs.__pod, &rhs.__pod );
}
attr_always_inline attr_header attr_hot
vec3 mul( const mat3& lhs, vec3 rhs ) {
    return mat3_mul_vec3( &lhs.__pod, rhs );
}
attr_always_inline attr_header attr_hot
mat3 div( const mat3& lhs, f32 rhs ) {
    return mat3_div( &lhs.__pod, rhs );
}

attr_always_inline attr_header attr_hot
mat3 operator+( const mat3& lhs, const mat3& rhs ) {
    return add( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat3& operator+=( mat3& lhs, const mat3& rhs ) {
    return lhs = lhs + rhs;
}
attr_always_inline attr_header attr_hot
mat3 operator-( mat3 lhs, const mat3& rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat3& operator-=( mat3& lhs, const mat3& rhs ) {
    return lhs = lhs - rhs;
}
attr_always_inline attr_header attr_hot
mat3 operator*( const mat3& lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat3 operator*( f32 lhs, const mat3& rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat3 operator*( const mat3& lhs, const mat3& rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
vec3 operator*( const mat3& lhs, vec3 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat3& operator*=( mat3& lhs, f32 rhs ) {
    return lhs = lhs * rhs;
}
attr_always_inline attr_header attr_hot
mat3& operator*=( mat3& lhs, const mat3& rhs ) {
    return lhs = lhs * rhs;
}
attr_always_inline attr_header attr_hot
mat3 operator/( const mat3& lhs, f32 rhs ) {
    return div( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat3& operator/=( mat3& lhs, f32 rhs ) {
    return lhs = lhs / rhs;
}

attr_always_inline attr_header attr_hot
void array_from_mat3( const mat3& m, f32* out_array ) {
    array_from_mat3( &m.__pod, out_array );
}
attr_always_inline attr_header attr_hot
mat3 transpose( const mat3& x ) {
    return mat3_transpose( &x.__pod );
}
attr_always_inline attr_header attr_hot
f32 determinant( const mat3& x ) {
    return mat3_determinant( &x.__pod );
}

#endif /* header guard */
