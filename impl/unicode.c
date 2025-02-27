/**
 * @file   unicode.c
 * @brief  Unicode implementation.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   October 02, 2024
*/
#include "core/unicode.h"

// TODO(alicia): 

attr_core_api
UnicodeValidationResult unicode_utf8_validate(
    usize len, const c8* utf8, rune32* opt_out_rune,
    const c8** opt_out_error, u32* opt_out_advance
) {
    const u8* uc = (const u8*)utf8;
    if( !len ) {
        return UNICODE_RESULT_UNFINISHED;
    }

    if( !(uc[0] & ~0x7F) ) {
        if( opt_out_rune ) {
            *opt_out_rune = uc[0];
        }
        if( opt_out_advance ) {
            *opt_out_advance = 1;
        }
        return UNICODE_RESULT_OK;
    } else if( (uc[0] & 0xE0) == 0xC0 ) {
        if( len >= 2 ) {
            rune32 rune = uc[0] & 0x1F;

            if( (uc[1] & 0xC0) != 0x80 ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 1;
                }
                return UNICODE_RESULT_INVALID;
            }

            rune = ( rune << 6 ) | ( uc[1] & 0x3F );

            if( opt_out_rune ) {
                *opt_out_rune = rune;
            }
            if( opt_out_advance ) {
                *opt_out_advance = 2;
            }
            return UNICODE_RESULT_OK;
        } else {
            if( opt_out_advance ) {
                *opt_out_advance = 2 - len;
            }
            return UNICODE_RESULT_UNFINISHED;
        }
    } else if( (uc[0] & 0xF0) == 0xE0 ) {
        if( len >= 3 ) {
            rune32 rune = uc[0] & 0x0F;

            if( (uc[1] & 0xC0) != 0x80 ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 1;
                }
                return UNICODE_RESULT_INVALID;
            }
            rune = ( rune << 6 ) | ( uc[1] & 0x3F );

            if( (uc[2] & 0xC0) != 0x80 ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 2;
                }
                return UNICODE_RESULT_INVALID;
            }
            rune = ( rune << 6 ) | ( uc[2] & 0x3F );

            if( opt_out_rune ) {
                *opt_out_rune = rune;
            }
            if( opt_out_advance ) {
                *opt_out_advance = 3;
            }
            return UNICODE_RESULT_OK;
        } else {
            if( opt_out_advance ) {
                *opt_out_advance = 3 - len;
            }
            return UNICODE_RESULT_UNFINISHED;
        }
    } else if( (uc[0] & 0xF8) == 0xF0 ) {
        if( len >= 4 ) {
            rune32 rune = uc[0] & 0x07;

            if( (uc[1] & 0xC0) != 0x80 ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 1;
                }
                return UNICODE_RESULT_INVALID;
            }
            rune = ( rune << 6 ) | ( uc[1] & 0x37 );

            if( (uc[2] & 0xC0) != 0x80 ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 2;
                }
                return UNICODE_RESULT_INVALID;
            }
            rune = ( rune << 6 ) | ( uc[2] & 0x37 );

            if( (uc[3] & 0xC0) != 0x80 ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 3;
                }
                return UNICODE_RESULT_INVALID;
            }
            rune = ( rune << 6 ) | ( uc[3] & 0x37 );

            if( rune > 0x10FFFF ) {
                if( opt_out_error ) {
                    *opt_out_error = utf8 + 3;
                }
                return UNICODE_RESULT_INVALID;
            }

            if( opt_out_advance ) {
                *opt_out_advance = 4;
            }
            return UNICODE_RESULT_OK;
        } else {
            if( opt_out_advance ) {
                *opt_out_advance = 4 - len;
            }
            return UNICODE_RESULT_UNFINISHED;
        }
    }

    if( opt_out_error ) {
        *opt_out_error = utf8;
    }
    return UNICODE_RESULT_INVALID;
}
attr_core_api
UnicodeValidationResult unicode_utf16_validate(
    usize len, const c16* utf16, rune32* opt_out_rune,
    const c16** opt_out_error, u32* opt_out_advance
) {
    if( !len ) {
        return UNICODE_RESULT_UNFINISHED;
    }

    if( utf16[0] <= 0xD7FF ) {
        // FIXME(alicia): may still be invalid
        rune32 rune = utf16[0];

        if( opt_out_rune ) {
            *opt_out_rune = rune;
        }
        if( opt_out_advance ) {
            *opt_out_advance = 1;
        }
        return UNICODE_RESULT_OK;
    } else if( utf16[0] <= 0xDBFF ) {
        if( len >= 2 ) {
            rune32 rune = 0;

            u16 hi = ( utf16[0] - 0xD800 ) * 0x400;
            u16 lo = ( utf16[1] - 0xDC00 );

            rune = (lo | hi) + 0x10000;

            if( rune > 0x10FFFF ) {
                if( opt_out_error ) {
                    *opt_out_error = utf16 + 2;
                }
                return UNICODE_RESULT_INVALID;
            }
            // FIXME(alicia): may still be invalid

            if( opt_out_rune ) {
                *opt_out_rune = rune;
            }
            if( opt_out_advance ) {
                *opt_out_advance = 2;
            }
            return UNICODE_RESULT_OK;
        } else {
            if( opt_out_advance ) {
                *opt_out_advance = 2 - len;
            }
            return UNICODE_RESULT_UNFINISHED;
        }
    }

    if( opt_out_error ) {
        *opt_out_error = utf16;
    }
    return UNICODE_RESULT_INVALID;
}
attr_core_api
UnicodeValidationResult unicode_utf32_validate(
    usize len, const c32* utf32, rune32* opt_out_rune,
    const c32** opt_out_error, u32* opt_out_advance
) {
    if( !len ) {
        return UNICODE_RESULT_UNFINISHED;
    }

    if( *utf32 > 0x10FFFF ) {
        if( opt_out_error ) {
            *opt_out_error = utf32;
        }
        return UNICODE_RESULT_INVALID;
    }

    // FIXME(alicia): may still be invalid

    if( opt_out_rune ) {
        *opt_out_rune = *utf32;
    }
    if( opt_out_advance ) {
        *opt_out_advance = 1;
    }
    return UNICODE_RESULT_OK;
}

