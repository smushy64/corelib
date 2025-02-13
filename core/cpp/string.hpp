#if !defined(CORE_CPP_STRING_HPP)
#define CORE_CPP_STRING_HPP
/**
 * @file   string.hpp
 * @brief  C++ String.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   January 01, 2025
*/
#include "core/cpp/slice.hpp"

struct _StringCPP;
struct _StringBufCPP;

#if !defined(CORE_STRING_H)
    #include "core/string.h"
#endif

attr_always_inline attr_header attr_hot constexpr
usize __strlen( const char* cstr ) {
    usize result = 0;
    while( *cstr++ ) {
        result++;
    }
    return result;
}

struct _StringCPP {
    typedef char Type;

    union {
        struct {
            /// @brief Byte length of string.
            usize len = 0;
            union {
                /// @brief Pointer to string buffer.
                const Type* cbuf = nullptr;
                /// @brief Constant pointer to string buffer.
                Type*       buf;
                /// @brief Pointer to string bytes.
                u8*         bytes;
                /// @brief Void pointer to string buffer.
                void*       _void;
            };
        };
        _StringPOD pod;
    };

    /// @brief Create empty string.
    attr_always_inline attr_header attr_hot constexpr
    _StringCPP() : len(0), buf(nullptr) {}

    /// @brief Create string.
    /// @param     len Byte length of string.
    /// @param[in] buf Pointer to start of string.
    attr_always_inline attr_header attr_hot constexpr
    _StringCPP( usize len, const Type* buf ) : len(len), cbuf(buf) {}

    /// @brief Create string from static character array.
    /// @param[in] array Static array.
    template<usize ArrayLength> attr_always_inline attr_header constexpr
    _StringCPP( const Type (&array)[ArrayLength] ) : _StringCPP( ArrayLength, array ) {}

    /// @brief Create string from string literal.
    /// @param[in] cstr String literal.
    attr_always_inline attr_header attr_hot constexpr
    _StringCPP( const char* cstr ) : _StringCPP( __strlen( cstr ), cstr ) {}

    /// @brief Implicitly convert POD string.
    /// @param[in] pod POD string.
    attr_always_inline attr_header attr_hot constexpr
    _StringCPP( const _StringPOD& pod ) : _StringCPP( pod.len, pod.cbuf ) {}

    attr_always_inline attr_header attr_hot constexpr
    _StringCPP( const Slice<char>& slice ) : _StringCPP( slice.len, (char*)slice.cbuf ) {}

    attr_always_inline attr_header attr_hot constexpr
    _StringCPP( const Slice<i8>& slice ) : _StringCPP( slice.len, (char*)slice.cbuf ) {}

    attr_always_inline attr_header attr_hot constexpr
    _StringCPP( const Slice<u8>& slice ) : _StringCPP( slice.len, (char*)slice.cbuf ) {}

    /// @brief Implicitly convert to POD string.
    /// @return POD string.
    attr_always_inline attr_header attr_hot constexpr
    operator _StringPOD() const {
        return { .len=len, .cbuf=cbuf };
    }

    attr_always_inline attr_header attr_hot constexpr
    operator Slice<char>() const {
        return Slice( len, cbuf );
    }

    GEN_RANGE_FOR( Type );
    GEN_SUBSCRIPT( Type );
};
attr_header
bool operator==( const _StringPOD& a, const _StringPOD& b ) {
    return string_cmp( a, b );
}
attr_header
bool operator!=( const _StringPOD& a, const _StringPOD& b ) {
    return !( a == b );
}
attr_header
bool operator==( const _StringCPP& a, const _StringCPP& b ) {
    return string_cmp( a.pod, b.pod );
}
attr_header
bool operator!=( const _StringCPP& a, const _StringCPP& b ) {
    return !( a == b );
}

struct _StringBufCPP {
    typedef char Type;
    
