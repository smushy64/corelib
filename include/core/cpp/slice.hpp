#if !defined(CORE_CPP_SLICE_HPP) && defined(__cplusplus)
#define CORE_CPP_SLICE_HPP
/**
 * @file   slice.hpp
 * @brief  C++ Slices.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 31, 2025
*/
#include "core/types.h"
#include "core/attributes.h"

/// @brief Slice of items.
/// @tparam T Type of items in slice.
template<typename T>
struct Slice;

/// @brief Buffer container.
/// @tparam T Type of items in buffer.
template<typename T>
struct Buffer;

/// @brief Check if slice is empty.
/// @param[in] slice Slice to check.
/// @return
///     - @c true  : Slice is empty.
///     - @c false : Slice is not empty.
template<typename SliceLike> attr_header
bool is_empty( const SliceLike& slice );

/// @brief Truncate length of slice.
/// @param[in] slice Slice to truncate.
/// @param     max   Maximum number of items result should have.
/// @return Truncated slice.
template<typename SliceLike> attr_header 
SliceLike truncate( const SliceLike& slice, usize max );

/// @brief Trim length of slice.
/// @param[in] slice  Slice to trim.
/// @param     amount Number of items to trim.
/// @return Trimmed slice.
template<typename SliceLike> attr_header
SliceLike trim( const SliceLike& slice, usize amount = 1 );

/// @brief Clip slice from slice.
/// @param[in] slice    Slice to clip from.
/// @param     from, to Range to clip from. [from,to)
/// @return Clippped slice.
template<typename SliceLike> attr_header
SliceLike clip( const SliceLike& slice, usize from, usize to );

/// @brief Advance slice by one or more items.
/// @param[in] slice  Slice to advance.
/// @param     amount Number of items to advance by.
/// @return Advanced slice.
template<typename SliceLike> attr_header
SliceLike advance( const SliceLike& slice, usize amount = 1 );

/// @brief Split slice into two slices.
/// @param[in]  slice         Slice to split.
/// @param      at            Index to split at.
/// @param[out] opt_out_left  (optional) Pointer to write left side of split.
/// @param[out] opt_out_right (optional) Pointer to write right side of split.
template<typename SliceLike> attr_header
void split(
    const SliceLike& slice,
    usize            at,
    SliceLike*       opt_out_left  = nullptr,
    SliceLike*       opt_out_right = nullptr );

/// @brief Check if buffer is full.
/// @param[in] buffer Buffer to check.
/// @return
///     - @c true  : Buffer is full.
///     - @c false : Buffer can still hold more items.
template<typename BufferLike> attr_header
bool is_full( const BufferLike& buffer );

/// @brief Calculate how many more items buffer can hold.
/// @param[in] buffer Buffer.
/// @return Number of items buffer can hold.
template<typename BufferLike> attr_header
usize remaining( const BufferLike& buffer );

/// @brief Reset buffer.
/// @param[in] buffer Buffer to reset.
template<typename BufferLike> attr_header
void reset( BufferLike& buffer );

/// @brief Attempt to push item into buffer.
/// @param[in] buffer Buffer to push item to.
/// @param     item   Item to push.
/// @return
///     - @c true  : Buffer had enough capacity to push item.
///     - @c false : Buffer is full.
template<typename BufferLike> attr_header
bool try_push( BufferLike& buffer, const typename BufferLike::Type& item );

/// @brief Attempt to pop item from buffer.
/// @param[in]  buffer       Buffer to pop from.
/// @param[out] opt_out_item (optional) Pointer to write popped item to.
/// @return
///     - @c true  : Buffer is not empty and last item was popped.
///     - @c false : Buffer is empty.
template<typename BufferLike> attr_header
bool pop( BufferLike& buffer, typename BufferLike::Type* opt_out_item = nullptr );

