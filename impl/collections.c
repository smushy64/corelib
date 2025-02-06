/**
 * Description:  Collection types and function implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 19, 2024
*/
#include "core/prelude.h"
#include "core/collections.h"
#include "core/memory.h"
#include "core/alloc.h"

attr_core_api b32 generic_array_from_alloc(
    usize stride, usize cap, void** buf, struct AllocatorInterface* allocator
) {
    void* _buf = allocator_alloc( allocator, stride * cap );
    if( !_buf ) {
        return false;
    }
    *buf = _buf;
    return true;
}
attr_core_api b32 generic_array_try_push(
    usize stride, usize* len, usize cap, void* buf, const void* item
) {
    if( *len == cap ) {
        return false;
    }

    memory_copy( (u8*)buf + (stride * *len), item, stride );
    *len = *len + 1;

    return true;
}
attr_core_api b32 generic_array_try_emplace(
    usize stride, usize* len, usize cap, void* buf, const void* item, usize at
) {
    if( *len == cap ) {
        return false;
    }

    usize move = *len - at;
    memory_move(
        (u8*)buf + (stride * (at + 1)),
        (u8*)buf + (stride * at),
        stride * move );
    memory_copy( (u8*)buf + (stride * at), item, stride );
    
    *len = *len + 1;

    return true;
}
attr_core_api b32 generic_array_try_insert(
    usize stride, usize* len, usize cap, void* buf,
    usize insert_len, const void* insert, usize at
) {
    if( *len + insert_len > cap ) {
        return false;
    }

    usize move = *len - at;
    memory_move(
        (u8*)buf + (stride * (at + insert_len)),
        (u8*)buf + (stride * at),
        stride * move );
    memory_copy( (u8*)buf + (stride * at), insert, stride * insert_len );
    *len = *len + insert_len;

    return true;
}
attr_core_api b32 generic_array_grow(
    usize stride, usize* cap, void** buf,
    usize amount, struct AllocatorInterface* allocator
) {
    if( !amount ) {
        return true;
    }
    usize old_size = stride * *cap;
    usize new_size = old_size + (stride * amount);
    void* new_buf  = allocator_realloc( allocator, *buf, old_size, new_size );
    if( !new_buf ) {
        return false;
    }
    *buf = new_buf;
    *cap = *cap + amount;
    return true;
}
attr_core_api b32 generic_array_push(
    usize stride, usize* len, usize* cap, void** buf,
    const void* item, struct AllocatorInterface* allocator
) {
    if( generic_array_try_push( stride, len, *cap, *buf, item ) ) {
        return true;
    }
    if( !generic_array_grow( stride, cap, buf, *cap, allocator ) ) {
        return false;
    }
    return generic_array_try_push( stride, len, *cap, *buf, item );
}
attr_core_api b32 generic_array_emplace(
    usize stride, usize* len, usize* cap, void** buf,
    const void* item, usize at, struct AllocatorInterface* allocator
) {
    if( generic_array_try_emplace( stride, len, *cap, *buf, item, at ) ) {
        return true;
    }
    if( !generic_array_grow( stride, cap, buf, *cap, allocator ) ) {
        return false;
    }
    return generic_array_try_emplace( stride, len, *cap, *buf, item, at );
}
attr_core_api b32 generic_array_insert(
    usize stride, usize* len, usize* cap, void** buf,
    usize insert_len, const void* insert, usize at,
    struct AllocatorInterface* allocator
) {
    if( generic_array_try_insert(
        stride, len, *cap, *buf, insert_len, insert, at
    ) ) {
        return true;
    }
    if( !generic_array_grow( stride, cap, buf, *cap, allocator ) ) {
        return false;
    }
    return generic_array_try_insert(
        stride, len, *cap, *buf, insert_len, insert, at );
}
attr_core_api b32 generic_array_pop(
    usize stride, usize* len, void* buf, void* opt_out_item
) {
    if( !*len ) {
        return false;
    }

    u8* at = (u8*)buf + (stride * *len);

    if( opt_out_item ) {
        memory_copy( opt_out_item, at, stride );
    }
    memory_zero( at, stride );
    *len = *len - 1;

    return true;
}
attr_core_api b32 generic_array_remove(
    usize stride, usize* len, void* buf, usize at
) {
    if( !*len ) {
        return false;
    }
    if( at == (*len - 1) ) {
        return generic_array_pop( stride, len, buf, 0 );
    }

    usize move = *len - (at + 1);
    memory_move(
        (u8*)buf + (stride * at),
        (u8*)buf + (stride * (at + 1)),
        stride * move );
    *len = *len - 1;
    memory_zero( (u8*)buf + (stride * *len), stride );

    return true;
}
attr_core_api b32 generic_array_remove_range(
    usize stride, usize* len, void* buf,
    usize from_inclusive, usize to_exclusive
) {
    if( from_inclusive >= to_exclusive ) {
        return false;
    }
    if( !*len || from_inclusive >= *len || to_exclusive > *len ) {
        return false;
    }

    // TODO(alicia): rewrite this >:(
    for( usize i = from_inclusive; i < to_exclusive; ++i ) {
        generic_array_remove( stride, len, buf, from_inclusive );
    }

    return true;
}
attr_core_api b32 generic_array_clone(
    usize stride, usize len, void* buf,
    usize* out_len, usize* out_cap, void** out_res,
    struct AllocatorInterface* allocator
) {
    usize dst_cap  = len + 8;
    usize src_size = stride * len;
    usize dst_size = stride * dst_cap;
    void* dst_buf  = allocator_alloc( allocator, dst_size );
    if( !dst_buf ) {
        return false;
    }

    memory_copy( dst_buf, buf, src_size );
    *out_len = len;
    *out_cap = dst_cap;
    *out_res = dst_buf;

    return true;
}
attr_core_api b32 queue_from_alloc(
    usize stride, usize cap, Queue* out_queue, struct AllocatorInterface* allocator
) {
    void* buf = allocator_alloc( allocator, stride * cap );
    if( !buf ) {
        return false;
    }

    out_queue->buf    = buf;
    out_queue->cap    = cap;
    out_queue->stride = stride;
    out_queue->head   = 0;
    out_queue->tail   = 0;

    return true;
}
attr_core_api void queue_free( Queue* queue, struct AllocatorInterface* allocator ) {
    if( queue && queue->buf ) {
        allocator_free( allocator, queue, queue->stride * queue->cap );
        memory_zero( queue, sizeof(*queue) );
    }
}
attr_core_api b32 queue_grow(
    Queue* queue, usize amount, struct AllocatorInterface* allocator
) {
    // TODO(alicia): do this in place >:(
    Queue new_queue = queue_empty();
    if( !queue_from_alloc(
        queue->stride, queue->cap + amount,
        &new_queue, allocator
    ) ) {
        return false;
    }

    void* item = 0;
    while( queue_dequeue_ref( queue, &item ) ) {
        queue_try_enqueue( &new_queue, item );
    }

    queue_free( queue, allocator );
    *queue = new_queue;

    return true;
}
attr_core_api b32 queue_try_enqueue( Queue* queue, const void* item ) {
    if( queue_is_full( queue ) ) {
        return false;
    }
    u32 index = queue->tail++ % queue->cap;
    memory_copy( (u8*)queue->buf + (queue->stride * index), item, queue->stride );
    return true;
}
attr_core_api b32 queue_enqueue(
    Queue* queue, const void* item, struct AllocatorInterface* allocator
) {
    if( queue_try_enqueue( queue, item ) ) {
        return true;
    }
    if( !queue_grow( queue, queue->cap, allocator ) ) {
        return false;
    }
    return queue_try_enqueue( queue, item );
}
attr_core_api b32 queue_dequeue_ref( Queue* queue, void** out_item ) {
    if( queue_is_empty( queue ) ) {
        return false;
    }
    u32 index = queue->head++ % queue->cap;
    *out_item = (u8*)queue->buf + (queue->stride * index);
    return true;
}
attr_core_api b32 queue_dequeue( Queue* queue, void* opt_out_item ) {
    void* item = NULL;
    if( !queue_dequeue_ref( queue, &item ) ) {
        return false;
    }
    if( opt_out_item ) {
        memory_copy( opt_out_item, item, queue->stride );
    }
    memory_zero( item, queue->stride );
    return true;
}
attr_core_api b32 queue_peek( Queue* queue, void** out_item ) {
    if( queue_is_empty( queue ) ) {
        return false;
    }
    u32 index = queue->head % queue->cap;
    *out_item = (u8*)queue->buf + (queue->stride * index);
    return true;
}
attr_core_api void queue_clear( Queue* queue ) {
    memory_zero( queue->buf, queue->stride * queue->cap );
    *queue = queue_new( queue->stride, queue->cap, queue->buf );
}

