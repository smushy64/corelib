#if !defined(CORE_CPP_STRING_HPP) && defined(__cplusplus)
#define CORE_CPP_STRING_HPP
/**
 * @file   string.hpp
 * @brief  C++ String.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 01, 2025
*/
#if !defined(CORE_STRING_H)
    #include "core/string.h"
#endif
#include "core/slice.h"

/// @brief UTF-8 String.
/// @details
/// Not necessarily null terminated.
struct _StringCPP;
/// @brief UTF-8 String Buffer.
/// @details
/// Not necessarily null terminated.
struct _StringBufCPP;

/// @brief UTF-8 String.
/// @details
/// Not necessarily null terminated.
typedef _StringCPP    String;
/// @brief UTF-8 String Buffer.
/// @details
/// Not necessarily null terminated.
typedef _StringBufCPP StringBuf;

/// @brief Check if string is empty.
/// @param str String to check.
/// @return
///     - @c true  : @c str is empty.
///     - @c false : @c str is not empty.
attr_header
bool string_is_empty( String str );
/// @brief Calculate UTF-8 length of string.
/// @note This function does not check if string is valid UTF-8!
/// @param str String to calculate length of.
/// @return UTF-8 length.
attr_header
usize string_len_utf8( String str );
/// @brief Index into string. Debug asserts that index is in bounds.
/// @param str   String to index into.
/// @param index Index of character.
/// @return Character at given index.
attr_header
char string_index( String str, usize index );
/// @brief Index into string, using UTF-8 index. Debug asserts that index is in bounds.
/// @param str   String to index into.
/// @param index UTF-8 index.
/// @return Codepoint at given index.
attr_header
c32 string_index_utf8( String str, usize index );
/// @brief Get next rune in string and advance.
/// @param      src           String to get next codepoint from.
/// @param[out] out_codepoint Pointer to write codepoint to.
/// @return String advanced by number of bytes in rune.
attr_header
String string_utf8_next( String src, c32* out_codepoint );
/// @brief Get pointer to first character in string.
/// @param str String to get pointer from.
/// @return
///     - @c NULL : @c str is empty
///     - Pointer : Pointer to first character in @c str.
attr_header
char* string_first( String str );
/// @brief Get pointer to last character in string.
/// @param str String to get pointer from.
/// @return
///     - @c NULL    : @c str is empty.
///     - @c Pointer : Pointer to last character in @c str.
attr_header
char* string_last( String str );
/// @brief Get first character in string without bounds checking.
/// @param str String to get character from.
/// @return First character in string.
attr_header
char string_first_unchecked( String str );
/// @brief Get last character in string without bounds checking.
/// @param str String to get character from.
/// @return Last character in string.
attr_header
char string_last_unchecked( String str );
/// @brief Compare strings for equality.
/// @param a, b Strings to compare.
/// @return
///     - @c true  : @c a and @c b are equal in length and contents.
///     - @c false : @c a and @c b are not equal.
attr_header
bool string_cmp( String a, String b );
/// @brief Search for ascii character in string.
/// @param      str           String to search in.
/// @param      c             Character to search for.
/// @param[out] opt_out_index (optional) Pointer to write index of character if found.
/// @return
///     - @c true  : Character was found. Index written to @c opt_out_index (if not null)
///     - @c false : Character was not found.
attr_header
bool string_find( String str, char c, usize* opt_out_index = nullptr );
/// @brief Tally number of times that given character occurs in string.
/// @param str String.
/// @param c   Character to tally.
/// @return Number of times @c c appears in @c str.
attr_header
usize string_find_count( String str, char c );
/// @brief Search for ascii character in string, searches from the end of string.
/// @param      str           String to search in.
/// @param      c             Character to search for.
/// @param[out] opt_out_index (optional) Pointer to write index of character if found.
/// @return
///     - @c true  : Character was found. Index written to @c opt_out_index (if not null)
///     - @c false : Character was not found.
attr_header
bool string_find_rev( String str, char c, usize* opt_out_index = nullptr );
/// @brief Search for any character in set in string.
/// @param      str           String to search in.
/// @param      set           Set of characters to search for.
/// @param[out] opt_out_index (optional) Pointer to write index of first character in set if any are found.
/// @return
///     - @c true  : Any character from set was found. Index written to @c opt_out_index (if not null)
///     - @c false : No characters from set were found.
attr_header
bool string_find_set( String str, String set, usize* opt_out_index = nullptr );
/// @brief Search for any character in set in string, searches from end of string.
/// @param      str           String to search in.
/// @param      set           Set of characters to search for.
/// @param[out] opt_out_index (optional) Pointer to write index of first character in set if any are found.
/// @return
///     - @c true  : Any character from set was found. Index written to @c opt_out_index (if not null)
///     - @c false : No characters from set were found.
attr_header
bool string_find_set_rev( String str, String set, usize* opt_out_index = nullptr );
/// @brief Tally number of times that any character in set occurs in string.
/// @param str String.
/// @param set Set to tally.
/// @return Number of times any character in @c set appears in @c str.
attr_header
usize string_find_set_count( String str, String set );
/// @brief Search for phrase in string.
/// @param str String to search in.
/// @param phrase Phrase to search for.
/// @param[out] opt_out_index (optional) Pointer to write index of first character of phrase. (if not null)
/// @return 
///     - @c true  : @c phrase was found in @c str.
///     - @c false : @c phrase was not found.
attr_header
bool string_find_phrase( String str, String phrase, usize* opt_out_index = nullptr );
/// @brief Search for phrase in string, searches from end of string.
/// @param str String to search in.
/// @param phrase Phrase to search for.
/// @param[out] opt_out_index (optional) Pointer to write index of first character of phrase. (if not null)
/// @return 
///     - @c true  : @c phrase was found in @c str.
///     - @c false : @c phrase was not found.
attr_header
bool string_find_phrase_rev( String str, String phrase, usize* opt_out_index = nullptr );
/// @brief Tally number of times that phrase occurs in string.
/// @param str    String.
/// @param phrase Phrase to tally.
/// @return Number of times @c phrase appears in @c str.
attr_header
usize string_find_phrase_count( String str, String phrase );
/// @brief Truncate length of string.
/// @details
/// Returns str if max is greater than length.
/// @param str String to truncate.
/// @param max Maximum length of string.
/// @return Truncated string.
attr_header
String string_truncate( String str, usize max );
/// @brief Subtract from string length.
/// @details
/// Returns empty string if amount is >= to length.
/// @param str    String to trim.
/// @param amount Number of bytes to trim off the end.
/// @return Trimmed string.
attr_header
String string_trim( String str, usize amount );
/// @brief Clip out slice from string.
/// @warning
/// Debug asserts that to > from and that they are within bounds of string.
/// @param str            String to clip from.
/// @param from_inclusive Start of range, inclusive.
/// @param to_exclusive   End of range, exclusive.
/// @return String slice.
attr_header
String string_clip( String str, usize from, usize to );
/// @brief Advance string slice by multiple bytes.
/// @details
/// If @c bytes is >= length, returns empty string.
/// @param str   String to advance.
/// @param bytes Bytes to advance by.
/// @return String advanced by @c bytes.
attr_header
String string_advance( String str, usize amount = 1 );
/// @brief Trim leading whitespace from string.
/// @param str String to trim.
/// @return Trimmed string.
attr_header
String string_trim_leading_whitespace( String str );
/// @brief Trim trailing whitespace from string.
/// @param str String to trim.
/// @return Trimmed string.
attr_header
String string_trim_trailing_whitespace( String str );
/// @brief Trim leading and trailing whitespace from string.
/// @param str String to trim.
/// @return Trimmed string.
attr_header
String string_trim_surrounding_whitespace( String str );
/// @brief Split string at given index.
/// @details
/// Debug asserts that @c at is <= source length.
/// @param      source        String to split from.
/// @param      at            Index to split at. Character at this index is not included in splits.
/// @param[out] opt_out_left  (optional) Pointer to write left side of split.
/// @param[out] opt_out_right (optional) Pointer to write right side of split.
attr_header
void string_split(
    String  source,
    usize   at,
    String* opt_out_left  = nullptr,
    String* opt_out_right = nullptr );
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
attr_header
bool string_split_ascii(
    String  source,
    char    c,
    String* opt_out_left  = nullptr,
    String* opt_out_right = nullptr );
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
attr_header
bool string_whitespace(
    String  source,
    String* opt_out_left  = nullptr,
    String* opt_out_right = nullptr );
