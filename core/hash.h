#if !defined(CORE_HASH_H)
#define CORE_HASH_H
/**
 * @file   hash.h
 * @brief  Hashing algorithms.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 25, 2024
*/
#include "core/types.h"
#include "core/attributes.h"

/// @brief Elf 64-bit hash function.
/// @param     len Length of @c buf in bytes.
/// @param[in] buf Pointer to start of buffer to hash.
/// @return 64-bit hash of @c buf.
attr_core_api
hash64 hash_elf_64( usize len, const void* buf );
/// @brief MurmurHash2 64-bit hash function.
/// @details
/// Original implementation is in the public domain and was written by Austin Appleby.
/// https://github.com/aappleby/smhasher
/// @param     len Length of @c buf in bytes.
/// @param[in] buf Pointer to start of buffer to hash.
/// @return 64-bit hash of @c buf.
attr_core_api
hash64 hash_murmur2_64( usize len, const void* buf );
/// @brief Cityhash 64-bit hash function.
/// @details
/// CityHash, by Geoff Pike and Jyrki Alakuijala
///
/// Original implementation is licensed under the MIT license.
///
/// https://github.com/google/cityhash
/// @param     len Length of @c buf in bytes.
/// @param[in] buf Pointer to start of buffer to hash.
/// @return 64-bit hash of @c buf.
attr_core_api
hash64 hash_city_64( usize len, const void* buf );

#endif /* header guard */
