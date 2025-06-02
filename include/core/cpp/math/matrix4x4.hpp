#if !defined(CORE_CPP_MATH_MATRIX4X4_HPP) && defined(__cplusplus)
#define CORE_CPP_MATH_MATRIX4X4_HPP
/**
 * @file   matrix4x4.hpp
 * @brief  C++ Math: Matrix4x4.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 31, 2025
*/
#if !defined(CORE_MATH_MATRIX4X4_H)
    #include "core/math/matrix4x4.h"
#endif
#include "core/math/quaternion.h"

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
        struct Matrix4x4 __pod;

        Vector4CPP col[4];
        f32        array[16];
    };

    constexpr Matrix4x4CPP();
    constexpr Matrix4x4CPP( const Matrix4x4& __pod );
    constexpr explicit Matrix4x4CPP(
        f32 m00, f32 m01, f32 m02, f32 m03,
        f32 m10, f32 m11, f32 m12, f32 m13,
        f32 m20, f32 m21, f32 m22, f32 m23,
        f32 m30, f32 m31, f32 m32, f32 m33 );
    constexpr explicit Matrix4x4CPP(
        Vector4CPP col0, Vector4CPP col1, Vector4CPP col2, Vector4CPP col3 );

    constexpr operator Matrix4x4() const;
    constexpr Vector4CPP operator[]( usize index ) const;
    constexpr Vector4CPP& operator[]( usize index );

    static constexpr Matrix4x4CPP zero();
    static constexpr Matrix4x4CPP identity();
};

typedef Matrix4x4CPP mat4x4;
typedef Matrix4x4CPP mat4;

constexpr attr_always_inline attr_header attr_hot
Matrix4x4CPP::Matrix4x4CPP()
    : m00(0.0f), m01(0.0f), m02(0.0f), m03(0.0f),
      m10(0.0f), m11(0.0f), m12(0.0f), m13(0.0f),
      m20(0.0f), m21(0.0f), m22(0.0f), m23(0.0f),
      m30(0.0f), m31(0.0f), m32(0.0f), m33(0.0f) {}
constexpr attr_always_inline attr_header attr_hot
Matrix4x4CPP::Matrix4x4CPP( const Matrix4x4& __pod )
    : __pod(__pod) {}
constexpr attr_always_inline attr_header attr_hot
Matrix4x4CPP::Matrix4x4CPP(
    f32 m00, f32 m01, f32 m02, f32 m03,
    f32 m10, f32 m11, f32 m12, f32 m13,
    f32 m20, f32 m21, f32 m22, f32 m23,
    f32 m30, f32 m31, f32 m32, f32 m33 )
    : m00(m00), m01(m01), m02(m02), m03(m03),
      m10(m10), m11(m11), m12(m12), m13(m13),
      m20(m20), m21(m21), m22(m22), m23(m23),
      m30(m30), m31(m31), m32(m32), m33(m33) {}
constexpr attr_always_inline attr_header attr_hot
Matrix4x4CPP::Matrix4x4CPP(
    Vector4CPP col0, Vector4CPP col1, Vector4CPP col2, Vector4CPP col3 )
    : col0(col0), col1(col1), col2(col2), col3(col3) {}

constexpr attr_always_inline attr_header attr_hot
Matrix4x4CPP::operator Matrix4x4() const {
    return *(Matrix4x4*)this;
}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP Matrix4x4CPP::operator[]( usize index ) const {
    return this->col[index];
}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP& Matrix4x4CPP::operator[]( usize index ) {
    return this->col[index];
}

constexpr attr_always_inline attr_header attr_hot
Matrix4x4CPP Matrix4x4CPP::zero() {
    return MAT4_ZERO;
}
constexpr attr_always_inline attr_header attr_hot
Matrix4x4CPP Matrix4x4CPP::identity() {
    return MAT4_IDENTITY;
}

