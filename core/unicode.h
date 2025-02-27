#if !defined(CORE_UNICODE_H)
#define CORE_UNICODE_H
/**
 * @file   unicode.h
 * @brief  Unicode functions.
 *
 * @details
 * Unicode categories are based on Unicode 16.0
 *
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   October 02, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/macros.h"
#include "core/stream.h"
#include "core/ascii.h"

/// @brief Unicode UTF-8 code point.
union UTFCodePoint8 {
    /// @brief Code units of UTF-8 code point.
    u8 code_units[4];
    /// @brief Bytes of UTF-8 code point.
    u8 bytes[4];
};
/// @brief Unicode UTF-16 code point.
union UTFCodePoint16 {
    /// @brief Code units of UTF-16 code point.
    u16 code_units[2];
    /// @brief Bytes of UTF-16 code point.
    /// @note
    /// Use unicode_cp16_read_byte() to access bytes as this is
    /// dependent on the current architecture's byte-order (endianess).
    u8  bytes[4];
};
/// @brief Unicode UTF-32 code point.
union UTFCodePoint32 {
    /// @brief UTF-32 code point.
    rune32 rune;
    /// @brief Code units of UTF-32 code point.
    u32    code_units[1];
    /// @brief Bytes of UTF-32 code point.
    /// @note
    /// Use unicode_cp32_read_byte() to access bytes as this is
    /// dependent on the current architecture's byte-order (endianess).
    u8     bytes[4];
};
/// @brief Unicode UTF-8 code point.
typedef union  UTFCodePoint8  UTFCodePoint8;
/// @brief Unicode UTF-16 code point.
typedef union UTFCodePoint16 UTFCodePoint16;
/// @brief Unicode UTF-32 code point.
typedef union UTFCodePoint32 UTFCodePoint32;

/// @brief Grapheme Cluster.
/// @details
/// Some lexical units require multiple runes
/// to be accurately represented.
struct GraphemeCluster {
    /// @brief Number of runes.
    usize len;
    /// @brief Union of pointers to grapheme cluster.
    union {
        /// @brief Pointer to start of grapheme cluster.
        rune32*       buf;
        /// @brief Constant pointer to start of grapheme cluster.
        const rune32* cbuf;
        /// @brief Pointer to start of grapheme cluster bytes.
        u8*           bytes;
        /// @brief Pointer to start of grapheme cluster bytes.
        void*         _void;
    };
};
typedef struct GraphemeCluster GraphemeCluster;

/// @brief Validation results.
typedef enum UnicodeValidationResult {
    /// @brief Unicode unit sequence is valid.
    UNICODE_RESULT_OK,
    /// @brief Unicode unit sequence is unfinished.
    UNICODE_RESULT_UNFINISHED,
    /// @brief Unicode unit sequence is invalid.
    UNICODE_RESULT_INVALID,
} UnicodeValidationResult;

/// @brief Construct UTF-8 code point from code units.
/// @param code_unit_0, code_unit_1, code_unit_2, code_unit_3 (cp8) Code units.
/// @return UTF-8 code point.
#define unicode_cp8_from_code_units( code_unit_0, code_unit_1, code_unit_2, code_unit_3 ) \
    (struct_literal(UTFCodePoint8){ .code_units = { \
        (code_unit_0), (code_unit_1), (code_unit_2), (code_unit_3) }})

/// @brief Construct UTF-16 code point from code units.
/// @param code_unit_0, code_unit_1 (cp16) Code units.
/// @return UTF-16 code point.
#define unicode_cp16_from_code_units( code_unit_0, code_unit_1 ) \
    (struct_literal(UTFCodePoint16){ .code_units = { \
        (code_unit_0), (code_unit_1) }})

/// @brief Construct UTF-32 code point from code units.
/// @param code_unit_0 (cp32) Code units.
/// @return UTF-32 code point.
#define unicode_cp32_from_code_units( code_unit_0 ) \
    (struct_literal(UTFCodePoint32){ .code_units = { (code_unit_0) }})

/// @brief Construct UTF-8 code point from bytes.
/// @param code_unit_0, code_unit_1, code_unit_2, code_unit_3 (c8) Code units.
/// @return UTF-8 code point.
#define unicode_cp8_from_bytes(                          \
    code_unit_0, code_unit_1, code_unit_2, code_unit_3 ) \
    (struct_literal(UTFCodePoint8){ .bytes = {           \
        (code_unit_0), (code_unit_1), (code_unit_2), (code_unit_3) } })

#if defined(CORE_ARCH_BIG_ENDIAN)

/// @brief Construct UTF-16 code point from bytes.
/// @param code_unit_0_low, code_unit_0_high (u8) First code unit, low and high byte.
/// @param code_unit_1_low, code_unit_1_high (u8) Second code unit, low and high byte.
/// @return UTF-16 code point.
#define unicode_cp16_from_bytes(                                           \
    code_unit_0_low, code_unit_0_high, code_unit_1_low, code_unit_1_high ) \
    (struct_literal(UTFCodePoint16){ .bytes = {                            \
        (code_unit_0_high), (code_unit_0_low), (code_unit_1_high), (code_unit_1_low) }})

/// @brief Construct UTF-32 code point from bytes.
/// @param low_0, low_1   (u8) Low bytes.
/// @param high_0, high_1 (u8) High bytes.
/// @return UTF-32 code point.
#define unicode_cp32_from_bytes( low_0, low_1, high_0, high_1 ) \
    (struct_literal(UTFCodePoint32){ .bytes = {                 \
        (high_1), (high_0), (low_1), (low_0) } })

/// @brief Access byte from UTF-16 code point.
/// @param[in] ptr_cp16  (UTFCodePoint16*) Pointer to code point to access.
/// @param     code_unit (int)             Index of code unit.
///                                          Either 0 or 1.
/// @param     byte      (int)             Index of byte.
///                                          Least-to-most significant byte-order.
/// @return Byte.
#define unicode_cp16_read_byte( ptr_cp16, code_unit, byte ) \
    (((u8*)((ptr_cp16)->code_units + code_unit))[1 - (byte)])

/// @brief Access byte from UTF-32 code point.
/// @param[in] ptr_cp32  (UTFCodePoint32*) Pointer to code point to access.
/// @param     byte      (int)             Index of byte.
///                                          Least-to-most significant byte-order.
/// @return Byte.
#define unicode_cp32_read_byte( ptr_cp32, byte ) \
    ((ptr_cp32)->bytes[3 - (byte)])

#else

/// @brief Construct UTF-16 code point from bytes.
/// @param code_unit_0_low, code_unit_0_high (u8) First code unit, low and high byte.
/// @param code_unit_1_low, code_unit_1_high (u8) Second code unit, low and high byte.
/// @return UTF-16 code point.
#define unicode_cp16_from_bytes(                                           \
    code_unit_0_low, code_unit_0_high, code_unit_1_low, code_unit_1_high ) \
    (struct_literal(UTFCodePoint16){ .bytes = {                            \
        (code_unit_0_low), (code_unit_0_high), (code_unit_1_low), (code_unit_1_high) }})

/// @brief Construct UTF-32 code point from bytes.
/// @param low_0, low_1   (u8) Low bytes.
/// @param high_0, high_1 (u8) High bytes.
/// @return UTF-32 code point.
#define unicode_cp32_from_bytes( low_0, low_1, high_0, high_1 ) \
    (struct_literal(UTFCodePoint32){ .bytes = {                 \
        (low_0), (low_1), (high_0), (high_1) } })

/// @brief Access byte from UTF-16 code point.
/// @param[in] ptr_cp16  (UTFCodePoint16*) Pointer to code point to access.
/// @param     code_unit (int)             Index of code unit.
///                                          Either 0 or 1.
/// @param     byte      (int)             Index of byte.
///                                          Least-to-most significant byte-order.
/// @return Byte.
#define unicode_cp16_read_byte( ptr_cp16, code_unit, byte ) \
    (((u8*)((ptr_cp16)->code_units + code_unit))[(byte)])

/// @brief Access byte from UTF-32 code point.
/// @param[in] ptr_cp32  (UTFCodePoint32*) Pointer to code point to access.
/// @param     byte      (int)             Index of byte.
///                                          Least-to-most significant byte-order.
/// @return Byte.
#define unicode_cp32_read_byte( ptr_cp32, byte ) \
    ((ptr_cp32)->bytes[(byte)])

#endif /* Little Endian */