/// @brief Attempt to emplace item in buffer.
/// @param[in] buffer Buffer to emplace in.
/// @param     item   Item to emplace.
/// @param     at     Index to emplace at.
/// @return
///     - @c true  : Item emplaced at index.
///     - @c false : Buffer is full.
template<typename BufferLike> attr_header
bool try_emplace( BufferLike& buffer, const typename BufferLike::Type& item, usize at );

/// @brief Attempt to insert items in buffer.
/// @param[in] buffer Buffer to insert items in.
/// @param     slice  Slice of items to insert.
/// @param     at     Index to insert at.
/// @return
///     - @c true  : Items inserted at index.
///     - @c false : Buffer does not have enough capacity to insert items.
template<typename BufferLike, typename SliceLike> attr_header
bool try_insert( BufferLike& buffer, const SliceLike& slice, usize at );

/// @brief Attempt to prepend items to beginning of buffer.
/// @param[in] buffer Buffer to prepend items in.
/// @param     slice  Slice of items to prepend.
/// @return
///     - @c true  : Items were prepended.
///     - @c false : Buffer does not have enough capacity to prepend items.
template<typename BufferLike, typename SliceLike> attr_header
bool try_prepend( BufferLike& buffer, const SliceLike& slice );

/// @brief Attempt to append items to end of buffer.
/// @param[in] buffer Buffer to append items in.
/// @param     slice  Slice of items to append.
/// @return
///     - @c true  : Items were appended.
///     - @c false : Buffer does not have enough capacity to append items.
template<typename BufferLike, typename SliceLike> attr_header
bool try_append( BufferLike& buffer, const SliceLike& slice );

/// @brief Remove item in buffer.
/// @param[in] buffer Buffer to remove item from.
/// @param     at     Index of item to remove.
template<typename BufferLike> attr_header
void remove( BufferLike& buffer, usize at );

/// @brief Remove item in buffer by swapping with last item.
/// @param[in] buffer Buffer to remove item from.
/// @param     at     Index of item to remove.
template<typename BufferLike> attr_header
void remove_swap( BufferLike& buffer, usize at );

/// @brief Slice of items.
/// @tparam T Type of items in slice.
template<typename T>
struct Slice {
    /// @brief Type of items in slice.
    typedef T Type;

    /// @brief Number of items in slice.
    usize len;
    /// @brief Union of slice pointers.
    union {
        /// @brief Const pointer to slice items.
        const T*    cbuf;
        /// @brief Pointer to slice items.
        T*          buf;
        /// @brief Pointer to slice item bytes.
        u8*         bytes;
        /// @brief Pointer to slice items.
        void*       ptr;
    };

    /// @brief Create empty slice.
    constexpr Slice();
    /// @brief Create slice.
    /// @param     len Number of items in slice.
    /// @param[in] buf Pointer to start of items in slice.
    constexpr explicit Slice( usize len, const Type* buf );
    /// @brief Create slice from array.
    /// @param[in] array       Array.
    /// @tparam    ArrayLength Number of items in array.
    template<usize ArrayLength>
    constexpr explicit Slice( const T (&array)[ArrayLength] );
    /// @brief Create slice from buffer.
    /// @param[in] buffer Buffer to create slice from.
    constexpr explicit Slice( const Buffer<T>& buffer );
    /// @brief Create slice from buffer.
    /// @param[in] buffer  Buffer to create slice from.
    /// @param     maximum Number of items to create slice of.
    constexpr explicit Slice( const Buffer<T>& buffer, usize maximum );

    /// @brief Index into slice.
    /// @param index Index of item to get.
    /// @return Reference to item at given index.
    constexpr const T& operator[]( usize index ) const;
    /// @brief Index into slice.
    /// @param index Index of item to get.
    /// @return Reference to item at given index.
    constexpr T& operator[]( usize index );

