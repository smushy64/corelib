/**
 * Description:  Math Implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 11, 2024
*/
#include "core/defines.h" // IWYU pragma: keep

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    #include "core/internal/sse.h" // IWYU pragma: keep
#elif defined(CORE_ENABLE_AVX_INSTRUCTIONS)
    /// MMX, SSE, AVX, AVX2
    #include <immintrin.h>
#endif

#include "core/types.h"
#include "core/attributes.h"
#include "core/constants.h"
#include "core/math.h"
#include "core/memory.h"

#if !defined(CORE_ARCH_X86)
    #warning "sine_cosine is not yet implemented!"
    #define INTERNAL_CORE_SINE_COSINE_NOT_IMPLEMENTED
#endif

attr_unused
attr_internal attr_always_inline inline
f32 internal_square_root_scalar( f32 x ) {
    if( x < 0.0f ) {
        return F32_NAN;
    }
    if( x == 0.0f ) {
        return 0.0f;
    }

    /// NOTE(alicia): this is probably
    /// slower than acceptable but you really
    /// shouldn't be compiling without SIMD support
    f32 result = x / 2.0f;
    for( usize i = 0; i < 50; ++i ) {
        result = ( result + (x / result) ) / 2.0f;
    }
    return result;
}
attr_unused
attr_internal attr_always_inline inline
f32 internal_inv_square_root_scalar( f32 x ) {
    return 1.0f / internal_square_root_scalar( x );
}

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)

attr_internal attr_always_inline inline
f32 internal_square_root_sse( f32 x ) {
    return _mm_cvtss_f32( _mm_sqrt_ss( _mm_set_ss( x ) ) );
}
attr_internal attr_always_inline inline
f32 internal_inv_square_root_sse( f32 x ) {
    return _mm_cvtss_f32( _mm_rsqrt_ss( _mm_set_ss( x ) ) );
}

#endif /* SSE functions */ 

attr_core_api f32 square_root( f32 x ) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_square_root_sse( x );
#else
    return internal_square_root_scalar( x );
#endif
}

attr_core_api f32 inv_square_root( f32 x ) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_inv_square_root_sse( x );
#else
    return internal_inv_square_root_scalar( x );
#endif
}

attr_core_api f32 nat_log( f32 x ) {
    if( x < 0.0f ) {
        return F32_NAN;
    }

    f32 div = ( x - 1.0f ) / ( x + 1.0f );

    f32 p3 = div * div * div;
    f32 p5 = div * div * div * div * div;
    f32 p7 = div * div * div * div * div * div * div;
    f32 p9 = div * div * div * div * div * div * div * div * div;

    f32 r3 = ( 1.0f / 3.0f ) * p3;
    f32 r5 = ( 1.0f / 5.0f ) * p5;
    f32 r7 = ( 1.0f / 7.0f ) * p7;
    f32 r9 = ( 1.0f / 9.0f ) * p9;
    
    return 2.0f * (div + r3 + r5 + r7 + r9);
}

attr_core_api f32 log_2( f32 x ) {
    if( x < 0.0f ) {
        return F32_NAN;
    }
    if( x == 2.0f ) {
        return 1.0f;
    }
    return nat_log( x ) * 1.49f;
}
attr_core_api f32 log_10( f32 x ) {
    if( x < 0.0f ) {
        return F32_NAN;
    }
    if( x == 10.0f ) {
        return 1.0f;
    }
    return nat_log( x ) / 2.3f;
}

attr_core_api f32 poweri( f32 base, i32 exp ) {
    u32 exp_abs = absolute( exp );
    f32 result  = base;
    for( u32 i = 1; i < exp_abs; ++i ) {
        result *= base;
    }
    if( exp < 0 ) {
        return 1.0f / result;
    } else {
        return result;
    }
}
attr_core_api f32 e_power( f32 x ) {
    if( x < -4.0f ) {
        return 0.0f;
    }

    // NOTE(alicia): preventing sequential operations
    f32 p2  = x * x;
    f32 p3  = x * x * x;
    f32 p4  = x * x * x * x;
    f32 p5  = x * x * x * x * x;
    f32 p6  = x * x * x * x * x * x;
    f32 p7  = x * x * x * x * x * x * x;
    f32 p8  = x * x * x * x * x * x * x * x;
    f32 p9  = x * x * x * x * x * x * x * x * x;
    f32 p10 = x * x * x * x * x * x * x * x * x * x;
    f32 p11 = x * x * x * x * x * x * x * x * x * x * x;

    f32 r = 1 + x +
        p2  / (F32_TWO_FACTORIAL)   +
        p3  / (F32_THREE_FACTORIAL) +
        p4  / (F32_FOUR_FACTORIAL)  +
        p5  / (F32_FIVE_FACTORIAL)  +
        p6  / (F32_SIX_FACTORIAL)   +
        p7  / (F32_SEVEN_FACTORIAL) +
        p8  / (F32_EIGHT_FACTORIAL) +
        p9  / (F32_NINE_FACTORIAL)  +
        p10 / (F32_TEN_FACTORIAL)   +
        p11 / (F32_ELEVEN_FACTORIAL);

    return r;
}
#define internal_floor( x )\
    (((x) > 0.0f) ? (i32)(x) : (i32)( (x) - 0.99999f ))

attr_core_api f32 mod( f32 lhs, f32 rhs ) {
    if( rhs == 0.0f ) {
        return lhs;
    }

    f32 lhs_div_rhs = lhs / rhs;
    f32 m = lhs - ( rhs * (f32)internal_floor( lhs_div_rhs ) );

    if( rhs > 0.0f ) {
        if( m >= rhs ) {
            return 0.0f;
        }

        if( m < 0.0f ) {
            if( (rhs + m) == rhs ) {
                return 0.0f;
            } else {
                return rhs + m;
            }
        }
    } else {
        if( m <= rhs ) {
            return 0.0f;
        }
        if( m > 0.0f ) {
            if( ( rhs + m ) == rhs ) {
                return 0.0f;
            } else {
                return rhs + m;
            }
        }
    }

    return m;
}

attr_core_api f32 wrap_deg( f32 deg ) {
    f32 result = mod( deg, 360.0f );
    return result < 0.0f ? result + 360.0f : result;
}
attr_core_api f32 wrap_rad( f32 rad ) {
    return mod( rad + F32_PI, F32_TAU ) - F32_PI;
}