/// @brief UTF-8 replacement character code point.
#define UNICODE_CP8_REPLACEMENT_CHARACTER \
    unicode_cp8_from_code_units( 0xEF, 0xBF, 0xBD, 0 )

/// @brief UTF-16 replacement character code point.
#define UNICODE_CP16_REPLACEMENT_CHARACTER \
    unicode_cp16_from_code_units( 0xFFFD, 0 )

/// @brief UTF-32 replacement character code point.
#define UNICODE_CP32_REPLACEMENT_CHARACTER \
    unicode_cp32_from_code_units( 0xFFFD )

/// @brief Convert UTF-8 code point to unicode rune.
/// @param cp8 UTF-8 code point.
/// @return Unicode rune. (UTF-32 encoding)
attr_always_inline attr_header
rune32 unicode_rune_from_cp8( UTFCodePoint8 cp8 );
/// @brief Convert UTF-16 code point to unicode rune.
/// @param cp16 UTF-16 code point.
/// @return Unicode rune. (UTF-32 encoding)
attr_always_inline attr_header
rune32 unicode_rune_from_cp16( UTFCodePoint16 cp16 );
/// @brief Convert UTF-32 code point to unicode rune.
///
/// @note
/// This function technically doesn't need to exist as
/// cp32 is the same as rune32.
/// That being said, if code point is invalid,
/// this function will return replacement character.
///
/// @param cp8 UTF-32 code point.
/// @return Unicode rune. (UTF-32 encoding)
attr_always_inline attr_header
rune32 unicode_rune_from_cp32( UTFCodePoint32 cp32 );

