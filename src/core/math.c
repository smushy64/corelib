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
    #warning "f32_sincos is not yet implemented!"
    #define INTERNAL_CORE_SINE_COSINE_NOT_IMPLEMENTED
#endif

attr_core_api
f32 luma_from_rgb( struct Vector3 color ) {
    return f32_sqrt( vec3_dot( color, vec3_new( 0.299, 0.587, 0.114 ) ) );
}
attr_core_api
struct Vector3 srgb_from_lin( struct Vector3 linear ) {
    return vec3_pow( linear, vec3_set(1.0 / 2.2) );
}
attr_core_api
struct Vector3 lin_from_srgb( struct Vector3 srgb ) {
    return vec3_pow( srgb, vec3_set(2.2) );
}

attr_core_api
struct Vector3 cie_xyz_from_rgb( struct Vector3 color ) {
    // Standard Illuminant D65
    // https://en.wikipedia.org/wiki/Oklab_color_space
    struct Matrix3x3 m = { .array = {
        0.4122214708, 0.2119034982, 0.0883024619,
        0.5363325263, 0.6806995451, 0.2817188376,
        0.0514459929, 0.1073969566, 0.6299787005,
    } };

    return mat3_mul_vec3( &m, color );
}
attr_core_api
struct Vector3 rgb_from_cie_xyz( struct Vector3 color ) {
    // Standard Illuminant D65
    // https://en.wikipedia.org/wiki/Oklab_color_space
    struct Matrix3x3 m = { .array = {
         3.240479, -0.969256,  0.055648,
        -1.537150,  1.875992, -0.204043,
        -0.498535,  0.041556,  1.057311
    } };

    return mat3_mul_vec3( &m, color );
}

attr_core_api
struct Vector3 cie_xyz_from_lms( struct Vector3 color ) {
    struct Matrix3x3 m = { .array = {
        1.4002,  -0.4592,  -0.1035,
       -0.5034,   1.2580,  -0.3557,
       -0.0159,   0.0240,   1.0570
    } };
    return mat3_mul_vec3( &m, color );
}
attr_core_api
struct Vector3 lms_from_cie_xyz( struct Vector3 color ) {
    struct Matrix3x3 m = { .array = {
        0.818933,  0.032984,  0.048200,
        0.361866,  0.929311,  0.264366,
       -0.128859,  0.036145,  0.633853
    } };

    return mat3_mul_vec3( &m, color );
}

attr_core_api
struct Vector3 oklab_from_lms( struct Vector3 color ) {
    struct Vector3 cbrt_transform = color;
    cbrt_transform.x = f32_cbrt( cbrt_transform.x );
    cbrt_transform.y = f32_cbrt( cbrt_transform.y );
    cbrt_transform.z = f32_cbrt( cbrt_transform.z );

    struct Matrix3x3 m = { .array = {
        0.210454,  1.977998,  0.025904,
        0.793617, -2.428592,  0.782771,
       -0.004072,  0.450593, -0.808675
    }};

    return mat3_mul_vec3( &m, cbrt_transform );
}
attr_core_api
struct Vector3 lms_from_oklab( struct Vector3 color ) {
    struct Matrix3x3 m = { .array = {
        4.07657,  -2.26843,  0.26702,
        0.35857,   1.20543,  -0.09235,
        0.18116,   0.05353,   0.79694
    }};

    struct Vector3 lms = mat3_mul_vec3( &m, color );

    lms.x = f32_powi( lms.x, 3 );
    lms.y = f32_powi( lms.y, 3 );
    lms.z = f32_powi( lms.z, 3 );

    return lms;
}

attr_core_api
f32 f32_cbrt( f32 x ) {
    if( x == 0.0f ) {
        return 0.0f;
    }

    u32 ux = *(u32*)&x;
    ux     = ux / 3 + 709921077; // Initial guess using integer bit manipulation

    f32 y = *(f32*)&ux;

    // Perform 2 iterations of Newton-Raphson for refinement
    y = (2.0f * y + x / (y * y)) / 3.0f;
    y = (2.0f * y + x / (y * y)) / 3.0f;

    return y;
}

attr_unused attr_internal attr_always_inline inline
f32 internal_f32_sqrt( f32 x ) {
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
attr_unused attr_internal attr_always_inline inline
f32 internal_f32_inversesqrt( f32 x ) {
    return 1.0f / internal_f32_sqrt( x );
}

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)

attr_internal attr_always_inline inline
f32 internal_f32_sqrt_sse( f32 x ) {
    return _mm_cvtss_f32( _mm_sqrt_ss( _mm_set_ss( x ) ) );
}
attr_internal attr_always_inline inline
f32 internal_f32_inversesqrt_sse( f32 x ) {
    return _mm_cvtss_f32( _mm_rsqrt_ss( _mm_set_ss( x ) ) );
}

#endif /* SSE functions */ 

attr_core_api
f32 f32_sqrt( f32 x ) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_f32_sqrt_sse( x );
#else
    return internal_f32_sqrt( x );
#endif
}

attr_core_api
f32 f32_inversesqrt( f32 x ) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_f32_inversesqrt_sse( x );
#else
    return internal_f32_inversesqrt( x );
#endif
}

