#if !defined(CORE_RAND_H)
#define CORE_RAND_H
/**
 * @file   rand.h
 * @brief  Random number generation.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 01, 2024
*/
#include "core/types.h"
#include "core/attributes.h"

/// @brief Get next random number using LCG generator.
/// @note state keeps track of generator state and is also the seed.
/// @param[in,out] in_out_state State of generator.
/// @return Next number in sequence.
attr_core_api u64 rand_lcg_next( u64* in_out_state );
/// @brief Get next random number using XOR shift generator.
/// @note state keeps track of generator state and is also the seed.
/// @param[in,out] in_out_state State of generator.
/// @return Next number in sequence.
attr_core_api u64 rand_xor_next( u64* in_out_state );

#endif /* header guard */
