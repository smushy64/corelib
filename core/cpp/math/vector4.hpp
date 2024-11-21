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
        struct { f32 x, y, z, w; };
        struct { f32 r, g, b, a; };
        struct Vector4 pod;

        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 0, 0> xx, rr;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 0, 1> xy, rg;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 0, 2> xz, rb;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 0, 3> xw, ra;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 1, 0> yx, gr;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 1, 1> yy, gg;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 1, 2> yz, gb;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 1, 3> yw, ga;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 2, 0> zx, br;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 2, 1> zy, bg;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 2, 2> zz, bb;
        SwizzlerConvert<Vector4CPP, Vector2CPP, f32, 2, 3> zw, ba;
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
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 0, 1, 2> xyz, rgb;
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
        SwizzlerConvert<Vector4CPP, Vector3CPP, f32, 1, 2, 3> yzw, gba;
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

    attr_always_inline attr_header
    Vector4CPP() : x(0), y(0), z(0), w(0) {}
    attr_always_inline attr_header
    Vector4CPP( const struct Vector4& v ) : x(v.x), y(v.y), z(v.z), w(v.w) {}
    attr_always_inline attr_header
    explicit Vector4CPP( f32 s ) : x(s), y(s), z(s), w(s) {}
    attr_always_inline attr_header
    explicit Vector4CPP( f32 x, f32 y, f32 z, f32 w ) : x(x), y(y), z(z), w(w) {}
    attr_always_inline attr_header
    explicit Vector4CPP( Vector2CPP xy, f32 z, f32 w ) : Vector4CPP( xy.x, xy.y, z, w ) {}
    attr_always_inline attr_header
    explicit Vector4CPP( f32 x, f32 y, Vector2CPP zw ) : Vector4CPP( x, y, zw.x, zw.y ) {}
    attr_always_inline attr_header
    explicit Vector4CPP( f32 x, Vector2CPP yz, f32 w ) : Vector4CPP( x, yz.x, yz.y, w ) {}
    attr_always_inline attr_header
    explicit Vector4CPP( Vector2CPP xy, Vector2CPP zw ) : Vector4CPP( xy.x, xy.y, zw.x, zw.y ) {}
    attr_always_inline attr_header
    explicit Vector4CPP( Vector3CPP xyz, f32 w ) : Vector4CPP( xyz.xy, xyz.z, w ) {}
    attr_always_inline attr_header
    explicit Vector4CPP( f32 x, Vector3CPP yzw ) : Vector4CPP( x, yzw.xy, yzw.z ) {}

    attr_always_inline attr_header
    operator Vector4() const {
        return *(struct Vector4*)this;
    }

    attr_always_inline attr_header static
    Vector4CPP zero() {
        return Vector4CPP();
    }
    attr_always_inline attr_header static
    Vector4CPP one() {
        return Vector4CPP( 1.0, 1.0, 1.0, 1.0 );
    }

    attr_always_inline attr_header static
    Vector4CPP red() {
        return Vector4CPP( 1.0, 0.0, 0.0, 1.0 );
    }
    attr_always_inline attr_header static
    Vector4CPP green() {
        return Vector4CPP( 0.0, 1.0, 0.0, 1.0 );
    }
    attr_always_inline attr_header static
    Vector4CPP blue() {
        return Vector4CPP( 0.0, 0.0, 1.0, 1.0 );
    }
    attr_always_inline attr_header static
    Vector4CPP yellow() {
        return Vector4CPP( 1.0, 1.0, 0.0, 1.0 );
    }
    attr_always_inline attr_header static
    Vector4CPP magenta() {
        return Vector4CPP( 1.0, 0.0, 1.0, 1.0 );
    }
    attr_always_inline attr_header static
    Vector4CPP cyan() {
        return Vector4CPP( 0.0, 1.0, 1.0, 1.0 );
    }
    attr_always_inline attr_header static
    Vector4CPP black() {
        return Vector4CPP( 0.0, 0.0, 0.0, 1.0 );
    }
    attr_always_inline attr_header static
    Vector4CPP white() {
        return Vector4CPP( 1.0, 1.0, 1.0, 1.0 );
    }
    attr_always_inline attr_header static
    Vector4CPP clear() {
        return Vector4CPP( 0.0, 0.0, 0.0, 0.0 );
    }

    attr_always_inline attr_header static
    Vector4CPP from_array( const f32 array[4] ) {
        return *(Vector4CPP*)array;
    }
    attr_always_inline attr_header
    void to_array( f32 out_array[4] ) const {
        out_array[0] = array[0];
        out_array[1] = array[1];
        out_array[2] = array[2];
        out_array[3] = array[3];
    }

    attr_always_inline attr_header
    f32 operator[]( usize idx ) const {
        return array[idx];
    }
    attr_always_inline attr_header
    f32& operator[]( usize idx ) {
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
attr_always_inline
attr_header Vector4CPP smoothstep(
    Vector4CPP a, Vector4CPP b, f32 t
) {
    return vec4_smoothstep( a.pod, b.pod, t );
}
attr_always_inline
attr_header Vector4CPP smootherstep(
    Vector4CPP a, Vector4CPP b, f32 t
) {
    return vec4_smootherstep( a.pod, b.pod, t );
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

#endif /* header guard */