attr_core_api
f32 f32_ln( f32 x ) {
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

attr_core_api
f32 f32_log2( f32 x ) {
    if( x < 0.0f ) {
        return F32_NAN;
    }
    if( x == 2.0f ) {
        return 1.0f;
    }
    return f32_ln( x ) * 1.49f;
}
attr_core_api
f32 f32_log10( f32 x ) {
    if( x < 0.0f ) {
        return F32_NAN;
    }
    if( x == 10.0f ) {
        return 1.0f;
    }
    return f32_ln( x ) / 2.3f;
}

attr_core_api
f32 f32_powi( f32 base, i32 exp ) {
    u32 exp_abs = num_abs( exp );
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
attr_core_api
f32 f32_exp( f32 x ) {
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

attr_core_api
f32 f32_mod( f32 lhs, f32 rhs ) {
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

attr_core_api
f32 f32_wrap_degrees( f32 deg ) {
    f32 result = f32_mod( deg, 360.0f );
    return result < 0.0f ? result + 360.0f : result;
}
attr_core_api
f32 f32_wrap_radians( f32 rad ) {
    return f32_mod( rad + F32_PI, F32_TAU ) - F32_PI;
}

attr_core_api
f32 f32_sin( f32 x ) {
#if defined(INTERNAL_CORE_SINE_COSINE_NOT_IMPLEMENTED)
    x = f32_wrap_radians(x);

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
    f32_sincos( x, &res, &c );
    return res;
#endif
}
attr_core_api
f32 f32_cos( f32 x ) {
#if defined(INTERNAL_CORE_SINE_COSINE_NOT_IMPLEMENTED)
    x = f32_wrap_radians(x);

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
    f32_sincos( x, &s, &res );
    return res;
#endif
}

attr_unused attr_internal attr_always_inline inline
void internal_f32_sincos( f32 x, f32* out_sine, f32* out_cosine ) {
    *out_sine   = f32_sin(x);
    *out_cosine = f32_cos(x);
}

#if defined(CORE_ARCH_X86)

#if defined(CORE_COMPILER_MSVC)

#pragma message ("internal_f32_sincos_fsincos is not yet implemented in MASM!")
// extern void internal_f32_sincos_fsincos( f32 x, f32* out_sine, f32* out_cos );

void internal_f32_sincos_fsincos( f32 x, f32* out_sine, f32* out_cos ) {
    internal_f32_sincos( x, out_sine, out_f32_cos );
}

#else /* MSVC */

attr_internal attr_always_inline inline attr_hot
void internal_f32_sincos_fsincos( f32 x, f32* out_sine, f32* out_cos ) {
    f32 s, c;
    __asm__ inline ( "fsincos" : "=t"(c), "=u"(s) : "0"(x) );
    *out_sine = s;
    *out_cos  = c;
}

#endif /* GCC || CLANG */

#endif /* x86 fsincos */

attr_core_api
void f32_sincos( f32 x, f32* out_sine, f32* out_f32_cos ) {
#if defined(CORE_ARCH_X86)
    internal_f32_sincos_fsincos( x, out_sine, out_f32_cos );
#else
    internal_f32_sincos( x, out_sine, out_f32_cos );
#endif
}

attr_core_api
f32 f32_asin( f32 x ) {
    // NOTE(alicia): don't ask me how i figured this out
    // i don't even know
    f32 sign_of_x = (((x) > 0.0f) - ((x) < 0.0f));
    f32 x_abs = x * sign_of_x;
    f32 x_sqr = x_abs * x_abs;

    const f32 magic_0 =  1.5707288f;
    const f32 magic_1 = -0.2121144f;
    const f32 magic_2 =  0.0742610f;
    const f32 magic_3 = -0.0187293f;

    f32 result = F32_HALF_PI - f32_sqrt( 1.0f - x_abs ) * (
        magic_0 +
        ( magic_1 * x_abs ) +
        ( magic_2 * x_sqr ) +
        ( magic_3 * ( x_sqr * x_abs ) )
    );

    return result * sign_of_x;
}
attr_core_api
f32 f32_atan( f32 x ) {
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
attr_core_api
f32 f32_atan2( f32 y, f32 x ) {
    if( y == 0.0f ) {
        if( x < 0.0f ) {
            return F32_PI;
        } else if( x == 0.0f ) {
            return F32_NAN;
        }
    }

    f32 x_sqr = x * x;
    f32 y_sqr = y * y;
    return 2.0f * f32_atan( y / ( f32_sqrt( x_sqr + y_sqr ) + x ) );
}

attr_core_api
struct Vector3 hsl_from_rgb( struct Vector3 c ) {
    struct Vector4 K = vec4_new( 0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0 );
    struct Vector4 p = vec4_mix(
        vec4_new(c.b, c.g, K.w, K.z),
        vec4_new(c.g, c.b, K.x, K.y),
        f32_step(c.b, c.g) );
    struct Vector4 q = vec4_mix(
        vec4_new(p.x, p.y, p.w, c.r),
        vec4_new(c.r, p.y, p.z, p.x),
        f32_step(p.x, c.r) );

    f32 d = q.x - num_min(q.w, q.y);
    f32 e = 1.0e-10;

    f32 x = num_abs(q.z + (q.w - q.y) / (6.0 * d + e));
    return vec3_new( x, d / (q.x + e), q.x );
}
attr_core_api
struct Vector3 rgb_from_hsl( struct Vector3 color ) {
    struct Vector4 K = vec4_new( 1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0 );

    struct Vector3 _fract         = vec3_fract( vec3_add( vec3_set( color.x ), K.xyz ) );
    struct Vector3 _fract6        = vec3_mul( _fract, 6.0f );
    struct Vector3 _fract6_sub_kw = vec3_sub( _fract6, vec3_set( K.w ) );

    struct Vector3 p = vec3_new(
        num_abs(_fract6_sub_kw.x),
        num_abs(_fract6_sub_kw.y),
        num_abs(_fract6_sub_kw.z) );

    return vec3_mul(
        vec3_mix(
            vec3_set( K.x ),
            vec3_clamp( vec3_sub( p, vec3_set( K.x ) ), VEC3_ZERO, VEC3_ONE ),
            color.y
        ), color.z );
}
attr_core_api
struct Vector3 vec3_slerp( struct Vector3 a, struct Vector3 b, f32 t ) {
    /*
     * sin[(1 - t)omega]        sin[t * omega]
     * ----------------- * a + ---------------- * p1
     *     sin[omega]             sin[omega]
     * */

    f32 omega     = f32_acos( vec3_dot( a, b ) );
    f32 sin_omega = f32_sin( omega );

    vec3 quotient_1 = vec3_mul( a, f32_sin( (1.0f - t) * omega ) / sin_omega );
    vec3 quotient_2 = vec3_mul( b, f32_sin( t * omega ) / sin_omega );

    return vec3_add( quotient_1, quotient_2 );
}

attr_unused attr_always_inline inline attr_internal
struct Quaternion internal_quat_mul_quat_scalar(
     struct Quaternion lhs, struct Quaternion rhs   
) {
    return quat_new( ( lhs.array[0] * rhs.array[0] ) -
        ( lhs.array[1] * rhs.array[1] + lhs.array[2] * rhs.array[2] + lhs.array[3] * rhs.array[3] ),
    ( lhs.array[0] * rhs.array[1] ) +
        ( rhs.array[0] * lhs.array[1] ) +
        ( ( lhs.array[2] * rhs.array[3] ) -
        ( lhs.array[3] * rhs.array[2] ) ),
    ( lhs.array[0] * rhs.array[2] ) +
        ( rhs.array[0] * lhs.array[2] ) +
        ( ( lhs.array[3] * rhs.array[1] ) -
        ( lhs.array[1] * rhs.array[3] ) ),
    ( lhs.array[0] * rhs.array[3] ) +
        ( rhs.array[0] * lhs.array[3] ) +
        ( ( lhs.array[1] * rhs.array[2] ) -
        ( lhs.array[2] * rhs.array[1] ) )  );
}

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)

attr_internal
struct Quaternion internal_quat_mul_quat_sse(
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

    b = _mm_loadu_ps( rhs.array );
    w = _mm_mul_ps( w, b );

    attr_align(16) struct Quaternion result;
    _mm_store_ps( result.array, _mm_add_ps( w, a ) );
    return result;
}

#endif

attr_core_api
struct Quaternion quat_mul_quat(
    struct Quaternion lhs, struct Quaternion rhs
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_quat_mul_quat_sse( lhs, rhs );
#else
    return internal_quat_mul_quat_scalar( lhs, rhs );
#endif
}
attr_core_api
struct Vector3 quat_mul_vec3(
    struct Quaternion lhs, struct Vector3 rhs
) {
    vec3 t = vec3_mul( vec3_cross( lhs.xyz, rhs ), 2.0f );
    return vec3_add( vec3_add( rhs, vec3_mul( t, lhs.w ) ), vec3_cross( lhs.xyz, t ) );
}
attr_core_api
struct Quaternion quat_slerp(
    struct Quaternion a, struct Quaternion b, f32 t
) {
    struct Quaternion _b = b;

    f32 theta     = quat_dot( a, b );
    f32 cos_theta = f32_cos( theta );

    if( cos_theta < 0.0f ) {
        _b        = quat_neg( _b );
        cos_theta = -cos_theta;
    }

    if( cos_theta > ( 1.0f - F32_EPSILON ) ) {
        return quat_lerp( a, b, t );
    } else {
        return quat_normalize(
            quat_div (
                quat_add(
                    quat_mul( a, f32_sin( (1.0f - t) * theta ) ),
                    quat_mul( _b, f32_sin( t * theta ) )
                ),
                f32_sin( theta )
            ) );
    }
}
attr_core_api
struct Quaternion quat_from_angle_axis(
    struct AngleAxis_ a
) {
    f32 half_angle = a.angle / 2.0f;
    f32 sin, cos;
    f32_sincos( half_angle, &sin, &cos );
    struct Quaternion result;
    result.w   = cos;
    result.xyz = vec3_mul( a.axis, sin );
    return quat_normalize( result );
}
attr_core_api
struct Quaternion quat_from_euler( f32 x, f32 y, f32 z ) {
    f32 hx = x / 2.0f;
    f32 hy = y / 2.0f;
    f32 hz = z / 2.0f;

    f32 x_sin, y_sin, z_sin;
    f32 x_cos, y_cos, z_cos;

    f32_sincos( hx, &x_sin, &x_cos );
    f32_sincos( hy, &y_sin, &y_cos );
    f32_sincos( hz, &z_sin, &z_cos );

    f32 xyz_sin = x_sin * y_sin * z_sin;
    f32 xyz_cos = x_cos * y_cos * z_cos;

    struct Quaternion result;

    result.w = xyz_cos + xyz_sin;
    result.x = ( x_sin * y_cos * z_cos ) + ( x_cos * y_sin * z_sin );
    result.y = ( x_cos * y_sin * z_cos ) + ( x_sin * y_cos * z_sin );
    result.z = ( x_cos * y_cos * z_sin ) + ( x_sin * y_sin * z_cos );

    return result;
}
attr_core_api
struct Vector3 quat_to_euler( struct Quaternion q ) {
    struct Vector3 result;

    result.x = f32_atan2(
        2.0f * ((q.w * q.x) + (q.y * q.z)),
        1.0f - 2.0f * ((q.x * q.x) + (q.y * q.y)) );

    result.y = f32_asin( 2.0f * ((q.w * q.y) - (q.z * q.x)) );

    result.z = f32_atan2(
        2.0f * ((q.w * q.z) + (q.x * q.y)),
        1.0f - 2.0f * ((q.y * q.y) + (q.z * q.z)) );
    return result;
}
attr_core_api
struct AngleAxis_ quat_to_angle_axis( struct Quaternion q ) {
    struct AngleAxis_ result;
    
    result.angle = f32_acos( q.w ) * 2.0f;
    result.axis  = vec3_div( q.xyz, f32_sqrt( 1.0f - ( q.w * q.w ) ) );

    return result;
}

attr_unused attr_hot attr_internal attr_always_inline inline
struct Matrix4x4 internal_mat4_mul_mat4_scalar( const struct Matrix4x4* lhs, const struct Matrix4x4* rhs ) {
    return mat4_new(
        // column - 0
        ( lhs->array[0] * rhs->array[0] ) + ( lhs->array[4] * rhs->array[1] ) + ( lhs->array[8]  * rhs->array[2] ) + ( lhs->array[12] * rhs->array[3] ),
        ( lhs->array[1] * rhs->array[0] ) + ( lhs->array[5] * rhs->array[1] ) + ( lhs->array[9]  * rhs->array[2] ) + ( lhs->array[13] * rhs->array[3] ),
        ( lhs->array[2] * rhs->array[0] ) + ( lhs->array[6] * rhs->array[1] ) + ( lhs->array[10] * rhs->array[2] ) + ( lhs->array[14] * rhs->array[3] ),
        ( lhs->array[3] * rhs->array[0] ) + ( lhs->array[7] * rhs->array[1] ) + ( lhs->array[11] * rhs->array[2] ) + ( lhs->array[15] * rhs->array[3] ),
        // column - 1
        ( lhs->array[0] * rhs->array[4] ) + ( lhs->array[4] * rhs->array[5] ) + ( lhs->array[8]  * rhs->array[6] ) + ( lhs->array[12] * rhs->array[7] ),
        ( lhs->array[1] * rhs->array[4] ) + ( lhs->array[5] * rhs->array[5] ) + ( lhs->array[9]  * rhs->array[6] ) + ( lhs->array[13] * rhs->array[7] ),
        ( lhs->array[2] * rhs->array[4] ) + ( lhs->array[6] * rhs->array[5] ) + ( lhs->array[10] * rhs->array[6] ) + ( lhs->array[14] * rhs->array[7] ),
        ( lhs->array[3] * rhs->array[4] ) + ( lhs->array[7] * rhs->array[5] ) + ( lhs->array[11] * rhs->array[6] ) + ( lhs->array[15] * rhs->array[7] ),
        // column - 2
        ( lhs->array[0] * rhs->array[8] ) + ( lhs->array[4] * rhs->array[9] ) + ( lhs->array[8]  * rhs->array[10] ) + ( lhs->array[12] * rhs->array[11] ),
        ( lhs->array[1] * rhs->array[8] ) + ( lhs->array[5] * rhs->array[9] ) + ( lhs->array[9]  * rhs->array[10] ) + ( lhs->array[13] * rhs->array[11] ),
        ( lhs->array[2] * rhs->array[8] ) + ( lhs->array[6] * rhs->array[9] ) + ( lhs->array[10] * rhs->array[10] ) + ( lhs->array[14] * rhs->array[11] ),
        ( lhs->array[3] * rhs->array[8] ) + ( lhs->array[7] * rhs->array[9] ) + ( lhs->array[11] * rhs->array[10] ) + ( lhs->array[15] * rhs->array[11] ),
        // column - 3
        ( lhs->array[0] * rhs->array[12] ) + ( lhs->array[4] * rhs->array[13] ) + ( lhs->array[8]  * rhs->array[14] ) + ( lhs->array[12] * rhs->array[15] ),
        ( lhs->array[1] * rhs->array[12] ) + ( lhs->array[5] * rhs->array[13] ) + ( lhs->array[9]  * rhs->array[14] ) + ( lhs->array[13] * rhs->array[15] ),
        ( lhs->array[2] * rhs->array[12] ) + ( lhs->array[6] * rhs->array[13] ) + ( lhs->array[10] * rhs->array[14] ) + ( lhs->array[14] * rhs->array[15] ),
        ( lhs->array[3] * rhs->array[12] ) + ( lhs->array[7] * rhs->array[13] ) + ( lhs->array[11] * rhs->array[14] ) + ( lhs->array[15] * rhs->array[15] ) );
}

attr_unused attr_hot attr_internal attr_always_inline inline
struct Matrix4x4 internal_mat4_add_scalar(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
    struct Matrix4x4 res;
    res.col0 = vec4_add( lhs->col0, rhs->col0 );
    res.col1 = vec4_add( lhs->col1, rhs->col1 );
    res.col2 = vec4_add( lhs->col2, rhs->col2 );
    res.col3 = vec4_add( lhs->col3, rhs->col3 );
    return res;
}
attr_unused attr_hot attr_internal attr_always_inline inline
struct Matrix4x4 internal_mat4_sub_scalar(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
    struct Matrix4x4 res;
    res.col0 = vec4_sub( lhs->col0, rhs->col0 );
    res.col1 = vec4_sub( lhs->col1, rhs->col1 );
    res.col2 = vec4_sub( lhs->col2, rhs->col2 );
    res.col3 = vec4_sub( lhs->col3, rhs->col3 );
    return res;
}
attr_unused attr_hot attr_internal attr_always_inline inline
struct Matrix4x4 internal_mat4_mul_scalar(
    const struct Matrix4x4* lhs, f32 rhs
) {
    struct Matrix4x4 res;
    res.col0 = vec4_mul( lhs->col0, rhs );
    res.col1 = vec4_mul( lhs->col1, rhs );
    res.col2 = vec4_mul( lhs->col2, rhs );
    res.col3 = vec4_mul( lhs->col3, rhs );
    return res;
}
attr_unused attr_hot attr_internal attr_always_inline inline
struct Matrix4x4 internal_mat4_div_scalar(
    const struct Matrix4x4* lhs, f32 rhs
) {
    struct Matrix4x4 res;
    res.col0 = vec4_div( lhs->col0, rhs );
    res.col1 = vec4_div( lhs->col1, rhs );
    res.col2 = vec4_div( lhs->col2, rhs );
    res.col3 = vec4_div( lhs->col3, rhs );
    return res;
}

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)

attr_hot attr_internal attr_always_inline inline
struct Matrix4x4 internal_mat4_add_sse(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
    __m128 lhs0 = _mm_loadu_ps( lhs->col0.array );
    __m128 lhs1 = _mm_loadu_ps( lhs->col1.array );
    __m128 lhs2 = _mm_loadu_ps( lhs->col2.array );
    __m128 lhs3 = _mm_loadu_ps( lhs->col3.array );

    __m128 rhs0 = _mm_loadu_ps( rhs->col0.array );
    __m128 rhs1 = _mm_loadu_ps( rhs->col1.array );
    __m128 rhs2 = _mm_loadu_ps( rhs->col2.array );
    __m128 rhs3 = _mm_loadu_ps( rhs->col3.array );

    __m128 res0 = _mm_add_ps( lhs0, rhs0 );
    __m128 res1 = _mm_add_ps( lhs1, rhs1 );
    __m128 res2 = _mm_add_ps( lhs2, rhs2 );
    __m128 res3 = _mm_add_ps( lhs3, rhs3 );

    struct Matrix4x4 res;
    _mm_storeu_ps( res.col0.array, res0 );
    _mm_storeu_ps( res.col1.array, res1 );
    _mm_storeu_ps( res.col2.array, res2 );
    _mm_storeu_ps( res.col3.array, res3 );

    return res;
}
attr_hot attr_internal attr_always_inline inline
struct Matrix4x4 internal_mat4_sub_sse(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
    __m128 lhs0 = _mm_loadu_ps( lhs->col0.array );
    __m128 lhs1 = _mm_loadu_ps( lhs->col1.array );
    __m128 lhs2 = _mm_loadu_ps( lhs->col2.array );
    __m128 lhs3 = _mm_loadu_ps( lhs->col3.array );

    __m128 rhs0 = _mm_loadu_ps( rhs->col0.array );
    __m128 rhs1 = _mm_loadu_ps( rhs->col1.array );
    __m128 rhs2 = _mm_loadu_ps( rhs->col2.array );
    __m128 rhs3 = _mm_loadu_ps( rhs->col3.array );

    __m128 res0 = _mm_sub_ps( lhs0, rhs0 );
    __m128 res1 = _mm_sub_ps( lhs1, rhs1 );
    __m128 res2 = _mm_sub_ps( lhs2, rhs2 );
    __m128 res3 = _mm_sub_ps( lhs3, rhs3 );

    struct Matrix4x4 res;
    _mm_storeu_ps( res.col0.array, res0 );
    _mm_storeu_ps( res.col1.array, res1 );
    _mm_storeu_ps( res.col2.array, res2 );
    _mm_storeu_ps( res.col3.array, res3 );

    return res;
}
attr_hot attr_internal attr_always_inline inline
struct Matrix4x4 internal_mat4_mul_sse(
    const struct Matrix4x4* lhs, f32 rhs
) {
    __m128 lhs0 = _mm_loadu_ps( lhs->col0.array );
    __m128 lhs1 = _mm_loadu_ps( lhs->col1.array );
    __m128 lhs2 = _mm_loadu_ps( lhs->col2.array );
    __m128 lhs3 = _mm_loadu_ps( lhs->col3.array );

    __m128 _rhs = _mm_set1_ps( rhs );

    __m128 res0 = _mm_mul_ps( lhs0, _rhs );
    __m128 res1 = _mm_mul_ps( lhs1, _rhs );
    __m128 res2 = _mm_mul_ps( lhs2, _rhs );
    __m128 res3 = _mm_mul_ps( lhs3, _rhs );

    struct Matrix4x4 res;
    _mm_storeu_ps( res.col0.array, res0 );
    _mm_storeu_ps( res.col1.array, res1 );
    _mm_storeu_ps( res.col2.array, res2 );
    _mm_storeu_ps( res.col3.array, res3 );

    return res;
}
attr_hot attr_internal attr_always_inline inline
struct Matrix4x4 internal_mat4_div_sse(
    const struct Matrix4x4* lhs, f32 rhs
) {
    __m128 lhs0 = _mm_loadu_ps( lhs->col0.array );
    __m128 lhs1 = _mm_loadu_ps( lhs->col1.array );
    __m128 lhs2 = _mm_loadu_ps( lhs->col2.array );
    __m128 lhs3 = _mm_loadu_ps( lhs->col3.array );

    __m128 _rhs = _mm_set1_ps( rhs );

    __m128 res0 = _mm_div_ps( lhs0, _rhs );
    __m128 res1 = _mm_div_ps( lhs1, _rhs );
    __m128 res2 = _mm_div_ps( lhs2, _rhs );
    __m128 res3 = _mm_div_ps( lhs3, _rhs );

    struct Matrix4x4 res;
    _mm_storeu_ps( res.col0.array, res0 );
    _mm_storeu_ps( res.col1.array, res1 );
    _mm_storeu_ps( res.col2.array, res2 );
    _mm_storeu_ps( res.col3.array, res3 );

    return res;
}

attr_hot attr_internal attr_always_inline inline
struct Matrix4x4 internal_mat4_mul_mat4_sse(
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

    _mm_storeu_ps( res.array + (0 * 4), col0_res );

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

    _mm_storeu_ps( res.array + (1 * 4), col1_res );

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

    _mm_storeu_ps( res.array + (2 * 4), col2_res );

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

    _mm_storeu_ps( res.array + (3 * 4), col3_res );

    return res;
}


#endif

attr_core_api
struct Matrix4x4 mat4_add(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_mat4_add_sse( lhs, rhs );
#else
    return internal_mat4_add_scalar( lhs, rhs );
#endif
}
attr_core_api
struct Matrix4x4 mat4_sub(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_mat4_sub_sse( lhs, rhs );
#else
    return internal_mat4_sub_scalar( lhs, rhs );
#endif
}
attr_core_api
struct Matrix4x4 mat4_mul(
    const struct Matrix4x4* lhs, f32 rhs
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_mat4_mul_sse( lhs, rhs );
#else
    return internal_mat4_mul_scalar( lhs, rhs );
#endif
}
attr_core_api
struct Matrix4x4 mat4_div(
    const struct Matrix4x4* lhs, f32 rhs
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_mat4_div_sse( lhs, rhs );
#else
    return internal_mat4_div_scalar( lhs, rhs );
#endif
}
attr_core_api
struct Matrix4x4 mat4_mul_mat4(
    const struct Matrix4x4* lhs, const struct Matrix4x4* rhs
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_mat4_mul_mat4_sse( lhs, rhs );
#else
    return internal_mat4_mul_mat4_scalar( lhs, rhs );
#endif
}
attr_core_api
struct Vector4 mat4_mul_vec4(
    const struct Matrix4x4* lhs, const struct Vector4 rhs
) {
    // NOTE(alicia): sse does not seem to help in this case. :(
    return vec4(
        (lhs->array[0] * rhs.array[0]) + (lhs->array[4] * rhs.array[1]) + (lhs->array[ 8] * rhs.array[2]) + (lhs->array[12] * rhs.array[3]),
        (lhs->array[1] * rhs.array[0]) + (lhs->array[5] * rhs.array[1]) + (lhs->array[ 9] * rhs.array[2]) + (lhs->array[13] * rhs.array[3]),
        (lhs->array[2] * rhs.array[0]) + (lhs->array[6] * rhs.array[1]) + (lhs->array[10] * rhs.array[2]) + (lhs->array[14] * rhs.array[3]),
        (lhs->array[3] * rhs.array[0]) + (lhs->array[7] * rhs.array[1]) + (lhs->array[11] * rhs.array[2]) + (lhs->array[15] * rhs.array[3])
    );
}
attr_core_api
f32 mat4_determinant( const struct Matrix4x4* m ) {
    struct Matrix3x3 sub0 = mat4_submatrix( m, 0, 0 );
    struct Matrix3x3 sub1 = mat4_submatrix( m, 1, 0 );
    struct Matrix3x3 sub2 = mat4_submatrix( m, 2, 0 );
    struct Matrix3x3 sub3 = mat4_submatrix( m, 3, 0 );

    return
        ( m->array[ 0] * mat3_determinant( &sub0 ) ) -
        ( m->array[ 4] * mat3_determinant( &sub1 ) ) +
        ( m->array[ 8] * mat3_determinant( &sub2 ) ) -
        ( m->array[12] * mat3_determinant( &sub3 ) );
}
attr_core_api
struct Matrix3x3 mat4_submatrix(
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

            res.array[i++] = m->col[c].array[r];
        }
    }

    return res;
}
attr_core_api
f32 mat4_cofactor( const struct Matrix4x4* m, u32 column, u32 row ) {
    f32 minor = mat4_minor( m, column, row );
    i32 exp   = ( row + 1 ) + ( column + 1 );
    return minor * f32_powi( -1.0f, exp );
}
attr_core_api
struct Matrix4x4 mat4_cofactor_matrix( const struct Matrix4x4* m ) {
    return mat4_new(
        mat4_cofactor( m, 0, 0 ), mat4_cofactor( m, 0, 1 ), mat4_cofactor( m, 0, 2 ), mat4_cofactor( m, 0, 3 ),
        mat4_cofactor( m, 1, 0 ), mat4_cofactor( m, 1, 1 ), mat4_cofactor( m, 1, 2 ), mat4_cofactor( m, 1, 3 ),
        mat4_cofactor( m, 2, 0 ), mat4_cofactor( m, 2, 1 ), mat4_cofactor( m, 2, 2 ), mat4_cofactor( m, 2, 3 ),
        mat4_cofactor( m, 3, 0 ), mat4_cofactor( m, 3, 1 ), mat4_cofactor( m, 3, 2 ), mat4_cofactor( m, 3, 3 ) );
}
attr_core_api
b32 mat4_inverse_checked(
    const struct Matrix4x4* m, struct Matrix4x4* out_inverse
) {
    f32 det = mat4_determinant( m );
    if( det == 0.0f ) {
        return false;
    }

    struct Matrix4x4 adjoint = mat4_adjoint( m );
    *out_inverse = mat4_div( &adjoint, det );
    return true;
}
attr_core_api
struct Matrix4x4 mat4_inverse( const struct Matrix4x4* m ) {
    f32 det = mat4_determinant( m );
    struct Matrix4x4 adjoint = mat4_adjoint( m );
    return mat4_div( &adjoint, det );
}
attr_core_api
b32 mat4_normal_matrix_checked(
    const struct Matrix4x4* m, struct Matrix3x3* out_normal
) {
    struct Matrix4x4 inverse;

    if( !mat4_inverse_checked( m, &inverse ) ) {
        return false;
    }

    *out_normal = mat3_new(
        inverse.array[0], inverse.array[4], inverse.array[ 8],
        inverse.array[1], inverse.array[5], inverse.array[ 9],
        inverse.array[2], inverse.array[6], inverse.array[10] );
    return true;
}
attr_core_api
struct Matrix3x3 mat4_normal_matrix( const struct Matrix4x4* m ) {
    struct Matrix4x4 inverse = mat4_inverse( m );
    return mat3_new(
        inverse.array[0], inverse.array[4], inverse.array[ 8],
        inverse.array[1], inverse.array[5], inverse.array[ 9],
        inverse.array[2], inverse.array[6], inverse.array[10] );
}
attr_core_api
struct Matrix4x4 mat4_view(
    struct Vector3 position, struct Vector3 target,
    struct Vector3 up
) {
    struct Vector3 z = vec3_normalize( vec3_sub( target, position ) );
    struct Vector3 x = vec3_normalize( vec3_cross( z, up ) );
    struct Vector3 y = vec3_cross( x, z );
    z = vec3_neg( z );

    f32 dx = -vec3_dot( x, position );
    f32 dy = -vec3_dot( y, position );
    f32 dz = -vec3_dot( z, position );

    return mat4_new(
        x.x, y.x, z.x, 0.0f,
        x.y, y.y, z.y, 0.0f,
        x.z, y.z, z.z, 0.0f,
         dx,  dy,  dz, 1.0f );
}
attr_core_api
struct Matrix4x4 mat4_ortho(
    f32 left, f32 right,
    f32 bottom, f32 top,
    f32 clip_near, f32 clip_far
) {
    struct Matrix4x4 res = MAT4_IDENTITY;
    f32 rl = right - left;
    f32 tb = top - bottom;
    f32 fn = clip_far - clip_near;

    res.array[ 0] =  2.0f / rl;
    res.array[ 5] =  2.0f / tb;
    res.array[10] = -2.0f / fn;
    res.array[12] = -( right + left ) / rl;
    res.array[13] = -( top + bottom ) / tb;
    res.array[14] = -( clip_far + clip_near ) / fn;

    return res;
}
attr_core_api
struct Matrix4x4 mat4_perspective(
    f32 fov, f32 aspect_ratio, f32 clip_near, f32 clip_far
) {
    struct Matrix4x4 res = MAT4_ZERO;
    
    f32 half_fov_tan = f32_tan( fov / 2.0f );

    res.array[ 0] = 1.0f / ( aspect_ratio * half_fov_tan );
    res.array[ 5] = 1.0f / half_fov_tan;
    res.array[10] = -( ( clip_far + clip_near ) / ( clip_far - clip_near ) );
    res.array[11] = -1.0f;
    res.array[14] =
        -( ( 2.0f * clip_far * clip_near ) / ( clip_far - clip_near ) );

    return res;
}
attr_core_api
struct Matrix4x4 mat4_rotation_pitch( f32 pitch ) {
    f32 sin, cos;
    f32_sincos( pitch, &sin, &cos );

    return mat4_new(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f,  cos,  sin, 0.0f,
        0.0f, -sin,  cos, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f );
}
attr_core_api
struct Matrix4x4 mat4_rotation_yaw( f32 yaw ) {
    f32 sin, cos;
    f32_sincos( yaw, &sin, &cos );

    return mat4_new(
         cos, 0.0f, -sin, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
         sin, 0.0f,  cos, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f );
}
attr_core_api
struct Matrix4x4 mat4_rotation_roll( f32 roll ) {
    f32 sin, cos;
    f32_sincos( roll, &sin, &cos );

    return mat4_new(
         cos,  sin, 0.0f, 0.0f,
        -sin,  cos, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f );
}
attr_core_api
struct Matrix4x4 mat4_rotation_euler( f32 pitch, f32 yaw, f32 roll ) {
    struct Matrix4x4 pitch_m = mat4_rotation_pitch( pitch );
    struct Matrix4x4 yaw_m   = mat4_rotation_yaw( yaw );
    struct Matrix4x4 roll_m  = mat4_rotation_roll( roll );

    struct Matrix4x4 temp = mat4_mul_mat4( &yaw_m, &roll_m );
    return mat4_mul_mat4( &pitch_m, &temp );
}
attr_core_api
struct Matrix4x4 mat4_rotation( struct Quaternion rotation ) {
    struct Matrix4x4 res = MAT4_IDENTITY;

    f32 _2x2 = 2.0f * (rotation.x * rotation.x);
    f32 _2y2 = 2.0f * (rotation.y * rotation.y);
    f32 _2z2 = 2.0f * (rotation.z * rotation.z);

    f32 _2xy = 2.0f * (rotation.x * rotation.y);
    f32 _2xz = 2.0f * (rotation.x * rotation.z);
    f32 _2yz = 2.0f * (rotation.y * rotation.z);
    f32 _2wx = 2.0f * (rotation.w * rotation.x);
    f32 _2wy = 2.0f * (rotation.w * rotation.y);
    f32 _2wz = 2.0f * (rotation.w * rotation.z);

    res.array[0] = 1.0f - _2y2 - _2z2;
    res.array[1] = _2xy + _2wz;
    res.array[2] = _2xz - _2wy;

    res.array[4] = _2xy - _2wz;
    res.array[5] = 1.0f - _2x2 - _2z2;
    res.array[6] = _2yz + _2wx;

    res.array[8]  = _2xz + _2wy;
    res.array[9]  = _2yz - _2wx;
    res.array[10] = 1.0f - _2x2 - _2y2;

    return res;
}
attr_core_api
struct Matrix4x4 mat4_transform(
    struct Vector3 translation, struct Quaternion rotation,
    struct Vector3 scale
) {
    struct Matrix4x4 t = mat4_translation_vec3( translation );
    struct Matrix4x4 r = mat4_rotation( rotation );
    struct Matrix4x4 s = mat4_scale_vec3( scale );

    struct Matrix4x4 temp = mat4_mul_mat4( &r, &s );
    return mat4_mul_mat4( &t, &temp );
}
attr_core_api
struct Matrix4x4 mat4_transform_euler(
    struct Vector3 translation, struct Vector3 rotation,
    struct Vector3 scale
) {
    struct Matrix4x4 t = mat4_translation_vec3( translation );
    struct Matrix4x4 r = mat4_rotation_euler_vec3( rotation );
    struct Matrix4x4 s = mat4_scale_vec3( scale );

    struct Matrix4x4 temp = mat4_mul_mat4( &r, &s );
    return mat4_mul_mat4( &t, &temp );
}

attr_core_api
struct Matrix2x2 mat2_from_array( const f32 array[4] ) {
    struct Matrix2x2 result;
    memory_copy( result.array, array, sizeof(f32) * 4 );
    return result;
}
attr_core_api
void mat2_to_array( struct Matrix2x2 m, f32* out_array ) {
    memory_copy( out_array, m.array, sizeof(f32) * 4 );
}
attr_core_api
struct Matrix3x3 mat3_from_array( const f32 array[9] ) {
    struct Matrix3x3 result;
    memory_copy( result.array, array, sizeof(f32) * 9 );
    return result;
}
attr_core_api
void mat3_to_array( const struct Matrix3x3* m, f32* out_array ) {
    memory_copy( out_array, m, sizeof(f32) * 9 );
}
attr_core_api
struct Matrix4x4 mat4_from_array( const f32 array[16] ) {
    struct Matrix4x4 result;
    memory_copy( result.array, array, sizeof(f32) * 16 );
    return result;
}
attr_core_api
void mat4_to_array( const struct Matrix4x4* m, f32* out_array ) {
    memory_copy( out_array, m, sizeof(f32) * 16 );
}

