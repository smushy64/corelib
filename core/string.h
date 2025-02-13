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
#include "core/assertions.h"
#include "core/stream.h"
#include "core/ascii.h"

// forward declaration.
struct AllocatorInterface;
attr_core_api
void memory_set( void* dst, u8 byte, usize size );

struct _StringPOD {
    /// @brief Byte length of string.
    usize len;
    union {
        /// @brief Constant pointer to string buffer.
        const char* cbuf;
        /// @brief Pointer to string buffer.
        char*       buf;
        /// @brief Pointer to string bytes.
        u8*         bytes;
        /// @brief Void pointer to string buffer.
        void*       _void;
    };
};
struct _StringBufPOD {
    /// @brief Byte capacity of string buffer.
    usize cap;
    union {
        struct {
            /// @brief Byte length of string.
            usize len;
            union {
                /// @brief Constant pointer to string buffer.
                const char* cbuf;
                /// @brief Pointer to string buffer.
                char*       buf;
                /// @brief Pointer to string bytes.
                u8*         bytes;
                /// @brief Void pointer to string buffer.
                void*       _void;
            };
        };
        /// @brief String buffer as a slice.
        struct _StringPOD slice;
    };
};
#if !defined(CORE_CPLUSPLUS)
    /// @brief UTF-8 String.
    /// @details
    /// Not necessarily null terminated.
    typedef struct _StringPOD    String;
    /// @brief UTF-8 String Buffer.
    /// @details
    /// Always accounts for null terminator.
    typedef struct _StringBufPOD StringBuf;
#endif

/// @brief Target for StringBuf stream.
typedef struct StringBufStreamTarget {
    /// @brief Allocator.
    struct AllocatorInterface* allocator;
    /// @brief String buffer.
    struct _StringBufPOD* buf;
} StringBufStreamTarget;

/// @brief Calculate ascii length of null terminated C string.
/// @param[in] c_string Pointer to string.
/// @return Ascii length of string excluding null terminator.
attr_core_api
usize cstr_len( const cstr* c_string );
/// @brief Calculate UTF-8 length of null terminated C string.
/// @param[in] c_string Pointer to string.
/// @return UTF-8 length of string excluding null terminator.
attr_core_api
usize cstr_len_utf8( const cstr* c_string );
/// @brief Compare two null terminated C strings.
/// @param[in] a, b Pointers to strings to compare. Cannot be null.
/// @return
///     - @c true  : @c a and @c b match in contents and length.
///     - @c false : @c a and @c b do not match in contents or length.
attr_core_api
b32 cstr_cmp( const cstr* a, const cstr* b );

/// @brief Create new string slice.
/// @param     len    Length of string slice.
/// @param[in] start  Pointer to start of slice.
/// @return String slice.
attr_always_inline attr_header
struct _StringPOD string_new( usize len, const void* start ) {
    struct _StringPOD result;
    result.len  = len;
    result.cbuf = (const char*)start;
    return result;
}
/// @brief Create empty string slice.
/// @return String slice.
attr_always_inline attr_header
struct _StringPOD string_empty(void) {
    return string_new( 0, NULL );
}
/// @brief Create new string slice from null terminated C string.
/// @param[in] cstr (const cstr*) C string.
/// @return String slice.
attr_always_inline attr_header
struct _StringPOD string_from_cstr( const char* cstr ) {
    return string_new( cstr_len( cstr ), cstr );
}
/// @brief Create new string slice from string literal.
/// @param literal (const char* literal) String literal.
/// @return String slice.
#define string_text( literal ) \
    string_new( sizeof(literal) - 1, literal )
/// @brief Hash string using ELF hash algorithm.
/// @note Requires core/hash.h
/// @param str (String) String to hash.
/// @return 64-bit hash of @c str.
#define string_hash_elf_64( str ) \
    hash_elf_64( (str).len, (str)._void )
/// @brief Hash string using MurmurHash2 algorithm.
/// @note Requires core/hash.h
/// @param str (String) String to hash.
/// @return 64-bit hash of @c str.
#define string_hash_murmur2_64( str ) \
    hash_murmur2_64( (str).len, (str)._void )
