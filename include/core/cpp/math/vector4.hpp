#if !defined(CORE_CPP_MATH_VECTOR4_HPP) && defined(__cplusplus)
#define CORE_CPP_MATH_VECTOR4_HPP
/**
 * @file   vector4.hpp
 * @brief  C++ Math: Vector4.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 30, 2025
*/
#if !defined(CORE_MATH_VECTOR4_H)
    #include "core/math/vector4.h"
#endif
#include "core/cpp/math/swizzler.hpp"

/// @brief 4 Component 32-bit Unsigned Integer Vector.
typedef UVector4 uvec4;
/// @brief 4 Component 64-bit Floating Point Vector.
typedef DVector4 dvec4;

/// @brief 4 Component 32-bit Floating Point Vector.
struct Vector4CPP {
    union {
        /// @brief X, Y, Z and W components.
        struct {
            /// @brief X, Y and Z components.
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
                /// @brief X, Y and Z components as a #Vector3.
                Vector3CPP xyz;
                /// @brief Red, Green and Blue components as a #Vector3.
                Vector3CPP rgb;
            };
            /// @brief W component.
            union {
                /// @brief W component.
                f32 w;
                /// @brief Alpha channel (W component).
                f32 a;
            };
        };
        /// @brief Swizzle yzw.
        struct {
            f32 __unused2;
            /// @brief Y, Z and W components as a #Vector3.
            Vector3CPP yzw;
        };
        /// @brief Swizzle gba.
        struct {
            f32 __unused3;
            /// @brief Green, Blue and Alpha channels as a #Vector3.
            Vector3CPP gba;
        };
        /// @brief Swizzle zw.
        struct {
            Vector2CPP __unused4;
            /// @brief Z and W components as a #Vector2.
            Vector2CPP zw;
        };
        /// @brief Swizzle ba.
        struct {
            Vector2CPP __unused5;
            /// @brief Blue and Alpha channels as a #Vector2.
            Vector2CPP ba;
        };
        /// @brief Swizzle XYZW.
        struct Vector4 xyzw;
        /// @brief Swizzle RGBA.
        struct Vector4 rgba;

        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 0, 0> xx, rr;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 0, 2> xz, rb;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 0, 3> xw, ra;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 1, 0> yx, gr;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 1, 1> yy, gg;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 1, 3> yw, ga;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 2, 0> zx, br;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 2, 1> zy, bg;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 2, 2> zz, bb;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 3, 0> wx, ar;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 3, 1> wy, ag;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 3, 2> wz, ab;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 3, 3> ww, aa;

        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 0, 0> xxx, rrr;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 0, 1> xxy, rrg;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 0, 2> xxz, rrb;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 0, 3> xxw, rra;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 1, 0> xyx, rgr;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 1, 1> xyy, rgg;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 1, 3> xyw, rga;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 2, 0> xzx, rbr;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 2, 1> xzy, rbg;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 2, 2> xzz, rbb;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 2, 3> xzw, rba;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 3, 0> xwx, rar;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 3, 1> xwy, rag;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 3, 2> xwz, rab;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 3, 3> xww, raa;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 0, 0> yxx, grr;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 0, 1> yxy, grg;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 0, 2> yxz, grb;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 0, 3> yxw, gra;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 1, 0> yyx, ggr;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 1, 1> yyy, ggg;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 1, 2> yyz, ggb;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 1, 3> yyw, gga;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 2, 0> yzx, gbr;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 2, 1> yzy, gbg;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 2, 2> yzz, gbb;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 3, 0> ywx, gar;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 3, 1> ywy, gag;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 3, 2> ywz, gab;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 3, 3> yww, gaa;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 0, 0> zxx, brr;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 0, 1> zxy, brg;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 0, 2> zxz, brb;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 0, 3> zxw, bra;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 1, 0> zyx, bgr;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 1, 1> zyy, bgg;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 1, 2> zyz, bgb;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 1, 3> zyw, bga;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 2, 0> zzx, bbr;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 2, 1> zzy, bbg;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 2, 2> zzz, bbb;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 2, 3> zzw, bba;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 3, 0> zwx, bar;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 3, 1> zwy, bag;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 3, 2> zwz, bab;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 2, 3, 3> zww, baa;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 0, 0> wxx, arr;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 0, 1> wxy, arg;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 0, 2> wxz, arb;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 0, 3> wxw, ara;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 1, 0> wyx, agr;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 1, 1> wyy, agg;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 1, 2> wyz, agb;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 1, 3> wyw, aga;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 2, 0> wzx, abr;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 2, 1> wzy, abg;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 2, 2> wzz, abb;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 2, 3> wzw, aba;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 3, 0> wwx, aar;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 3, 1> wwy, aag;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 3, 2> wwz, aab;
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 3, 3, 3> www, aaa;