/// @brief Count number of code units in UTF-8 code point.
///
/// @note
/// This function only checks the first code unit to see how
/// many code units this code point should contain.
/// To properly validate code point, use unicode_utf8_validate().
///
/// @param cp8 UTF-8 code point.
/// @return
///     - @c 1-4 : Number of code units in code point.
///     - @c 0   : UTF-8 code point is invalid.
attr_always_inline attr_header
u32 unicode_cp8_code_unit_count( UTFCodePoint8 cp8 );
/// @brief Count number of code units in UTF-16 code point.
///
/// @note
/// This function only checks the first code unit to see how
/// many code units this code point should contain.
/// To properly validate code point, use unicode_utf16_validate().
///
/// @param cp16 UTF-16 code point.
/// @return
///     - @c 1-2 : Number of code units in sequence.
///     - @c 0   : UTF-16 sequence is invalid.
attr_always_inline attr_header
u32 unicode_cp16_code_unit_count( UTFCodePoint16 cp16 );
/// @brief Count number of code units in UTF-32 code point.
///
/// @note
/// This function only exists for parity with 
/// unicode_cp8_code_unit_count() and
/// unicode_cp16_code_unit_count().
/// To properly validate code point, use unicode_utf32_validate().
///
/// @param cp32 UTF-32 code point.
/// @return Always 1.
attr_always_inline attr_header
u32 unicode_cp32_code_unit_count( UTFCodePoint32 cp32 );

/// @brief Validate next UTF-8 code point.
/// @param      len             Length of string in code units. (bytes)
/// @param[in]  utf8            UTF-8 String. This function will read 4 code units at most.
/// @param[out] opt_out_rune    (optional) Pointer to write decoded rune if no error was encountered.
/// @param[out] opt_out_error   (optional) Pointer to write start of error if error encountered.
/// @param[out] opt_out_advance (optional) Pointer to write number of bytes to advance string by.
/// @return Result.
attr_core_api
UnicodeValidationResult unicode_utf8_validate(
    usize len, const c8* utf8, rune32* opt_out_rune,
    const c8** opt_out_error, u32* opt_out_advance );
