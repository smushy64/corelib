#if !defined(CORE_SORT_H)
#define CORE_SORT_H
/**
 * @file   sort.h
 * @brief  Sorting and searching algorithms.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 21, 2024
*/
#include "core/types.h"
#include "core/attributes.h"

/// @brief Comparison function prototype for sorting.
/// @param[in] lhs, rhs Pointer to values to compare.
/// @param size Size of each value to compare.
/// @param[in] params (optional) Optional parameters.
/// @return True means rhs was selected, otherwise lhs.
typedef b32 SortCmpFN( void* lhs, void* rhs, usize size, void* params );

/// @brief Comparison function prototype for searching.
/// @param[in] lhs, rhs Pointer to values to compare.
/// @param size Size of each value to compare.
/// @param[in] params (optional) Optional parameters.
/// @return True if lhs and rhs are equal.
typedef SortCmpFN SearchCmpFN;

/// @brief Quicksort algorithm for i32.
/// @note Prefer this function over #quicksort_generic() when sorting integers.
/// @param from_inclusive Start of sort range, must be positive.
/// @param to_inclusive End of sort range, must be > from_inclusive and positive.
/// @param[in] buffer Pointer to integers to sort.
/// @param reverse If true, numbers are sorted from greatest -> least, otherwise least -> greatest.
attr_core_api void quicksort_i32(
    isize from_inclusive, isize to_inclusive, i32* buffer, b32 reverse );
/// @brief Quicksort algorithm for u32.
/// @note Prefer this function over #quicksort_generic() when sorting unsigned integers.
/// @param from_inclusive Start of sort range, must be positive.
/// @param to_inclusive End of sort range, must be > from_inclusive and positive.
/// @param[in] buffer Pointer to integers to sort.
/// @param reverse If true, numbers are sorted from greatest -> least, otherwise least -> greatest.
attr_core_api void quicksort_u32(
    isize from_inclusive, isize to_inclusive, u32* buffer, b32 reverse );
/// @brief Quicksort algorithm for f32.
/// @note Prefer this function over #quicksort_generic() when sorting floats.
/// @param from_inclusive Start of sort range, must be positive.
/// @param to_inclusive End of sort range, must be > from_inclusive and positive.
/// @param[in] buffer Pointer to floats to sort.
/// @param reverse If true, numbers are sorted from greatest -> least, otherwise least -> greatest.
attr_core_api void quicksort_f32(
    isize from_inclusive, isize to_inclusive, f32* buffer, b32 reverse );

/// @brief Generic quicksort algorithm.
/// @param from_inclusive Start of sort range, must be positive.
/// @param to_inclusive End of sort range, must be > from_inclusive and positive.
/// @param stride Size of each value in buffer.
/// @param[in] buffer Pointer to values to sort.
/// @param cmp Function to use for comparisons.
/// @param[in] opt_cmp_params (optional) Parameters for comparison function.
/// @param[in] swap_buffer Buffer for swapping values, must be able to hold >= stride.
attr_core_api void quicksort_generic(
    isize from_inclusive, isize to_inclusive,
    usize stride, void* buffer,
    SortCmpFN* cmp, void* opt_cmp_params, void* swap_buffer );

/// @brief Reverse items in a buffer.
/// @param len Number of items in buffer.
/// @param stride Size of each item in buffer.
/// @param[in] buffer Pointer to start of buffer.
/// @param[in] swap_buffer Pointer to swap buffer, must be able to hold stride bytes.
attr_core_api void sort_reverse(
    usize len, usize stride, void* buffer, void* swap_buffer );

#endif /* header guard */