        Swizzler<Vector4CPP, f32, 0, 0, 0, 0> xxxx, rrrr;
        Swizzler<Vector4CPP, f32, 0, 0, 0, 1> xxxy, rrrg;
        Swizzler<Vector4CPP, f32, 0, 0, 0, 2> xxxz, rrrb;
        Swizzler<Vector4CPP, f32, 0, 0, 0, 3> xxxw, rrra;
        Swizzler<Vector4CPP, f32, 0, 0, 1, 0> xxyx, rrgr;
        Swizzler<Vector4CPP, f32, 0, 0, 1, 1> xxyy, rrgg;
        Swizzler<Vector4CPP, f32, 0, 0, 1, 2> xxyz, rrgb;
        Swizzler<Vector4CPP, f32, 0, 0, 1, 3> xxyw, rrga;
        Swizzler<Vector4CPP, f32, 0, 0, 2, 0> xxzx, rrbr;
        Swizzler<Vector4CPP, f32, 0, 0, 2, 1> xxzy, rrbg;
        Swizzler<Vector4CPP, f32, 0, 0, 2, 2> xxzz, rrbb;
        Swizzler<Vector4CPP, f32, 0, 0, 2, 3> xxzw, rrba;
        Swizzler<Vector4CPP, f32, 0, 0, 3, 0> xxwx, rrar;
        Swizzler<Vector4CPP, f32, 0, 0, 3, 1> xxwy, rrag;
        Swizzler<Vector4CPP, f32, 0, 0, 3, 2> xxwz, rrab;
        Swizzler<Vector4CPP, f32, 0, 0, 3, 3> xxww, rraa;
        Swizzler<Vector4CPP, f32, 0, 1, 0, 0> xyxx, rgrr;
        Swizzler<Vector4CPP, f32, 0, 1, 0, 1> xyxy, rgrg;
        Swizzler<Vector4CPP, f32, 0, 1, 0, 2> xyxz, rgrb;
        Swizzler<Vector4CPP, f32, 0, 1, 0, 3> xyxw, rgra;
        Swizzler<Vector4CPP, f32, 0, 1, 1, 0> xyyx, rggr;
        Swizzler<Vector4CPP, f32, 0, 1, 1, 1> xyyy, rggg;
        Swizzler<Vector4CPP, f32, 0, 1, 1, 0> xyyz, rggb;
        Swizzler<Vector4CPP, f32, 0, 1, 1, 3> xyyw, rgga;
        Swizzler<Vector4CPP, f32, 0, 1, 2, 0> xyzx, rgbr;
        Swizzler<Vector4CPP, f32, 0, 1, 2, 1> xyzy, rgbg;
        Swizzler<Vector4CPP, f32, 0, 1, 2, 2> xyzz, rgbb;
        Swizzler<Vector4CPP, f32, 0, 1, 3, 0> xywx, rgar;
        Swizzler<Vector4CPP, f32, 0, 1, 3, 1> xywy, rgag;
        Swizzler<Vector4CPP, f32, 0, 1, 3, 2> xywz, rgab;
        Swizzler<Vector4CPP, f32, 0, 1, 3, 3> xyww, rgaa;
        Swizzler<Vector4CPP, f32, 0, 2, 0, 0> xzxx, rbrr;
        Swizzler<Vector4CPP, f32, 0, 2, 0, 1> xzxy, rbrg;
        Swizzler<Vector4CPP, f32, 0, 2, 0, 2> xzxz, rbrb;
        Swizzler<Vector4CPP, f32, 0, 2, 0, 3> xzxw, rbra;
        Swizzler<Vector4CPP, f32, 0, 2, 1, 0> xzyx, rbgr;
        Swizzler<Vector4CPP, f32, 0, 2, 1, 1> xzyy, rbgg;
        Swizzler<Vector4CPP, f32, 0, 2, 1, 2> xzyz, rbgb;
        Swizzler<Vector4CPP, f32, 0, 2, 1, 3> xzyw, rbga;
        Swizzler<Vector4CPP, f32, 0, 2, 2, 0> xzzx, rbbr;
        Swizzler<Vector4CPP, f32, 0, 2, 2, 1> xzzy, rbbg;
        Swizzler<Vector4CPP, f32, 0, 2, 2, 2> xzzz, rbbb;
        Swizzler<Vector4CPP, f32, 0, 2, 2, 3> xzzw, rbba;
        Swizzler<Vector4CPP, f32, 0, 2, 3, 0> xzwx, rbar;
        Swizzler<Vector4CPP, f32, 0, 2, 3, 1> xzwy, rbag;
        Swizzler<Vector4CPP, f32, 0, 2, 3, 2> xzwz, rbab;
        Swizzler<Vector4CPP, f32, 0, 2, 3, 3> xzww, rbaa;
        Swizzler<Vector4CPP, f32, 0, 3, 0, 0> xwxx, rarr;
        Swizzler<Vector4CPP, f32, 0, 3, 0, 1> xwxy, rarg;
        Swizzler<Vector4CPP, f32, 0, 3, 0, 2> xwxz, rarb;
        Swizzler<Vector4CPP, f32, 0, 3, 0, 3> xwxw, rara;
        Swizzler<Vector4CPP, f32, 0, 3, 1, 0> xwyx, ragr;
        Swizzler<Vector4CPP, f32, 0, 3, 1, 1> xwyy, ragg;
        Swizzler<Vector4CPP, f32, 0, 3, 1, 2> xwyz, ragb;
        Swizzler<Vector4CPP, f32, 0, 3, 1, 3> xwyw, raga;
        Swizzler<Vector4CPP, f32, 0, 3, 0, 3> xwzx, rabr;
        Swizzler<Vector4CPP, f32, 0, 3, 2, 1> xwzy, rabg;
        Swizzler<Vector4CPP, f32, 0, 3, 2, 2> xwzz, rabb;
        Swizzler<Vector4CPP, f32, 0, 3, 2, 3> xwzw, raba;
        Swizzler<Vector4CPP, f32, 0, 3, 3, 0> xwwx, raar;
        Swizzler<Vector4CPP, f32, 0, 3, 3, 1> xwwy, raag;
        Swizzler<Vector4CPP, f32, 0, 3, 3, 2> xwwz, raab;
        Swizzler<Vector4CPP, f32, 0, 3, 3, 3> xwww, raaa;
        Swizzler<Vector4CPP, f32, 1, 0, 0, 0> yxxx, grrr;
        Swizzler<Vector4CPP, f32, 1, 0, 0, 1> yxxy, grrg;
        Swizzler<Vector4CPP, f32, 1, 0, 0, 2> yxxz, grrb;
        Swizzler<Vector4CPP, f32, 1, 0, 0, 3> yxxw, grra;
        Swizzler<Vector4CPP, f32, 1, 0, 1, 0> yxyx, grgr;
        Swizzler<Vector4CPP, f32, 1, 0, 1, 1> yxyy, grgg;
        Swizzler<Vector4CPP, f32, 1, 0, 1, 2> yxyz, grgb;
        Swizzler<Vector4CPP, f32, 1, 0, 1, 3> yxyw, grga;
        Swizzler<Vector4CPP, f32, 1, 0, 2, 0> yxzx, grbr;
        Swizzler<Vector4CPP, f32, 1, 0, 2, 1> yxzy, grbg;
        Swizzler<Vector4CPP, f32, 1, 0, 2, 2> yxzz, grbb;
        Swizzler<Vector4CPP, f32, 1, 0, 2, 3> yxzw, grba;
        Swizzler<Vector4CPP, f32, 1, 0, 3, 0> yxwx, grar;
        Swizzler<Vector4CPP, f32, 1, 0, 3, 1> yxwy, grag;
        Swizzler<Vector4CPP, f32, 1, 0, 3, 2> yxwz, grab;
        Swizzler<Vector4CPP, f32, 1, 0, 3, 3> yxww, graa;
        Swizzler<Vector4CPP, f32, 1, 1, 0, 0> yyxx, ggrr;
        Swizzler<Vector4CPP, f32, 1, 1, 0, 1> yyxy, ggrg;
        Swizzler<Vector4CPP, f32, 1, 1, 0, 2> yyxz, ggrb;
        Swizzler<Vector4CPP, f32, 1, 1, 0, 3> yyxw, ggra;
        Swizzler<Vector4CPP, f32, 1, 1, 1, 0> yyyx, gggr;
        Swizzler<Vector4CPP, f32, 1, 1, 1, 1> yyyy, gggg;
        Swizzler<Vector4CPP, f32, 1, 1, 1, 2> yyyz, gggb;
        Swizzler<Vector4CPP, f32, 1, 1, 1, 3> yyyw, ggga;
        Swizzler<Vector4CPP, f32, 1, 1, 2, 0> yyzx, ggbr;
        Swizzler<Vector4CPP, f32, 1, 1, 2, 1> yyzy, ggbg;
        Swizzler<Vector4CPP, f32, 1, 1, 2, 2> yyzz, ggbb;
        Swizzler<Vector4CPP, f32, 1, 1, 2, 3> yyzw, ggba;
        Swizzler<Vector4CPP, f32, 1, 1, 3, 0> yywx, ggar;
        Swizzler<Vector4CPP, f32, 1, 1, 3, 1> yywy, ggag;
        Swizzler<Vector4CPP, f32, 1, 1, 3, 2> yywz, ggab;
        Swizzler<Vector4CPP, f32, 1, 1, 3, 3> yyww, ggaa;
        Swizzler<Vector4CPP, f32, 1, 2, 0, 0> yzxx, gbrr;
        Swizzler<Vector4CPP, f32, 1, 2, 0, 1> yzxy, gbrg;
        Swizzler<Vector4CPP, f32, 1, 2, 0, 2> yzxz, gbrb;
        Swizzler<Vector4CPP, f32, 1, 2, 0, 3> yzxw, gbra;
        Swizzler<Vector4CPP, f32, 1, 2, 1, 0> yzyx, gbgr;
        Swizzler<Vector4CPP, f32, 1, 2, 1, 1> yzyy, gbgg;
        Swizzler<Vector4CPP, f32, 1, 2, 1, 2> yzyz, gbgb;
        Swizzler<Vector4CPP, f32, 1, 2, 1, 3> yzyw, gbga;
        Swizzler<Vector4CPP, f32, 1, 2, 2, 0> yzzx, gbbr;
        Swizzler<Vector4CPP, f32, 1, 2, 2, 1> yzzy, gbbg;
        Swizzler<Vector4CPP, f32, 1, 2, 2, 2> yzzz, gbbb;
        Swizzler<Vector4CPP, f32, 1, 2, 2, 3> yzzw, gbba;
        Swizzler<Vector4CPP, f32, 1, 2, 3, 0> yzwx, gbar;
        Swizzler<Vector4CPP, f32, 1, 2, 3, 1> yzwy, gbag;
        Swizzler<Vector4CPP, f32, 1, 2, 3, 2> yzwz, gbab;
        Swizzler<Vector4CPP, f32, 1, 2, 3, 3> yzww, gbaa;
        Swizzler<Vector4CPP, f32, 1, 3, 0, 0> ywxx, garr;
        Swizzler<Vector4CPP, f32, 1, 3, 0, 1> ywxy, garg;
        Swizzler<Vector4CPP, f32, 1, 3, 0, 2> ywxz, garb;
        Swizzler<Vector4CPP, f32, 1, 3, 0, 3> ywxw, gara;
        Swizzler<Vector4CPP, f32, 1, 3, 1, 0> ywyx, gagr;
        Swizzler<Vector4CPP, f32, 1, 3, 1, 1> ywyy, gagg;
        Swizzler<Vector4CPP, f32, 1, 3, 1, 2> ywyz, gagb;
        Swizzler<Vector4CPP, f32, 1, 3, 1, 3> ywyw, gaga;
        Swizzler<Vector4CPP, f32, 1, 3, 2, 0> ywzx, gabr;
        Swizzler<Vector4CPP, f32, 1, 3, 2, 1> ywzy, gabg;
        Swizzler<Vector4CPP, f32, 1, 3, 2, 2> ywzz, gabb;
        Swizzler<Vector4CPP, f32, 1, 3, 2, 3> ywzw, gaba;
        Swizzler<Vector4CPP, f32, 1, 3, 3, 0> ywwx, gaar;
        Swizzler<Vector4CPP, f32, 1, 3, 3, 1> ywwy, gaag;
        Swizzler<Vector4CPP, f32, 1, 3, 3, 2> ywwz, gaab;
        Swizzler<Vector4CPP, f32, 1, 3, 3, 3> ywww, gaaa;
        Swizzler<Vector4CPP, f32, 2, 0, 0, 0> zxxx, brrr;
        Swizzler<Vector4CPP, f32, 2, 0, 0, 1> zxxy, brrg;
        Swizzler<Vector4CPP, f32, 2, 0, 0, 2> zxxz, brrb;
        Swizzler<Vector4CPP, f32, 2, 0, 0, 3> zxxw, brra;
        Swizzler<Vector4CPP, f32, 2, 0, 1, 0> zxyx, brgr;
        Swizzler<Vector4CPP, f32, 2, 0, 1, 1> zxyy, brgg;
        Swizzler<Vector4CPP, f32, 2, 0, 1, 2> zxyz, brgb;
        Swizzler<Vector4CPP, f32, 2, 0, 1, 3> zxyw, brga;
        Swizzler<Vector4CPP, f32, 2, 0, 2, 0> zxzx, brbr;
        Swizzler<Vector4CPP, f32, 2, 0, 2, 1> zxzy, brbg;
        Swizzler<Vector4CPP, f32, 2, 0, 2, 2> zxzz, brbb;
        Swizzler<Vector4CPP, f32, 2, 0, 2, 3> zxzw, brba;
        Swizzler<Vector4CPP, f32, 2, 0, 3, 0> zxwx, brar;
        Swizzler<Vector4CPP, f32, 2, 0, 3, 1> zxwy, brag;
        Swizzler<Vector4CPP, f32, 2, 0, 3, 2> zxwz, brab;
        Swizzler<Vector4CPP, f32, 2, 0, 3, 3> zxww, braa;
        Swizzler<Vector4CPP, f32, 2, 1, 0, 0> zyxx, bgrr;
        Swizzler<Vector4CPP, f32, 2, 1, 0, 1> zyxy, bgrg;
        Swizzler<Vector4CPP, f32, 2, 1, 0, 2> zyxz, bgrb;
        Swizzler<Vector4CPP, f32, 2, 1, 0, 3> zyxw, bgra;
        Swizzler<Vector4CPP, f32, 2, 1, 1, 0> zyyx, bggr;
        Swizzler<Vector4CPP, f32, 2, 1, 1, 1> zyyy, bggg;
        Swizzler<Vector4CPP, f32, 2, 1, 1, 2> zyyz, bggb;
        Swizzler<Vector4CPP, f32, 2, 1, 1, 3> zyyw, bgga;
        Swizzler<Vector4CPP, f32, 2, 1, 2, 0> zyzx, bgbr;
        Swizzler<Vector4CPP, f32, 2, 1, 2, 1> zyzy, bgbg;
        Swizzler<Vector4CPP, f32, 2, 1, 2, 2> zyzz, bgbb;
        Swizzler<Vector4CPP, f32, 2, 1, 2, 3> zyzw, bgba;
        Swizzler<Vector4CPP, f32, 2, 1, 3, 0> zywx, bgar;
        Swizzler<Vector4CPP, f32, 2, 1, 3, 1> zywy, bgag;
        Swizzler<Vector4CPP, f32, 2, 1, 3, 2> zywz, bgab;
        Swizzler<Vector4CPP, f32, 2, 1, 3, 3> zyww, bgaa;
        Swizzler<Vector4CPP, f32, 2, 2, 0, 0> zzxx, bbrr;
        Swizzler<Vector4CPP, f32, 2, 2, 0, 1> zzxy, bbrg;
        Swizzler<Vector4CPP, f32, 2, 2, 0, 2> zzxz, bbrb;
        Swizzler<Vector4CPP, f32, 2, 2, 0, 3> zzxw, bbra;
        Swizzler<Vector4CPP, f32, 2, 2, 1, 0> zzyx, bbgr;
        Swizzler<Vector4CPP, f32, 2, 2, 1, 1> zzyy, bbgg;
        Swizzler<Vector4CPP, f32, 2, 2, 1, 2> zzyz, bbgb;
        Swizzler<Vector4CPP, f32, 2, 2, 1, 3> zzyw, bbga;
        Swizzler<Vector4CPP, f32, 2, 2, 2, 0> zzzx, bbbr;
        Swizzler<Vector4CPP, f32, 2, 2, 2, 1> zzzy, bbbg;
        Swizzler<Vector4CPP, f32, 2, 2, 2, 2> zzzz, bbbb;
        Swizzler<Vector4CPP, f32, 2, 2, 2, 3> zzzw, bbba;
        Swizzler<Vector4CPP, f32, 2, 2, 3, 0> zzwx, bbar;
        Swizzler<Vector4CPP, f32, 2, 2, 3, 1> zzwy, bbag;
        Swizzler<Vector4CPP, f32, 2, 2, 3, 2> zzwz, bbab;
        Swizzler<Vector4CPP, f32, 2, 2, 3, 3> zzww, bbaa;
        Swizzler<Vector4CPP, f32, 2, 3, 0, 0> zwxx, barr;
        Swizzler<Vector4CPP, f32, 2, 3, 0, 1> zwxy, barg;
        Swizzler<Vector4CPP, f32, 2, 3, 0, 2> zwxz, barb;
        Swizzler<Vector4CPP, f32, 2, 3, 0, 3> zwxw, bara;
        Swizzler<Vector4CPP, f32, 2, 3, 1, 0> zwyx, bagr;
        Swizzler<Vector4CPP, f32, 2, 3, 1, 1> zwyy, bagg;
        Swizzler<Vector4CPP, f32, 2, 3, 1, 2> zwyz, bagb;
        Swizzler<Vector4CPP, f32, 2, 3, 1, 3> zwyw, baga;
        Swizzler<Vector4CPP, f32, 2, 3, 2, 0> zwzx, babr;
        Swizzler<Vector4CPP, f32, 2, 3, 2, 1> zwzy, babg;
        Swizzler<Vector4CPP, f32, 2, 3, 2, 2> zwzz, babb;
        Swizzler<Vector4CPP, f32, 2, 3, 2, 3> zwzw, baba;
        Swizzler<Vector4CPP, f32, 2, 3, 3, 0> zwwx, baar;
        Swizzler<Vector4CPP, f32, 2, 3, 3, 1> zwwy, baag;
        Swizzler<Vector4CPP, f32, 2, 3, 3, 2> zwwz, baab;
        Swizzler<Vector4CPP, f32, 2, 3, 3, 3> zwww, baaa;
        Swizzler<Vector4CPP, f32, 3, 0, 0, 0> wxxx, arrr;
        Swizzler<Vector4CPP, f32, 3, 0, 0, 1> wxxy, arrg;
        Swizzler<Vector4CPP, f32, 3, 0, 0, 2> wxxz, arrb;
        Swizzler<Vector4CPP, f32, 3, 0, 0, 3> wxxw, arra;
        Swizzler<Vector4CPP, f32, 3, 0, 1, 0> wxyx, argr;
        Swizzler<Vector4CPP, f32, 3, 0, 1, 1> wxyy, argg;
        Swizzler<Vector4CPP, f32, 3, 0, 1, 2> wxyz, argb;
        Swizzler<Vector4CPP, f32, 3, 0, 1, 3> wxyw, arga;
        Swizzler<Vector4CPP, f32, 3, 0, 2, 0> wxzx, arbr;
        Swizzler<Vector4CPP, f32, 3, 0, 2, 1> wxzy, arbg;
        Swizzler<Vector4CPP, f32, 3, 0, 2, 2> wxzz, arbb;
        Swizzler<Vector4CPP, f32, 3, 0, 2, 3> wxzw, arba;
        Swizzler<Vector4CPP, f32, 3, 0, 3, 0> wxwx, arar;
        Swizzler<Vector4CPP, f32, 3, 0, 3, 1> wxwy, arag;
        Swizzler<Vector4CPP, f32, 3, 0, 3, 2> wxwz, arab;
        Swizzler<Vector4CPP, f32, 3, 0, 3, 3> wxww, araa;
        Swizzler<Vector4CPP, f32, 3, 1, 0, 0> wyxx, agrr;
        Swizzler<Vector4CPP, f32, 3, 1, 0, 1> wyxy, agrg;
        Swizzler<Vector4CPP, f32, 3, 1, 0, 2> wyxz, agrb;
        Swizzler<Vector4CPP, f32, 3, 1, 0, 3> wyxw, agra;
        Swizzler<Vector4CPP, f32, 3, 1, 1, 0> wyyx, aggr;
        Swizzler<Vector4CPP, f32, 3, 1, 1, 1> wyyy, aggg;
        Swizzler<Vector4CPP, f32, 3, 1, 1, 2> wyyz, aggb;
        Swizzler<Vector4CPP, f32, 3, 1, 1, 3> wyyw, agga;
        Swizzler<Vector4CPP, f32, 3, 1, 2, 0> wyzx, agbr;
        Swizzler<Vector4CPP, f32, 3, 1, 2, 1> wyzy, agbg;
        Swizzler<Vector4CPP, f32, 3, 1, 2, 2> wyzz, agbb;
        Swizzler<Vector4CPP, f32, 3, 1, 2, 3> wyzw, agba;
        Swizzler<Vector4CPP, f32, 3, 1, 3, 0> wywx, agar;
        Swizzler<Vector4CPP, f32, 3, 1, 3, 1> wywy, agag;
        Swizzler<Vector4CPP, f32, 3, 1, 3, 2> wywz, agab;
        Swizzler<Vector4CPP, f32, 3, 1, 3, 3> wyww, agaa;
        Swizzler<Vector4CPP, f32, 3, 2, 0, 0> wzxx, abrr;
        Swizzler<Vector4CPP, f32, 3, 2, 0, 1> wzxy, abrg;
        Swizzler<Vector4CPP, f32, 3, 2, 0, 2> wzxz, abrb;
        Swizzler<Vector4CPP, f32, 3, 2, 0, 3> wzxw, abra;
        Swizzler<Vector4CPP, f32, 3, 2, 1, 0> wzyx, abgr;
        Swizzler<Vector4CPP, f32, 3, 2, 1, 1> wzyy, abgg;
        Swizzler<Vector4CPP, f32, 3, 2, 1, 2> wzyz, abgb;
        Swizzler<Vector4CPP, f32, 3, 2, 1, 3> wzyw, abga;
        Swizzler<Vector4CPP, f32, 3, 2, 2, 0> wzzx, abbr;
        Swizzler<Vector4CPP, f32, 3, 2, 2, 1> wzzy, abbg;
        Swizzler<Vector4CPP, f32, 3, 2, 2, 2> wzzz, abbb;
        Swizzler<Vector4CPP, f32, 3, 2, 2, 3> wzzw, abba;
        Swizzler<Vector4CPP, f32, 3, 2, 3, 0> wzwx, abar;
        Swizzler<Vector4CPP, f32, 3, 2, 3, 1> wzwy, abag;
        Swizzler<Vector4CPP, f32, 3, 2, 3, 2> wzwz, abab;
        Swizzler<Vector4CPP, f32, 3, 2, 3, 3> wzww, abaa;
        Swizzler<Vector4CPP, f32, 3, 3, 0, 0> wwxx, aarr;
        Swizzler<Vector4CPP, f32, 3, 3, 0, 1> wwxy, aarg;
        Swizzler<Vector4CPP, f32, 3, 3, 0, 2> wwxz, aarb;
        Swizzler<Vector4CPP, f32, 3, 3, 0, 3> wwxw, aara;
        Swizzler<Vector4CPP, f32, 3, 3, 1, 0> wwyx, aagr;
        Swizzler<Vector4CPP, f32, 3, 3, 1, 1> wwyy, aagg;
        Swizzler<Vector4CPP, f32, 3, 3, 1, 2> wwyz, aagb;
        Swizzler<Vector4CPP, f32, 3, 3, 1, 3> wwyw, aaga;
        Swizzler<Vector4CPP, f32, 3, 3, 2, 0> wwzx, aabr;
        Swizzler<Vector4CPP, f32, 3, 3, 2, 1> wwzy, aabg;
        Swizzler<Vector4CPP, f32, 3, 3, 2, 2> wwzz, aabb;
        Swizzler<Vector4CPP, f32, 3, 3, 2, 3> wwzw, aaba;
        Swizzler<Vector4CPP, f32, 3, 3, 3, 0> wwwx, aaar;
        Swizzler<Vector4CPP, f32, 3, 3, 3, 1> wwwy, aaag;
        Swizzler<Vector4CPP, f32, 3, 3, 3, 2> wwwz, aaab;
        Swizzler<Vector4CPP, f32, 3, 3, 3, 3> wwww, aaaa;

