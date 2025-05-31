#if !defined(CORE_CPP_MATH_VECTOR3_HPP) && defined(__cplusplus)
#define CORE_CPP_MATH_VECTOR3_HPP
/**
 * @file   vector3.hpp
 * @brief  C++ Math: Vector3.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 29, 2025
*/
#if !defined(CORE_MATH_VECTOR3_H)
    #include "core/math/vector3.h"
#endif
#include "core/cpp/math/swizzler.hpp"

/// @brief 3 Component 32-bit Unsigned Integer Vector.
typedef UVector3 uvec3;
/// @brief 3 Component 64-bit Floating Point Vector.
typedef DVector3 dvec3;

/// @brief 3 Component 32-bit Floating Point Vector.
struct Vector3CPP {
    /// @brief Union of vector component aliases.
    union {
        /// @brief X, Y, and Z components.
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
                /// @brief X and Y components -- as Red and Green channels.
                struct {
                    /// @brief X component -- as Red channel.
                    f32 r;
                    /// @brief Y component -- as Green channel.
                    f32 g;
                };
                /// @brief Swizzle X and Y components.
                Vector2CPP xy;
                /// @brief Swizzle X and Y components -- as Red and Green channels.
                Vector2CPP rg;
            };
            /// @brief Z component.
            union {
                /// @brief Z component.
                f32 z;
                /// @brief Z component -- as Blue channel.
                f32 b;
            };
        };
        /// @brief Swizzle Y and Z components.
        struct {
            f32 __unused0;
            /// @brief Swizzle Y and Z components.
            union {
                /// @brief Swizzle Y and Z components.
                Vector2CPP yz;
                /// @brief Swizzle Y and Z components -- as Green and Blue channels.
                Vector2CPP gb;
            };
        };
        /// @brief Swizzle XYZ.
        Vector3 xyz;
        /// @brief Swizzle XYZ.
        Vector3 rgb;

        /// @brief Swizzle XX.
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 0, 0> xx, rr;
        /// @brief Swizzle XZ.
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 0, 2> xz, rb;
        /// @brief Swizzle YZ.
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 1, 0> yx, gr;
        /// @brief Swizzle YY.
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 1, 1> yy, gg;
        /// @brief Swizzle ZX.
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 2, 0> zx, br;
        /// @brief Swizzle ZY.
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 2, 1> zy, bg;
        /// @brief Swizzle ZZ.
        SwizzlerConvert<Vector3CPP, Vector2CPP, f32, 2, 2> zz, bb;

        /// @brief Swizzle XXX.
        Swizzler<Vector3CPP, f32, 0, 0, 0> xxx, rrr;
        /// @brief Swizzle XXY.
        Swizzler<Vector3CPP, f32, 0, 0, 1> xxy, rrg;
        /// @brief Swizzle XXZ.
        Swizzler<Vector3CPP, f32, 0, 0, 2> xxz, rrb;
        /// @brief Swizzle XYX.
        Swizzler<Vector3CPP, f32, 0, 1, 0> xyx, rgr;
        /// @brief Swizzle XYY.
        Swizzler<Vector3CPP, f32, 0, 1, 1> xyy, rgg;
        /// @brief Swizzle XZX.
        Swizzler<Vector3CPP, f32, 0, 2, 0> xzx, rbr;
        /// @brief Swizzle XZY.
        Swizzler<Vector3CPP, f32, 0, 2, 1> xzy, rbg;
        /// @brief Swizzle XZZ.
        Swizzler<Vector3CPP, f32, 0, 2, 2> xzz, rbb;
        /// @brief Swizzle YXX.
        Swizzler<Vector3CPP, f32, 1, 0, 0> yxx, grr;
        /// @brief Swizzle YXY.
        Swizzler<Vector3CPP, f32, 1, 0, 1> yxy, grg;
        /// @brief Swizzle YXZ.
        Swizzler<Vector3CPP, f32, 1, 0, 2> yxz, grb;
        /// @brief Swizzle YYX.
        Swizzler<Vector3CPP, f32, 1, 1, 0> yyx, ggr;
        /// @brief Swizzle YYY.
        Swizzler<Vector3CPP, f32, 1, 1, 1> yyy, ggg;
        /// @brief Swizzle YYZ.
        Swizzler<Vector3CPP, f32, 1, 1, 2> yyz, ggb;
        /// @brief Swizzle YZX.
        Swizzler<Vector3CPP, f32, 1, 2, 0> yzx, gbr;
        /// @brief Swizzle YZY.
        Swizzler<Vector3CPP, f32, 1, 2, 1> yzy, gbg;
        /// @brief Swizzle YZZ.
        Swizzler<Vector3CPP, f32, 1, 2, 2> yzz, gbb;
        /// @brief Swizzle ZXX.
        Swizzler<Vector3CPP, f32, 2, 0, 0> zxx, brr;
        /// @brief Swizzle ZXY.
        Swizzler<Vector3CPP, f32, 2, 0, 1> zxy, brg;
        /// @brief Swizzle ZXZ.
        Swizzler<Vector3CPP, f32, 2, 0, 2> zxz, brb;
        /// @brief Swizzle ZYX.
        Swizzler<Vector3CPP, f32, 2, 1, 0> zyx, bgr;
        /// @brief Swizzle ZYY.
        Swizzler<Vector3CPP, f32, 2, 1, 1> zyy, bgg;
        /// @brief Swizzle ZYZ.
        Swizzler<Vector3CPP, f32, 2, 1, 2> zyz, bgb;
        /// @brief Swizzle ZZX.
        Swizzler<Vector3CPP, f32, 2, 2, 0> zzx, bbr;
        /// @brief Swizzle ZZY.
        Swizzler<Vector3CPP, f32, 2, 2, 1> zzy, bbg;
        /// @brief Swizzle ZZZ.
        Swizzler<Vector3CPP, f32, 2, 2, 2> zzz, bbb;

        /// @brief X, Y and Z components as an array.
        f32 array[3];
    };

    /// @brief Create empty vector.
    constexpr Vector3CPP();
    /// @brief Implicitly convert POD vector to C++ vector.
    constexpr Vector3CPP( const Vector3& __pod );
    /// @brief Create vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    constexpr explicit Vector3CPP( f32 x, f32 y, f32 z );
    /// @brief Create vector.
    /// @param x X, Y and Z components.
    constexpr explicit Vector3CPP( f32 x );
    /// @brief Create vector.
    /// @param x  X component.
    /// @param yz Y and Z components.
    constexpr explicit Vector3CPP( f32 x, Vector2CPP yz );
    /// @brief Create vector.
    /// @param xy X and Y components.
    /// @param z  Z component.
    constexpr explicit Vector3CPP( Vector2CPP xy, f32 z );
    /// @brief Convert integer vector to float vector.
    constexpr explicit Vector3CPP( const IVector3& iv );
    /// @brief Convert boolean vector to float vector.
    constexpr explicit Vector3CPP( const BVector3& bv );

    /// @brief Implicitly convert C++ vector to POD vector.
    constexpr operator Vector3() const;
    /// @brief Index into vector.
    /// @param index Index of component.
    /// @return Component.
    constexpr f32 operator[]( usize index ) const;
    /// @brief Index into vector.
    /// @param index Index of component.
    /// @return Component.
    constexpr f32& operator[]( usize index );

    /// @brief Vector zero constant.
    /// @return Value.
    static constexpr Vector3CPP zero();
    /// @brief Vector one constant.
    /// @return Value.
    static constexpr Vector3CPP one();

    /// @brief Vector up constant.
    /// @return Value.
    static constexpr Vector3CPP up();
    /// @brief Vector right constant.
    /// @return Value.
    static constexpr Vector3CPP right();
    /// @brief Vector down constant.
    /// @return Value.
    static constexpr Vector3CPP down();
    /// @brief Vector left constant.
    /// @return Value.
    static constexpr Vector3CPP left();
    /// @brief Vector forward constant.
    /// @return Value.
    static constexpr Vector3CPP forward();
    /// @brief Vector back constant.
    /// @return Value.
    static constexpr Vector3CPP back();

    /// @brief Vector color black constant.
    /// @return Value.
    static constexpr Vector3CPP black();
    /// @brief Vector color white constant.
    /// @return Value.
    static constexpr Vector3CPP white();
    /// @brief Vector color red constant.
    /// @return Value.
    static constexpr Vector3CPP red();
    /// @brief Vector color green constant.
    /// @return Value.
    static constexpr Vector3CPP green();
    /// @brief Vector color blue constant.
    /// @return Value.
    static constexpr Vector3CPP blue();
    /// @brief Vector color yellow constant.
    /// @return Value.
    static constexpr Vector3CPP yellow();
    /// @brief Vector color magenta constant.
    /// @return Value.
    static constexpr Vector3CPP magenta();
    /// @brief Vector color cyan constant.
    /// @return Value.
    static constexpr Vector3CPP cyan();
};
/// @brief 3 Component 32-bit Integer Vector.
struct IVector3CPP {
    /// @brief Union of vector component aliases.
    union {
        /// @brief X, Y, and Z components.
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
                /// @brief Swizzle X and Y components.
                IVector2CPP xy;
            };
            /// @brief Z component.
            i32 z;
        };
        /// @brief Swizzle Y and Z components.
        struct {
            i32 __unused0;
            /// @brief Swizzle Y and Z components.
            IVector2CPP yz;
        };
        /// @brief Swizzle XYZ.
        IVector3 xyz;

        /// @brief Swizzle XX.
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 0, 0> xx;
        /// @brief Swizzle XZ.
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 0, 2> xz;
        /// @brief Swizzle YZ.
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 1, 0> yx;
        /// @brief Swizzle YY.
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 1, 1> yy;
        /// @brief Swizzle ZX.
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 2, 0> zx;
        /// @brief Swizzle ZY.
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 2, 1> zy;
        /// @brief Swizzle ZZ.
        SwizzlerConvert<IVector3CPP, IVector2CPP, i32, 2, 2> zz;

        /// @brief Swizzle XXX.
        Swizzler<IVector3CPP, i32, 0, 0, 0> xxx;
        /// @brief Swizzle XXY.
        Swizzler<IVector3CPP, i32, 0, 0, 1> xxy;
        /// @brief Swizzle XXZ.
        Swizzler<IVector3CPP, i32, 0, 0, 2> xxz;
        /// @brief Swizzle XYX.
        Swizzler<IVector3CPP, i32, 0, 1, 0> xyx;
        /// @brief Swizzle XYY.
        Swizzler<IVector3CPP, i32, 0, 1, 1> xyy;
        /// @brief Swizzle XZX.
        Swizzler<IVector3CPP, i32, 0, 2, 0> xzx;
        /// @brief Swizzle XZY.
        Swizzler<IVector3CPP, i32, 0, 2, 1> xzy;
        /// @brief Swizzle XZZ.
        Swizzler<IVector3CPP, i32, 0, 2, 2> xzz;
        /// @brief Swizzle YXX.
        Swizzler<IVector3CPP, i32, 1, 0, 0> yxx;
        /// @brief Swizzle YXY.
        Swizzler<IVector3CPP, i32, 1, 0, 1> yxy;
        /// @brief Swizzle YXZ.
        Swizzler<IVector3CPP, i32, 1, 0, 2> yxz;
        /// @brief Swizzle YYX.
        Swizzler<IVector3CPP, i32, 1, 1, 0> yyx;
        /// @brief Swizzle YYY.
        Swizzler<IVector3CPP, i32, 1, 1, 1> yyy;
        /// @brief Swizzle YYZ.
        Swizzler<IVector3CPP, i32, 1, 1, 2> yyz;
        /// @brief Swizzle YZX.
        Swizzler<IVector3CPP, i32, 1, 2, 0> yzx;
        /// @brief Swizzle YZY.
        Swizzler<IVector3CPP, i32, 1, 2, 1> yzy;
        /// @brief Swizzle YZZ.
        Swizzler<IVector3CPP, i32, 1, 2, 2> yzz;
        /// @brief Swizzle ZXX.
        Swizzler<IVector3CPP, i32, 2, 0, 0> zxx;
        /// @brief Swizzle ZXY.
        Swizzler<IVector3CPP, i32, 2, 0, 1> zxy;
        /// @brief Swizzle ZXZ.
        Swizzler<IVector3CPP, i32, 2, 0, 2> zxz;
        /// @brief Swizzle ZYX.
        Swizzler<IVector3CPP, i32, 2, 1, 0> zyx;
        /// @brief Swizzle ZYY.
        Swizzler<IVector3CPP, i32, 2, 1, 1> zyy;
        /// @brief Swizzle ZYZ.
        Swizzler<IVector3CPP, i32, 2, 1, 2> zyz;
        /// @brief Swizzle ZZX.
        Swizzler<IVector3CPP, i32, 2, 2, 0> zzx;
        /// @brief Swizzle ZZY.
        Swizzler<IVector3CPP, i32, 2, 2, 1> zzy;
        /// @brief Swizzle ZZZ.
        Swizzler<IVector3CPP, i32, 2, 2, 2> zzz;

        /// @brief X, Y and Z components as an array.
        i32 array[3];
    };

    /// @brief Create empty vector.
    constexpr IVector3CPP();
    /// @brief Implicitly convert POD vector to C++ vector.
    constexpr IVector3CPP( const IVector3& __pod );
    /// @brief Create vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    constexpr explicit IVector3CPP( i32 x, i32 y, i32 z );
    /// @brief Create vector.
    /// @param x X, Y and Z components.
    constexpr explicit IVector3CPP( i32 x );
    /// @brief Create vector.
    /// @param x  X component.
    /// @param yz Y and Z components.
    constexpr explicit IVector3CPP( i32 x, IVector2CPP yz );
    /// @brief Create vector.
    /// @param xy X and Y components.
    /// @param z  Z component.
    constexpr explicit IVector3CPP( IVector2CPP xy, i32 z );
    /// @brief Convert float vector to integer vector.
    constexpr explicit IVector3CPP( const Vector3& v );
    /// @brief Convert boolean vector to integer vector.
    constexpr explicit IVector3CPP( const BVector3& bv );

    /// @brief Implicitly convert C++ vector to POD vector.
    constexpr operator IVector3() const;
    /// @brief Index into vector.
    /// @param index Index of component.
    /// @return Component.
    constexpr i32 operator[]( usize index ) const;
    /// @brief Index into vector.
    /// @param index Index of component.
    /// @return Component.
    constexpr i32& operator[]( usize index );

    /// @brief Vector zero constant.
    /// @return Value.
    static constexpr IVector3CPP zero();
    /// @brief Vector one constant.
    /// @return Value.
    static constexpr IVector3CPP one();
    /// @brief Vector up constant.
    /// @return Value.
    static constexpr IVector3CPP up();
    /// @brief Vector right constant.
    /// @return Value.
    static constexpr IVector3CPP right();
    /// @brief Vector down constant.
    /// @return Value.
    static constexpr IVector3CPP down();
    /// @brief Vector left constant.
    /// @return Value.
    static constexpr IVector3CPP left();
    /// @brief Vector forward constant.
    /// @return Value.
    static constexpr IVector3CPP forward();
    /// @brief Vector back constant.
    /// @return Value.
    static constexpr IVector3CPP back();
};
/// @brief 3 Component 32-bit Boolean Vector.
struct BVector3CPP {
    /// @brief Union of vector component aliases.
    union {
        /// @brief X, Y, and Z components.
        struct {
            /// @brief X and Y components.
            union {
                /// @brief X and Y components.
                struct {
                    /// @brief X component.
                    b32 x;
                    /// @brief Y component.
                    b32 y;
                };
                /// @brief Swizzle X and Y components.
                BVector2CPP xy;
            };
            /// @brief Z component.
            b32 z;
        };
        /// @brief Swizzle Y and Z components.
        struct {
            b32 __unused0;
            /// @brief Swizzle Y and Z components.
            BVector2CPP yz;
        };
        /// @brief Swizzle XYZ.
        BVector3 xyz;

