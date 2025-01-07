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
        struct Matrix3x3 pod;

        Vector3CPP col[3];
        f32 array[9];
    };

    attr_always_inline attr_header constexpr
    Matrix3x3CPP() : col0(), col1(), col2() {}
    attr_always_inline attr_header constexpr
    explicit Matrix3x3CPP(
        f32 m00, f32 m01, f32 m02,
        f32 m10, f32 m11, f32 m12,
        f32 m20, f32 m21, f32 m22
    ) :
    col0( m00, m01, m02 ),
    col1( m10, m11, m12 ),
    col2( m20, m21, m22 ) {}
    attr_always_inline attr_header constexpr
    explicit Matrix3x3CPP(
        Vector3CPP col0, Vector3CPP col1, Vector3CPP col2 ) :
    col0(col0), col1(col1), col2(col2) {}
    attr_always_inline attr_header constexpr
    Matrix3x3CPP( const struct Matrix3x3& m ) : pod(m) {}

    attr_always_inline attr_header constexpr
    operator Matrix3x3() const {
        return *(struct Matrix3x3*)this;
    }

    attr_always_inline attr_header static constexpr
    Matrix3x3CPP zero() {
        return Matrix3x3CPP();
    }
    attr_always_inline attr_header static constexpr
    Matrix3x3CPP identity() {
        return Matrix3x3CPP(
            1.0, 0.0, 0.0, 
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0 );
    }

    attr_always_inline attr_header static constexpr
    Matrix3x3CPP from_array( const f32 array[9] ) {
        return *(Matrix3x3CPP*)array;
    }
    attr_always_inline attr_header constexpr
    void to_array( f32* out_array ) {
        for( usize i = 0; i < 9; ++i ) {
            out_array[i] = array[i];
        }
    }

    attr_always_inline attr_header constexpr
    const Vector3CPP& operator[]( usize idx ) const {
        return col[idx];
    }
    attr_always_inline attr_header constexpr
    Vector3CPP& operator[]( usize idx ) {
        return col[idx];
    }
};
attr_always_inline attr_header
Matrix3x3CPP add( const Matrix3x3CPP& lhs, const Matrix3x3CPP& rhs ) {
    return mat3_add( &lhs.pod, &rhs.pod );
}
attr_always_inline attr_header
Matrix3x3CPP sub( const Matrix3x3CPP& lhs, const Matrix3x3CPP& rhs ) {
    return mat3_sub( &lhs.pod, &rhs.pod );
}
attr_always_inline attr_header
Matrix3x3CPP mul( const Matrix3x3CPP& lhs, f32 rhs ) {
    return mat3_mul( &lhs.pod, rhs );
}
attr_always_inline attr_header
Matrix3x3CPP mul( f32 lhs, const Matrix3x3CPP& rhs ) {
    return mat3_mul( &rhs.pod, lhs );
}
attr_always_inline attr_header
Matrix3x3CPP mul( const Matrix3x3CPP& lhs, const Matrix3x3CPP& rhs ) {
    return mat3_mul_mat3( &lhs.pod, &rhs.pod );
}
attr_always_inline attr_header
Matrix3x3CPP div( const Matrix3x3CPP& lhs, f32 rhs ) {
    return mat3_div( &lhs.pod, rhs );
}
attr_always_inline attr_header
Matrix3x3 transpose( const Matrix3x3& m ) {
    return mat3_transpose( &m );
}
attr_always_inline attr_header
f32 determinant( const Matrix3x3& m ) {
    return mat3_determinant( &m );
}

attr_always_inline attr_header
Matrix3x3CPP operator+( const Matrix3x3& lhs, const Matrix3x3& rhs ) {
    return add( lhs, rhs );
}
attr_always_inline attr_header
Matrix3x3CPP operator-( const Matrix3x3& lhs, const Matrix3x3& rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline attr_header
Matrix3x3CPP operator*( const Matrix3x3& lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Matrix3x3CPP operator*( f32 lhs, const Matrix3x3& rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Matrix3x3CPP operator*( const Matrix3x3& lhs, const Matrix3x3& rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Matrix3x3CPP operator/( const Matrix3x3& lhs, f32 rhs ) {
    return div( lhs, rhs );
}


#endif /* header guard */