attr_core_api
u32 unicode_cp8_from_string( usize len, const c8* utf8, UTFCodePoint8* out_cp8 ) {
    if( !len ) {
        return 0;
    }

    const u8* uc = (const u8*)utf8;

    if( !(uc[0] & ~0x7F) ) {
        *out_cp8 = unicode_cp8_from_code_units( uc[0], 0, 0, 0 );
        return 1;
    } else if( (uc[0] & 0xE0) == 0xC0 ) {
        if( len >= 2 ) {
            *out_cp8 = unicode_cp8_from_code_units( uc[0], uc[1], 0, 0 );
            return 2;
        } else {
            goto unicode_cp8_from_string_error;
        }
    } else if( (uc[0] & 0xF0) == 0xE0 ) {
        if( len >= 3 ) {
            *out_cp8 = unicode_cp8_from_code_units( uc[0], uc[1], uc[2], 0 );
            return 3;
        } else {
            goto unicode_cp8_from_string_error;
        }
    } else if( (uc[0] & 0xF8) == 0xF0 ) {
        if( len >= 4 ) {
            *out_cp8 = unicode_cp8_from_code_units( uc[0], uc[1], uc[2], uc[3] );
            return 4;
        } else {
            goto unicode_cp8_from_string_error;
        }
    }

unicode_cp8_from_string_error:
    *out_cp8 = UNICODE_CP8_REPLACEMENT_CHARACTER;
    return 1;
}
attr_core_api
u32 unicode_cp16_from_string( usize len, const c16* utf16, UTFCodePoint16* out_cp16 ) {
    if( !len ) {
        return 0;
    }

    if( utf16[0] <= 0xD7FF ) {
        *out_cp16 = unicode_cp16_from_code_units( utf16[0], 0 );
        return 1;
    } else if( utf16[0] <= 0xDBFF ) {
        if( len >= 2 ) {
            *out_cp16 = unicode_cp16_from_code_units( utf16[0], utf16[1] );
            return 2;
        } else {
            goto unicode_cp16_from_string_error;
        }
    }

unicode_cp16_from_string_error:
    *out_cp16 = UNICODE_CP16_REPLACEMENT_CHARACTER;
    return 1;
}
attr_core_api
u32 unicode_cp32_from_string( usize len, const c32* utf32, UTFCodePoint32* out_cp32 ) {
    if( !len ) {
        return 0;
    }

    if( *utf32 > 0x10FFFF ) {
        *out_cp32 = UNICODE_CP32_REPLACEMENT_CHARACTER;
        return 1;
    }

    out_cp32->rune = *utf32;
    return 1;
}

