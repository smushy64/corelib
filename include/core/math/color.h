#if !defined(CORE_MATH_COLOR_H)
#define CORE_MATH_COLOR_H
/**
 * @file   color.h
 * @brief  Color functions and types.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 12, 2025
*/
#include "core/math/vector3.h"
#include "core/math/vector4.h"

/// @brief Create packed RGBA from RGBA.
/// @param color RGBA color.
/// @return Color packed into 32-bit integer.
attr_header attr_always_inline attr_hot
u32 packed_rgba_from_rgba( struct Vector4 color ) {
    return
        ((u32)(color.r * 255.0f) <<  0) |
        ((u32)(color.g * 255.0f) <<  8) |
        ((u32)(color.b * 255.0f) << 16) |
        ((u32)(color.a * 255.0f) << 24);
}
/// @brief Create packed RGBx from RGBA.
/// @param color RGBA color.
/// @return Color packed into 32-bit integer.
attr_header attr_always_inline attr_hot
u32 packed_rgb_from_rgb( struct Vector3 color ) {
    return packed_rgba_from_rgba( vec4_new( color.r, color.g, color.b, 1.0f ) );
}
/// @brief Create RGBA from packed RGBA.
/// @param packed Packed RGBA color.
/// @return RGBA color.
attr_header attr_always_inline attr_hot
struct Vector4 rgba_from_packed_rgba( u32 packed ) {
    struct Vector4 result;
    result.r = (f32)((packed >>  0) & 0xFF) / 255.0f;
    result.g = (f32)((packed >>  8) & 0xFF) / 255.0f;
    result.b = (f32)((packed >> 16) & 0xFF) / 255.0f;
    result.a = (f32)((packed >> 24) & 0xFF) / 255.0f;
    return result;
}
/// @brief Create RGB from packed RGBx.
/// @param packed Packed RGBx color.
/// @return RGB color.
attr_header attr_always_inline attr_hot
struct Vector3 rgb_from_packed_rgb( u32 packed ) {
    struct Vector4 result = rgba_from_packed_rgba( packed );
    return result.rgb;
}



/// @brief Create packed ABGR from RGBA.
/// @param color RGBA color.
/// @return Color packed into 32-bit integer.
attr_header attr_always_inline attr_hot
u32 packed_abgr_from_rgba( struct Vector4 color ) {
    return
        ((u32)(color.r * 255.0f) << 24) |
        ((u32)(color.g * 255.0f) << 16) |
        ((u32)(color.b * 255.0f) <<  8) |
        ((u32)(color.a * 255.0f) <<  0);
}
/// @brief Create packed BGR from RGB.
/// @param color RGB color.
/// @return Color packed into 32-bit integer.
attr_header attr_always_inline attr_hot
u32 packed_bgr_from_rgb( struct Vector3 color ) {
    return packed_abgr_from_rgba( vec4_new( color.r, color.g, color.b, 1.0f ) );
}
/// @brief Create RGBA from packed ABGR.
/// @param packed Packed ABGR color.
/// @return RGBA color.
attr_header attr_always_inline attr_hot
struct Vector4 rgba_from_packed_abgr( u32 packed ) {
    struct Vector4 result;
    result.r = (f32)((packed >> 24) & 0xFF) / 255.0f;
    result.g = (f32)((packed >> 16) & 0xFF) / 255.0f;
    result.b = (f32)((packed >>  8) & 0xFF) / 255.0f;
    result.a = (f32)((packed >>  0) & 0xFF) / 255.0f;
    return result;
}
/// @brief Create RGB from packed xBGR.
/// @param packed Packed xBGR color.
/// @return RGB color.
attr_header attr_always_inline attr_hot
struct Vector3 rgb_from_packed_bgr( u32 packed ) {
    struct Vector4 result = rgba_from_packed_abgr( packed );
    return result.rgb;
}

/// @brief Create new HSL color.
/// @param hue        Hue, 0.0 to 360.0
/// @param saturation Saturation, 0.0 to 1.0
/// @param lightness  Lightness, 0.0 to 1.0
/// @return HSL color.
attr_header attr_always_inline attr_hot
struct Vector3 hsl_new( f32 hue, f32 saturation, f32 lightness ) {
    struct Vector3 result;
    result.x = f32_wrap_degrees( hue );
    result.y = ((saturation) < (0.0) ? (0.0) : ((saturation) > (1.0) ? (1.0) : (saturation)));
    result.z = ((lightness) < (0.0) ? (0.0) : ((lightness) > (1.0) ? (1.0) : (lightness)));
    return result;
}
/// @brief Create HSL color from RGB.
/// @param color RGB color.
/// @return HSL color.
attr_core_api
struct Vector3 hsl_from_rgb( struct Vector3 color );
/// @brief Create RGB color from HSL.
/// @param color HSL color.
/// @return RGB color.
attr_core_api
struct Vector3 rgb_from_hsl( struct Vector3 color );

/// @brief Create new HSLA color.
/// @param hue        Hue, 0.0 to 360.0
/// @param saturation Saturation, 0.0 to 1.0
/// @param lightness  Lightness, 0.0 to 1.0
/// @param alpha      Alpha.
/// @return HSLA color.
attr_header attr_always_inline attr_hot
struct Vector4 hsla_new( f32 hue, f32 saturation, f32 lightness, f32 alpha ) {
    struct Vector4 result;
    result.xyz = hsl_new( hue, saturation, lightness );
    result.a   = alpha;
    return result;
}
/// @brief Create HSLA color from RGBA.
/// @param color RGBA color.
/// @return HSLA color.
attr_header attr_always_inline attr_hot
struct Vector4 hsla_from_rgba( struct Vector4 color ) {
    struct Vector4 result;
    result.xyz = hsl_from_rgb( color.rgb );
    result.a   = color.a;
    return result;
}
/// @brief Create RGBA color from HSLA.
/// @param color HSLA color.
/// @return RGBA color.
attr_header attr_always_inline attr_hot
struct Vector4 rgba_from_hsla( struct Vector4 color ) {
    struct Vector4 result;
    result.xyz = rgb_from_hsl( color.xyz );
    result.a   = color.a;
    return result;
}

