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
#include "core/math/quaternion.h"

struct Matrix4x4CPP : public Matrix4x4 {
    attr_header Matrix4x4CPP() : Matrix4x4{
        .m00=0, .m01=0, .m02=0, .m03=0,
        .m10=0, .m11=0, .m12=0, .m13=0,
        .m20=0, .m21=0, .m22=0, .m23=0,
        .m30=0, .m31=0, .m32=0, .m33=0,
    } {}
    attr_header Matrix4x4CPP(
        f32 m00, f32 m01, f32 m02, f32 m03,
        f32 m10, f32 m11, f32 m12, f32 m13,
        f32 m20, f32 m21, f32 m22, f32 m23,
        f32 m30, f32 m31, f32 m32, f32 m33
    ) : Matrix4x4{
        .m00=m00, .m01=m01, .m02=m02, .m03=m03,
        .m10=m10, .m11=m11, .m12=m12, .m13=m13,
        .m20=m20, .m21=m21, .m22=m22, .m23=m23,
        .m30=m30, .m31=m31, .m32=m32, .m33=m33,
    } {}
    attr_header Matrix4x4CPP( const Matrix4x4& m ) : Matrix4x4{m} {}
    attr_header explicit Matrix4x4CPP( const f32 a[16] ) :
        Matrix4x4CPP(from_array(a)) {}

    attr_header static Matrix4x4CPP zero() {
        return MAT4_ZERO;
    }
    attr_header static Matrix4x4CPP identity() {
        return MAT4_IDENTITY;
    }
    attr_header static Matrix4x4CPP from_array( const f32 a[16] ) {
        return m4_from_array( a );
    }
    attr_header static Matrix4x4CPP view(
        f32 position_x, f32 position_y, f32 position_z,
        f32 target_x, f32 target_y, f32 target_z,
        f32 up_x, f32 up_y, f32 up_z
    ) {
        return m4_view(
            v3(position_x,position_y,position_z),
            v3(target_x,target_y,target_z),
            v3(up_x,up_y,up_z) );
    }
    attr_header static Matrix4x4CPP view(
        const Vector3& position, const Vector3& target, const Vector3& up
    ) {
        return m4_view( position, target, up );
    }
    attr_header static Matrix4x4CPP view_2d(
        f32 position_x, f32 position_y,
        f32 up_x, f32 up_y
    ) {
        return m4_view_2d( v2(position_x,position_y), v2(up_x,up_y) );
    }
    attr_header static Matrix4x4CPP view_2d(
        const Vector2& position, const Vector2& up
    ) {
        return m4_view_2d( position, up );
    }
    attr_header static Matrix4x4CPP ortho(
        f32 left, f32 right,
        f32 bottom, f32 top,
        f32 clip_near, f32 clip_far
    ) {
        return m4_ortho( left, right, bottom, top, clip_near, clip_far );
    }
    attr_header static Matrix4x4CPP ortho(
        f32 left, f32 right,
        f32 bottom, f32 top
    ) {
        return ortho( left, right, bottom, top, 0.0001f, 10000.0f );
    }
    attr_header static Matrix4x4CPP perspective(
        f32 field_of_view, f32 aspect_ratio, f32 clip_near, f32 clip_far
    ) {
        return m4_perspective( field_of_view, aspect_ratio, clip_near, clip_far );
    }
    attr_header static Matrix4x4CPP perspective(
        f32 field_of_view, f32 aspect_ratio
    ) {
        return perspective( field_of_view, aspect_ratio, 0.0001f, 10000.0f );
    }
    attr_header static Matrix4x4CPP translation( f32 x, f32 y, f32 z ) {
        return m4_translation( x, y, z );
    }
    attr_header static Matrix4x4CPP translation( const Vector3& translation ) {
        return m4_translation_v3( translation );
    }
    attr_header static Matrix4x4CPP translation_2d( f32 x, f32 y ) {
        return m4_translation_2d( x, y );
    }
    attr_header static Matrix4x4CPP translation_2d( const Vector2& translation ) {
        return m4_translation_v2( translation );
    }
    attr_header static Matrix4x4CPP rotation_pitch( f32 pitch ) {
        return m4_rotation_pitch( pitch );
    }
    attr_header static Matrix4x4CPP rotation_yaw( f32 yaw ) {
        return m4_rotation_yaw( yaw );
    }
    attr_header static Matrix4x4CPP rotation_roll( f32 roll ) {
        return m4_rotation_roll( roll );
    }
    attr_header static Matrix4x4CPP rotation( f32 pitch, f32 yaw, f32 roll ) {
        return m4_rotation_euler( pitch, yaw, roll );
    }
    attr_header static Matrix4x4CPP rotation( const Vector3& euler ) {
        return m4_rotation_euler_v3( euler );
    }
    attr_header static Matrix4x4CPP rotation( const Quaternion& rotation ) {
        return m4_rotation( rotation );
    }
    attr_header static Matrix4x4CPP rotation_2d( f32 rotation ) {
        return m4_rotation_2d( rotation );
    }
    attr_header static Matrix4x4CPP scale( f32 width, f32 height, f32 length ) {
        return m4_scale( width, height, length );
    }
    attr_header static Matrix4x4CPP scale( const Vector3& scale ) {
        return m4_scale_v3( scale );
    }
    attr_header static Matrix4x4CPP scale_2d( f32 width, f32 height ) {
        return m4_scale_2d( width, height );
    }
    attr_header static Matrix4x4CPP scale_2d( const Vector2& scale ) {
        return m4_scale_v2( scale );
    }
    attr_header static Matrix4x4CPP transform(
        const Vector3& translation, const Quaternion& rotation,
        const Vector3& scale
    ) {
        return m4_transform( translation, rotation, scale );
    }
    attr_header static Matrix4x4CPP transform(
        const Vector3& translation, const Vector3& euler,
        const Vector3& scale
    ) {
        return m4_transform_euler( translation, euler, scale );
    }
    attr_header static Matrix4x4CPP transform_2d(
        const Vector2& translation, f32 rotation,
        const Vector2& scale
    ) {
        return m4_transform_2d( translation, rotation, scale );
    }