    /// @brief Range-based for loop begin.
    /// @return Pointer to start of slice.
    const T* begin() const;
    /// @brief Range-based for loop end.
    /// @return Pointer to end of slice.
    const T* end() const;
    /// @brief Range-based for loop begin.
    /// @return Pointer to start of slice.
    T* begin();
    /// @brief Range-based for loop end.
    /// @return Pointer to end of slice.
    T* end();
};

/// @brief Buffer container.
/// @tparam T Type of items in buffer.
template<typename T>
struct Buffer {
    /// @brief Type of items in buffer.
    typedef T Type;

    /// @brief Number of items buffer can hold.
    usize cap;
    /// @brief Union of buffer slice components.
    union {
        /// @brief Buffer slice components.
        struct {
            /// @brief Number of items in buffer.
            usize len;
            /// @brief Union of buffer pointers.
            union {
                /// @brief Const pointer to buffer items.
                const T*    cbuf;
                /// @brief Pointer to buffer items.
                T*          buf;
                /// @brief Pointer to buffer item bytes.
                u8*         bytes;
                /// @brief Pointer to buffer items.
                void*       ptr;
            };
        };
        /// @brief Buffer as a slice.
        Slice<T> slice;
    };

    /// @brief Create empty buffer.
    constexpr Buffer();
    /// @brief Create buffer.
    /// @param     cap Number of items buffer can hold.
    /// @param[in] buf Pointer to buffer items.
    constexpr explicit Buffer( usize cap, const T* buf );
    /// @brief Create buffer.
    /// @param     cap Number of items buffer can hold.
    /// @param     len Initial number of items in buffer.
    /// @param[in] buf Pointer to buffer items.
    constexpr explicit Buffer( usize cap, usize len, const T* buf );
    /// @brief Create buffer from slice.
    /// @param[in] slice   Slice of items.
    /// @param     opt_len (optional) Initial number of items in buffer.
    constexpr explicit Buffer( const Slice<T>& slice, usize opt_len = 0 );
    /// @brief Create buffer from array.
    /// @param[in] array       Array of items.
    /// @param     opt_len     (optional) Initial number of items in buffer.
    /// @tparam    ArrayLength Number of items in array.
    template<usize ArrayLength>
    constexpr explicit Buffer( const T (&array)[ArrayLength], usize opt_len = 0 );

    /// @brief Implicitly convert to slice.
    /// @return Slice of items.
    constexpr operator Slice<T>() const;
    /// @brief Index into buffer.
    /// @param index Index of item to get.
    /// @return Reference to item at given index.
    constexpr const T& operator[]( usize index ) const;
    /// @brief Index into buffer.
    /// @param index Index of item to get.
    /// @return Reference to item at given index.
    constexpr T& operator[]( usize index );

    /// @brief Range-based for loop begin.
    /// @return Pointer to start of buffer.
    const T* begin() const;
    /// @brief Range-based for loop end.
    /// @return Pointer to end of buffer.
    const T* end() const;
    /// @brief Range-based for loop begin.
    /// @return Pointer to start of buffer.
    T* begin();
    /// @brief Range-based for loop end.
    /// @return Pointer to end of buffer.
    T* end();
};

// NOTE(alicia): implementation -----------------------------------------------

attr_core_api
void* memory_copy( void* attr_restrict dst, const void* attr_restrict src, usize size );
attr_core_api
void* memory_move( void* dst, const void* src, usize size );
attr_core_api
void* memory_set( void* memory, u8 byte, usize size );

template<typename T> constexpr attr_header
Slice<T>::Slice()
    : len(0), cbuf(nullptr) {}
template<typename T> constexpr attr_header
Slice<T>::Slice( usize len, const Type* buf )
    : len(len), cbuf(buf) {}
template<typename T> template<usize ArrayLength> constexpr attr_header
Slice<T>::Slice( const Type (&array)[ArrayLength] )
    : Slice( ArrayLength, array ) {}
template<typename T> constexpr attr_header
Slice<T>::Slice( const Buffer<T>& buffer )
    : Slice( buffer.len, buffer.buf ) {}
