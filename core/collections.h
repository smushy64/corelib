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
#include "core/slice.h"
#include "core/string.h"

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

/// @brief First in, First Out buffer.
/// @see #GenericBuffer
typedef struct GenericBuffer Stack;
/// @brief Generic list of items.
/// @see #GenericBuffer
typedef struct GenericBuffer List;
/// @brief First in, Last Out buffer.
typedef struct Queue {
    /// @brief Union of fields in common with GenericSlice.
    union {
        /// @brief Buffer, stride and length.
        struct {
            /// @brief Pointer to buffer.
            void* buf;
            /// @brief Size of each item in buffer.
            usize stride;
            /// @brief Number of items in queue.
            usize len;
        };
        /// @brief %Queue as a slice.
        struct GenericSlice slice;
    };
    /// @brief Number of items that queue can hold.
    usize cap;
    /// @brief Front of queue.
    isize front;
    /// @brief Back of queue.
    isize back;
} Queue;
/// @brief %Hashmap buffer.
typedef struct Hashmap {
    /// @brief Union of fields in common with GenericSlice.
    union {
        /// @brief Buffer, stride and length.
        struct {
            /// @brief Pointer to items buffer.
            void* buf;
            /// @brief Size of each item in buffer.
            usize stride;
            /// @brief Number of items in hashmap.
            usize len;
        };
        /// @brief %Hashmap as a slice.
        struct GenericSlice slice;
    };
    /// @brief Number of items hashmap can hold.
    usize cap;
    /// @brief Pointer to keys buffer.
    u64*  keys;
    /// @brief Largest key value. Used for search algorithm.
    u64   largest_key;
} Hashmap;
/// @brief Calculate number of bytes required to create packed bool list for given boolean count.
/// @param boolean_count (usize) Number of booleans list should be able to hold.
/// @return (usize) Bytes required to store given boolean count.
#define packed_bool_memory_requirement( boolean_count )\
    ( ((boolean_count) / 8) + (((boolean_count) % 8) ? 1 : 0 ) )
/// @brief Calculate number of booleans can be packed in given number of bytes.
/// @param byte_count (usize) Number of bytes that boolean array holds.
/// @return (usize) Number of booleans that can be packed into number of bytes provided.
#define packed_bool_cap( byte_count )\
    ( byte_count * 8 )
/// @brief Set packed boolean in byte array.
/// @param[in] bytes (b8[]) Byte array holding packed booleans.
/// @param index (usize) Index of boolean in byte array.
/// @param value (b8) Value to set boolean to.
#define packed_bool_set( bytes, index, value ) do {\
    b8* at = (bytes) + ((index) / 8);\
    b8  bitfield = (1 << ((index) % 8));\
    if( (value) ) {\
        *at |= bitfield;\
    } else {\
        *at &= ~bitfield;\
    }\
} while(0)
/// @brief Get value of packed boolean in byte array.
/// @param[in] bytes (b8[]) Byte array holding packed booleans.
/// @param index (usize) Index of boolean in packed byte array.
/// @return (b8) Value of boolean at given index in packed boolean byte array.
#define packed_bool_get( bytes, index )\
    (( (bytes)[(index) / 8] & (1 << ((index) % 8)) ) == (1 << ((index) % 8)))