    attr_header void to_array( f32 out_array[16] ) {
        m4_to_array( this, out_array );
    }
    attr_header Matrix4x4CPP add( const Matrix4x4& rhs ) const {
        return m4_add( this, &rhs );
    }
    attr_header Matrix4x4CPP sub( const Matrix4x4& rhs ) const {
        return m4_sub( this, &rhs );
    }
    attr_header Matrix4x4CPP mul( f32 rhs ) const {
        return m4_mul( this, rhs );
    }
    attr_header Matrix4x4CPP mul( const Matrix4x4& rhs ) const {
        return m4_mul_m4( this, &rhs );
    }
    attr_header Vector4 mul( const Vector4& rhs ) const {
        return m4_mul_v4( this, rhs );
    }
    attr_header Vector3 mul( const Vector3& rhs ) const {
        return m4_mul_v3( this, rhs );
    }
    attr_header Matrix4x4CPP div( f32 rhs ) const {
        return m4_div( this, rhs );
    }
    attr_header Matrix4x4CPP transpose(void) const {
        return m4_transpose( this );
    }
    attr_header f32 determinant(void) const {
        return m4_determinant( this );
    }
    attr_header Matrix3x3 submatrix( u32 column, u32 row ) const {
        return m4_submatrix( this, column, row );
    }
    attr_header f32 minor( u32 column, u32 row ) const {
        return m4_minor( this, column, row );
    }
    attr_header f32 cofactor( u32 column, u32 row ) const {
        return m4_cofactor( this, column, row );
    }
    attr_header Matrix4x4CPP cofactor_matrix(void) const {
        return m4_cofactor_matrix( this );
    }
    attr_header Matrix4x4CPP adjoint(void) const {
        return m4_adjoint( this );
    }
    attr_header b32 inverse( Matrix4x4CPP* out_inverse ) const {
        return m4_inverse( this, out_inverse );
    }
    attr_header Matrix4x4CPP inverse_unchecked(void) const {
        return m4_inverse_unchecked( this );
    }
    attr_header b32 normal_matrix( Matrix3x3* out_normal ) const {
        return m4_normal_matrix( this, out_normal );
    }
    attr_header Matrix3x3 normal_matrix_unchecked(void) const {
        return m4_normal_matrix_unchecked( this );
    }

