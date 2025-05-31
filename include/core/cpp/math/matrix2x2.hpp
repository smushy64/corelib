#if !defined(CORE_CPP_MATH_MATRIX2X2_HPP) && defined(__cplusplus)
#define CORE_CPP_MATH_MATRIX2X2_HPP
/**
 * @file   matrix2x2.hpp
 * @brief  C++ Math: Matrix2x2.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 30, 2025
*/
#if !defined(CORE_MATH_MATRIX2X2_H)
    #include "core/math/matrix2x2.h"
#endif

struct Matrix2x2CPP {
    union {
        struct {
            union {
                struct { f32 m00, m01; };
                Vector2CPP col0;
            };
            union {
                struct { f32 m10, m11; };
                Vector2CPP col1;
            };
        };
        struct Matrix2x2 __pod;

        Vector2CPP col[2];
        f32 array[4];
    };

    constexpr Matrix2x2CPP();
    constexpr Matrix2x2CPP( const Matrix2x2& __pod );
    constexpr explicit Matrix2x2CPP( f32 m00, f32 m01, f32 m10, f32 m11 );
    constexpr explicit Matrix2x2CPP( Vector2CPP col0, Vector2CPP col1 );

    constexpr operator Matrix2x2() const;
    constexpr Vector2CPP operator[]( usize index ) const;
    constexpr Vector2CPP& operator[]( usize index );

    static constexpr Matrix2x2CPP zero();
    static constexpr Matrix2x2CPP identity();
};

typedef Matrix2x2CPP mat2x2;
typedef Matrix2x2CPP mat2;

constexpr attr_always_inline attr_header attr_hot
Matrix2x2CPP::Matrix2x2CPP()
    : m00(0.0f), m01(0.0f), m10(0.0f), m11(0.0f) {}
constexpr attr_always_inline attr_header attr_hot
Matrix2x2CPP::Matrix2x2CPP( const Matrix2x2& __pod )
    : __pod(__pod) {}
constexpr attr_always_inline attr_header attr_hot
Matrix2x2CPP::Matrix2x2CPP( f32 m00, f32 m01, f32 m10, f32 m11 )
    : m00(m00), m01(m01), m10(m10), m11(m11) {}
constexpr attr_always_inline attr_header attr_hot
Matrix2x2CPP::Matrix2x2CPP( Vector2CPP col0, Vector2CPP col1 )
    : col0(col0), col1(col1) {}

constexpr attr_always_inline attr_header attr_hot
Matrix2x2CPP::operator Matrix2x2() const {
    return *(Matrix2x2*)this;
}
constexpr attr_always_inline attr_header attr_hot
Vector2CPP Matrix2x2CPP::operator[]( usize index ) const {
    return this->col[index];
}
constexpr attr_always_inline attr_header attr_hot
Vector2CPP& Matrix2x2CPP::operator[]( usize index ) {
    return this->col[index];
}

constexpr attr_always_inline attr_header attr_hot
Matrix2x2CPP Matrix2x2CPP::zero() {
    return MAT2_ZERO;
}
constexpr attr_always_inline attr_header attr_hot
Matrix2x2CPP Matrix2x2CPP::identity() {
    return MAT2_IDENTITY;
}

attr_always_inline attr_header attr_hot
mat2 add( mat2 lhs, mat2 rhs ) {
    return mat2_add( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat2 sub( mat2 lhs, mat2 rhs ) {
    return mat2_sub( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat2 mul( mat2 lhs, f32 rhs ) {
    return mat2_mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat2 mul( f32 lhs, mat2 rhs ) {
    return mat2_mul( rhs, lhs );
}
attr_always_inline attr_header attr_hot
mat2 mul( mat2 lhs, mat2 rhs ) {
    return mat2_mul_mat2( lhs, rhs );
}
attr_always_inline attr_header attr_hot
vec2 mul( mat2 lhs, vec2 rhs ) {
    return mat2_mul_vec2( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat2 div( mat2 lhs, f32 rhs ) {
    return mat2_div( lhs, rhs );
}

attr_always_inline attr_header attr_hot
mat2 operator+( mat2 lhs, mat2 rhs ) {
    return add( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat2& operator+=( mat2& lhs, mat2 rhs ) {
    return lhs = lhs + rhs;
}
attr_always_inline attr_header attr_hot
mat2 operator-( mat2 lhs, mat2 rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat2& operator-=( mat2& lhs, mat2 rhs ) {
    return lhs = lhs - rhs;
}
attr_always_inline attr_header attr_hot
mat2 operator*( mat2 lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat2 operator*( f32 lhs, mat2 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat2 operator*( mat2 lhs, mat2 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
vec2 operator*( mat2 lhs, vec2 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat2& operator*=( mat2& lhs, f32 rhs ) {
    return lhs = lhs * rhs;
}
attr_always_inline attr_header attr_hot
mat2& operator*=( mat2& lhs, mat2 rhs ) {
    return lhs = lhs * rhs;
}
attr_always_inline attr_header attr_hot
mat2 operator/( mat2 lhs, f32 rhs ) {
    return div( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat2& operator/=( mat2& lhs, f32 rhs ) {
    return lhs = lhs / rhs;
}

attr_always_inline attr_header attr_hot
mat2 transpose( mat2 x ) {
    return mat2_transpose( x );
}
attr_always_inline attr_header attr_hot
f32 determinant( mat2 x ) {
    return mat2_determinant( x );
}

#endif /* header guard */