/// @brief Validate next UTF-16 code point.
/// @param      len             Length of string in code units. (shorts)
/// @param[in]  utf16           UTF-16 String. This function will read 2 code units at most.
/// @param[out] opt_out_rune    (optional) Pointer to write decoded rune if no error was encountered.
/// @param[out] opt_out_error   (optional) Pointer to write start of error if error encountered.
/// @param[out] opt_out_advance (optional) Pointer to write number of bytes to advance string by.
/// @return Result.
attr_core_api
UnicodeValidationResult unicode_utf16_validate(
    usize len, const c16* utf16, rune32* opt_out_rune,
    const c16** opt_out_error, u32* opt_out_advance );
/// @brief Validate next UTF-32 code point.
/// @param      len             Length of string in code units. (shorts)
/// @param[in]  utf32           UTF-32 String. This function will read 1 code unit at most.
/// @param[out] opt_out_rune    (optional) Pointer to write decoded rune if no error was encountered.
/// @param[out] opt_out_error   (optional) Pointer to write start of error if error encountered.
/// @param[out] opt_out_advance (optional) Pointer to write number of bytes to advance string by.
/// @return Result.
attr_core_api
UnicodeValidationResult unicode_utf32_validate(
    usize len, const c32* utf32, rune32* opt_out_rune,
    const c32** opt_out_error, u32* opt_out_advance );

/// @brief Get next UTF-8 code point from UTF-8 string.
///
/// @details
/// If @c len is zero, function returns zero otherwise function will always return
/// a minimum of 1, even if the next code point in string is invalid.
/// UNICODE_CP8_REPLACEMENT_CHARACTER is written to @c out_cp8 if code point is invalid.
///
/// @param      len     Length of UTF-8 string in code units. (bytes)
/// @param[in]  utf8    UTF-8 String.
/// @param[out] out_cp8 Pointer to write UTF-8 code point to.
/// @return Number of code units (bytes) to advance string by.
attr_core_api
u32 unicode_cp8_from_string( usize len, const c8* utf8, UTFCodePoint8* out_cp8 );
/// @brief Get next UTF-16 code point from UTF-16 string.
///
/// @details
/// If @c len is zero, function returns zero otherwise function will always return
/// a minimum of 1, even if the next code point in string is invalid.
/// UNICODE_CP16_REPLACEMENT_CHARACTER is written to @c out_cp16 if code point is invalid.
///
/// @param      len      Length of UTF-16 string in code units. (shorts)
/// @param[in]  utf16    UTF-16 String.
/// @param[out] out_cp16 Pointer to write UTF-16 code point to.
/// @return Number of code units (shorts) to advance string by.
attr_core_api
u32 unicode_cp16_from_string( usize len, const c16* utf16, UTFCodePoint16* out_cp16 );
/// @brief Get next UTF-32 code point from UTF-32 string.
///
/// @details
/// If @c len is zero, function returns zero otherwise function will always return
/// a minimum of 1, even if the next code point in string is invalid.
/// UNICODE_CP32_REPLACEMENT_CHARACTER is written to @c out_cp32 if code point is invalid.
///
/// @param      len      Length of UTF-32 string in code units. (ints)
/// @param[in]  utf32    UTF-32 String.
/// @param[out] out_cp32 Pointer to write UTF-32 code point to.
/// @return Number of code units (ints) to advance string by.
attr_core_api
u32 unicode_cp32_from_string( usize len, const c32* utf32, UTFCodePoint32* out_cp32 );


/// @brief Convert UTF-16 code point to UTF-8 code point.
/// @param cp16 UTF-16 code point.
/// @return UTF-8 code point.
attr_always_inline attr_header
UTFCodePoint8 unicode_cp8_from_cp16( UTFCodePoint16 cp16 );
/// @brief Convert UTF-32 code point to UTF-8 code point.
/// @param cp32 UTF-32 code point.
/// @return UTF-8 code point.
attr_always_inline attr_header
UTFCodePoint8 unicode_cp8_from_cp32( UTFCodePoint32 cp32 );

/// @brief Convert UTF-8 code point to UTF-16 code point.
/// @param cp8 UTF-8 code point.
/// @return UTF-16 code point.
attr_always_inline attr_header
UTFCodePoint16 unicode_cp16_from_cp8( UTFCodePoint8 cp8 );
/// @brief Convert UTF-32 code point to UTF-16 code point.
/// @param cp32 UTF-32 code point.
/// @return UTF-16 code point.
attr_always_inline attr_header
UTFCodePoint16 unicode_cp16_from_cp32( UTFCodePoint32 cp32 );

