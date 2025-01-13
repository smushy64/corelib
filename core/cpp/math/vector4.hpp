#if !defined(CORE_CPP_MATH_VECTOR4_HPP)
#define CORE_CPP_MATH_VECTOR4_HPP
/**
 * @file   vector4.hpp
 * @brief  C++ Vector4.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 28, 2024
*/

struct Vector4CPP;
struct IVector4CPP;

#if !defined(CORE_MATH_VECTOR4_H)
    #include "core/math/vector4.h"
#endif

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
        // struct { f32 x, y, z, w; };
        // struct { f32 r, g, b, a; };
        struct Vector4 pod;

        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 0, 0> xx, rr;
        // SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 0, 1> xy, rg;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 0, 2> xz, rb;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 0, 3> xw, ra;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 1, 0> yx, gr;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 1, 1> yy, gg;
        // SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 1, 2> yz, gb;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 1, 3> yw, ga;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 2, 0> zx, br;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 2, 1> zy, bg;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 2, 2> zz, bb;
        // SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 2, 3> zw, ba;
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
        // SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 1, 2> xyz, rgb;
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
        // SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 2, 3> yzw, gba;
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
        Swizzler<Vector4CPP, f32, 0, 1, 2, 3> xyzw, rgba;
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

    attr_always_inline attr_header constexpr
    Vector4CPP() : x(0), y(0), z(0), w(0) {}
    attr_always_inline attr_header constexpr
    Vector4CPP( const struct Vector4& v ) : x(v.x), y(v.y), z(v.z), w(v.w) {}
    attr_always_inline attr_header constexpr
    explicit Vector4CPP( const struct IVector4& v ) : x(v.x), y(v.y), z(v.z), w(v.w) {}
    attr_always_inline attr_header constexpr
    explicit Vector4CPP( f32 s ) : x(s), y(s), z(s), w(s) {}
    attr_always_inline attr_header constexpr
    explicit Vector4CPP( f32 x, f32 y, f32 z, f32 w ) : x(x), y(y), z(z), w(w) {}
    attr_always_inline attr_header constexpr
    explicit Vector4CPP( Vector2CPP xy, f32 z, f32 w ) : Vector4CPP( xy.x, xy.y, z, w ) {}
    attr_always_inline attr_header constexpr
    explicit Vector4CPP( f32 x, f32 y, Vector2CPP zw ) : Vector4CPP( x, y, zw.x, zw.y ) {}
    attr_always_inline attr_header constexpr
    explicit Vector4CPP( f32 x, Vector2CPP yz, f32 w ) : Vector4CPP( x, yz.x, yz.y, w ) {}
    attr_always_inline attr_header constexpr
    explicit Vector4CPP( Vector2CPP xy, Vector2CPP zw ) : Vector4CPP( xy.x, xy.y, zw.x, zw.y ) {}
    attr_always_inline attr_header constexpr
    explicit Vector4CPP( Vector3CPP xyz, f32 w ) : Vector4CPP( xyz.xy, xyz.z, w ) {}
    attr_always_inline attr_header constexpr
    explicit Vector4CPP( f32 x, Vector3CPP yzw ) : Vector4CPP( x, yzw.xy, yzw.z ) {}

    attr_always_inline attr_header constexpr
    operator Vector4() const {
        return *(struct Vector4*)this;
    }

    attr_always_inline attr_header static constexpr
    Vector4CPP zero() {
        return Vector4CPP();
    }
    attr_always_inline attr_header static constexpr
    Vector4CPP one() {
        return Vector4CPP( 1.0, 1.0, 1.0, 1.0 );
    }

    attr_always_inline attr_header static constexpr
    Vector4CPP red() {
        return Vector4CPP( 1.0, 0.0, 0.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector4CPP green() {
        return Vector4CPP( 0.0, 1.0, 0.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector4CPP blue() {
        return Vector4CPP( 0.0, 0.0, 1.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector4CPP yellow() {
        return Vector4CPP( 1.0, 1.0, 0.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector4CPP magenta() {
        return Vector4CPP( 1.0, 0.0, 1.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector4CPP cyan() {
        return Vector4CPP( 0.0, 1.0, 1.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector4CPP black() {
        return Vector4CPP( 0.0, 0.0, 0.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector4CPP white() {
        return Vector4CPP( 1.0, 1.0, 1.0, 1.0 );
    }
    attr_always_inline attr_header static constexpr
    Vector4CPP clear() {
        return Vector4CPP( 0.0, 0.0, 0.0, 0.0 );
    }

    attr_always_inline attr_header static constexpr
    Vector4CPP from_array( const f32 array[4] ) {
        return *(Vector4CPP*)array;
    }
    attr_always_inline attr_header constexpr
    void to_array( f32 out_array[4] ) const {
        out_array[0] = array[0];
        out_array[1] = array[1];
        out_array[2] = array[2];
        out_array[3] = array[3];
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
                        /// @brief X and Y components as an #IVector2.
                        IVector2CPP xy;
                    };
                    /// @brief Z component.
                    union {
                        /// @brief Z component.
                        i32 z;
                    };
                };
                /// @brief Swizzle yz.
                struct {
                    i32 __unused0;
                    /// @brief Y and Z components as an #IVector2.
                    IVector2CPP yz;
                };
                /// @brief X, Y and Z components as an #IVector3.
                IVector3CPP xyz;
            };
            /// @brief W component.
            union {
                /// @brief W component.
                i32 w;
            };
        };
        /// @brief Swizzle yzw.
        struct {
            i32 __unused2;
            /// @brief Y, Z and W components as an #IVector3.
            IVector3CPP yzw;
        };
        /// @brief Swizzle zw.
        struct {
            IVector2CPP __unused4;
            /// @brief Z and W components as an #IVector2.
            IVector2CPP zw;
        };
        // struct { i32 x, y, z, w; };
        struct IVector4 pod;

        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 0, 0> xx;
        // SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 0, 1> xy;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 0, 2> xz;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 0, 3> xw;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 1, 0> yx;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 1, 1> yy;
        // SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 1, 2> yz;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 1, 3> yw;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 2, 0> zx;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 2, 1> zy;
        SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 2, 2> zz;
        // SwizzlerConvert<IVector4CPP, IVector2CPP, i32, 2, 3> zw;
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
        // SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 0, 1, 2> xyz;
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
        // SwizzlerConvert<IVector4CPP, IVector3CPP, i32, 1, 2, 3> yzw;
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
        Swizzler<IVector4CPP, i32, 0, 1, 2, 3> xyzw;
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

    attr_always_inline attr_header constexpr
    IVector4CPP() : x(0), y(0), z(0), w(0) {}
    attr_always_inline attr_header constexpr
    IVector4CPP( const struct IVector4& v ) : x(v.x), y(v.y), z(v.z), w(v.w) {}
    attr_always_inline attr_header constexpr
    explicit IVector4CPP( const Vector4CPP& v ) : x(v.x), y(v.y), z(v.z), w(v.w) {}
    attr_always_inline attr_header constexpr
    explicit IVector4CPP( i32 s ) : x(s), y(s), z(s), w(s) {}
    attr_always_inline attr_header constexpr
    explicit IVector4CPP( i32 x, i32 y, i32 z, i32 w ) : x(x), y(y), z(z), w(w) {}
    attr_always_inline attr_header constexpr
    explicit IVector4CPP( IVector2CPP xy, i32 z, i32 w ) : IVector4CPP( xy.x, xy.y, z, w ) {}
    attr_always_inline attr_header constexpr
    explicit IVector4CPP( i32 x, i32 y, IVector2CPP zw ) : IVector4CPP( x, y, zw.x, zw.y ) {}
    attr_always_inline attr_header constexpr
    explicit IVector4CPP( i32 x, IVector2CPP yz, i32 w ) : IVector4CPP( x, yz.x, yz.y, w ) {}
    attr_always_inline attr_header constexpr
    explicit IVector4CPP( IVector2CPP xy, IVector2CPP zw ) : IVector4CPP( xy.x, xy.y, zw.x, zw.y ) {}
    attr_always_inline attr_header constexpr
    explicit IVector4CPP( IVector3CPP xyz, i32 w ) : IVector4CPP( xyz.xy, xyz.z, w ) {}
    attr_always_inline attr_header constexpr
    explicit IVector4CPP( i32 x, IVector3CPP yzw ) : IVector4CPP( x, yzw.xy, yzw.z ) {}

    attr_always_inline attr_header constexpr
    operator IVector4() const {
        return *(struct IVector4*)this;
    }

    attr_always_inline attr_header static constexpr
    IVector4CPP zero() {
        return IVector4CPP();
    }
    attr_always_inline attr_header static constexpr
    IVector4CPP one() {
        return IVector4CPP( 1 );
    }

    attr_always_inline attr_header static constexpr
    IVector4CPP from_array( const i32 array[4] ) {
        return *(IVector4CPP*)array;
    }
    attr_always_inline attr_header constexpr
    void to_array( i32 out_array[4] ) const {
        out_array[0] = array[0];
        out_array[1] = array[1];
        out_array[2] = array[2];
        out_array[3] = array[3];
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
attr_always_inline
attr_header Vector4CPP add(
    Vector4CPP lhs, Vector4CPP rhs
) {
    return vec4_add( lhs.pod, rhs.pod );
}
attr_always_inline
attr_header Vector4CPP sub(
    Vector4CPP lhs, Vector4CPP rhs
) {
    return vec4_sub( lhs.pod, rhs.pod );
}
attr_always_inline
attr_header Vector4CPP mul( Vector4CPP lhs, f32 rhs ) {
    return vec4_mul( lhs.pod, rhs );
}
attr_always_inline
attr_header Vector4CPP mul( f32 lhs, Vector4CPP rhs ) {
    return vec4_mul( rhs.pod, lhs );
}
attr_always_inline
attr_header Vector4CPP mul(
    Vector4CPP lhs, Vector4CPP rhs
) {
    return vec4_mul_vec4( lhs.pod, rhs.pod );
}
attr_always_inline
attr_header Vector4CPP hadamard(
    Vector4CPP lhs, Vector4CPP rhs
) {
    return mul(lhs, rhs);
}
attr_always_inline
attr_header Vector4CPP div( Vector4CPP lhs, f32 rhs ) {
    return vec4_div( lhs.pod, rhs );
}
attr_always_inline
attr_header Vector4CPP div( Vector4CPP lhs, Vector4CPP rhs ) {
    return vec4_div_vec4( lhs.pod, rhs.pod );
}
attr_always_inline
attr_header Vector4CPP mod( Vector4CPP lhs, f32 rhs ) {
    return vec4_mod( lhs.pod, rhs );
}
attr_always_inline
attr_header Vector4CPP mod( Vector4CPP lhs, Vector4CPP rhs ) {
    return vec4_mod_vec4( lhs.pod, rhs.pod );
}
attr_always_inline
attr_header Vector4CPP neg( Vector4CPP x ) {
    return vec4_neg( x.pod );
}
attr_always_inline
attr_header f32 hadd( Vector4CPP x ) {
    return vec4_hadd( x.pod );
}
attr_always_inline
attr_header f32 hmul( Vector4CPP x ) {
    return vec4_hmul( x.pod );
}
attr_always_inline
attr_header f32 dot( Vector4CPP lhs, Vector4CPP rhs ) {
    return vec4_dot( lhs.pod, rhs.pod );
}
attr_always_inline
attr_header f32 hmax( Vector4CPP x ) {
    return vec4_hmax( x.pod );
}
attr_always_inline
attr_header Vector4CPP max( Vector4CPP x, Vector4CPP y ) {
    return vec4_max( x.pod, y.pod );
}
attr_always_inline
attr_header f32 hmin( Vector4CPP x ) {
    return vec4_hmin( x.pod );
}
attr_always_inline
attr_header Vector4CPP min( Vector4CPP x, Vector4CPP y ) {
    return vec4_min( x.pod, y.pod );
}
attr_always_inline
attr_header f32 length_sqr( Vector4CPP x ) {
    return vec4_length_sqr( x.pod );
}
attr_always_inline
attr_header f32 length( Vector4CPP x ) {
    return vec4_length( x.pod );
}
attr_always_inline
attr_header f32 distance_sqr( Vector4CPP a, Vector4CPP b ) {
    return vec4_distance_sqr( a.pod, b.pod );
}
attr_always_inline
attr_header f32 distance( Vector4CPP a, Vector4CPP b ) {
    return vec4_distance( a.pod, b.pod );
}
attr_always_inline
attr_header Vector4CPP normalize( Vector4CPP x ) {
    return vec4_normalize( x.pod );
}
attr_always_inline
attr_header Vector4CPP clamp(
    Vector4CPP v, Vector4CPP min, Vector4CPP max
) {
    return vec4_clamp( v.pod, min.pod, max.pod );
}
attr_always_inline
attr_header Vector4CPP clamp(
    Vector4CPP v, f32 min, f32 max
) {
    return vec4_clamp_length( v.pod, min, max );
}
attr_header
Vector4CPP abs( Vector4CPP v ) {
    return vec4_abs( v.pod );
}
attr_header
Vector4CPP sign( Vector4CPP v ) {
    return vec4_sign( v.pod );
}
attr_header
Vector4CPP trunc( Vector4CPP v ) {
    return vec4_trunc( v.pod );
}
attr_header
Vector4CPP floor( Vector4CPP v ) {
    return vec4_floor( v.pod );
}
attr_header
Vector4CPP ceil( Vector4CPP v ) {
    return vec4_ceil( v.pod );
}
attr_header
Vector4CPP round( Vector4CPP v ) {
    return vec4_round( v.pod );
}
attr_header
Vector4CPP fract( Vector4CPP v ) {
    return vec4_fract( v.pod );
}
attr_always_inline
attr_header Vector4CPP lerp(
    Vector4CPP a, Vector4CPP b, f32 t
) {
    return vec4_lerp( a.pod, b.pod, t );
}
attr_always_inline
attr_header Vector4CPP mix(
    Vector4CPP a, Vector4CPP b, f32 t
) {
    return lerp( a, b, t );
}
attr_header
Vector4CPP step( Vector4CPP edge, Vector4CPP x ) {
    return vec4_step( edge.pod, x.pod );
}
attr_header
Vector4CPP step( f32 edge, Vector4CPP x ) {
    return vec4_step_scalar( edge, x.pod );
}
attr_header
Vector4CPP smoothstep( Vector4CPP edge0, Vector4CPP edge1, Vector4CPP x ) {
    return vec4_smoothstep( edge0.pod, edge1.pod, x.pod );
}
attr_header
Vector4CPP smoothstep( f32 edge0, f32 edge1, Vector4CPP x ) {
    return vec4_smoothstep_scalar( edge0, edge1, x.pod );
}
attr_header
Vector4CPP smootherstep( Vector4CPP edge0, Vector4CPP edge1, Vector4CPP x ) {
    return vec4_smootherstep( edge0.pod, edge1.pod, x.pod );
}
attr_header
Vector4CPP smootherstep( f32 edge0, f32 edge1, Vector4CPP x ) {
    return vec4_smootherstep_scalar( edge0, edge1, x.pod );
}
attr_always_inline attr_header
Vector4CPP radians( Vector4CPP degrees ) {
    return vec4_radians( degrees.pod );
}
attr_always_inline attr_header
Vector4CPP degrees( Vector4CPP radians ) {
    return vec4_degrees( radians.pod );
}
attr_always_inline attr_header
Vector4CPP sin( Vector4CPP angle ) {
    return vec4_sin( angle.pod );
}
attr_always_inline attr_header
Vector4CPP cos( Vector4CPP angle ) {
    return vec4_cos( angle.pod );
}
attr_always_inline attr_header
Vector4CPP tan( Vector4CPP angle ) {
    return vec4_tan( angle.pod );
}
attr_always_inline attr_header
Vector4CPP asin( Vector4CPP angle ) {
    return vec4_asin( angle.pod );
}
attr_always_inline attr_header
Vector4CPP acos( Vector4CPP angle ) {
    return vec4_acos( angle.pod );
}
attr_always_inline attr_header
Vector4CPP atan2( Vector4CPP y, Vector4CPP x ) {
    return vec4_atan2( y.pod, x.pod );
}
attr_always_inline attr_header
Vector4CPP pow( Vector4CPP base, Vector4CPP exp ) {
    return vec4_pow( base.pod, exp.pod );
}
attr_always_inline attr_header
Vector4CPP exp( Vector4CPP x ) {
    return vec4_exp( x.pod );
}
attr_always_inline attr_header
Vector4CPP ln( Vector4CPP x ) {
    return vec4_ln( x.pod );
}
attr_always_inline attr_header
Vector4CPP log2( Vector4CPP x ) {
    return vec4_log2( x.pod );
}
attr_always_inline attr_header
Vector4CPP sqrt( Vector4CPP x ) {
    return vec4_sqrt( x.pod );
}
attr_always_inline attr_header
Vector4CPP inversesqrt( Vector4CPP x ) {
    return vec4_inversesqrt( x.pod );
}
attr_always_inline
attr_header b32 cmp( Vector4CPP a, Vector4CPP b ) {
    return vec4_cmp( a.pod, b.pod );
}

attr_always_inline
attr_header Vector4CPP operator-( Vector4CPP v ) {
    return neg( v );
}
attr_always_inline
attr_header Vector4CPP& operator+=( Vector4CPP& lhs, Vector4CPP rhs ) {
    return lhs = add( lhs, rhs );
}
attr_always_inline
attr_header Vector4CPP& operator-=( Vector4CPP& lhs, Vector4CPP rhs ) {
    return lhs = sub( lhs, rhs );
}
attr_always_inline
attr_header Vector4CPP& operator*=( Vector4CPP& lhs, Vector4CPP rhs ) {
    return lhs = mul( lhs, rhs );
}
attr_always_inline
attr_header Vector4CPP& operator*=( Vector4CPP& lhs, f32 rhs ) {
    return lhs = mul( lhs, rhs );
}
attr_always_inline
attr_header Vector4CPP& operator/=( Vector4CPP& lhs, Vector4CPP rhs ) {
    return lhs = div( lhs, rhs );
}
attr_always_inline
attr_header Vector4CPP& operator/=( Vector4CPP& lhs, f32 rhs ) {
    return lhs = div( lhs, rhs );
}
attr_always_inline
attr_header Vector4CPP operator+( Vector4CPP lhs, Vector4CPP rhs ) {
    return add( lhs, rhs );
}
attr_always_inline
attr_header Vector4CPP operator-( Vector4CPP lhs, Vector4CPP rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline
attr_header Vector4CPP operator*( Vector4CPP lhs, Vector4CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline
attr_header Vector4CPP operator*( Vector4CPP lhs, f32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline
attr_header Vector4CPP operator*( f32 lhs, Vector4CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline
attr_header Vector4CPP operator/( Vector4CPP lhs, Vector4CPP rhs ) {
    return div( lhs, rhs );
}
attr_always_inline
attr_header Vector4CPP operator/( Vector4CPP lhs, f32 rhs ) {
    return div( lhs, rhs );
}
attr_always_inline
attr_header b32 operator==( Vector4CPP a, Vector4CPP b ) {
    return cmp( a, b );
}
attr_always_inline
attr_header b32 operator!=( Vector4CPP a, Vector4CPP b ) {
    return !( a == b );
}

attr_always_inline attr_header
IVector4CPP add( IVector4CPP lhs, IVector4CPP rhs ) {
    return ivec4_add( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector4CPP sub( IVector4CPP lhs, IVector4CPP rhs ) {
    return ivec4_sub( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector4CPP mul( IVector4CPP lhs, IVector4CPP rhs ) {
    return ivec4_mul_ivec4( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector4CPP mul( IVector4CPP lhs, i32 rhs ) {
    return ivec4_mul( lhs.pod, rhs );
}
attr_always_inline attr_header
IVector4CPP mul( i32 lhs, IVector4CPP rhs ) {
    return ivec4_mul( rhs.pod, lhs );
}
attr_always_inline attr_header
IVector4CPP hadamard( IVector4CPP lhs, IVector4CPP rhs ) {
    return ivec4_hadamard( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector4CPP div( IVector4CPP lhs, IVector4CPP rhs ) {
    return ivec4_div_ivec4( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector4CPP div( IVector4CPP lhs, i32 rhs ) {
    return ivec4_div( lhs.pod, rhs );
}
attr_always_inline attr_header
IVector4CPP mod( IVector4CPP lhs, IVector4CPP rhs ) {
    return ivec4_mod_ivec4( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
IVector4CPP mod( IVector4CPP lhs, i32 rhs ) {
    return ivec4_mod( lhs.pod, rhs );
}
attr_always_inline attr_header
IVector4CPP neg( IVector4CPP v ) {
    return ivec4_neg( v.pod );
}
attr_always_inline attr_header
i32 hadd( IVector4CPP v ) {
    return ivec4_hadd( v.pod );
}
attr_always_inline attr_header
i32 hmul( IVector4CPP v ) {
    return ivec4_hmul( v.pod );
}
attr_always_inline attr_header
f32 dot( IVector4CPP lhs, IVector4CPP rhs ) {
    return ivec4_dot( lhs.pod, rhs.pod );
}
attr_always_inline attr_header
f32 length_sqr( IVector4CPP v ) {
    return ivec4_length_sqr( v.pod );
}
attr_always_inline attr_header
f32 length( IVector4CPP v ) {
    return ivec4_length( v.pod );
}
attr_always_inline attr_header
IVector4CPP min( IVector4CPP x, IVector4CPP y ) {
    return ivec4_min( x.pod, y.pod );
}
attr_always_inline attr_header
IVector4CPP max( IVector4CPP x, IVector4CPP y ) {
    return ivec4_max( x.pod, y.pod );
}
attr_always_inline attr_header
IVector4CPP abs( IVector4CPP v ) {
    return ivec4_abs( v.pod );
}
attr_always_inline attr_header
IVector4CPP sign( IVector4CPP v ) {
    return ivec4_sign( v.pod );
}
attr_always_inline attr_header
b32 cmp( IVector4CPP a, IVector4CPP b ) {
    return ivec4_cmp( a.pod, b.pod );
}

attr_always_inline attr_header
IVector4CPP& operator+=( IVector4CPP& lhs, IVector4CPP rhs ) {
    return lhs = add( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP& operator-=( IVector4CPP& lhs, IVector4CPP rhs ) {
    return lhs = sub( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP& operator*=( IVector4CPP& lhs, IVector4CPP rhs ) {
    return lhs = mul( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP& operator*=( IVector4CPP& lhs, i32 rhs ) {
    return lhs = mul( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP& operator/=( IVector4CPP& lhs, IVector4CPP rhs ) {
    return lhs = div( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP& operator/=( IVector4CPP& lhs, i32 rhs ) {
    return lhs = div( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP& operator%=( IVector4CPP& lhs, IVector4CPP rhs ) {
    return lhs = mod( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP& operator%=( IVector4CPP& lhs, i32 rhs ) {
    return lhs = mod( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP operator+( IVector4CPP lhs, IVector4CPP rhs ) {
    return add( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP operator-( IVector4CPP lhs, IVector4CPP rhs ) {
    return sub( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP operator*( IVector4CPP lhs, IVector4CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP operator*( IVector4CPP lhs, i32 rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP operator*( i32 lhs, IVector4CPP rhs ) {
    return mul( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP operator/( IVector4CPP lhs, IVector4CPP rhs ) {
    return div( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP operator/( IVector4CPP lhs, i32 rhs ) {
    return div( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP operator%( IVector4CPP lhs, IVector4CPP rhs ) {
    return mod( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP operator%( IVector4CPP lhs, i32 rhs ) {
    return mod( lhs, rhs );
}
attr_always_inline attr_header
IVector4CPP operator-( IVector4CPP v ) {
    return neg( v );
}
attr_always_inline attr_header
b32 operator==( IVector4CPP a, IVector4CPP b ) {
    return cmp( a, b );
}
attr_always_inline attr_header
b32 operator!=( IVector4CPP a, IVector4CPP b ) {
    return !( a == b );
}


#endif /* header guard */