attr_core_api f32 sine( f32 x ) {
#if defined(INTERNAL_CORE_SINE_COSINE_NOT_IMPLEMENTED)
    x = wrap_rad(x);

    f32 pow2  = x * x;
    f32 pow3  = pow2 * x;
    f32 pow5  = pow3 * pow2;
    f32 pow7  = pow5 * pow2;
    f32 pow9  = pow7 * pow2;
    f32 pow11 = pow9 * pow2;

    return x -
        ( pow3  / F32_THREE_FACTORIAL  ) +
        ( pow5  / F32_FIVE_FACTORIAL   ) -
        ( pow7  / F32_SEVEN_FACTORIAL  ) +
        ( pow9  / F32_NINE_FACTORIAL   ) -
        ( pow11 / F32_ELEVEN_FACTORIAL );
#else
    f32 res, c;
    sine_cosine( x, &res, &c );
    return res;
#endif
}
attr_core_api f32 cosine( f32 x ) {
#if defined(INTERNAL_CORE_SINE_COSINE_NOT_IMPLEMENTED)
    x = wrap_rad(x);

    f32 pow2  = x * x;
    f32 pow4  = pow2 * pow2;
    f32 pow6  = pow4 * pow2;
    f32 pow8  = pow6 * pow2;
    f32 pow10 = pow8 * pow2;

    return 1.0f -
        ( pow2  / F32_TWO_FACTORIAL   ) +
        ( pow4  / F32_FOUR_FACTORIAL  ) -
        ( pow6  / F32_SIX_FACTORIAL   ) +
        ( pow8  / F32_EIGHT_FACTORIAL ) -
        ( pow10 / F32_TEN_FACTORIAL   );
#else
    f32 res, s;
    sine_cosine( x, &s, &res );
    return res;
#endif
}

attr_unused attr_internal attr_always_inline inline
void internal_sine_cosine( f32 x, f32* out_sine, f32* out_cosine ) {
    *out_sine   = sine(x);
    *out_cosine = cosine(x);
}

#if defined(CORE_ARCH_X86)

#if defined(CORE_COMPILER_MSVC)

#pragma message ("internal_sine_cosine_fsincos is not yet implemented in MASM!")
// extern void internal_sine_cosine_fsincos( f32 x, f32* out_sine, f32* out_cosine );

void internal_sine_cosine_fsincos( f32 x, f32* out_sine, f32* out_cosine ) {
    internal_sine_cosine( x, out_sine, out_cosine );
}

#else /* MSVC */

attr_internal
attr_always_inline inline attr_optimized
void internal_sine_cosine_fsincos( f32 x, f32* out_sine, f32* out_cosine ) {
    f32 s, c;
    __asm__ inline ( "fsincos" : "=t"(c), "=u"(s) : "0"(x) );
    *out_sine   = s;
    *out_cosine = c;
}

#endif /* GCC || CLANG */

#endif /* x86 fsincos */

attr_core_api void sine_cosine( f32 x, f32* out_sine, f32* out_cosine ) {
#if defined(CORE_ARCH_X86)
    internal_sine_cosine_fsincos( x, out_sine, out_cosine );
#else
    internal_sine_cosine( x, out_sine, out_cosine );
#endif
}

attr_core_api f32 arc_sine( f32 x ) {
    // NOTE(alicia): don't ask me how i figured this out
    // i don't even know
    f32 sign_of_x = signum( x );
    f32 x_abs = x * sign_of_x;
    f32 x_sqr = x_abs * x_abs;

    const f32 magic_0 =  1.5707288f;
    const f32 magic_1 = -0.2121144f;
    const f32 magic_2 =  0.0742610f;
    const f32 magic_3 = -0.0187293f;

    f32 result = F32_HALF_PI - square_root( 1.0f - x_abs ) * (
        magic_0 +
        ( magic_1 * x_abs ) +
        ( magic_2 * x_sqr ) +
        ( magic_3 * ( x_sqr * x_abs ) )
    );

    return result * sign_of_x;
}
attr_core_api f32 arc_tangent( f32 x ) {
    // NOTE(alicia): preventing sequential operations
    f32 p3  = x * x * x;
    f32 p5  = x * x * x * x * x;
    f32 p7  = x * x * x * x * x * x * x;
    f32 p9  = x * x * x * x * x * x * x * x * x;
    f32 p11 = x * x * x * x * x * x * x * x * x * x * x;
    f32 p13 = x * x * x * x * x * x * x * x * x * x * x * x * x;

    return x -
        ( p3  / 3.0f ) +
        ( p5  / 5.0f ) -
        ( p7  / 7.0f ) +
        ( p9  / 9.0f ) -
        ( p11 / 11.0f ) +
        ( p13 / 13.0f );
}
attr_core_api f32 arc_tangent2( f32 y, f32 x ) {
    if( y == 0.0f ) {
        if( x < 0.0f ) {
            return F32_PI;
        } else if( x == 0.0f ) {
            return F32_NAN;
        }
    }

    f32 x_sqr = x * x;
    f32 y_sqr = y * y;
    return 2.0f * arc_tangent( y / ( square_root( x_sqr + y_sqr ) + x ) );
}

attr_core_api struct Vector3 rgb_to_hsl( struct Vector3 _rgb ) {
    f32 R = _rgb.r;
    f32 G = _rgb.g;
    f32 B = _rgb.b;

    u32 X_max_i = 0;
    if( R > G ) {
        if( R < B ) {
            X_max_i = 2;
        }
    } else {
        if( G > B ) {
            X_max_i = 1;
        } else {
            X_max_i = 2;
        }
    }

    f32 X_max = _rgb.v[X_max_i];
    f32 X_min = v3_min( _rgb );

    f32 C = X_max - X_min;

    f32 H = 0.0f;
    f32 S = 0.0f;
    f32 L = ( X_max + X_min ) / 2.0f;

    if( !fcmp( C, 0.0f ) ) {
        f32 segment = 0.0f, shift = 0.0f;
        switch( X_max_i ) {
            case 0: { // R
                segment = (G - B) / C;
                shift   = 0.0f;
                if( segment < 0.0f ) {
                    shift = 360.0f / 60.0f;
                }
            } break;
            case 1: { // G
                segment = (B - R) / C;
                shift   = 120.0f / 60.0f;
            } break;
            case 2: { // B
                segment = (R - G) / C;
                shift   = 240.0f / 60.0f;
            } break;
            default: break;
        }
        H = ( segment + shift ) * 60.0f;
        f32 d = 2.0f * L - 1.0f;
        S = C / ( 1.0f - absolute( d ) );
    }

    return v3( H, S, L );
}
attr_core_api struct Vector3 hsl_to_rgb( struct Vector3 _hsl ) {
    f32 H = _hsl.h;
    f32 S = _hsl.s;
    f32 L = _hsl.l;

    f32 a = S * min(L, 1.0f - L);

    #define k(n) mod( n + (H / 30.0f), 12.0f )
    #define f(n) L - ( a * max( -1.0f, min( min( k(n) - 3.0f, 9.0f - k(n) ), 1.0f ) ) )

    return rgb( f(0.0f), f(8.0f), f(4.0f) );

    #undef k
    #undef f
}