    union {
        struct {
            /// @brief Byte capacity of string buffer.
            usize cap = 0;
            union {
                struct {
                    /// @brief Byte length of string.
                    usize len = 0;
                    union {
                        /// @brief Constant pointer to string buffer.
                        const Type* cbuf = nullptr;
                        /// @brief Pointer to string buffer.
                        Type*       buf;
                        /// @brief Pointer to string bytes.
                        u8*         bytes;
                        /// @brief Void pointer to string buffer.
                        void*       _void;
                    };
                };
                /// @brief String buffer as a slice.
                _StringCPP  string;
                /// @brief String buffer as a slice.
                Slice<char> slice;

                _StringPOD  string_pod;
            };
        };
        _StringBufPOD pod;
    };

    /// @brief Create empty string buffer.
    attr_header constexpr
    _StringBufCPP() {}

    /// @brief Create string buffer.
    /// @param     cap Byte capacity of string buffer.
    /// @param     len Byte length of string buffer.
    /// @param[in] buf Pointer to start of string buffer.
    attr_header constexpr
    _StringBufCPP( usize cap, usize len, const Type* buf ) :
        cap(cap), len(len), cbuf(buf) {}

    /// @brief Create string buffer.
    /// @param     cap Byte capacity of string buffer.
    /// @param[in] buf Pointer to start of string buffer.
    attr_header constexpr
    _StringBufCPP( usize cap, const Type* buf ) : _StringBufCPP( cap, 0, buf ) {}

    /// @brief Create string buffer.
    /// @param slice String slice.
    template<typename SliceLike> attr_header constexpr
    explicit _StringBufCPP( const SliceLike& slice ) : _StringBufCPP( slice.len, slice.cbuf ) {}

    /// @brief Create string buffer.
    /// @param array Static character array.
    template<usize ArrayLength> attr_header constexpr
    _StringBufCPP( const Type (&array)[ArrayLength] ) : _StringBufCPP( ArrayLength, array ) {}

    /// @brief Implicitly convert from POD string buffer.
    /// @param pod String buffer POD.
    attr_header constexpr
    _StringBufCPP( const _StringBufPOD& pod ) : _StringBufCPP( pod.cap, pod.len, pod.cbuf ) {}

    attr_always_inline attr_header attr_hot constexpr
    _StringBufCPP( const Buffer<char>& buf ) : _StringBufCPP( buf.cap, buf.len, buf.cbuf ) {}

    attr_always_inline attr_header attr_hot constexpr
    _StringBufCPP( const Buffer<u8>& buf ) : _StringBufCPP( buf.cap, buf.len, (char*)buf.cbuf ) {}

    attr_always_inline attr_header attr_hot constexpr
    _StringBufCPP( const Buffer<i8>& buf ) : _StringBufCPP( buf.cap, buf.len, (char*)buf.cbuf ) {}

    /// @brief Implicitly convert to POD string buffer.
    /// @return String buffer POD.
    attr_header constexpr
    operator _StringBufPOD() const {
        return { .cap=cap, .len=len, .cbuf=cbuf };
    }

    /// @brief Implicitly convert to POD string slice.
    /// @return String slice POD.
    attr_header constexpr
    operator _StringPOD() const {
        return { .len=len, .cbuf=cbuf };
    }

    /// @brief Implicitly convert to string slice.
    /// @return String slice.
    attr_header constexpr
    operator _StringCPP() const {
        return _StringCPP( len, cbuf );
    }

    /// @brief Implicitly convert to slice.
    /// @return Slice.
    attr_header constexpr
    operator Slice<Type>() const {
        return slice;
    }

    attr_always_inline attr_header attr_hot constexpr
    operator Buffer<char>() const {
        return Buffer( cap, len, cbuf );
    }