/// @brief Hash string using Cityhash algorithm.
/// @note Requires core/hash.h
/// @param str (String) String to hash.
/// @return 64-bit hash of @c str.
#define string_hash_city_64( str ) \
    hash_city_64( (str).len, (str)._void )
/// @brief Check if string is empty.
/// @param str String to check.
/// @return
///     - @c true  : @c str is empty.
///     - @c false : @c str is not empty.
attr_always_inline attr_header
b32 string_is_empty( struct _StringPOD str ) {
    return str.len == 0;
}
/// @brief Calculate UTF-8 length of string.
/// @note This function does not check if string is valid UTF-8!
/// @param str String to calculate length of.
/// @return UTF-8 length.
attr_core_api
usize string_len_utf8( struct _StringPOD str );
/// @brief Index into string. Debug asserts that index is in bounds.
/// @param str   String to index into.
/// @param index Index of character.
/// @return Character at given index.
attr_always_inline attr_header
char string_index( struct _StringPOD str, usize index ) {
    debug_assert( index < str.len );
    return str.cbuf[index];
}
/// @brief Index into string, using UTF-8 index. Debug asserts that index is in bounds.
/// @param str   String to index into.
/// @param index UTF-8 index.
/// @return Codepoint at given index.
attr_core_api
c32 string_index_utf8( struct _StringPOD str, usize index );
/// @brief Get next rune in string and advance.
/// @param      src           String to get next codepoint from.
/// @param[out] out_codepoint Pointer to write codepoint to.
/// @return String advanced by number of bytes in rune.
attr_core_api
struct _StringPOD string_utf8_next( struct _StringPOD src, c32* out_codepoint );
/// @brief Get pointer to first character in string.
/// @param str String to get pointer from.
/// @return
///     - @c NULL : @c str is empty
///     - Pointer : Pointer to first character in @c str.
attr_always_inline attr_header
char* string_first( struct _StringPOD str ) {
    if( string_is_empty( str ) ) {
        return NULL;
    }
    return str.buf;
}
/// @brief Get pointer to last character in string.
/// @param str String to get pointer from.
/// @return
///     - @c NULL    : @c str is empty.
///     - @c Pointer : Pointer to last character in @c str.
attr_always_inline attr_header
char* string_last( struct _StringPOD str ) {
    if( string_is_empty( str ) ) {
        return NULL;
    }
    return str.buf + (str.len - 1);
}
/// @brief Get first character in string without bounds checking.
/// @param str String to get character from.
/// @return First character in string.
attr_always_inline attr_header
char string_first_unchecked( struct _StringPOD str ) {
    return *string_first( str );
}
/// @brief Get last character in string without bounds checking.
/// @param str String to get character from.
/// @return Last character in string.
attr_always_inline attr_header
char string_last_unchecked( struct _StringPOD str ) {
    return *string_last( str );
}
/// @brief Compare strings for equality.
/// @param a, b Strings to compare.
/// @return
///     - @c true  : @c a and @c b are equal in length and contents.
///     - @c false : @c a and @c b are not equal.
attr_core_api
b32 string_cmp( struct _StringPOD a, struct _StringPOD b );
/// @brief Search for ascii character in string.
/// @param      str           String to search in.
/// @param      c             Character to search for.
/// @param[out] opt_out_index (optional) Pointer to write index of character if found.
/// @return
///     - @c true  : Character was found. Index written to @c opt_out_index (if not null)
///     - @c false : Character was not found.
attr_core_api
b32 string_find( struct _StringPOD str, char c, usize* opt_out_index );
/// @brief Tally number of times that given character occurs in string.
/// @param str String.
/// @param c   Character to tally.
/// @return Number of times @c c appears in @c str.
attr_core_api
usize string_find_count( struct _StringPOD str, char c );
/// @brief Search for ascii character in string, searches from the end of string.
/// @param      str           String to search in.
/// @param      c             Character to search for.
/// @param[out] opt_out_index (optional) Pointer to write index of character if found.
/// @return
///     - @c true  : Character was found. Index written to @c opt_out_index (if not null)
///     - @c false : Character was not found.
attr_core_api
b32 string_find_rev( struct _StringPOD str, char c, usize* opt_out_index );
/// @brief Search for any character in set in string.
/// @param      str           String to search in.
/// @param      set           Set of characters to search for.
/// @param[out] opt_out_index (optional) Pointer to write index of first character in set if any are found.
/// @return
///     - @c true  : Any character from set was found. Index written to @c opt_out_index (if not null)
///     - @c false : No characters from set were found.
attr_core_api
b32 string_find_set( struct _StringPOD str, struct _StringPOD set, usize* opt_out_index );
/// @brief Search for any character in set in string, searches from end of string.
/// @param      str           String to search in.
/// @param      set           Set of characters to search for.
/// @param[out] opt_out_index (optional) Pointer to write index of first character in set if any are found.
/// @return
///     - @c true  : Any character from set was found. Index written to @c opt_out_index (if not null)
///     - @c false : No characters from set were found.
attr_core_api
b32 string_find_set_rev( struct _StringPOD str, struct _StringPOD set, usize* opt_out_index );
/// @brief Tally number of times that any character in set occurs in string.
/// @param str String.
/// @param set Set to tally.
/// @return Number of times any character in @c set appears in @c str.
attr_core_api
usize string_find_set_count( struct _StringPOD str, struct _StringPOD set );
/// @brief Search for phrase in string.
/// @param str String to search in.
/// @param phrase Phrase to search for.
/// @param[out] opt_out_index (optional) Pointer to write index of first character of phrase. (if not null)
/// @return 
///     - @c true  : @c phrase was found in @c str.
///     - @c false : @c phrase was not found.
attr_core_api
b32 string_find_phrase( struct _StringPOD str, struct _StringPOD phrase, usize* opt_out_index );
/// @brief Search for phrase in string, searches from end of string.
/// @param str String to search in.
/// @param phrase Phrase to search for.
/// @param[out] opt_out_index (optional) Pointer to write index of first character of phrase. (if not null)
/// @return 
///     - @c true  : @c phrase was found in @c str.
///     - @c false : @c phrase was not found.
attr_core_api
b32 string_find_phrase_rev(
    struct _StringPOD str, struct _StringPOD phrase, usize* opt_out_index );
