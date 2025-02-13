#if !defined(CORE_COLLECTIONS_H)
#define CORE_COLLECTIONS_H
/**
 * @file   collections.h
 * @brief  Collection types.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 19, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/defines.h" // IWYU pragma: export

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

// forward declaration
struct AllocatorInterface;

/// @brief Generate name of dynamic array type.
/// @details
/// Define dynamic array with darray_define() before using this macro.
/// @param T (type) Type of dynamic array.
#define darray_name(T)\
    DynamicArray_##T
/// @brief Generate name of dynamic array struct type.
/// @details
/// Define dynamic array with darray_define() before using this macro.
/// @param T (type) Type of dynamic array.
#define darray( T )\
    struct darray_name(T)
/// @brief Define a new dynamic array structure.
/// @details
/// - len (usize) Length of dynamic array.
/// - cap (usize) Maximum number of items that array can hold.
/// - buf (T*)    Pointer to start of array.
/// @param T (type) Type of dynamic array items.
#define darray_define( T )\
typedef struct darray_name(T) {\
    usize len;\
    usize cap;\
    T*    buf;\
} darray_name(T)

#if !defined(CORE_DISABLE_PREDEFINED_ARRAYS)
darray_define(i8);
darray_define(i16);
darray_define(i32);
darray_define(i64);
darray_define(isize);

darray_define(u8);
darray_define(u16);
darray_define(u32);
darray_define(u64);
darray_define(usize);

darray_define(b8);
darray_define(b16);
darray_define(b32);
darray_define(b64);
darray_define(bsize);

darray_define(f32);
darray_define(f64);
#endif

/// @brief First in, first out container.
typedef struct Queue {
    /// @brief Index of head of queue.
    u32 head;
    /// @brief Index of tail of queue.
    u32 tail;
    /// @brief Total number of items queue can hold.
    u32 cap;
    /// @brief Size of each item in queue.
    u32 stride;
    /// @brief Pointer to start of queue buffer.
    void* buf;
} Queue;

/// @brief Map of key-value pairs.
typedef struct Hashmap {
    /// @brief Size of each item in hashmap.
    u32 stride;
    /// @brief Total number of items that hashmap can hold.
    /// @note This also corresponds to total number of keys
    /// that hashmap can hold.
    u32 cap;
    /// @brief Number of items in hashmap.
    /// @note This also corresponds to number of keys in hashmap.
    u32 len;
    /// @brief Pointer to start of hashmap items array.
    void* buf;
    /// @brief Value of the largest key in @c keys. Used internally only.
    hash64 largest_key;
} Hashmap;

/// @brief Tightly packed boolean array.
typedef b8 PackedBool;

/// @brief Allocate generic array.
/// @param      stride    Size of items in generic array.
/// @param      cap       Number of items in generic array.
/// @param[out] buf       Pointer to write start of generic array.
/// @param[in]  allocator Pointer to allocator interface.
/// @return
///     - @c true  : Allocated new generic array.
///     - @c false : Failed to allocate new generic array.
attr_core_api
b32 generic_array_from_alloc(
    usize stride, usize cap, void** buf, struct AllocatorInterface* allocator );
/// @brief Attempt to push new item in generic array.
/// @param         stride Size of items in array.
/// @param[in,out] len    Number of items in array.
/// @param         cap    Number of items array can hold.
/// @param[in]     buf    Pointer to start of array.
/// @param[in]     item   Pointer to item to push. Must be of size @c stride.
/// @return
///     - @c true  : Array had enough capacity to push new item.
///     - @c false : Array is full.
attr_core_api
b32 generic_array_try_push(
    usize stride, usize* len, usize cap, void* buf, const void* item );
/// @brief Attempt to emplace new item in generic array.
/// @param         stride Size of items in array.
/// @param[in,out] len    Number of items in array.
/// @param         cap    Number of items array can hold.
/// @param[in]     buf    Pointer to start of array.
/// @param[in]     item   Pointer to item to emplace. Must be of size @c stride.
/// @param         at     Index to emplace item at.
/// @return
///     - @c true  : Array had enough capacity to emplace new item.
///     - @c false : Array is full.
attr_core_api
b32 generic_array_try_emplace(
    usize stride, usize* len, usize cap, void* buf, const void* item, usize at );
/// @brief Attempt to insert array in generic array.
/// @param         stride     Size of items in array.
/// @param[in,out] len        Number of items in array.
/// @param         cap        Number of items array can hold.
/// @param[in]     buf        Pointer to start of array.
/// @param         insert_len Number of items to insert.
/// @param[in]     insert     Pointer to array to insert. Items in this array must be of size @c stride.
/// @param         at         Index to insert array at.
/// @return
///     - @c true  : Array had enough capacity to insert items.
///     - @c false : Array cannot hold @c insert_len new items.
attr_core_api
b32 generic_array_try_insert(
    usize stride, usize* len, usize cap, void* buf,
    usize insert_len, const void* insert, usize at );
/// @brief Attempt to prepend array in generic array.
/// @param         stride      Size of items in array.
/// @param[in,out] len         Number of items in array.
/// @param         cap         Number of items array can hold.
/// @param[in]     buf         Pointer to start of array.
/// @param         prepend_len Number of items to prepend.
/// @param[in]     prepend     Pointer to array to prepend. Items in this array must be of size @c stride.
/// @return
///     - @c true  : Array had enough capacity to prepend items.
///     - @c false : Array cannot hold @c prepend_len new items.
attr_always_inline attr_header
b32 generic_array_try_prepend(
    usize stride, usize* len, usize cap, void* buf,
    usize prepend_len, const void* prepend
) {
    return generic_array_try_insert(
        stride, len, cap, buf, prepend_len, prepend, 0 );
}
/// @brief Attempt to append array in generic array.
/// @param         stride     Size of items in array.
/// @param[in,out] len        Number of items in array.
/// @param         cap        Number of items array can hold.
/// @param[in]     buf        Pointer to start of array.
/// @param         append_len Number of items to append.
/// @param[in]     append     Pointer to array to append. Items in this array must be of size @c stride.
/// @return
///     - @c true  : Array had enough capacity to append items.
///     - @c false : Array cannot hold @c append_len new items.
attr_always_inline attr_header
b32 generic_array_try_append(
    usize stride, usize* len, usize cap, void* buf,
    usize append_len, const void* append
) {
    return generic_array_try_insert(
        stride, len, cap, buf, append_len, append, *len ? *len - 1 : 0 );
}
/// @brief Grow dynamically allocated generic array.
/// @param         stride    Size of items in array.
/// @param[in,out] cap       Pointer to capacity of array.
/// @param[in,out] buf       Pointer to pointer to start of array.
/// @param         amount    Number of items to grow array by.
/// @param[in]     allocator Pointer to allocator interface.
/// @return
///     - @c true  : Reallocated array successfully.
///     - @c false : Failed to reallocate array.
attr_core_api
b32 generic_array_grow(
    usize stride, usize* cap, void** buf,
    usize amount, struct AllocatorInterface* allocator );
/// @brief Push new item in generic array.
/// @param         stride    Size of items in array.
/// @param[in,out] len       Number of items in array.
/// @param         cap       Number of items array can hold.
/// @param[in]     buf       Pointer to start of array.
/// @param[in]     item      Pointer to item to push. Must be of size @c stride.
/// @param[in]     allocator Pointer to allocator interface.
/// @return
///     - @c true  : New item pushed. If reallocation was necessary, it was successful.
///     - @c false : Failed to reallocate array.
attr_core_api
b32 generic_array_push(
    usize stride, usize* len, usize* cap, void** buf,
    const void* item, struct AllocatorInterface* allocator );
/// @brief Emplace new item in generic array.
/// @param         stride    Size of items in array.
/// @param[in,out] len       Number of items in array.
/// @param         cap       Number of items array can hold.
/// @param[in]     buf       Pointer to start of array.
/// @param[in]     item      Pointer to item to emplace. Must be of size @c stride.
/// @param         at        Index to emplace item at.
/// @param[in]     allocator Pointer to allocator interface.
/// @return
///     - @c true  : New item emplaced. If reallocation was necessary, it was successful.
///     - @c false : Failed to reallocate array.
attr_core_api
b32 generic_array_emplace(
    usize stride, usize* len, usize* cap, void** buf,
    const void* item, usize at, struct AllocatorInterface* allocator );
/// @brief Insert array in generic array.
/// @param         stride     Size of items in array.
/// @param[in,out] len        Number of items in array.
/// @param         cap        Number of items array can hold.
/// @param[in]     buf        Pointer to start of array.
/// @param         insert_len Number of items to insert.
/// @param[in]     insert     Pointer to array to insert. Items in this array must be of size @c stride.
/// @param         at         Index to insert array at.
/// @param[in]     allocator  Pointer to allocator interface.
/// @return
///     - @c true  : Items inserted. If reallocation was necessary, it was successful.
///     - @c false : Failed to reallocate array.
attr_core_api
b32 generic_array_insert(
    usize stride, usize* len, usize* cap, void** buf,
    usize insert_len, const void* insert, usize at,
    struct AllocatorInterface* allocator );
/// @brief Prepend array in generic array.
/// @param         stride      Size of items in array.
/// @param[in,out] len         Number of items in array.
/// @param         cap         Number of items array can hold.
/// @param[in]     buf         Pointer to start of array.
/// @param         prepend_len Number of items to prepend.
/// @param[in]     prepend     Pointer to array to prepend. Items in this array must be of size @c stride.
/// @param[in]     allocator   Pointer to allocator interface.
/// @return
///     - @c true  : Items prepended. If reallocation was necessary, it was successful.
///     - @c false : Failed to reallocate array.
attr_always_inline attr_header
b32 generic_array_prepend(
    usize stride, usize* len, usize* cap, void** buf,
    usize prepend_len, const void* prepend,
    struct AllocatorInterface* allocator
) {
    return generic_array_insert(
        stride, len, cap, buf, prepend_len, prepend, 0, allocator );
}
/// @brief Append array in generic array.
/// @param         stride     Size of items in array.
/// @param[in,out] len        Number of items in array.
/// @param         cap        Number of items array can hold.
/// @param[in]     buf        Pointer to start of array.
/// @param         append_len Number of items to append.
/// @param[in]     append     Pointer to array to append. Items in this array must be of size @c stride.
/// @param[in]     allocator  Pointer to allocator interface.
/// @return
///     - @c true  : Items appended. If reallocation was necessary, it was successful.
///     - @c false : Failed to reallocate array.
attr_always_inline attr_header
b32 generic_array_append(
    usize stride, usize* len, usize* cap, void** buf,
    usize append_len, const void* append,
    struct AllocatorInterface* allocator
) {
    return generic_array_insert(
        stride, len, cap, buf, append_len,
        append, *len ? *len - 1 : 0, allocator );
}
/// @brief Pop last item in generic array.
/// @param         stride       Size of items in array.
/// @param[in,out] len          Number of items in array.
/// @param[in]     buf          Pointer to start of array.
/// @param[out]    opt_out_item (optional) Pointer to write popped item.
/// @return
///     - @c true  : Array had item to pop.
///     - @c false : Array already empty.
attr_core_api
b32 generic_array_pop(
    usize stride, usize* len, void* buf, void* opt_out_item );
/// @brief Remove item from generic array.
/// @param         stride Size of items in array.
/// @param[in,out] len    Number of items in array.
/// @param[in]     buf    Pointer to start of array.
/// @param         at     Index of item to remove.
/// @return
///     - @c true  : Removed item from array.
///     - @c false : Array is empty or @c at is outside bounds of array.
attr_core_api
b32 generic_array_remove(
    usize stride, usize* len, void* buf, usize at );
/// @brief Remove range of items from generic array.
/// @param         stride         Size of items in array.
/// @param[in,out] len            Number of items in array.
/// @param[in]     buf            Pointer to start of array.
/// @param         from_inclusive Start of range to remove.
/// @param         to_exclusive   End of range to remove.
/// @return
///     - @c true  : Removed items from array.
///     - @c false : Range provided is invalid.
attr_core_api
b32 generic_array_remove_range(
    usize stride, usize* len, void* buf,
    usize from_inclusive, usize to_exclusive );
/// @brief Allocate new generic array and copy items from given array.
/// @param      stride    Size of items in arrays.
/// @param      len       Number of items in array.
/// @param[in]  buf       Pointer to start of array to clone.
/// @param[out] out_len   Pointer to write destination array length.
/// @param[out] out_cap   Pointer to write destination array capacity.
/// @param[out] out_res   Pointer to write destination array pointer.
/// @param[in]  allocator Pointer to allocator interface.
/// @return
///     - @c true  : Allocated new generic array and copied items.
///     - @c false : Failed to allocate new array.
attr_core_api
b32 generic_array_clone(
    usize stride, usize len, void* buf,
    usize* out_len, usize* out_cap, void** out_res,
    struct AllocatorInterface* allocator );

/// @brief Create new dynamic array.
/// @param     T        (type)  Type of dynamic array. Must be defined with darray_define()
/// @param     capacity (usize) Capacity of dynamic array buffer.
/// @param[in] buffer   (T*)    Pointer to start of dynamic array.
/// @return darray(T)
#define darray_new( T, capacity, buffer )\
    struct_literal( darray(T) ){ .cap=capacity, .len=0, .buf=buffer }
/// @brief Create empty dynamic array.
/// @param T (type) Type of dynamic array. Must be defined with darray_define().
/// @return darray(T)
#define darray_empty( T )\
    struct_literal( darray(T) ){ .cap=0, .len=0, .buf=0 }
/// @brief Allocate dynamic array.
/// @param      cap       (usize)               Number of items to allocate.
/// @param[out] out_array (darray*)             Pointer to dynamic array to allocate.
/// @param[in]  allocator (AllocatorInterface*) Pointer to allocator interface.
/// @return
///     - @c true  : Allocated dynamic array successfully.
///     - @c false : Failed to allocate dynamic array.
#define darray_from_alloc( cap, out_array, allocator )\
    (darray_cap(out_array) = generic_array_from_alloc(\
        darray_stride(out_array), cap,\
        (void**)&darray_buf(out_array), allocator ) ? cap : 0) ? true : false
/// @brief Free dynamic array.
/// @param[in] array     (darray*)             Pointer to dynamic array.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator interface.
#define darray_free( array, allocator )\
    (allocator)->free( darray_buf(array), darray_stride(array) * darray_cap(array),\
        0, (allocator)->ctx )
/// @brief Index into dynamic array.
/// @param[in] array (darray*) Dynamic array to index into.
/// @param     index (usize)   Index to get.
/// @return Type of array items.
#define darray_index( array, index ) (array)->buf[index]
/// @brief Get size of dynamic array items.
/// @param[in] array (darray*) Dynamic array.
/// @return Size of array items.
#define darray_stride( array ) sizeof(*((array)->buf))
/// @brief Get length of dynamic array.
/// @param[in] array (darray*) Dynamic array.
/// @return Number of items in dynamic array.
#define darray_len( array ) (array)->len
/// @brief Get capacity of dynamic array.
/// @param[in] array (darray*) Dynamic array.
/// @return Number of items dynamic array can hold.
#define darray_cap( array ) (array)->cap
/// @brief Get pointer to start of dynamic array.
/// @param[in] array (darray*) Dynamic array.
/// @return Pointer to start of array.
#define darray_buf( array ) (array)->buf
/// @brief Check if dynamic array is empty.
/// @param[in] array (darray*) Dynamic array.
/// @return
///     - @c true  : Dynamic array is empty.
///     - @c false : Dynamic array is not empty.
#define darray_is_empty( array ) (darray_len(array) == 0)
/// @brief Check if dynamic array is full.
/// @param[in] array (darray*) Dynamic array.
/// @return
///     - @c true  : Dynamic array is full.
///     - @c false : Dynamic array is not full.
#define darray_is_full( array ) (darray_len(array) == darray_cap(array))
/// @brief Calculate how many additional items dynamic array can hold.
/// @param[in] array (darray*) Dynamic array.
/// @return (usize) Number of additional items array can hold.
#define darray_remaining( array ) (darray_cap(array) - darray_len(array))
/// @brief Attempt to push new item to end of dynamic array.
/// @param[in] array (darray*) Pointer to dynamic array.
/// @param[in] item  (T*)      Pointer to item to push.
/// @return
///     - @c true  : Dynamic array had enough capacity to push new item.
///     - @c false : Dynamic array is full.
#define darray_try_push( array, item )\
    generic_array_try_push( darray_stride(array), &darray_len(array),\
        darray_cap(array), darray_buf(array), item )
/// @brief Attempt to emplace new item in dynamic array.
/// @param[in] array (darray*) Pointer to dynamic array.
/// @param[in] item  (T*)      Pointer to item to emplace.
/// @param     at    (usize)   Index to emplace item in.
/// @return
///     - @c true  : Dynamic array had enough capacity to emplace new item.
///     - @c false : Dynamic array is full.
#define darray_try_emplace( array, item, at )\
    generic_array_try_emplace( darray_stride(array), &darray_len(array),\
        darray_cap(array), darray_buf(buf), item, at )
/// @brief Attempt to insert array in dynamic array.
/// @param[in] array     (darray*) Pointer to dynamic array.
/// @param     items_len (usize)   Number of items in array to be inserted.
/// @param[in] items     (T*)      Pointer to array to insert.
/// @param     at        (usize)   Index to insert items in.
/// @return
///     - @c true  : Dynamic array had enough capacity to insert items.
///     - @c false : Dynamic array does not have enough capacity to insert.
#define darray_try_insert( array, items_len, items, at )\
    generic_array_try_insert( darray_stride(array), &darray_len(array),\
        darray_cap(array), darray_buf(buf), items_len, items, at )
/// @brief Attempt to prepend array in dynamic array.
/// @param[in] array     (darray*) Pointer to dynamic array.
/// @param     items_len (usize)   Number of items in array to be prepended.
/// @param[in] items     (T*)      Pointer to array to prepend.
/// @return
///     - @c true  : Dynamic array had enough capacity to prepend items.
///     - @c false : Dynamic array does not have enough capacity to prepend.
#define darray_try_prepend( array, items_len, items )\
    generic_array_try_prepend( darray_stride(array), &darray_len(array),\
        darray_cap(array), darray_buf(buf), items_len, items )
/// @brief Attempt to append array in dynamic array.
/// @param[in] array     (darray*) Pointer to dynamic array.
/// @param     items_len (usize)   Number of items in array to be appended.
/// @param[in] items     (T*)      Pointer to array to append.
/// @return
///     - @c true  : Dynamic array had enough capacity to append items.
///     - @c false : Dynamic array does not have enough capacity to append.
#define darray_try_append( array, items_len, items )\
    generic_array_try_append( darray_stride(array), &darray_len(array),\
        darray_cap(array), darray_buf(buf), items_len, items )
/// @brief Push new item to end of dynamic array.
/// @param[in] array     (darray*)             Pointer to dynamic array.
/// @param[in] item      (T*)                  Pointer to item to insert.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator interface.
/// @return
///     - @c true  : Pushed new item. If reallocation was required, it was successful.
///     - @c false : Failed to reallocate dynamic array.
#define darray_push( array, item, allocator )\
    generic_array_push( darray_stride(array), &darray_len(array),\
        &darray_cap(array), (void**)&darray_buf(array), item, allocator )
/// @brief Emplace new item in dynamic array.
/// @param[in] array     (darray*)             Pointer to dynamic array.
/// @param[in] item      (T*)                  Pointer to item to emplace.
/// @param     at        (usize)               Index to emplace item at.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator interface.
/// @return
///     - @c true  : Emplaced new item. If reallocation was required, it was successful.
///     - @c false : Failed to reallocate dynamic array.
#define darray_emplace( array, item, at, allocator )\
    generic_array_emplace( darray_stride(array), &darray_len(array),\
        &darray_cap(array), (void**)&darray_buf(buf), item, at, allocator )
/// @brief Insert array in dynamic array.
/// @param[in] array     (darray*)             Pointer to dynamic array.
/// @param     items_len (usize)               Number of items to insert.
/// @param[in] items     (T*)                  Pointer to start of array to insert.
/// @param     at        (usize)               Index to insert items at.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator interface.
/// @return
///     - @c true  : Inserted new items. If reallocation was required, it was successful.
///     - @c false : Failed to reallocate dynamic array.
#define darray_insert( array, items_len, items, at, allocator )\
    generic_array_insert( darray_stride(array), &darray_len(array),\
        &darray_cap(array), (void**)&darray_buf(buf),\
        items_len, items, at, allocator )
/// @brief Prepend array to start of dynamic array.
/// @param[in] array     (darray*)             Pointer to dynamic array.
/// @param     items_len (usize)               Number of items to prepend.
/// @param[in] items     (T*)                  Pointer to start of array to prepend.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator interface.
/// @return
///     - @c true  : New items prepended. If reallocation was required, it was successful.
///     - @c false : Failed to reallocate dynamic array.
#define darray_prepend( array, items_len, items, allocator )\
    generic_array_prepend( darray_stride(array), &darray_len(array),\
        &darray_cap(array), (void**)&darray_buf(buf), items_len, items, allocator )
/// @brief Append array to end of dynamic array.
/// @param[in] array     (darray*)             Pointer to dynamic array.
/// @param     items_len (usize)               Number of items to append.
/// @param[in] items     (T*)                  Pointer to start of array to append.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator interface.
/// @return
///     - @c true  : New items appended. If reallocation was required, it was successful.
///     - @c false : Failed to reallocate dynamic array.
#define darray_append( array, items_len, items, allocator )\
    generic_array_append( darray_stride(array), &darray_len(array),\
        &darray_cap(array), (void**)&darray_buf(buf), items_len, items, allocator )
/// @brief Pop item from end of array.
/// @param[in]  array        (darray*) Pointer to dynamic array.
/// @param[out] opt_out_item (T*)      (optional) Pointer to write popped item.
/// @return
///     - @c true  : Item popped.
///     - @c false : Array was empty.
#define darray_pop( array, opt_out_item )\
    generic_array_pop( darray_stride(array), &darray_len(array),\
        darray_buf(array), opt_out_item )
/// @brief Remove item from array.
/// @param[in] array (darray*) Pointer to dynamic array.
/// @param     at    (usize)   Index of item to remove.
/// @return
///     - @c true  : Item removed.
///     - @c false : Index was out of bounds.
#define darray_remove( array, at )\
    generic_array_remove( darray_stride(array), &darray_len(array),\
        darray_buf(array), at )
/// @brief Remove range from array.
/// @param[in] array          (darray*) Pointer to dynamic array.
/// @param     from_inclusive (usize)   Start of range to remove.
/// @param     to_exclusive   (usize)   End of range to remove.
/// @return
///     - @c true  : Removed range of items.
///     - @c false : Range was out of bounds of array.
#define darray_remove_range( array, from_inclusive, to_exclusive )\
    generic_array_remove_range( darray_stride(array), &darray_len(array),\
        darray_buf(array), from_inclusive, to_exclusive )
/// @brief Clone dynamic array to new array.
/// @param[out] dst       (darray*)            Pointer to destination dynamic array.
/// @param[in]  src       (darray*)            Pointer to source dynamic array.
/// @param[in]  allocator (AllocatorInterface) Pointer to allocator interface.
/// @return
///     - @c true  : Cloned @c src to @c dst.
///     - @c false : Failed to allocate @c dst.
#define darray_clone( dst, src, allocator )\
    generic_array_clone( darray_stride(src), darray_len(src),\
        darray_buf(src), &darray_len(dst), &darray_cap(dst),\
        (void**)&darray_buf(dst), allocator )

/// @brief Calculate how many bytes are needed for given number of bits.
/// @param count (integer) Number of bits in packed bool array.
/// @return (integer) Number of bytes required for packed boolean array.
#define packed_bool_calculate_size( count )\
    (((count) / 8) + (((count) % 8) ? 1 : 0))
/// @brief Calculate how many bits given number of bytes can hold.
/// @note Assumes system bitdepth is 8.
/// @param count (integer) Number of bytes.
/// @return (integer) Number of bits.
#define packed_bool_bytes_to_bits( count )\
    ((count) * 8)
/// @brief Get value of boolean in packed bool array at given index.
/// @param[in] array (PackedBool*) Pointer to start of packed bool array.
/// @param     index (integer)     Index to get value of.
/// @return (b8) Value of bit at given index.
#define packed_bool_index( array, index )\
    (( (array)[(index) / 8] & (1 << ((index) % 8)) ) == (1 << ((index) % 8)))
/// @brief Set value of bit in packed bool array.
/// @param[in] array (PackedBool*) Pointer to start of packed bool array.
/// @param     index (integer)     Index of bit to set.
/// @param     value (bool)        Value to set bit to.
/// @return (b8) @c value
#define packed_bool_set( array, index, value )\
    ( (value) ?\
        ( *((array) + ((index) / 8)) |=  (1 << ((index) % 8)) ) :\
        ( *((array) + ((index) / 8)) &= ~(1 << ((index) % 8)) ) )

/// @brief Create new queue.
/// @param _stride  (usize) Size of each item in queue.
/// @param capacity (usize) Number of items queue can hold.
/// @param buffer   (void*) Pointer to start of queue buffer.
/// @return Queue.
#define queue_new( _stride, capacity, buffer )\
    struct_literal(Queue){\
        .stride=_stride, .cap=capacity, .buf=buffer, .head=0, .tail=0 }
/// @brief Create empty queue.
/// @return Queue.
#define queue_empty() queue_new( 0, 0, 0 )
/// @brief Create queue from allocator.
/// @param      stride    Size of each item in queue.
/// @param      cap       Number of items queue can hold.
/// @param[out] out_queue Pointer to write new queue.
/// @param[in]  allocator Pointer to allocator interface.
/// @return
///     - @c true  : Allocated new queue.
///     - @c false : Failed to allocate queue.
attr_core_api
b32 queue_from_alloc(
    usize stride, usize cap, Queue* out_queue, struct AllocatorInterface* allocator );
/// @brief Free queue created from allocator.
/// @param[in] queue     Pointer to queue to free.
/// @param[in] allocator Pointer to allocator interface.
attr_core_api
void queue_free( Queue* queue, struct AllocatorInterface* allocator );
/// @brief Calculate number of items in queue.
/// @param[in] queue Queue.
/// @return Number of items in queue.
attr_always_inline attr_header
usize queue_len( const Queue* queue ) {
    if( queue->head > queue->tail ) {
        return queue->tail + ((0xFFFFFFFF - queue->head) + 1);
    } else {
        return queue->tail - queue->head;
    }
}
/// @brief Check if queue is full.
/// @param[in] queue Queue.
/// @return
///     - @c true  : Queue is full.
///     - @c false : Queue is not full.
attr_always_inline attr_header
b32 queue_is_full( const Queue* queue ) {
    return queue_len( queue ) == queue->cap;
}
/// @brief Check if queue is empty.
/// @param[in] queue Queue.
/// @return
///     - @c true  : Queue is empty.
///     - @c false : Queue is not empty.
attr_always_inline attr_header
b32 queue_is_empty( const Queue* queue ) {
    return queue_len( queue ) == 0;
}
/// @brief Reallocate queue buffer.
/// @param[in] queue     Queue to reallocate.
/// @param     amount    Number of additional items to allocate.
/// @param[in] allocator Pointer to allocator interface.
/// @return
///     - @c true  : Reallocated queue buffer.
///     - @c false : Failed to reallocate queue buffer.
attr_core_api
b32 queue_grow(
    Queue* queue, usize amount, struct AllocatorInterface* allocator );
/// @brief Attempt to enqueue new item in queue.
/// @param[in] queue Queue.
/// @param[in] item  Pointer to item to enqueue.
/// @return
///     - @c true  : Queue had enough capacity to enqueue new item.
///     - @c false : Queue is full.
attr_core_api
b32 queue_try_enqueue( Queue* queue, const void* item );
/// @brief Enqueue new item in queue.
/// @param[in] queue     Queue.
/// @param[in] item      Pointer to item to enqueue.
/// @param[in] allocator Pointer to allocator interface.
/// @return
///     - @c true  : Item enqueued. If reallocation is needed, it was successful.
///     - @c false : Failed to reallocate queue.
attr_core_api
b32 queue_enqueue(
    Queue* queue, const void* item, struct AllocatorInterface* allocator );
/// @brief Dequeue item by reference.
/// @note
/// Item should be used before enqueueing as it may be overwritten.
/// @param[in]  queue    Queue.
/// @param[out] out_item Pointer to write item pointer to.
/// @return
///     - @c true  : Removed item from queue and wrote out its pointer.
///     - @c false : Queue is empty.
attr_core_api
b32 queue_dequeue_ref( Queue* queue, void** out_item );
/// @brief Dequeue item.
/// @param[in]  queue        Queue.
/// @param[out] opt_out_item (optional) Pointer to write item value to.
/// @return
///     - @c true  : Removed item from queue and zeroed out its memory.
///     - @c false : Queue is empty.
attr_core_api
b32 queue_dequeue( Queue* queue, void* opt_out_item );
/// @brief Peek at next item in queue without removing it.
/// @note
/// Item should be used before enqueueing as it may be overwritten.
/// @param[in]  queue    Queue.
/// @param[out] out_item Pointer to write item pointer to.
/// @return
///     - @c true  : Peeked at item.
///     - @c false : Queue is empty.
attr_core_api
b32 queue_peek( Queue* queue, void** out_item );
/// @brief Reset queue and zero out memory.
/// @param[in] queue Queue to clear.
attr_core_api
void queue_clear( Queue* queue );

/// @brief Calculate how much memory is required for given hashmap.
/// @param stride   (u32) Size of items in hashmap.
/// @param capacity (u32) Total number of items hashmap can hold.
/// @return (usize) Bytes required for hashmap buffer. 
#define hashmap_memory_requirement( stride, capacity )\
    (((stride) * (capacity)) + (sizeof(hash64) * (capacity)))
/// @brief Create a new hashmap from existing buffer.
/// @param     _stride  (u32)   Size of items in hashmap.
/// @param     capacity (u32)   Total number of items hashmap can hold.
/// @param[in] buffer   (void*) Pointer to start of hashmap buffer. Must be able to hold result of hashmap_memory_requirement().
/// @return Hashmap.
#define hashmap_new( _stride, capacity, buffer )\
   struct_literal(Hashmap){\
       .stride=_stride, .cap=capacity, .buf=(void*)buffer,\
       .len=0, .largest_key=0 }
/// @brief Create empty hashmap.
/// @return Hashmap.
#define hashmap_empty() hashmap_new( 0, 0, 0 )
/// @brief Create hashmap from allocator.
/// @param      stride    Size of items in hashmap.
/// @param      cap       Total number of items hashmap can hold.
/// @param[out] out_map   Pointer to write hashmap to.
/// @param[in]  allocator Pointer to allocator interface.
/// @return
///     - @c true  : Hashmap allocated successfully.
///     - @c false : Failed to allocate hashmap.
attr_core_api
b32 hashmap_from_alloc(
    usize stride, usize cap, Hashmap* out_map,
    struct AllocatorInterface* allocator );
/// @brief Free allocated hashmap.
/// @param[in] map       Hashmap to free.
/// @param[in] allocator Pointer to allocator interface.
attr_core_api
void hashmap_free(
    Hashmap* map, struct AllocatorInterface* allocator );
/// @brief Get pointer to hashmap keys. Keys are stored inline with hashmap buffer.
/// @param[in] map (Hashmap*) Pointer to hashmap.
/// @return (hash64*) Pointer to start of keys array.
#define hashmap_keys( map ) (hash64*)(((u8*)(map)->buf) + ((map)->stride * (map)->cap))
/// @brief Check if hashmap is empty.
/// @param[in] map Hashmap.
/// @return
///     - @c true  : Hashmap is empty.
///     - @c false : Hashmap is not empty.
attr_always_inline attr_header
b32 hashmap_is_empty( const Hashmap* map ) {
    return map->len == 0;
}
/// @brief Check if hashmap is full.
/// @param[in] map Hashmap.
/// @return
///     - @c true  : Hashmap is full.
///     - @c false : Hashmap is not full.
attr_always_inline attr_header
b32 hashmap_is_full( const Hashmap* map ) {
    return map->len == map->cap;
}
/// @brief Set hashmap length to zero and zero out memory.
/// @param[in] map Hashmap.
attr_core_api
void hashmap_clear( Hashmap* map );
/// @brief Reallocate hashmap buffer.
/// @param[in] map       Hashmap.
/// @param     amount    Number of items to grow buffer by.
/// @param[in] allocator Pointer to allocator interface.
/// @return
///     - @c true  : Reallocated hashmap buffer successfully.
///     - @c false : Failed to reallocate hashmap buffer.
attr_core_api
b32 hashmap_grow(
    Hashmap* map, u32 amount, struct AllocatorInterface* allocator );
/// @brief Attempt to insert new value in hashmap.
/// @param[in] map  Hashmap.
/// @param     key  Key of value.
/// @param[in] item Pointer to item to insert.
/// @return
///     - @c true  : Hashmap had enough capacity to insert new item.
///     - @c false : Hashmap is full.
attr_core_api
b32 hashmap_try_insert( Hashmap* map, hash64 key, const void* item );
/// @brief Attempt to insert new value in hashmap.
/// @note requires string.h
/// @param[in] map  (Hashmap*)    Hashmap.
/// @param     key  (String)      Key of value as a string.
/// @param[in] item (const void*) Pointer to item to insert.
/// @return
///     - @c true  : Hashmap had enough capacity to insert new item.
///     - @c false : Hashmap is full.
#define hashmap_try_insert_string( map, key, item )\
    hashmap_try_insert( map, hash_string_64( key ), item )
/// @brief Attempt to insert new value in hashmap.
/// @note requires string.h
/// @param[in] map  (Hashmap*)       Hashmap.
/// @param     key  (string literal) Key of value as a string literal.
/// @param[in] item (const void*)    Pointer to item to insert.
/// @return
///     - @c true  : Hashmap had enough capacity to insert new item.
///     - @c false : Hashmap is full.
#define hashmap_try_insert_text( map, key, item )\
    hashmap_try_insert( map, hash_text_64( key ), item )
/// @brief Insert new value in hashmap.
/// @param[in] map       Hashmap.
/// @param     key       Key of value.
/// @param[in] item      Pointer to item to insert.
/// @param[in] allocator Pointer to allocator interface.
/// @return
///     - @c true  : New item inserted. If reallocated, it was successful.
///     - @c false : Failed to reallocate hashmap buffer.
attr_core_api
b32 hashmap_insert(
    Hashmap* map, hash64 key, const void* item,
    struct AllocatorInterface* allocator );
/// @brief Insert new value in hashmap.
/// @note requires string.h
/// @param[in] map       (Hashmap*)            Hashmap.
/// @param     key       (String)              Key of value as a string.
/// @param[in] item      (const void*)         Pointer to item to insert.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator interface.
/// @return
///     - @c true  : New item inserted. If reallocated, it was successful.
///     - @c false : Failed to reallocate hashmap buffer.
#define hashmap_insert_string( map, key, item, allocator )\
    hashmap_insert( map, hash_string_64( key ), item, allocator )
/// @brief Insert new value in hashmap.
/// @note requires string.h
/// @param[in] map       (Hashmap*)            Hashmap.
/// @param     key       (string literal)      Key of value as a string literal.
/// @param[in] item      (const void*)         Pointer to item to insert.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator interface.
/// @return
///     - @c true  : New item inserted. If reallocated, it was successful.
///     - @c false : Failed to reallocate hashmap buffer.
#define hashmap_insert_text( map, key, item, allocator )\
    hashmap_insert( map, hash_text_64( key ), item, allocator )
/// @brief Remove item from hashmap.
/// @param[in]  map          Hashmap.
/// @param      key          Key of value to remove.
/// @param[out] opt_out_item (optional) Pointer to write value of removed item to.
/// @return
///     - @c true  : Found item and removed it.
///     - @c false : @c key does not exist.
attr_core_api
b32 hashmap_remove( Hashmap* map, hash64 key, void* opt_out_item );
/// @brief Remove item from hashmap.
/// @note requires string.h
/// @param[in]  map          (Hashmap*) Hashmap.
/// @param      key          (String)   Key of value as a string.
/// @param[out] opt_out_item (void*)    (optional) Pointer to write value of removed item to.
/// @return
///     - @c true  : Found item and removed it.
///     - @c false : @c key does not exist.
#define hashmap_remove_string( map, key, opt_out_item )\
    hashmap_remove( map, hash_string_64( key ), opt_out_item )
/// @brief Remove item from hashmap.
/// @note requires string.h
/// @param[in]  map          (Hashmap*)       Hashmap.
/// @param      key          (string literal) Key of value as a string literal.
/// @param[out] opt_out_item (void*)          (optional) Pointer to write value of removed item to.
/// @return
///     - @c true  : Found item and removed it.
///     - @c false : @c key does not exist.
#define hashmap_remove_text( map, key, opt_out_item )\
    hashmap_remove( map, hash_text_64( key ), opt_out_item )
/// @brief Get pointer to item in hashmap.
/// @param[in] map Hashmap.
/// @param     key Key of value to get.
/// @return
///     - NULL    : @c key does not exist.
///     - Pointer : Pointer to item.
attr_core_api
void* hashmap_index_ref( Hashmap* map, hash64 key );
/// @brief Get pointer to item in hashmap.
/// @note requires string.h
/// @param[in] map (Hashmap*) Hashmap.
/// @param     key (String)   Key as a string.
/// @return (void*)
///     - NULL    : @c key does not exist.
///     - Pointer : Pointer to item.
#define hashmap_index_ref_string( map, key )\
    hashmap_index_ref( map, hash_string_64( key ) )
/// @brief Get pointer to item in hashmap.
/// @note requires string.h
/// @param[in] map (Hashmap*)       Hashmap.
/// @param     key (string literal) Key as a string literal.
/// @return (void*)
///     - NULL    : @c key does not exist.
///     - Pointer : Pointer to item.
#define hashmap_index_ref_text( map, key )\
    hashmap_index_ref( map, hash_text_64( key ) )
/// @brief Get item in hashmap.
/// @param[in]  map      Hashmap.
/// @param      key      Key of value to get.
/// @param[out] out_item Pointer to write item to.
/// @return
///     - @c true  : Found item and copied to @c out_item.
///     - @c false : @c key does not exist.
attr_core_api
b32 hashmap_index( Hashmap* map, hash64 key, void* out_item );
/// @brief Get item in hashmap.
/// @note requires string.h
/// @param[in]  map      (Hashmap*) Hashmap.
/// @param      key      (String)   Key as a string.
/// @param[out] out_item (void*)    Pointer to write item to.
/// @return
///     - @c true  : Found item and copied to @c out_item.
///     - @c false : @c key does not exist.
#define hashmap_index_string( map, key, out_item )\
    hashmap_index( map, hash_string_64( key ), out_item )
/// @brief Get item in hashmap.
/// @note requires string.h
/// @param[in]  map      (Hashmap*)       Hashmap.
/// @param      key      (string literal) Key as a string literal.
/// @param[out] out_item (void*)          Pointer to write item to.
/// @return
///     - @c true  : Found item and copied to @c out_item.
///     - @c false : @c key does not exist.
#define hashmap_index_text( map, key, out_item )\
    hashmap_index( map, hash_text_64( key ), out_item )
/// @brief Check if hashmap contains key.
/// @param[in] map Hashmap.
/// @param     key Key of value to check for.
/// @return
///     - @c true  : Hashmap contains key.
///     - @c false : Key does not exist.
attr_always_inline attr_header
b32 hashmap_contains( Hashmap* map, hash64 key ) {
    return hashmap_index_ref( map, key ) != NULL;
}
/// @brief Check if hashmap contains key.
/// @note requires string.h
/// @param[in] map (Hashmap*) Hashmap.
/// @param     key (String)   Key as a string.
/// @return
///     - @c true  : Hashmap contains key.
///     - @c false : Key does not exist.
#define hashmap_contains_string( map, key )\
    hashmap_contains( map, hash_string_64( key ) )
/// @brief Check if hashmap contains key.
/// @note requires string.h
/// @param[in] map (Hashmap*)       Hashmap.
/// @param     key (string literal) Key as a string literal.
/// @return
///     - @c true  : Hashmap contains key.
///     - @c false : Key does not exist.
#define hashmap_contains_text( map, key )\
    hashmap_contains( map, hash_text_64( key ) )
/// @brief Set value of key.
/// @param[in] map  Hashmap.
/// @param     key  Key of value to set.
/// @param[in] item Pointer to copy to value.
/// @return
///     - @c true  : Found value and copied @c item to it.
///     - @c false : @c key does not exist.
attr_core_api
b32 hashmap_set( Hashmap* map, hash64 key, const void* item );
/// @brief Set value of key.
/// @note requires string.h
/// @param[in] map  (Hashmap*)    Hashmap.
/// @param     key  (String)      Key as a string.
/// @param[in] item (const void*) Pointer to copy to value.
/// @return
///     - @c true  : Found value and copied @c item to it.
///     - @c false : @c key does not exist.
#define hashmap_set_string( map, key, item )\
    hashmap_set( map, hash_string_64(key), item )
/// @brief Set value of key.
/// @note requires string.h
/// @param[in] map  (Hashmap*)       Hashmap.
/// @param     key  (string literal) Key as a string literal.
/// @param[in] item (const void*)    Pointer to copy to value.
/// @return
///     - @c true  : Found value and copied @c item to it.
///     - @c false : @c key does not exist.
#define hashmap_set_text( map, key, item )\
    hashmap_set( map, hash_text_64(key), item )

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#endif /* header guard */
