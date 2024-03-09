/**
 * Description:  Sorting and searching algorithm implementations.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 21, 2024
*/
#include "core/prelude.h"
#include "core/sort.h"
#include "core/memory.h"

attr_internal isize quicksort_partition_i32(
    isize low, isize high, i32* buffer, b32 reverse
) {
    i32* pivot = buffer + high;

    isize i = low - 1;

    for( isize j = low; j <= high - 1; ++j ) {
        i32* at_j = buffer + j;

        b32 cmp = reverse ? *pivot < *at_j : *at_j < *pivot;
        if( cmp ) {
            ++i;
            i32* at_i = buffer + i;
            i32 temp = *at_j;
            *at_j = *at_i;
            *at_i = temp;
        }
    }

    i32 temp = *(buffer + ( i + 1 ));
    *(buffer + (i + 1)) = *(buffer + high);
    *(buffer + high) = temp;

    return i + 1;
}
attr_core_api void quicksort_i32(
    isize from_inclusive, isize to_inclusive, i32* buffer, b32 reverse
) {

    while( from_inclusive < to_inclusive ) {
        isize partition_index =
            quicksort_partition_i32( from_inclusive, to_inclusive, buffer, reverse );

        if(
            (partition_index - from_inclusive) <
            (to_inclusive - partition_index)
        ) {
            quicksort_i32( from_inclusive, partition_index - 1, buffer, reverse );
            from_inclusive = partition_index + 1;
        } else {
            quicksort_i32( partition_index + 1, to_inclusive, buffer, reverse );
            to_inclusive = partition_index - 1;
        }
    }
}

attr_always_inline inline
attr_internal void internal_sort_swap(
    void* a, void* b, void* swap_buffer, usize size
) {
    memory_copy( swap_buffer, a, size );
    memory_copy( a, b, size );
    memory_copy( b, swap_buffer, size );
}

attr_internal isize quicksort_generic_partition(
    isize low, isize high, usize stride, void* buffer,
    SortCmpFN* cmp, void* cmp_params, void* swap_buffer
) {
    u8* buf = buffer;
    void* pivot = buf + (high * stride);

    isize i = (isize)low - 1;

    for( isize j = (isize)low; j <= (isize)high - 1; ++j ) {
        void* at_j = buf + (j * stride);

        if( cmp( at_j, pivot, stride, cmp_params ) ) {
            ++i;
            void* at_i = buf + (i * stride);
            internal_sort_swap( at_i, at_j, swap_buffer, stride );
        }
    }

    i++;
    internal_sort_swap(
        buf + (i * stride), buf + (high * stride), swap_buffer, stride );
    return i;
}
attr_core_api void quicksort_generic(
    isize from_inclusive, isize to_inclusive,
    usize stride, void* buffer,
    SortCmpFN* cmp, void* cmp_params, void* swap_buffer
) {
    while( from_inclusive < to_inclusive ) {
        isize partition_index = quicksort_generic_partition(
            from_inclusive, to_inclusive, stride,
            buffer, cmp, cmp_params, swap_buffer );

        if( partition_index - from_inclusive < to_inclusive - partition_index ) {
            quicksort_generic(
                from_inclusive, partition_index - 1,
                stride, buffer, cmp, cmp_params, swap_buffer );
            from_inclusive = partition_index + 1;
        } else {
            quicksort_generic(
                partition_index + 1, to_inclusive,
                stride, buffer, cmp, cmp_params, swap_buffer );
            to_inclusive = partition_index - 1;
        }
    }
}

attr_internal isize quicksort_partition_u32(
    isize low, isize high, u32* buffer, b32 reverse
) {
    u32* pivot = buffer + high;

    isize i = low - 1;

    for( isize j = low; j <= high - 1; ++j ) {
        u32* at_j = buffer + j;

        b32 cmp = reverse ? *pivot < *at_j : *at_j < *pivot;
        if( cmp ) {
            ++i;
            u32* at_i = buffer + i;
            u32 temp = *at_j;
            *at_j = *at_i;
            *at_i = temp;
        }
    }

    u32 temp = *(buffer + ( i + 1 ));
    *(buffer + (i + 1)) = *(buffer + high);
    *(buffer + high) = temp;

    return i + 1;
}
attr_core_api void quicksort_u32(
    isize from_inclusive, isize to_inclusive, u32* buffer, b32 reverse
) {

    while( from_inclusive < to_inclusive ) {
        isize partition_index =
            quicksort_partition_u32( from_inclusive, to_inclusive, buffer, reverse );

        if(
            (partition_index - from_inclusive) <
            (to_inclusive - partition_index)
        ) {
            quicksort_u32( from_inclusive, partition_index - 1, buffer, reverse );
            from_inclusive = partition_index + 1;
        } else {
            quicksort_u32( partition_index + 1, to_inclusive, buffer, reverse );
            to_inclusive = partition_index - 1;
        }
    }
}

attr_internal isize quicksort_partition_f32(
    isize low, isize high, f32* buffer, b32 reverse
) {
    f32* pivot = buffer + high;

    isize i = low - 1;

    for( isize j = low; j <= high - 1; ++j ) {
        f32* at_j = buffer + j;

        b32 cmp = reverse ? *pivot < *at_j : *at_j < *pivot;
        if( cmp ) {
            ++i;
            f32* at_i = buffer + i;
            f32 temp = *at_j;
            *at_j = *at_i;
            *at_i = temp;
        }
    }

    f32 temp = *(buffer + ( i + 1 ));
    *(buffer + (i + 1)) = *(buffer + high);
    *(buffer + high) = temp;

    return i + 1;
}
attr_core_api void quicksort_f32(
    isize from_inclusive, isize to_inclusive, f32* buffer, b32 reverse
) {

    while( from_inclusive < to_inclusive ) {
        isize partition_index =
            quicksort_partition_f32( from_inclusive, to_inclusive, buffer, reverse );

        if(
            (partition_index - from_inclusive) <
            (to_inclusive - partition_index)
        ) {
            quicksort_f32( from_inclusive, partition_index - 1, buffer, reverse );
            from_inclusive = partition_index + 1;
        } else {
            quicksort_f32( partition_index + 1, to_inclusive, buffer, reverse );
            to_inclusive = partition_index - 1;
        }
    }
}

attr_core_api void sort_reverse(
    usize len, usize stride, void* buffer, void* swap_buffer
) {
    u8* bytes = buffer;
    usize start = 0;
    usize end   = len - 1;

    while( start < end ) {
        u8* start_ptr = bytes + ( start * stride );
        u8* end_ptr   = bytes + ( end * stride );
        internal_sort_swap( start_ptr, end_ptr, swap_buffer, stride );

        start++;
        end--;
    }
}