attr_always_inline attr_header attr_hot
mat4 add( const mat4& lhs, const mat4& rhs ) {
    return mat4_add( &lhs.__pod, &rhs.__pod );
}
attr_always_inline attr_header attr_hot
mat4 sub( const mat4& lhs, const mat4& rhs ) {
    return mat4_sub( &lhs.__pod, &rhs.__pod );
}
attr_always_inline attr_header attr_hot
mat4 mul( const mat4& lhs, f32 rhs ) {
    return mat4_mul( &lhs.__pod, rhs );
}
attr_always_inline attr_header attr_hot
mat4 mul( f32 lhs, const mat4& rhs ) {
    return mat4_mul( &rhs.__pod, lhs );
}
attr_always_inline attr_header attr_hot
mat4 mul( const mat4& lhs, const mat4& rhs ) {
    return mat4_mul_mat4( &lhs.__pod, &rhs.__pod );
}
attr_always_inline attr_header attr_hot
vec4 mul( const mat4& lhs, vec4 rhs ) {
    return mat4_mul_vec4( &lhs.__pod, rhs );
}
attr_always_inline attr_header attr_hot
vec3 mul( const mat4& lhs, vec3 rhs ) {
    return mat4_mul_vec3( &lhs.__pod, rhs );
}
attr_always_inline attr_header attr_hot
mat4 div( const mat4& lhs, f32 rhs ) {
    return mat4_div( &lhs.__pod, rhs );
}