attr_unused
attr_always_inline inline
attr_internal struct Quaternion internal_q_mul_q_scalar(
     struct Quaternion lhs, struct Quaternion rhs   
) {
    return q( ( lhs.v[0] * rhs.v[0] ) -
        ( lhs.v[1] * rhs.v[1] + lhs.v[2] * rhs.v[2] + lhs.v[3] * rhs.v[3] ),
    ( lhs.v[0] * rhs.v[1] ) +
        ( rhs.v[0] * lhs.v[1] ) +
        ( ( lhs.v[2] * rhs.v[3] ) -
        ( lhs.v[3] * rhs.v[2] ) ),
    ( lhs.v[0] * rhs.v[2] ) +
        ( rhs.v[0] * lhs.v[2] ) +
        ( ( lhs.v[3] * rhs.v[1] ) -
        ( lhs.v[1] * rhs.v[3] ) ),
    ( lhs.v[0] * rhs.v[3] ) +
        ( rhs.v[0] * lhs.v[3] ) +
        ( ( lhs.v[1] * rhs.v[2] ) -
        ( lhs.v[2] * rhs.v[1] ) )  );
}

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)

attr_internal struct Quaternion internal_q_mul_q_sse(
    struct Quaternion lhs, struct Quaternion rhs
) {
    const __m128 negate_first128 = _mm_setr_ps( -1.0f, 1.0f, 1.0f, 1.0f );

    __m128 a, b, c, d, w;
    w = _mm_set1_ps( lhs.w );

    a = _mm_setr_ps( lhs.x, lhs.x, lhs.y, lhs.z );
    b = _mm_setr_ps( rhs.x, rhs.w, rhs.w, rhs.w );
    a = _mm_mul_ps( a, b );

    b = _mm_setr_ps( lhs.y, lhs.y, lhs.z, lhs.x );
    c = _mm_setr_ps( rhs.y, rhs.z, rhs.x, rhs.y );
    b = _mm_mul_ps( b, c );

    c = _mm_setr_ps( lhs.z, lhs.z, lhs.x, lhs.y );
    d = _mm_setr_ps( rhs.z, rhs.y, rhs.z, rhs.x );
    c = _mm_mul_ps( c, d );

    c = _mm_mul_ps( negate_first128, c );

    d = _mm_sub_ps( b, c );
    a = _mm_add_ps( a, d );

    b = _mm_loadu_ps( rhs.v );
    w = _mm_mul_ps( w, b );

    attr_align(16) struct Quaternion result;
    _mm_store_ps( result.v, _mm_add_ps( w, a ) );
    return result;
}

#endif

attr_core_api struct Quaternion q_mul_q(
    struct Quaternion lhs, struct Quaternion rhs
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_q_mul_q_sse( lhs, rhs );
#else
    return internal_q_mul_q_scalar( lhs, rhs );
#endif
}
attr_core_api struct Vector3 q_mul_v3(
    struct Quaternion lhs, struct Vector3 rhs
) {
    vec3 t = v3_mul( v3_cross( lhs.xyz, rhs ), 2.0f );
    return v3_add( v3_add( rhs, v3_mul( t, lhs.w ) ), v3_cross( lhs.xyz, t ) );
}
attr_core_api struct Quaternion q_slerp(
    struct Quaternion a, struct Quaternion b, f32 t
) {
    struct Quaternion _b = b;

    f32 theta     = q_dot( a, b );
    f32 cos_theta = cosine( theta );

    if( cos_theta < 0.0f ) {
        _b        = q_neg( _b );
        cos_theta = -cos_theta;
    }

    if( cos_theta > ( 1.0f - F32_EPSILON ) ) {
        return q_lerp( a, b, t );
    } else {
        return q_normalize(
            q_div (
                q_add(
                    q_mul( a, sine( (1.0f - t) * theta ) ),
                    q_mul( _b, sine( t * theta ) )
                ),
                sine( theta )
            ) );
    }
}
attr_core_api struct Quaternion q_from_angle_axis(
    struct AngleAxis_ a
) {
    f32 half_angle = a.angle / 2.0f;
    f32 sin, cos;
    sine_cosine( half_angle, &sin, &cos );
    struct Quaternion result;
    result.w   = cos;
    result.xyz = v3_mul( a.axis, sin );
    return q_normalize( result );
}
attr_core_api struct Quaternion q_from_euler( f32 x, f32 y, f32 z ) {
    f32 hx = x / 2.0f;
    f32 hy = y / 2.0f;
    f32 hz = z / 2.0f;

    f32 x_sin, y_sin, z_sin;
    f32 x_cos, y_cos, z_cos;

    sine_cosine( hx, &x_sin, &x_cos );
    sine_cosine( hy, &y_sin, &y_cos );
    sine_cosine( hz, &z_sin, &z_cos );

    f32 xyz_sin = x_sin * y_sin * z_sin;
    f32 xyz_cos = x_cos * y_cos * z_cos;

    struct Quaternion result;

    result.w = xyz_cos + xyz_sin;
    result.x = ( x_sin * y_cos * z_cos ) + ( x_cos * y_sin * z_sin );
    result.y = ( x_cos * y_sin * z_cos ) + ( x_sin * y_cos * z_sin );
    result.z = ( x_cos * y_cos * z_sin ) + ( x_sin * y_sin * z_cos );

    return result;
}
attr_core_api struct Vector3 q_to_euler( struct Quaternion q ) {
    struct Vector3 result;

    result.x = arc_tangent2(
        2.0f * ((q.w * q.x) + (q.y * q.z)),
        1.0f - 2.0f * ((q.x * q.x) + (q.y * q.y)) );

    result.y = arc_sine_real( 2.0f * ((q.w * q.y) - (q.z * q.x)) );

    result.z = arc_tangent2(
        2.0f * ((q.w * q.z) + (q.x * q.y)),
        1.0f - 2.0f * ((q.y * q.y) + (q.z * q.z)) );
    return result;
}
attr_core_api struct AngleAxis_ q_to_angle_axis( struct Quaternion q ) {
    struct AngleAxis_ result;
    
    result.angle = arc_cosine( q.w ) * 2.0f;
    result.axis  = v3_div( q.xyz, square_root( 1.0f - ( q.w * q.w ) ) );

    return result;
}