/// @brief Calculate luma from linear RGB.
/// @param rgb RGB Color.
/// @return Luma.
attr_core_api
f32 luma_from_rgb( struct Vector3 rgb );

/// @brief Create sRGB from linear RGB color.
/// @param linear Linear RGB color.
/// @return sRGB.
attr_core_api
struct Vector3 srgb_from_lin( struct Vector3 linear );
/// @brief Create linear RGB color from sRGB color.
/// @param srgb sRGB color.
/// @return Linear RGB color.
attr_core_api
struct Vector3 lin_from_srgb( struct Vector3 srgb );

/// @brief Create sRGBA from linear RGBA color.
/// @param linear Linear RGBA color.
/// @return sRGBA.
attr_header attr_always_inline attr_hot
struct Vector4 srgba_from_lin( struct Vector4 linear ) {
    struct Vector4 result;
    result.rgb = srgb_from_lin( linear.rgb );
    result.a   = linear.a;
    return result;
}
/// @brief Create linear RGBA color from sRGBA color.
/// @param srgba sRGBA color.
/// @return Linear RGBA color.
attr_header attr_always_inline attr_hot
struct Vector4 lin_from_srgba( struct Vector4 srgba ) {
    struct Vector4 result;
    result.rgb = lin_from_srgb( srgba.rgb );
    result.a   = srgba.a;
    return result;
}

/// @brief Create CIE XYZ from linear RGB color.
/// @param color Linear RGB color.
/// @return CIE XYZ.
attr_core_api
struct Vector3 cie_xyz_from_rgb( struct Vector3 color );
/// @brief Create linear RGB color from CIE XYZ.
/// @param color CIE XYZ.
/// @return Linear RGB.
attr_core_api
struct Vector3 rgb_from_cie_xyz( struct Vector3 color );

/// @brief Create CIE XYZ from sRGB color.
/// @param color sRGB color.
/// @return CIE XYZ.
attr_header attr_always_inline attr_hot
struct Vector3 cie_xyz_from_srgb( struct Vector3 color ) {
    return cie_xyz_from_rgb( lin_from_srgb( color ) );
}
/// @brief Create sRGB color from CIE XYZ.
/// @param color CIE XYZ.
/// @return sRGB.
attr_header attr_always_inline attr_hot
struct Vector3 srgb_from_cie_xyz( struct Vector3 color ) {
    return srgb_from_lin( rgb_from_cie_xyz( color ) );
}

/// @brief Create CIE XYZ color from LMS color.
/// @param color LMS color.
/// @return CIE XYZ color.
attr_core_api
struct Vector3 cie_xyz_from_lms( struct Vector3 color );
/// @brief Create LMS color from CIE XYZ color.
/// @param color LMS color.
/// @return LMS color.
attr_core_api
struct Vector3 lms_from_cie_xyz( struct Vector3 color );

/// @brief Create OKLab color from LMS color.
/// @param color LMS color.
/// @return OKLab color.
attr_core_api
struct Vector3 oklab_from_lms( struct Vector3 color );
/// @brief Create LMS color from OKLab color.
/// @param color OKLab color.
/// @return LMS color.
attr_core_api
struct Vector3 lms_from_oklab( struct Vector3 color );

/// @brief Create OKLab color from CIE XYZ color.
/// @param color CIE XYZ color.
/// @return OKLab color.
attr_header attr_always_inline attr_hot
struct Vector3 oklab_from_cie_xyz( struct Vector3 color ) {
    return oklab_from_lms( lms_from_cie_xyz( color ) );
}
/// @brief Create CIE XYZ color from OKLab color.
/// @param color OKLab color.
/// @return CIE XYZ color.
attr_header attr_always_inline attr_hot
struct Vector3 cie_xyz_from_oklab( struct Vector3 color ) {
    return cie_xyz_from_lms( lms_from_oklab( color ) );
}

/// @brief Create OKLab color from sRGB color.
/// @param color sRGB color.
/// @return OKLab color.
attr_header attr_always_inline attr_hot
struct Vector3 oklab_from_srgb( struct Vector3 color ) {
    return oklab_from_cie_xyz( cie_xyz_from_srgb( color ) );
}
/// @brief Create sRGB color from OKLab color.
/// @param color OKLab color.
/// @return sRGB color.
attr_header attr_always_inline attr_hot
struct Vector3 srgb_from_oklab( struct Vector3 color ) {
    return srgb_from_cie_xyz( cie_xyz_from_oklab( color ) );
}

/// @brief Create OKLab color from linear RGB color.
/// @param color Linear RGB color.
/// @return OKLab color.
attr_header attr_always_inline attr_hot
struct Vector3 oklab_from_rgb( struct Vector3 color ) {
    return oklab_from_cie_xyz( cie_xyz_from_rgb( color ) );
}
/// @brief Create linear RGB color from OKLab color.
/// @param color OKLab color.
/// @return Linear RGB color.
attr_header attr_always_inline attr_hot
struct Vector3 rgb_from_oklab( struct Vector3 color ) {
    return rgb_from_cie_xyz( cie_xyz_from_oklab( color ) );
}

#endif /* header guard */