    GEN_RANGE_FOR( Type );
    GEN_SUBSCRIPT( Type );
};
attr_header
bool operator==( const _StringBufPOD& a, const _StringBufPOD& b ) {
    return string_cmp( a.slice, b.slice );
}
attr_header
bool operator!=( const _StringBufPOD& a, const _StringBufPOD& b ) {
    return !( a == b );
}
attr_header
bool operator==( const _StringBufCPP& a, const _StringBufCPP& b ) {
    return string_cmp( a.string_pod, b.string_pod );
}
attr_header
bool operator!=( const _StringBufCPP& a, const _StringBufCPP& b ) {
    return !( a == b );
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
    struct _StringCPP source, usize at,
    struct _StringCPP* opt_out_left, struct _StringCPP* opt_out_right
) {
    return string_split(
        source.pod, at, (_StringPOD*)opt_out_left, (_StringPOD*)opt_out_right );
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
    struct _StringCPP source, char c,
    struct _StringCPP* opt_out_left, struct _StringCPP* opt_out_right
) {
    return string_split_ascii(
        source.pod, c,
        (_StringPOD*)opt_out_left, (_StringPOD*)opt_out_right );
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
    struct _StringCPP source,
    struct _StringCPP* opt_out_left,
    struct _StringCPP* opt_out_right
) {
    return string_split_whitespace(
        source.pod,
        (_StringPOD*)opt_out_left,
        (_StringPOD*)opt_out_right );
}