attr_unused
attr_optimized attr_internal
attr_always_inline inline
struct Matrix4x4 internal_m4_mul_m4_scalar( const struct Matrix4x4* lhs, const struct Matrix4x4* rhs ) {
    return m4(
        // column - 0
        ( lhs->v[0] * rhs->v[0] ) + ( lhs->v[4] * rhs->v[1] ) + ( lhs->v[8]  * rhs->v[2] ) + ( lhs->v[12] * rhs->v[3] ),
        ( lhs->v[1] * rhs->v[0] ) + ( lhs->v[5] * rhs->v[1] ) + ( lhs->v[9]  * rhs->v[2] ) + ( lhs->v[13] * rhs->v[3] ),
        ( lhs->v[2] * rhs->v[0] ) + ( lhs->v[6] * rhs->v[1] ) + ( lhs->v[10] * rhs->v[2] ) + ( lhs->v[14] * rhs->v[3] ),
        ( lhs->v[3] * rhs->v[0] ) + ( lhs->v[7] * rhs->v[1] ) + ( lhs->v[11] * rhs->v[2] ) + ( lhs->v[15] * rhs->v[3] ),
        // column - 1
        ( lhs->v[0] * rhs->v[4] ) + ( lhs->v[4] * rhs->v[5] ) + ( lhs->v[8]  * rhs->v[6] ) + ( lhs->v[12] * rhs->v[7] ),
        ( lhs->v[1] * rhs->v[4] ) + ( lhs->v[5] * rhs->v[5] ) + ( lhs->v[9]  * rhs->v[6] ) + ( lhs->v[13] * rhs->v[7] ),
        ( lhs->v[2] * rhs->v[4] ) + ( lhs->v[6] * rhs->v[5] ) + ( lhs->v[10] * rhs->v[6] ) + ( lhs->v[14] * rhs->v[7] ),
        ( lhs->v[3] * rhs->v[4] ) + ( lhs->v[7] * rhs->v[5] ) + ( lhs->v[11] * rhs->v[6] ) + ( lhs->v[15] * rhs->v[7] ),
        // column - 2
        ( lhs->v[0] * rhs->v[8] ) + ( lhs->v[4] * rhs->v[9] ) + ( lhs->v[8]  * rhs->v[10] ) + ( lhs->v[12] * rhs->v[11] ),
        ( lhs->v[1] * rhs->v[8] ) + ( lhs->v[5] * rhs->v[9] ) + ( lhs->v[9]  * rhs->v[10] ) + ( lhs->v[13] * rhs->v[11] ),
        ( lhs->v[2] * rhs->v[8] ) + ( lhs->v[6] * rhs->v[9] ) + ( lhs->v[10] * rhs->v[10] ) + ( lhs->v[14] * rhs->v[11] ),
        ( lhs->v[3] * rhs->v[8] ) + ( lhs->v[7] * rhs->v[9] ) + ( lhs->v[11] * rhs->v[10] ) + ( lhs->v[15] * rhs->v[11] ),
        // column - 3
        ( lhs->v[0] * rhs->v[12] ) + ( lhs->v[4] * rhs->v[13] ) + ( lhs->v[8]  * rhs->v[14] ) + ( lhs->v[12] * rhs->v[15] ),
        ( lhs->v[1] * rhs->v[12] ) + ( lhs->v[5] * rhs->v[13] ) + ( lhs->v[9]  * rhs->v[14] ) + ( lhs->v[13] * rhs->v[15] ),
        ( lhs->v[2] * rhs->v[12] ) + ( lhs->v[6] * rhs->v[13] ) + ( lhs->v[10] * rhs->v[14] ) + ( lhs->v[14] * rhs->v[15] ),
        ( lhs->v[3] * rhs->v[12] ) + ( lhs->v[7] * rhs->v[13] ) + ( lhs->v[11] * rhs->v[14] ) + ( lhs->v[15] * rhs->v[15] ) );
}

attr_unused
attr_optimized attr_internal
attr_always_inline inline
struct Matrix4x4 internal_m4_add_scalar(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
    struct Matrix4x4 res;
    res.col0 = v4_add( lhs->col0, rhs->col0 );
    res.col1 = v4_add( lhs->col1, rhs->col1 );
    res.col2 = v4_add( lhs->col2, rhs->col2 );
    res.col3 = v4_add( lhs->col3, rhs->col3 );
    return res;
}
attr_unused
attr_optimized attr_internal
attr_always_inline inline
struct Matrix4x4 internal_m4_sub_scalar(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
    struct Matrix4x4 res;
    res.col0 = v4_sub( lhs->col0, rhs->col0 );
    res.col1 = v4_sub( lhs->col1, rhs->col1 );
    res.col2 = v4_sub( lhs->col2, rhs->col2 );
    res.col3 = v4_sub( lhs->col3, rhs->col3 );
    return res;
}
attr_unused
attr_optimized attr_internal
attr_always_inline inline
struct Matrix4x4 internal_m4_mul_scalar(
    const struct Matrix4x4* lhs, f32 rhs
) {
    struct Matrix4x4 res;
    res.col0 = v4_mul( lhs->col0, rhs );
    res.col1 = v4_mul( lhs->col1, rhs );
    res.col2 = v4_mul( lhs->col2, rhs );
    res.col3 = v4_mul( lhs->col3, rhs );
    return res;
}
attr_unused
attr_optimized attr_internal
attr_always_inline inline
struct Matrix4x4 internal_m4_div_scalar(
    const struct Matrix4x4* lhs, f32 rhs
) {
    struct Matrix4x4 res;
    res.col0 = v4_div( lhs->col0, rhs );
    res.col1 = v4_div( lhs->col1, rhs );
    res.col2 = v4_div( lhs->col2, rhs );
    res.col3 = v4_div( lhs->col3, rhs );
    return res;
}

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)