    attr_header const Vector4CPP& operator[]( usize idx ) const {
        const Vector4* ptr = c + idx;
        return *(const Vector4CPP*)ptr;
    }
    attr_header Vector4CPP& operator[]( usize idx ) {
        Vector4* ptr = c + idx;
        return *(Vector4CPP*)ptr;
    }
    attr_header Matrix4x4CPP& operator+=( const Matrix4x4CPP& rhs ) {
        return *this = add( rhs );
    }
    attr_header Matrix4x4CPP& operator-=( const Matrix4x4CPP& rhs ) {
        return *this = sub( rhs );
    }
    attr_header Matrix4x4CPP& operator*=( f32 rhs ) {
        return *this = mul( rhs );
    }
    attr_header Matrix4x4CPP& operator/=( f32 rhs ) {
        return *this = div( rhs );
    }
};
attr_header Matrix4x4 operator+( const Matrix4x4& lhs, const Matrix4x4& rhs ) {
    return m4_add( &lhs, &rhs );
}
attr_header Matrix4x4 operator-( const Matrix4x4& lhs, const Matrix4x4& rhs ) {
    return m4_sub( &lhs, &rhs );
}
attr_header Matrix4x4 operator*( const Matrix4x4& lhs, f32 rhs ) {
    return m4_mul( &lhs, rhs );
}
attr_header Matrix4x4 operator*( f32 lhs, const Matrix4x4& rhs ) {
    return m4_mul( &rhs, lhs );
}
attr_header Matrix4x4 operator*( const Matrix4x4& lhs, const Matrix4x4& rhs ) {
    return m4_mul_m4( &lhs, &rhs );
}
attr_header Vector4 operator*( const Matrix4x4& lhs, const Vector4& rhs ) {
    return m4_mul_v4( &lhs, rhs );
}
attr_header Vector3 operator*( const Matrix4x4& lhs, const Vector3& rhs ) {
    return m4_mul_v3( &lhs, rhs );
}
attr_header Matrix4x4 operator/( const Matrix4x4& lhs, f32 rhs ) {
    return m4_div( &lhs, rhs );
}

attr_header Matrix4x4 add( const Matrix4x4& lhs, const Matrix4x4& rhs ) {
    return m4_add( &lhs, &rhs );
}
attr_header Matrix4x4 sub( const Matrix4x4& lhs, const Matrix4x4& rhs ) {
    return m4_sub( &lhs, &rhs );
}
attr_header Matrix4x4 mul( const Matrix4x4& lhs, f32 rhs ) {
    return m4_mul( &lhs, rhs );
}
attr_header Matrix4x4 mul( f32 lhs, const Matrix4x4& rhs ) {
    return m4_mul( &rhs, lhs );
}
attr_header Matrix4x4 mul( const Matrix4x4& lhs, const Matrix4x4& rhs ) {
    return m4_mul_m4( &lhs, &rhs );
}
attr_header Vector4 mul( const Matrix4x4& lhs, const Vector4& rhs ) {
    return m4_mul_v4( &lhs, rhs );
}
attr_header Vector3 mul( const Matrix4x4& lhs, const Vector3& rhs ) {
    return m4_mul_v3( &lhs, rhs );
}
attr_header Matrix4x4 div( const Matrix4x4& lhs, f32 rhs ) {
    return m4_div( &lhs, rhs );
}
attr_header Matrix4x4 transponse( const Matrix4x4& m ) {
    return m4_transpose( &m );
}
attr_header f32 determinant( const Matrix4x4& m ) {
    return m4_determinant( &m );
}
attr_header Matrix4x4 adjoint( const Matrix4x4& m ) {
    return m4_adjoint( &m );
}

#endif /* header guard */