/// @brief Tally number of times that phrase occurs in string.
/// @param str    String.
/// @param phrase Phrase to tally.
/// @return Number of times @c phrase appears in @c str.
attr_core_api
usize string_find_phrase_count( struct _StringPOD str, struct _StringPOD phrase );
/// @brief Truncate length of string.
/// @details
/// Returns str if max is greater than length.
/// @param str String to truncate.
/// @param max Maximum length of string.
/// @return Truncated string.
attr_always_inline attr_header
struct _StringPOD string_truncate( struct _StringPOD str, usize max ) {
    struct _StringPOD res = str;
    if( max >= res.len ) {
        return res;
    }
    res.len = max;
    return res;
}
/// @brief Subtract from string length.
/// @details
/// Returns empty string if amount is >= to length.
/// @param str    String to trim.
/// @param amount Number of bytes to trim off the end.
/// @return Trimmed string.
attr_always_inline attr_header
struct _StringPOD string_trim( struct _StringPOD str, usize amount ) {
    return string_truncate( str, amount >= str.len ? 0 : str.len - amount );
}
/// @brief Clip out slice from string.
/// @warning
/// Debug asserts that to > from and that they are within bounds of string.
/// @param str            String to clip from.
/// @param from_inclusive Start of range, inclusive.
/// @param to_exclusive   End of range, exclusive.
/// @return String slice.
attr_always_inline attr_header
struct _StringPOD string_clip(
    struct _StringPOD str, usize from_inclusive, usize to_exclusive
) {
    debug_assert( to_exclusive >= from_inclusive );
    debug_assert( from_inclusive < str.len );
    debug_assert( to_exclusive <= str.len );

    struct _StringPOD res = str;
    res.cbuf = res.cbuf + from_inclusive;
    res.len  = to_exclusive - from_inclusive;
    return res;
}
/// @brief Advance string slice by one byte.
/// @param str String to advance.
/// @return String advanced by one byte.
attr_always_inline attr_header
struct _StringPOD string_advance( struct _StringPOD str ) {
    if( string_is_empty( str ) ) {
        return str;
    }
    struct _StringPOD res = str;
    res.cbuf++;
    res.len--;
    return res;
}
/// @brief Advance string slice by multiple bytes.
/// @details
/// If @c bytes is >= length, returns empty string.
/// @param str   String to advance.
/// @param bytes Bytes to advance by.
/// @return String advanced by @c bytes.
attr_always_inline attr_header
struct _StringPOD string_advance_by( struct _StringPOD str, usize bytes ) {
    struct _StringPOD res = str;
    if( bytes >= res.len ) {
        res.cbuf = res.cbuf + res.len;
        res.len  = 0;
        return res;
    }
    res.cbuf = res.cbuf + bytes;
    res.len -= bytes;
    return res;
}
/// @brief Trim leading whitespace from string.
/// @param str String to trim.
/// @return Trimmed string.
attr_core_api
struct _StringPOD string_trim_leading_whitespace( struct _StringPOD str );
/// @brief Trim trailing whitespace from string.
/// @param str String to trim.
/// @return Trimmed string.
attr_core_api
struct _StringPOD string_trim_trailing_whitespace( struct _StringPOD str );
/// @brief Trim leading and trailing whitespace from string.
/// @param str String to trim.
/// @return Trimmed string.
attr_always_inline attr_header
struct _StringPOD string_trim_surrounding_whitespace( struct _StringPOD str ) {
    return string_trim_leading_whitespace( string_trim_trailing_whitespace( str ) );
}
/// @brief Split string at given index.
/// @details
/// Debug asserts that @c at is <= source length.
/// @param      source        String to split from.
/// @param      at            Index to split at. Character at this index is not included in splits.
/// @param[out] opt_out_left  (optional) Pointer to write left side of split.
/// @param[out] opt_out_right (optional) Pointer to write right side of split.
attr_always_inline attr_header
void string_split(
    struct _StringPOD source, usize at,
    struct _StringPOD* opt_out_left, struct _StringPOD* opt_out_right
) {
    debug_assert( at <= source.len );
    if( opt_out_left ) {
        *opt_out_left = string_truncate( source, at );
    }
    if( opt_out_right ) {
        *opt_out_right = string_advance_by( source, at + 1 );
    }
}
/// @brief Split string at first instance of given ascii character.
/// @details
/// Debug asserts that @c at is <= source length.
/// @param      source        String to split from.
/// @param      c             Character to split at. Character at this index is not included in splits.
/// @param[out] opt_out_left  (optional) Pointer to write left side of split.
/// @param[out] opt_out_right (optional) Pointer to write right side of split.
/// @return
///     - @c true  : @c c was found in @c source.
///     - @c false : @c c was not found, string was not split.
attr_always_inline attr_header
b32 string_split_ascii(
    struct _StringPOD source, char c,
    struct _StringPOD* opt_out_left, struct _StringPOD* opt_out_right
) {
    usize at = 0;
    if( string_find( source, c, &at ) ) {
        string_split( source, at, opt_out_left, opt_out_right );
        return true;
    }
    return false;
}
/// @brief Split string at first instance of whitespace character.
/// @details
/// Debug asserts that @c at is <= source length.
/// Trims whitespace from results.
/// @param      source        String to split from.
/// @param[out] opt_out_left  (optional) Pointer to write left side of split.
/// @param[out] opt_out_right (optional) Pointer to write right side of split.
/// @return
///     - @c true  : @c c was found in @c source.
///     - @c false : @c c was not found, string was not split.
attr_always_inline attr_header
b32 string_split_whitespace(
    struct _StringPOD source,
    struct _StringPOD* opt_out_left, struct _StringPOD* opt_out_right
) {
    const char whitespace_set[] = { CHARACTER_SET_WHITESPACE };
    usize at = 0;
    struct _StringPOD set = { .len=4, .cbuf=whitespace_set };
    if( string_find_set( source, set, &at ) ) {
        string_split( source, at, opt_out_left, opt_out_right );
        if( opt_out_left ) {
            *opt_out_left = string_trim_surrounding_whitespace( *opt_out_left );
        }
        if( opt_out_right ) {
            *opt_out_right = string_trim_surrounding_whitespace( *opt_out_right );
        }
        return true;
    }
    return false;
}
/// @brief Parse signed integer from source string.
/// @details Parsing is successful if valid integer is found from start of string.
/// @param      str     String to parse integer from.
/// @param[out] out_int Pointer to integer to receive result.
/// @return
///     - @c true  : Integer was successfully parsed from string.
///     - @c false : Failed to parse integer.
attr_core_api
b32 string_parse_int( struct _StringPOD str, i64* out_int );
/// @brief Parse unsigned integer from source string.
/// @details Parsing is successful if valid integer is found from start of string.
/// @param      str      String to parse integer from.
/// @param[out] out_uint Pointer to integer to receive result.
/// @return
///     - @c true  : Integer was successfully parsed from string.
///     - @c false : Failed to parse integer.
attr_core_api
b32 string_parse_uint( struct _StringPOD str, u64* out_uint );
/// @brief Parse float from source string.
/// @details Parsing is successful if valid float is found from start of string.
/// @param      str       String to parse float from.
/// @param[out] out_float Pointer to float to receive result.
/// @return
///     - @c true  : Float was successfully parsed from string.
///     - @c false : Failed to parse float.
attr_core_api
b32 string_parse_float( struct _StringPOD str, f64* out_float );
/// @brief Reverse contents of string.
/// @note String must be from mutable source (aka not from literal or read-only memory).
/// @param str String to reverse.
attr_core_api
void string_mut_reverse( struct _StringPOD str );
/// @brief Set all characters in string to given ascii character.
/// @note String must be from mutable source (aka not from literal or read-only memory).
/// @param str String to modify.
/// @param c   Character to set.
attr_core_api
void string_mut_set( struct _StringPOD str, char c );
/// @brief Convert all alphabetic ascii characters in string to upper case.
/// @note String must be from mutable source (aka not from literal or read-only memory).
/// @param str String to modify.
attr_core_api
void string_mut_to_upper( struct _StringPOD str );
/// @brief Convert all alphabetic ascii characters in string to lower case.
/// @note String must be from mutable source (aka not from literal or read-only memory).
/// @param str String to modify.
attr_core_api
void string_mut_to_lower( struct _StringPOD str );
/// @brief Convert all alphabetic ascii characters to upper case.
/// @param[in] stream Stream function.
/// @param[in] target Target to stream to.
/// @param     str    Source string.
/// @return Number of characters that could not fit in stream.
/// @see #StreamBytesFN
attr_core_api
usize string_stream_to_upper(
    StreamBytesFN* stream, void* target, struct _StringPOD str );
