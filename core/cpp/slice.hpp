#if !defined(CORE_CPP_SLICE_HPP)
#define CORE_CPP_SLICE_HPP
/**
 * @file   slice.hpp
 * @brief  Slice.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   January 01, 2025
*/
#include "core/types.h"
#include "core/attributes.h"

// NOTE(alicia): forward declaration

attr_core_api void memory_copy(
    void* attr_restrict dst, const void* attr_restrict src, usize size );
attr_core_api void memory_move(
    void* dst, const void* src, usize size );

#define GEN_RANGE_FOR( Type ) \
    attr_header const Type* begin() const { \
        return cbuf; \
    } \
    attr_header const Type* end() const { \
        return cbuf + len; \
    } \
    attr_header Type* begin() { \
        return buf; \
    } \
    attr_header Type* end() { \
        return buf + len; \
    }

#define GEN_SUBSCRIPT( Type ) \
    attr_header const Type& operator[]( usize idx ) const { \
        return cbuf[idx]; \
    } \
    attr_header Type& operator[]( usize idx ) { \
        return buf[idx]; \
    }

/// @brief Slice.
template<typename T>
struct Slice {
    typedef T Type;

    /// @brief Length of slice.
    usize len = 0;
    union {
        /// @brief Const pointer to slice buffer.
        const Type* cbuf = nullptr;
        /// @brief Pointer to slice buffer.
        Type*       buf;
        /// @brief Pointer to slice bytes.
        u8*         bytes;
        /// @brief Void pointer to slice buffer.
        void*       _void;
    };

    /// @brief Create empty slice.
    attr_header constexpr
    Slice() : len(0), cbuf(nullptr) {}
    /// @brief Create slice.
    /// @param     len Length of slice.
    /// @param[in] buf Pointer to start of slice.
    attr_header constexpr
    Slice( usize len, const Type* buf ) : len(len), cbuf(buf) {}
    /// @brief Create slice.
    /// @param array Static array.
    template<usize ArrayLength> attr_header constexpr
    Slice( const Type (&array)[ArrayLength] ) : Slice( ArrayLength, array ) {}

    GEN_RANGE_FOR( Type );
    GEN_SUBSCRIPT( Type );
};
/// @brief Buffer.
template<typename T>
struct Buffer {
    typedef T Type;
    
    /// @brief Capacity of buffer.
    usize cap = 0;
    union {
        struct {
            /// @brief Length of buffer.
            usize len = 0;
            union {
                /// @brief Const pointer to buffer.
                const Type* cbuf = nullptr;
                /// @brief Pointer to buffer.
                Type*       buf;
                /// @brief Pointer to bytes.
                u8*         bytes;
                /// @brief Void pointer to buffer.
                void*       _void;
            };
        };
        /// @brief Buffer as a slice.
        Slice<T> slice;
    };

    /// @brief Create an empty buffer.
    attr_header constexpr
    Buffer() : cap(0), len(0), cbuf(nullptr) {}
    /// @brief Create a buffer.
    /// @param     cap Capacity of buffer.
    /// @param     len Length of buffer.
    /// @param[in] buf Pointer to start of buffer.
    attr_header constexpr
    Buffer( usize cap, usize len, const Type* buf ) :
        cap(cap), len(len), cbuf(buf) {}
    /// @brief Create a buffer.
    /// @param     len Length of buffer.
    /// @param[in] buf Pointer to start of buffer.
    attr_header constexpr
    Buffer( usize cap, const Type* buf ) : Buffer( cap, 0, buf ) {}
    /// @brief Create a buffer from a slice.
    /// @param slice Slice.
    template<typename SliceLike> attr_header constexpr
    explicit Buffer( const SliceLike& slice ) : Buffer( slice.len, slice.cbuf ) {}
    /// @brief Create a buffer from an array.
    /// @param array Static array.
    template<usize ArrayLength> attr_header constexpr
    Buffer( const Type (&array)[ArrayLength] ) : Buffer( ArrayLength, array ) {}

    /// @brief Implicitly convert to slice.
    attr_header constexpr
    operator Slice<Type>() const {
        return slice;
    }

    GEN_RANGE_FOR( Type );
    GEN_SUBSCRIPT( Type );
};