        f32 array[4];
    };

    /// @brief Create empty vector.
    constexpr Vector4CPP();
    /// @brief Implicitly convert POD vector to C++ vector.
    constexpr Vector4CPP( const Vector4& __pod );
    /// @brief Create vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @param w W component.
    constexpr explicit Vector4CPP( f32 x, f32 y, f32 z, f32 w );
    /// @brief Create vector.
    /// @param x X, Y, Z and W components.
    constexpr explicit Vector4CPP( f32 x );
    /// @brief Create vector.
    /// @param x  X component.
    /// @param y  Y component.
    /// @param zw Z and W components.
    constexpr explicit Vector4CPP( f32 x, f32 y, Vector2CPP zw );
    /// @brief Create vector.
    /// @param x  X component.
    /// @param yz Y and Z components.
    /// @param w  W component.
    constexpr explicit Vector4CPP( f32 x, Vector2CPP yz, f32 w );
    /// @brief Create vector.
    /// @param xy X and Y components.
    /// @param z  Z component.
    /// @param w  W component.
    constexpr explicit Vector4CPP( Vector2CPP xy, f32 z, f32 w );
    /// @brief Create vector.
    /// @param xy X and Y components.
    /// @param zw Z and W components.
    constexpr explicit Vector4CPP( Vector2CPP xy, Vector2CPP zw );
    /// @brief Create vector.
    /// @param x   X component.
    /// @param yzw Y, Z and W components.
    constexpr explicit Vector4CPP( f32 x, Vector3CPP yzw );
    /// @brief Create vector.
    /// @param xyz X, Y and Z components.
    /// @param w   W component.
    constexpr explicit Vector4CPP( Vector3CPP xyz, f32 w );
    /// @brief Convert integer vector to float vector.
    constexpr explicit Vector4CPP( const IVector4& iv );
    /// @brief Convert boolean vector to float vector.
    constexpr explicit Vector4CPP( const BVector4& bv );

    /// @brief Implicitly convert C++ vector to POD vector.
    constexpr operator Vector4() const;
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
    static constexpr Vector4CPP zero();
    /// @brief Vector one constant.
    /// @return Value.
    static constexpr Vector4CPP one();

    /// @brief Vector color clear constant.
    /// @return Value.
    static constexpr Vector4CPP clear();
    /// @brief Vector color black constant.
    /// @return Value.
    static constexpr Vector4CPP black();
    /// @brief Vector color white constant.
    /// @return Value.
    static constexpr Vector4CPP white();
    /// @brief Vector color red constant.
    /// @return Value.
    static constexpr Vector4CPP red();
    /// @brief Vector color green constant.
    /// @return Value.
    static constexpr Vector4CPP green();
    /// @brief Vector color blue constant.
    /// @return Value.
    static constexpr Vector4CPP blue();
    /// @brief Vector color yellow constant.
    /// @return Value.
    static constexpr Vector4CPP yellow();
    /// @brief Vector color magenta constant.
    /// @return Value.
    static constexpr Vector4CPP magenta();
    /// @brief Vector color cyan constant.
    /// @return Value.
    static constexpr Vector4CPP cyan();
};
/// @brief 4 Component 32-bit Integer Vector.
struct IVector4CPP {
    union {
        /// @brief X, Y, Z and W components.
        struct {
            /// @brief X, Y and Z components.
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
                        /// @brief X and Y components as a #IVector2.
                        IVector2CPP xy;
                    };
                    /// @brief Z component.
                    i32 z;
                };
                /// @brief Swizzle yz.
                struct {
                    i32 __unused0;
                    /// @brief Y and Z components as a #IVector2.
                    IVector2CPP yz;
                };
                /// @brief X, Y and Z components as a #IVector3.
                IVector3CPP xyz;
            };
            /// @brief W component.
            i32 w;
        };
        /// @brief Swizzle yzw.
        struct {
            i32 __unused2;
            /// @brief Y, Z and W components as a #IVector3.
            IVector3CPP yzw;
        };
        /// @brief Swizzle zw.
        struct {
            IVector2CPP __unused4;
            /// @brief Z and W components as a #IVector2.
            IVector2CPP zw;
        };
        /// @brief Swizzle XYZW.
        struct IVector4 xyzw;

        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 0, 0> xx;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 0, 2> xz;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 0, 3> xw;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 1, 0> yx;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 1, 1> yy;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 1, 3> yw;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 2, 0> zx;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 2, 1> zy;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 2, 2> zz;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 3, 0> wx;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 3, 1> wy;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 3, 2> wz;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 3, 3> ww;

        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 0, 0> xxx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 0, 1> xxy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 0, 2> xxz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 0, 3> xxw;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 1, 0> xyx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 1, 1> xyy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 1, 3> xyw;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 2, 0> xzx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 2, 1> xzy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 2, 2> xzz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 2, 3> xzw;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 3, 0> xwx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 3, 1> xwy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 3, 2> xwz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 3, 3> xww;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 0, 0> yxx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 0, 1> yxy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 0, 2> yxz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 0, 3> yxw;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 1, 0> yyx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 1, 1> yyy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 1, 2> yyz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 1, 3> yyw;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 2, 0> yzx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 2, 1> yzy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 2, 2> yzz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 3, 0> ywx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 3, 1> ywy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 3, 2> ywz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 3, 3> yww;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 0, 0> zxx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 0, 1> zxy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 0, 2> zxz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 0, 3> zxw;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 1, 0> zyx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 1, 1> zyy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 1, 2> zyz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 1, 3> zyw;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 2, 0> zzx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 2, 1> zzy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 2, 2> zzz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 2, 3> zzw;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 3, 0> zwx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 3, 1> zwy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 3, 2> zwz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 2, 3, 3> zww;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 0, 0> wxx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 0, 1> wxy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 0, 2> wxz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 0, 3> wxw;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 1, 0> wyx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 1, 1> wyy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 1, 2> wyz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 1, 3> wyw;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 2, 0> wzx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 2, 1> wzy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 2, 2> wzz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 2, 3> wzw;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 3, 0> wwx;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 3, 1> wwy;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 3, 2> wwz;
        SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 3, 3, 3> www;

        Swizzler<IVector4CPP, i32, 0, 0, 0, 0> xxxx;
        Swizzler<IVector4CPP, i32, 0, 0, 0, 1> xxxy;
        Swizzler<IVector4CPP, i32, 0, 0, 0, 2> xxxz;
        Swizzler<IVector4CPP, i32, 0, 0, 0, 3> xxxw;
        Swizzler<IVector4CPP, i32, 0, 0, 1, 0> xxyx;
        Swizzler<IVector4CPP, i32, 0, 0, 1, 1> xxyy;
        Swizzler<IVector4CPP, i32, 0, 0, 1, 2> xxyz;
        Swizzler<IVector4CPP, i32, 0, 0, 1, 3> xxyw;
        Swizzler<IVector4CPP, i32, 0, 0, 2, 0> xxzx;
        Swizzler<IVector4CPP, i32, 0, 0, 2, 1> xxzy;
        Swizzler<IVector4CPP, i32, 0, 0, 2, 2> xxzz;
        Swizzler<IVector4CPP, i32, 0, 0, 2, 3> xxzw;
        Swizzler<IVector4CPP, i32, 0, 0, 3, 0> xxwx;
        Swizzler<IVector4CPP, i32, 0, 0, 3, 1> xxwy;
        Swizzler<IVector4CPP, i32, 0, 0, 3, 2> xxwz;
        Swizzler<IVector4CPP, i32, 0, 0, 3, 3> xxww;
        Swizzler<IVector4CPP, i32, 0, 1, 0, 0> xyxx;
        Swizzler<IVector4CPP, i32, 0, 1, 0, 1> xyxy;
        Swizzler<IVector4CPP, i32, 0, 1, 0, 2> xyxz;
        Swizzler<IVector4CPP, i32, 0, 1, 0, 3> xyxw;
        Swizzler<IVector4CPP, i32, 0, 1, 1, 0> xyyx;
        Swizzler<IVector4CPP, i32, 0, 1, 1, 1> xyyy;
        Swizzler<IVector4CPP, i32, 0, 1, 1, 0> xyyz;
        Swizzler<IVector4CPP, i32, 0, 1, 1, 3> xyyw;
        Swizzler<IVector4CPP, i32, 0, 1, 2, 0> xyzx;
        Swizzler<IVector4CPP, i32, 0, 1, 2, 1> xyzy;
        Swizzler<IVector4CPP, i32, 0, 1, 2, 2> xyzz;
        Swizzler<IVector4CPP, i32, 0, 1, 3, 0> xywx;
        Swizzler<IVector4CPP, i32, 0, 1, 3, 1> xywy;
        Swizzler<IVector4CPP, i32, 0, 1, 3, 2> xywz;
        Swizzler<IVector4CPP, i32, 0, 1, 3, 3> xyww;
        Swizzler<IVector4CPP, i32, 0, 2, 0, 0> xzxx;
        Swizzler<IVector4CPP, i32, 0, 2, 0, 1> xzxy;
        Swizzler<IVector4CPP, i32, 0, 2, 0, 2> xzxz;
        Swizzler<IVector4CPP, i32, 0, 2, 0, 3> xzxw;
        Swizzler<IVector4CPP, i32, 0, 2, 1, 0> xzyx;
        Swizzler<IVector4CPP, i32, 0, 2, 1, 1> xzyy;
        Swizzler<IVector4CPP, i32, 0, 2, 1, 2> xzyz;
        Swizzler<IVector4CPP, i32, 0, 2, 1, 3> xzyw;
        Swizzler<IVector4CPP, i32, 0, 2, 2, 0> xzzx;
        Swizzler<IVector4CPP, i32, 0, 2, 2, 1> xzzy;
        Swizzler<IVector4CPP, i32, 0, 2, 2, 2> xzzz;
        Swizzler<IVector4CPP, i32, 0, 2, 2, 3> xzzw;
        Swizzler<IVector4CPP, i32, 0, 2, 3, 0> xzwx;
        Swizzler<IVector4CPP, i32, 0, 2, 3, 1> xzwy;
        Swizzler<IVector4CPP, i32, 0, 2, 3, 2> xzwz;
        Swizzler<IVector4CPP, i32, 0, 2, 3, 3> xzww;
        Swizzler<IVector4CPP, i32, 0, 3, 0, 0> xwxx;
        Swizzler<IVector4CPP, i32, 0, 3, 0, 1> xwxy;
        Swizzler<IVector4CPP, i32, 0, 3, 0, 2> xwxz;
        Swizzler<IVector4CPP, i32, 0, 3, 0, 3> xwxw;
        Swizzler<IVector4CPP, i32, 0, 3, 1, 0> xwyx;
        Swizzler<IVector4CPP, i32, 0, 3, 1, 1> xwyy;
        Swizzler<IVector4CPP, i32, 0, 3, 1, 2> xwyz;
        Swizzler<IVector4CPP, i32, 0, 3, 1, 3> xwyw;
        Swizzler<IVector4CPP, i32, 0, 3, 0, 3> xwzx;
        Swizzler<IVector4CPP, i32, 0, 3, 2, 1> xwzy;
        Swizzler<IVector4CPP, i32, 0, 3, 2, 2> xwzz;
        Swizzler<IVector4CPP, i32, 0, 3, 2, 3> xwzw;
        Swizzler<IVector4CPP, i32, 0, 3, 3, 0> xwwx;
        Swizzler<IVector4CPP, i32, 0, 3, 3, 1> xwwy;
        Swizzler<IVector4CPP, i32, 0, 3, 3, 2> xwwz;
        Swizzler<IVector4CPP, i32, 0, 3, 3, 3> xwww;
        Swizzler<IVector4CPP, i32, 1, 0, 0, 0> yxxx;
        Swizzler<IVector4CPP, i32, 1, 0, 0, 1> yxxy;
        Swizzler<IVector4CPP, i32, 1, 0, 0, 2> yxxz;
        Swizzler<IVector4CPP, i32, 1, 0, 0, 3> yxxw;
        Swizzler<IVector4CPP, i32, 1, 0, 1, 0> yxyx;
        Swizzler<IVector4CPP, i32, 1, 0, 1, 1> yxyy;
        Swizzler<IVector4CPP, i32, 1, 0, 1, 2> yxyz;
        Swizzler<IVector4CPP, i32, 1, 0, 1, 3> yxyw;
        Swizzler<IVector4CPP, i32, 1, 0, 2, 0> yxzx;
        Swizzler<IVector4CPP, i32, 1, 0, 2, 1> yxzy;
        Swizzler<IVector4CPP, i32, 1, 0, 2, 2> yxzz;
        Swizzler<IVector4CPP, i32, 1, 0, 2, 3> yxzw;
        Swizzler<IVector4CPP, i32, 1, 0, 3, 0> yxwx;
        Swizzler<IVector4CPP, i32, 1, 0, 3, 1> yxwy;
        Swizzler<IVector4CPP, i32, 1, 0, 3, 2> yxwz;
        Swizzler<IVector4CPP, i32, 1, 0, 3, 3> yxww;
        Swizzler<IVector4CPP, i32, 1, 1, 0, 0> yyxx;
        Swizzler<IVector4CPP, i32, 1, 1, 0, 1> yyxy;
        Swizzler<IVector4CPP, i32, 1, 1, 0, 2> yyxz;
        Swizzler<IVector4CPP, i32, 1, 1, 0, 3> yyxw;
        Swizzler<IVector4CPP, i32, 1, 1, 1, 0> yyyx;
        Swizzler<IVector4CPP, i32, 1, 1, 1, 1> yyyy;
        Swizzler<IVector4CPP, i32, 1, 1, 1, 2> yyyz;
        Swizzler<IVector4CPP, i32, 1, 1, 1, 3> yyyw;
        Swizzler<IVector4CPP, i32, 1, 1, 2, 0> yyzx;
        Swizzler<IVector4CPP, i32, 1, 1, 2, 1> yyzy;
        Swizzler<IVector4CPP, i32, 1, 1, 2, 2> yyzz;
        Swizzler<IVector4CPP, i32, 1, 1, 2, 3> yyzw;
        Swizzler<IVector4CPP, i32, 1, 1, 3, 0> yywx;
        Swizzler<IVector4CPP, i32, 1, 1, 3, 1> yywy;
        Swizzler<IVector4CPP, i32, 1, 1, 3, 2> yywz;
        Swizzler<IVector4CPP, i32, 1, 1, 3, 3> yyww;
        Swizzler<IVector4CPP, i32, 1, 2, 0, 0> yzxx;
        Swizzler<IVector4CPP, i32, 1, 2, 0, 1> yzxy;
        Swizzler<IVector4CPP, i32, 1, 2, 0, 2> yzxz;
        Swizzler<IVector4CPP, i32, 1, 2, 0, 3> yzxw;
        Swizzler<IVector4CPP, i32, 1, 2, 1, 0> yzyx;
        Swizzler<IVector4CPP, i32, 1, 2, 1, 1> yzyy;
        Swizzler<IVector4CPP, i32, 1, 2, 1, 2> yzyz;
        Swizzler<IVector4CPP, i32, 1, 2, 1, 3> yzyw;
        Swizzler<IVector4CPP, i32, 1, 2, 2, 0> yzzx;
        Swizzler<IVector4CPP, i32, 1, 2, 2, 1> yzzy;
        Swizzler<IVector4CPP, i32, 1, 2, 2, 2> yzzz;
        Swizzler<IVector4CPP, i32, 1, 2, 2, 3> yzzw;
        Swizzler<IVector4CPP, i32, 1, 2, 3, 0> yzwx;
        Swizzler<IVector4CPP, i32, 1, 2, 3, 1> yzwy;
        Swizzler<IVector4CPP, i32, 1, 2, 3, 2> yzwz;
        Swizzler<IVector4CPP, i32, 1, 2, 3, 3> yzww;
        Swizzler<IVector4CPP, i32, 1, 3, 0, 0> ywxx;
        Swizzler<IVector4CPP, i32, 1, 3, 0, 1> ywxy;
        Swizzler<IVector4CPP, i32, 1, 3, 0, 2> ywxz;
        Swizzler<IVector4CPP, i32, 1, 3, 0, 3> ywxw;
        Swizzler<IVector4CPP, i32, 1, 3, 1, 0> ywyx;
        Swizzler<IVector4CPP, i32, 1, 3, 1, 1> ywyy;
        Swizzler<IVector4CPP, i32, 1, 3, 1, 2> ywyz;
        Swizzler<IVector4CPP, i32, 1, 3, 1, 3> ywyw;
        Swizzler<IVector4CPP, i32, 1, 3, 2, 0> ywzx;
        Swizzler<IVector4CPP, i32, 1, 3, 2, 1> ywzy;
        Swizzler<IVector4CPP, i32, 1, 3, 2, 2> ywzz;
        Swizzler<IVector4CPP, i32, 1, 3, 2, 3> ywzw;
        Swizzler<IVector4CPP, i32, 1, 3, 3, 0> ywwx;
        Swizzler<IVector4CPP, i32, 1, 3, 3, 1> ywwy;
        Swizzler<IVector4CPP, i32, 1, 3, 3, 2> ywwz;
        Swizzler<IVector4CPP, i32, 1, 3, 3, 3> ywww;
        Swizzler<IVector4CPP, i32, 2, 0, 0, 0> zxxx;
        Swizzler<IVector4CPP, i32, 2, 0, 0, 1> zxxy;
        Swizzler<IVector4CPP, i32, 2, 0, 0, 2> zxxz;
        Swizzler<IVector4CPP, i32, 2, 0, 0, 3> zxxw;
        Swizzler<IVector4CPP, i32, 2, 0, 1, 0> zxyx;
        Swizzler<IVector4CPP, i32, 2, 0, 1, 1> zxyy;
        Swizzler<IVector4CPP, i32, 2, 0, 1, 2> zxyz;
        Swizzler<IVector4CPP, i32, 2, 0, 1, 3> zxyw;
        Swizzler<IVector4CPP, i32, 2, 0, 2, 0> zxzx;
        Swizzler<IVector4CPP, i32, 2, 0, 2, 1> zxzy;
        Swizzler<IVector4CPP, i32, 2, 0, 2, 2> zxzz;
        Swizzler<IVector4CPP, i32, 2, 0, 2, 3> zxzw;
        Swizzler<IVector4CPP, i32, 2, 0, 3, 0> zxwx;
        Swizzler<IVector4CPP, i32, 2, 0, 3, 1> zxwy;
        Swizzler<IVector4CPP, i32, 2, 0, 3, 2> zxwz;
        Swizzler<IVector4CPP, i32, 2, 0, 3, 3> zxww;
        Swizzler<IVector4CPP, i32, 2, 1, 0, 0> zyxx;
        Swizzler<IVector4CPP, i32, 2, 1, 0, 1> zyxy;
        Swizzler<IVector4CPP, i32, 2, 1, 0, 2> zyxz;
        Swizzler<IVector4CPP, i32, 2, 1, 0, 3> zyxw;
        Swizzler<IVector4CPP, i32, 2, 1, 1, 0> zyyx;
        Swizzler<IVector4CPP, i32, 2, 1, 1, 1> zyyy;
        Swizzler<IVector4CPP, i32, 2, 1, 1, 2> zyyz;
        Swizzler<IVector4CPP, i32, 2, 1, 1, 3> zyyw;
        Swizzler<IVector4CPP, i32, 2, 1, 2, 0> zyzx;
        Swizzler<IVector4CPP, i32, 2, 1, 2, 1> zyzy;
        Swizzler<IVector4CPP, i32, 2, 1, 2, 2> zyzz;
        Swizzler<IVector4CPP, i32, 2, 1, 2, 3> zyzw;
        Swizzler<IVector4CPP, i32, 2, 1, 3, 0> zywx;
        Swizzler<IVector4CPP, i32, 2, 1, 3, 1> zywy;
        Swizzler<IVector4CPP, i32, 2, 1, 3, 2> zywz;
        Swizzler<IVector4CPP, i32, 2, 1, 3, 3> zyww;
        Swizzler<IVector4CPP, i32, 2, 2, 0, 0> zzxx;
        Swizzler<IVector4CPP, i32, 2, 2, 0, 1> zzxy;
        Swizzler<IVector4CPP, i32, 2, 2, 0, 2> zzxz;
        Swizzler<IVector4CPP, i32, 2, 2, 0, 3> zzxw;
        Swizzler<IVector4CPP, i32, 2, 2, 1, 0> zzyx;
        Swizzler<IVector4CPP, i32, 2, 2, 1, 1> zzyy;
        Swizzler<IVector4CPP, i32, 2, 2, 1, 2> zzyz;
        Swizzler<IVector4CPP, i32, 2, 2, 1, 3> zzyw;
        Swizzler<IVector4CPP, i32, 2, 2, 2, 0> zzzx;
        Swizzler<IVector4CPP, i32, 2, 2, 2, 1> zzzy;
        Swizzler<IVector4CPP, i32, 2, 2, 2, 2> zzzz;
        Swizzler<IVector4CPP, i32, 2, 2, 2, 3> zzzw;
        Swizzler<IVector4CPP, i32, 2, 2, 3, 0> zzwx;
        Swizzler<IVector4CPP, i32, 2, 2, 3, 1> zzwy;
        Swizzler<IVector4CPP, i32, 2, 2, 3, 2> zzwz;
        Swizzler<IVector4CPP, i32, 2, 2, 3, 3> zzww;
        Swizzler<IVector4CPP, i32, 2, 3, 0, 0> zwxx;
        Swizzler<IVector4CPP, i32, 2, 3, 0, 1> zwxy;
        Swizzler<IVector4CPP, i32, 2, 3, 0, 2> zwxz;
        Swizzler<IVector4CPP, i32, 2, 3, 0, 3> zwxw;
        Swizzler<IVector4CPP, i32, 2, 3, 1, 0> zwyx;
        Swizzler<IVector4CPP, i32, 2, 3, 1, 1> zwyy;
        Swizzler<IVector4CPP, i32, 2, 3, 1, 2> zwyz;
        Swizzler<IVector4CPP, i32, 2, 3, 1, 3> zwyw;
        Swizzler<IVector4CPP, i32, 2, 3, 2, 0> zwzx;
        Swizzler<IVector4CPP, i32, 2, 3, 2, 1> zwzy;
        Swizzler<IVector4CPP, i32, 2, 3, 2, 2> zwzz;
        Swizzler<IVector4CPP, i32, 2, 3, 2, 3> zwzw;
        Swizzler<IVector4CPP, i32, 2, 3, 3, 0> zwwx;
        Swizzler<IVector4CPP, i32, 2, 3, 3, 1> zwwy;
        Swizzler<IVector4CPP, i32, 2, 3, 3, 2> zwwz;
        Swizzler<IVector4CPP, i32, 2, 3, 3, 3> zwww;
        Swizzler<IVector4CPP, i32, 3, 0, 0, 0> wxxx;
        Swizzler<IVector4CPP, i32, 3, 0, 0, 1> wxxy;
        Swizzler<IVector4CPP, i32, 3, 0, 0, 2> wxxz;
        Swizzler<IVector4CPP, i32, 3, 0, 0, 3> wxxw;
        Swizzler<IVector4CPP, i32, 3, 0, 1, 0> wxyx;
        Swizzler<IVector4CPP, i32, 3, 0, 1, 1> wxyy;
        Swizzler<IVector4CPP, i32, 3, 0, 1, 2> wxyz;
        Swizzler<IVector4CPP, i32, 3, 0, 1, 3> wxyw;
        Swizzler<IVector4CPP, i32, 3, 0, 2, 0> wxzx;
        Swizzler<IVector4CPP, i32, 3, 0, 2, 1> wxzy;
        Swizzler<IVector4CPP, i32, 3, 0, 2, 2> wxzz;
        Swizzler<IVector4CPP, i32, 3, 0, 2, 3> wxzw;
        Swizzler<IVector4CPP, i32, 3, 0, 3, 0> wxwx;
        Swizzler<IVector4CPP, i32, 3, 0, 3, 1> wxwy;
        Swizzler<IVector4CPP, i32, 3, 0, 3, 2> wxwz;
        Swizzler<IVector4CPP, i32, 3, 0, 3, 3> wxww;
        Swizzler<IVector4CPP, i32, 3, 1, 0, 0> wyxx;
        Swizzler<IVector4CPP, i32, 3, 1, 0, 1> wyxy;
        Swizzler<IVector4CPP, i32, 3, 1, 0, 2> wyxz;
        Swizzler<IVector4CPP, i32, 3, 1, 0, 3> wyxw;
        Swizzler<IVector4CPP, i32, 3, 1, 1, 0> wyyx;
        Swizzler<IVector4CPP, i32, 3, 1, 1, 1> wyyy;
        Swizzler<IVector4CPP, i32, 3, 1, 1, 2> wyyz;
        Swizzler<IVector4CPP, i32, 3, 1, 1, 3> wyyw;
        Swizzler<IVector4CPP, i32, 3, 1, 2, 0> wyzx;
        Swizzler<IVector4CPP, i32, 3, 1, 2, 1> wyzy;
        Swizzler<IVector4CPP, i32, 3, 1, 2, 2> wyzz;
        Swizzler<IVector4CPP, i32, 3, 1, 2, 3> wyzw;
        Swizzler<IVector4CPP, i32, 3, 1, 3, 0> wywx;
        Swizzler<IVector4CPP, i32, 3, 1, 3, 1> wywy;
        Swizzler<IVector4CPP, i32, 3, 1, 3, 2> wywz;
        Swizzler<IVector4CPP, i32, 3, 1, 3, 3> wyww;
        Swizzler<IVector4CPP, i32, 3, 2, 0, 0> wzxx;
        Swizzler<IVector4CPP, i32, 3, 2, 0, 1> wzxy;
        Swizzler<IVector4CPP, i32, 3, 2, 0, 2> wzxz;
        Swizzler<IVector4CPP, i32, 3, 2, 0, 3> wzxw;
        Swizzler<IVector4CPP, i32, 3, 2, 1, 0> wzyx;
        Swizzler<IVector4CPP, i32, 3, 2, 1, 1> wzyy;
        Swizzler<IVector4CPP, i32, 3, 2, 1, 2> wzyz;
        Swizzler<IVector4CPP, i32, 3, 2, 1, 3> wzyw;
        Swizzler<IVector4CPP, i32, 3, 2, 2, 0> wzzx;
        Swizzler<IVector4CPP, i32, 3, 2, 2, 1> wzzy;
        Swizzler<IVector4CPP, i32, 3, 2, 2, 2> wzzz;
        Swizzler<IVector4CPP, i32, 3, 2, 2, 3> wzzw;
        Swizzler<IVector4CPP, i32, 3, 2, 3, 0> wzwx;
        Swizzler<IVector4CPP, i32, 3, 2, 3, 1> wzwy;
        Swizzler<IVector4CPP, i32, 3, 2, 3, 2> wzwz;
        Swizzler<IVector4CPP, i32, 3, 2, 3, 3> wzww;
        Swizzler<IVector4CPP, i32, 3, 3, 0, 0> wwxx;
        Swizzler<IVector4CPP, i32, 3, 3, 0, 1> wwxy;
        Swizzler<IVector4CPP, i32, 3, 3, 0, 2> wwxz;
        Swizzler<IVector4CPP, i32, 3, 3, 0, 3> wwxw;
        Swizzler<IVector4CPP, i32, 3, 3, 1, 0> wwyx;
        Swizzler<IVector4CPP, i32, 3, 3, 1, 1> wwyy;
        Swizzler<IVector4CPP, i32, 3, 3, 1, 2> wwyz;
        Swizzler<IVector4CPP, i32, 3, 3, 1, 3> wwyw;
        Swizzler<IVector4CPP, i32, 3, 3, 2, 0> wwzx;
        Swizzler<IVector4CPP, i32, 3, 3, 2, 1> wwzy;
        Swizzler<IVector4CPP, i32, 3, 3, 2, 2> wwzz;
        Swizzler<IVector4CPP, i32, 3, 3, 2, 3> wwzw;
        Swizzler<IVector4CPP, i32, 3, 3, 3, 0> wwwx;
        Swizzler<IVector4CPP, i32, 3, 3, 3, 1> wwwy;
        Swizzler<IVector4CPP, i32, 3, 3, 3, 2> wwwz;
        Swizzler<IVector4CPP, i32, 3, 3, 3, 3> wwww;

        i32 array[4];
    };

    /// @brief Create empty vector.
    constexpr IVector4CPP();
    /// @brief Implicitly convert POD vector to C++ vector.
    constexpr IVector4CPP( const IVector4& __pod );
    /// @brief Create vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @param w W component.
    constexpr explicit IVector4CPP( i32 x, i32 y, i32 z, i32 w );
    /// @brief Create vector.
    /// @param x X, Y, Z and W components.
    constexpr explicit IVector4CPP( i32 x );
    /// @brief Create vector.
    /// @param x  X component.
    /// @param y  Y component.
    /// @param zw Z and W components.
    constexpr explicit IVector4CPP( i32 x, i32 y, IVector2CPP zw );
    /// @brief Create vector.
    /// @param x  X component.
    /// @param yz Y and Z components.
    /// @param w  W component.
    constexpr explicit IVector4CPP( i32 x, IVector2CPP yz, i32 w );
    /// @brief Create vector.
    /// @param xy X and Y components.
    /// @param z  Z component.
    /// @param w  W component.
    constexpr explicit IVector4CPP( IVector2CPP xy, i32 z, i32 w );
    /// @brief Create vector.
    /// @param xy X and Y components.
    /// @param zw Z and W components.
    constexpr explicit IVector4CPP( IVector2CPP xy, IVector2CPP zw );
    /// @brief Create vector.
    /// @param x   X component.
    /// @param yzw Y, Z and W components.
    constexpr explicit IVector4CPP( i32 x, IVector3CPP yzw );
    /// @brief Create vector.
    /// @param xyz X, Y and Z components.
    /// @param w   W component.
    constexpr explicit IVector4CPP( IVector3CPP xyz, i32 w );
    /// @brief Convert float vector to integer vector.
    constexpr explicit IVector4CPP( const Vector4& iv );
    /// @brief Convert boolean vector to float vector.
    constexpr explicit IVector4CPP( const BVector4& bv );

    /// @brief Implicitly convert C++ vector to POD vector.
    constexpr operator IVector4() const;
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
    static constexpr IVector4CPP zero();
    /// @brief Vector one constant.
    /// @return Value.
    static constexpr IVector4CPP one();
};
/// @brief 4 Component 32-bit Boolean Vector.
struct BVector4CPP {
    union {
        /// @brief X, Y, Z and W components.
        struct {
            /// @brief X, Y and Z components.
            union {
                /// @brief X, Y and Z components.
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
                        /// @brief X and Y components as a #BVector2.
                        BVector2CPP xy;
                    };
                    /// @brief Z component.
                    b32 z;
                };
                /// @brief Swizzle yz.
                struct {
                    b32 __unused0;
                    /// @brief Y and Z components as a #BVector2.
                    BVector2CPP yz;
                };
                /// @brief X, Y and Z components as a #BVector3.
                BVector3CPP xyz;
            };
            /// @brief W component.
            b32 w;
        };
        /// @brief Swizzle yzw.
        struct {
            b32 __unused2;
            /// @brief Y, Z and W components as a #BVector3.
            BVector3CPP yzw;
        };
        /// @brief Swizzle zw.
        struct {
            BVector2CPP __unused4;
            /// @brief Z and W components as a #BVector2.
            BVector2CPP zw;
        };
        /// @brief Swizzle XYZW.
        struct BVector4 xyzw;

        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 0, 0> xx;
        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 0, 2> xz;
        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 0, 3> xw;
        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 1, 0> yx;
        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 1, 1> yy;
        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 1, 3> yw;
        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 2, 0> zx;
        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 2, 1> zy;
        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 2, 2> zz;
        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 3, 0> wx;
        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 3, 1> wy;
        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 3, 2> wz;
        SwizzlerConvert<BVector4CPP, BVector2CPP, b32, 3, 3> ww;

        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 0, 0> xxx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 0, 1> xxy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 0, 2> xxz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 0, 3> xxw;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 1, 0> xyx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 1, 1> xyy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 1, 3> xyw;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 2, 0> xzx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 2, 1> xzy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 2, 2> xzz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 2, 3> xzw;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 3, 0> xwx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 3, 1> xwy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 3, 2> xwz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 0, 3, 3> xww;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 0, 0> yxx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 0, 1> yxy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 0, 2> yxz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 0, 3> yxw;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 1, 0> yyx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 1, 1> yyy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 1, 2> yyz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 1, 3> yyw;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 2, 0> yzx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 2, 1> yzy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 2, 2> yzz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 3, 0> ywx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 3, 1> ywy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 3, 2> ywz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 1, 3, 3> yww;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 0, 0> zxx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 0, 1> zxy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 0, 2> zxz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 0, 3> zxw;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 1, 0> zyx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 1, 1> zyy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 1, 2> zyz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 1, 3> zyw;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 2, 0> zzx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 2, 1> zzy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 2, 2> zzz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 2, 3> zzw;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 3, 0> zwx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 3, 1> zwy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 3, 2> zwz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 2, 3, 3> zww;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 0, 0> wxx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 0, 1> wxy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 0, 2> wxz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 0, 3> wxw;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 1, 0> wyx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 1, 1> wyy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 1, 2> wyz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 1, 3> wyw;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 2, 0> wzx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 2, 1> wzy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 2, 2> wzz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 2, 3> wzw;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 3, 0> wwx;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 3, 1> wwy;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 3, 2> wwz;
        SwizzlerConvert<BVector4CPP, BVector3CPP, b32, 3, 3, 3> www;

        Swizzler<BVector4CPP, b32, 0, 0, 0, 0> xxxx;
        Swizzler<BVector4CPP, b32, 0, 0, 0, 1> xxxy;
        Swizzler<BVector4CPP, b32, 0, 0, 0, 2> xxxz;
        Swizzler<BVector4CPP, b32, 0, 0, 0, 3> xxxw;
        Swizzler<BVector4CPP, b32, 0, 0, 1, 0> xxyx;
        Swizzler<BVector4CPP, b32, 0, 0, 1, 1> xxyy;
        Swizzler<BVector4CPP, b32, 0, 0, 1, 2> xxyz;
        Swizzler<BVector4CPP, b32, 0, 0, 1, 3> xxyw;
        Swizzler<BVector4CPP, b32, 0, 0, 2, 0> xxzx;
        Swizzler<BVector4CPP, b32, 0, 0, 2, 1> xxzy;
        Swizzler<BVector4CPP, b32, 0, 0, 2, 2> xxzz;
        Swizzler<BVector4CPP, b32, 0, 0, 2, 3> xxzw;
        Swizzler<BVector4CPP, b32, 0, 0, 3, 0> xxwx;
        Swizzler<BVector4CPP, b32, 0, 0, 3, 1> xxwy;
        Swizzler<BVector4CPP, b32, 0, 0, 3, 2> xxwz;
        Swizzler<BVector4CPP, b32, 0, 0, 3, 3> xxww;
        Swizzler<BVector4CPP, b32, 0, 1, 0, 0> xyxx;
        Swizzler<BVector4CPP, b32, 0, 1, 0, 1> xyxy;
        Swizzler<BVector4CPP, b32, 0, 1, 0, 2> xyxz;
        Swizzler<BVector4CPP, b32, 0, 1, 0, 3> xyxw;
        Swizzler<BVector4CPP, b32, 0, 1, 1, 0> xyyx;
        Swizzler<BVector4CPP, b32, 0, 1, 1, 1> xyyy;
        Swizzler<BVector4CPP, b32, 0, 1, 1, 0> xyyz;
        Swizzler<BVector4CPP, b32, 0, 1, 1, 3> xyyw;
        Swizzler<BVector4CPP, b32, 0, 1, 2, 0> xyzx;
        Swizzler<BVector4CPP, b32, 0, 1, 2, 1> xyzy;
        Swizzler<BVector4CPP, b32, 0, 1, 2, 2> xyzz;
        Swizzler<BVector4CPP, b32, 0, 1, 3, 0> xywx;
        Swizzler<BVector4CPP, b32, 0, 1, 3, 1> xywy;
        Swizzler<BVector4CPP, b32, 0, 1, 3, 2> xywz;
        Swizzler<BVector4CPP, b32, 0, 1, 3, 3> xyww;
        Swizzler<BVector4CPP, b32, 0, 2, 0, 0> xzxx;
        Swizzler<BVector4CPP, b32, 0, 2, 0, 1> xzxy;
        Swizzler<BVector4CPP, b32, 0, 2, 0, 2> xzxz;
        Swizzler<BVector4CPP, b32, 0, 2, 0, 3> xzxw;
        Swizzler<BVector4CPP, b32, 0, 2, 1, 0> xzyx;
        Swizzler<BVector4CPP, b32, 0, 2, 1, 1> xzyy;
        Swizzler<BVector4CPP, b32, 0, 2, 1, 2> xzyz;
        Swizzler<BVector4CPP, b32, 0, 2, 1, 3> xzyw;
        Swizzler<BVector4CPP, b32, 0, 2, 2, 0> xzzx;
        Swizzler<BVector4CPP, b32, 0, 2, 2, 1> xzzy;
        Swizzler<BVector4CPP, b32, 0, 2, 2, 2> xzzz;
        Swizzler<BVector4CPP, b32, 0, 2, 2, 3> xzzw;
        Swizzler<BVector4CPP, b32, 0, 2, 3, 0> xzwx;
        Swizzler<BVector4CPP, b32, 0, 2, 3, 1> xzwy;
        Swizzler<BVector4CPP, b32, 0, 2, 3, 2> xzwz;
        Swizzler<BVector4CPP, b32, 0, 2, 3, 3> xzww;
        Swizzler<BVector4CPP, b32, 0, 3, 0, 0> xwxx;
        Swizzler<BVector4CPP, b32, 0, 3, 0, 1> xwxy;
        Swizzler<BVector4CPP, b32, 0, 3, 0, 2> xwxz;
        Swizzler<BVector4CPP, b32, 0, 3, 0, 3> xwxw;
        Swizzler<BVector4CPP, b32, 0, 3, 1, 0> xwyx;
        Swizzler<BVector4CPP, b32, 0, 3, 1, 1> xwyy;
        Swizzler<BVector4CPP, b32, 0, 3, 1, 2> xwyz;
        Swizzler<BVector4CPP, b32, 0, 3, 1, 3> xwyw;
        Swizzler<BVector4CPP, b32, 0, 3, 0, 3> xwzx;
        Swizzler<BVector4CPP, b32, 0, 3, 2, 1> xwzy;
        Swizzler<BVector4CPP, b32, 0, 3, 2, 2> xwzz;
        Swizzler<BVector4CPP, b32, 0, 3, 2, 3> xwzw;
        Swizzler<BVector4CPP, b32, 0, 3, 3, 0> xwwx;
        Swizzler<BVector4CPP, b32, 0, 3, 3, 1> xwwy;
        Swizzler<BVector4CPP, b32, 0, 3, 3, 2> xwwz;
        Swizzler<BVector4CPP, b32, 0, 3, 3, 3> xwww;
        Swizzler<BVector4CPP, b32, 1, 0, 0, 0> yxxx;
        Swizzler<BVector4CPP, b32, 1, 0, 0, 1> yxxy;
        Swizzler<BVector4CPP, b32, 1, 0, 0, 2> yxxz;
        Swizzler<BVector4CPP, b32, 1, 0, 0, 3> yxxw;
        Swizzler<BVector4CPP, b32, 1, 0, 1, 0> yxyx;
        Swizzler<BVector4CPP, b32, 1, 0, 1, 1> yxyy;
        Swizzler<BVector4CPP, b32, 1, 0, 1, 2> yxyz;
        Swizzler<BVector4CPP, b32, 1, 0, 1, 3> yxyw;
        Swizzler<BVector4CPP, b32, 1, 0, 2, 0> yxzx;
        Swizzler<BVector4CPP, b32, 1, 0, 2, 1> yxzy;
        Swizzler<BVector4CPP, b32, 1, 0, 2, 2> yxzz;
        Swizzler<BVector4CPP, b32, 1, 0, 2, 3> yxzw;
        Swizzler<BVector4CPP, b32, 1, 0, 3, 0> yxwx;
        Swizzler<BVector4CPP, b32, 1, 0, 3, 1> yxwy;
        Swizzler<BVector4CPP, b32, 1, 0, 3, 2> yxwz;
        Swizzler<BVector4CPP, b32, 1, 0, 3, 3> yxww;
        Swizzler<BVector4CPP, b32, 1, 1, 0, 0> yyxx;
        Swizzler<BVector4CPP, b32, 1, 1, 0, 1> yyxy;
        Swizzler<BVector4CPP, b32, 1, 1, 0, 2> yyxz;
        Swizzler<BVector4CPP, b32, 1, 1, 0, 3> yyxw;
        Swizzler<BVector4CPP, b32, 1, 1, 1, 0> yyyx;
        Swizzler<BVector4CPP, b32, 1, 1, 1, 1> yyyy;
        Swizzler<BVector4CPP, b32, 1, 1, 1, 2> yyyz;
        Swizzler<BVector4CPP, b32, 1, 1, 1, 3> yyyw;
        Swizzler<BVector4CPP, b32, 1, 1, 2, 0> yyzx;
        Swizzler<BVector4CPP, b32, 1, 1, 2, 1> yyzy;
        Swizzler<BVector4CPP, b32, 1, 1, 2, 2> yyzz;
        Swizzler<BVector4CPP, b32, 1, 1, 2, 3> yyzw;
        Swizzler<BVector4CPP, b32, 1, 1, 3, 0> yywx;
        Swizzler<BVector4CPP, b32, 1, 1, 3, 1> yywy;
        Swizzler<BVector4CPP, b32, 1, 1, 3, 2> yywz;
        Swizzler<BVector4CPP, b32, 1, 1, 3, 3> yyww;
        Swizzler<BVector4CPP, b32, 1, 2, 0, 0> yzxx;
        Swizzler<BVector4CPP, b32, 1, 2, 0, 1> yzxy;
        Swizzler<BVector4CPP, b32, 1, 2, 0, 2> yzxz;
        Swizzler<BVector4CPP, b32, 1, 2, 0, 3> yzxw;
        Swizzler<BVector4CPP, b32, 1, 2, 1, 0> yzyx;
        Swizzler<BVector4CPP, b32, 1, 2, 1, 1> yzyy;
        Swizzler<BVector4CPP, b32, 1, 2, 1, 2> yzyz;
        Swizzler<BVector4CPP, b32, 1, 2, 1, 3> yzyw;
        Swizzler<BVector4CPP, b32, 1, 2, 2, 0> yzzx;
        Swizzler<BVector4CPP, b32, 1, 2, 2, 1> yzzy;
        Swizzler<BVector4CPP, b32, 1, 2, 2, 2> yzzz;
        Swizzler<BVector4CPP, b32, 1, 2, 2, 3> yzzw;
        Swizzler<BVector4CPP, b32, 1, 2, 3, 0> yzwx;
        Swizzler<BVector4CPP, b32, 1, 2, 3, 1> yzwy;
        Swizzler<BVector4CPP, b32, 1, 2, 3, 2> yzwz;
        Swizzler<BVector4CPP, b32, 1, 2, 3, 3> yzww;
        Swizzler<BVector4CPP, b32, 1, 3, 0, 0> ywxx;
        Swizzler<BVector4CPP, b32, 1, 3, 0, 1> ywxy;
        Swizzler<BVector4CPP, b32, 1, 3, 0, 2> ywxz;
        Swizzler<BVector4CPP, b32, 1, 3, 0, 3> ywxw;
        Swizzler<BVector4CPP, b32, 1, 3, 1, 0> ywyx;
        Swizzler<BVector4CPP, b32, 1, 3, 1, 1> ywyy;
        Swizzler<BVector4CPP, b32, 1, 3, 1, 2> ywyz;
        Swizzler<BVector4CPP, b32, 1, 3, 1, 3> ywyw;
        Swizzler<BVector4CPP, b32, 1, 3, 2, 0> ywzx;
        Swizzler<BVector4CPP, b32, 1, 3, 2, 1> ywzy;
        Swizzler<BVector4CPP, b32, 1, 3, 2, 2> ywzz;
        Swizzler<BVector4CPP, b32, 1, 3, 2, 3> ywzw;
        Swizzler<BVector4CPP, b32, 1, 3, 3, 0> ywwx;
        Swizzler<BVector4CPP, b32, 1, 3, 3, 1> ywwy;
        Swizzler<BVector4CPP, b32, 1, 3, 3, 2> ywwz;
        Swizzler<BVector4CPP, b32, 1, 3, 3, 3> ywww;
        Swizzler<BVector4CPP, b32, 2, 0, 0, 0> zxxx;
        Swizzler<BVector4CPP, b32, 2, 0, 0, 1> zxxy;
        Swizzler<BVector4CPP, b32, 2, 0, 0, 2> zxxz;
        Swizzler<BVector4CPP, b32, 2, 0, 0, 3> zxxw;
        Swizzler<BVector4CPP, b32, 2, 0, 1, 0> zxyx;
        Swizzler<BVector4CPP, b32, 2, 0, 1, 1> zxyy;
        Swizzler<BVector4CPP, b32, 2, 0, 1, 2> zxyz;
        Swizzler<BVector4CPP, b32, 2, 0, 1, 3> zxyw;
        Swizzler<BVector4CPP, b32, 2, 0, 2, 0> zxzx;
        Swizzler<BVector4CPP, b32, 2, 0, 2, 1> zxzy;
        Swizzler<BVector4CPP, b32, 2, 0, 2, 2> zxzz;
        Swizzler<BVector4CPP, b32, 2, 0, 2, 3> zxzw;
        Swizzler<BVector4CPP, b32, 2, 0, 3, 0> zxwx;
        Swizzler<BVector4CPP, b32, 2, 0, 3, 1> zxwy;
        Swizzler<BVector4CPP, b32, 2, 0, 3, 2> zxwz;
        Swizzler<BVector4CPP, b32, 2, 0, 3, 3> zxww;
        Swizzler<BVector4CPP, b32, 2, 1, 0, 0> zyxx;
        Swizzler<BVector4CPP, b32, 2, 1, 0, 1> zyxy;
        Swizzler<BVector4CPP, b32, 2, 1, 0, 2> zyxz;
        Swizzler<BVector4CPP, b32, 2, 1, 0, 3> zyxw;
        Swizzler<BVector4CPP, b32, 2, 1, 1, 0> zyyx;
        Swizzler<BVector4CPP, b32, 2, 1, 1, 1> zyyy;
        Swizzler<BVector4CPP, b32, 2, 1, 1, 2> zyyz;
        Swizzler<BVector4CPP, b32, 2, 1, 1, 3> zyyw;
        Swizzler<BVector4CPP, b32, 2, 1, 2, 0> zyzx;
        Swizzler<BVector4CPP, b32, 2, 1, 2, 1> zyzy;
        Swizzler<BVector4CPP, b32, 2, 1, 2, 2> zyzz;
        Swizzler<BVector4CPP, b32, 2, 1, 2, 3> zyzw;
        Swizzler<BVector4CPP, b32, 2, 1, 3, 0> zywx;
        Swizzler<BVector4CPP, b32, 2, 1, 3, 1> zywy;
        Swizzler<BVector4CPP, b32, 2, 1, 3, 2> zywz;
        Swizzler<BVector4CPP, b32, 2, 1, 3, 3> zyww;
        Swizzler<BVector4CPP, b32, 2, 2, 0, 0> zzxx;
        Swizzler<BVector4CPP, b32, 2, 2, 0, 1> zzxy;
        Swizzler<BVector4CPP, b32, 2, 2, 0, 2> zzxz;
        Swizzler<BVector4CPP, b32, 2, 2, 0, 3> zzxw;
        Swizzler<BVector4CPP, b32, 2, 2, 1, 0> zzyx;
        Swizzler<BVector4CPP, b32, 2, 2, 1, 1> zzyy;
        Swizzler<BVector4CPP, b32, 2, 2, 1, 2> zzyz;
        Swizzler<BVector4CPP, b32, 2, 2, 1, 3> zzyw;
        Swizzler<BVector4CPP, b32, 2, 2, 2, 0> zzzx;
        Swizzler<BVector4CPP, b32, 2, 2, 2, 1> zzzy;
        Swizzler<BVector4CPP, b32, 2, 2, 2, 2> zzzz;
        Swizzler<BVector4CPP, b32, 2, 2, 2, 3> zzzw;
        Swizzler<BVector4CPP, b32, 2, 2, 3, 0> zzwx;
        Swizzler<BVector4CPP, b32, 2, 2, 3, 1> zzwy;
        Swizzler<BVector4CPP, b32, 2, 2, 3, 2> zzwz;
        Swizzler<BVector4CPP, b32, 2, 2, 3, 3> zzww;
        Swizzler<BVector4CPP, b32, 2, 3, 0, 0> zwxx;
        Swizzler<BVector4CPP, b32, 2, 3, 0, 1> zwxy;
        Swizzler<BVector4CPP, b32, 2, 3, 0, 2> zwxz;
        Swizzler<BVector4CPP, b32, 2, 3, 0, 3> zwxw;
        Swizzler<BVector4CPP, b32, 2, 3, 1, 0> zwyx;
        Swizzler<BVector4CPP, b32, 2, 3, 1, 1> zwyy;
        Swizzler<BVector4CPP, b32, 2, 3, 1, 2> zwyz;
        Swizzler<BVector4CPP, b32, 2, 3, 1, 3> zwyw;
        Swizzler<BVector4CPP, b32, 2, 3, 2, 0> zwzx;
        Swizzler<BVector4CPP, b32, 2, 3, 2, 1> zwzy;
        Swizzler<BVector4CPP, b32, 2, 3, 2, 2> zwzz;
        Swizzler<BVector4CPP, b32, 2, 3, 2, 3> zwzw;
        Swizzler<BVector4CPP, b32, 2, 3, 3, 0> zwwx;
        Swizzler<BVector4CPP, b32, 2, 3, 3, 1> zwwy;
        Swizzler<BVector4CPP, b32, 2, 3, 3, 2> zwwz;
        Swizzler<BVector4CPP, b32, 2, 3, 3, 3> zwww;
        Swizzler<BVector4CPP, b32, 3, 0, 0, 0> wxxx;
        Swizzler<BVector4CPP, b32, 3, 0, 0, 1> wxxy;
        Swizzler<BVector4CPP, b32, 3, 0, 0, 2> wxxz;
        Swizzler<BVector4CPP, b32, 3, 0, 0, 3> wxxw;
        Swizzler<BVector4CPP, b32, 3, 0, 1, 0> wxyx;
        Swizzler<BVector4CPP, b32, 3, 0, 1, 1> wxyy;
        Swizzler<BVector4CPP, b32, 3, 0, 1, 2> wxyz;
        Swizzler<BVector4CPP, b32, 3, 0, 1, 3> wxyw;
        Swizzler<BVector4CPP, b32, 3, 0, 2, 0> wxzx;
        Swizzler<BVector4CPP, b32, 3, 0, 2, 1> wxzy;
        Swizzler<BVector4CPP, b32, 3, 0, 2, 2> wxzz;
        Swizzler<BVector4CPP, b32, 3, 0, 2, 3> wxzw;
        Swizzler<BVector4CPP, b32, 3, 0, 3, 0> wxwx;
        Swizzler<BVector4CPP, b32, 3, 0, 3, 1> wxwy;
        Swizzler<BVector4CPP, b32, 3, 0, 3, 2> wxwz;
        Swizzler<BVector4CPP, b32, 3, 0, 3, 3> wxww;
        Swizzler<BVector4CPP, b32, 3, 1, 0, 0> wyxx;
        Swizzler<BVector4CPP, b32, 3, 1, 0, 1> wyxy;
        Swizzler<BVector4CPP, b32, 3, 1, 0, 2> wyxz;
        Swizzler<BVector4CPP, b32, 3, 1, 0, 3> wyxw;
        Swizzler<BVector4CPP, b32, 3, 1, 1, 0> wyyx;
        Swizzler<BVector4CPP, b32, 3, 1, 1, 1> wyyy;
        Swizzler<BVector4CPP, b32, 3, 1, 1, 2> wyyz;
        Swizzler<BVector4CPP, b32, 3, 1, 1, 3> wyyw;
        Swizzler<BVector4CPP, b32, 3, 1, 2, 0> wyzx;
        Swizzler<BVector4CPP, b32, 3, 1, 2, 1> wyzy;
        Swizzler<BVector4CPP, b32, 3, 1, 2, 2> wyzz;
        Swizzler<BVector4CPP, b32, 3, 1, 2, 3> wyzw;
        Swizzler<BVector4CPP, b32, 3, 1, 3, 0> wywx;
        Swizzler<BVector4CPP, b32, 3, 1, 3, 1> wywy;
        Swizzler<BVector4CPP, b32, 3, 1, 3, 2> wywz;
        Swizzler<BVector4CPP, b32, 3, 1, 3, 3> wyww;
        Swizzler<BVector4CPP, b32, 3, 2, 0, 0> wzxx;
        Swizzler<BVector4CPP, b32, 3, 2, 0, 1> wzxy;
        Swizzler<BVector4CPP, b32, 3, 2, 0, 2> wzxz;
        Swizzler<BVector4CPP, b32, 3, 2, 0, 3> wzxw;
        Swizzler<BVector4CPP, b32, 3, 2, 1, 0> wzyx;
        Swizzler<BVector4CPP, b32, 3, 2, 1, 1> wzyy;
        Swizzler<BVector4CPP, b32, 3, 2, 1, 2> wzyz;
        Swizzler<BVector4CPP, b32, 3, 2, 1, 3> wzyw;
        Swizzler<BVector4CPP, b32, 3, 2, 2, 0> wzzx;
        Swizzler<BVector4CPP, b32, 3, 2, 2, 1> wzzy;
        Swizzler<BVector4CPP, b32, 3, 2, 2, 2> wzzz;
        Swizzler<BVector4CPP, b32, 3, 2, 2, 3> wzzw;
        Swizzler<BVector4CPP, b32, 3, 2, 3, 0> wzwx;
        Swizzler<BVector4CPP, b32, 3, 2, 3, 1> wzwy;
        Swizzler<BVector4CPP, b32, 3, 2, 3, 2> wzwz;
        Swizzler<BVector4CPP, b32, 3, 2, 3, 3> wzww;
        Swizzler<BVector4CPP, b32, 3, 3, 0, 0> wwxx;
        Swizzler<BVector4CPP, b32, 3, 3, 0, 1> wwxy;
        Swizzler<BVector4CPP, b32, 3, 3, 0, 2> wwxz;
        Swizzler<BVector4CPP, b32, 3, 3, 0, 3> wwxw;
        Swizzler<BVector4CPP, b32, 3, 3, 1, 0> wwyx;
        Swizzler<BVector4CPP, b32, 3, 3, 1, 1> wwyy;
        Swizzler<BVector4CPP, b32, 3, 3, 1, 2> wwyz;
        Swizzler<BVector4CPP, b32, 3, 3, 1, 3> wwyw;
        Swizzler<BVector4CPP, b32, 3, 3, 2, 0> wwzx;
        Swizzler<BVector4CPP, b32, 3, 3, 2, 1> wwzy;
        Swizzler<BVector4CPP, b32, 3, 3, 2, 2> wwzz;
        Swizzler<BVector4CPP, b32, 3, 3, 2, 3> wwzw;
        Swizzler<BVector4CPP, b32, 3, 3, 3, 0> wwwx;
        Swizzler<BVector4CPP, b32, 3, 3, 3, 1> wwwy;
        Swizzler<BVector4CPP, b32, 3, 3, 3, 2> wwwz;
        Swizzler<BVector4CPP, b32, 3, 3, 3, 3> wwww;

        b32 array[4];
    };

    /// @brief Create empty vector.
    constexpr BVector4CPP();
    /// @brief Implicitly convert POD vector to C++ vector.
    constexpr BVector4CPP( const BVector4& __pod );
    /// @brief Create vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @param w W component.
    constexpr explicit BVector4CPP( b32 x, b32 y, b32 z, b32 w );
    /// @brief Create vector.
    /// @param x X, Y, Z and W components.
    constexpr explicit BVector4CPP( b32 x );
    /// @brief Create vector.
    /// @param x  X component.
    /// @param y  Y component.
    /// @param zw Z and W components.
    constexpr explicit BVector4CPP( b32 x, b32 y, BVector2CPP zw );
    /// @brief Create vector.
    /// @param x  X component.
    /// @param yz Y and Z components.
    /// @param w  W component.
    constexpr explicit BVector4CPP( b32 x, BVector2CPP yz, b32 w );
    /// @brief Create vector.
    /// @param xy X and Y components.
    /// @param z  Z component.
    /// @param w  W component.
    constexpr explicit BVector4CPP( BVector2CPP xy, b32 z, b32 w );
    /// @brief Create vector.
    /// @param xy X and Y components.
    /// @param zw Z and W components.
    constexpr explicit BVector4CPP( BVector2CPP xy, BVector2CPP zw );
    /// @brief Create vector.
    /// @param x   X component.
    /// @param yzw Y, Z and W components.
    constexpr explicit BVector4CPP( b32 x, BVector3CPP yzw );
    /// @brief Create vector.
    /// @param xyz X, Y and Z components.
    /// @param w   W component.
    constexpr explicit BVector4CPP( BVector3CPP xyz, b32 w );
    /// @brief Convert float vector to boolean vector.
    constexpr explicit BVector4CPP( const Vector4& iv );
    /// @brief Convert integer vector to boolean vector.
    constexpr explicit BVector4CPP( const IVector4& bv );

    /// @brief Implicitly convert C++ vector to POD vector.
    constexpr operator BVector4() const;
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
    static constexpr BVector4CPP zero();
    /// @brief Vector one constant.
    /// @return Value.
    static constexpr BVector4CPP one();
};

