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

attr_always_inline attr_header attr_optimized constexpr
usize __strlen( const char* cstr ) {
    usize result = 0;
    while( *cstr++ ) {
        result++;
    }
    return result;
}

struct _StringCPP {
    typedef char Type;

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

    /// @brief Create empty string.
    attr_always_inline attr_header attr_optimized constexpr
    _StringCPP() : len(0), buf(nullptr) {}

    /// @brief Create string.
    /// @param     len Byte length of string.
    /// @param[in] buf Pointer to start of string.
    attr_always_inline attr_header attr_optimized constexpr
    _StringCPP( usize len, const Type* buf ) : len(len), cbuf(buf) {}

    /// @brief Create string from static character array.
    /// @param[in] array Static array.
    template<usize ArrayLength> attr_always_inline attr_header constexpr
    _StringCPP( const Type (&array)[ArrayLength] ) : _StringCPP( ArrayLength, array ) {}

    /// @brief Create string from string literal.
    /// @param[in] cstr String literal.
    attr_always_inline attr_header attr_optimized constexpr
    _StringCPP( const char* cstr ) : _StringCPP( __strlen( cstr ), cstr ) {}

    /// @brief Implicitly convert POD string.
    /// @param[in] pod POD string.
    attr_always_inline attr_header attr_optimized constexpr
    _StringCPP( const _StringPOD& pod ) : _StringCPP( pod.len, pod.cbuf ) {}

    attr_always_inline attr_header attr_optimized constexpr
    _StringCPP( const Slice<char>& slice ) : _StringCPP( slice.len, (char*)slice.cbuf ) {}

    attr_always_inline attr_header attr_optimized constexpr
    _StringCPP( const Slice<i8>& slice ) : _StringCPP( slice.len, (char*)slice.cbuf ) {}

    attr_always_inline attr_header attr_optimized constexpr
    _StringCPP( const Slice<u8>& slice ) : _StringCPP( slice.len, (char*)slice.cbuf ) {}

    /// @brief Implicitly convert to POD string.
    /// @return POD string.
    attr_always_inline attr_header attr_optimized constexpr
    operator _StringPOD() const {
        return { .len=len, .cbuf=cbuf };
    }

    attr_always_inline attr_header attr_optimized constexpr
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

struct _StringBufCPP {
    typedef char Type;
    
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
        _StringCPP  slice;
        /// @brief String buffer as a slice.
        Slice<char> _slice;
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

    attr_always_inline attr_header attr_optimized constexpr
    _StringBufCPP( const Buffer<char>& buf ) : _StringBufCPP( buf.cap, buf.len, buf.cbuf ) {}

    attr_always_inline attr_header attr_optimized constexpr
    _StringBufCPP( const Buffer<u8>& buf ) : _StringBufCPP( buf.cap, buf.len, (char*)buf.cbuf ) {}

    attr_always_inline attr_header attr_optimized constexpr
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
        return _slice;
    }

    attr_always_inline attr_header attr_optimized constexpr
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

#endif /* header guard */