/// @brief Create a new stack.
/// @param item_size Size of each item in stack.
/// @param cap Number of items stack buffer can hold.
/// @param[in] buffer Pointer to start of stack buffer.
/// @return New stack.
attr_always_inline
attr_header Stack stack_new( usize item_size, usize cap, void* buffer ) {
    Stack stack;
    stack.buf    = buffer;
    stack.stride = item_size;
    stack.cap    = cap;
    stack.len    = 0;
    return stack;
}
/// @brief Check if stack is empty.
/// @param[in] stack Stack to check.
/// @return True if stack is empty.
attr_always_inline
attr_header b32 stack_is_empty( const Stack* stack ) {
    return stack->len == 0;
}
/// @brief Check if stack is full.
/// @param[in] stack Stack to check.
/// @return True if stack is full.
attr_always_inline
attr_header b32 stack_is_full( const Stack* stack ) {
    return stack->len == stack->cap;
}
/// @brief Reset stack.
///
/// Sets stack.len to zero.
/// @param[in] stack Pointer to stack to reset.
attr_always_inline
attr_header void stack_reset( Stack* stack ) {
    stack->len = 0;
}
/// @brief Rebase stack buffer pointer after reallocation.
/// @param[in] stack Stack to rebase.
/// @param new_cap Capacity of new buffer.
/// @param[in] new_buffer Pointer to new buffer.
attr_always_inline
attr_header void stack_rebase( Stack* stack, usize new_cap, void* new_buffer ) {
    stack->cap = new_cap;
    stack->buf = new_buffer;
}
/// @brief Push new item to the top of the stack.
/// @param[in] stack Stack to push to.
/// @param[in] item Pointer to item, size must be equal to stack.stride.
/// @return True if there was enough space in stack to push.
attr_core_api b32 stack_push( Stack* stack, void* item );
/// @brief Pop top element from stack and get pointer to it.
/// @param[in] stack Stack to pop from.
/// @param[out] opt_out_item (optional) Pointer to pointer to receive pointer to item.
/// @return True if stack had an item to pop.
attr_core_api b32 stack_pop_ref( Stack* stack, void** opt_out_item );
/// @brief Pop top element from stack and copy value to buffer.
/// @param[in] stack Stack to pop from.
/// @param[out] opt_out_item (optional) Pointer to buffer to receive item, must be able to hold stack.stride.
/// @return True if stack had an item to pop.
attr_core_api b32 stack_pop( Stack* stack, void* opt_out_item );

/// @brief Create a new list.
/// @param item_size Size of each item in list.
/// @param cap Number of items list buffer can hold.
/// @param[in] buffer Pointer to start of list buffer.
/// @return New list.
attr_always_inline
attr_header List list_new( usize item_size, usize cap, void* buffer ) {
    List list;
    list.buf    = buffer;
    list.stride = item_size;
    list.cap    = cap;
    list.len    = 0;
    return list;
}
/// @brief Check if list is empty.
/// @param[in] list List to check.
/// @return True if list is empty.
attr_always_inline
attr_header b32 list_is_empty( const List* list ) {
    return list->len == 0;
}
/// @brief Check if list is full.
/// @param[in] list List to check.
/// @return True if list is full.
attr_always_inline
attr_header b32 list_is_full( const List* list ) {
    return list->len == list->cap;
}
/// @brief Reset list.
///
/// Sets list.len to zero.
/// @param[in] list Pointer to list to reset.
attr_always_inline
attr_header void list_reset( List* list ) {
    list->len = 0;
}
/// @brief Rebase list buffer pointer after reallocation.
/// @param[in] list List to rebase.
/// @param new_cap Capacity of new buffer.
/// @param[in] new_buffer Pointer to new buffer.
attr_always_inline
attr_header void list_rebase( List* list, usize new_cap, void* new_buffer ) {
    list->cap = new_cap;
    list->buf = new_buffer;
}
/// @brief Get pointer to item in list by index.
/// @param[in] list List to get item from.
/// @param index Index of item.
/// @return Pointer to item, NULL if index is out of bounds.
attr_always_inline
attr_header void* list_get_ref( List* list, usize index ) {
    return (void*)((u8*)list->buf + (list->stride * index));
}
/// @brief Copy item at index to out pointer.
/// @param[in] list List to get item from.
/// @param index Index of item.
/// @param[out] out_item Pointer to copy item to, must be able to hold list.stride bytes.
attr_core_api void list_get( const List* list, usize index, void* out_item );
/// @brief Get pointer to item in list by index.
/// @brief Checks if index is in bounds.
/// @param[in] list List to get item from.
/// @param index Index of item.
/// @return Pointer to item, NULL if index is out of bounds.
attr_always_inline
attr_header void* list_get_ref_checked( List* list, usize index ) {
    if( index >= list->len ) {
        return NULL;
    }
    return list_get_ref( list, index );
}
/// @brief Copy item at index to out pointer.
/// @brief Checks if index is in bounds.
/// @param[in] list List to get item from.
/// @param index Index of item.
/// @param[out] out_item Pointer to copy item to, must be able to hold list.stride bytes.
/// @return True if index is within bounds.
attr_always_inline
attr_header b32 list_get_checked( const List* list, usize index, void* out_item ) {
    if( index >= list->len ) {
        return false;
    }
    list_get( list, index, out_item );
    return true;
}
/// @brief Append items to end of list.
/// @param[in] list List to append to.
/// @param count Number of items to append.
/// @param[in] items Items to copy to end of list.
/// @return True if list has space for items.
attr_core_api b32 list_append( List* list, usize count, const void* items );
/// @brief Prepend items to start of list.
/// @param[in] list List to prepend to.
/// @param count Number of items to prepend.
/// @param[in] items Items to copy to start of list.
/// @return True if list has space for items.
attr_core_api b32 list_prepend( List* list, usize count, const void* items );
/// @brief Insert item into list at given index.
/// @brief Items from that index to end of list are moved forward and item is copied to index.
/// @param[in] list List to insert into.
/// @param index Index to insert into.
/// @param[in] item Pointer to item to copy.
/// @return True if list has space for new item.
attr_core_api b32 list_insert( List* list, usize index, const void* item );
/// @brief Overwrite all values in list up to capacity with given value.
/// @param[in] list List to fill.
/// @param[in] value Value to fill with.
attr_core_api void list_fill( List* list, const void* value );
/// @brief Push new item to the top of the list.
/// @param[in] list List to push to.
/// @param[in] item Pointer to item, size must be equal to list.stride.
/// @return True if there was enough space in list to push.
attr_always_inline
attr_header b32 list_push( List* list, void* item ) {
    return stack_push( list, item );
}
/// @brief Pop top element from list and get pointer to it.
/// @param[in] list List to pop from.
/// @param[out] opt_out_item (optional) Pointer to pointer to receive pointer to item.
/// @return True if list had an item to pop.
attr_always_inline
attr_header b32 list_pop_ref( List* list, void** opt_out_item ) {
    return stack_pop_ref( list, opt_out_item );
}
/// @brief Pop top element from list and copy value to buffer.
/// @param[in] list List to pop from.
/// @param[out] opt_out_item (optional) Pointer to buffer to receive item, must be able to hold list.stride.
/// @return True if list had an item to pop.
attr_always_inline
attr_header b32 list_pop( List* list, void* opt_out_item ) {
    return stack_pop( list, opt_out_item );
}