/// @brief 4 Component 32-bit Floating Point Vector.
typedef Vector4CPP vec4;
/// @brief 4 Component 32-bit Integer Vector.
typedef IVector4CPP ivec4;
/// @brief 4 Component 32-bit Boolean Vector.
typedef BVector4CPP bvec4;

constexpr attr_always_inline attr_header attr_hot
Vector4CPP::Vector4CPP()
    : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP::Vector4CPP( const Vector4& __pod )
    : x(__pod.x), y(__pod.y), z(__pod.z), w(__pod.w) {}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP::Vector4CPP( f32 x, f32 y, f32 z, f32 w )
    : x(x), y(y), z(z), w(w) {}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP::Vector4CPP( f32 x )
    : x(x), y(x), z(x), w(x) {}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP::Vector4CPP( f32 x, f32 y, Vector2CPP zw )
    : x(x), y(y), z(zw.x), w(zw.y) {}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP::Vector4CPP( f32 x, Vector2CPP yz, f32 w )
    : x(x), y(yz.x), z(yz.y), w(w) {}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP::Vector4CPP( Vector2CPP xy, f32 z, f32 w )
    : x(xy.x), y(xy.y), z(z), w(w) {}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP::Vector4CPP( Vector2CPP xy, Vector2CPP zw )
    : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP::Vector4CPP( f32 x, Vector3CPP yzw )
    : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP::Vector4CPP( Vector3CPP xyz, f32 w )
    : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP::Vector4CPP( const IVector4& iv )
    : x(iv.x), y(iv.y), z(iv.z), w(iv.w) {}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP::Vector4CPP( const BVector4& bv )
    : x(bv.x ? 1.0f : 0.0f),
      y(bv.y ? 1.0f : 0.0f),
      z(bv.z ? 1.0f : 0.0f),
      w(bv.w ? 1.0f : 0.0f) {}

