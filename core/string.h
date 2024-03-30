#if !defined(CORE_STRING_H)
#define CORE_STRING_H
/**
 * @file   string.h
 * @brief  String functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   January 30, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/macros.h"
#include "core/slice.h"
#include "core/stream.h"

/// @brief Slice of a character buffer.
/// @see #CoreSlice
typedef struct CoreSlice  String;
/// @brief Mutable character buffer.
/// @see #CoreBuffer
typedef struct CoreBuffer StringBuf;

/// @brief Check if ASCII character is whitespace.
/// @param c Character to check.
/// @return True if character is whitespace.
attr_always_inline
attr_header b32 char_is_whitespace( char c ) {
    return c == 0x20 || c == 0x09 || c == 0x0D || c == 0x0A;
}
/// @brief Check if ASCII character is a number.
/// @param c Character to check
/// @return True if character is a number.
attr_always_inline
attr_header b32 char_is_number( char c ) {
    return c >= '0' && c <= '9';
}
/// @brief Convert ASCII character to upper case.
/// @param c Character to convert.
/// @return Converted character, returns the same character if it's not a letter.
attr_always_inline
attr_header char char_to_upper( char c ) {
    if( c >= 'a' && c <= 'z' ) {
        return c - ('a' - 'A');
    }
    return c;
}
/// @brief Convert ASCII character to lower case.
/// @param c Character to convert.
/// @return Converted character, returns the same character if it's not a letter.
attr_always_inline
attr_header char char_to_lower( const char c ) {
    if( c >= 'A' && c <= 'Z' ) {
        return c + ('a' - 'A');
    }
    return c;
}

/// @brief Calculate length of null-terminated ASCII string.
/// @param[in] ascii Pointer to string.
/// @return Length of string excluding null-terminator.
attr_core_api usize asciiz_len( const char* ascii );

/// @brief Hash a text buffer.
/// @param len Length of text buffer in bytes.
/// @param[in] text Pointer to text buffer.
/// @return Hash of text buffer.
attr_core_api u64 text_buffer_hash( usize len, const char* text );
/// @brief Hash a string literal.
/// @param literal ASCII null-terminated string literal.
/// @return Hash of string literal.
#define literal_hash( literal )\
    text_buffer_hash( sizeof(literal) - 1, literal  )

/// @brief Create a new string from character buffer and a length.
/// @param[in] cc Pointer to character buffer.
/// @param len Length of character buffer.
/// @return String.
attr_always_inline
attr_header String string_new( const char* cc, usize len ) {
    String result;
    result.cc  = cc;
    result.len = len;
    return result;
}
#if defined(CORE_CPLUSPLUS)
    /// @brief Create a new const String from string literal.
    /// @param literal (const char*) String literal to create new string from.
    /// @return Read-only String.
    #define string_text( literal )\
        String{ .cc=literal, .len=sizeof(literal) - 1 }
#else
    /// @brief Create a new const String from string literal.
    /// @param literal (const char*) String literal to create new string from.
    /// @return Read-only String.
    #define string_text( literal )\
        (struct CoreSlice){ .cc=literal, .len=sizeof(literal) - 1 }
#endif
/// @brief Create a new string from null-terminated ASCII string.
/// @param opt_len (optional) Length of string (if 0, length is calculated).
/// @param[in] ascii Pointer to string.
/// @return String
attr_always_inline
attr_header String string_from_asciiz( usize opt_len, const char* ascii ) {
    String result;
    result.cc  = ascii;
    result.len = opt_len ? opt_len : asciiz_len( ascii );
    return result;
}
/// @brief Get pointer to string's buffer.
/// @param str String to get pointer of.
/// @return Pointer to string's buffer.
attr_always_inline
attr_header const char* string_str( String str ) {
    return str.c;
}
/// @brief Get length of string.
/// @param str String to get length of.
/// @return ASCII length of string.
attr_always_inline
attr_header usize string_len( String str ) {
    return str.len;
}
/// @brief Get last character in string.
/// @param str String.
/// @param[out] out_char Pointer to write to.
/// @return False if string was empty.
attr_always_inline
attr_header b32 string_last( String str, char* out_char ) {
    if( !str.len ) {
        return false;
    }
    *out_char = str.cc[str.len - 1];
    return true;
}
/// @brief Get last character in string.
/// @note Does not bounds check, use only when you know string is NOT empty.
/// @param str String.
/// @return Last character in string.
attr_always_inline
attr_header char string_last_unchecked( String str ) {
    return str.cc[str.len - 1];
}
/// @brief Check if string contains a null-terminator.
/// @param str String to check.
/// @return True if string contains null-terminator.
attr_always_inline
attr_header b32 string_is_null_terminated( const String str ) {
    return !str.cc[str.len] || (str.len && !str.cc[str.len - 1]);
}
/// @brief Hash a string.
/// @param str String to hash.
/// @return Hash of string.
/// @see #text_buffer_hash()
attr_always_inline
attr_header u64 string_hash( String str ) {
    return text_buffer_hash( str.len, str.cc );
}
/// @brief Compare two Strings for equality.
/// @param a,b Strings to compare.
/// @return True if strings are equal in length and
/// buffers contain the same text.
attr_core_api b32 string_cmp( const String a, const String b );
/// @brief Find first instance of ASCII character in string.
/// @param str String to search in.
/// @param c Character to search for.
/// @param[out] opt_out_pos (optional) Position of first match found. Can be NULL.
/// @return True if character is found in string.
attr_core_api b32 string_find(
    const String str, const char c, usize* opt_out_pos );
/// @brief Find last instance of ASCII character in string.
/// @param str String to search in.
/// @param c Character to search for.
/// @param[out] opt_out_pos (optional) Position of last match found. Can be NULL.
/// @return True if character is found in string.
attr_core_api b32 string_find_last(
    const String str, const char c, usize* opt_out_pos );
/// @brief Find first whitespace character in string.
/// @param str String to search in.
/// @param[out] opt_out_pos (optional) Position of first match found. Can be NULL.
/// @return True if whitespace was found in string.
attr_core_api b32 string_find_whitespace( const String str, usize* opt_out_pos );
/// @brief Find last whitespace character in string.
/// @param str String to search in.
/// @param[out] opt_out_pos (optional) Position of last match found. Can be NULL.
/// @return True if whitespace was found in string.
attr_core_api b32 string_find_last_whitespace( const String str, usize* opt_out_pos );
/// @brief Count how many instances of ASCII character appear in string.
/// @param str String to search in.
/// @param c Character to search for.
/// @return Number of times character appears in string.
attr_core_api usize string_count( const String str, const char c );
/// @brief Find first instance of phrase in string.
/// @param str String to search in.
/// @param phrase String to search for.
/// @param[out] opt_out_pos (optional) Position of start of first match found. Can be NULL.
/// @return True if phrase is found in string.
attr_core_api b32 string_find_phrase(
    const String str, const String phrase, usize* opt_out_pos );
/// @brief Find last instance of phrase in string.
/// @param str String to search in.
/// @param phrase String to search for.
/// @param[out] opt_out_pos (optional) Position of start of last match found. Can be NULL.
/// @return True if phrase is found in string.
attr_core_api b32 string_find_phrase_last(
    const String str, const String phrase, usize* opt_out_pos );
/// @brief Reverse contents of string.
/// @param str String to reverse.
attr_core_api void string_reverse( String str );
/// @brief Create a truncated string from source string.
/// @note Asserts that max_len <= src.len.
/// @param src String to create truncated string from.
/// @param max_len Length of truncated string. 
/// @return Truncated string.
attr_always_inline
attr_header String string_truncate( const String src, usize max_len ) {
    assert( max_len <= src.len );
    String result = src;
    result.len = max_len;
    return result;
}
/// @brief Create a string that advances from source string.
/// @note Asserts that advance_len <= src.len.
/// @param src String to advance from.
/// @param advance_len Amount to advance by. 
/// @return Advanced string.
attr_always_inline
attr_header String string_advance( const String src, usize advance_len ) {
    assert( advance_len <= src.len );
    String result = src;
    result.cc  += advance_len;
    result.len -= advance_len;
    return result;
}
/// @brief Create a substring from source string.
/// @param src String to create substring from.
/// @param from_inclusive Index of first character in trim.
/// @param to_exclusive Index of last character + 1.
/// @return Substring of source string.
attr_always_inline
attr_header String string_trim_range(
    const String src, usize from_inclusive, usize to_exclusive
) {
    String result = src;
    result.cc  += from_inclusive;
    result.len  = to_exclusive - from_inclusive;
    return result;
}
/// @brief Create a substring from source string by trimming leading whitespace.
/// @param src String to create substring from.
/// @return String without leading whitespace.
attr_core_api String string_trim_leading_whitespace( const String src );
/// @brief Create a substring from source string by trimming trailing whitespace.
/// @param src String to create substring from.
/// @return String without trailing whitespace.
attr_core_api String string_trim_trailing_whitespace( const String src );
/// @brief Create a substring from source string by trimming leading and trailing whitespace.
/// @param src String to create substring from.
/// @return String without leading or trailing whitespace.
attr_header String string_trim_whitespace( const String src ) {
    String res = string_trim_leading_whitespace( src );
    return string_trim_trailing_whitespace( res );
}
/// @brief Set all characters in string to provided ASCII character.
/// @param str String to modify.
/// @param c Character to set.
attr_core_api void string_set( String str, const char c );
/// @brief Convert all ASCII lower case characters to upper case.
/// @param str String to modify.
attr_core_api void string_to_upper( String str );
/// @brief Convert all ASCII lower case characters to upper case.
///
/// Streams result to target.
/// @param stream Stream function.
/// @param[in] target Target to stream to.
/// @param str Source string.
/// @return Number of characters that could not fit in stream.
/// @see #StreamBytesFN
attr_core_api usize string_stream_to_upper(
    StreamBytesFN* stream, void* target, const String str );
/// @brief Convert all ASCII upper case characters to lower case.
/// @param str String to modify.
attr_core_api void string_to_lower( String str );
/// @brief Convert all ASCII upper case characters to lower case.
///
/// Streams result to target.
/// @param stream Stream function.
/// @param[in] target Target to stream to.
/// @param str Source string.
/// @return Number of characters that could not fit in stream.
/// @see #StreamBytesFN
attr_core_api usize string_stream_to_lower(
    StreamBytesFN* stream, void* target, const String str );
/// @brief Split source string into two substrings.
/// @param src String to split from.
/// @param index Index to split at. Should be less than src.len.
/// @param[out] opt_out_first (optional) First substring half of string.
/// @param[out] opt_out_last (optional) Last substring half of string.
attr_always_inline
attr_header void string_split(
    const String src, usize index, String* opt_out_first, String* opt_out_last
) {
    String first = string_new(0, 0);
    String last  = string_new(0, 0);

    usize i = index > src.len ? src.len : index;

    first.cc  = src.cc;
    first.len = i;

    last.cc  = src.cc + i + 1;
    last.len = src.len - i;
    if( last.len ) {
        last.len--;
    }

    if( opt_out_first ) {
        *opt_out_first = first;
    }
    if( opt_out_last ) {
        *opt_out_last = last;
    }
}
/// @brief Split source string into two substrings using first instance of character.
/// @param src String to split from.
/// @param c Character to split at.
/// @param[out] opt_out_first (optional) First substring half of string.
/// @param[out] opt_out_last (optional) Last substring half of string.
/// @return True if character is found, false if it wasn't.
attr_always_inline
attr_header b32 string_split_char(
    const String src, const char c, String* opt_out_first, String* opt_out_last
) {
    usize pos = 0;
    if( !string_find( src, c, &pos ) ) {
        return false;
    }

    string_split( src, pos, opt_out_first, opt_out_last );
    return true;
}
/// @brief Split source string into two substrings using first instance of whitespace.
/// @param src String to split from.
/// @param[out] opt_out_first (optional) First substring half of string.
/// @param[out] opt_out_last (optional) Last substring half of string.
/// @return True if whitespace is found, false if it wasn't.
attr_always_inline
attr_header b32 string_split_whitespace(
    const String src, String* opt_out_first, String* opt_out_last
) {
    usize pos = 0;
    if( !string_find_whitespace( src, &pos ) ) {
        return false;
    }

    string_split( src, pos, opt_out_first, opt_out_last );
    return true;
}
/// @brief Parse signed integer from source string.
///
/// Parsing is successful if valid integer is found from start of string.
/// @param str String to parse integer from.
/// @param[out] out_int Pointer to integer to receive result.
/// @return True if string was successfully parsed.
attr_core_api b32 string_parse_int( const String str, i64* out_int );
/// @brief Parse unsigned integer from source string.
///
/// Parsing is successful if valid integer is found from start of string.
/// @param str String to parse integer from.
/// @param[out] out_uint Pointer to integer to receive result.
/// @return True if string was successfully parsed.
attr_core_api b32 string_parse_uint( const String str, u64* out_uint );
/// @brief Parse float from source string.
///
/// Parsing is successful if valid float is found from start of string.
/// @param str String to parse float from.
/// @param[out] out_float Pointer to float to receive result.
/// @return True if string was successfully parsed.
attr_core_api b32 string_parse_float( const String str, f64* out_float );

/// @brief Create a new string buffer.
/// @param size Size of buffer.
/// @param[in] buffer Pointer to start of buffer.
/// @return String buffer.
attr_always_inline
attr_header StringBuf string_buf_new( const usize size, char* buffer ) {
    StringBuf result;
    result.c   = buffer;
    result.len = 0;
    result.cap = size;
    return result;
}
/// @brief Create a new string buffer using default allocator.
///
/// @note Buffer should be freed using #memory_free().
/// @param size Size of buffer.
/// @param[in] opt_copy_buffer Buffer to copy into string_buf.
/// @param[out] out_buf String buffer.
/// @return True if allocation was successful.
attr_core_api b32 string_buf_new_alloc(
    const usize size, const char* opt_copy_buffer, StringBuf* out_buf );
/// @brief Attempt to reallocate string buffer using default allocator.
///
/// Does nothing and returns true if new_size is less than buf.cap.
/// @param[in] buf Pointer to string buffer to reallocate.
/// @param new_size Desired new size of string buffer.
/// @return True if reallocation was successful.
attr_core_api b32 string_buf_realloc( StringBuf* buf, const usize new_size );
/// @brief Query size of reallocation when string_buf_*_alloc functions are used.
/// @return Size of reallocations in bytes.
attr_core_api usize string_buf_query_realloc_size(void);
/// @brief Query default size of reallocation when string_buf_*_alloc functions are used.
/// @return Default size of reallocations in bytes.
attr_core_api usize string_buf_query_default_realloc_size(void);
/// @brief Set default reallocation size when string_buf_*_alloc functions are used.
/// @param size New reallocation size, clamped to a minimum of 16 bytes.
attr_core_api void string_buf_set_default_realloc_size( const usize size );
/// @brief Free a string_buf's buffer.
/// @note Use only when buffer is allocated with default system allocator.
/// @param[in] buf Buffer to free.
attr_core_api void string_buf_free( StringBuf* buf );
/// @brief Create a new string buffer from static array.
/// @param array Static, local character array.
/// @return String buffer.
#define string_buf_from_array( array )\
    string_buf_new( static_array_len( (array) ), (array) )

/// @brief Get pointer to string buffer's buffer.
/// @param[in] buf String buffer.
/// @return Pointer to start of buffer.
attr_always_inline
attr_header char* string_buf_str( const StringBuf* buf ) {
    return buf->c;
}
/// @brief Get string buffer's length.
/// @param[in] buf String buffer.
/// @return Length of string buffer.
attr_always_inline
attr_header usize string_buf_len( const StringBuf* buf ) {
    return buf->len;
}
/// @brief Get string buffer's capacity in bytes.
/// @param[in] buf String buffer.
/// @return Capacity of string buffer in bytes.
attr_always_inline
attr_header usize string_buf_cap( const StringBuf* buf ) {
    return buf->cap;
}
/// @brief Check if string buffer is empty.
/// @param[in] buf String buffer.
/// @return True if string buffer is empty.
attr_always_inline
attr_header b32 string_buf_is_empty( const StringBuf* buf ) {
    return buf->len == 0;
}
/// @brief Check if string buffer is full.
/// @param[in] buf String buffer.
/// @return True if string buffer is full.
attr_always_inline
attr_header b32 string_buf_is_full( const StringBuf* buf ) {
    return buf->len == buf->cap;
}
/// @brief Clear a string buffer.
///
/// Sets length to zero.
/// @param[in] buf Buffer to clear.
attr_always_inline
attr_header void string_buf_clear( StringBuf* buf ) {
    buf->len = 0;
}

/// @brief Set all characters in buffer to ASCII character, up to max or capacity.
///
/// Does not affect buffer's length.
/// @param[in] buf Buffer to modify.
/// @param c Character to set buffer to.
/// @param opt_max (optional) Stops at this value if it's not zero and it's < buf.cap.
attr_core_api void string_buf_set( StringBuf* buf, const char c, const usize opt_max );
/// @brief Push new ASCII character to the end of string buffer.
/// @param[in] buf Pointer to string buffer.
/// @param c ASCII character to push onto string buffer.
/// @return True if string buffer had enough capacity to push new character.
attr_always_inline
attr_header b32 string_buf_push( StringBuf* buf, const char c ) {
    if( buf->len >= buf->cap ) {
        return false;
    }

    buf->c[buf->len++] = c;
    return true;
}
/// @brief Push new ASCII character to the end of string buffer.
///
/// Reallocates string buffer using default system allocator.
/// @note String buffer must be allocated using default system allocator.
/// @param[in] buf Pointer to string buffer.
/// @param c ASCII character to push onto string buffer.
/// @return True if string buffer was successfully pushed to or successfully reallocated.
attr_always_inline
attr_header b32 string_buf_push_alloc( StringBuf* buf, const char c ) {
    if( string_buf_push( buf, c ) ) {
        return true;
    }
    if( !string_buf_realloc( buf, string_buf_query_realloc_size() ) ) {
        return false;
    }
    string_buf_push( buf, c );
    return true;
}
/// @brief Pop last ASCII character from the end of string buffer.
/// @param[in] buf Pointer to string buffer.
/// @param[out] opt_out_c (optional) Pointer to ASCII character to receive popped character.
/// @return True if string buffer still has characters to pop.
attr_always_inline
attr_header b32 string_buf_pop( StringBuf* buf, char* opt_out_c ) {
    if( !buf->len ) {
        return false;
    }

    char c = buf->c[--buf->len];
    if( opt_out_c ) {
        *opt_out_c = c;
    }

    return true;
}
/// @brief Copy source string to destination string buffer.
///
/// Copies from dst len to dst cap or src len, whichever is smaller. 
/// @param[in] dst Destination buffer.
/// @param src Source string.
/// @return Number of bytes copied.
attr_core_api usize string_buf_copy( StringBuf* dst, const String src );
/// @brief Prepend source string to destination string buffer.
///
/// Moves destination buffer up by src.len and copies src.buffer.
/// @param[in] dst Destination buffer.
/// @param src Source string.
/// @return True if dst has enough capacity for entire src string.
attr_core_api b32 string_buf_prepend( StringBuf* dst, const String src );
/// @brief Prepend source string to destination string buffer.
///
/// Reallocates dst string if it does not have enough capacity for src string.
/// Moves destination buffer up by src.len and copies src.buffer.
/// @note Dst string must be allocated with default system allocator.
/// @param[in] dst Destination buffer.
/// @param src Source string.
/// @return True if prepend was successful or if reallocation and prepend was successful.
attr_always_inline
attr_header b32 string_buf_prepend_alloc( StringBuf* dst, const String src ) {
    if( string_buf_prepend( dst, src ) ) {
        return true;
    }

    if( !string_buf_realloc( dst, string_buf_query_realloc_size() + src.len ) ) {
        return false;
    }
    string_buf_prepend( dst, src );
    return true;
}
/// @brief Append source string to end of destination string buffer.
///
/// Copies entire src string to end of dst buffer.
/// @param[in] dst Destination buffer.
/// @param src Source string.
/// @return True if dst has enough capacity for entire src string.
attr_core_api b32 string_buf_append( StringBuf* dst, const String src );
/// @brief Append source string to end of destination string buffer.
///
/// Reallocates using default system allocator if dst does not have enough capacity.
/// Copies entire src string to end of dst buffer.
/// @note Dst string buffer must be allocated using default system allocator.
/// @param[in] dst Destination buffer.
/// @param src Source string.
/// @return True if append was successful or if reallocation and append was successful.
attr_always_inline
attr_header b32 string_buf_append_alloc( StringBuf* dst, const String src ) {
    if( string_buf_append( dst, src ) ) {
        return true;
    }

    if( !string_buf_realloc( dst, string_buf_query_realloc_size() + src.len ) ) {
        return false;
    }
    string_buf_append( dst, src );
    return true;
}
/// @brief Insert source string into destination string buffer.
///
/// Moves dst buffer from index to index + src.len and copies src.buffer to index.
/// @param[in] dst Destination buffer.
/// @param src Source string.
/// @param index Index to insert at, must be < dst.len.
/// @return True if dst has enough capacity for entire src string at provided index.
attr_core_api b32 string_buf_insert(
    StringBuf* dst, const String src, const usize index );
/// @brief Insert source string into destination string buffer.
///
/// Reallocates using default system allocator if dst does not have enough capacity.
/// Copies entire src string to end of dst buffer.
/// @note Dst string buffer must be allocated using default system allocator.
/// @param[in] dst Destination buffer.
/// @param src Source string.
/// @param index Index to insert at, must be < dst.len.
/// @return True if insert was successful or if reallocation and insert was successful.
attr_always_inline
attr_header b32 string_buf_insert_alloc(
    StringBuf* dst, const String src, const usize index
) {
    if( index > dst->len ) {
        return false;
    }
    if( string_buf_insert( dst, src, index ) ) {
        return true;
    }

    if( !string_buf_realloc( dst, string_buf_query_realloc_size() + src.len ) ) {
        return false;
    }
    string_buf_insert( dst, src, index );
    return true;
}
/// @brief Stream buffer into string buffer.
/// @param[in] dst (StringBuf*) Destination buffer.
/// @param count Number of bytes in src buffer.
/// @param[in] src Pointer to src buffer.
/// @return Number of bytes that could not be copied to dst buffer.
attr_core_api usize string_buf_stream(
    void* dst, usize count, const void* src );
/// @brief Write a formatted string to string buffer using text buffer format and variadic list of format arguments.
/// @param[in] dst Destination buffer.
/// @param format_len Length of format text (ASCII).
/// @param[in] format Format text (ASCII).
/// @param va Variadic list of format arguments.
/// @return Number of characters that could not be written to buffer.
attr_core_api usize string_buf_fmt_text_buffer_va(
    StringBuf* dst, const usize format_len, const char* format, va_list va );
/// @brief Write a formatted string to string buffer using text buffer format.
/// @param[in] dst Destination buffer.
/// @param format_len Length of format text (ASCII).
/// @param[in] format Format text (ASCII).
/// @param ... Format arguments.
/// @return Number of characters that could not be written to buffer.
attr_header usize string_buf_fmt_text_buffer(
    StringBuf* dst, const usize format_len, const char* format, ...
) {
    va_list va;
    va_start( va, format );
    usize result = string_buf_fmt_text_buffer_va( dst, format_len, format, va );
    va_end( va );

    return result;
}
/// @brief Write a formatted string to string buffer using string literal format and variadic list of format arguments.
/// @param[in] dst Destination buffer.
/// @param format String literal format.
/// @param va Variadic list of format arguments.
/// @return Number of characters that could not be written to buffer.
#define string_buf_fmt_va( dst, format, va )\
    string_buf_fmt_text_buffer_va( dst, sizeof(format) - 1, format, va )
/// @brief Write a formatted string to string buffer using string literal format.
/// @param[in] dst Destination buffer.
/// @param format String literal format.
/// @param ... Format arguments.
/// @return Number of characters that could not be written to buffer.
#define string_buf_fmt( dst, format, ... )\
    string_buf_fmt_text_buffer( dst, sizeof(format) - 1, format, ##__VA_ARGS__ )

#endif /* header guard */
