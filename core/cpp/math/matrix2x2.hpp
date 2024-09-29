#if !defined(CORE_CPP_MATH_MATRIX2X2_HPP)
#define CORE_CPP_MATH_MATRIX2X2_HPP
/**
 * @file   matrix2x2.hpp
 * @brief  C++ Matrix2x2.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/

struct Matrix2x2CPP;

#if !defined(CORE_MATH_MATRIX2X2_H)
    #include "core/math/matrix2x2.h"
#endif

struct Matrix2x2CPP : public Matrix2x2 {
    attr_header Matrix2x2CPP() : Matrix2x2{.m00=0,.m01=0,.m10=0,.m11=0} {}
    attr_header Matrix2x2CPP( f32 m00, f32 m01, f32 m10, f32 m11 ) :
        Matrix2x2{ .m00=m00,.m01=m01,.m10=m10,.m11=m11 } {}
    attr_header Matrix2x2CPP( const Matrix2x2& m ) : Matrix2x2{m} {}
    attr_header explicit Matrix2x2CPP( const f32 a[4] ) :
        Matrix2x2CPP( from_array(a) ) {}

    attr_header static Matrix2x2CPP zero(void) {
        return MAT2_ZERO;
    }
    attr_header static Matrix2x2CPP identity(void) {
        return MAT2_IDENTITY;
    }
    attr_header static Matrix2x2CPP from_array( const f32 a[4] ) {
        return m2_from_array( a );
    }
    
    attr_header void to_array( f32 out_array[4] ) const {
        m2_to_array( *this, out_array );
    }
    attr_header Matrix2x2CPP add( const Matrix2x2CPP& rhs ) const {
        return m2_add( *this, rhs );
    }
    attr_header Matrix2x2CPP sub( const Matrix2x2CPP& rhs ) const {
        return m2_sub( *this, rhs );
    }
    attr_header Matrix2x2CPP mul( f32 rhs ) const {
        return m2_mul( *this, rhs );
    }
    attr_header Matrix2x2CPP mul( const Matrix2x2CPP& rhs ) const {
        return m2_mul_m2( *this, rhs );
    }
    attr_header Matrix2x2CPP div( f32 rhs ) const {
        return m2_div( *this, rhs );
    }
    attr_header Matrix2x2CPP transpose(void) const {
        return m2_transpose( *this );
    }
    attr_header f32 determinant(void) const {
        return m2_determinant( *this );
    }

    attr_header const Vector2CPP& operator[]( usize idx ) const {
        const Vector2* ptr = c + idx;
        return *(const Vector2CPP*)ptr;
    }
    attr_header Vector2CPP& operator[]( usize idx ) {
        Vector2* ptr = c + idx;
        return *(Vector2CPP*)ptr;
    }
    attr_header Matrix2x2CPP& operator+=( const Matrix2x2CPP& rhs ) {
        return *this = add( rhs );
    }
    attr_header Matrix2x2CPP& operator-=( const Matrix2x2CPP& rhs ) {
        return *this = sub( rhs );
    }
    attr_header Matrix2x2CPP& operator*=( f32 rhs ) {
        return *this = mul( rhs );
    }
    attr_header Matrix2x2CPP& operator*=( const Matrix2x2CPP& rhs ) {
        return *this = mul( rhs );
    }
    attr_header Matrix2x2CPP& operator/=( f32 rhs ) {
        return *this = div( rhs );
    }
};
attr_header Matrix2x2 operator+(
    const Matrix2x2& lhs, const Matrix2x2& rhs
) {
    return m2_add( lhs, rhs );
}
attr_header Matrix2x2 operator-(
    const Matrix2x2& lhs, const Matrix2x2& rhs
) {
    return m2_sub( lhs, rhs );
}
attr_header Matrix2x2 operator*(
    const Matrix2x2& lhs, const Matrix2x2& rhs
) {
    return m2_mul_m2( lhs, rhs );
}
attr_header Matrix2x2 operator*(
    const Matrix2x2& lhs, f32 rhs
) {
    return m2_mul( lhs, rhs );
}
attr_header Matrix2x2 operator*(
    f32 lhs, const Matrix2x2& rhs
) {
    return m2_mul( rhs, lhs );
}
attr_header Matrix2x2 operator/(
    const Matrix2x2& lhs, f32 rhs
) {
    return m2_div( lhs, rhs );
}

attr_header Matrix2x2 add(
    const Matrix2x2& lhs, const Matrix2x2& rhs
) {
    return lhs + rhs;
}
attr_header Matrix2x2 sub(
    const Matrix2x2& lhs, const Matrix2x2& rhs
) {
    return lhs - rhs;
}
attr_header Matrix2x2 mul(
    const Matrix2x2& lhs, const Matrix2x2& rhs
) {
    return lhs * rhs;
}
attr_header Matrix2x2 mul(
    const Matrix2x2& lhs, f32 rhs
) {
    return lhs * rhs;
}
attr_header Matrix2x2 mul(
    f32 lhs, const Matrix2x2& rhs
) {
    return lhs * rhs;
}
attr_header Matrix2x2 div(
    const Matrix2x2& lhs, f32 rhs
) {
    return lhs / rhs;
}
attr_header Matrix2x2 transpose( const Matrix2x2& m ) {
    return m2_transpose( m );
}
attr_header f32 determinant( const Matrix2x2& m ) {
    return m2_determinant( m );
}

#endif /* header guard */