/// @brief Check if slice is empty.
/// @param slice Slice to check.
/// @return
///     - @c true  : Slice is empty.
///     - @c false : Slice is not empty.
template<typename SliceLike> attr_header
bool slice_is_empty( const SliceLike& slice ) {
    return slice.len == 0;
}
/// @brief Truncate slice.
/// @param slice Slice to truncate.
/// @param max   Maximum length of slice.
/// @return Truncated slice.
template<typename SliceLike> attr_header
SliceLike slice_truncate( const SliceLike& slice, usize max ) {
    SliceLike res = slice;
    if( max >= res.len ) {
        return res;
    }
    res.len = max;
    return res;
}
/// @brief Trim slice length.
/// @param slice  Slice to trim.
/// @param amount (optional, default = 1) Number of items to trim from length.
/// @return Trimmed slice.
template<typename SliceLike> attr_header
SliceLike slice_trim( const SliceLike& slice, usize amount = 1 ) {
    return slice_truncate( slice, amount >= slice.len ? 0 : slice.len - amount );
}
/// @brief Clip slice out of slice.
/// @param slice          Slice to clip from.
/// @param from_inclusive Start of range to clip.
/// @param to_exclusive   End of range to clip.
/// @return Clipped slice.
template<typename SliceLike> attr_header
SliceLike slice_clip(
    const SliceLike& slice, usize from_inclusive, usize to_exclusive 
) {
    SliceLike res = slice;
    res.cbuf = res.cbuf + from_inclusive;
    res.len  = to_exclusive - from_inclusive;
    return res;
}
/// @brief Advance slice.
/// @param slice  Slice to advance.
/// @param amount (optional, default = 1) Number of items to advance by.
/// @return Slice.
template<typename SliceLike> attr_header
SliceLike slice_advance( const SliceLike& slice, usize amount = 1 ) {
    SliceLike res = slice;
    if( amount >= res.len ) {
        res.cbuf = res.cbuf + res.len;
        res.len  = 0;
        return res;
    } else {
        res.cbuf = res.cbuf + amount;
        res.len -= amount;
        return res;
    }
}
/// @brief Split slice.
/// @param      slice         Slice to create splits from.
/// @param      at            Index to split at. Item at this index is not included in splits.
/// @param[out] opt_out_left  (optional) Pointer to write left side of split to.
/// @param[out] opt_out_right (optional) Pointer to write right side of split to.
template<typename SliceLike> attr_header
void slice_split(
    const SliceLike& slice, usize at,
    SliceLike* opt_out_left, SliceLike* opt_out_right
) {
    if( opt_out_left ) {
        *opt_out_left = slice_truncate( slice, at );
    }
    if( opt_out_right ) {
        *opt_out_right = slice_advance( slice, at + 1 );
    }
}