/// @brief Calculate remaining space in string buffer.
/// @param[in] buf String buffer.
/// @return Number of bytes (characters) remaining in string.
attr_always_inline attr_header
usize string_buf_remaining( const struct _StringBufCPP* buf ) {
    return string_buf_remaining( (_StringBufPOD*)buf );
}
/// @brief Check if string buffer is empty.
/// @param[in] buf Pointer to string buffer.
/// @return
///     - @c true  : String buffer is empty.
///     - @c false : String buffer is not empty.
attr_always_inline attr_header
b32 string_buf_is_empty( const struct _StringBufCPP* buf ) {
    return string_buf_is_empty( (_StringBufPOD*)buf );
}
/// @brief Check if string buffer is full.
/// @note String buffer always tries to have space for null terminator.
/// @param[in] buf Pointer to string buffer.
/// @return
///     - @c true  : String buffer is full.
///     - @c false : String buffer still has space.
attr_always_inline attr_header
b32 string_buf_is_full( const struct _StringBufCPP* buf ) {
    return string_buf_is_full( (_StringBufPOD*)buf );
}
/// @brief Set string buffer length to zero and zero out memory.
/// @param[in] buf Pointer to string buffer.
attr_always_inline attr_header
void string_buf_clear( struct _StringBufCPP* buf ) {
    string_buf_clear( (_StringBufPOD*)buf );
}
/// @brief Attempt to push character to end of string buffer.
/// @param[in] buf String buffer to push character to.
/// @param     c   Character to push.
/// @return
///     - @c true  : @c buf had enough space to push character.
///     - @c false : @c buf is full.
attr_always_inline attr_header
b32 string_buf_try_push( struct _StringBufCPP* buf, char c ) {
    return string_buf_try_push( (_StringBufPOD*)buf, c );
}
/// @brief Attempt to emplace a character inside of string buffer.
/// @param[in] buf String buffer to emplace character in.
/// @param     c   Character to emplace.
/// @param     at  Index to emplace character at.
/// @return
///     - @c true  : @c buf had enough space to emplace character.
///     - @c false : @c buf is full.
attr_always_inline attr_header
b32 string_buf_try_emplace( struct _StringBufCPP* buf, char c, usize at ) {
    return string_buf_try_emplace( (_StringBufPOD*)buf, c, at );
}
/// @brief Pop character from end of string buffer.
/// @param[in]  buf       String buffer to pop from.
/// @param[out] opt_out_c (optional) Pointer to write popped character to.
/// @return
///     - @c true  : @c buf had character to pop.
///     - @c false : @c buf was empty.
attr_always_inline attr_header
b32 string_buf_pop( struct _StringBufCPP* buf, char* opt_out_c ) {
    return string_buf_pop( (_StringBufPOD*)buf, opt_out_c );
}
/// @brief Attempt to insert string in string buffer.
/// @param[in] buf    String buffer to insert in.
/// @param     insert String to insert.
/// @param     at     Index to insert at. Must be 0 to @c buf.len
/// @return
///     - @c true  : @c buf had enough capacity to insert string.
///     - @c false : @c buf did not have enough capacity to insert.
attr_always_inline attr_header
b32 string_buf_try_insert(
    struct _StringBufCPP* buf, struct _StringPOD insert, usize at
) {
    return string_buf_try_insert( (_StringBufPOD*)buf, insert, at );
}
/// @brief Attempt to prepend string in string buffer.
/// @param[in] buf     String buffer to insert in.
/// @param     prepend String to prepend.
/// @return
///     - @c true  : @c buf had enough capacity to insert string.
///     - @c false : @c buf did not have enough capacity to insert.
attr_always_inline attr_header
b32 string_buf_try_prepend( struct _StringBufCPP* buf, struct _StringPOD prepend ) {
    return string_buf_try_prepend( (_StringBufPOD*)buf, prepend );
}
/// @brief Attempt to append string in string buffer.
/// @param[in] buf    String buffer to insert in.
/// @param     append String to append.
/// @return
///     - @c true  : @c buf had enough capacity to insert string.
///     - @c false : @c buf did not have enough capacity to insert.
attr_always_inline attr_header
b32 string_buf_try_append( struct _StringBufCPP* buf, struct _StringPOD append ) {
    return string_buf_try_append( (_StringBufPOD*)buf, append );
}
/// @brief Remove character from string buffer.
/// @param[in] buf Buffer to remove character from.
/// @param     at  Index of character to remove. Must be in bounds.
attr_always_inline attr_header
void string_buf_remove( struct _StringBufCPP* buf, usize at ) {
    string_buf_remove( (_StringBufPOD*)buf, at );
}
/// @brief Remove range of characters from string buffer.
/// @param[in] buf            Buffer to remove characters from.
/// @param     from_inclusive Start of range to remove.
/// @param     to_exclusive   End of range to remove.
attr_always_inline attr_header
void string_buf_remove_range(
    struct _StringBufCPP* buf, usize from_inclusive, usize to_exclusive
) {
    string_buf_remove_range( (_StringBufPOD*)buf, from_inclusive, to_exclusive );
}
/// @brief Attempt to write a formatted string to string buffer.
/// @param[in] buf        Buffer to write to.
/// @param     format_len Length of format string.
/// @param     format     Format string.
/// @param     va         Variadic format arguments.
/// @return
///     - Zero   : @c buf had enough capacity to write format string.
///     - > Zero : Number of characters not written to @c buf.
attr_always_inline attr_header
usize string_buf_try_fmt_buffer_va(
    struct _StringBufCPP* buf, usize format_len, const char* format, va_list va
) {
    return string_buf_try_fmt_buffer_va( (_StringBufPOD*)buf, format_len, format, va );
}
/// @brief Attempt to write a formatted string to string buffer.
/// @param[in] buf        Buffer to write to.
/// @param     format_len Length of format string.
/// @param     format     Format string.
/// @param     ...        Format arguments.
/// @return
///     - Zero   : @c buf had enough capacity to write format string.
///     - > Zero : Number of characters not written to @c buf.
attr_always_inline attr_header
usize string_buf_try_fmt_buffer(
    struct _StringBufCPP* buf, usize format_len, const char* format, ...
) {
    va_list va;
    va_start( va, format );
    usize result = string_buf_try_fmt_buffer_va( (_StringBufPOD*)buf, format_len, format, va );
    va_end( va );
    return result;
}

#endif /* header guard */
