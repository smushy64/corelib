/**
 * Description:  Collection types and function implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 19, 2024
*/
#include "core/prelude.h"
#include "core/collections.h"
#include "core/memory.h"

attr_core_api b32 stack_push( Stack* stack, void* item ) {
    if( stack->len == stack->cap ) {
        return false;
    }

    usize at = stack->len++;
    memory_copy( (u8*)stack->buf + (at * stack->stride), item, stack->stride );
    return true;

}
attr_core_api b32 stack_pop_ref( Stack* stack, void** opt_out_item ) {
    if( !stack->len ) {
        return false;
    }

    usize at = --stack->len;
    if( opt_out_item ) {
        *opt_out_item = (void*)((u8*)stack->buf + (at * stack->stride));
    }
    return true;
}
attr_core_api b32 stack_pop( Stack* stack, void* opt_out_item ) {
    void* at = NULL;
    if( !stack_pop_ref( stack, &at ) ) {
        return false;
    }

    if( opt_out_item ) {
        memory_copy( opt_out_item, at, stack->stride );
    }
    return true;
}
attr_core_api void list_get( const List* list, usize index, void* out_item ) {
    memory_copy( out_item, (const u8*)list->buf + (list->stride * index), list->stride );
}
attr_core_api b32 list_append( List* list, usize count, const void* items ) {
    if( (list->len + count) > list->cap ) {
        return false;
    }

    memory_copy(
        (u8*)list->buf + (list->len * list->stride),
        items, count * list->stride );
    list->len += count;
    return true;
}
attr_core_api b32 list_prepend( List* list, usize count, const void* items ) {
    if( (list->len + count) > list->cap ) {
        return false;
    }

    memory_move(
        (u8*)list->buf + (list->stride * count),
        list->buf,
        list->stride * count );
    memory_copy( list->buf, items, count * list->stride );
    list->len += count;
    return true;
}
attr_core_api b32 list_insert( List* list, usize index, const void* item ) {
    if( list->len >= list->cap ) {
        return false;
    }

    void* src = (void*)((u8*)list->buf + (list->stride * index));
    void* dst = (void*)((u8*)src + list->stride);
    usize move_count = list->len - index;
    memory_move( dst, src, move_count );

    memory_copy( src, item, list->stride );
    list->len++;
    return true;
}
attr_core_api void list_fill( List* list, const void* value ) {
    memory_set_chunks( list->buf, list->stride, value, list->cap );
}
attr_core_api b32 queue_enqueue( Queue* queue, void* item ) {
    if( queue_is_full( queue ) ) {
        return false;
    }

    isize back = queue->back++;
    back = back % queue->cap;
    queue->len++;

    memory_copy( (u8*)queue->buf + (back * queue->stride), item, queue->stride );
    return true;
}
attr_core_api b32 queue_dequeue_ref( Queue* queue, void** opt_out_item ) {
    if( !queue->len ) {
        return false;
    }

    isize front = ++queue->front;
    front = front % queue->cap;
    queue->len--;

    if( opt_out_item ) {
        *opt_out_item = (void*)((u8*)queue->buf + (front * queue->stride));
    }

    return true;
}
attr_core_api b32 queue_dequeue( Queue* queue, void* opt_out_item ) {
    void* at = NULL;
    if( !queue_dequeue_ref( queue, &at ) ) {
        return false;
    }

    if( opt_out_item ) {
        memory_copy( opt_out_item, at, queue->stride );
    }
    return true;
}

#define INTERNAL_COLLECTIONS_HASHMAP_MAX_LINEAR_SEARCH (32)

