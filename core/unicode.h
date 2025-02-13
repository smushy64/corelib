#if !defined(CORE_UNICODE_H)
#define CORE_UNICODE_H
/**
 * @file   unicode.h
 * @brief  Unicode functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   October 02, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/macros.h"

/// @brief UTF-8 codepoints.
typedef struct UTF8 {
    /// @brief Number of actual codepoints.
    u32 len;
    /// @brief Bytes.
    u8 bytes[4];
} UTF8;
/// @brief UTF-16 codepoints.
typedef struct UTF16 {
    /// @brief Number of actual codepoints.
    u32 len;
    union {
        /// @brief UTF-16 characters.
        u16 shorts[2];
        /// @brief Bytes.
        u8  bytes[4];
    };
} UTF16;

/// @brief UTF-32 codepoint for replacement character. 
#define UTF32_REPLACEMENT_CHARACTER 0xFFFD
/// @brief UTF-16 codepoint for replacement character. 
#define UTF16_REPLACEMENT_CHARACTER \
    struct_literal(UTF16){ .len=1,.shorts={ 0xFFFD, 0 }}
/// @brief UTF-8 codepoint for replacement character. 
#define UTF8_REPLACEMENT_CHARACTER \
    struct_literal(UTF8){ .len=3,.bytes={ 0xEF, 0xBF, 0xBD, 0 }}

/// @brief Convert UTF-8 to codepoint.
/// @param      utf8               UTF-8 characters.
/// @param[out] opt_out_read_count (optional) Pointer to write number of bytes read from UTF-8.
/// @return UTF-32 codepoint.
attr_always_inline attr_header
c32 utf8_to_codepoint(
    UTF8 utf8, usize* opt_out_read_count
) {
    if( !utf8.len ) {
        return UTF32_REPLACEMENT_CHARACTER;
    }

    usize read_count = 0;
    c32 codepoint = UTF32_REPLACEMENT_CHARACTER;
    if( utf8.bytes[0] < 0x80 ) {
        codepoint = utf8.bytes[0];
        read_count = 1;
    } else if( (utf8.bytes[0] & 0xE0) == 0xC0 && utf8.len >= 2 ) {
        codepoint = utf8.bytes[0] & 0x1F;
        codepoint = (codepoint << 6) | (utf8.bytes[1] & 0x3F);
        read_count = 2;
    } else if( (utf8.bytes[0] & 0xF0) == 0xE0 && utf8.len >= 3 ) {
        codepoint = utf8.bytes[0] & 0x0F;
        codepoint = (codepoint << 6) | (utf8.bytes[1] & 0x3F);
        codepoint = (codepoint << 6) | (utf8.bytes[2] & 0x3F);
        read_count = 3;
    } else if( (utf8.bytes[0] & 0xF8) == 0xF0 && utf8.len >= 4 ) {
        codepoint = utf8.bytes[0] & 0x07;
        codepoint = (codepoint << 6) | (utf8.bytes[1] & 0x3F);
        codepoint = (codepoint << 6) | (utf8.bytes[2] & 0x3F);
        codepoint = (codepoint << 6) | (utf8.bytes[3] & 0x3F);
        read_count = 4;
    }

    if( opt_out_read_count ) {
        *opt_out_read_count = read_count;
    }

    return codepoint;
}
/// @brief Convert UTF-32 codepoint to UTF-8 characters.
/// @param codepoint UTF-32 codepoint to convert.
/// @return UTF-8 characters.
attr_always_inline attr_header
UTF8 codepoint_to_utf8( c32 codepoint ) {
    UTF8 result = {0};
    if( codepoint <= 0x007F ) {
        result.bytes[0] = codepoint;
        result.len = 1;
    } else if( codepoint <= 0x07FF ) {
        result.bytes[0] = ((codepoint >> 6) & 0x1F) | 0xC0;
        result.bytes[1] = (codepoint & 0x3F) | 0x80;
        result.len = 2;
    } else if( codepoint <= 0xFFFF ) {
        result.bytes[0] = ((codepoint >> 12) & 0x0F) | 0xE0;
        result.bytes[1] = ((codepoint >> 6) & 0x3F) | 0x80;
        result.bytes[2] = ((codepoint) & 0x3F) | 0x80;
        result.len = 3;
    } else if( codepoint <= 0x10FFFF) {
        result.bytes[0] = ((codepoint >> 18) & 0x07) | 0xF0;
        result.bytes[1] = ((codepoint >> 12) & 0x3F) | 0x80;
        result.bytes[2] = ((codepoint >> 6) & 0x3F) | 0x80;
        result.bytes[3] = (codepoint & 0x3F) | 0x80;
        result.len = 4;
    } else {
        result = UTF8_REPLACEMENT_CHARACTER;
    }

    return result;
}
/// @brief Convert UTF-16 to codepoint.
/// @param      utf16              UTF-16 characters.
/// @param[out] opt_out_read_count (optional) Pointer to write number of shorts read from UTF-16.
/// @return UTF-32 codepoint.
attr_always_inline attr_header
c32 utf16_to_codepoint( UTF16 utf16, usize* opt_out_read_count ) {
    if( !utf16.len ) {
        return UTF32_REPLACEMENT_CHARACTER;
    }
    usize read_count = 0;
    c32 codepoint = UTF32_REPLACEMENT_CHARACTER;

    if( utf16.shorts[0] <= 0xD7FF ) {
        codepoint = utf16.shorts[0];
        read_count = 1;
    } else if( utf16.len >= 2 && utf16.shorts[0] <= 0xDBFF ) {
        c16 hi = ( utf16.shorts[0] - 0xD800 ) * 0x400;
        c16 lo = utf16.shorts[1] - 0XDC00;

        codepoint = (lo | hi) + 0x10000;
        read_count = 2;
    }

    if( opt_out_read_count ) {
        *opt_out_read_count = read_count;
    }

    return codepoint;
}
/// @brief Convert UTF-32 codepoint to UTF-16 characters.
/// @param codepoint UTF-32 codepoint to convert.
/// @return UTF-16 characters.
attr_always_inline attr_header
UTF16 codepoint_to_utf16( c32 codepoint ) {
    UTF16 result = UTF16_REPLACEMENT_CHARACTER;

    if( codepoint <= 0xFFFF ) {
        if( codepoint >= 0xD800 && codepoint <= 0xDFFF ) {
            return result;
        }
        result.shorts[0] = (u16)codepoint;
        result.len       = 1;
    } else if( codepoint <= 0x10FFFF ) {
        u32 utf32 = codepoint - 0x10000;
        result.shorts[0] = (u16)((utf32 >> 10) + 0xD800);
        result.shorts[1] = (u16)((utf32 & 0x3FF) + 0xDC00);
        result.len = 2;
    }

    return result;
}

#endif /* header guard */