/// @brief Convert UTF-8 code point to UTF-32 code point.
/// @param cp8 UTF-8 code point.
/// @return UTF-32 code point.
attr_always_inline attr_header
UTFCodePoint32 unicode_cp32_from_cp8( UTFCodePoint8 cp8 );
/// @brief Convert UTF-16 code point to UTF-32 code point.
/// @param cp16 UTF-16 code point.
/// @return UTF-32 code point.
attr_always_inline attr_header
UTFCodePoint32 unicode_cp32_from_cp16( UTFCodePoint16 cp16 );

/// @brief Convert UTF-16 string to UTF-8 string.
/// @param[in] stream Streaming function.
/// @param[in] target Target for streaming function.
/// @param     len    Number of UTF-16 code units in string. (shorts)
/// @param[in] string UTF-16 string.
/// @return Number of bytes that could not be streamed to target.
attr_core_api
usize unicode_utf8_from_utf16(
    StreamBytesFN* stream, void* target, usize len, const c16* string );
/// @brief Convert UTF-32 string to UTF-8 string.
/// @param[in] stream Streaming function.
/// @param[in] target Target for streaming function.
/// @param     len    Number of UTF-32 code units in string. (ints)
/// @param[in] string UTF-32 string.
/// @return Number of bytes that could not be streamed to target.
attr_core_api
usize unicode_utf8_from_utf32(
    StreamBytesFN* stream, void* target, usize len, const c32* string );

/// @brief Convert UTF-8 string to UTF-16 string.
/// @param[in] stream Streaming function.
/// @param[in] target Target for streaming function.
/// @param     len    Number of UTF-8 code units in string. (bytes)
/// @param[in] string UTF-8 string.
/// @return Number of bytes that could not be streamed to target.
attr_core_api
usize unicode_utf16_from_utf8(
    StreamBytesFN* stream, void* target, usize len, const c8* string );
/// @brief Convert UTF-32 string to UTF-16 string.
/// @param[in] stream Streaming function.
/// @param[in] target Target for streaming function.
/// @param     len    Number of UTF-32 code units in string. (ints)
/// @param[in] string UTF-32 string.
/// @return Number of bytes that could not be streamed to target.
attr_core_api
usize unicode_utf16_from_utf32(
    StreamBytesFN* stream, void* target, usize len, const c32* string );

/// @brief Convert UTF-8 string to UTF-32 string.
/// @param[in] stream Streaming function.
/// @param[in] target Target for streaming function.
/// @param     len    Number of UTF-8 code units in string. (bytes)
/// @param[in] string UTF-8 string.
/// @return Number of bytes that could not be streamed to target.
attr_core_api
usize unicode_utf32_from_utf8(
    StreamBytesFN* stream, void* target, usize len, const c8* string );
/// @brief Convert UTF-16 string to UTF-32 string.
/// @param[in] stream Streaming function.
/// @param[in] target Target for streaming function.
/// @param     len    Number of UTF-16 code units in string. (shorts)
/// @param[in] string UTF-16 string.
/// @return Number of bytes that could not be streamed to target.
attr_core_api
usize unicode_utf32_from_utf16(
    StreamBytesFN* stream, void* target, usize len, const c16* string );