attr_core_api b32 hashmap_from_alloc(
    usize stride, usize cap, Hashmap* out_map,
    struct AllocatorInterface* allocator
) {
    void* buf = allocator_alloc( allocator, (stride * cap) + (sizeof(hash64) * cap) );
    if( !buf ) {
        return false;
    }
    *out_map = hashmap_new( stride, cap, buf );
    return true;
}
attr_core_api void hashmap_free(
    Hashmap* map, struct AllocatorInterface* allocator
) {
    if( map && map->buf ) {
        allocator_free(
            allocator, map->buf, (map->stride * map->cap) + (sizeof(hash64) * map->cap) );
        memory_zero( map, sizeof(*map) );
    }
}
attr_core_api void hashmap_clear( Hashmap* map ) {
    memory_zero( map->buf, (map->stride * map->cap) + (sizeof(hash64) * map->cap) );
    *map = hashmap_new( map->stride, map->cap, map->buf );
}
attr_core_api b32 hashmap_grow(
    Hashmap* map, u32 amount, struct AllocatorInterface* allocator
) {
    usize old_size = (map->stride * map->cap) + (sizeof(hash64) * map->cap);
    usize new_size = old_size + ( (map->stride * amount) + (sizeof(hash64) * amount) );
    void* buf = allocator_realloc( allocator, map->buf, old_size, new_size );
    if( !buf ) {
        return false;
    }

    u8* keys_src = (u8*)buf + (map->stride * map->cap);
    u8* keys_dst = (u8*)buf + (map->stride * (map->cap + amount));

    memory_move( keys_dst, keys_src, sizeof(hash64) * map->cap );
    memory_zero( keys_src, keys_dst - keys_src );

    map->buf = buf;
    map->cap = map->cap + amount;

    return true;
}
attr_internal void internal_hashmap_push( Hashmap* map, hash64 key, const void* item ) {
    memory_copy( (u8*)map->buf + (map->stride * map->len), item, map->stride );
    hash64* keys = hashmap_keys( map );
    keys[map->len++] = key;

    if( key > map->largest_key ) {
        map->largest_key = key;
    }
}
attr_core_api b32 hashmap_try_insert( Hashmap* map, hash64 key, const void* item ) {
    if( hashmap_is_full( map ) ) {
        return false;
    }

    if( hashmap_is_empty( map ) || key > map->largest_key ) {
        internal_hashmap_push( map, key, item );
        return true;
    }

    hash64* keys = hashmap_keys( map );

    u32 idx = 0;
    u32 lo  = 0;
    u32 hi  = map->len;
    while( lo < hi ) {
        u32 mid = (lo + hi) >> 1;
        if( keys[mid] < key ) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
        idx = lo;
    }

    u32 copy_count = map->len - idx;
    if( !copy_count ) {
        internal_hashmap_push( map, key, item );
        return true;
    }

    u32 copy_size = sizeof(hash64) * copy_count;
    memory_move( keys + idx + 1, keys + idx, copy_size );
    keys[idx] = key;

    copy_size = map->stride * copy_count;
    memory_move(
        (u8*)map->buf + ( map->stride * (idx + 1) ),
        (u8*)map->buf + ( map->stride * idx ),
        copy_size );
    memory_copy( (u8*)map->buf + (map->stride * idx), item, map->stride );
    map->len++;

    if( key > map->largest_key ) {
        map->largest_key = key;
    }

    return true;
}
attr_core_api b32 hashmap_insert(
    Hashmap* map, hash64 key, const void* item,
    struct AllocatorInterface* allocator
) {
    if( hashmap_try_insert( map, key, item ) ) {
        return true;
    }
    if( !hashmap_grow( map, map->cap, allocator ) ) {
        return false;
    }
    return hashmap_try_insert( map, key, item );
}
attr_core_api b32 hashmap_remove( Hashmap* map, hash64 key, void* opt_out_item ) {
    void* item = hashmap_index_ref( map, key );
    if( !item ) {
        return false;
    }

    if( opt_out_item ) {
        memory_copy( opt_out_item, item, map->stride );
    }

    hash64* keys = hashmap_keys( map );

    usize   idx     = ((usize)item - (usize)map->buf) / map->stride;
    hash64* key_ptr = keys + idx;

    if( *key_ptr == map->largest_key ) {
        if( idx ) {
            map->largest_key = *(key_ptr - 1);
        } else {
            map->largest_key = 0;
        }
    }

    if( idx == (map->len - 1) ) {
        map->len--;
        return true;
    }

    usize copy_len = map->len - idx;

    u8* src = (u8*)item + map->stride;
    memory_move( item, src, map->stride * copy_len );

    src = (u8*)(key_ptr + 1);
    memory_move( key_ptr, src, sizeof(hash64) * copy_len );

    map->len--;
    return true;
}
attr_core_api void* hashmap_index_ref( Hashmap* map, hash64 key ) {
    if( hashmap_is_empty( map ) || key > map->largest_key ) {
        return NULL;
    }

    hash64* keys = hashmap_keys( map );

    u32 lo = 0;
    u32 hi = map->len;
    while( lo < hi ) {
        u32    mid = (lo + hi) >> 1;
        hash64 k   = keys[mid];

        if( k == key ) {
            return (u8*)map->buf + (map->stride * mid);
        } else if( k < key ) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    return NULL;
}
attr_core_api b32 hashmap_index( Hashmap* map, hash64 key, void* out_item ) {
    void* item = hashmap_index_ref( map, key );
    if( !item ) {
        return false;
    }
    memory_copy( out_item, item, map->stride );
    return true;
}
attr_core_api b32 hashmap_set( Hashmap* map, hash64 key, const void* item ) {
    void* at = hashmap_index_ref( map, key );
    if( !item ) {
        return false;
    }

    memory_copy( at, item, map->stride );
    return true;
}