/// @brief Parse signed integer from source string.
/// @details Parsing is successful if valid integer is found from start of string.
/// @param      str     String to parse integer from.
/// @param[out] out_int Pointer to integer to receive result.
/// @return
///     - @c true  : Integer was successfully parsed from string.
///     - @c false : Failed to parse integer.
attr_header
bool string_parse_int( String str, i64* out_int );
/// @brief Parse unsigned integer from source string.
/// @details Parsing is successful if valid integer is found from start of string.
/// @param      str      String to parse integer from.
/// @param[out] out_uint Pointer to integer to receive result.
/// @return
///     - @c true  : Integer was successfully parsed from string.
///     - @c false : Failed to parse integer.
attr_header
bool string_parse_uint( String str, u64* out_uint );
/// @brief Parse float from source string.
/// @details Parsing is successful if valid float is found from start of string.
/// @param      str       String to parse float from.
/// @param[out] out_float Pointer to float to receive result.
/// @return
///     - @c true  : Float was successfully parsed from string.
///     - @c false : Failed to parse float.
attr_header
bool string_parse_float( String str, f64* out_float );

// NOTE(alicia):
// string_mut don't have overloads with C++ string
// the point of these overloads is to use string literals with these functions ""
// or to fill in optional arguments with default values

/// @brief Convert all alphabetic ascii characters to upper case.
/// @param[in] stream Stream function.
/// @param[in] target Target to stream to.
/// @param     str    Source string.
/// @return Number of characters that could not fit in stream.
/// @see #StreamBytesFN
attr_header
usize string_stream_to_upper( StreamBytesFN* stream, void* target, String str );
/// @brief Convert all alphabetic ascii characters to lower case.
/// @param[in] stream Stream function.
/// @param[in] target Target to stream to.
/// @param     str    Source string.
/// @return Number of characters that could not fit in stream.
/// @see #StreamBytesFN
attr_header
usize string_stream_to_lower( StreamBytesFN* stream, void* target, String str );
/// @brief Compare strings for equality.
/// @param a, b Strings to compare.
/// @return
///     - @c true  : @c a and @c b are equal in length and contents.
///     - @c false : @c a and @c b are not equal.
attr_header
bool cmp( String a, String b );
/// @brief Create string.
/// @param[in] str String.
/// @param     len Length of string.
/// @return String.
attr_header
String operator ""_s( const char* str, usize len );

#undef string_buf_from_stack