/// @brief Check if unicode rune is whitespace.
///
/// @note
/// This function is the same as ascii_is_whitespace().
///
/// @param rune Unicode rune.
/// @return
///     - @c true  : Rune is whitespace character.
///     - @c false : Rune is not whitespace character.
attr_always_inline attr_header
b32 unicode_is_whitespace( rune32 rune );
/// @brief Check if unicode rune is path separator.
///
/// @note
/// This function is the same as ascii_is_path_separator().
///
/// @param rune Unicode rune.
/// @return
///     - @c true  : Rune is path separator character.
///     - @c false : Rune is not path separator character.
attr_always_inline attr_header
b32 unicode_is_path_separator( rune32 rune );
/// @brief Check if unicode rune is numeric (0-9).
///
/// @note
/// This function is the same as ascii_is_numeric().
///
/// @param rune Unicode rune to check.
/// @return
///     - @c true  : Rune is numeric.
///     - @c false : Rune is not numeric.
attr_always_inline attr_header
b32 unicode_is_numeric( rune32 rune );
/// @brief Check if unicode rune is a mathematical symbol.
///
/// @note
/// This is based on the section
/// 'Symbols and Punctuation' > 'Mathematical Symbols'
/// from
/// https://www.unicode.org/charts/
/// for version
/// Unicode 16.0
///
/// @param rune Unicode rune to check.
/// @return
///     - @c true  : Rune is mathematical symbol.
///     - @c false : Rune is not mathematical symbol.
attr_core_api
b32 unicode_is_mathematical_symbol( rune32 rune );
/// @brief Check if unicode rune is upper-case.
///
/// @note
/// This function checks against all characters used for writing.
///
/// @param rune Unicode rune to check.
/// @return
///     - @c true  : Rune is upper-case.
///     - @c false : Rune is not upper-case.
///                    It could be lower-case or a character
///                    that has no upper-case representation.
///                    e.g. Chinese characters cannot be upper or
///                    lower case so result is false.
attr_core_api
b32 unicode_is_alphabetic_upper( rune32 rune );
/// @brief Check if unicode rune is lower-case.
///
/// @note
/// This function checks against all characters used for writing.
///
/// @param rune Unicode rune to check.
/// @return
///     - @c true  : Rune is lower-case.
///     - @c false : Rune is not lower-case.
///                    It could be upper-case or a character
///                    that has no lower-case representation.
///                    e.g. Chinese characters cannot be upper or
///                    lower case so result is false.
attr_core_api
b32 unicode_is_alphabetic_lower( rune32 rune );
/// @brief Check if unicode rune is a character used for writing.
///
/// @note
/// This function checks if character is used for writing.
/// Excludes emojis or other symbolic characters.
///
/// @param rune Unicode rune to check.
/// @return
///     - @c true  : Rune is a character used for writing.
///     - @c false : Rune is numeric or some kind of symbolic character.
attr_core_api
b32 unicode_is_alphabetic( rune32 rune );
/// @brief Check if unicode rune is character used for writing or numeric.
///
/// @note
/// If you're trying to check if rune is alphabetic or 0-9,
/// use unicode_is_numeric_09() and unicode_is_alphabetic().
/// This function checks against all numeric characters.
///
/// @param rune Unicode rune to check.
/// @return
///     - @c true  : Rune is alphabetic or numeric.
///     - @c false : Rune is neither alphabetic or numeric.
attr_always_inline attr_header
b32 unicode_is_alphanumeric( rune32 rune );
/// @brief Check if unicode rune is an emoji.
///
/// @note
/// This is based on the section
/// 'Symbols and Punctuation' > 'Emoji & Pictographs'
/// from
/// https://www.unicode.org/charts/
/// for version
/// Unicode 16.0
///
/// @param rune Unicode rune to check.
/// @return
///     - @c true  : Rune is an emoji.
///     - @c false : Rune is not an emoji.
attr_core_api
b32 unicode_is_emoji( rune32 rune );
/// @brief Check if unicode rune is also ASCII.
/// @param rune Unicode rune to check.
/// @return
///     - @c true  : Rune represents an ASCII character.
///     - @c false : Rune is not ASCII.
attr_always_inline attr_header
b32 unicode_is_ascii( rune32 rune );

/// @brief Convert lower-case rune to upper-case.
///
/// @note
/// If @c rune does not have an upper-case representation,
/// function returns @c rune.
/// This function is designed only for characters
/// that can be represented with a single rune.
///
/// @param rune Unicode rune to convert.
/// @return Upper-case version.
attr_core_api
rune32 unicode_to_upper( rune32 rune );
/// @brief Convert upper-case rune to lower-case.
///
/// @note
/// If @c rune does not have an lower-case representation,
/// function returns @c rune.
/// This function is designed only for characters
/// that can be represented with a single rune.
///
/// @param rune Unicode rune to convert.
/// @return Lower-case version.
attr_core_api
rune32 unicode_to_lower( rune32 rune );