template<typename T> constexpr attr_header
Slice<T>::Slice( const Buffer<T>& buffer, usize maximum )
    : Slice( buffer.len > maximum ? maximum : buffer.len, buffer.buf ) {}

template<typename T> constexpr attr_header
const T& Slice<T>::operator[]( usize index ) const {
    return this->cbuf[index];
}
template<typename T> constexpr attr_header
T& Slice<T>::operator[]( usize index ) {
    return this->buf[index];
}

template<typename T> attr_header
const T* Slice<T>::begin() const {
    return this->cbuf;
}
template<typename T> attr_header
const T* Slice<T>::end() const {
    return this->cbuf + this->len;
}
template<typename T> attr_header
T* Slice<T>::begin() {
    return this->buf;
}
template<typename T> attr_header
T* Slice<T>::end() {
    return this->buf + this->len;
}

template<typename SliceLike> attr_header
bool is_empty( const SliceLike& slice ) {
    return slice.len == 0;
}
template<typename SliceLike> attr_header 
SliceLike truncate( const SliceLike& slice, usize max ) {
    SliceLike result = slice;
    if( max >= result.len ) {
        return result;
    }
    result.len = max;
    return result;
}
template<typename SliceLike> attr_header
SliceLike trim( const SliceLike& slice, usize amount ) {
    return truncate( slice, amount >= slice.len ? 0 : slice.len - amount );
}
template<typename SliceLike> attr_header
SliceLike clip( const SliceLike& slice, usize from, usize to ) {
    SliceLike result = slice;
    if( from >= result.len ) {
        goto out_of_bounds;
    } else {
        usize new_len = to - from;
        if( new_len >= result.len ) {
            goto out_of_bounds;
        } else {
            result.buf = result.buf + from;
            result.len = new_len;
        }
    }

    return result;
out_of_bounds:
    result.buf = result.buf + result.len;
    result.len = 0;
    return result;
}
template<typename SliceLike> attr_header
SliceLike advance( const SliceLike& slice, usize amount ) {
    SliceLike result = slice;
    if( amount >= result.len ) {
        result.buf = result.buf + result.len;
        result.len = 0;
    } else {
        result.buf = result.buf + amount;
        result.len = result.len - amount;
    }
    return result;
}
template<typename SliceLike> attr_header
void split(
    const SliceLike& slice,
    usize            at,
    SliceLike*       opt_out_left,
    SliceLike*       opt_out_right
) {
    if( opt_out_left ) {
        *opt_out_left = truncate( slice, at );
    }
    if( opt_out_right ) {
        *opt_out_right = advance( slice, at + 1 );
    }
}

template<typename T> constexpr attr_header
Buffer<T>::Buffer()
    : cap(0), len(0), cbuf(nullptr) {}
template<typename T> constexpr attr_header
Buffer<T>::Buffer( usize cap, const T* buf )
    : cap(cap), len(0), cbuf(buf) {}
template<typename T> constexpr attr_header
Buffer<T>::Buffer( usize cap, usize len, const T* buf )
    : cap(cap), len(len), cbuf(buf) {}
template<typename T> constexpr attr_header
Buffer<T>::Buffer( const Slice<T>& slice, usize opt_len )
    : cap(slice.len), len(opt_len), buf(slice.buf) {}
template<typename T> template<usize ArrayLength>
constexpr attr_header
Buffer<T>::Buffer( const T (&array)[ArrayLength], usize opt_len )
    : cap(ArrayLength), len(opt_len), buf(array) {}

template<typename T> constexpr attr_header
Buffer<T>::operator Slice<T>() const {
    return this->slice;
}
template<typename T> constexpr attr_header
const T& Buffer<T>::operator[]( usize index ) const {
    return this->cbuf[index];
}
template<typename T> constexpr attr_header
T& Buffer<T>::operator[]( usize index ) {
    return this->buf[index];
}