attr_optimized attr_internal
attr_always_inline inline
struct Matrix4x4 internal_m4_add_sse(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
    __m128 lhs0 = _mm_loadu_ps( lhs->col0.v );
    __m128 lhs1 = _mm_loadu_ps( lhs->col1.v );
    __m128 lhs2 = _mm_loadu_ps( lhs->col2.v );
    __m128 lhs3 = _mm_loadu_ps( lhs->col3.v );

    __m128 rhs0 = _mm_loadu_ps( rhs->col0.v );
    __m128 rhs1 = _mm_loadu_ps( rhs->col1.v );
    __m128 rhs2 = _mm_loadu_ps( rhs->col2.v );
    __m128 rhs3 = _mm_loadu_ps( rhs->col3.v );

    __m128 res0 = _mm_add_ps( lhs0, rhs0 );
    __m128 res1 = _mm_add_ps( lhs1, rhs1 );
    __m128 res2 = _mm_add_ps( lhs2, rhs2 );
    __m128 res3 = _mm_add_ps( lhs3, rhs3 );

    struct Matrix4x4 res;
    _mm_storeu_ps( res.col0.v, res0 );
    _mm_storeu_ps( res.col1.v, res1 );
    _mm_storeu_ps( res.col2.v, res2 );
    _mm_storeu_ps( res.col3.v, res3 );

    return res;
}
attr_optimized attr_internal
attr_always_inline inline
struct Matrix4x4 internal_m4_sub_sse(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
    __m128 lhs0 = _mm_loadu_ps( lhs->col0.v );
    __m128 lhs1 = _mm_loadu_ps( lhs->col1.v );
    __m128 lhs2 = _mm_loadu_ps( lhs->col2.v );
    __m128 lhs3 = _mm_loadu_ps( lhs->col3.v );

    __m128 rhs0 = _mm_loadu_ps( rhs->col0.v );
    __m128 rhs1 = _mm_loadu_ps( rhs->col1.v );
    __m128 rhs2 = _mm_loadu_ps( rhs->col2.v );
    __m128 rhs3 = _mm_loadu_ps( rhs->col3.v );

    __m128 res0 = _mm_sub_ps( lhs0, rhs0 );
    __m128 res1 = _mm_sub_ps( lhs1, rhs1 );
    __m128 res2 = _mm_sub_ps( lhs2, rhs2 );
    __m128 res3 = _mm_sub_ps( lhs3, rhs3 );

    struct Matrix4x4 res;
    _mm_storeu_ps( res.col0.v, res0 );
    _mm_storeu_ps( res.col1.v, res1 );
    _mm_storeu_ps( res.col2.v, res2 );
    _mm_storeu_ps( res.col3.v, res3 );

    return res;
}
attr_optimized attr_internal
attr_always_inline inline
struct Matrix4x4 internal_m4_mul_sse(
    const struct Matrix4x4* lhs, f32 rhs
) {
    __m128 lhs0 = _mm_loadu_ps( lhs->col0.v );
    __m128 lhs1 = _mm_loadu_ps( lhs->col1.v );
    __m128 lhs2 = _mm_loadu_ps( lhs->col2.v );
    __m128 lhs3 = _mm_loadu_ps( lhs->col3.v );

    __m128 _rhs = _mm_set1_ps( rhs );

    __m128 res0 = _mm_mul_ps( lhs0, _rhs );
    __m128 res1 = _mm_mul_ps( lhs1, _rhs );
    __m128 res2 = _mm_mul_ps( lhs2, _rhs );
    __m128 res3 = _mm_mul_ps( lhs3, _rhs );

    struct Matrix4x4 res;
    _mm_storeu_ps( res.col0.v, res0 );
    _mm_storeu_ps( res.col1.v, res1 );
    _mm_storeu_ps( res.col2.v, res2 );
    _mm_storeu_ps( res.col3.v, res3 );

    return res;
}
attr_optimized attr_internal
attr_always_inline inline
struct Matrix4x4 internal_m4_div_sse(
    const struct Matrix4x4* lhs, f32 rhs
) {
    __m128 lhs0 = _mm_loadu_ps( lhs->col0.v );
    __m128 lhs1 = _mm_loadu_ps( lhs->col1.v );
    __m128 lhs2 = _mm_loadu_ps( lhs->col2.v );
    __m128 lhs3 = _mm_loadu_ps( lhs->col3.v );

    __m128 _rhs = _mm_set1_ps( rhs );

    __m128 res0 = _mm_div_ps( lhs0, _rhs );
    __m128 res1 = _mm_div_ps( lhs1, _rhs );
    __m128 res2 = _mm_div_ps( lhs2, _rhs );
    __m128 res3 = _mm_div_ps( lhs3, _rhs );

    struct Matrix4x4 res;
    _mm_storeu_ps( res.col0.v, res0 );
    _mm_storeu_ps( res.col1.v, res1 );
    _mm_storeu_ps( res.col2.v, res2 );
    _mm_storeu_ps( res.col3.v, res3 );

    return res;
}

attr_optimized attr_internal
attr_always_inline inline
struct Matrix4x4 internal_m4_mul_m4_sse(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
    struct Matrix4x4 res = MAT4_ZERO;

    // hand unrolled rows and columns

    __m128 col0_mul0    = _mm_loadu_ps( (f32*)lhs + (0 * 4) );
    __m128 col0_mul1    = _mm_set1_ps( *((f32*)rhs + ((0 * 4) + 0)) );
    __m128 col0_addend0 = _mm_mul_ps( col0_mul0, col0_mul1 );

    __m128 col0_mul2    = _mm_loadu_ps( (f32*)lhs + (1 * 4) );
    __m128 col0_mul3    = _mm_set1_ps( *((f32*)rhs + ((0 * 4) + 1)) );
    __m128 col0_addend1 = _mm_mul_ps( col0_mul2, col0_mul3 );

    __m128 col0_mul4    = _mm_loadu_ps( (f32*)lhs + (2 * 4) );
    __m128 col0_mul5    = _mm_set1_ps( *((f32*)rhs + ((0 * 4) + 2)) );
    __m128 col0_addend2 = _mm_mul_ps( col0_mul4, col0_mul5 );

    __m128 col0_mul6    = _mm_loadu_ps( (f32*)lhs + (3 * 4) );
    __m128 col0_mul7    = _mm_set1_ps( *((f32*)rhs + ((0 * 4) + 3)) );
    __m128 col0_addend3 = _mm_mul_ps( col0_mul6, col0_mul7 );

    __m128 col0_addend4 = _mm_add_ps( col0_addend0, col0_addend1 );
    __m128 col0_addend5 = _mm_add_ps( col0_addend2, col0_addend3 );
    __m128 col0_res     = _mm_add_ps( col0_addend4, col0_addend5 );

    _mm_storeu_ps( res.v + (0 * 4), col0_res );

    __m128 col1_mul0    = _mm_loadu_ps( (f32*)lhs + (0 * 4) );
    __m128 col1_mul1    = _mm_set1_ps( *((f32*)rhs + ((1 * 4) + 0)) );
    __m128 col1_addend0 = _mm_mul_ps( col1_mul0, col1_mul1 );

    __m128 col1_mul2    = _mm_loadu_ps( (f32*)lhs + (1 * 4) );
    __m128 col1_mul3    = _mm_set1_ps( *((f32*)rhs + ((1 * 4) + 1)) );
    __m128 col1_addend1 = _mm_mul_ps( col1_mul2, col1_mul3 );

    __m128 col1_mul4    = _mm_loadu_ps( (f32*)lhs + (2 * 4) );
    __m128 col1_mul5    = _mm_set1_ps( *((f32*)rhs + ((1 * 4) + 2)) );
    __m128 col1_addend2 = _mm_mul_ps( col1_mul4, col1_mul5 );

    __m128 col1_mul6    = _mm_loadu_ps( (f32*)lhs + (3 * 4) );
    __m128 col1_mul7    = _mm_set1_ps( *((f32*)rhs + ((1 * 4) + 3)) );
    __m128 col1_addend3 = _mm_mul_ps( col1_mul6, col1_mul7 );

    __m128 col1_addend4 = _mm_add_ps( col1_addend0, col1_addend1 );
    __m128 col1_addend5 = _mm_add_ps( col1_addend2, col1_addend3 );
    __m128 col1_res     = _mm_add_ps( col1_addend4, col1_addend5 );

    _mm_storeu_ps( res.v + (1 * 4), col1_res );

    __m128 col2_mul0    = _mm_loadu_ps( (f32*)lhs + (0 * 4) );
    __m128 col2_mul1    = _mm_set1_ps( *((f32*)rhs + ((2 * 4) + 0)) );
    __m128 col2_addend0 = _mm_mul_ps( col2_mul0, col2_mul1 );

    __m128 col2_mul2    = _mm_loadu_ps( (f32*)lhs + (1 * 4) );
    __m128 col2_mul3    = _mm_set1_ps( *((f32*)rhs + ((2 * 4) + 1)) );
    __m128 col2_addend1 = _mm_mul_ps( col2_mul2, col2_mul3 );

    __m128 col2_mul4    = _mm_loadu_ps( (f32*)lhs + (2 * 4) );
    __m128 col2_mul5    = _mm_set1_ps( *((f32*)rhs + ((2 * 4) + 2)) );
    __m128 col2_addend2 = _mm_mul_ps( col2_mul4, col2_mul5 );

    __m128 col2_mul6    = _mm_loadu_ps( (f32*)lhs + (3 * 4) );
    __m128 col2_mul7    = _mm_set1_ps( *((f32*)rhs + ((2 * 4) + 3)) );
    __m128 col2_addend3 = _mm_mul_ps( col2_mul6, col2_mul7 );

    __m128 col2_addend4 = _mm_add_ps( col2_addend0, col2_addend1 );
    __m128 col2_addend5 = _mm_add_ps( col2_addend2, col2_addend3 );
    __m128 col2_res     = _mm_add_ps( col2_addend4, col2_addend5 );

    _mm_storeu_ps( res.v + (2 * 4), col2_res );

    __m128 col3_mul0    = _mm_loadu_ps( (f32*)lhs + (0 * 4) );
    __m128 col3_mul1    = _mm_set1_ps( *((f32*)rhs + ((3 * 4) + 0)) );
    __m128 col3_addend0 = _mm_mul_ps( col3_mul0, col3_mul1 );

    __m128 col3_mul2    = _mm_loadu_ps( (f32*)lhs + (1 * 4) );
    __m128 col3_mul3    = _mm_set1_ps( *((f32*)rhs + ((3 * 4) + 1)) );
    __m128 col3_addend1 = _mm_mul_ps( col3_mul2, col3_mul3 );

    __m128 col3_mul4    = _mm_loadu_ps( (f32*)lhs + (2 * 4) );
    __m128 col3_mul5    = _mm_set1_ps( *((f32*)rhs + ((3 * 4) + 2)) );
    __m128 col3_addend2 = _mm_mul_ps( col3_mul4, col3_mul5 );

    __m128 col3_mul6    = _mm_loadu_ps( (f32*)lhs + (3 * 4) );
    __m128 col3_mul7    = _mm_set1_ps( *((f32*)rhs + ((3 * 4) + 3)) );
    __m128 col3_addend3 = _mm_mul_ps( col3_mul6, col3_mul7 );

    __m128 col3_addend4 = _mm_add_ps( col3_addend0, col3_addend1 );
    __m128 col3_addend5 = _mm_add_ps( col3_addend2, col3_addend3 );
    __m128 col3_res     = _mm_add_ps( col3_addend4, col3_addend5 );

    _mm_storeu_ps( res.v + (3 * 4), col3_res );

    return res;
}


