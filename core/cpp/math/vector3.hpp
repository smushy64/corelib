#if !defined(CORE_CPP_MATH_VECTOR3_HPP)
#define CORE_CPP_MATH_VECTOR3_HPP
/**
 * @file   vector3.hpp
 * @brief  C++ Vector3.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/
#include "core/cpp/math/swizzler.hpp"

struct Vector3CPP;
struct IVector3CPP;

#if !defined(CORE_MATH_VECTOR3_H)
    #include "core/math/vector3.h"
#endif

struct Vector3CPP {
    union {
        /// @brief X, Y and Z components.
        struct {
            /// @brief X and Y components.
            union {
                /// @brief X and Y components.
                struct {
                    /// @brief X component.
                    f32 x;
                    /// @brief Y component.
                    f32 y;
                };
                /// @brief Red and Green channels.
                struct {
                    /// @brief Red channel (X component).
                    f32 r;
                    /// @brief Green channel (Y component).
                    f32 g;
                };
                /// @brief Hue and Saturation.
                struct {
                    /// @brief Hue (X component).
                    f32 h;
                    /// @brief Saturation (X component).
                    f32 s;
                };
                /// @brief X and Y components as a #Vector2.
                Vector2CPP xy;
                /// @brief Red and Green channels as a #Vector2.
                Vector2CPP rg;
            };
            /// @brief Z component.
            union {
                /// @brief Z component.
                f32 z;
                /// @brief Blue channel (Z component).
                f32 b;
                /// @brief Lightness (Z component).
                f32 l;
            };
        };
        /// @brief Swizzle yz.
        struct {
            f32 __unused0;
            /// @brief Y and Z components as a #Vector2.
            Vector2CPP yz;
        };
        /// @brief Swizzle gb.
        struct {
            f32 __unused1;
            /// @brief Green and Blue channels as a #Vector2.
            Vector2CPP gb;
        };
        struct Vector3 pod;

        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 0, 0> xx, rr;
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 0, 2> xz, rb;
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 1, 0> yx, gr;
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 1, 1> yy, gg;
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 2, 0> zx, br;
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 2, 1> zy, bg;
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 2, 2> zz, bb;

        Swizzler<Vector3CPP, f32, 0, 0, 0> xxx, rrr;
        Swizzler<Vector3CPP, f32, 0, 0, 1> xxy, rrg;
        Swizzler<Vector3CPP, f32, 0, 0, 2> xxz, rrb;
        Swizzler<Vector3CPP, f32, 0, 1, 0> xyx, rgr;
        Swizzler<Vector3CPP, f32, 0, 1, 1> xyy, rgg;
        Swizzler<Vector3CPP, f32, 0, 1, 2> xyz, rgb;
        Swizzler<Vector3CPP, f32, 0, 2, 0> xzx, rbr;
        Swizzler<Vector3CPP, f32, 0, 2, 1> xzy, rbg;
        Swizzler<Vector3CPP, f32, 0, 2, 2> xzz, rbb;
        Swizzler<Vector3CPP, f32, 1, 0, 0> yxx, grr;
        Swizzler<Vector3CPP, f32, 1, 0, 1> yxy, grg;
        Swizzler<Vector3CPP, f32, 1, 0, 2> yxz, grb;
        Swizzler<Vector3CPP, f32, 1, 1, 0> yyx, ggr;
        Swizzler<Vector3CPP, f32, 1, 1, 1> yyy, ggg;
        Swizzler<Vector3CPP, f32, 1, 1, 2> yyz, ggb;
        Swizzler<Vector3CPP, f32, 1, 2, 0> yzx, gbr;
        Swizzler<Vector3CPP, f32, 1, 2, 1> yzy, gbg;
        Swizzler<Vector3CPP, f32, 1, 2, 2> yzz, gbb;
        Swizzler<Vector3CPP, f32, 2, 0, 0> zxx, brr;
        Swizzler<Vector3CPP, f32, 2, 0, 1> zxy, brg;
        Swizzler<Vector3CPP, f32, 2, 0, 2> zxz, brb;
        Swizzler<Vector3CPP, f32, 2, 1, 0> zyx, bgr;
        Swizzler<Vector3CPP, f32, 2, 1, 1> zyy, bgg;
        Swizzler<Vector3CPP, f32, 2, 1, 2> zyz, bgb;
        Swizzler<Vector3CPP, f32, 2, 2, 0> zzx, bbr;
        Swizzler<Vector3CPP, f32, 2, 2, 1> zzy, bbg;
        Swizzler<Vector3CPP, f32, 2, 2, 2> zzz, bbb;

        f32 array[3];
    };

    attr_always_inline attr_header constexpr
    Vector3CPP() : x(0), y(0), z(0) {}
    attr_always_inline attr_header constexpr
    Vector3CPP( const struct Vector3& v ) : x(v.x), y(v.y), z(v.z) {}
    attr_always_inline attr_header constexpr
    explicit Vector3CPP( const struct IVector3& v ) : x(v.x), y(v.y), z(v.z) {}
    attr_always_inline attr_header constexpr
    explicit Vector3CPP( f32 s ) : x(s), y(s), z(s) {}
    attr_always_inline attr_header constexpr
    explicit Vector3CPP( f32 x, f32 y, f32 z ) : x(x), y(y), z(z) {}
    attr_always_inline attr_header constexpr
    explicit Vector3CPP( Vector2CPP xy, f32 z ) : Vector3CPP( xy.x, xy.y, z ) {}
    attr_always_inline attr_header constexpr
    explicit Vector3CPP( f32 x, Vector2CPP yz ) : Vector3CPP( x, yz.x, yz.y ) {}

    attr_always_inline attr_header constexpr
    operator Vector3() const {
        return *(struct Vector3*)this;
    }

    attr_always_inline attr_header static constexpr
    Vector3CPP zero() {
        return Vector3CPP();
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP one() {
        return Vector3CPP( 1.0, 1.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP left() {
        return Vector3CPP( -1.0, 0.0, 0.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP right() {
        return Vector3CPP( 1.0, 0.0, 0.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP up() {
        return Vector3CPP( 0.0, 1.0, 0.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP down() {
        return Vector3CPP( 0.0, -1.0, 0.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP forward() {
        return Vector3CPP( 0.0, 0.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP back() {
        return Vector3CPP( 0.0, 0.0, -1.0 );
    }

    attr_always_inline attr_header static constexpr
    Vector3CPP red() {
        return Vector3CPP( 1.0, 0.0, 0.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP green() {
        return Vector3CPP( 0.0, 1.0, 0.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP blue() {
        return Vector3CPP( 0.0, 0.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP yellow() {
        return Vector3CPP( 1.0, 1.0, 0.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP magenta() {
        return Vector3CPP( 1.0, 0.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP cyan() {
        return Vector3CPP( 0.0, 1.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP black() {
        return Vector3CPP( 0.0, 0.0, 0.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector3CPP white() {
        return Vector3CPP( 1.0, 1.0, 1.0 );
    }

    attr_always_inline attr_header constexpr
    Vector3CPP to_hsl() const {
        return rgb_to_hsl( pod );
    }
    attr_always_inline attr_header constexpr
    Vector3CPP to_rgb() const {
        return hsl_to_rgb( pod );
    }

    attr_always_inline attr_header static constexpr
    Vector3CPP from_array( const f32 array[3] ) {
        return *(Vector3CPP*)array;
    }
    attr_always_inline attr_header constexpr
    void to_array( f32 out_array[3] ) const {
        out_array[0] = array[0];
        out_array[1] = array[1];
        out_array[2] = array[2];
    }

    attr_always_inline attr_header constexpr
    f32 operator[]( usize idx ) const {
        return array[idx];
    }
    attr_always_inline attr_header constexpr
    f32& operator[]( usize idx ) {
        return array[idx];
    }
};
struct IVector3CPP {
    union {
        /// @brief X, Y and Z components.
        struct {
            /// @brief X and Y components.
            union {
                /// @brief X and Y components.
                struct {
                    /// @brief X component.
                    i32 x;
                    /// @brief Y component.
                    i32 y;
                };
                /// @brief X and Y components as an #IVector2.
                IVector2CPP xy;
            };
            /// @brief Z component.
            i32 z;
        };
        /// @brief Swizzle yz.
        struct {
            i32 __unused0;
            IVector2CPP yz;
        };
        // struct { i32 x, y, z; };
        struct IVector3 pod;

        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 0, 0> xx;
        // SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 0, 1> xy;
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 0, 2> xz;
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 1, 0> yx;
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 1, 1> yy;
        // SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 1, 2> yz;
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 2, 0> zx;
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 2, 1> zy;
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 2, 2> zz;

        Swizzler<IVector3CPP, i32, 0, 0, 0> xxx;
        Swizzler<IVector3CPP, i32, 0, 0, 1> xxy;
        Swizzler<IVector3CPP, i32, 0, 0, 2> xxz;
        Swizzler<IVector3CPP, i32, 0, 1, 0> xyx;
        Swizzler<IVector3CPP, i32, 0, 1, 1> xyy;
        Swizzler<IVector3CPP, i32, 0, 1, 2> xyz;
        Swizzler<IVector3CPP, i32, 0, 2, 0> xzx;
        Swizzler<IVector3CPP, i32, 0, 2, 1> xzy;
        Swizzler<IVector3CPP, i32, 0, 2, 2> xzz;
        Swizzler<IVector3CPP, i32, 1, 0, 0> yxx;
        Swizzler<IVector3CPP, i32, 1, 0, 1> yxy;
        Swizzler<IVector3CPP, i32, 1, 0, 2> yxz;
        Swizzler<IVector3CPP, i32, 1, 1, 0> yyx;
        Swizzler<IVector3CPP, i32, 1, 1, 1> yyy;
        Swizzler<IVector3CPP, i32, 1, 1, 2> yyz;
        Swizzler<IVector3CPP, i32, 1, 2, 0> yzx;
        Swizzler<IVector3CPP, i32, 1, 2, 1> yzy;
        Swizzler<IVector3CPP, i32, 1, 2, 2> yzz;
        Swizzler<IVector3CPP, i32, 2, 0, 0> zxx;
        Swizzler<IVector3CPP, i32, 2, 0, 1> zxy;
        Swizzler<IVector3CPP, i32, 2, 0, 2> zxz;
        Swizzler<IVector3CPP, i32, 2, 1, 0> zyx;
        Swizzler<IVector3CPP, i32, 2, 1, 1> zyy;
        Swizzler<IVector3CPP, i32, 2, 1, 2> zyz;
        Swizzler<IVector3CPP, i32, 2, 2, 0> zzx;
        Swizzler<IVector3CPP, i32, 2, 2, 1> zzy;
        Swizzler<IVector3CPP, i32, 2, 2, 2> zzz;

        i32 array[3];
    };

    attr_always_inline attr_header constexpr
    IVector3CPP() : x(0), y(0), z(0) {}
    attr_always_inline attr_header constexpr
    IVector3CPP( const struct IVector3& v ) : x(v.x), y(v.y), z(v.z) {}
    attr_always_inline attr_header constexpr
    explicit IVector3CPP( const Vector3CPP& v ) : x(v.x), y(v.y), z(v.z) {}
    attr_always_inline attr_header constexpr
    explicit IVector3CPP( i32 s ) : x(s), y(s), z(s) {}
    attr_always_inline attr_header constexpr
    explicit IVector3CPP( i32 x, i32 y, i32 z ) : x(x), y(y), z(z) {}
    attr_always_inline attr_header constexpr
    explicit IVector3CPP( IVector2CPP xy, i32 z ) : IVector3CPP( xy.x, xy.y, z ) {}
    attr_always_inline attr_header constexpr
    explicit IVector3CPP( i32 x, IVector2CPP yz ) : IVector3CPP( x, yz.x, yz.y ) {}

    attr_always_inline attr_header constexpr
    operator IVector3() const {
        return *(struct IVector3*)this;
    }

    attr_always_inline attr_header static constexpr
    IVector3CPP zero() {
        return IVector3CPP();
    }
    attr_always_inline attr_header static constexpr
    IVector3CPP one() {
        return IVector3CPP( 1, 1, 1 );
    }
    attr_always_inline attr_header static constexpr
    IVector3CPP left() {
        return IVector3CPP( -1, 0, 0 );
    }
    attr_always_inline attr_header static constexpr
    IVector3CPP right() {
        return IVector3CPP( 1, 0, 0 );
    }
    attr_always_inline attr_header static constexpr
    IVector3CPP up() {
        return IVector3CPP( 0, 1, 0 );
    }
    attr_always_inline attr_header static constexpr
    IVector3CPP down() {
        return IVector3CPP( 0, -1, 0 );
    }
    attr_always_inline attr_header static constexpr
    IVector3CPP forward() {
        return IVector3CPP( 0, 0, 1 );
    }
    attr_always_inline attr_header static constexpr
    IVector3CPP back() {
        return IVector3CPP( 0, 0, -1 );
    }

    attr_always_inline attr_header static constexpr
    IVector3CPP from_array( const i32 array[3] ) {
        return *(IVector3CPP*)array;
    }
    attr_always_inline attr_header constexpr
    void to_array( i32 out_array[3] ) const {
        out_array[0] = array[0];
        out_array[1] = array[1];
        out_array[2] = array[2];
    }

    attr_always_inline attr_header constexpr
    i32 operator[]( usize idx ) const {
        return array[idx];
    }
    attr_always_inline attr_header constexpr
    i32& operator[]( usize idx ) {
        return array[idx];
    }
};
attr_always_inline attr_header
Vector3CPP add( Vector3CPP lhs, Vector3CPP rhs ) {
    return vec3_add( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Vector3CPP sub( Vector3CPP lhs, Vector3CPP rhs ) {
    return vec3_sub( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Vector3CPP mul( Vector3CPP lhs, f32 rhs ) {
    return vec3_mul( lhs.pod, rhs );
}
attr_always_inline attr_header
Vector3CPP mul( f32 lhs, Vector3CPP rhs ) {
    return vec3_mul( rhs.pod, lhs );
}
attr_always_inline attr_header
Vector3CPP mul( Vector3CPP lhs, Vector3CPP rhs ) {
    return vec3_mul_vec3( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Vector3CPP hadamard( Vector3CPP lhs, Vector3CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP div( Vector3CPP lhs, f32 rhs ) {
    return vec3_div( lhs.pod, rhs );
}
attr_always_inline attr_header
Vector3CPP div( Vector3CPP lhs, Vector3CPP rhs ) {
    return vec3_div_vec3( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Vector3CPP cross( Vector3CPP lhs, Vector3CPP rhs ) {
    return vec3_cross( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
f32 dot( Vector3CPP lhs, Vector3CPP rhs ) {
    return vec3_dot( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
Vector3CPP neg( Vector3CPP x ) {
    return vec3_neg( x.pod );
}
attr_always_inline attr_header
f32 hmax( Vector3CPP x ) {
    return vec3_hmax( x.pod );
}
attr_always_inline attr_header
f32 hmid( Vector3CPP x ) {
    return vec3_hmid( x.pod );
}
attr_always_inline attr_header
f32 hmin( Vector3CPP x ) {
    return vec3_hmin( x.pod );
}
attr_always_inline attr_header
Vector3CPP max( Vector3CPP x, Vector3CPP y ) {
    return vec3_max( x.pod, y.pod );
}
attr_always_inline attr_header
Vector3CPP min( Vector3CPP x, Vector3CPP y ) {
    return vec3_min( x.pod, y.pod );
}
attr_always_inline attr_header
f32 length_sqr( Vector3CPP x ) {
    return vec3_length_sqr( x.pod );
}
attr_always_inline attr_header
f32 length( Vector3CPP x ) {
    return vec3_length( x.pod );
}
attr_always_inline attr_header
f32 distance_sqr( Vector3CPP a, Vector3CPP b ) {
    return vec3_distance_sqr( a.pod, b.pod );
}
attr_always_inline attr_header
f32 distance( Vector3CPP a, Vector3CPP b ) {
    return vec3_distance( a.pod, b.pod );
}
attr_always_inline attr_header
Vector3CPP normalize( Vector3CPP x ) {
    return vec3_normalize( x.pod );
}
attr_always_inline attr_header
Vector3CPP reflect( Vector3CPP direction, Vector3CPP normal ) {
    return vec3_reflect( direction.pod, normal.pod );
}
attr_always_inline attr_header
Vector3CPP clamp( Vector3CPP v, Vector3CPP min, Vector3CPP max ) {
    return vec3_clamp( v.pod, min.pod, max.pod );
}
attr_always_inline attr_header
Vector3CPP clamp( Vector3CPP v, f32 min, f32 max ) {
    return vec3_clamp_length( v.pod, min, max );
}
attr_always_inline attr_header
f32 angle( Vector3CPP a, Vector3CPP b ) {
    return vec3_angle( a.pod, b.pod );
}
attr_always_inline attr_header
Vector3CPP abs( Vector3CPP x ) {
    return vec3_abs( x.pod );
}
attr_always_inline attr_header
Vector3CPP sign( Vector3CPP x ) {
    return vec3_sign( x.pod );
}
attr_always_inline attr_header
Vector3CPP trunc( Vector3CPP x ) {
    return vec3_trunc( x.pod );
}
attr_always_inline attr_header
Vector3CPP floor( Vector3CPP x ) {
    return vec3_floor( x.pod );
}
attr_always_inline attr_header
Vector3CPP ceil( Vector3CPP x ) {
    return vec3_ceil( x.pod );
}
attr_always_inline attr_header
Vector3CPP round( Vector3CPP x ) {
    return vec3_round( x.pod );
}
attr_always_inline attr_header
Vector3CPP fract( Vector3CPP x ) {
    return vec3_fract( x.pod );
}
attr_always_inline attr_header
Vector3CPP slerp( Vector3CPP a, Vector3CPP b, f32 t ) {
    return vec3_slerp( a.pod, b.pod, t );
}
attr_always_inline attr_header
Vector3CPP lerp( Vector3CPP a, Vector3CPP b, f32 t ) {
    return vec3_lerp( a.pod, b.pod, t );
}
attr_always_inline attr_header
Vector3CPP mix( Vector3CPP a, Vector3CPP b, f32 t ) {
    return lerp( a, b, t );
}
attr_always_inline attr_header
Vector3CPP step( Vector3CPP edge, Vector3CPP x ) {
    return vec3_step( edge.pod, x.pod );
}
attr_always_inline attr_header
Vector3CPP step( f32 edge, Vector3CPP x ) {
    return vec3_step_scalar( edge, x.pod );
}
attr_always_inline attr_header
Vector3CPP smoothstep( Vector3CPP edge0, Vector3CPP edge1, Vector3CPP x ) {
    return vec3_smoothstep( edge0.pod, edge1.pod, x.pod );
}
attr_always_inline attr_header
Vector3CPP smoothstep( f32 edge0, f32 edge1, Vector3CPP x ) {
    return vec3_smoothstep_scalar( edge0, edge1, x.pod );
}
attr_always_inline attr_header
Vector3CPP smootherstep( Vector3CPP edge0, Vector3CPP edge1, Vector3CPP x ) {
    return vec3_smootherstep( edge0.pod, edge1.pod, x.pod );
}
attr_always_inline attr_header
Vector3CPP smootherstep( f32 edge0, f32 edge1, Vector3CPP x ) {
    return vec3_smootherstep_scalar( edge0, edge1, x.pod );
}
attr_always_inline attr_header
Vector3CPP radians( Vector3CPP degrees ) {
    return vec3_radians( degrees.pod );
}
attr_always_inline attr_header
Vector3CPP degrees( Vector3CPP radians ) {
    return vec3_degrees( radians.pod );
}
attr_always_inline attr_header
Vector3CPP sin( Vector3CPP angle ) {
    return vec3_sin( angle.pod );
}
attr_always_inline attr_header
Vector3CPP cos( Vector3CPP angle ) {
    return vec3_cos( angle.pod );
}
attr_always_inline attr_header
Vector3CPP tan( Vector3CPP angle ) {
    return vec3_tan( angle.pod );
}
attr_always_inline attr_header
Vector3CPP asin( Vector3CPP angle ) {
    return vec3_asin( angle.pod );
}
attr_always_inline attr_header
Vector3CPP acos( Vector3CPP angle ) {
    return vec3_acos( angle.pod );
}
attr_always_inline attr_header
Vector3CPP atan2( Vector3CPP y, Vector3CPP x ) {
    return vec3_atan2( y.pod, x.pod );
}
attr_always_inline attr_header
b32 cmp( Vector3CPP a, Vector3CPP b ) {
    return vec3_cmp( a.pod, b.pod );
}

attr_always_inline attr_header
Vector3CPP operator-( Vector3CPP v ) {
    return neg( v );
}
attr_always_inline attr_header
Vector3CPP& operator+=( Vector3CPP& lhs, Vector3CPP rhs ) {
    return lhs = add( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP& operator-=( Vector3CPP& lhs, Vector3CPP rhs ) {
    return lhs = sub( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP& operator*=( Vector3CPP& lhs, Vector3CPP rhs ) {
    return lhs = mul( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP& operator/=( Vector3CPP& lhs, Vector3CPP rhs ) {
    return lhs = div( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP& operator*=( Vector3CPP& lhs, f32 rhs ) {
    return lhs = mul( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP& operator/=( Vector3CPP& lhs, f32 rhs ) {
    return lhs = div( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP operator+( Vector3CPP lhs, Vector3CPP rhs ) {
    return add( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP operator-( Vector3CPP lhs, Vector3CPP rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP operator*( Vector3CPP lhs, Vector3CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP operator/( Vector3CPP lhs, Vector3CPP rhs ) {
    return div( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP operator*( Vector3CPP lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP operator*( f32 lhs, Vector3CPP  rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
Vector3CPP operator/( Vector3CPP lhs, f32 rhs ) {
    return div( lhs, rhs );
}
attr_always_inline attr_header
b32 operator==( Vector3CPP a, Vector3CPP b ) {
    return cmp( a, b );
}
attr_always_inline attr_header
b32 operator!=( Vector3CPP a, Vector3CPP b ) {
    return !(a == b);
}

attr_always_inline attr_header
IVector3CPP add( IVector3CPP lhs, IVector3CPP rhs ) {
    return ivec3_add( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector3CPP sub( IVector3CPP lhs, IVector3CPP rhs ) {
    return ivec3_sub( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector3CPP mul( IVector3CPP lhs, IVector3CPP rhs ) {
    return ivec3_mul_ivec3( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector3CPP mul( IVector3CPP lhs, i32 rhs ) {
    return ivec3_mul( lhs.pod, rhs );
}
attr_always_inline attr_header
IVector3CPP mul( i32 lhs, IVector3CPP rhs ) {
    return ivec3_mul( rhs.pod, lhs );
}
attr_always_inline attr_header
IVector3CPP hadamard( IVector3CPP lhs, IVector3CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
IVector3CPP div( IVector3CPP lhs, i32 rhs ) {
    return ivec3_div( lhs.pod, rhs );
}
attr_always_inline attr_header
IVector3CPP div( IVector3CPP lhs, IVector3CPP rhs ) {
    return ivec3_div_ivec3( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector3CPP neg( IVector3CPP v ) {
    return ivec3_neg( v.pod );
}
attr_always_inline attr_header
i32 hadd( IVector3CPP v ) {
    return ivec3_hadd( v );
}
attr_always_inline attr_header
i32 hmul( IVector3CPP v ) {
    return ivec3_hmul( v );
}
attr_always_inline attr_header
f32 dot( IVector3CPP lhs, IVector3CPP rhs ) {
    return ivec3_dot( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
f32 length_sqr( IVector3CPP v ) {
    return ivec3_length_sqr( v );
}
attr_always_inline attr_header
f32 length( IVector3CPP v ) {
    return ivec3_length( v );
}
attr_always_inline attr_header
f32 cmp( IVector3CPP a, IVector3CPP b ) {
    return ivec3_cmp( a.pod, b.pod );
}

#endif /* header guard */
