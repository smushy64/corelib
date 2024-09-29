#if !defined(CORE_CPP_MATH_MATRIX3X3_HPP)
#define CORE_CPP_MATH_MATRIX3X3_HPP
/**
 * @file   matrix3x3.hpp
 * @brief  C++ Matrix3x3.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/

struct Matrix3x3CPP;

#if !defined(CORE_MATH_MATRIX3X3_H)
    #include "core/math/matrix3x3.h"
#endif

struct Matrix3x3CPP : public Matrix3x3 {
    attr_header Matrix3x3CPP() : Matrix3x3{
        .m00=0,.m01=0,.m02=0,
        .m10=0,.m11=0,.m12=0,
        .m20=0,.m21=0,.m22=0
    } {}
    attr_header Matrix3x3CPP(
        f32 m00, f32 m01, f32 m02,
        f32 m10, f32 m11, f32 m12,
        f32 m20, f32 m21, f32 m22
    ) : Matrix3x3{
        .m00=m00, .m01=m01, .m02=m02,
        .m10=m10, .m11=m11, .m12=m12,
        .m20=m20, .m21=m21, .m22=m22,
    } {}
    attr_header Matrix3x3CPP( const Matrix3x3& m ) : Matrix3x3{m} {}
    attr_header explicit Matrix3x3CPP( const f32 a[9] ) :
        Matrix3x3CPP(from_array(a)) {}

    attr_header static Matrix3x3CPP zero(void) {
        return MAT3_ZERO;
    }
    attr_header static Matrix3x3CPP identity(void) {
        return MAT3_IDENTITY;
    }
    attr_header static Matrix3x3CPP from_array( const f32 a[9] ) {
        return m3_from_array( a );
    }
    
    attr_header void to_array( f32 out_array[9] ) const {
        m3_to_array( this, out_array );
    }
    attr_header Matrix3x3CPP add( const Matrix3x3CPP& rhs ) const {
        return m3_add( this, &rhs );
    }
    attr_header Matrix3x3CPP sub( const Matrix3x3CPP& rhs ) const {
        return m3_sub( this, &rhs );
    }
    attr_header Matrix3x3CPP mul( f32 rhs ) const {
        return m3_mul( this, rhs );
    }
    attr_header Matrix3x3CPP mul( const Matrix3x3CPP& rhs ) const {
        return m3_mul_m3( this, &rhs );
    }
    attr_header Matrix3x3CPP div( f32 rhs ) const {
        return m3_div( this, rhs );
    }
    attr_header Matrix3x3CPP transpose(void) const {
        return m3_transpose( this );
    }
    attr_header f32 determinant(void) const {
        return m3_determinant( this );
    }

    attr_header const Vector3CPP& operator[]( usize idx ) const {
        const Vector3* ptr = c + idx;
        return *(const Vector3CPP*)ptr;
    }
    attr_header Vector3CPP& operator[]( usize idx ) {
        Vector3* ptr = c + idx;
        return *(Vector3CPP*)ptr;
    }
    attr_header Matrix3x3CPP& operator+=( const Matrix3x3CPP& rhs ) {
        return *this = add( rhs );
    }
    attr_header Matrix3x3CPP& operator-=( const Matrix3x3CPP& rhs ) {
        return *this = sub( rhs );
    }
    attr_header Matrix3x3CPP& operator*=( f32 rhs ) {
        return *this = mul( rhs );
    }
    attr_header Matrix3x3CPP& operator*=( const Matrix3x3CPP& rhs ) {
        return *this = mul( rhs );
    }
    attr_header Matrix3x3CPP& operator/=( f32 rhs ) {
        return *this = div( rhs );
    }
};
attr_header Matrix3x3 operator+(
    const Matrix3x3& lhs, const Matrix3x3& rhs
) {
    return m3_add( &lhs, &rhs );
}
attr_header Matrix3x3 operator-(
    const Matrix3x3& lhs, const Matrix3x3& rhs
) {
    return m3_sub( &lhs, &rhs );
}
attr_header Matrix3x3 operator*(
    const Matrix3x3& lhs, const Matrix3x3& rhs
) {
    return m3_mul_m3( &lhs, &rhs );
}
attr_header Matrix3x3 operator*(
    const Matrix3x3& lhs, f32 rhs
) {
    return m3_mul( &lhs, rhs );
}
attr_header Matrix3x3 operator*(
    f32 lhs, const Matrix3x3& rhs
) {
    return m3_mul( &rhs, lhs );
}
attr_header Matrix3x3 operator/(
    const Matrix3x3& lhs, f32 rhs
) {
    return m3_div( &lhs, rhs );
}

attr_header Matrix3x3 add(
    const Matrix3x3& lhs, const Matrix3x3& rhs
) {
    return lhs + rhs;
}
attr_header Matrix3x3 sub(
    const Matrix3x3& lhs, const Matrix3x3& rhs
) {
    return lhs - rhs;
}
attr_header Matrix3x3 mul(
    const Matrix3x3& lhs, const Matrix3x3& rhs
) {
    return lhs * rhs;
}
attr_header Matrix3x3 mul(
    const Matrix3x3& lhs, f32 rhs
) {
    return lhs * rhs;
}
attr_header Matrix3x3 mul(
    f32 lhs, const Matrix3x3& rhs
) {
    return lhs * rhs;
}
attr_header Matrix3x3 div(
    const Matrix3x3& lhs, f32 rhs
) {
    return lhs / rhs;
}
attr_header Matrix3x3 transpose( const Matrix3x3& m ) {
    return m3_transpose( &m );
}
attr_header f32 determinant( const Matrix3x3& m ) {
    return m3_determinant( &m );
}

#endif /* header guard */