#endif

attr_core_api struct Matrix4x4 m4_add(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_m4_add_sse( lhs, rhs );
#else
    return internal_m4_add_scalar( lhs, rhs );
#endif
}
attr_core_api struct Matrix4x4 m4_sub(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_m4_sub_sse( lhs, rhs );
#else
    return internal_m4_sub_scalar( lhs, rhs );
#endif
}
attr_core_api struct Matrix4x4 m4_mul(
    const struct Matrix4x4* lhs, f32 rhs
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_m4_mul_sse( lhs, rhs );
#else
    return internal_m4_mul_scalar( lhs, rhs );
#endif
}
attr_core_api struct Matrix4x4 m4_div(
    const struct Matrix4x4* lhs, f32 rhs
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_m4_div_sse( lhs, rhs );
#else
    return internal_m4_div_scalar( lhs, rhs );
#endif
}
attr_core_api struct Matrix4x4 m4_mul_m4(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_m4_mul_m4_sse( lhs, rhs );
#else
    return internal_m4_mul_m4_scalar( lhs, rhs );
#endif
}
attr_core_api struct Vector4 m4_mul_v4(
    const struct Matrix4x4* lhs, const struct Vector4 rhs
) {
    // NOTE(alicia): sse does not seem to help in this case. :(
    return v4(
        (lhs->v[0] * rhs.v[0]) + (lhs->v[4] * rhs.v[1]) + (lhs->v[ 8] * rhs.v[2]) + (lhs->v[12] * rhs.v[3]),
        (lhs->v[1] * rhs.v[0]) + (lhs->v[5] * rhs.v[1]) + (lhs->v[ 9] * rhs.v[2]) + (lhs->v[13] * rhs.v[3]),
        (lhs->v[2] * rhs.v[0]) + (lhs->v[6] * rhs.v[1]) + (lhs->v[10] * rhs.v[2]) + (lhs->v[14] * rhs.v[3]),
        (lhs->v[3] * rhs.v[0]) + (lhs->v[7] * rhs.v[1]) + (lhs->v[11] * rhs.v[2]) + (lhs->v[15] * rhs.v[3])
    );
}
attr_core_api f32 m4_determinant( const struct Matrix4x4* m ) {
    // TODO(alicia): sse?

    struct Matrix3x3 sub0 = m4_submatrix( m, 0, 0 );
    struct Matrix3x3 sub1 = m4_submatrix( m, 1, 0 );
    struct Matrix3x3 sub2 = m4_submatrix( m, 2, 0 );
    struct Matrix3x3 sub3 = m4_submatrix( m, 3, 0 );

    return
        ( m->v[ 0] * m3_determinant( &sub0 ) ) -
        ( m->v[ 4] * m3_determinant( &sub1 ) ) +
        ( m->v[ 8] * m3_determinant( &sub2 ) ) -
        ( m->v[12] * m3_determinant( &sub3 ) );
}
attr_core_api struct Matrix3x3 m4_submatrix(
    const struct Matrix4x4* m, u32 row, u32 column
) {
    struct Matrix3x3 res;
    u32 i = 0;
    for( u32 r = 0; r < 4; ++r ) {
        if( r == row ) {
            continue;
        }

        for( u32 c = 0; c < 4; ++c ) {
            if( c == column ) {
                continue;
            }

            res.v[i++] = m->c[c].v[r];
        }
    }

    return res;
}
attr_core_api f32 m4_cofactor( const struct Matrix4x4* m, u32 column, u32 row ) {
    f32 minor = m4_minor( m, column, row );
    i32 exp   = ( row + 1 ) + ( column + 1 );
    return minor * poweri( -1.0f, exp );
}
attr_core_api struct Matrix4x4 m4_cofactor_matrix( const struct Matrix4x4* m ) {
    return m4(
        m4_cofactor( m, 0, 0 ), m4_cofactor( m, 0, 1 ), m4_cofactor( m, 0, 2 ), m4_cofactor( m, 0, 3 ),
        m4_cofactor( m, 1, 0 ), m4_cofactor( m, 1, 1 ), m4_cofactor( m, 1, 2 ), m4_cofactor( m, 1, 3 ),
        m4_cofactor( m, 2, 0 ), m4_cofactor( m, 2, 1 ), m4_cofactor( m, 2, 2 ), m4_cofactor( m, 2, 3 ),
        m4_cofactor( m, 3, 0 ), m4_cofactor( m, 3, 1 ), m4_cofactor( m, 3, 2 ), m4_cofactor( m, 3, 3 ) );
}
attr_core_api b32 m4_inverse(
    const struct Matrix4x4* m, struct Matrix4x4* out_inverse
) {
    f32 det = m4_determinant( m );
    if( det == 0.0f ) {
        return false;
    }

    struct Matrix4x4 adjoint = m4_adjoint( m );
    *out_inverse = m4_div( &adjoint, det );
    return true;
}
attr_core_api struct Matrix4x4 m4_inverse_unchecked( const struct Matrix4x4* m ) {
    f32 det = m4_determinant( m );
    struct Matrix4x4 adjoint = m4_adjoint( m );
    return m4_div( &adjoint, det );
}
attr_core_api b32 m4_normal_matrix(
    const struct Matrix4x4* m, struct Matrix3x3* out_normal
) {
    struct Matrix4x4 inverse;

    if( !m4_inverse( m, &inverse ) ) {
        return false;
    }

    *out_normal = m3(
        inverse.v[0], inverse.v[4], inverse.v[ 8],
        inverse.v[1], inverse.v[5], inverse.v[ 9],
        inverse.v[2], inverse.v[6], inverse.v[10] );
    return true;
}
attr_core_api struct Matrix3x3 m4_normal_matrix_unchecked( const struct Matrix4x4* m ) {
    struct Matrix4x4 inverse = m4_inverse_unchecked( m );
    return m3(
        inverse.v[0], inverse.v[4], inverse.v[ 8],
        inverse.v[1], inverse.v[5], inverse.v[ 9],
        inverse.v[2], inverse.v[6], inverse.v[10] );
}
attr_core_api struct Matrix4x4 m4_view(
    struct Vector3 position, struct Vector3 target,
    struct Vector3 up
) {
    struct Vector3 z = v3_normalize( v3_sub( target, position ) );
    struct Vector3 x = v3_normalize( v3_cross( z, up ) );
    struct Vector3 y = v3_cross( x, z );
    z = v3_neg( z );

    f32 dx = -v3_dot( x, position );
    f32 dy = -v3_dot( y, position );
    f32 dz = -v3_dot( z, position );

    return m4(
        x.x, y.x, z.x, 0.0f,
        x.y, y.y, z.y, 0.0f,
        x.z, y.z, z.z, 0.0f,
         dx,  dy,  dz, 1.0f );
}
attr_core_api struct Matrix4x4 m4_ortho(
    f32 left, f32 right,
    f32 bottom, f32 top,
    f32 clip_near, f32 clip_far
) {
    struct Matrix4x4 res = MAT4_IDENTITY;
    f32 rl = right - left;
    f32 tb = top - bottom;
    f32 fn = clip_far - clip_near;

    res.v[ 0] =  2.0f / rl;
    res.v[ 5] =  2.0f / tb;
    res.v[10] = -2.0f / fn;
    res.v[12] = -( right + left ) / rl;
    res.v[13] = -( top + bottom ) / tb;
    res.v[14] = -( clip_far + clip_near ) / fn;

    return res;
}
attr_core_api struct Matrix4x4 m4_perspective(
    f32 fov, f32 aspect_ratio, f32 clip_near, f32 clip_far
) {
    struct Matrix4x4 res = MAT4_ZERO;
    
    f32 half_fov_tan = tangent( fov / 2.0f );

    res.v[ 0] = 1.0f / ( aspect_ratio * half_fov_tan );
    res.v[ 5] = 1.0f / half_fov_tan;
    res.v[10] = -( ( clip_far + clip_near ) / ( clip_far - clip_near ) );
    res.v[11] = -1.0f;
    res.v[14] =
        -( ( 2.0f * clip_far * clip_near ) / ( clip_far - clip_near ) );

    return res;
}
attr_core_api struct Matrix4x4 m4_rotation_pitch( f32 pitch ) {
    f32 sin, cos;
    sine_cosine( pitch, &sin, &cos );

    return m4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f,  cos,  sin, 0.0f,
        0.0f, -sin,  cos, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f );
}
attr_core_api struct Matrix4x4 m4_rotation_yaw( f32 yaw ) {
    f32 sin, cos;
    sine_cosine( yaw, &sin, &cos );

    return m4(
         cos, 0.0f, -sin, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
         sin, 0.0f,  cos, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f );
}
attr_core_api struct Matrix4x4 m4_rotation_roll( f32 roll ) {
    f32 sin, cos;
    sine_cosine( roll, &sin, &cos );

    return m4(
         cos,  sin, 0.0f, 0.0f,
        -sin,  cos, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f );
}
attr_core_api struct Matrix4x4 m4_rotation_euler( f32 pitch, f32 yaw, f32 roll ) {
    struct Matrix4x4 pitch_m = m4_rotation_pitch( pitch );
    struct Matrix4x4 yaw_m   = m4_rotation_yaw( yaw );
    struct Matrix4x4 roll_m  = m4_rotation_roll( roll );

    struct Matrix4x4 temp = m4_mul_m4( &yaw_m, &roll_m );
    return m4_mul_m4( &pitch_m, &temp );
}
attr_core_api struct Matrix4x4 m4_rotation( struct Quaternion rotation ) {
    struct Matrix4x4 res = MAT4_IDENTITY;

    // TODO(alicia): sse?

    f32 _2x2 = 2.0f * (rotation.x * rotation.x);
    f32 _2y2 = 2.0f * (rotation.y * rotation.y);
    f32 _2z2 = 2.0f * (rotation.z * rotation.z);

    f32 _2xy = 2.0f * (rotation.x * rotation.y);
    f32 _2xz = 2.0f * (rotation.x * rotation.z);
    f32 _2yz = 2.0f * (rotation.y * rotation.z);
    f32 _2wx = 2.0f * (rotation.w * rotation.x);
    f32 _2wy = 2.0f * (rotation.w * rotation.y);
    f32 _2wz = 2.0f * (rotation.w * rotation.z);

    res.v[0] = 1.0f - _2y2 - _2z2;
    res.v[1] = _2xy + _2wz;
    res.v[2] = _2xz - _2wy;

    res.v[4] = _2xy - _2wz;
    res.v[5] = 1.0f - _2x2 - _2z2;
    res.v[6] = _2yz + _2wx;

    res.v[8]  = _2xz + _2wy;
    res.v[9]  = _2yz - _2wx;
    res.v[10] = 1.0f - _2x2 - _2y2;

    return res;
}
attr_core_api struct Matrix4x4 m4_transform(
    struct Vector3 translation, struct Quaternion rotation,
    struct Vector3 scale
) {
    struct Matrix4x4 t = m4_translation_v3( translation );
    struct Matrix4x4 r = m4_rotation( rotation );
    struct Matrix4x4 s = m4_scale_v3( scale );

    struct Matrix4x4 temp = m4_mul_m4( &r, &s );
    return m4_mul_m4( &t, &temp );
}
attr_core_api struct Matrix4x4 m4_transform_euler(
    struct Vector3 translation, struct Vector3 rotation,
    struct Vector3 scale
) {
    struct Matrix4x4 t = m4_translation_v3( translation );
    struct Matrix4x4 r = m4_rotation_euler_v3( rotation );
    struct Matrix4x4 s = m4_scale_v3( scale );

    struct Matrix4x4 temp = m4_mul_m4( &r, &s );
    return m4_mul_m4( &t, &temp );
}