attr_core_api
usize unicode_utf8_from_utf16(
    StreamBytesFN* stream, void* target, usize len, const c16* string
) {
    usize result = 0;

    const c16* utf16     = string;
    usize      remaining = len;
    while( remaining ) {
        UTFCodePoint16 cp16 = {};
        usize advance = unicode_cp16_from_string( remaining, utf16, &cp16 );

        UTFCodePoint8 cp8 = unicode_cp8_from_cp16( cp16 );

        result += stream( target, unicode_cp8_code_unit_count( cp8 ), cp8.code_units );

        utf16     += advance;
        remaining -= advance;
    }

    return result;
}
attr_core_api
usize unicode_utf8_from_utf32(
    StreamBytesFN* stream, void* target, usize len, const c32* string
) {
    usize result = 0;

    const c32* utf32     = string;
    usize      remaining = len;
    while( remaining ) {
        UTFCodePoint32 cp32 = {};
        usize advance = unicode_cp32_from_string( remaining, utf32, &cp32 );

        UTFCodePoint8 cp8 = unicode_cp8_from_cp32( cp32 );

        result += stream( target, unicode_cp8_code_unit_count( cp8 ), cp8.code_units );

        utf32     += advance;
        remaining -= advance;
    }

    return result;
}
attr_core_api
usize unicode_utf16_from_utf8(
    StreamBytesFN* stream, void* target, usize len, const c8* string
) {
    usize result = 0;

    const u8* utf8      = (const u8*)string;
    usize     remaining = len;
    while( remaining ) {
        UTFCodePoint8 cp8 = {};
        usize advance = unicode_cp8_from_string( remaining, utf8, &cp8 );

        UTFCodePoint16 cp16 = unicode_cp16_from_cp8( cp8 );

        result += stream(
            target, sizeof(u16) * unicode_cp16_code_unit_count( cp16 ), cp16.code_units );

        utf8      += advance;
        remaining -= advance;
    }

    return result;
}
attr_core_api
usize unicode_utf16_from_utf32(
    StreamBytesFN* stream, void* target, usize len, const c32* string
) {
    usize result = 0;

    const c32* utf32     = string;
    usize      remaining = len;
    while( remaining ) {
        UTFCodePoint32 cp32 = {};
        usize advance = unicode_cp32_from_string( remaining, utf32, &cp32 );

        UTFCodePoint16 cp16 = unicode_cp16_from_cp32( cp32 );

        result += stream(
            target, sizeof(u16) * unicode_cp16_code_unit_count( cp16 ), cp16.code_units );

        utf32     += advance;
        remaining -= advance;
    }

    return result;
}
attr_core_api
usize unicode_utf32_from_utf8(
    StreamBytesFN* stream, void* target, usize len, const c8* string
) {
    usize result = 0;

    const u8* utf8      = (const u8*)string;
    usize     remaining = len;
    while( remaining ) {
        UTFCodePoint8 cp8 = {};
        usize advance = unicode_cp8_from_string( remaining, utf8, &cp8 );

        UTFCodePoint32 cp32 = unicode_cp32_from_cp8( cp8 );

        result += stream(
            target, sizeof(u32) * unicode_cp32_code_unit_count( cp32 ), cp32.code_units );

        utf8      += advance;
        remaining -= advance;
    }

    return result;
}
attr_core_api
usize unicode_utf32_from_utf16(
    StreamBytesFN* stream, void* target, usize len, const c16* string 
) {
    usize result = 0;

    const c16* utf16     = string;
    usize      remaining = len;
    while( remaining ) {
        UTFCodePoint16 cp16 = {};
        usize advance = unicode_cp16_from_string( remaining, utf16, &cp16 );

        UTFCodePoint32 cp32 = unicode_cp32_from_cp16( cp16 );

        result += stream(
            target, sizeof(u32) * unicode_cp32_code_unit_count( cp32 ), cp32.code_units );

        utf16     += advance;
        remaining -= advance;
    }

    return result;
}
attr_core_api
b32 unicode_is_mathematical( rune32 rune ) {
    return
        ((rune >= 0x02190) && (rune <= 0x021FF)) || // Arrows
        ((rune >= 0x1D400) && (rune <= 0x1D7FF)) || // Mathematical alphanumeric symbols
        ((rune >= 0x02200) && (rune <= 0x022FF)) || // Mathematical operators
        ((rune >= 0x025A0) && (rune <= 0x025FF)) || // Geometric shapes
        0;
}
attr_core_api
b32 unicode_is_alphabetic_upper( rune32 rune ) {
    if( unicode_is_ascii( rune ) ) {
        return ascii_is_alphabetic_upper( rune );
    }
    // TODO(alicia): check against other writing systems.
    return false;
}
attr_core_api
b32 unicode_is_alphabetic_lower( rune32 rune ) {
    if( unicode_is_ascii( rune ) ) {
        return ascii_is_alphabetic_lower( rune );
    }
    // TODO(alicia): check against other writing systems.
    return false;
}
attr_core_api
b32 unicode_is_alphabetic( rune32 rune ) {
    if( unicode_is_ascii( rune ) ) {
        return ascii_is_alphabetic( rune );
    }
    // TODO(alicia): check against other writing systems.
    return false;
}
attr_core_api
b32 unicode_is_emoji( rune32 rune ) {
    return 
        ((rune >= 0x02700) && (rune <= 0x027BF)) || // Dingbats
        ((rune >= 0x1F600) && (rune <= 0x1F64F)) || // Emoticons
        ((rune >= 0x02600) && (rune <= 0x026FF)) || // Miscellaneous Symbols
        ((rune >= 0x1F300) && (rune <= 0x1F5FF)) || // Miscellaneous Symbols and Pictographs
        ((rune >= 0x1F900) && (rune <= 0x1F9FF)) || // Supplemental Symbols and Pictographs
        ((rune >= 0x1FA70) && (rune <= 0x1FAFF)) || // Symbols and Pictographs Extended-A
        ((rune >= 0x1F680) && (rune <= 0x1F6FF)) || // Transport and Map Symbols
        0;
}
attr_core_api
rune32 unicode_to_upper( rune32 rune ) {
    if( unicode_is_ascii( rune ) ) {
        return ascii_to_upper( rune );
    }
    // TODO(alicia): 
    return rune;
}
attr_core_api
rune32 unicode_to_lower( rune32 rune ) {
    if( unicode_is_ascii( rune ) ) {
        return ascii_to_lower( rune );
    }
    // TODO(alicia): 
    return rune;
}
attr_core_api
GraphemeCluster unicode_cluster_to_upper( GraphemeCluster cluster ) {
    // TODO(alicia): 
    return cluster;
}
attr_core_api
GraphemeCluster unicode_cluster_to_lower( GraphemeCluster cluster ) {
    // TODO(alicia): 
    return cluster;
}