        /// @brief Swizzle XX.
        SwizzlerConvert<BVector3CPP, BVector2CPP, b32, 0, 0> xx;
        /// @brief Swizzle XZ.
        SwizzlerConvert<BVector3CPP, BVector2CPP, b32, 0, 2> xz;
        /// @brief Swizzle YZ.
        SwizzlerConvert<BVector3CPP, BVector2CPP, b32, 1, 0> yx;
        /// @brief Swizzle YY.
        SwizzlerConvert<BVector3CPP, BVector2CPP, b32, 1, 1> yy;
        /// @brief Swizzle ZX.
        SwizzlerConvert<BVector3CPP, BVector2CPP, b32, 2, 0> zx;
        /// @brief Swizzle ZY.
        SwizzlerConvert<BVector3CPP, BVector2CPP, b32, 2, 1> zy;
        /// @brief Swizzle ZZ.
        SwizzlerConvert<BVector3CPP, BVector2CPP, b32, 2, 2> zz;

        /// @brief Swizzle XXX.
        Swizzler<BVector3CPP, b32, 0, 0, 0> xxx;
        /// @brief Swizzle XXY.
        Swizzler<BVector3CPP, b32, 0, 0, 1> xxy;
        /// @brief Swizzle XXZ.
        Swizzler<BVector3CPP, b32, 0, 0, 2> xxz;
        /// @brief Swizzle XYX.
        Swizzler<BVector3CPP, b32, 0, 1, 0> xyx;
        /// @brief Swizzle XYY.
        Swizzler<BVector3CPP, b32, 0, 1, 1> xyy;
        /// @brief Swizzle XZX.
        Swizzler<BVector3CPP, b32, 0, 2, 0> xzx;
        /// @brief Swizzle XZY.
        Swizzler<BVector3CPP, b32, 0, 2, 1> xzy;
        /// @brief Swizzle XZZ.
        Swizzler<BVector3CPP, b32, 0, 2, 2> xzz;
        /// @brief Swizzle YXX.
        Swizzler<BVector3CPP, b32, 1, 0, 0> yxx;
        /// @brief Swizzle YXY.
        Swizzler<BVector3CPP, b32, 1, 0, 1> yxy;
        /// @brief Swizzle YXZ.
        Swizzler<BVector3CPP, b32, 1, 0, 2> yxz;
        /// @brief Swizzle YYX.
        Swizzler<BVector3CPP, b32, 1, 1, 0> yyx;
        /// @brief Swizzle YYY.
        Swizzler<BVector3CPP, b32, 1, 1, 1> yyy;
        /// @brief Swizzle YYZ.
        Swizzler<BVector3CPP, b32, 1, 1, 2> yyz;
        /// @brief Swizzle YZX.
        Swizzler<BVector3CPP, b32, 1, 2, 0> yzx;
        /// @brief Swizzle YZY.
        Swizzler<BVector3CPP, b32, 1, 2, 1> yzy;
        /// @brief Swizzle YZZ.
        Swizzler<BVector3CPP, b32, 1, 2, 2> yzz;
        /// @brief Swizzle ZXX.
        Swizzler<BVector3CPP, b32, 2, 0, 0> zxx;
        /// @brief Swizzle ZXY.
        Swizzler<BVector3CPP, b32, 2, 0, 1> zxy;
        /// @brief Swizzle ZXZ.
        Swizzler<BVector3CPP, b32, 2, 0, 2> zxz;
        /// @brief Swizzle ZYX.
        Swizzler<BVector3CPP, b32, 2, 1, 0> zyx;
        /// @brief Swizzle ZYY.
        Swizzler<BVector3CPP, b32, 2, 1, 1> zyy;
        /// @brief Swizzle ZYZ.
        Swizzler<BVector3CPP, b32, 2, 1, 2> zyz;
        /// @brief Swizzle ZZX.
        Swizzler<BVector3CPP, b32, 2, 2, 0> zzx;
        /// @brief Swizzle ZZY.
        Swizzler<BVector3CPP, b32, 2, 2, 1> zzy;
        /// @brief Swizzle ZZZ.
        Swizzler<BVector3CPP, b32, 2, 2, 2> zzz;

