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
        struct Matrix2x2 pod;

        Vector2CPP col[2];
        f32 array[4];
    };

    attr_always_inline attr_header constexpr
    Matrix2x2CPP() : col0(), col1() {}
    attr_always_inline attr_header constexpr
    explicit Matrix2x2CPP(
        f32 m00, f32 m01,
        f32 m10, f32 m11
    ) :
    col0( m00, m01 ),
    col1( m10, m11 ) {}
    attr_always_inline attr_header constexpr
    explicit Matrix2x2CPP( Vector2CPP col0, Vector2CPP col1 ) :
        col0(col0), col1(col1) {}
    attr_always_inline attr_header constexpr
    Matrix2x2CPP( const struct Matrix2x2& m ) : pod(m) {}

    attr_always_inline attr_header constexpr
    operator Matrix2x2() const {
        return *(struct Matrix2x2*)this;
    }

    attr_always_inline attr_header static constexpr
    Matrix2x2CPP zero() {
        return Matrix2x2CPP();
    }
    attr_always_inline attr_header static constexpr
    Matrix2x2CPP identity() {
        return Matrix2x2CPP(
            1.0, 0.0,
            0.0, 1.0 );
    }

    attr_always_inline attr_header static constexpr
    Matrix2x2CPP from_array( const f32 array[4] ) {
        return *(Matrix2x2CPP*)array;
    }
    attr_always_inline attr_header constexpr
    void to_array( f32* out_array ) {
        out_array[0] = array[0];
        out_array[1] = array[1];
        out_array[2] = array[2];
        out_array[3] = array[3];
    }

    attr_always_inline attr_header constexpr
    const Vector2CPP& operator[]( usize idx ) const {
        return col[idx];
    }
    attr_always_inline attr_header constexpr
    Vector2CPP& operator[]( usize idx ) {
        return col[idx];
    }
};
attr_always_inline attr_header
Matrix2x2CPP add( Matrix2x2CPP lhs, Matrix2x2CPP rhs ) {
    return mat2_add( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Matrix2x2CPP sub( Matrix2x2CPP lhs, Matrix2x2CPP rhs ) {
    return mat2_sub( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Matrix2x2CPP mul( Matrix2x2CPP lhs, f32 rhs ) {
    return mat2_mul( lhs.pod, rhs );
}
attr_always_inline attr_header
Matrix2x2CPP mul( f32 lhs, Matrix2x2CPP rhs ) {
    return mat2_mul( rhs.pod, lhs );
}
attr_always_inline attr_header
Matrix2x2CPP mul( Matrix2x2CPP lhs, Matrix2x2CPP rhs ) {
    return mat2_mul_mat2( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Matrix2x2CPP div( Matrix2x2CPP lhs, f32 rhs ) {
    return mat2_div( lhs.pod, rhs );
}
attr_always_inline attr_header
Matrix2x2CPP transpose( Matrix2x2CPP m ) {
    return mat2_transpose( m.pod );
}
attr_always_inline attr_header
f32 determinant( Matrix2x2CPP m ) {
    return mat2_determinant( m.pod );
}

attr_always_inline attr_header
Matrix2x2CPP operator+( Matrix2x2CPP lhs, Matrix2x2CPP rhs ) {
    return add( lhs, rhs );
}
attr_always_inline attr_header
Matrix2x2CPP operator-( Matrix2x2CPP lhs, Matrix2x2CPP rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline attr_header
Matrix2x2CPP operator*( Matrix2x2CPP lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Matrix2x2CPP operator*( f32 lhs, Matrix2x2CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Matrix2x2CPP operator*( Matrix2x2CPP lhs, Matrix2x2CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Matrix2x2CPP operator/( Matrix2x2CPP lhs, f32 rhs ) {
    return div( lhs, rhs );
}

#endif /* header guard */