/// @brief Convert upper-case grapheme cluster to lower-case.
///
/// @note
/// If @c rune does not have an lower-case representation,
/// function returns @c cluster.
/// Return value should always be considered read-only.
///
/// @param cluster Unicode grapheme cluster to convert.
/// @return Lower-case version.
attr_core_api
GraphemeCluster unicode_cluster_to_upper( GraphemeCluster cluster );
/// @brief Convert lower-case grapheme cluster to upper-case.
///
/// @note
/// If @c rune does not have an upper-case representation,
/// function returns @c cluster.
/// Return value should always be considered read-only.
///
/// @param cluster Unicode grapheme cluster to convert.
/// @return Upper-case version.
attr_core_api
GraphemeCluster unicode_cluster_to_lower( GraphemeCluster cluster );


attr_always_inline attr_header
rune32 unicode_rune_from_cp8( UTFCodePoint8 cp8 ) {
    return unicode_cp32_from_cp8( cp8 ).rune;
}
attr_always_inline attr_header
rune32 unicode_rune_from_cp16( UTFCodePoint16 cp16 ) {
    return unicode_cp32_from_cp16( cp16 ).rune;
}
attr_always_inline attr_header
rune32 unicode_rune_from_cp32( UTFCodePoint32 cp32 ) {
    return cp32.rune;
}
attr_always_inline attr_header
u32 unicode_cp8_code_unit_count( UTFCodePoint8 cp8 ) {
    if( !(cp8.code_units[0] & ~0x7F) ) {
        return 1;
    } else if( (cp8.code_units[0] & 0xE0) == 0xC0 ) {
        return 2;
    } else if( (cp8.code_units[0] & 0xF0) == 0xE0 ) {
        return 3;
    } else if( (cp8.code_units[0] & 0xF8) == 0xF0 ) {
        return 4;
    } else {
        return 0;
    }
}
attr_always_inline attr_header
u32 unicode_cp16_code_unit_count( UTFCodePoint16 cp16 ) {
    if( cp16.code_units[0] <= 0xD7FF ) {
        return 1;
    } else if( cp16.code_units[0] <= 0xDBFF ) {
        return 2;
    } else {
        return 0;
    }
}
attr_always_inline attr_header
u32 unicode_cp32_code_unit_count( UTFCodePoint32 cp32 ) {
    // NOTE(alicia): this function only exists for
    // parity with unicode_cp8/16_code_unit_count.
    unused(cp32);
    return 1;
}
attr_always_inline attr_header
UTFCodePoint8 unicode_cp8_from_cp16( UTFCodePoint16 cp16 ) {
    // TODO(alicia): write dedicated function for this conversion.
    UTFCodePoint32 cp32 = unicode_cp32_from_cp16( cp16 );
    return unicode_cp8_from_cp32( cp32 );
}
attr_always_inline attr_header
UTFCodePoint8 unicode_cp8_from_cp32( UTFCodePoint32 cp32 ) {
    UTFCodePoint8 result;
    if( cp32.rune <= 0x007F ) {
        result = unicode_cp8_from_bytes( (c8)cp32.rune, 0, 0, 0 );
    } else if( cp32.rune <= 0x07FF ) {
        result = unicode_cp8_from_bytes(
            ((cp32.rune >> 6) & 0x1F) | 0xC0, 
            ((cp32.rune     ) & 0x3F) | 0x80,
            0, 0 );
    } else if( cp32.rune <= 0xFFFF ) {
        result = unicode_cp8_from_bytes(
            ((cp32.rune >> 12) & 0x0F) | 0xE0,
            ((cp32.rune >>  6) & 0x3F) | 0x80,
            ((cp32.rune      ) & 0x3F) | 0x80,
            0 );
    } else if( cp32.rune <= 0x10FFFF ) {
        result = unicode_cp8_from_bytes(
            ((cp32.rune >> 18) & 0x07) | 0xF0,
            ((cp32.rune >> 12) & 0x3F) | 0x80,
            ((cp32.rune >>  6) & 0x3F) | 0x80,
            ((cp32.rune      ) & 0x3F) | 0x80 );
    } else {
        result = UNICODE_CP8_REPLACEMENT_CHARACTER;
    }
    return result;
}
attr_always_inline attr_header
UTFCodePoint16 unicode_cp16_from_cp8( UTFCodePoint8 cp8 ) {
    // TODO(alicia): write dedicated function for this conversion.
    UTFCodePoint32 cp32 = unicode_cp32_from_cp8( cp8 );
    return unicode_cp16_from_cp32( cp32 );
}
attr_always_inline attr_header
UTFCodePoint16 unicode_cp16_from_cp32( UTFCodePoint32 cp32 ) {
    UTFCodePoint16 result;

    if( cp32.rune <= 0xFFFF ) {
        if( cp32.rune >= 0xD800 && cp32.rune <= 0xDFFF ) {
            result = UNICODE_CP16_REPLACEMENT_CHARACTER;
        } else {
            result.code_units[0] = cp32.rune;
        }
    } else if( cp32.rune <= 0x10FFFF ) {
        u32 value = cp32.rune - 0x10000;

        result.code_units[0] = (c16)((value >>    10) + 0xD800);
        result.code_units[1] = (c16)((value  & 0x3FF) + 0xDC00);
    } else {
        result = UNICODE_CP16_REPLACEMENT_CHARACTER;
    }

    return result;
}
attr_always_inline attr_header
UTFCodePoint32 unicode_cp32_from_cp8( UTFCodePoint8 cp8 ) {
    UTFCodePoint32 result;

    if( cp8.code_units[0] < 0x80 ) {

        result.rune = cp8.code_units[0];
        
    } else if( (cp8.code_units[0] & 0xE0) == 0xC0 ) {


        result.rune = cp8.code_units[0] & 0x1F;
        result.rune = (result.rune << 6) | (cp8.code_units[1] & 0x3F);

    } else if( (cp8.code_units[0] & 0xF0) == 0xE0 ) {

        result.rune = cp8.code_units[0] & 0x0F;
        result.rune = (result.rune << 6) | (cp8.code_units[1] & 0x3F);
        result.rune = (result.rune << 6) | (cp8.code_units[2] & 0x3F);

    } else if( (cp8.code_units[0] & 0xF8) ) {

        result.rune = cp8.code_units[0] & 0x07;
        result.rune = (result.rune << 6) | (cp8.code_units[1] & 0x37);
        result.rune = (result.rune << 6) | (cp8.code_units[2] & 0x37);
        result.rune = (result.rune << 6) | (cp8.code_units[3] & 0x37);

    } else {

        result = UNICODE_CP32_REPLACEMENT_CHARACTER;

    }
    return result;
}
attr_always_inline attr_header
UTFCodePoint32 unicode_cp32_from_cp16( UTFCodePoint16 cp16 ) {
    UTFCodePoint32 result;

    if( cp16.code_units[0] <= 0xD7FF ) {
        result.rune = cp16.code_units[0];
    } else if( cp16.code_units[0] <= 0xDBFF ) {
        u16 hi = ( cp16.code_units[0] - 0xD800 ) * 0x400;
        u16 lo = ( cp16.code_units[1] - 0xDC00 );

        result.rune = (lo | hi) + 0x10000;
    } else {
        result = UNICODE_CP32_REPLACEMENT_CHARACTER;
    }

    return result;
}
attr_always_inline attr_header
b32 unicode_is_whitespace( rune32 rune ) {
    if( unicode_is_ascii( rune ) ) {
        return ascii_is_whitespace( rune );
    }
    return false;
}
attr_always_inline attr_header
b32 unicode_is_path_separator( rune32 rune ) {
    if( unicode_is_ascii( rune ) ) {
        return ascii_is_path_separator( rune );
    }
    return false;
}
attr_always_inline attr_header
b32 unicode_is_numeric( rune32 rune ) {
    if( unicode_is_ascii( rune ) ) {
        return ascii_is_numeric( rune );
    }
    return false;
}
attr_always_inline attr_header
b32 unicode_is_ascii( rune32 rune ) {
    return !(rune & ~0x7F);
}
attr_always_inline attr_header
b32 unicode_is_alphanumeric( rune32 rune ) {
    return unicode_is_numeric( rune ) || unicode_is_alphabetic( rune );
}

#endif /* header guard */
