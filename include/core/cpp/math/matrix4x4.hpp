#if !defined(CORE_CPP_MATH_MATRIX4X4_HPP)
#define CORE_CPP_MATH_MATRIX4X4_HPP
/**
 * @file   matrix4x4.hpp
 * @brief  C++ Matrix4x4.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/
struct Matrix4x4CPP;

#if !defined(CORE_MATH_MATRIX4X4_H)
    #include "core/math/matrix4x4.h"
#endif
#include "core/math/quaternion.h" // IWYU pragma: keep 

struct Matrix4x4CPP {
    union {
        struct {
            union {
                struct { f32 m00, m01, m02, m03; };
                Vector4CPP col0;
            };
            union {
                struct { f32 m10, m11, m12, m13; };
                Vector4CPP col1;
            };
            union {
                struct { f32 m20, m21, m22, m23; };
                Vector4CPP col2;
            };
            union {
                struct { f32 m30, m31, m32, m33; };
                Vector4CPP col3;
            };
        };
        struct Matrix4x4 pod;

        Vector4CPP col[4];
        f32 array[16];
    };

    attr_always_inline attr_header constexpr
    Matrix4x4CPP() : col0(), col1(), col2(), col3() {}
    attr_always_inline attr_header constexpr
    explicit Matrix4x4CPP(
        f32 m00, f32 m01, f32 m02, f32 m03,
        f32 m10, f32 m11, f32 m12, f32 m13,
        f32 m20, f32 m21, f32 m22, f32 m23,
        f32 m30, f32 m31, f32 m32, f32 m33
    ) :
    col0( m00, m01, m02, m03 ),
    col1( m10, m11, m12, m13 ),
    col2( m20, m21, m22, m23 ),
    col3( m30, m31, m32, m33 ) {}
    attr_always_inline attr_header constexpr
    explicit Matrix4x4CPP(
        Vector4CPP col0, Vector4CPP col1, Vector4CPP col2, Vector4CPP col3 ) :
    col0(col0), col1(col1), col2(col2), col3(col3) {}
    attr_always_inline attr_header constexpr
    Matrix4x4CPP( const struct Matrix4x4& m ) : pod(m) {}

    attr_always_inline attr_header constexpr
    operator Matrix4x4() const {
        return *(struct Matrix4x4*)this;
    }

    attr_always_inline attr_header static constexpr
    Matrix4x4CPP zero() {
        return Matrix4x4CPP();
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP identity() {
        return Matrix4x4CPP(
            1.0, 0.0, 0.0, 0.0, 
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0 );
    }

    attr_always_inline attr_header static constexpr
    Matrix4x4CPP view(
        Vector3CPP position, Vector3CPP target, Vector3CPP up = Vector3CPP::up()
    ) {
        return mat4_view( position.pod, target.pod, up.pod );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP view( Vector2CPP position, Vector2CPP up = Vector2CPP::up() ) {
        return mat4_view_2d( position.pod, up.pod );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP ortho(
        f32 left, f32 right,
        f32 bottom, f32 top,
        f32 clip_near = 0.0001f, f32 clip_far = 1000.0f
    ) {
        return mat4_ortho( left, right, bottom, top, clip_near, clip_far );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP perspective(
        f32 field_of_view, f32 aspect_ratio,
        f32 clip_near = 0.0001f, f32 clip_far = 1000.0f
    ) {
        return mat4_perspective( field_of_view, aspect_ratio, clip_near, clip_far );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP translation( f32 x, f32 y, f32 z ) {
        return mat4_translation( x, y, z );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP translation( Vector3CPP translation ) {
        return mat4_translation_vec3( translation );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP translation( f32 x, f32 y ) {
        return mat4_translation_2d( x, y );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP translation( Vector2CPP translation ) {
        return mat4_translation_vec2( translation );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP rotation_pitch( f32 pitch ) {
        return mat4_rotation_pitch( pitch );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP rotation_yaw( f32 yaw ) {
        return mat4_rotation_yaw( yaw );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP rotation_roll( f32 roll ) {
        return mat4_rotation_roll( roll );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP rotation( f32 pitch, f32 yaw, f32 roll ) {
        return mat4_rotation_euler( pitch, yaw, roll );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP rotation( Vector3CPP euler ) {
        return mat4_rotation_euler_vec3( euler );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP rotation( QuaternionCPP rotation ) {
        return mat4_rotation( rotation );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP rotation( f32 rotation ) {
        return mat4_rotation_2d( rotation );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP scale( f32 width, f32 height, f32 length ) {
        return mat4_scale( width, height, length );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP scale( f32 width, f32 height ) {
        return mat4_scale_2d( width, height );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP scale( Vector3CPP dimensions ) {
        return mat4_scale_vec3( dimensions );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP scale( Vector2CPP dimensions ) {
        return mat4_scale_vec2( dimensions );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP transform(
        Vector3CPP translation, QuaternionCPP rotation, Vector3CPP scale
    ) {
        return mat4_transform( translation.pod, rotation.pod, scale.pod );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP transform(
        Vector3CPP translation, Vector3CPP rotation, Vector3CPP scale
    ) {
        return mat4_transform_euler( translation.pod, rotation.pod, scale.pod );
    }
    attr_always_inline attr_header static constexpr
    Matrix4x4CPP transform(
        Vector2CPP translation, f32 rotation, Vector2CPP scale
    ) {
        return mat4_transform_2d( translation.pod, rotation, scale.pod );
    }

    attr_always_inline attr_header static constexpr
    Matrix4x4CPP from_array( const f32 array[16] ) {
        return *(Matrix4x4CPP*)array;
    }
    attr_always_inline attr_header constexpr
    void to_array( f32* out_array ) {
        for( usize i = 0; i < 16; ++i ) {
            out_array[i] = array[i];
        }
    }

    attr_always_inline attr_header constexpr
    Matrix3x3CPP submatrix( u32 column, u32 row ) const {
        return mat4_submatrix( &this->pod, column, row );
    }
    attr_always_inline attr_header constexpr
    f32 minor( u32 column, u32 row ) const {
        return mat4_minor( &this->pod, column, row );
    }
    attr_always_inline attr_header constexpr
    f32 cofactor( u32 column, u32 row ) const {
        return mat4_cofactor( &this->pod, column, row );
    }
    attr_always_inline attr_header constexpr
    Matrix3x3CPP normal_matrix() const {
        return mat4_normal_matrix( &this->pod );
    }
    attr_always_inline attr_header constexpr
    b32 normal_matrix( Matrix3x3CPP* out_normal ) const {
        return mat4_normal_matrix_checked( &this->pod, &out_normal->pod );
    }

    attr_always_inline attr_header constexpr
    const Vector4CPP& operator[]( usize idx ) const {
        return col[idx];
    }
    attr_always_inline attr_header constexpr
    Vector4CPP& operator[]( usize idx ) {
        return col[idx];
    }
};
attr_always_inline attr_header
Matrix4x4CPP add( const Matrix4x4CPP& lhs, const Matrix4x4CPP& rhs ) {
    return mat4_add( &lhs.pod, &rhs.pod );
}
attr_always_inline attr_header
Matrix4x4CPP sub( const Matrix4x4CPP& lhs, const Matrix4x4CPP& rhs ) {
    return mat4_sub( &lhs.pod, &rhs.pod );
}
attr_always_inline attr_header
Matrix4x4CPP mul( const Matrix4x4CPP& lhs, f32 rhs ) {
    return mat4_mul( &lhs.pod, rhs );
}
attr_always_inline attr_header
Matrix4x4CPP mul( f32 lhs, const Matrix4x4CPP& rhs ) {
    return mat4_mul( &rhs.pod, lhs );
}
attr_always_inline attr_header
Matrix4x4CPP mul( const Matrix4x4CPP& lhs, const Matrix4x4CPP& rhs ) {
    return mat4_mul_mat4( &lhs.pod, &rhs.pod );
}
attr_always_inline attr_header
Vector3CPP mul( const Matrix4x4CPP& lhs, Vector3CPP rhs ) {
    return mat4_mul_vec3( &lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Vector4CPP mul( const Matrix4x4CPP& lhs, Vector4CPP rhs ) {
    return mat4_mul_vec4( &lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Matrix4x4CPP div( const Matrix4x4CPP& lhs, f32 rhs ) {
    return mat4_div( &lhs.pod, rhs );
}
attr_always_inline attr_header
Matrix4x4CPP transpose( const Matrix4x4CPP& m ) {
    return mat4_transpose( &m.pod );
}
attr_always_inline attr_header
f32 determinant( const Matrix4x4CPP& m ) {
    return mat4_determinant( &m.pod );
}
attr_always_inline attr_header
Matrix4x4CPP cofactor_matrix( const Matrix4x4CPP& m ) {
    return mat4_cofactor_matrix( &m.pod );
}
attr_always_inline attr_header
Matrix4x4CPP adjoint( const Matrix4x4CPP& m ) {
    return mat4_adjoint( &m.pod );
}
attr_always_inline attr_header
Matrix4x4CPP inverse( const Matrix4x4CPP& m ) {
    return mat4_inverse( &m.pod );
}
attr_always_inline attr_header
b32 inverse_checked( const Matrix4x4CPP& m, Matrix4x4CPP* out_inverse ) {
    return mat4_inverse_checked( &m.pod, &out_inverse->pod );
}

attr_always_inline attr_header
Matrix4x4CPP operator+( const Matrix4x4CPP& lhs, const Matrix4x4CPP& rhs ) {
    return add( lhs, rhs );
}
attr_always_inline attr_header
Matrix4x4CPP operator-( const Matrix4x4CPP& lhs, const Matrix4x4CPP& rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline attr_header
Matrix4x4CPP operator*( const Matrix4x4CPP& lhs, const Matrix4x4CPP& rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Matrix4x4CPP operator*( f32 lhs, const Matrix4x4CPP& rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Matrix4x4CPP operator*( const Matrix4x4CPP& lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP operator*( const Matrix4x4CPP& lhs, Vector3CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Vector4CPP operator*( const Matrix4x4CPP& lhs, Vector4CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Matrix4x4CPP operator/( const Matrix4x4CPP& lhs, f32 rhs ) {
    return div( lhs, rhs );
}

#endif /* header guard */