attr_core_api b32 hashmap_insert( Hashmap* hashmap, u64 key, const void* value ) {
    if( hashmap->len == hashmap->cap ) {
        return false;
    }

    if( !hashmap->len || key > hashmap->largest_key ) {
        goto internal_hashmap_insert_push;
    }

    usize index = 0;
    if( key > hashmap->keys[0] ) {
        if( hashmap->len < INTERNAL_COLLECTIONS_HASHMAP_MAX_LINEAR_SEARCH ) {
            usize i = hashmap->len;
            do {
                i--;
                u64 k = hashmap->keys[i];
                if( key > k ) {
                    index = i + 1;
                    break;
                }
            } while( i );
        } else {
            // binary tree approach, maybe faster than linear lookup
            usize low  = 0;
            usize high = hashmap->len;
            while( low < high ) {
                usize mid = ( low + high ) >> 1;
                if( hashmap->keys[mid] < key ) {
                    low = mid + 1;
                } else {
                    high = mid;
                }
            }
            index = low;
        }
    }

    usize copy_count = hashmap->len - index;
    if( copy_count ) {
        usize copy_size  = copy_count * sizeof(u64);
        memory_move(
            hashmap->keys + index + 1, hashmap->keys + index, copy_size );
        hashmap->keys[index] = key;

        copy_size = copy_count * hashmap->stride;
        memory_move(
            (u8*)hashmap->buf + ((index + 1) * hashmap->stride),
            (u8*)hashmap->buf + (index * hashmap->stride),
            copy_size );
        memory_copy(
            (u8*)hashmap->buf + (index * hashmap->stride),
            value, hashmap->stride );

        hashmap->len++;

        if( key > hashmap->largest_key ) {
            hashmap->largest_key = key;
        }
    } else {
internal_hashmap_insert_push:

        memory_copy(
            (u8*)hashmap->buf + (hashmap->len * hashmap->stride),
            value, hashmap->stride );
        hashmap->keys[hashmap->len++] = key;

        if( key > hashmap->largest_key ) {
            hashmap->largest_key = key;
        }
    }

    return true;
}
attr_core_api b32 hashmap_remove( Hashmap* hashmap, u64 key, void* opt_out_item ) {
    void* item = NULL;
    if( !hashmap_get_ref( hashmap, key, &item ) ) {
        return false;
    }

    if( opt_out_item ) {
        memory_copy( opt_out_item, item, hashmap->stride );
    }

    usize item_index = ((usize)item - (usize)hashmap->buf) / hashmap->stride;
    u64* key_ptr = hashmap->keys + item_index;

    if( *key_ptr == hashmap->largest_key ) {
        if( item_index ) {
            hashmap->largest_key = *(key_ptr - 1);
        } else {
            hashmap->largest_key = 0;
        }
    }

    if( item_index == (hashmap->len - 1) ) {
        hashmap->len--;
        return true;
    }

    usize copy_len = hashmap->len - item_index;

    void* copy_point = (void*)((u8*)item + hashmap->stride);
    memory_move( item, copy_point, copy_len * hashmap->stride );

    copy_point = (void*)(key_ptr + 1);
    memory_move(
        key_ptr, copy_point, copy_len * sizeof(key) );

    hashmap->len--;
    return true;
}
attr_core_api b32 hashmap_contains_key( const Hashmap* hashmap, u64 key ) {
    void* item = NULL;
    return hashmap_get_ref( (Hashmap*)hashmap, key, &item );
}
attr_core_api b32 hashmap_get_ref( Hashmap* hashmap, u64 key, void** out_item ) {
    if( !hashmap->len || key > hashmap->largest_key ) {
        return false;
    }

    if( hashmap->len < INTERNAL_COLLECTIONS_HASHMAP_MAX_LINEAR_SEARCH ) {
        for( usize i = 0; i < hashmap->len; ++i ) {
            if( hashmap->keys[i] == key ) {
                *out_item = (void*)((u8*)hashmap->buf + (i * hashmap->stride));
                return true;
            }
        }
    } else {
        usize low  = 0;
        usize high = hashmap->len;
        while( low < high ) {
            usize mid = (low + high) >> 1;
            u64 k = hashmap->keys[mid];
            if( k == key ) {
                *out_item = (void*)( (u8*)hashmap->buf + (mid * hashmap->stride) );
                return true;
            } else if( k < key ) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }
    }

    return false;
}
attr_core_api b32 hashmap_get( Hashmap* hashmap, u64 key, void* out_item ) {
    void* item = NULL;
    if( !hashmap_get_ref( hashmap, key, &item ) ) {
        return false;
    }

    memory_copy( out_item, item, hashmap->stride );
    return true;
}
attr_core_api b32 hashmap_set( Hashmap* hashmap, u64 key, const void* new_value ) {
    void* item = NULL;
    if( !hashmap_get_ref( hashmap, key, &item ) ) {
        return false;
    }

    memory_copy( item, new_value, hashmap->stride );
    return true;
}