template<typename T> attr_header
const T* Buffer<T>::begin() const {
    return this->cbuf;
}
template<typename T> attr_header
const T* Buffer<T>::end() const {
    return this->cbuf + this->len;
}
template<typename T> attr_header
T* Buffer<T>::begin() {
    return this->buf;
}
template<typename T> attr_header
T* Buffer<T>::end() {
    return this->buf + this->len;
}

template<typename BufferLike> attr_header
bool is_full( const BufferLike& buffer ) {
    return buffer.len >= buffer.cap;
}
template<typename BufferLike> attr_header
usize remaining( const BufferLike& buffer ) {
    return buffer.cap - buffer.len;
}
template<typename BufferLike> attr_header
void reset( BufferLike& buffer ) {
    buffer.len = 0;
}
template<typename BufferLike> attr_header
bool try_push( BufferLike& buffer, const typename BufferLike::Type& item ) {
    if( is_full( buffer ) ) {
        return false;
    }
    buffer.buf[buffer.len++] = item;
    return true;
}
template<typename BufferLike> attr_header
bool pop( BufferLike& buffer, typename BufferLike::Type* opt_out_item ) {
    if( is_empty( buffer ) ) {
        return false;
    }
    buffer.len -= 1;
    if( opt_out_item ) {
        *opt_out_item = buffer.buf[buffer.len];
    }
    return true;
}
template<typename BufferLike> attr_header
bool try_emplace( BufferLike& buffer, const typename BufferLike::Type& item, usize at ) {
    if( is_full( buffer ) ) {
        return false;
    }

    usize move_amount = buffer.len - at;

    memory_move(
        buffer.buf + at + 1,
        buffer.buf + at,
        sizeof(typename BufferLike::Type) * move_amount );

    buffer.buf[at]  = item;
    buffer.len     += 1;
    return true;
}
template<typename BufferLike, typename SliceLike> attr_header
bool try_insert( BufferLike& buffer, const SliceLike& slice, usize at ) {
    static_assert( sizeof(typename BufferLike::Type) == sizeof(typename SliceLike::Type) );

    if( is_empty(slice) ) {
        return true;
    }
    if( (at + slice.len) > buffer.cap ) {
        return false;
    }

    if( at > buffer.len ) {
        memory_set(
            buffer.buf + buffer.len, 0,
            sizeof(typename BufferLike::Type) * (at - buffer.len) );
    }

    usize move_amount = buffer.len ? (buffer.len - at) : 0;
    if( move_amount ) {
        memory_move(
            buffer.buf + at + slice.len,
            buffer.buf + at,
            sizeof(typename BufferLike::Type) * move_amount );
    }
    memory_copy(
        buffer.buf + at,
        slice.buf,
        sizeof(typename BufferLike::Type) * slice.len );
    if( at > buffer.len ) {
        buffer.len = at;
    }
    buffer.len += slice.len;

    return true;
}
template<typename BufferLike, typename SliceLike> attr_header
bool try_prepend( BufferLike& buffer, const SliceLike& slice ) {
    return try_insert( buffer, slice, 0 );
}
template<typename BufferLike, typename SliceLike> attr_header
bool try_append( BufferLike& buffer, const SliceLike& slice ) {
    return try_insert( buffer, slice, buffer.len );
}
template<typename BufferLike> attr_header
void remove( BufferLike& buffer, usize at ) {
    usize move_amount = buffer.len - at;
    memory_move(
        buffer.buf + at,
        buffer.buf + at + 1,
        sizeof(typename BufferLike::Type) * move_amount );
    buffer.len -= 1;
}
template<typename BufferLike> attr_header
void remove_swap( BufferLike& buffer, usize at ) {
    if( buffer.len == 1 && !at ) {
        buffer.len = 0;
        return;
    }
    buffer.len     -= 1;
    buffer.buf[at]  = buffer.buf[buffer.len];
}

#endif /* header guard */