attr_always_inline attr_header attr_hot
mat4 operator+( const mat4& lhs, const mat4& rhs ) {
    return add( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat4& operator+=( mat4& lhs, const mat4& rhs ) {
    return lhs = lhs + rhs;
}
attr_always_inline attr_header attr_hot
mat4 operator-( mat4 lhs, const mat4& rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat4& operator-=( mat4& lhs, const mat4& rhs ) {
    return lhs = lhs - rhs;
}
attr_always_inline attr_header attr_hot
mat4 operator*( const mat4& lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat4 operator*( f32 lhs, const mat4& rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat4 operator*( const mat4& lhs, const mat4& rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
vec4 operator*( const mat4& lhs, vec4 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
vec3 operator*( const mat4& lhs, vec3 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat4& operator*=( mat4& lhs, f32 rhs ) {
    return lhs = lhs * rhs;
}
attr_always_inline attr_header attr_hot
mat4& operator*=( mat4& lhs, const mat4& rhs ) {
    return lhs = lhs * rhs;
}
attr_always_inline attr_header attr_hot
mat4 operator/( const mat4& lhs, f32 rhs ) {
    return div( lhs, rhs );
}
attr_always_inline attr_header attr_hot
mat4& operator/=( mat4& lhs, f32 rhs ) {
    return lhs = lhs / rhs;
}

attr_always_inline attr_header attr_hot
void array_from_mat4( const mat4& m, f32* out_array ) {
    array_from_mat4( &m.__pod, out_array );
}
attr_always_inline attr_header attr_hot
mat4 transpose( const mat4& m ) {
    return mat4_transpose( &m.__pod );
}
attr_always_inline attr_header attr_hot
f32 determinant( const mat4& m ) {
    return mat4_determinant( &m.__pod );
}
attr_always_inline attr_header attr_hot
mat3 submatrix( const mat4& m, u32 column, u32 row ) {
    return mat4_submatrix( &m.__pod, column, row );
}
attr_always_inline attr_header attr_hot
f32 minor( const mat4& m, u32 column, u32 row ) {
    return mat4_minor( &m.__pod, column, row );
}
attr_always_inline attr_header attr_hot
f32 cofactor( const mat4& m, u32 column, u32 row ) {
    return mat4_cofactor( &m.__pod, column, row );
}
attr_always_inline attr_header attr_hot
mat4 cofactor_matrix( const mat4& m ) {
    return mat4_cofactor_matrix( &m.__pod );
}
attr_always_inline attr_header attr_hot
mat4 adjoint( const mat4& m ) {
    return mat4_adjoint( &m.__pod );
}
attr_always_inline attr_header attr_hot
bool inverse_checked( const mat4& m, mat4* out_inverse ) {
    return mat4_inverse_checked( &m.__pod, (Matrix4x4*)out_inverse );
}
attr_always_inline attr_header attr_hot
mat4 inverse( const mat4& m ) {
    return mat4_inverse( &m.__pod );
}
attr_always_inline attr_header attr_hot
bool normal_matrix_checked( const mat4& m, mat3* out_normal ) {
    return mat4_normal_matrix_checked( &m.__pod, (Matrix3x3*)out_normal );
}
attr_always_inline attr_header attr_hot
mat3 normal_matrix( const mat4& m ) {
    return mat4_normal_matrix( &m.__pod );
}

attr_always_inline attr_header attr_hot
mat4 view( vec3 position, vec3 target, vec3 up = vec3::up() ) {
    return mat4_view( position, target, up );
}
attr_always_inline attr_header attr_hot
mat4 view_2d( vec2 position, vec2 up ) {
    return mat4_view_2d( position, up );
}
attr_always_inline attr_header attr_hot
mat4 ortho(
    f32 left,
    f32 right,
    f32 bottom,
    f32 top,
    f32 clip_near = 0.001f,
    f32 clip_far  = 1000.0f
) {
    return mat4_ortho( left, right, bottom, top, clip_near, clip_far );
}
attr_always_inline attr_header attr_hot
mat4 perspective(
    f32 field_of_view,
    f32 aspect_ratio,
    f32 clip_near = 0.001f,
    f32 clip_far  = 1000.0f
) {
    return mat4_perspective( field_of_view, aspect_ratio, clip_near, clip_far );
}
attr_always_inline attr_header attr_hot
mat4 translation( f32 x, f32 y, f32 z ) {
    return mat4_translation( x, y, z );
}
attr_always_inline attr_header attr_hot
mat4 translation( vec3 translation ) {
    return mat4_translation_vec3( translation );
}
attr_always_inline attr_header attr_hot
mat4 translation_2d( f32 x, f32 y ) {
    return mat4_translation_2d( x, y );
}
attr_always_inline attr_header attr_hot
mat4 translation_2d( vec2 translation ) {
    return mat4_translation_2d_vec2( translation );
}
attr_always_inline attr_header attr_hot
mat4 rotation_pitch( f32 pitch ) {
    return mat4_rotation_pitch( pitch );
}
attr_always_inline attr_header attr_hot
mat4 rotation_yaw( f32 yaw ) {
    return mat4_rotation_yaw( yaw );
}
attr_always_inline attr_header attr_hot
mat4 rotation_roll( f32 roll ) {
    return mat4_rotation_roll( roll );
}
attr_always_inline attr_header attr_hot
mat4 rotation_euler( f32 pitch, f32 yaw, f32 roll ) {
    return mat4_rotation_euler( pitch, yaw, roll );
}
attr_always_inline attr_header attr_hot
mat4 rotation_euler( vec3 euler ) {
    return mat4_rotation_euler_vec3( euler );
}
attr_always_inline attr_header attr_hot
mat4 rotation( quat rotation ) {
    return mat4_rotation( rotation );
}
attr_always_inline attr_header attr_hot
mat4 rotation_2d( f32 rotation ) {
    return mat4_rotation_2d( rotation );
}
attr_always_inline attr_header attr_hot
mat4 scale( f32 width, f32 height, f32 length ) {
    return mat4_scale( width, height, length );
}
attr_always_inline attr_header attr_hot
mat4 scale( vec3 scale ) {
    return mat4_scale_vec3( scale );
}
attr_always_inline attr_header attr_hot
mat4 scale_2d( f32 width, f32 height ) {
    return mat4_scale_2d( width, height );
}
attr_always_inline attr_header attr_hot
mat4 scale_2d( vec2 scale ) {
    return mat4_scale_2d_vec2( scale );
}
attr_always_inline attr_header attr_hot
mat4 transform( vec3 translation, quat rotation, vec3 scale ) {
    return mat4_transform( translation, rotation, scale );
}
attr_always_inline attr_header attr_hot
mat4 transform( vec3 translation, vec3 euler, vec3 scale ) {
    return mat4_transform_euler( translation, euler, scale );
}
attr_always_inline attr_header attr_hot
mat4 transform_2d( vec2 translation, f32 rotation, vec2 scale ) {
    return mat4_transform_2d( translation, rotation, scale );
}

#endif /* header guard */