/// @brief Convert all alphabetic ascii characters to lower case.
/// @param[in] stream Stream function.
/// @param[in] target Target to stream to.
/// @param     str    Source string.
/// @return Number of characters that could not fit in stream.
/// @see #StreamBytesFN
attr_core_api
usize string_stream_to_lower(
    StreamBytesFN* stream, void* target, struct _StringPOD str );

/// @brief Create new string buffer.
/// @param     capacity Capacity of string buffer.
/// @param[in] start    Pointer to start of string buffer.
/// @return String buffer.
attr_always_inline attr_header
struct _StringBufPOD string_buf_new( usize capacity, const void* start ) {
    struct _StringBufPOD result;
    result.cap  = capacity;
    result.len  = 0;
    result.cbuf = (const char*)start;
    return result;
}
/// @brief Create empty string buffer.
/// @return String buffer.
attr_always_inline attr_header
struct _StringBufPOD string_buf_empty(void) {
    return string_buf_new( 0, NULL );
}
/// @brief Initialize a string buffer from the stack.
/// @details
/// Defines a stack char buffer with name @c name\#\#_buffer
/// of given size and a StringBuf from that buffer with given name.
/// @param name (valid identifier) Name of string buffer.
/// @param size (usize)            Size of string buffer.
#define string_buf_from_stack( name, size )\
    char name##_buffer[size];\
    memory_zero( name##_buffer, size );\
    struct _StringBufPOD name = string_buf_new( size, name##_buffer )
/// @brief Create string buffer with given allocator.
/// @param[in]  allocator Pointer to allocator interface.
/// @param      size      Size of allocation.
/// @param[out] out_buf   Pointer to write new string buffer to.
/// @return
///     - @c true  : String buffer successfully allocated.
///     - @c false : Failed to allocate string buffer.
attr_core_api
b32 string_buf_from_alloc(
    struct AllocatorInterface* allocator, usize size, struct _StringBufPOD* out_buf );
/// @brief Create string buffer with given allocator.
/// @param[in]  allocator Pointer to allocator interface.
/// @param      src       String to copy to buffer.
/// @param[out] out_buf   Pointer to write new string buffer to.
/// @return
///     - @c true  : String buffer successfully allocated.
///     - @c false : Failed to allocate string buffer.
attr_core_api
b32 string_buf_from_string_alloc(
    struct AllocatorInterface* allocator,
    struct _StringPOD src, struct _StringBufPOD* out_buf );
/// @brief Grow string buffer capacity.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer to grow.
/// @param     amount    Number of bytes to grow by.
/// @return
///     - @c true  : String buffer successfully reallocated.
///     - @c false : Failed to reallocate string buffer.
attr_core_api
b32 string_buf_grow(
    struct AllocatorInterface* allocator, struct _StringBufPOD* buf, usize amount );
/// @brief Free string buffer from allocator.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer to free.
attr_core_api
void string_buf_free(
    struct AllocatorInterface* allocator, struct _StringBufPOD* buf );
/// @brief Calculate remaining space in string buffer.
/// @param buf String buffer.
/// @return Number of bytes remaining in string buffer.
attr_always_inline attr_header
usize string_buf_remaining( struct _StringBufPOD buf ) {
    return buf.cap ? (buf.cap - 1) - buf.len : 0;
}
/// @brief Check if string buffer is empty.
/// @param buf Pointer to string buffer.
/// @return
///     - @c true  : String buffer is empty.
///     - @c false : String buffer is not empty.
attr_always_inline attr_header
b32 string_buf_is_empty( struct _StringBufPOD buf ) {
    return buf.len == 0;
}
/// @brief Check if string buffer is full.
/// @note String buffer always tries to have space for null terminator.
/// @param buf Pointer to string buffer.
/// @return
///     - @c true  : String buffer is full.
///     - @c false : String buffer still has space.
attr_always_inline attr_header
b32 string_buf_is_full( struct _StringBufPOD buf ) {
    return buf.len == (buf.cap - 1);
}
/// @brief Set string buffer length to zero and zero out memory.
/// @param[in] buf Pointer to string buffer.
attr_always_inline attr_header
void string_buf_clear( struct _StringBufPOD* buf ) {
    memory_set( buf->_void, 0, buf->len );
    buf->len = 0;
}
/// @brief Clone string buffer.
/// @details
/// Allocates @c src.len + 1 in @c dst buffer.
/// @param[in]  allocator Pointer to allocator interface.
/// @param[out] dst       Destination string buffer.
/// @param[in]  src       Source string buffer.
/// @return
///     - @c true  : Allocated @c dst buffer and copied contents of @c src to it.
///     - @c false : Failed to allocate @c dst buffer.
attr_core_api
b32 string_buf_clone(
    struct AllocatorInterface* allocator, struct _StringBufPOD* dst, struct _StringPOD src );
/// @brief Attempt to push character to end of string buffer.
/// @param[in] buf String buffer to push character to.
/// @param     c   Character to push.
/// @return
///     - @c true  : @c buf had enough space to push character.
///     - @c false : @c buf is full.
attr_core_api
b32 string_buf_try_push( struct _StringBufPOD* buf, char c );
/// @brief Push character to end of string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       String buffer to push character to.
/// @param     c         Character to push.
/// @return
///     - @c true  : Pushed new character. If allocation was required, it was successful.
///     - @c false : Failed to reallocate @c buf.
attr_core_api
b32 string_buf_push(
    struct AllocatorInterface* allocator, struct _StringBufPOD* buf, char c );
/// @brief Attempt to emplace a character inside of string buffer.
/// @param[in] buf String buffer to emplace character in.
/// @param     c   Character to emplace.
/// @param     at  Index to emplace character at.
/// @return
///     - @c true  : @c buf had enough space to emplace character.
///     - @c false : @c buf is full.
attr_core_api
b32 string_buf_try_emplace( struct _StringBufPOD* buf, char c, usize at );
/// @brief Emplace a character inside of string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       String buffer to emplace character in.
/// @param     c         Character to emplace.
/// @param     at        Index to emplace character at.
/// @return
///     - @c true  : Emplaced character. If allocation was required, it was successful.
///     - @c false : Failed to reallocate @c buf.
attr_core_api
b32 string_buf_emplace(
    struct AllocatorInterface* allocator, struct _StringBufPOD* buf, char c, usize at );
/// @brief Pop character from end of string buffer.
/// @param[in]  buf       String buffer to pop from.
/// @param[out] opt_out_c (optional) Pointer to write popped character to.
/// @return
///     - @c true  : @c buf had character to pop.
///     - @c false : @c buf was empty.
attr_core_api
b32 string_buf_pop( struct _StringBufPOD* buf, char* opt_out_c );
/// @brief Attempt to insert string in string buffer.
/// @param[in] buf    String buffer to insert in.
/// @param     insert String to insert.
/// @param     at     Index to insert at. Must be 0 to @c buf.len
/// @return
///     - @c true  : @c buf had enough capacity to insert string.
///     - @c false : @c buf did not have enough capacity to insert.
attr_core_api
b32 string_buf_try_insert( struct _StringBufPOD* buf, struct _StringPOD insert, usize at );
/// @brief Attempt to prepend string in string buffer.
/// @param[in] buf     String buffer to insert in.
/// @param     prepend String to prepend.
/// @return
///     - @c true  : @c buf had enough capacity to insert string.
///     - @c false : @c buf did not have enough capacity to insert.
attr_always_inline attr_header
b32 string_buf_try_prepend( struct _StringBufPOD* buf, struct _StringPOD prepend ) {
    return string_buf_try_insert( buf, prepend, 0 );
}
/// @brief Attempt to append string in string buffer.
/// @param[in] buf    String buffer to insert in.
/// @param     append String to append.
/// @return
///     - @c true  : @c buf had enough capacity to insert string.
///     - @c false : @c buf did not have enough capacity to insert.
attr_always_inline attr_header
b32 string_buf_try_append( struct _StringBufPOD* buf, struct _StringPOD append ) {
    return string_buf_try_insert( buf, append, buf->len );
}
/// @brief Insert string in string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer to insert in.
/// @param     insert    String to insert.
/// @param     at        Index to insert at.
/// @return
///     - @c true  : Inserted string. If reallocation was required, allocation succeeded.
///     - @c false : Failed to reallocate @c buf.
attr_core_api
b32 string_buf_insert(
    struct AllocatorInterface* allocator, struct _StringBufPOD* buf,
    struct _StringPOD insert, usize at );
/// @brief Prepend string in string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer to prepend in.
/// @param     prepend   String to prepend.
/// @return
///     - @c true  : Inserted string. If reallocation was required, allocation succeeded.
///     - @c false : Failed to reallocate @c buf.
attr_always_inline attr_header
b32 string_buf_prepend(
    struct AllocatorInterface* allocator, struct _StringBufPOD* buf,
    struct _StringPOD prepend
) {
    return string_buf_insert( allocator, buf, prepend, 0 );
}
/// @brief Append string in string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer to append in.
/// @param     append    String to append.
/// @return
///     - @c true  : Inserted string. If reallocation was required, allocation succeeded.
///     - @c false : Failed to reallocate @c buf.
attr_always_inline attr_header
b32 string_buf_append(
    struct AllocatorInterface* allocator, struct _StringBufPOD* buf,
    struct _StringPOD append
) {
    return string_buf_insert( allocator, buf, append, buf->len );
}
/// @brief Remove character from string buffer.
/// @param[in] buf Buffer to remove character from.
/// @param     at  Index of character to remove. Must be in bounds.
attr_core_api
void string_buf_remove( struct _StringBufPOD* buf, usize at );
/// @brief Remove range of characters from string buffer.
/// @param[in] buf            Buffer to remove characters from.
/// @param     from_inclusive Start of range to remove.
/// @param     to_exclusive   End of range to remove.
attr_core_api
void string_buf_remove_range(
    struct _StringBufPOD* buf, usize from_inclusive, usize to_exclusive );

/// @brief Stream format function for string buffer.
/// @param[in] StringBuf Pointer to string buffer.
/// @param     count     Number of bytes to stream to buffer.
/// @param[in] bytes     Pointer to bytes to stream to buffer.
/// @return
///     - Zero   : @c buf had enough capacity to write format string.
///     - > Zero : Number of characters not written to @c buf.
attr_core_api
usize string_buf_try_stream(
    void* StringBuf, usize count, const void* bytes );
/// @brief Stream format function for string buffer.
/// @param[in] StringBufStreamTarget Pointer to #StringBufStreamTarget.
/// @param     count                 Number of bytes to stream to buffer.
/// @param[in] bytes                 Pointer to bytes to stream to buffer.
/// @return
///     - @c true  : Wrote entire format string to @c buf. If allocation was required, it was successful.
///     - @c false : Failed to reallocate @c buf.
attr_core_api
bsize string_buf_stream(
    void* StringBufStreamTarget, usize count, const void* bytes );

/// @brief Try to write formatted string to end of string buffer.
/// @param[in] buf (StringBuf*)     Pointer to string buffer to write to.
/// @param     fmt (string literal) Format string literal.
/// @param     va  (va_list)        Variadic arguments.
/// @return Number of bytes that could not be written to buffer.
#define string_buf_try_fmt_va( buf, fmt, va ) \
    internal_string_buf_try_fmt_va( buf, sizeof(fmt) - 1, fmt, va )

/// @brief Try to write formatted string to end of string buffer.
/// @param[in] buf (StringBuf*)     Pointer to string buffer to write to.
/// @param     fmt (string literal) Format string literal.
/// @param     ... (arguments)      Arguments.
/// @return Number of bytes that could not be written to buffer.
#define string_buf_try_fmt( buf, fmt, ... ) \
    internal_string_buf_try_fmt( buf, sizeof(fmt) - 1, fmt, ##__VA_ARGS__ )

/// @brief Write formatted string to end of string buffer.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator.
/// @param[in] buf       (StringBuf*)          Pointer to string buffer to write to.
/// @param     fmt       (string literal)      Format string literal.
/// @param     va        (va_list)             Variadic arguments.
/// @return
///     - true  : Wrote formatted string to buffer successfully.
///     - false : Failed to reallocate string buffer.
#define string_buf_fmt_va( allocator, buf, fmt, va ) \
    internal_string_buf_fmt_va( allocator, buf, sizeof(fmt) - 1, fmt, va )

/// @brief Write formatted string to end of string buffer.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator.
/// @param[in] buf       (StringBuf*)          Pointer to string buffer to write to.
/// @param     fmt       (string literal)      Format string literal.
/// @param     ...       (arguments)           Arguments.
/// @return
///     - true  : Wrote formatted string to buffer successfully.
///     - false : Failed to reallocate string buffer.
#define string_buf_fmt( allocator, buf, fmt, ... ) \
    internal_string_buf_fmt( allocator, buf, sizeof(fmt) - 1, fmt, ##__VA_ARGS__ )


attr_core_api
usize internal_string_buf_try_fmt_va(
    struct _StringBufPOD* buf, usize format_len, const char* format, va_list va );

attr_always_inline attr_header
usize internal_string_buf_try_fmt(
    struct _StringBufPOD* buf, usize format_len, const char* format, ...
) {
    va_list va;
    va_start( va, format );
    usize result = internal_string_buf_try_fmt_va( buf, format_len, format, va );
    va_end( va );
    return result;
}

attr_core_api
b32 internal_string_buf_fmt_va(
    struct AllocatorInterface* allocator, struct _StringBufPOD* buf,
    usize format_len, const char* format, va_list va );

attr_always_inline attr_header
b32 internal_string_buf_fmt(
    struct AllocatorInterface* allocator, struct _StringBufPOD* buf,
    usize format_len, const char* format, ...
) {
    va_list va;
    va_start( va, format );
    b32 result = internal_string_buf_fmt_va( allocator, buf, format_len, format, va );
    va_end( va );
    return result;
}

#if defined(CORE_CPLUSPLUS)
    #if !defined(CORE_CPP_STRING_HPP)
        #include "core/cpp/string.hpp"
    #endif
    /// @brief UTF-8 String.
    /// @details
    /// Not necessarily null terminated.
    typedef _StringCPP    String;
    /// @brief UTF-8 String Buffer.
    /// @details
    /// Always accounts for null terminator.
    typedef _StringBufCPP StringBuf;
#endif

#endif /* header guard */