        /// @brief X, Y and Z components as an array.
        b32 array[3];
    };

    /// @brief Create empty vector.
    constexpr BVector3CPP();
    /// @brief Implicitly convert POD vector to C++ vector.
    constexpr BVector3CPP( const BVector3& __pod );
    /// @brief Create vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    constexpr explicit BVector3CPP( b32 x, b32 y, b32 z );
    /// @brief Create vector.
    /// @param x X, Y and Z components.
    constexpr explicit BVector3CPP( b32 x );
    /// @brief Create vector.
    /// @param x  X component.
    /// @param yz Y and Z components.
    constexpr explicit BVector3CPP( b32 x, BVector2CPP yz );
    /// @brief Create vector.
    /// @param xy X and Y components.
    /// @param z  Z component.
    constexpr explicit BVector3CPP( BVector2CPP xy, b32 z );
    /// @brief Convert float vector to boolean vector.
    constexpr explicit BVector3CPP( const Vector3& v );
    /// @brief Convert integer vector to boolean vector.
    constexpr explicit BVector3CPP( const IVector3& bv );

    /// @brief Implicitly convert C++ vector to POD vector.
    constexpr operator BVector3() const;
    /// @brief Index into vector.
    /// @param index Index of component.
    /// @return Component.
    constexpr b32 operator[]( usize index ) const;
    /// @brief Index into vector.
    /// @param index Index of component.
    /// @return Component.
    constexpr b32& operator[]( usize index );