/// @brief Initialize a string buffer from the stack.
/// @details
/// Defines a stack char buffer with name @c name\#\#_buffer
/// of given size and a StringBuf from that buffer with given name.
/// @param name (valid identifier) Name of string buffer.
/// @param size (usize)            Size of string buffer.
#define string_buf_from_stack( name, size ) \
    char      name##_buffer[size];          \
    StringBuf name = string_buf_new( size, memory_zero( name##_buffer, size ) )
/// @brief Create string buffer with given allocator.
/// @param[in]  allocator Pointer to allocator interface.
/// @param      size      Size of allocation.
/// @param[out] out_buf   Pointer to write new string buffer to.
/// @return
///     - @c true  : String buffer successfully allocated.
///     - @c false : Failed to allocate string buffer.
attr_header
bool string_buf_from_alloc( AllocatorInterface* allocator, usize size, StringBuf* out_buf );
/// @brief Create string buffer with given allocator.
/// @param[in]  allocator Pointer to allocator interface.
/// @param      src       String to copy to buffer.
/// @param[out] out_buf   Pointer to write new string buffer to.
/// @return
///     - @c true  : String buffer successfully allocated.
///     - @c false : Failed to allocate string buffer.
attr_header
bool string_buf_from_string_alloc(
    AllocatorInterface* allocator, String src, StringBuf* out_buf );
/// @brief Grow string buffer capacity.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer to grow.
/// @param     amount    Number of bytes to grow by.
/// @return
///     - @c true  : String buffer successfully reallocated.
///     - @c false : Failed to reallocate string buffer.
attr_header
bool string_buf_grow( AllocatorInterface* allocator, StringBuf* buf, usize amount );
/// @brief Free string buffer from allocator.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer to free.
attr_header
void string_buf_free( AllocatorInterface* allocator, StringBuf* buf );
/// @brief Calculate remaining space in string buffer.
/// @param buf String buffer.
/// @return Number of bytes remaining in string buffer.
attr_header
usize string_buf_remaining( const StringBuf& buf );
/// @brief Check if string buffer is empty.
/// @param buf Pointer to string buffer.
/// @return
///     - @c true  : String buffer is empty.
///     - @c false : String buffer is not empty.
attr_header
bool string_buf_is_empty( const StringBuf& buf );
/// @brief Check if string buffer is full.
/// @note String buffer always tries to have space for null terminator.
/// @param buf Pointer to string buffer.
/// @return
///     - @c true  : String buffer is full.
///     - @c false : String buffer still has space.
attr_header
bool string_buf_is_full( const StringBuf& buf );
/// @brief Set string buffer length to zero and zero out memory.
/// @param[in] buf Pointer to string buffer.
attr_header
void string_buf_clear( StringBuf* buf );
/// @brief Clone string buffer.
/// @details
/// Allocates @c src.len + 1 in @c dst buffer.
/// @param[in]  allocator Pointer to allocator interface.
/// @param[out] dst       Destination string buffer.
/// @param[in]  src       Source string buffer.
/// @return
///     - @c true  : Allocated @c dst buffer and copied contents of @c src to it.
///     - @c false : Failed to allocate @c dst buffer.
attr_header
bool string_buf_clone( AllocatorInterface* allocator, StringBuf* dst, String src );
/// @brief Attempt to push character to end of string buffer.
/// @param[in] buf String buffer to push character to.
/// @param     c   Character to push.
/// @return
///     - @c true  : @c buf had enough space to push character.
///     - @c false : @c buf is full.
attr_header
bool string_buf_try_push( StringBuf* buf, char c );
/// @brief Push character to end of string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       String buffer to push character to.
/// @param     c         Character to push.
/// @return
///     - @c true  : Pushed new character. If allocation was required, it was successful.
///     - @c false : Failed to reallocate @c buf.
attr_header
bool string_buf_push( AllocatorInterface* allocator, StringBuf* buf, char c );
/// @brief Attempt to emplace a character inside of string buffer.
/// @param[in] buf String buffer to emplace character in.
/// @param     c   Character to emplace.
/// @param     at  Index to emplace character at.
/// @return
///     - @c true  : @c buf had enough space to emplace character.
///     - @c false : @c buf is full.
attr_header
bool string_buf_try_emplace( StringBuf* buf, char c, usize at );
/// @brief Emplace a character inside of string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       String buffer to emplace character in.
/// @param     c         Character to emplace.
/// @param     at        Index to emplace character at.
/// @return
///     - @c true  : Emplaced character. If allocation was required, it was successful.
///     - @c false : Failed to reallocate @c buf.
attr_header
bool string_buf_emplace( AllocatorInterface* allocator, StringBuf* buf, char c, usize at );
/// @brief Pop character from end of string buffer.
/// @param[in]  buf       String buffer to pop from.
/// @param[out] opt_out_c (optional) Pointer to write popped character to.
/// @return
///     - @c true  : @c buf had character to pop.
///     - @c false : @c buf was empty.
attr_header
bool string_buf_pop( StringBuf* buf, char* opt_out_c = nullptr );
/// @brief Attempt to insert string in string buffer.
/// @param[in] buf    String buffer to insert in.
/// @param     insert String to insert.
/// @param     at     Index to insert at. Must be 0 to @c buf.len
/// @return
///     - @c true  : @c buf had enough capacity to insert string.
///     - @c false : @c buf did not have enough capacity to insert.
attr_header
bool string_buf_try_insert( StringBuf* buf, String insert, usize at );
/// @brief Attempt to prepend string in string buffer.
/// @param[in] buf     String buffer to insert in.
/// @param     prepend String to prepend.
/// @return
///     - @c true  : @c buf had enough capacity to insert string.
///     - @c false : @c buf did not have enough capacity to insert.
attr_header
bool string_buf_try_prepend( StringBuf* buf, String prepend );
/// @brief Attempt to append string in string buffer.
/// @param[in] buf    String buffer to insert in.
/// @param     append String to append.
/// @return
///     - @c true  : @c buf had enough capacity to insert string.
///     - @c false : @c buf did not have enough capacity to insert.
attr_header
bool string_buf_try_append( StringBuf* buf, String append );
/// @brief Insert string in string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer to insert in.
/// @param     insert    String to insert.
/// @param     at        Index to insert at.
/// @return
///     - @c true  : Inserted string. If reallocation was required, allocation succeeded.
///     - @c false : Failed to reallocate @c buf.
attr_header
bool string_buf_insert(
    AllocatorInterface* allocator, StringBuf* buf, String insert, usize at );
/// @brief Prepend string in string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer to prepend in.
/// @param     prepend   String to prepend.
/// @return
///     - @c true  : Inserted string. If reallocation was required, allocation succeeded.
///     - @c false : Failed to reallocate @c buf.
attr_header
bool string_buf_prepend(
    AllocatorInterface* allocator, StringBuf* buf, String prepend );
/// @brief Append string in string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer to append in.
/// @param     append    String to append.
/// @return
///     - @c true  : Inserted string. If reallocation was required, allocation succeeded.
///     - @c false : Failed to reallocate @c buf.
attr_header
bool string_buf_append(
    AllocatorInterface* allocator, StringBuf* buf, String append );
/// @brief Remove character from string buffer.
/// @param[in] buf Buffer to remove character from.
/// @param     at  Index of character to remove. Must be in bounds.
attr_header
void string_buf_remove( StringBuf* buf, usize at );
/// @brief Remove range of characters from string buffer.
/// @param[in] buf  Buffer to remove characters from.
/// @param     from Start of range to remove.
/// @param     to   End of range to remove.
attr_header
void string_buf_remove_range( StringBuf* buf, usize from, usize to );

// NOTE(alicia): 
// usize string_buf_try_stream( void* StringBuf, usize count, const void* bytes );
// bsize string_buf_stream( void* StringBufStreamTarget, usize count, const void* bytes );

/// @brief Attempt to write formatted string to end of string buffer.
/// @param[in] buf    Pointer to string buffer.
/// @param     format Format string.
/// @param[in] va     Variadic arguments.
/// @return Number of bytes that could not be written to buffer.
attr_header
usize string_buf_try_fmt_va( StringBuf* buf, String format, va_list va );
/// @brief Attempt to write formatted string to end of string buffer.
/// @param[in] buf    Pointer to string buffer.
/// @param     format Format string.
/// @param[in] ...    Arguments.
/// @return Number of bytes that could not be written to buffer.
attr_header
usize string_buf_try_fmt( StringBuf* buf, String format, ... );
/// @brief Write formatted string to end of string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer.
/// @param     format    Format string.
/// @param[in] va        Variadic arguments.
/// @return
///     - @c true  : Formatted string written to string buffer.
///     - @c false : Failed to reallocate string buffer.
attr_header
bsize string_buf_fmt_va(
    AllocatorInterface* allocator,
    StringBuf*          buf,
    String              format,
    va_list             va );
/// @brief Write formatted string to end of string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer.
/// @param     format    Format string.
/// @param[in] ...       Arguments.
/// @return
///     - @c true  : Formatted string written to string buffer.
///     - @c false : Failed to reallocate string buffer.
attr_header
bsize string_buf_fmt(
    AllocatorInterface* allocator,
    StringBuf*          buf,
    String              format,
    ... );

#undef string_buf_try_fmt_va_text
#undef string_buf_try_fmt_text
#undef string_buf_fmt_va_text
#undef string_buf_fmt_text

/// @brief Attempt to write formatted string to end of string buffer.
/// @param[in] buf    Pointer to string buffer.
/// @param     format Format string.
/// @param[in] va     Variadic arguments.
/// @return Number of bytes that could not be written to buffer.
#define string_buf_try_fmt_va_text( buf, format, va ) \
    string_buf_try_fmt_va( buf, format, va )
/// @brief Attempt to write formatted string to end of string buffer.
/// @param[in] buf    Pointer to string buffer.
/// @param     format Format string.
/// @param[in] ...    Arguments.
/// @return Number of bytes that could not be written to buffer.
#define string_buf_try_fmt_text( buf, format, ... ) \
    string_buf_try_fmt( buf, format, ##__VA_ARGS__ )
/// @brief Write formatted string to end of string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer.
/// @param     format    Format string.
/// @param[in] va        Variadic arguments.
/// @return
///     - @c true  : Formatted string written to string buffer.
///     - @c false : Failed to reallocate string buffer.
#define string_buf_fmt_va_text( allocator, buf, format, va ) \
    string_buf_fmt_va( allocator, buf, format, va )
/// @brief Write formatted string to end of string buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to string buffer.
/// @param     format    Format string.
/// @param[in] ...       Arguments.
/// @return
///     - @c true  : Formatted string written to string buffer.
///     - @c false : Failed to reallocate string buffer.
#define string_buf_fmt_text( allocator, buf, format, ... ) \
    string_buf_fmt( allocator, buf, format, ##__VA_ARGS__ )

/// @brief UTF-8 String.
/// @details
/// Not necessarily null terminated.
struct _StringCPP {
    /// @brief Type of items in string.
    typedef char Type;

    /// @brief Union of string slice components.
    union {
        /// @brief String slice components.
        struct {
            /// @brief Number of bytes in string.
            usize len;
            /// @brief Union of string slice pointers.
            union {
                /// @brief Const pointer to string bytes.
                const char* cbuf;
                /// @brief Pointer to string bytes.
                char*        buf;
                /// @brief Pointer to string bytes.
                u8*        bytes;
                /// @brief Pointer to string bytes.
                c8*         utf8;
                /// @brief Pointer to string data.
                void*        ptr;
            };
        };
        _StringPOD __pod;
    };

    /// @brief Create empty string.
    constexpr _StringCPP();
    /// @brief Create string.
    /// @param     len Length of string.
    /// @param[in] buf Pointer to start of string buffer.
    constexpr explicit _StringCPP( usize len, const char* buf );
    /// @brief Create string from array.
    /// @param[in] array       Array of string characters.
    /// @tparam    ArrayLength Number of bytes in string array.
    template<usize ArrayLength>
    constexpr _StringCPP( const char (&array)[ArrayLength] );
    /// @brief Create string from C-string.
    /// @param[in] cstr Pointer to start of string.
    constexpr explicit _StringCPP( const char* cstr );
    /// @brief Implicitly convert POD string to C++ string.
    /// @param __pod POD string.
    constexpr _StringCPP( const _StringPOD& __pod );
    /// @brief Implicitly convert slice to string.
    /// @param slice Slice of characters.
    constexpr _StringCPP( const Slice<char>& slice );
    /// @brief Implicitly convert slice to string.
    /// @param slice Slice of characters.
    constexpr _StringCPP( const Slice<u8>& slice );
    /// @brief Implicitly convert slice to string.
    /// @param slice Slice of characters.
    constexpr _StringCPP( const Slice<i8>& slice );

    /// @brief Implicitly convert C++ string to POD string.
    /// @return POD string.
    constexpr operator _StringPOD() const;
    /// @brief Implicitly convert C++ string to slice.
    /// @return Slice.
    constexpr operator Slice<char>() const;
    /// @brief Implicitly convert C++ string to slice.
    /// @return Slice.
    constexpr operator Slice<i8>() const;
    /// @brief Implicitly convert C++ string to slice.
    /// @return Slice.
    constexpr operator Slice<u8>() const;

    /// @brief Index into string buffer.
    /// @param index Index of byte.
    /// @return Byte.
    constexpr char operator[]( usize index ) const;
    /// @brief Index into string buffer.
    /// @param index Index of byte.
    /// @return Byte.
    constexpr char& operator[]( usize index );

    /// @brief Range-based for loop begin.
    /// @return Pointer to start of string.
    const char* begin() const;
    /// @brief Range-based for loop end.
    /// @return Pointer to end of string.
    const char* end() const;
    /// @brief Range-based for loop begin.
    /// @return Pointer to start of string.
    char* begin();
    /// @brief Range-based for loop end.
    /// @return Pointer to end of string.
    char* end();
};
/// @brief UTF-8 String Buffer.
/// @details
/// Not necessarily null terminated.
struct _StringBufCPP {
    /// @brief Type of items in string.
    typedef char Type;

    /// @brief Union of string buffer components.
    union {
        /// @brief String buffer components.
        struct {
            /// @brief Number of bytes string buffer can hold.
            usize cap;
            /// @brief Union of string components.
            union {
                /// @brief String slice components.
                struct {
                    /// @brief Number of bytes in string.
                    usize len;
                    /// @brief Union of string buffer pointers.
                    union {
                        /// @brief Const pointer to string bytes.
                        const char* cbuf;
                        /// @brief Pointer to string bytes.
                        char*        buf;
                        /// @brief Pointer to string bytes.
                        u8*        bytes;
                        /// @brief Pointer to string bytes.
                        c8*         utf8;
                        /// @brief Pointer to string data.
                        void*        ptr;
                    };
                };
                /// @brief String buffer as a slice.
                _StringCPP slice;
                _StringPOD __slice_pod;
            };
        };
        _StringBufPOD __pod;
    };

    /// @brief Create empty string buffer.
    constexpr _StringBufCPP();
    /// @brief Create string buffer.
    /// @param     cap Number of character bytes buffer can hold.
    /// @param[in] buf Pointer to start of buffer.
    constexpr explicit _StringBufCPP( usize cap, const char* buf );
    /// @brief Create string buffer.
    /// @param     cap Number of character bytes buffer can hold.
    /// @param     len Initial number of bytes in buffer.
    /// @param[in] buf Pointer to start of buffer.
    constexpr explicit _StringBufCPP( usize cap, usize len, const char* buf );
    /// @brief Create string buffer.
    /// @param[in] slice   String to create buffer from.
    /// @param     opt_len (optional) Initial length of buffer.
    constexpr explicit _StringBufCPP( const _StringPOD& slice, usize opt_len = 0 );
    /// @brief Create string buffer.
    /// @param[in] slice   String to create buffer from.
    /// @param     opt_len (optional) Initial length of buffer.
    constexpr explicit _StringBufCPP( const Slice<char>& slice, usize opt_len = 0 );
    /// @brief Create string buffer.
    /// @param[in] slice   String to create buffer from.
    /// @param     opt_len (optional) Initial length of buffer.
    constexpr explicit _StringBufCPP( const Slice<i8>& slice, usize opt_len = 0 );
    /// @brief Create string buffer.
    /// @param[in] slice   String to create buffer from.
    /// @param     opt_len (optional) Initial length of buffer.
    constexpr explicit _StringBufCPP( const Slice<u8>& slice, usize opt_len = 0 );
    /// @brief Create string buffer.
    /// @param[in] array       Array to create buffer from.
    /// @param     opt_len     (optional) Initial length of buffer.
    /// @tparam    ArrayLength Number of bytes array can hold.
    template<usize ArrayLength>
    constexpr explicit _StringBufCPP( const char (&array)[ArrayLength], usize opt_len = 0 );

    /// @brief Implicitly convert POD string buffer to C++ string buffer.
    /// @param pod String buffer (POD).
    constexpr _StringBufCPP( const _StringBufPOD& pod );

    /// @brief Implicitly convert C++ string buffer to POD string buffer.
    /// @return POD string buffer.
    constexpr operator _StringBufPOD() const;
    /// @brief Implicitly convert string buffer to string.
    /// @return String.
    constexpr operator _StringCPP() const;
    /// @brief Implicitly convert string buffer to POD string.
    /// @return POD string.
    constexpr operator _StringPOD() const;
    /// @brief Implicitly convert string buffer to slice.
    /// @return Slice.
    constexpr operator Slice<char>() const;
    /// @brief Implicitly convert string buffer to slice.
    /// @return Slice.
    constexpr operator Slice<u8>() const;
    /// @brief Implicitly convert string buffer to slice.
    /// @return Slice.
    constexpr operator Slice<i8>() const;

    /// @brief Index into string buffer.
    /// @param index Index of byte.
    /// @return Byte.
    constexpr char operator[]( usize index ) const;
    /// @brief Index into string buffer.
    /// @param index Index of byte.
    /// @return Byte.
    constexpr char& operator[]( usize index );

    /// @brief Range-based for loop begin.
    /// @return Pointer to start of string.
    const char* begin() const;
    /// @brief Range-based for loop end.
    /// @return Pointer to end of string.
    const char* end() const;
    /// @brief Range-based for loop begin.
    /// @return Pointer to start of string.
    char* begin();
    /// @brief Range-based for loop end.
    /// @return Pointer to end of string.
    char* end();
};

// NOTE(alicia): implementation -----------------------------------------------

attr_always_inline attr_header attr_hot constexpr
usize ___strlen___( const char* cstr ) {
    usize result = 0;
    while( *cstr++ ) {
        result++;
    }
    return result;
}

constexpr attr_header
_StringCPP::_StringCPP()
    : len(0), cbuf(nullptr) {}
constexpr attr_header
_StringCPP::_StringCPP( usize len, const char* buf )
    : len(len), cbuf(buf) {}
template<usize ArrayLength> constexpr attr_always_inline attr_header attr_hot
_StringCPP::_StringCPP( const char (&array)[ArrayLength] )
    : len(ArrayLength ? ArrayLength - 1 : 0), cbuf(array) {}
constexpr attr_always_inline attr_header attr_hot
_StringCPP::_StringCPP( const char* cstr )
    : len(___strlen___(cstr)), cbuf(cstr) {}
constexpr attr_header
_StringCPP::_StringCPP( const _StringPOD& __pod )
    : len(__pod.len), cbuf(__pod.cbuf) {}
constexpr attr_header
_StringCPP::_StringCPP( const Slice<char>& slice )
    : len(slice.len), cbuf((const char*)slice.cbuf) {}
constexpr attr_header
_StringCPP::_StringCPP( const Slice<u8>& slice )
    : len(slice.len), cbuf((const char*)slice.cbuf) {}
constexpr attr_header
_StringCPP::_StringCPP( const Slice<i8>& slice )
    : len(slice.len), cbuf((const char*)slice.cbuf) {}

constexpr attr_header
_StringCPP::operator _StringPOD() const {
    return this->__pod;
}
constexpr attr_header
_StringCPP::operator Slice<char>() const {
    return *(Slice<char>*)this;
}
constexpr attr_header
_StringCPP::operator Slice<i8>() const {
    return *(Slice<i8>*)this;
}
constexpr attr_header
_StringCPP::operator Slice<u8>() const {
    return *(Slice<u8>*)this;
}

constexpr attr_header
char _StringCPP::operator[]( usize index ) const {
    return this->cbuf[index];
}
constexpr attr_header
char& _StringCPP::operator[]( usize index ) {
    return this->buf[index];
}

attr_header
const char* _StringCPP::begin() const {
    return this->cbuf;
}
attr_header
const char* _StringCPP::end() const {
    return this->cbuf + this->len;
}
attr_header
char* _StringCPP::begin() {
    return this->buf;
}
attr_header
char* _StringCPP::end() {
    return this->buf + this->len;
}

attr_always_inline attr_header attr_hot
bool string_is_empty( String str ) {
    return string_is_empty( str.__pod );
}
attr_always_inline attr_header attr_hot
usize string_len_utf8( String str ) {
    return string_len_utf8( str.__pod );
}
attr_always_inline attr_header attr_hot
char string_index( String str, usize index ) {
    return string_index( str.__pod, index );
}
attr_always_inline attr_header attr_hot
c32 string_index_utf8( String str, usize index ) {
    return string_index_utf8( str.__pod, index );
}
attr_always_inline attr_header attr_hot
String string_utf8_next( String src, c32* out_codepoint ) {
    return string_utf8_next( src.__pod, out_codepoint );
}
attr_always_inline attr_header attr_hot
char* string_first( String str ) {
    return string_first( str.__pod );
}
attr_always_inline attr_header attr_hot
char* string_last( String str ) {
    return string_last( str.__pod );
}
attr_always_inline attr_header attr_hot
char string_first_unchecked( String str ) {
    return string_first_unchecked( str.__pod );
}
attr_always_inline attr_header attr_hot
char string_last_unchecked( String str ) {
    return string_last_unchecked( str.__pod );
}
attr_always_inline attr_header attr_hot
bool string_cmp( String a, String b ) {
    return string_cmp( a.__pod, b.__pod );
}
attr_always_inline attr_header attr_hot
bool string_find( String str, char c, usize* opt_out_index ) {
    return string_find( str.__pod, c, opt_out_index );
}
attr_always_inline attr_header attr_hot
usize string_find_count( String str, char c ) {
    return string_find_count( str.__pod, c );
}
attr_always_inline attr_header attr_hot
bool string_find_rev( String str, char c, usize* opt_out_index ) {
    return string_find_rev( str.__pod, c, opt_out_index );
}
attr_always_inline attr_header attr_hot
bool string_find_set( String str, String set, usize* opt_out_index ) {
    return string_find_set( str.__pod, set.__pod, opt_out_index );
}
attr_always_inline attr_header attr_hot
bool string_find_set_rev( String str, String set, usize* opt_out_index ) {
    return string_find_set_rev( str.__pod, set.__pod, opt_out_index );
}
attr_always_inline attr_header attr_hot
usize string_find_set_count( String str, String set ) {
    return string_find_set_count( str.__pod, set.__pod );
}
attr_always_inline attr_header attr_hot
bool string_find_phrase( String str, String phrase, usize* opt_out_index ) {
    return string_find_phrase( str.__pod, phrase.__pod, opt_out_index );
}
attr_always_inline attr_header attr_hot
bool string_find_phrase_rev( String str, String phrase, usize* opt_out_index ) {
    return string_find_phrase_rev( str.__pod, phrase.__pod, opt_out_index );
}
attr_always_inline attr_header attr_hot
usize string_find_phrase_count( String str, String phrase ) {
    return string_find_phrase_count( str.__pod, phrase.__pod );
}
attr_always_inline attr_header attr_hot
String string_truncate( String str, usize max ) {
    return string_truncate( str.__pod, max );
}
attr_always_inline attr_header attr_hot
String string_trim( String str, usize amount ) {
    return string_trim( str.__pod, amount );
}
attr_always_inline attr_header attr_hot
String string_clip( String str, usize from, usize to ) {
    return string_clip( str.__pod, from, to );
}
attr_always_inline attr_header attr_hot
String string_advance( String str, usize amount ) {
    return string_advance_by( str.__pod, amount );
}
attr_always_inline attr_header attr_hot
String string_trim_leading_whitespace( String str ) {
    return string_trim_leading_whitespace( str.__pod );
}
attr_always_inline attr_header attr_hot
String string_trim_trailing_whitespace( String str ) {
    return string_trim_trailing_whitespace( str.__pod );
}
attr_always_inline attr_header attr_hot
String string_trim_surrounding_whitespace( String str ) {
    return string_trim_surrounding_whitespace( str.__pod );
}
attr_always_inline attr_header attr_hot
void string_split(
    String  source,
    usize   at,
    String* opt_out_left,
    String* opt_out_right
) {
    return string_split(
        source.__pod, at, (_StringPOD*)opt_out_left, (_StringPOD*)opt_out_right );
}
attr_always_inline attr_header attr_hot
bool string_split_ascii(
    String  source,
    char    c,
    String* opt_out_left,
    String* opt_out_right
) {
    return string_split_ascii(
        source.__pod, c, (_StringPOD*)opt_out_left, (_StringPOD*)opt_out_right );
}
attr_always_inline attr_header attr_hot
bool string_whitespace(
    String  source,
    String* opt_out_left,
    String* opt_out_right
) {
    return string_split_whitespace(
        source.__pod, (_StringPOD*)opt_out_left, (_StringPOD*)opt_out_right );
}
attr_always_inline attr_header attr_hot
bool string_parse_int( String str, i64* out_int ) {
    return string_parse_int( str.__pod, out_int );
}
attr_always_inline attr_header attr_hot
bool string_parse_uint( String str, u64* out_uint ) {
    return string_parse_uint( str.__pod, out_uint );
}
attr_always_inline attr_header attr_hot
bool string_parse_float( String str, f64* out_float ) {
    return string_parse_float( str.__pod, out_float );
}
attr_always_inline attr_header attr_hot
usize string_stream_to_upper( StreamBytesFN* stream, void* target, String str ) {
    return string_stream_to_upper( stream, target, str.__pod );
}
attr_always_inline attr_header attr_hot
usize string_stream_to_lower( StreamBytesFN* stream, void* target, String str ) {
    return string_stream_to_lower( stream, target, str.__pod );
}
attr_always_inline attr_header attr_hot
bool cmp( String a, String b ) {
    return string_cmp( a, b );
}
attr_always_inline attr_header attr_hot
String operator ""_s( const char* str, usize len ) {
    return String( len, str );
}




constexpr attr_header
_StringBufCPP::_StringBufCPP()
    : cap(0), len(0), cbuf(nullptr) {}
constexpr attr_header
_StringBufCPP::_StringBufCPP( usize cap, const char* buf )
    : cap(cap), len(0), cbuf(buf) {}
constexpr attr_header
_StringBufCPP::_StringBufCPP( usize cap, usize len, const char* buf )
    : cap(cap), len(len), cbuf(buf) {}
constexpr attr_header
_StringBufCPP::_StringBufCPP( const _StringPOD& slice, usize opt_len )
    : cap(slice.len), len(opt_len), cbuf(slice.cbuf) {}
constexpr attr_header
_StringBufCPP::_StringBufCPP( const Slice<char>& slice, usize opt_len )
    : cap(slice.len), len(opt_len), cbuf(slice.cbuf) {}
constexpr attr_header
_StringBufCPP::_StringBufCPP( const Slice<i8>& slice, usize opt_len )
    : cap(slice.len), len(opt_len), cbuf((const char*)slice.cbuf) {}
constexpr attr_header
_StringBufCPP::_StringBufCPP( const Slice<u8>& slice, usize opt_len )
    : cap(slice.len), len(opt_len), cbuf((const char*)slice.cbuf) {}
template<usize ArrayLength> constexpr attr_header
_StringBufCPP::_StringBufCPP( const char (&array)[ArrayLength], usize opt_len )
    : cap(ArrayLength), len(opt_len), cbuf(array) {}
constexpr attr_header attr_always_inline attr_hot
_StringBufCPP::_StringBufCPP( const _StringBufPOD& pod )
    : cap(pod.cap), len(pod.len), cbuf(pod.cbuf) {}

constexpr attr_header
_StringBufCPP::operator _StringBufPOD() const {
    return this->__pod;
}
constexpr attr_header
_StringBufCPP::operator _StringCPP() const {
    return this->slice;
}
constexpr attr_header
_StringBufCPP::operator _StringPOD() const {
    return this->__slice_pod;
}
constexpr attr_header
_StringBufCPP::operator Slice<char>() const {
    return *(Slice<char>*)&this->slice;
}
constexpr attr_header
_StringBufCPP::operator Slice<u8>() const {
    return *(Slice<u8>*)&this->slice;
}
constexpr attr_header
_StringBufCPP::operator Slice<i8>() const {
    return *(Slice<i8>*)&this->slice;
}

constexpr attr_header
char _StringBufCPP::operator[]( usize index ) const {
    return this->cbuf[index];
}
constexpr attr_header
char& _StringBufCPP::operator[]( usize index ) {
    return this->buf[index];
}

attr_header
const char* _StringBufCPP::begin() const {
    return this->cbuf;
}
attr_header
const char* _StringBufCPP::end() const {
    return this->cbuf + this->len;
}
attr_header
char* _StringBufCPP::begin() {
    return this->buf;
}
attr_header
char* _StringBufCPP::end() {
    return this->buf + this->len;
}

attr_always_inline attr_header attr_hot
bool string_buf_from_alloc( AllocatorInterface* allocator, usize size, StringBuf* out_buf ) {
    return string_buf_from_alloc( allocator, size, (_StringBufPOD*)out_buf );
}
attr_always_inline attr_header attr_hot
bool string_buf_from_string_alloc(
    AllocatorInterface* allocator, String src, StringBuf* out_buf
) {
    return string_buf_from_string_alloc( allocator, src.__pod, (_StringBufPOD*)out_buf );
}
attr_always_inline attr_header attr_hot
bool string_buf_grow( AllocatorInterface* allocator, StringBuf* buf, usize amount ) {
    return string_buf_grow( allocator, (_StringBufPOD*)buf, amount );
}
attr_always_inline attr_header attr_hot
void string_buf_free( AllocatorInterface* allocator, StringBuf* buf ) {
    string_buf_free( allocator, (_StringBufPOD*)buf );
}
attr_always_inline attr_header attr_hot
usize string_buf_remaining( const StringBuf& buf ) {
    return string_buf_remaining( buf.__pod );
}
attr_always_inline attr_header attr_hot
bool string_buf_is_empty( const StringBuf& buf ) {
    return string_buf_is_empty( buf.__pod );
}
attr_always_inline attr_header attr_hot
bool string_buf_is_full( const StringBuf& buf ) {
    return string_buf_is_full( buf.__pod );
}
attr_always_inline attr_header attr_hot
void string_buf_clear( StringBuf* buf ) {
    string_buf_clear( (_StringBufPOD*)buf );
}
attr_always_inline attr_header attr_hot
bool string_buf_clone( AllocatorInterface* allocator, StringBuf* dst, String src ) {
    return string_buf_clone( allocator, (_StringBufPOD*)dst, src.__pod );
}
attr_always_inline attr_header attr_hot
bool string_buf_try_push( StringBuf* buf, char c ) {
    return string_buf_try_push( (_StringBufPOD*)buf, c );
}
attr_always_inline attr_header attr_hot
bool string_buf_push( AllocatorInterface* allocator, StringBuf* buf, char c ) {
    return string_buf_push( allocator, (_StringBufPOD*)buf, c );
}
attr_always_inline attr_header attr_hot
bool string_buf_try_emplace( StringBuf* buf, char c, usize at ) {
    return string_buf_try_emplace( (_StringBufPOD*)buf, c, at );
}
attr_always_inline attr_header attr_hot
bool string_buf_emplace( AllocatorInterface* allocator, StringBuf* buf, char c, usize at ) {
    return string_buf_emplace( allocator, (_StringBufPOD*)buf, c, at );
}
attr_always_inline attr_header attr_hot
bool string_buf_pop( StringBuf* buf, char* opt_out_c ) {
    return string_buf_pop( (_StringBufPOD*)buf, opt_out_c );
}
attr_always_inline attr_header attr_hot
bool string_buf_try_insert( StringBuf* buf, String insert, usize at ) {
    return string_buf_try_insert( (_StringBufPOD*)buf, insert.__pod, at );
}
attr_always_inline attr_header attr_hot
bool string_buf_try_prepend( StringBuf* buf, String prepend ) {
    return string_buf_try_prepend( (_StringBufPOD*)buf, prepend.__pod );
}
attr_always_inline attr_header attr_hot
bool string_buf_try_append( StringBuf* buf, String append ) {
    return string_buf_try_append( (_StringBufPOD*)buf, append );
}
attr_always_inline attr_header attr_hot
bool string_buf_insert(
    AllocatorInterface* allocator, StringBuf* buf, String insert, usize at
) {
    return string_buf_insert( allocator, (_StringBufPOD*)buf, insert.__pod, at );
}
attr_always_inline attr_header attr_hot
bool string_buf_prepend(
    AllocatorInterface* allocator, StringBuf* buf, String prepend
) {
    return string_buf_prepend( allocator, (_StringBufPOD*)buf, prepend.__pod );
}
attr_always_inline attr_header attr_hot
bool string_buf_append(
    AllocatorInterface* allocator, StringBuf* buf, String append
) {
    return string_buf_append( allocator, (_StringBufPOD*)buf, append.__pod );
}
attr_always_inline attr_header attr_hot
void string_buf_remove( StringBuf* buf, usize at ) {
    string_buf_remove( (_StringBufPOD*)buf, at );
}
attr_always_inline attr_header attr_hot
void string_buf_remove_range( StringBuf* buf, usize from, usize to ) {
    string_buf_remove_range( (_StringBufPOD*)buf, from, to );
}
attr_always_inline attr_header attr_hot
usize string_buf_try_fmt_va( StringBuf* buf, String format, va_list va ) {
    return string_buf_try_fmt_va( (_StringBufPOD*)buf, format.__pod, va );
}
attr_always_inline attr_header attr_hot
usize string_buf_try_fmt( StringBuf* buf, String format, ... ) {
    va_list va;
    va_start( va, format );
    usize result = string_buf_try_fmt_va( buf, format, va );
    va_end( va );
    return result;
}
attr_always_inline attr_header attr_hot
bsize string_buf_fmt_va(
    AllocatorInterface* allocator,
    StringBuf*          buf,
    String              format,
    va_list             va
) {
    return string_buf_fmt_va( allocator, (_StringBufPOD*)buf, format.__pod, va );
}
attr_always_inline attr_header attr_hot
bsize string_buf_fmt(
    AllocatorInterface* allocator,
    StringBuf*          buf,
    String              format,
    ...
) {
    va_list va;
    va_start( va, format );
    bsize result = string_buf_fmt_va( allocator, buf, format, va );
    va_end( va );
    return result;
}

#endif /* header guard */