/// @brief Create a new queue.
/// @param item_size Size of each item in queue.
/// @param cap Number of items queue buffer can hold.
/// @param[in] buffer Pointer to buffer, must be able to hold item_size * cap bytes.
/// @return New queue.
attr_always_inline
attr_header Queue queue_new( usize item_size, usize cap, void* buffer ) {
    Queue queue;
    queue.buf    = buffer;
    queue.cap    = cap;
    queue.stride = item_size;
    queue.front  = -1;
    queue.back   = 0;
    queue.len    = 0;
    return queue;
}
/// @brief Check if queue is empty.
/// @param[in] queue Stack to check.
/// @return True if queue is empty.
attr_always_inline
attr_header b32 queue_is_empty( const Queue* queue ) {
    return queue->len == 0;
}
/// @brief Check if queue is full.
/// @param[in] queue Queue to check.
/// @return True if queue is full.
attr_always_inline
attr_header b32 queue_is_full( const Queue* queue ) {
    return queue->len == queue->cap;
}
/// @brief Reset a queue.
/// @param[in] queue Queue to reset.
attr_always_inline
attr_header void queue_reset( Queue* queue ) {
    queue->front = -1;
    queue->back  = 0;
    queue->len   = 0;
}
// TODO(alicia): queue rebase

/// @brief Enqueue an item into queue.
/// @param[in] queue Queue to enqueue item into.
/// @param[in] item Pointer to item to enqueue.
/// @return True if queue had enough capacity to enqueue a new item.
attr_core_api b32 queue_enqueue( Queue* queue, void* item );
/// @brief Dequeue an item from queue and get pointer to it.
/// @param[in] queue Queue to dequeue from.
/// @param[out] opt_out_item (optional) Pointer to pointer to receive pointer to item.
/// @return True if queue has an item to dequeue.
attr_core_api b32 queue_dequeue_ref( Queue* queue, void** opt_out_item );
/// @brief Dequeue an item from queue and copy its value to buffer.
/// @param[in] queue Queue to dequeue from.
/// @param[out] opt_out_item (optional) Pointer to buffer to copy item to.
/// @return True if queue has an item to dequeue.
attr_core_api b32 queue_dequeue( Queue* queue, void* opt_out_item );

