#if !defined(CORE_CPP_SYSTEM_HPP) && defined(__cplusplus)
#define CORE_CPP_SYSTEM_HPP
/**
 * @file   system.hpp
 * @brief  C++ System.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 01, 2025
*/
#if !defined(CORE_SYSTEM_H)
    #include "core/system.h"
#endif
#include "core/cpp/bitwise.hpp"

CPP_BITWISE_OPERATORS(CPUFeatureFlags, u32);

#endif /* header guard */