    /// @brief Vector zero constant.
    /// @return Value.
    static constexpr BVector3CPP zero();
    /// @brief Vector one constant.
    /// @return Value.
    static constexpr BVector3CPP one();
};

/// @brief 3 Component 32-bit Floating Point Vector.
typedef  Vector3CPP  vec3;
/// @brief 3 Component 32-bit Integer Vector.
typedef IVector3CPP ivec3;
/// @brief 3 Component 32-bit Boolean Vector.
typedef BVector3CPP bvec3;

constexpr attr_always_inline attr_header attr_hot
Vector3CPP::Vector3CPP()
    : x(0.0f), y(0.0f), z(0.0f) {}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP::Vector3CPP( const Vector3& __pod )
    : x(__pod.x), y(__pod.y), z(__pod.z) {}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP::Vector3CPP( f32 x, f32 y, f32 z )
    : x(x), y(y), z(z) {}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP::Vector3CPP( f32 x )
    : x(x), y(x), z(x) {}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP::Vector3CPP( f32 x, Vector2CPP yz )
    : x(x), y(yz.x), z(yz.y) {}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP::Vector3CPP( Vector2CPP xy, f32 z )
    : x(xy.x), y(xy.y), z(z) {}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP::Vector3CPP( const IVector3& iv )
    : x(iv.x), y(iv.y), z(iv.z) {}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP::Vector3CPP( const BVector3& bv )
    : x(bv.x ? 1.0f : 0.0f), y(bv.y ? 1.0f : 0.0f), z(bv.z ? 1.0f : 0.0f) {} 

