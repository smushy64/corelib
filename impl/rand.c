/**
 * Description:  Random number generators implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: March 01, 2024
*/
#include "core/prelude.h"
#include "core/rand.h"

#define RAND_LCG_CONSTANT_A (166463)
#define RAND_LCG_CONSTANT_B (235)
#define RAND_LCG_CONSTANT_M (21446837)

attr_core_api u64 rand_lcg_next( u64* in_out_state ) {
    *in_out_state = (
        RAND_LCG_CONSTANT_A * *in_out_state +
        RAND_LCG_CONSTANT_B
    ) % RAND_LCG_CONSTANT_M;
    return *in_out_state;
}

#define RAND_XOR_CONSTANT_0 (12)
#define RAND_XOR_CONSTANT_1 (25)
#define RAND_XOR_CONSTANT_2 (27)
#define RAND_XOR_CONSTANT_3 (0x2545F4914F6CDD1DULL)
attr_core_api u64 rand_xor_next( u64* in_out_state ) {
    u64 x = *in_out_state;

    x ^= x >> RAND_XOR_CONSTANT_0;
    x ^= x << RAND_XOR_CONSTANT_1;
    x ^= x >> RAND_XOR_CONSTANT_2;

    x *= RAND_XOR_CONSTANT_3;

    *in_out_state = x;
    return x;
}