attr_core_api
usize ascii_from_utf8(
    StreamBytesFN* stream, void* target,
    usize len, const c8* string,
    char opt_replacement_character 
) {
    usize result = 0;

    const c8* utf8  = string;
    usize remaining = len;
    while( remaining ) {
        UTFCodePoint8 cp8 = {};
        u32 advance = unicode_cp8_from_string( remaining, utf8, &cp8 );

        if( !(cp8.code_units[0] & ~0x7F) ) {
            result += stream( target, 1, cp8.code_units );
        } else if( opt_replacement_character ) {
            result += stream( target, 1, &opt_replacement_character );
        }

        utf8      += advance;
        remaining -= advance;
    }

    return result;
}
attr_core_api
usize ascii_from_utf16(
    StreamBytesFN* stream, void* target,
    usize len, const c16* string,
    char opt_replacement_character
) {
    usize result = 0;

    const c16* utf16 = string;
    usize remaining  = len;
    while( remaining ) {
        UTFCodePoint16 cp16 = {};
        u32 advance = unicode_cp16_from_string( remaining, utf16, &cp16 );

        UTFCodePoint8 cp8 = unicode_cp8_from_cp16( cp16 );

        if( !(cp8.code_units[0] & ~0x7F) ) {
            result += stream( target, 1, cp8.code_units );
        } else if( opt_replacement_character ) {
            result += stream( target, 1, &opt_replacement_character );
        }

        utf16     += advance;
        remaining -= advance;
    }

    return result;
}
attr_core_api
usize ascii_from_utf32(
    StreamBytesFN* stream, void* target,
    usize len, const c32* string,
    char opt_replacement_character
) {
    usize result = 0;

    const c32* utf32 = string;
    usize remaining  = len;
    while( remaining ) {
        UTFCodePoint32 cp32 = {};
        u32 advance = unicode_cp32_from_string( remaining, utf32, &cp32 );

        UTFCodePoint8 cp8 = unicode_cp8_from_cp32( cp32 );

        if( !(cp8.code_units[0] & ~0x7F) ) {
            result += stream( target, 1, cp8.code_units );
        } else if( opt_replacement_character ) {
            result += stream( target, 1, &opt_replacement_character );
        }

        utf32     += advance;
        remaining -= advance;
    }

    return result;
}