attr_core_api struct Matrix2x2 m2_from_array( const f32 array[4] ) {
    struct Matrix2x2 result;
    memory_copy( result.v, array, sizeof(f32) * 4 );
    return result;
}
attr_core_api void m2_to_array( struct Matrix2x2 m, f32* out_array ) {
    memory_copy( out_array, m.v, sizeof(f32) * 4 );
}
attr_core_api struct Matrix3x3 m3_from_array( const f32 array[9] ) {
    struct Matrix3x3 result;
    memory_copy( result.v, array, sizeof(f32) * 9 );
    return result;
}
attr_core_api void m3_to_array( const struct Matrix3x3* m, f32* out_array ) {
    memory_copy( out_array, m, sizeof(f32) * 9 );
}
attr_core_api struct Matrix4x4 m4_from_array( const f32 array[16] ) {
    struct Matrix4x4 result;
    memory_copy( result.v, array, sizeof(f32) * 16 );
    return result;
}
attr_core_api void m4_to_array( const struct Matrix4x4* m, f32* out_array ) {
    memory_copy( out_array, m, sizeof(f32) * 16 );
}
attr_core_api f32 v2_mag( struct Vector2 x ) {
    return square_root( v2_sqrmag( x ) );
}
attr_core_api struct Vector2 v2_normalize( struct Vector2 x ) {
    f32 sqrm = v2_sqrmag( x );
    if( sqrm == 0.0f ) {
        return VEC2_ZERO;
    } else {
        return v2_div( x, square_root( sqrm ) );
    }
}
attr_core_api struct Vector2 v2_rotate( struct Vector2 v, f32 angle ) {
    f32 sin = 0.0f, cos = 0.0f;
    sine_cosine( angle, &sin, &cos );
    struct Vector2 a = v2( cos, sin );
    struct Vector2 b = v2( -sin, cos );

    a = v2_mul( a, v.x );
    b = v2_mul( b, v.y );

    return v2_add( a, b );
}
attr_core_api f32 v2_angle( struct Vector2 a, struct Vector2 b ) {
    return arc_cosine( v2_dot( a, b ) );
}
attr_core_api struct Vector2 v2_lerp(
    struct Vector2 a, struct Vector2 b, f32 t
) {
    return v2( lerpf( a.x, b.x, t ), lerpf( a.y, b.y, t ) );
}
attr_core_api struct Vector2 v2_smooth_step(
    struct Vector2 a, struct Vector2 b, f32 t
) {
    return v2( smooth_stepf( a.x, b.x, t ), smooth_stepf( a.y, b.y, t ) );
}
attr_core_api struct Vector2 v2_smoother_step(
    struct Vector2 a, struct Vector2 b, f32 t
) {
    return v2( smoother_stepf( a.x, b.x, t ), smoother_stepf( a.y, b.y, t ) );
}
attr_core_api f32 v3_mag( struct Vector3 x ) {
    return square_root( v3_sqrmag( x ) );
}
attr_core_api struct Vector3 v3_normalize( struct Vector3 x ) {
    f32 m = v3_sqrmag( x );
    if( m == 0.0f ) {
        return VEC3_ZERO;
    } else {
        return v3_div( x, square_root( m ) );
    }
}
attr_core_api f32 v3_angle( struct Vector3 a, struct Vector3 b ) {
    return arc_cosine( v3_dot( a, b ) );
}
attr_core_api struct Vector3 v3_lerp(
    struct Vector3 a, struct Vector3 b, f32 t
) {
    return v3(
        lerpf( a.x, b.x, t ),
        lerpf( a.y, b.y, t ),
        lerpf( a.z, b.z, t ) );
}
attr_core_api struct Vector3 v3_smooth_step(
    struct Vector3 a, struct Vector3 b, f32 t
) {
    return v3(
        smooth_stepf( a.x, b.x, t ),
        smooth_stepf( a.y, b.y, t ),
        smooth_stepf( a.z, b.z, t ) );
}
attr_core_api struct Vector3 v3_smoother_step(
    struct Vector3 a, struct Vector3 b, f32 t
) {
    return v3(
        smoother_stepf( a.x, b.x, t ),
        smoother_stepf( a.y, b.y, t ),
        smoother_stepf( a.z, b.z, t ) );
}
attr_core_api f32 v4_mag( struct Vector4 x ) {
    return square_root( v4_sqrmag( x ) );
}
attr_core_api struct Vector4 v4_normalize( struct Vector4 x ) {
    f32 m = v4_sqrmag( x );
    if( m == 0.0f ) {
        return VEC4_ZERO;
    } else {
        return v4_div( x, square_root( m ) );
    }
}
attr_core_api struct Vector4 v4_lerp(
    struct Vector4 a, struct Vector4 b, f32 t
) {
    return v4(
        lerpf( a.x, b.x, t ),
        lerpf( a.y, b.y, t ),
        lerpf( a.z, b.z, t ),
        lerpf( a.w, b.w, t ) );
}
attr_core_api struct Vector4 v4_smooth_step(
    struct Vector4 a, struct Vector4 b, f32 t
) {
    return v4(
        smooth_stepf( a.x, b.x, t ),
        smooth_stepf( a.y, b.y, t ),
        smooth_stepf( a.z, b.z, t ),
        smooth_stepf( a.w, b.w, t ) );
}
attr_core_api struct Vector4 v4_smoother_step(
    struct Vector4 a, struct Vector4 b, f32 t
) {
    return v4(
        smoother_stepf( a.x, b.x, t ),
        smoother_stepf( a.y, b.y, t ),
        smoother_stepf( a.z, b.z, t ),
        smoother_stepf( a.w, b.w, t ) );
}
attr_core_api f32 q_mag( struct Quaternion q ) {
    return square_root( q_sqrmag( q ) );
}
attr_core_api struct Quaternion q_lerp(
    struct Quaternion a, struct Quaternion b, f32 t
) {
    struct Quaternion l = q(
        lerpf( a.w, b.w, t ),
        lerpf( a.x, b.y, t ),
        lerpf( a.y, b.y, t ),
        lerpf( a.z, b.z, t ) );
    return q_normalize( l );
}