/// @brief Create a new hashmap.
/// @param stride Size of each item in hashmap.
/// @param capacity Number of items hashmap buffer can hold.
/// @param[in] items_buffer Buffer for items, must be able to hold stride * capacity bytes.
/// @param[in] keys_buffer Buffer for keys, must be able to hold capacity.
/// @return New hashmap.
attr_always_inline
attr_header Hashmap hashmap_new(
    usize stride, usize capacity, void* items_buffer, u64* keys_buffer
) {
    Hashmap map;
    map.buf         = items_buffer;
    map.keys        = keys_buffer;
    map.cap         = capacity;
    map.stride      = stride;
    map.largest_key = 0;
    map.len         = 0;
    return map;
}
/// @brief Check if hashmap is empty.
/// @param[in] hashmap Hashmap to check.
/// @return True if hashmap is empty.
attr_always_inline
attr_header b32 hashmap_is_empty( const Hashmap* hashmap ) {
    return hashmap->len == 0;
}
/// @brief Check if hashmap is full.
/// @param[in] hashmap Hashmap to check.
/// @return True if hashmap is full.
attr_always_inline
attr_header b32 hashmap_is_full( const Hashmap* hashmap ) {
    return hashmap->len == hashmap->cap;
}
/// @brief Reset a hashmap.
/// @param[in] hashmap Hashmap to reset.
attr_always_inline
attr_header void hashmap_reset( Hashmap* hashmap ) {
    hashmap->largest_key = 0;
    hashmap->len         = 0;
}
/// @brief Convert string to hashmap key.
/// @see #string_hash().
/// @param s String to hash.
/// @return Hash of string.
attr_always_inline
attr_header u64 hashmap_string_to_key( const String s ) {
    return string_hash( s );
}
/// @brief Check if hashmap contains key.
/// @param[in] hashmap Hashmap to check for key.
/// @param key Key to search for.
/// @return True if hashmap contains key.
attr_core_api b32 hashmap_contains_key( const Hashmap* hashmap, u64 key );
/// @brief Insert item into hashmap with given key.
/// @param[in] hashmap Hashmap to insert key value pair into.
/// @param key Key of value.
/// @param[in] value Pointer to value to insert.
/// @return True if hashmap has space for new value.
attr_core_api b32 hashmap_insert( Hashmap* hashmap, u64 key, const void* value );
/// @brief Insert item into hashmap with given key.
/// @brief Checks if key already exists in hashmap before inserting.
/// @param[in] hashmap Hashmap to insert key value pair into.
/// @param key Key of value.
/// @param[in] value Pointer to value to insert.
/// @return True if hashmap has space for new value and key is unique.
attr_always_inline
attr_header b32 hashmap_insert_checked(
    Hashmap* hashmap, u64 key, const void* value
) {
    if( hashmap_contains_key( hashmap, key ) ) {
        return false;
    }
    return hashmap_insert( hashmap, key, value );
}
/// @brief Remove item with given key from hashmap.
/// @param[in] hashmap Hashmap to remove key value pair from.
/// @param key Key of item to remove.
/// @param[out] opt_out_item (optional) Pointer to receive value.
/// @return True if hashmap contained key value pair being removed.
attr_core_api b32 hashmap_remove( Hashmap* hashmap, u64 key, void* opt_out_item );
/// @brief Get pointer to item with given key.
/// @param[in] hashmap Hashmap to get item from.
/// @param key Key of item.
/// @param[out] out_item Pointer to receive pointer to item.
/// @return True if hashmap contained key.
attr_core_api b32 hashmap_get_ref( Hashmap* hashmap, u64 key, void** out_item );
/// @brief Get pointer to item with given key and copy to buffer.
/// @param[in] hashmap Hashmap to get item from.
/// @param key Key of item.
/// @param[out] out_item Pointer to copy item to.
/// @return True if hashmap contained key.
attr_core_api b32 hashmap_get( Hashmap* hashmap, u64 key, void* out_item );
/// @brief Set value of key value pair.
/// @param[in] hashmap Hashmap to modify.
/// @param key Key of item.
/// @param[in] new_value Pointer to new item value.
/// @return True if hashmap contained key.
attr_core_api b32 hashmap_set( Hashmap* hashmap, u64 key, const void* new_value );

