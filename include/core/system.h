#if !defined(CORE_SYSTEM_H)
#define CORE_SYSTEM_H
/**
 * @file   system.h
 * @brief  Query system information.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 27, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/string.h"

/// @brief Feature flags.
/// @details
/// Bitfield describing features that the current CPU supports.
typedef enum CPUFeatureFlags {
    /// @brief Current CPU supports SSE instructions.
    CPU_FEATURE_SSE     = (1 << 0),
    /// @brief Current CPU supports SSE2 instructions.
    CPU_FEATURE_SSE2    = (1 << 1),
    /// @brief Current CPU supports SSE3 instructions.
    CPU_FEATURE_SSE3    = (1 << 2),
    /// @brief Current CPU supports SSSE3 instructions.
    CPU_FEATURE_SSSE3   = (1 << 3),
    /// @brief Current CPU supports SSE4.1 instructions.
    CPU_FEATURE_SSE4_1  = (1 << 4),
    /// @brief Current CPU supports SSE4.2 instructions.
    CPU_FEATURE_SSE4_2  = (1 << 5),
    /// @brief Current CPU supports AVX instructions.
    CPU_FEATURE_AVX     = (1 << 6),
    /// @brief Current CPU supports AVX2 instructions.
    CPU_FEATURE_AVX2    = (1 << 7),
    /// @brief Current CPU supports AVX-512 instructions.
    CPU_FEATURE_AVX_512 = (1 << 8),
} CPUFeatureFlags;
/// @brief Current CPU supports all SSE instructions.
#define CPU_FEATURE_SSE_MASK \
    (   CPU_FEATURE_SSE      | \
        CPU_FEATURE_SSE2     | \
        CPU_FEATURE_SSE3     | \
        CPU_FEATURE_SSSE3    | \
        CPU_FEATURE_SSE4_1   | \
        CPU_FEATURE_SSE4_2 )
/// @brief Current CPU supports AVX and AVX2 instructions.
#define CPU_FEATURE_AVX_MASK \
    ( CPU_FEATURE_AVX | CPU_FEATURE_AVX2 )

/// @brief System Information.
typedef struct SystemInfo {
    /// @brief #String containing CPU name.
    String cpu_name;
    /// @brief #String containing GPU name.
    String gpu_name;
    /// @brief Total memory in system.
    usize total_memory;
    /// @brief Size of pages.
    usize page_size;
    /// @brief Number of logical processing units that system has.
    u32 cpu_count;
    /// @brief Bitfield of CPU features.
    ///
    /// @see #CPUFeatureFlags
    CPUFeatureFlags feature_flags;
} SystemInfo;
/// @brief Query information about the current system.
/// @param[out] out_info Pointer to SystemInfo to fill out.
attr_core_api
void system_query_info( SystemInfo* out_info );
/// @brief Check if x86 cpu has SSE instructions
/// @details
/// SSE, SSE2, SSE3, SSSE3, SSE4.1 and SSE4.2
/// @param flags Feature flags.
/// @return Bitfield with missing instructions.
/// Zero if all SSE instructions are present.
attr_always_inline attr_header
CPUFeatureFlags system_feature_check_x86_sse( CPUFeatureFlags flags ) {
    return ~flags & CPU_FEATURE_SSE_MASK;
}
/// @brief Check if x86 cpu has AVX instructions
/// @details
/// AVX and AVX2 only.
/// @param flags Feature flags.
/// @return Bitfield with missing instructions.
/// Zero if all AVX instructions are present.
attr_always_inline attr_header
CPUFeatureFlags system_feature_check_x86_avx( CPUFeatureFlags flags ) {
    return ~flags & CPU_FEATURE_AVX_MASK;
}

#endif /* header guard */
