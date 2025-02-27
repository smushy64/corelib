#if !defined(CORE_ASCII_H)
#define CORE_ASCII_H
/**
 * @file   ascii.h
 * @brief  ASCII functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 05, 2025
*/
#include "core/attributes.h"
#include "core/types.h"
#include "core/stream.h"

/// @brief Set of whitespace characters.
#define CHARACTER_SET_WHITESPACE 0x20, 0x09, 0x0D, 0x0A

/// @brief Check if ASCII character is whitespace.
/// @param c Character to check.
/// @return
///     - @c true  : @c c is whitespace.
///     - @c false : @c c is not whitespace.
attr_always_inline attr_header
b32 ascii_is_whitespace( char c ) {
    return c == 0x20 || c == 0x09 || c == 0x0D || c == 0x0A;
}
/// @brief Check if ASCII character is a path separator.
/// @details
/// Checks for current platform's path separator only.
/// @param c Character to check.
/// @return
///     - @c true  : @c c is a path separator.
///     - @c false : @c c is not a path separator.
attr_always_inline attr_header
b32 ascii_is_path_separator( char c ) {
#if defined(CORE_PLATFORM_WINDOWS)
    return c == '/' || c == '\\';
#else
    return c == '/';
#endif
}
/// @brief Check if ASCII character is numeric.
/// @param c Character to check.
/// @return
///     - @c true  : @c c is numeric.
///     - @c false : @c c is not numeric.
attr_always_inline attr_header
b32 ascii_is_numeric( char c ) {
    return c >= '0' && c <= '9';
}
/// @brief Check if ASCII character is alphabetic upper case.
/// @param c Character to check.
/// @return
///     - @c true  : @c c is alphabetic upper case.
///     - @c false : @c c is not alphabetic upper case.
attr_always_inline attr_header
b32 ascii_is_alphabetic_upper( char c ) {
    return ( c >= 'A' && c <= 'Z' );
}
/// @brief Check if ASCII character is alphabetic lower case.
/// @param c Character to check.
/// @return
///     - @c true  : @c c is alphabetic lower case.
///     - @c false : @c c is not alphabetic lower case.
attr_always_inline attr_header
b32 ascii_is_alphabetic_lower( char c ) {
    return ( c >= 'a' && c <= 'z' );
}
/// @brief Check if ASCII character is alphabetic.
/// @param c Character to check.
/// @return
///     - @c true  : @c c is alphabetic.
///     - @c false : @c c is not alphabetic.
attr_always_inline attr_header
b32 ascii_is_alphabetic( char c ) {
    return
        ascii_is_alphabetic_lower( c ) ||
        ascii_is_alphabetic_upper( c );
}
/// @brief Check if ASCII character is alphanumeric.
/// @param c Character to check.
/// @return
///     - @c true  : @c c is alphanumeric.
///     - @c false : @c c is not alphanumeric.
attr_always_inline attr_header
b32 ascii_is_alphanumeric( char c ) {
    return ascii_is_alphabetic( c ) || ascii_is_numeric( c );
}
/// @brief Convert ASCII character to upper case.
/// @param c Character to convert.
/// @return Converted character, returns the same character if it's not a letter.
attr_always_inline attr_header
char ascii_to_upper( char c ) {
    if( ascii_is_alphabetic_lower( c ) ) {
        return c - ('a' - 'A');
    }
    return c;
}
/// @brief Convert ASCII character to lower case.
/// @param c Character to convert.
/// @return Converted character, returns the same character if it's not a letter.
attr_always_inline attr_header
char ascii_to_lower( char c ) {
    if( ascii_is_alphabetic_upper( c ) ) {
        return c + ('a' - 'A');
    }
    return c;
}

/// @brief Convert UTF-8 string to ASCII string.
///
/// @details
/// By default, characters without ASCII equivalent will be
/// skipped but if a non-zero value is provided in @c opt_replacement_character,
/// they will be replaced with that character.
///
/// @internal
/// This function is defined in unicode.c
///
/// @param[in] stream                    Streaming function.
/// @param[in] target                    Target for streaming.
/// @param     len                       Length of UTF-8 string in code units. (bytes)
/// @param[in] utf8                      UTF-8 string.
/// @param     opt_replacement_character (optional) Character to use when unicode character
///                                        has no ASCII equivalent.
/// @return Number of bytes that could not be written to target.
attr_core_api
usize ascii_from_utf8(
    StreamBytesFN* stream, void* target,
    usize len, const c8* utf8,
    char opt_replacement_character );

/// @brief Convert UTF-16 string to ASCII string.
///
/// @details
/// By default, characters without ASCII equivalent will be
/// skipped but if a non-zero value is provided in @c opt_replacement_character,
/// they will be replaced with that character.
///
/// @internal
/// This function is defined in unicode.c
///
/// @param[in] stream                    Streaming function.
/// @param[in] target                    Target for streaming.
/// @param     len                       Length of UTF-16 string in code units. (shorts)
/// @param[in] utf16                     UTF-16 string.
/// @param     opt_replacement_character (optional) Character to use when unicode character
///                                        has no ASCII equivalent.
/// @return Number of bytes that could not be written to target.
attr_core_api
usize ascii_from_utf16(
    StreamBytesFN* stream, void* target,
    usize len, const c16* utf16,
    char opt_replacement_character );

/// @brief Convert UTF-32 string to ASCII string.
///
/// @details
/// By default, characters without ASCII equivalent will be
/// skipped but if a non-zero value is provided in @c opt_replacement_character,
/// they will be replaced with that character.
///
/// @internal
/// This function is defined in unicode.c
///
/// @param[in] stream                    Streaming function.
/// @param[in] target                    Target for streaming.
/// @param     len                       Length of UTF-32 string in code units. (ints)
/// @param[in] utf32                     UTF-32 string.
/// @param     opt_replacement_character (optional) Character to use when unicode character
///                                        has no ASCII equivalent.
/// @return Number of bytes that could not be written to target.
attr_core_api
usize ascii_from_utf32(
    StreamBytesFN* stream, void* target,
    usize len, const c32* utf32,
    char opt_replacement_character );



#endif /* header guard */