/// @brief Check if buffer is empty.
/// @param buffer Buffer to check.
/// @return
///     - @c true  : Buffer is empty.
///     - @c false : Buffer is not empty.
template<typename BufferLike> attr_header
bool buffer_is_empty( const BufferLike& buffer ) {
    return buffer.len == 0;
}
/// @brief Check if buffer is full.
/// @param buffer Buffer to check.
/// @return
///     - @c true  : Buffer is full.
///     - @c false : Buffer is not full.
template<typename BufferLike> attr_header
bool buffer_is_full( const BufferLike& buffer ) {
    return buffer.cap == buffer.len;
}
/// @brief Calculate how many items can still fit in buffer.
/// @param buffer Buffer.
/// @return Number of items that can fit in buffer.
template<typename BufferLike> attr_header
usize buffer_remaining( const BufferLike& buffer ) {
    return buffer.cap - buffer.len;
}
/// @brief Set length of buffer to zero.
/// @param buffer Buffer to clear.
template<typename BufferLike> attr_header
void buffer_clear( BufferLike& buffer ) {
    buffer.len = 0;
}
/// @brief Try to push item to end of buffer.
/// @param buffer Buffer to push to.
/// @param item   Item to push.
/// @return
///     - @c true  : @c buffer had enough capacity to push item.
///     - @c false : @c buffer is full. 
template<typename BufferLike> attr_header
bool buffer_try_push( BufferLike& buffer, const typename BufferLike::Type& item ) {
    if( buffer_is_full(buffer) ) {
        return false;
    }
    buffer[buffer.len++] = item;
    return true;
}
/// @brief Try to pop item from end of buffer.
/// @param      buffer       Buffer to pop item from.
/// @param[out] opt_out_item (optional) Pointer to write popped item to.
/// @return
///     - @c true  : @c buffer popped item.
///     - @c false : @c buffer is empty. 
template<typename BufferLike> attr_header
bool buffer_pop( BufferLike& buffer, typename BufferLike::Type* opt_out_item = nullptr ) {
    if( buffer_is_empty( buffer ) ) {
        return false;
    }
    buffer.len--;
    if( opt_out_item ) {
        *opt_out_item = buffer[buffer.len];
    }
    return true;
}
/// @brief Try to place item at given index in buffer.
/// @param buffer Buffer to emplace in.
/// @param item   Item to emplace.
/// @param at     Index to emplace item at.
/// @return
///     - @c true  : @c buffer had enough capacity to emplace item.
///     - @c false : @c buffer is full.
template<typename BufferLike> attr_header
bool buffer_try_emplace(
    BufferLike& buffer, const typename BufferLike::Type& item, usize at
) {
    using BT = typename BufferLike::Type;
    if( buffer_is_full( buffer ) ) {
        return false;
    }
    usize move = buffer.len - at;
    memory_move( buffer.buf + at + 1, buffer.buf + at, sizeof(BT) * move );
    buffer[at] = item;
    buffer.len++;
    return true;
}
/// @brief Try to insert slice at given index in buffer.
/// @param buffer Buffer to insert in.
/// @param slice  Slice to insert.
/// @param at     Index to insert at.
/// @return
///     - @c true  : @c buffer had enough capacity to insert slice.
///     - @c false : @c buffer is full.
template<typename BufferLike, typename SliceLike> attr_header
bool buffer_try_insert( BufferLike& buffer, const SliceLike& slice, usize at ) {
    using BT = typename BufferLike::Type;
    using ST = typename SliceLike::Type;
    static_assert( sizeof(BT) == sizeof(ST) );

    if( (buffer.len + slice.len) > (buffer.cap) ) {
        return false;
    }

    usize move = buffer.len ? (buffer.len - at) : 0;
    memory_move( buffer.buf + at + slice.len, buffer.buf + at, sizeof(BT) * move );
    memory_copy( buffer.buf + at, slice.cbuf, sizeof(ST) * slice.len );
    buffer.len += slice.len;
    return true;
}
/// @brief Try to prepend slice to beginning of buffer.
/// @param buffer Buffer to prepend to.
/// @param slice  Slice to prepend.
/// @return
///     - @c true  : @c buffer had enough capacity to prepend slice.
///     - @c false : @c buffer is full.
template<typename BufferLike, typename SliceLike> attr_header
bool buffer_try_prepend( BufferLike& buffer, const SliceLike& slice ) {
    return buffer_try_insert( buffer, slice, 0 );
}
/// @brief Try to append slice to end of buffer.
/// @param buffer Buffer to append to.
/// @param slice  Slice to append.
/// @return
///     - @c true  : @c buffer had enough capacity to append slice.
///     - @c false : @c buffer is full.
template<typename BufferLike, typename SliceLike> attr_header
bool buffer_try_append( BufferLike& buffer, const SliceLike& slice ) {
    return buffer_try_insert( buffer, slice, buffer.len );
}
/// @brief Remove item from buffer.
/// @param buffer Buffer to remove item from.
/// @param at     Index of item to remove.
template<typename BufferLike> attr_header
void buffer_remove( BufferLike& buffer, usize at ) {
    using BT = typename BufferLike::Type;

    usize move = buffer.len - at;
    memory_move( buffer.buf + at, buffer.buf + at + 1, sizeof(BT) * move );
    buffer.len--;
}
/// @brief Remove item from buffer.
/// @details
///     Swaps last item in buffer instead of moving buffer to fill space.
/// @param buffer Buffer to remove item from.
/// @param at     Index of item to remove.
template<typename BufferLike> attr_header
void buffer_swap_remove( BufferLike& buffer, usize at ) {
    buffer[at] = buffer[--buffer.len];
}
/// @brief Remove range of items from buffer.
/// @param buffer         Buffer to remove items from.
/// @param from_inclusive Start of range to remove.
/// @param to_exclusive   End of range to remove.
template<typename BufferLike> attr_header
void buffer_remove_range( BufferLike& buffer, usize from_inclusive, usize to_exclusive ) {
    using BT = typename BufferLike::Type;

    usize span = to_exclusive - from_inclusive;
    memory_move(
        buffer.buf + from_inclusive,
        buffer.buf + to_exclusive,
        sizeof(BT) * (buffer.len - span) );
    buffer.len -= span;
}

#endif /* header guard */