constexpr attr_always_inline attr_header attr_hot
Vector4CPP::operator Vector4() const {
    return *(Vector4*)this;
}
constexpr attr_always_inline attr_header attr_hot
f32 Vector4CPP::operator[]( usize index ) const {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
f32& Vector4CPP::operator[]( usize index ) {
    return this->array[index];
}

constexpr attr_always_inline attr_header attr_hot
Vector4CPP Vector4CPP::zero() {
    return VEC4_ZERO;
}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP Vector4CPP::one() {
    return VEC4_ONE;
}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP Vector4CPP::clear() {
    return RGBA_CLEAR;
}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP Vector4CPP::black() {
    return RGBA_BLACK;
}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP Vector4CPP::white() {
    return RGBA_WHITE;
}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP Vector4CPP::red() {
    return RGBA_RED;
}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP Vector4CPP::green() {
    return RGBA_GREEN;
}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP Vector4CPP::blue() {
    return RGBA_BLUE;
}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP Vector4CPP::yellow() {
    return RGBA_YELLOW;
}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP Vector4CPP::magenta() {
    return RGBA_MAGENTA;
}
constexpr attr_always_inline attr_header attr_hot
Vector4CPP Vector4CPP::cyan() {
    return RGBA_CYAN;
}

/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
constexpr attr_always_inline attr_header attr_hot
vec4 add( vec4 lhs, vec4 rhs ) {
    return vec4_add( lhs, rhs );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
constexpr attr_always_inline attr_header attr_hot
vec4 sub( vec4 lhs, vec4 rhs ) {
    return vec4_sub( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec4 mul( vec4 lhs, f32 rhs ) {
    return vec4_mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Scalar to multiply components by.
/// @param rhs Vector to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec4 mul( f32 lhs, vec4 rhs ) {
    return vec4_mul( rhs, lhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec4 mul( vec4 lhs, vec4 rhs ) {
    return vec4_mul_vec4( lhs, rhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec4 hadamard( vec4 lhs, vec4 rhs ) {
    return vec4_mul_vec4( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec4 div( vec4 lhs, f32 rhs ) {
    return vec4_div( lhs, rhs );
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec4 div( vec4 lhs, vec4 rhs ) {
    return vec4_div_vec4( lhs, rhs );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of modulus division.
constexpr attr_always_inline attr_header attr_hot
vec4 mod( vec4 lhs, f32 rhs ) {
    return vec4_mod( lhs, rhs );
}
/// @brief Modulus divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of modulus division.
constexpr attr_always_inline attr_header attr_hot
vec4 mod( vec4 lhs, vec4 rhs ) {
    return vec4_mod_vec4( lhs, rhs );
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
constexpr attr_always_inline attr_header attr_hot
vec4 neg( vec4 x ) {
    return vec4_neg( x );
}

/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
constexpr attr_always_inline attr_header attr_hot
vec4 operator+( vec4 lhs, vec4 rhs ) {
    return add( lhs, rhs );
}
/// @brief Component-wise add vectors.
/// @param lhs, rhs Vectors to add.
/// @return Result of addition.
constexpr attr_always_inline attr_header attr_hot
vec4& operator+=( vec4& lhs, vec4 rhs ) {
    return lhs = lhs + rhs;
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
constexpr attr_always_inline attr_header attr_hot
vec4 operator-( vec4 lhs, vec4 rhs ) {
    return sub( lhs, rhs );
}
/// @brief Component-wise subtract vectors.
/// @param lhs, rhs Vectors to subtract.
/// @return Result of subtraction.
constexpr attr_always_inline attr_header attr_hot
vec4& operator-=( vec4& lhs, vec4 rhs ) {
    return lhs = lhs - rhs;
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec4 operator*( vec4 lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Scalar to multiply components by.
/// @param rhs Vector to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec4 operator*( f32 lhs, vec4 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec4 operator*( vec4 lhs, vec4 rhs ) {
    return mul( lhs, rhs );
}
/// @brief Multiply vector components.
/// @param lhs Vector to multiply.
/// @param rhs Scalar to multiply components by.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec4& operator*=( vec4& lhs, f32 rhs ) {
    return lhs = lhs * rhs;
}
/// @brief Hadamard product between two vectors.
///
/// Component-wise multiplication of two vectors.
/// @param lhs, rhs Vectors to multiply.
/// @return Result of multiplication.
constexpr attr_always_inline attr_header attr_hot
vec4& operator*=( vec4& lhs, vec4 rhs ) {
    return lhs = lhs * rhs;
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec4 operator/( vec4 lhs, f32 rhs ) {
    return div( lhs, rhs );
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec4 operator/( vec4 lhs, vec4 rhs ) {
    return div( lhs, rhs );
}
/// @brief Divide vector components.
/// @param lhs Vector to divide.
/// @param rhs Scalar to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec4& operator/=( vec4 lhs, f32 rhs ) {
    return lhs = lhs / rhs;
}
/// @brief Component-wise division.
/// @param lhs Vector to divide.
/// @param rhs Vector to divide components by.
/// @return Result of division.
constexpr attr_always_inline attr_header attr_hot
vec4& operator/=( vec4 lhs, vec4 rhs ) {
    return lhs = lhs / rhs;
}
/// @brief Negate components of a vector.
/// @param x Vector to negate.
/// @return Result of negation.
constexpr attr_always_inline attr_header attr_hot
vec4 operator-( vec4 x ) {
    return neg(x);
}

attr_always_inline attr_header attr_hot
vec4 rotl( vec4 x ) {
    return vec4_rotl( x );
}
attr_always_inline attr_header attr_hot
vec4 rotr( vec4 x ) {
    return vec4_rotr( x );
}
attr_always_inline attr_header attr_hot
f32 hadd( vec4 x ) {
    return vec4_hadd( x );
}
attr_always_inline attr_header attr_hot
f32 hmul( vec4 x ) {
    return vec4_hmul( x );
}
attr_always_inline attr_header attr_hot
f32 dot( vec4 lhs, vec4 rhs ) {
    return vec4_dot( lhs, rhs );
}
attr_always_inline attr_header attr_hot
f32 min( vec4 x ) {
    return vec4_min( x );
}
attr_always_inline attr_header attr_hot
vec4 min( vec4 x, vec4 y ) {
    return vec4_min_vec4( x, y );
}
attr_always_inline attr_header attr_hot
f32 max( vec4 x ) {
    return vec4_max( x );
}
attr_always_inline attr_header attr_hot
vec4 max( vec4 x, vec4 y ) {
    return vec4_max_vec4( x, y );
}
attr_always_inline attr_header attr_hot
vec4 clamp( vec4 v, vec4 min, vec4 max ) {
    return vec4_clamp( v, min, max );
}
attr_always_inline attr_header attr_hot
vec4 clamp( vec4 v, f32 min, f32 max ) {
    return vec4_clamp_length( v, min, max );
}
attr_always_inline attr_header attr_hot
f32 length_sqr( vec4 x ) {
    return vec4_length_sqr( x );
}
attr_always_inline attr_header attr_hot
f32 length( vec4 x ) {
    return vec4_length( x );
}
attr_always_inline attr_header attr_hot
f32 distance_sqr( vec4 a, vec4 b ) {
    return vec4_distance_sqr( a, b );
}
attr_always_inline attr_header attr_hot
f32 distance( vec4 a, vec4 b ) {
    return vec4_distance( a, b );
}
attr_always_inline attr_header attr_hot
vec4 normalize( vec4 x ) {
    return vec4_normalize( x );
}
attr_always_inline attr_header attr_hot
vec4 abs( vec4 x ) {
    return vec4_abs( x );
}
attr_always_inline attr_header attr_hot
vec4 sign( vec4 x ) {
    return vec4_sign( x );
}
attr_always_inline attr_header attr_hot
vec4 trunc( vec4 x ) {
    return vec4_trunc( x );
}
attr_always_inline attr_header attr_hot
vec4 floor( vec4 x ) {
    return vec4_floor( x );
}
attr_always_inline attr_header attr_hot
vec4 ceil( vec4 x ) {
    return vec4_ceil( x );
}
attr_always_inline attr_header attr_hot
vec4 round( vec4 x ) {
    return vec4_round( x );
}
attr_always_inline attr_header attr_hot
vec4 fract( vec4 x ) {
    return vec4_fract( x );
}
attr_always_inline attr_header attr_hot
vec4 lerp( vec4 a, vec4 b, f32 t ) {
    return vec4_lerp( a, b, t );
}
attr_always_inline attr_header attr_hot
vec4 lerp( vec4 a, vec4 b, vec4 t ) {
    return vec4_lerp_vec4( a, b, t );
}
attr_always_inline attr_header attr_hot
vec4 mix( vec4 a, vec4 b, f32 t ) {
    return lerp( a, b, t );
}
attr_always_inline attr_header attr_hot
vec4 mix( vec4 a, vec4 b, vec4 t ) {
    return lerp( a, b, t );
}
attr_always_inline attr_header attr_hot
vec4 step( vec4 edge, vec4 x ) {
    return vec4_step_vec4( edge, x );
}
attr_always_inline attr_header attr_hot
vec4 step( f32 edge, vec4 x ) {
    return vec4_step( edge, x );
}
attr_always_inline attr_header attr_hot
vec4 smoothstep( vec4 edge0, vec4 edge1, vec4 x ) {
    return vec4_smoothstep_vec4( edge0, edge1, x );
}
attr_always_inline attr_header attr_hot
vec4 smoothstep( f32 edge0, f32 edge1, vec4 x ) {
    return vec4_smoothstep( edge0, edge1, x );
}
attr_always_inline attr_header attr_hot
vec4 smootherstep( vec4 edge0, vec4 edge1, vec4 x ) {
    return vec4_smootherstep_vec4( edge0, edge1, x );
}
attr_always_inline attr_header attr_hot
vec4 smootherstep( f32 edge0, f32 edge1, vec4 x ) {
    return vec4_smootherstep( edge0, edge1, x );
}
attr_always_inline attr_header attr_hot
bvec4 isnan( vec4 x ) {
    return vec4_isnan( x );
}
attr_always_inline attr_header attr_hot
bvec4 isinf( vec4 x ) {
    return vec4_isinf( x );
}
attr_always_inline attr_header attr_hot
vec4 radians( vec4 degrees ) {
    return vec4_radians( degrees );
}
attr_always_inline attr_header attr_hot
vec4 degrees( vec4 radians ) {
    return vec4_degrees( radians );
}
attr_always_inline attr_header attr_hot
vec4 sin( vec4 x ) {
    return vec4_sin( x );
}
attr_always_inline attr_header attr_hot
vec4 cos( vec4 x ) {
    return vec4_cos( x );
}
attr_always_inline attr_header attr_hot
vec4 tan( vec4 x ) {
    return vec4_tan( x );
}
attr_always_inline attr_header attr_hot
vec4 asin( vec4 x ) {
    return vec4_asin( x );
}
attr_always_inline attr_header attr_hot
vec4 acos( vec4 x ) {
    return vec4_acos( x );
}
attr_always_inline attr_header attr_hot
vec4 atan( vec4 x ) {
    return vec4_atan( x );
}
attr_always_inline attr_header attr_hot
vec4 atan2( vec4 y, vec4 x ) {
    return vec4_atan2( y, x );
}
attr_always_inline attr_header attr_hot
vec4 pow( vec4 base, vec4 exp ) {
    return vec4_pow( base, exp );
}
attr_always_inline attr_header attr_hot
vec4 exp( vec4 x ) {
    return vec4_exp( x );
}
attr_always_inline attr_header attr_hot
vec4 exp2( vec4 x ) {
    return vec4_exp2( x );
}
attr_always_inline attr_header attr_hot
vec4 ln( vec4 x ) {
    return vec4_ln( x );
}
attr_always_inline attr_header attr_hot
vec4 log2( vec4 x ) {
    return vec4_log2( x );
}
attr_always_inline attr_header attr_hot
vec4 log10( vec4 x ) {
    return vec4_log10( x );
}
attr_always_inline attr_header attr_hot
vec4 sqrt( vec4 x ) {
    return vec4_sqrt( x );
}
attr_always_inline attr_header attr_hot
vec4 inversesqrt( vec4 x ) {
    return vec4_inversesqrt( x );
}
attr_always_inline attr_header attr_hot
bool cmp( vec4 a, vec4 b ) {
    return vec4_cmp( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 lt( vec4 a, vec4 b ) {
    return vec4_lt( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 gt( vec4 a, vec4 b ) {
    return vec4_gt( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 lteq( vec4 a, vec4 b ) {
    return vec4_lteq( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 gteq( vec4 a, vec4 b ) {
    return vec4_gteq( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 eq( vec4 a, vec4 b ) {
    return vec4_eq( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 neq( vec4 a, vec4 b ) {
    return vec4_neq( a, b );
}

constexpr attr_always_inline attr_header attr_hot
IVector4CPP::IVector4CPP()
    : x(0), y(0), z(0), w(0) {}
constexpr attr_always_inline attr_header attr_hot
IVector4CPP::IVector4CPP( const IVector4& __pod )
     : x(__pod.x), y(__pod.y), z(__pod.z), w(__pod.w) {}
constexpr attr_always_inline attr_header attr_hot
IVector4CPP::IVector4CPP( i32 x, i32 y, i32 z, i32 w )
    : x(x), y(y), z(z), w(w) {}
constexpr attr_always_inline attr_header attr_hot
IVector4CPP::IVector4CPP( i32 x )
    : x(x), y(x), z(x), w(x) {}
constexpr attr_always_inline attr_header attr_hot
IVector4CPP::IVector4CPP( i32 x, i32 y, IVector2CPP zw )
    : x(x), y(y), z(zw.x), w(zw.y) {}
constexpr attr_always_inline attr_header attr_hot
IVector4CPP::IVector4CPP( i32 x, IVector2CPP yz, i32 w )
    : x(x), y(yz.x), z(yz.y), w(w) {}
constexpr attr_always_inline attr_header attr_hot
IVector4CPP::IVector4CPP( IVector2CPP xy, i32 z, i32 w )
    : x(xy.x), y(xy.y), z(z), w(w) {}
constexpr attr_always_inline attr_header attr_hot
IVector4CPP::IVector4CPP( IVector2CPP xy, IVector2CPP zw )
    : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
constexpr attr_always_inline attr_header attr_hot
IVector4CPP::IVector4CPP( i32 x, IVector3CPP yzw )
    : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}
constexpr attr_always_inline attr_header attr_hot
IVector4CPP::IVector4CPP( IVector3CPP xyz, i32 w )
    : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
constexpr attr_always_inline attr_header attr_hot
IVector4CPP::IVector4CPP( const Vector4& v )
    : x(trunc(v.x)), y(trunc(v.y)), z(trunc(v.z)), w(trunc(v.w)) {}
constexpr attr_always_inline attr_header attr_hot
IVector4CPP::IVector4CPP( const BVector4& bv )
    : x(bv.x ? 1 : 0),
      y(bv.y ? 1 : 0),
      z(bv.z ? 1 : 0),
      w(bv.w ? 1 : 0) {}

constexpr attr_always_inline attr_header attr_hot
IVector4CPP::operator IVector4() const {
    return *(IVector4*)this;
}
constexpr attr_always_inline attr_header attr_hot
i32 IVector4CPP::operator[]( usize index ) const {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
i32& IVector4CPP::operator[]( usize index ) {
    return this->array[index];
}

constexpr attr_always_inline attr_header attr_hot
IVector4CPP IVector4CPP::zero() {
    return IVEC4_ZERO;
}
constexpr attr_always_inline attr_header attr_hot
IVector4CPP IVector4CPP::one() {
    return IVEC4_ONE;
}

constexpr attr_always_inline attr_header attr_hot
ivec4 add( ivec4 lhs, ivec4 rhs ) {
    return ivec4_add( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 sub( ivec4 lhs, ivec4 rhs ) {
    return ivec4_sub( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 mul( ivec4 lhs, i32 rhs ) {
    return ivec4_mul( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 mul( i32 lhs, ivec4 rhs ) {
    return mul( rhs, lhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 mul( ivec4 lhs, ivec4 rhs ) {
    return ivec4_mul_ivec4( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 hadamard( ivec4 lhs, ivec4 rhs ) {
    return ivec4_hadamard( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 div( ivec4 lhs, i32 rhs ) {
    return ivec4_div( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 div( ivec4 lhs, ivec4 rhs ) {
    return ivec4_div_ivec4( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 mod( ivec4 lhs, i32 rhs ) {
    return ivec4_mod( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 mod( ivec4 lhs, ivec4 rhs ) {
    return ivec4_mod_ivec4( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 neg( ivec4 x ) {
    return ivec4_neg( x );
}

constexpr attr_always_inline attr_header attr_hot
ivec4 operator+( ivec4 lhs, ivec4 rhs ) {
    return add( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4& operator+=( ivec4& lhs, ivec4 rhs ) {
    return lhs = lhs + rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec4 operator-( ivec4 lhs, ivec4 rhs ) {
    return sub( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4& operator-=( ivec4& lhs, ivec4 rhs ) {
    return lhs = lhs - rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec4 operator*( ivec4 lhs, i32 rhs ) {
    return mul( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 operator*( i32 lhs, ivec4 rhs ) {
    return mul( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 operator*( ivec4 lhs, ivec4 rhs ) {
    return mul( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4& operator*=( ivec4& lhs, i32 rhs ) {
    return lhs = lhs * rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec4& operator*=( ivec4& lhs, ivec4 rhs ) {
    return lhs = lhs * rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec4 operator/( ivec4 lhs, i32 rhs ) {
    return div( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 operator/( ivec4 lhs, ivec4 rhs ) {
    return div( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4& operator/=( ivec4& lhs, i32 rhs ) {
    return lhs = lhs / rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec4& operator/=( ivec4& lhs, ivec4 rhs ) {
    return lhs = lhs / rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec4 operator%( ivec4 lhs, i32 rhs ) {
    return mod( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4 operator%( ivec4 lhs, ivec4 rhs ) {
    return mod( lhs, rhs );
}
constexpr attr_always_inline attr_header attr_hot
ivec4& operator%=( ivec4& lhs, i32 rhs ) {
    return lhs = lhs % rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec4& operator%=( ivec4& lhs, ivec4 rhs ) {
    return lhs = lhs % rhs;
}
constexpr attr_always_inline attr_header attr_hot
ivec4 operator-( ivec4 x ) {
    return neg( x );
}

attr_always_inline attr_header attr_hot
ivec4 rotl( ivec4 x ) {
    return ivec4_rotl( x );
}
attr_always_inline attr_header attr_hot
ivec4 rotr( ivec4 x ) {
    return ivec4_rotr( x );
}
attr_always_inline attr_header attr_hot
i32 hadd( ivec4 x ) {
    return ivec4_hadd( x );
}
attr_always_inline attr_header attr_hot
i32 hmul( ivec4 x ) {
    return ivec4_hmul( x );
}
attr_always_inline attr_header attr_hot
f32 length_sqr( ivec4 x ) {
    return ivec4_length_sqr( x );
}
attr_always_inline attr_header attr_hot
f32 length( ivec4 x ) {
    return ivec4_length( x );
}
attr_always_inline attr_header attr_hot
f32 dot( ivec4 lhs, ivec4 rhs ) {
    return ivec4_dot( lhs, rhs );
}
attr_always_inline attr_header attr_hot
i32 min( ivec4 x ) {
    return ivec4_min( x );
}
attr_always_inline attr_header attr_hot
ivec4 min( ivec4 x, ivec4 y ) {
    return ivec4_min_ivec4( x, y );
}
attr_always_inline attr_header attr_hot
i32 max( ivec4 x ) {
    return ivec4_max( x );
}
attr_always_inline attr_header attr_hot
ivec4 max( ivec4 x, ivec4 y ) {
    return ivec4_max_ivec4( x, y );
}
attr_always_inline attr_header attr_hot
ivec4 clamp( ivec4 v, ivec4 min, ivec4 max ) {
    return ivec4_clamp( v, min, max );
}
attr_always_inline attr_header attr_hot
ivec4 abs( ivec4 x ) {
    return ivec4_abs( x );
}
attr_always_inline attr_header attr_hot
ivec4 sign( ivec4 x ) {
    return ivec4_sign( x );
}
attr_always_inline attr_header attr_hot
bool cmp( ivec4 a, ivec4 b ) {
    return ivec4_cmp( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 lt( ivec4 a, ivec4 b ) {
    return ivec4_lt( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 gt( ivec4 a, ivec4 b ) {
    return ivec4_gt( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 lteq( ivec4 a, ivec4 b ) {
    return ivec4_lteq( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 gteq( ivec4 a, ivec4 b ) {
    return ivec4_gteq( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 eq( ivec4 a, ivec4 b ) {
    return ivec4_eq( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 neq( ivec4 a, ivec4 b ) {
    return ivec4_neq( a, b );
}

constexpr attr_always_inline attr_header attr_hot
BVector4CPP::BVector4CPP()
    : x(false), y(false), z(false), w(false) {}
constexpr attr_always_inline attr_header attr_hot
BVector4CPP::BVector4CPP( const BVector4& __pod )
     : x(__pod.x), y(__pod.y), z(__pod.z), w(__pod.w) {}
constexpr attr_always_inline attr_header attr_hot
BVector4CPP::BVector4CPP( b32 x, b32 y, b32 z, b32 w )
    : x(x), y(y), z(z), w(w) {}
constexpr attr_always_inline attr_header attr_hot
BVector4CPP::BVector4CPP( b32 x )
    : x(x), y(x), z(x), w(x) {}
constexpr attr_always_inline attr_header attr_hot
BVector4CPP::BVector4CPP( b32 x, b32 y, BVector2CPP zw )
    : x(x), y(y), z(zw.x), w(zw.y) {}
constexpr attr_always_inline attr_header attr_hot
BVector4CPP::BVector4CPP( b32 x, BVector2CPP yz, b32 w )
    : x(x), y(yz.x), z(yz.y), w(w) {}
constexpr attr_always_inline attr_header attr_hot
BVector4CPP::BVector4CPP( BVector2CPP xy, b32 z, b32 w )
    : x(xy.x), y(xy.y), z(z), w(w) {}
constexpr attr_always_inline attr_header attr_hot
BVector4CPP::BVector4CPP( BVector2CPP xy, BVector2CPP zw )
    : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
constexpr attr_always_inline attr_header attr_hot
BVector4CPP::BVector4CPP( b32 x, BVector3CPP yzw )
    : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}
constexpr attr_always_inline attr_header attr_hot
BVector4CPP::BVector4CPP( BVector3CPP xyz, b32 w )
    : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
constexpr attr_always_inline attr_header attr_hot
BVector4CPP::BVector4CPP( const Vector4& v )
    : x(v.x == 0.0f ? false : true),
      y(v.y == 0.0f ? false : true),
      z(v.z == 0.0f ? false : true),
      w(v.w == 0.0f ? false : true) {}
constexpr attr_always_inline attr_header attr_hot
BVector4CPP::BVector4CPP( const IVector4& iv )
    : x(iv.x ? true : false),
      y(iv.y ? true : false),
      z(iv.z ? true : false),
      w(iv.w ? true : false) {}

constexpr attr_always_inline attr_header attr_hot
BVector4CPP::operator BVector4() const {
    return *(BVector4*)this;
}
constexpr attr_always_inline attr_header attr_hot
b32 BVector4CPP::operator[]( usize index ) const {
    return this->array[index];
}
constexpr attr_always_inline attr_header attr_hot
b32& BVector4CPP::operator[]( usize index ) {
    return this->array[index];
}

constexpr attr_always_inline attr_header attr_hot
BVector4CPP BVector4CPP::zero() {
    return BVEC4_ZERO;
}
constexpr attr_always_inline attr_header attr_hot
BVector4CPP BVector4CPP::one() {
    return BVEC4_ONE;
}

attr_always_inline attr_header attr_hot
bvec4 eq( bvec4 a, bvec4 b ) {
    return bvec4_eq( a, b );
}
attr_always_inline attr_header attr_hot
bvec4 neq( bvec4 a, bvec4 b ) {
    return bvec4_neq( a, b );
}
attr_always_inline attr_header attr_hot
bool any( bvec4 x ) {
    return bvec4_any( x );
}
attr_always_inline attr_header attr_hot
bool all( bvec4 x ) {
    return bvec4_all( x );
}
attr_always_inline attr_header attr_hot
bvec4 flip( bvec4 x ) {
    return bvec4_flip( x );
}

#endif /* header guard */