constexpr attr_always_inline attr_header attr_hot
Vector3CPP::operator Vector3() const {
    return *(Vector3*)this;
}
constexpr attr_always_inline attr_header attr_hot
f32 Vector3CPP::operator[]( usize index ) const {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
f32& Vector3CPP::operator[]( usize index ) {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::zero() {
    return VEC3_ZERO;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::one() {
    return VEC3_ONE;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::up() {
    return VEC3_UP;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::right() {
    return VEC3_RIGHT;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::down() {
    return VEC3_DOWN;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::left() {
    return VEC3_LEFT;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::forward() {
    return VEC3_FORWARD;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::back() {
    return VEC3_BACK;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::black() {
    return RGB_BLACK;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::white() {
    return RGB_WHITE;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::red() {
    return RGB_RED;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::blue() {
    return RGB_BLUE;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::green() {
    return RGB_GREEN;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::yellow() {
    return RGB_YELLOW;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::magenta() {
    return RGB_MAGENTA;
}
constexpr attr_always_inline attr_header attr_hot
Vector3CPP Vector3CPP::cyan() {
    return RGB_CYAN;
}

/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
constexpr attr_always_inline attr_header attr_hot
vec3 add( vec3 lhs, vec3 rhs ) {
    return vec3_add( lhs, rhs );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
constexpr attr_always_inline attr_header attr_hot
vec3 sub( vec3 lhs, vec3 rhs ) {
    return vec3_sub( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec3 mul( vec3 lhs, f32 rhs ) {
    return vec3_mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Scalar to multiply components by.
/// @param rhs Vector to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec3 mul( f32 lhs, vec3 rhs ) {
    return vec3_mul( rhs, lhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec3 mul( vec3 lhs, vec3 rhs ) {
    return vec3_mul_vec3( lhs, rhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec3 hadamard( vec3 lhs, vec3 rhs ) {
    return vec3_mul_vec3( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec3 div( vec3 lhs, f32 rhs ) {
    return vec3_div( lhs, rhs );
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec3 div( vec3 lhs, vec3 rhs ) {
    return vec3_div_vec3( lhs, rhs );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of modulus division.
constexpr attr_always_inline attr_header attr_hot
vec3 mod( vec3 lhs, f32 rhs ) {
    return vec3_mod( lhs, rhs );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of modulus division.
constexpr attr_always_inline attr_header attr_hot
vec3 mod( vec3 lhs, vec3 rhs ) {
    return vec3_mod_vec3( lhs, rhs );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
constexpr attr_always_inline attr_header attr_hot
vec3 neg( vec3 x ) {
    return vec3_neg( x );
}

/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
constexpr attr_always_inline attr_header attr_hot
vec3 operator+( vec3 lhs, vec3 rhs ) {
    return add( lhs, rhs );
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
constexpr attr_always_inline attr_header attr_hot
vec3& operator+=( vec3& lhs, vec3 rhs ) {
    return lhs = lhs + rhs;
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
constexpr attr_always_inline attr_header attr_hot
vec3 operator-( vec3 lhs, vec3 rhs ) {
    return sub( lhs, rhs );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
constexpr attr_always_inline attr_header attr_hot
vec3& operator-=( vec3& lhs, vec3 rhs ) {
    return lhs = lhs - rhs;
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec3 operator*( vec3 lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Scalar to multiply components by.
/// @param rhs Vector to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec3 operator*( f32 lhs, vec3 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec3 operator*( vec3 lhs, vec3 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec3& operator*=( vec3& lhs, f32 rhs ) {
    return lhs = lhs * rhs;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec3& operator*=( vec3& lhs, vec3 rhs ) {
    return lhs = lhs * rhs;
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec3 operator/( vec3 lhs, f32 rhs ) {
    return div( lhs, rhs );
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec3 operator/( vec3 lhs, vec3 rhs ) {
    return div( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec3& operator/=( vec3 lhs, f32 rhs ) {
    return lhs = lhs / rhs;
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec3& operator/=( vec3 lhs, vec3 rhs ) {
    return lhs = lhs / rhs;
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
constexpr attr_always_inline attr_header attr_hot
vec3 operator-( vec3 x ) {
    return neg(x);
}

attr_always_inline attr_header attr_hot
f32 hadd( vec3 x ) {
    return vec3_hadd( x );
}
attr_always_inline attr_header attr_hot
f32 hmul( vec3 x ) {
    return vec3_hmul( x );
}
attr_always_inline attr_header attr_hot
vec3 cross( vec3 lhs, vec3 rhs ) {
    return vec3_cross( lhs, rhs );
}
attr_always_inline attr_header attr_hot
f32 dot( vec3 lhs, vec3 rhs ) {
    return vec3_dot( lhs, rhs );
}
attr_always_inline attr_header attr_hot
vec3 rotl( vec3 x ) {
    return vec3_rotl( x );
}
attr_always_inline attr_header attr_hot
vec3 rotr( vec3 x ) {
    return vec3_rotr( x );
}
attr_always_inline attr_header attr_hot
f32 max( vec3 x ) {
    return vec3_max( x );
}
attr_always_inline attr_header attr_hot
vec3 max( vec3 x, vec3 y ) {
    return vec3_max_vec3( x, y );
}
attr_always_inline attr_header attr_hot
f32 mid( vec3 x ) {
    return vec3_mid( x );
}
attr_always_inline attr_header attr_hot
f32 min( vec3 x ) {
    return vec3_min( x );
}
attr_always_inline attr_header attr_hot
vec3 min( vec3 x, vec3 y ) {
    return vec3_min_vec3( x, y );
}
attr_always_inline attr_header attr_hot
vec3 clamp( vec3 v, vec3 min, vec3 max ) {
    return vec3_clamp( v, min, max );
}
attr_always_inline attr_header attr_hot
vec3 clamp( vec3 v, f32 min, f32 max ) {
    return vec3_clamp_length( v, min, max );
}
attr_always_inline attr_header attr_hot
f32 length_sqr( vec3 x ) {
    return vec3_length_sqr( x );
}
attr_always_inline attr_header attr_hot
f32 length( vec3 x ) {
    return vec3_length( x );
}
attr_always_inline attr_header attr_hot
f32 distance_sqr( vec3 a, vec3 b ) {
    return vec3_distance_sqr( a, b );
}
attr_always_inline attr_header attr_hot
f32 distance( vec3 a, vec3 b ) {
    return vec3_distance( a, b );
}
attr_always_inline attr_header attr_hot
vec3 normalize( vec3 x ) {
    return vec3_normalize( x );
}
attr_always_inline attr_header attr_hot
vec3 reflect( vec3 d, vec3 n ) {
    return vec3_reflect( d, n );
}
attr_always_inline attr_header attr_hot
f32 angle( vec3 a, vec3 b ) {
    return vec3_angle( a, b );
}
attr_always_inline attr_header attr_hot
vec3 abs( vec3 x ) {
    return vec3_abs( x );
}
attr_always_inline attr_header attr_hot
vec3 sign( vec3 x ) {
    return vec3_sign( x );
}
attr_always_inline attr_header attr_hot
vec3 trunc( vec3 x ) {
    return vec3_trunc( x );
}
attr_always_inline attr_header attr_hot
vec3 floor( vec3 x ) {
    return vec3_floor( x );
}
attr_always_inline attr_header attr_hot
vec3 ceil( vec3 x ) {
    return vec3_ceil( x );
}
attr_always_inline attr_header attr_hot
vec3 round( vec3 x ) {
    return vec3_round( x );
}
attr_always_inline attr_header attr_hot
vec3 fract( vec3 x ) {
    return vec3_fract( x );
}
attr_always_inline attr_header attr_hot
vec3 lerp( vec3 a, vec3 b, f32 t ) {
    return vec3_lerp( a, b, t );
}
attr_always_inline attr_header attr_hot
vec3 lerp( vec3 a, vec3 b, vec3 t ) {
    return vec3_lerp_vec3( a, b, t );
}
attr_always_inline attr_header attr_hot
vec3 mix( vec3 a, vec3 b, f32 t ) {
    return lerp( a, b, t );
}
attr_always_inline attr_header attr_hot
vec3 mix( vec3 a, vec3 b, vec3 t ) {
    return lerp( a, b, t );
}
attr_always_inline attr_header attr_hot
vec3 slerp( vec3 a, vec3 b, f32 t ) {
    return vec3_slerp( a, b, t );
}
attr_always_inline attr_header attr_hot
vec3 step( f32 edge, vec3 x ) {
    return vec3_step( edge, x );
}
attr_always_inline attr_header attr_hot
vec3 step( vec3 edge, vec3 x ) {
    return vec3_step_vec3( edge, x );
}
attr_always_inline attr_header attr_hot
vec3 smoothstep( f32 edge0, f32 edge1, vec3 x ) {
    return vec3_smoothstep( edge0, edge1, x );
}
attr_always_inline attr_header attr_hot
vec3 smoothstep( vec3 edge0, vec3 edge1, vec3 x ) {
    return vec3_smoothstep_vec3( edge0, edge1, x );
}
attr_always_inline attr_header attr_hot
vec3 smootherstep( f32 edge0, f32 edge1, vec3 x ) {
    return vec3_smootherstep( edge0, edge1, x );
}
attr_always_inline attr_header attr_hot
vec3 smootherstep( vec3 edge0, vec3 edge1, vec3 x ) {
    return vec3_smootherstep_vec3( edge0, edge1, x );
}
attr_always_inline attr_header attr_hot
bvec3 isnan( vec3 x ) {
    return vec3_isnan( x );
}
attr_always_inline attr_header attr_hot
bvec3 isinf( vec3 x ) {
    return vec3_isinf( x );
}
attr_always_inline attr_header attr_hot
vec3 radians( vec3 degrees ) {
    return vec3_radians( degrees );
}
attr_always_inline attr_header attr_hot
vec3 degrees( vec3 radians ) {
    return vec3_degrees( radians );
}
attr_always_inline attr_header attr_hot
vec3 sin( vec3 x ) {
    return vec3_sin( x );
}
attr_always_inline attr_header attr_hot
vec3 cos( vec3 x ) {
    return vec3_cos( x );
}
attr_always_inline attr_header attr_hot
vec3 tan( vec3 x ) {
    return vec3_tan( x );
}
attr_always_inline attr_header attr_hot
vec3 asin( vec3 x ) {
    return vec3_asin( x );
}
attr_always_inline attr_header attr_hot
vec3 acos( vec3 x ) {
    return vec3_acos( x );
}
attr_always_inline attr_header attr_hot
vec3 atan( vec3 x ) {
    return vec3_atan( x );
}
attr_always_inline attr_header attr_hot
vec3 atan2( vec3 y, vec3 x ) {
    return vec3_atan2( y, x );
}
attr_always_inline attr_header attr_hot
vec3 pow( vec3 base, vec3 exp ) {
    return vec3_pow( base, exp );
}
attr_always_inline attr_header attr_hot
vec3 exp( vec3 x ) {
    return vec3_exp( x );
}
attr_always_inline attr_header attr_hot
vec3 exp2( vec3 x ) {
    return vec3_exp2( x );
}
attr_always_inline attr_header attr_hot
vec3 ln( vec3 x ) {
    return vec3_ln( x );
}
attr_always_inline attr_header attr_hot
vec3 log2( vec3 x ) {
    return vec3_log2( x );
}
attr_always_inline attr_header attr_hot
vec3 log10( vec3 x ) {
    return vec3_log10( x );
}
attr_always_inline attr_header attr_hot
vec3 sqrt( vec3 x ) {
    return vec3_sqrt( x );
}
attr_always_inline attr_header attr_hot
vec3 inversesqrt( vec3 x ) {
    return vec3_inversesqrt( x );
}
attr_always_inline attr_header attr_hot
vec3 cbrt( vec3 x ) {
    return vec3_cbrt( x );
}
attr_always_inline attr_header attr_hot
bool cmp( vec3 a, vec3 b ) {
    return vec3_cmp( a, b );
}
attr_always_inline attr_header attr_hot
bvec3 lt( vec3 a, vec3 b ) {
    return vec3_lt( a, b );
}
attr_always_inline attr_header attr_hot
bvec3 lteq( vec3 a, vec3 b ) {
    return vec3_lteq( a, b );
}
attr_always_inline attr_header attr_hot
bvec3 gt( vec3 a, vec3 b ) {
    return vec3_gt( a, b );
}
attr_always_inline attr_header attr_hot
bvec3 gteq( vec3 a, vec3 b ) {
    return vec3_gteq( a, b );
}
attr_always_inline attr_header attr_hot
bvec3 eq( vec3 a, vec3 b ) {
    return vec3_eq( a, b );
}
attr_always_inline attr_header attr_hot
bvec3 neq( vec3 a, vec3 b ) {
    return vec3_neq( a, b );
}

constexpr attr_always_inline attr_header attr_hot
IVector3CPP::IVector3CPP()
    : x(0), y(0), z(0) {}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP::IVector3CPP( const IVector3& __pod )
    : x(__pod.x), y(__pod.y), z(__pod.z) {}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP::IVector3CPP( i32 x, i32 y, i32 z )
    : x(x), y(y), z(z) {}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP::IVector3CPP( i32 x )
    : x(x), y(x), z(x) {}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP::IVector3CPP( i32 x, IVector2CPP yz )
    : x(x), y(yz.x), z(yz.y) {}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP::IVector3CPP( IVector2CPP xy, i32 z )
    : x(xy.x), y(xy.y), z(z) {}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP::IVector3CPP( const Vector3& v )
    : x(v.x), y(v.y), z(v.z) {}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP::IVector3CPP( const BVector3& bv )
    : x(bv.x ? 1 : 0), y(bv.y ? 1 : 0), z(bv.z ? 1 : 0) {} 

constexpr attr_always_inline attr_header attr_hot
IVector3CPP::operator IVector3() const {
    return *(IVector3*)this;
}
constexpr attr_always_inline attr_header attr_hot
i32 IVector3CPP::operator[]( usize index ) const {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
i32& IVector3CPP::operator[]( usize index ) {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP IVector3CPP::zero() {
    return IVEC3_ZERO;
}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP IVector3CPP::one() {
    return IVEC3_ONE;
}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP IVector3CPP::up() {
    return IVEC3_UP;
}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP IVector3CPP::right() {
    return IVEC3_RIGHT;
}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP IVector3CPP::down() {
    return IVEC3_DOWN;
}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP IVector3CPP::left() {
    return IVEC3_LEFT;
}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP IVector3CPP::forward() {
    return IVEC3_FORWARD;
}
constexpr attr_always_inline attr_header attr_hot
IVector3CPP IVector3CPP::back() {
    return IVEC3_BACK;
}

/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
constexpr attr_always_inline attr_header attr_hot
ivec3 add( ivec3 lhs, ivec3 rhs ) {
    return ivec3_add( lhs, rhs );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
constexpr attr_always_inline attr_header attr_hot
ivec3 sub( ivec3 lhs, ivec3 rhs ) {
    return ivec3_sub( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
ivec3 mul( ivec3 lhs, i32 rhs ) {
    return ivec3_mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Scalar to multiply components by.
/// @param rhs Vector to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
ivec3 mul( i32 lhs, ivec3 rhs ) {
    return ivec3_mul( rhs, lhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
ivec3 mul( ivec3 lhs, ivec3 rhs ) {
    return ivec3_mul_ivec3( lhs, rhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
ivec3 hadamard( ivec3 lhs, ivec3 rhs ) {
    return ivec3_mul_ivec3( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
ivec3 div( ivec3 lhs, i32 rhs ) {
    return ivec3_div( lhs, rhs );
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
ivec3 div( ivec3 lhs, ivec3 rhs ) {
    return ivec3_div_ivec3( lhs, rhs );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of modulus division.
constexpr attr_always_inline attr_header attr_hot
ivec3 mod( ivec3 lhs, i32 rhs ) {
    return ivec3_mod( lhs, rhs );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of modulus division.
constexpr attr_always_inline attr_header attr_hot
ivec3 mod( ivec3 lhs, ivec3 rhs ) {
    return ivec3_mod_ivec3( lhs, rhs );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
constexpr attr_always_inline attr_header attr_hot
ivec3 neg( ivec3 x ) {
    return ivec3_neg( x );
}

/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
constexpr attr_always_inline attr_header attr_hot
ivec3 operator+( ivec3 lhs, ivec3 rhs ) {
    return add( lhs, rhs );
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
constexpr attr_always_inline attr_header attr_hot
ivec3& operator+=( ivec3& lhs, ivec3 rhs ) {
    return lhs = lhs + rhs;
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
constexpr attr_always_inline attr_header attr_hot
ivec3 operator-( ivec3 lhs, ivec3 rhs ) {
    return sub( lhs, rhs );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
constexpr attr_always_inline attr_header attr_hot
ivec3& operator-=( ivec3& lhs, ivec3 rhs ) {
    return lhs = lhs - rhs;
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
ivec3 operator*( ivec3 lhs, i32 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Scalar to multiply components by.
/// @param rhs Vector to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
ivec3 operator*( i32 lhs, ivec3 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
ivec3 operator*( ivec3 lhs, ivec3 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
ivec3& operator*=( ivec3& lhs, i32 rhs ) {
    return lhs = lhs * rhs;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
ivec3& operator*=( ivec3& lhs, ivec3 rhs ) {
    return lhs = lhs * rhs;
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
ivec3 operator/( ivec3 lhs, i32 rhs ) {
    return div( lhs, rhs );
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
ivec3 operator/( ivec3 lhs, ivec3 rhs ) {
    return div( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
ivec3& operator/=( ivec3 lhs, i32 rhs ) {
    return lhs = lhs / rhs;
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
ivec3& operator/=( ivec3 lhs, ivec3 rhs ) {
    return lhs = lhs / rhs;
}
/// @brief Modular division vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
ivec3 operator%( ivec3 lhs, i32 rhs ) {
    return mod( lhs, rhs );
}
/// @brief Component-wise modular division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
ivec3 operator%( ivec3 lhs, ivec3 rhs ) {
    return mod( lhs, rhs );
}
/// @brief Modular divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
ivec3& operator%=( ivec3 lhs, i32 rhs ) {
    return lhs = lhs % rhs;
}
/// @brief Component-wise modular division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
ivec3& operator%=( ivec3 lhs, ivec3 rhs ) {
    return lhs = lhs % rhs;
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
constexpr attr_always_inline attr_header attr_hot
ivec3 operator-( ivec3 x ) {
    return neg(x);
}

attr_always_inline attr_header attr_hot
ivec3 rotl( ivec3 x ) {
    return ivec3_rotl( x );
}
attr_always_inline attr_header attr_hot
ivec3 rotr( ivec3 x ) {
    return ivec3_rotr( x );
}
attr_always_inline attr_header attr_hot
i32 hadd( ivec3 x ) {
    return ivec3_hadd( x );
}
attr_always_inline attr_header attr_hot
i32 hmul( ivec3 x ) {
    return ivec3_hmul( x );
}
attr_always_inline attr_header attr_hot
f32 dot( ivec3 lhs, ivec3 rhs ) {
    return ivec3_dot( lhs, rhs );
}
attr_always_inline attr_header attr_hot
f32 length_sqr( ivec3 x ) {
    return ivec3_length_sqr( x );
}
attr_always_inline attr_header attr_hot
f32 length( ivec3 x ) {
    return ivec3_length( x );
}
attr_always_inline attr_header attr_hot
ivec3 abs( ivec3 x ) {
    return ivec3_abs( x );
}
attr_always_inline attr_header attr_hot
ivec3 sign( ivec3 x ) {
    return ivec3_sign( x );
}
attr_always_inline attr_header attr_hot
i32 min( ivec3 x ) {
    return ivec3_min( x );
}
attr_always_inline attr_header attr_hot
ivec3 min( ivec3 x, ivec3 y ) {
    return ivec3_min_ivec3( x, y );
}
attr_always_inline attr_header attr_hot
i32 mid( ivec3 x ) {
    return ivec3_mid( x );
}
attr_always_inline attr_header attr_hot
i32 max( ivec3 x ) {
    return ivec3_max( x );
}
attr_always_inline attr_header attr_hot
ivec3 max( ivec3 x, ivec3 y ) {
    return ivec3_max_ivec3( x, y );
}
attr_always_inline attr_header attr_hot
ivec3 clamp( ivec3 v, ivec3 min, ivec3 max ) {
    return ivec3_clamp( v, min, max );
}
attr_always_inline attr_header attr_hot
bvec3 lt( ivec3 lhs, ivec3 rhs ) {
    return ivec3_lt( lhs, rhs );
}
attr_always_inline attr_header attr_hot
bvec3 lteq( ivec3 lhs, ivec3 rhs ) {
    return ivec3_lteq( lhs, rhs );
}
attr_always_inline attr_header attr_hot
bvec3 gt( ivec3 lhs, ivec3 rhs ) {
    return ivec3_gt( lhs, rhs );
}
attr_always_inline attr_header attr_hot
bvec3 gteq( ivec3 lhs, ivec3 rhs ) {
    return ivec3_gteq( lhs, rhs );
}
attr_always_inline attr_header attr_hot
bvec3 eq( ivec3 lhs, ivec3 rhs ) {
    return ivec3_eq( lhs, rhs );
}
attr_always_inline attr_header attr_hot
bvec3 neq( ivec3 lhs, ivec3 rhs ) {
    return ivec3_neq( lhs, rhs );
}

constexpr attr_always_inline attr_header attr_hot
BVector3CPP::BVector3CPP()
    : x(false), y(false), z(false) {}
constexpr attr_always_inline attr_header attr_hot
BVector3CPP::BVector3CPP( const BVector3& __pod )
    : x(__pod.x), y(__pod.y), z(__pod.z) {}
constexpr attr_always_inline attr_header attr_hot
BVector3CPP::BVector3CPP( b32 x, b32 y, b32 z )
    : x(x), y(y), z(z) {}
constexpr attr_always_inline attr_header attr_hot
BVector3CPP::BVector3CPP( b32 x )
    : x(x), y(x), z(x) {}
constexpr attr_always_inline attr_header attr_hot
BVector3CPP::BVector3CPP( b32 x, BVector2CPP yz )
    : x(x), y(yz.x), z(yz.y) {}
constexpr attr_always_inline attr_header attr_hot
BVector3CPP::BVector3CPP( BVector2CPP xy, b32 z )
    : x(xy.x), y(xy.y), z(z) {}
constexpr attr_always_inline attr_header attr_hot
BVector3CPP::BVector3CPP( const Vector3& v )
    : x(v.x == 0.0f ? false : true),
      y(v.y == 0.0f ? false : true),
      z(v.z == 0.0f ? false : true) {} 
constexpr attr_always_inline attr_header attr_hot
BVector3CPP::BVector3CPP( const IVector3& iv )
    : x(iv.x), y(iv.y), z(iv.z) {}

constexpr attr_always_inline attr_header attr_hot
BVector3CPP::operator BVector3() const {
    return *(BVector3*)this;
}
constexpr attr_always_inline attr_header attr_hot
b32 BVector3CPP::operator[]( usize index ) const {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
b32& BVector3CPP::operator[]( usize index ) {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
BVector3CPP BVector3CPP::zero() {
    return BVEC3_ZERO;
}
constexpr attr_always_inline attr_header attr_hot
BVector3CPP BVector3CPP::one() {
    return BVEC3_ONE;
}

attr_always_inline attr_header attr_hot
bvec3 eq( bvec3 a, bvec3 b ) {
    return bvec3_eq( a, b );
}
attr_always_inline attr_header attr_hot
bvec3 neq( bvec3 a, bvec3 b ) {
    return bvec3_neq( a, b );
}
attr_always_inline attr_header attr_hot
bool any( bvec3 x ) {
    return bvec3_any( x );
}
attr_always_inline attr_header attr_hot
bool all( bvec3 x ) {
    return bvec3_all( x );
}
attr_always_inline attr_header attr_hot
bvec3 flip( bvec3 x ) {
    return bvec3_flip( x );
}

#endif /* header guard */