/// @brief Insert item into hashmap with given key.
/// @param[in] hashmap Hashmap to insert key value pair into.
/// @param string_key String key of item.
/// @param[in] value Pointer to value to insert.
/// @return True if hashmap has space for new value.
attr_always_inline
attr_header b32 hashmap_insert_by_string(
    Hashmap* hashmap, const String string_key, const void* value
) {
    u64 key = hashmap_string_to_key( string_key );
    return hashmap_insert( hashmap, key, value );
}
/// @brief Insert item into hashmap with given key.
/// @brief Checks if key already exists in hashmap before inserting.
/// @param[in] hashmap Hashmap to insert key value pair into.
/// @param string_key String key of item.
/// @param[in] value Pointer to value to insert.
/// @return True if hashmap has space for new value and key is unique.
attr_always_inline
attr_header b32 hashmap_insert_checked_by_string(
    Hashmap* hashmap, const String string_key, const void* value
) {
    u64 key = hashmap_string_to_key( string_key );
    return hashmap_insert_checked( hashmap, key, value );
}
/// @brief Check if hashmap contains key.
/// @param[in] hashmap Hashmap to check for key.
/// @param string_key String key of item.
/// @return True if hashmap contains key.
attr_always_inline
attr_header b32 hashmap_contains_key_string(
    const Hashmap* hashmap, const String string_key
) {
    u64 key = hashmap_string_to_key( string_key );
    return hashmap_contains_key( hashmap, key );
}
/// @brief Get pointer to item with given key.
/// @param[in] hashmap Hashmap to get item from.
/// @param string_key String key of item.
/// @param[out] out_item Pointer to receive pointer to item.
/// @return True if hashmap contained key.
attr_always_inline
attr_header b32 hashmap_get_ref_by_string(
    Hashmap* hashmap, const String string_key, void** out_item
) {
    u64 key = hashmap_string_to_key( string_key );
    return hashmap_get_ref( hashmap, key, out_item );
}
/// @brief Get pointer to item with given key and copy to buffer.
/// @param[in] hashmap Hashmap to get item from.
/// @param string_key String key of item.
/// @param[out] out_item Pointer to copy item to.
/// @return True if hashmap contained key.
attr_always_inline
attr_header b32 hashmap_get_by_string(
    Hashmap* hashmap, const String string_key, void* out_item
) {
    u64 key = hashmap_string_to_key( string_key );
    return hashmap_get( hashmap, key, out_item );
}
/// @brief Set value of key value pair.
/// @param[in] hashmap Hashmap to modify.
/// @param string_key String key of item.
/// @param[in] new_value Pointer to new item value.
/// @return True if hashmap contained key.
attr_always_inline
attr_header b32 hashmap_set_by_string(
    Hashmap* hashmap, const String string_key, const void* new_value
) {
    u64 key = hashmap_string_to_key( string_key );
    return hashmap_set( hashmap, key, new_value );
}
/// @brief Remove item with given key from hashmap.
/// @param[in] hashmap Hashmap to remove key value pair from.
/// @param string_key String key of item to remove.
/// @param[out] opt_out_item (optional) Pointer to receive value.
/// @return True if hashmap contained key value pair being removed.
attr_always_inline
attr_header b32 hashmap_remove_by_string(
    Hashmap* hashmap, const String string_key, void* opt_out_item
) {
    u64 key = hashmap_string_to_key( string_key );
    return hashmap_remove( hashmap, key, opt_out_item );
}

#if defined(CORE_CPLUSPLUS) && defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
    #pragma clang diagnostic pop
#endif

#endif /* header guard */
