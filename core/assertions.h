#if !defined(CORE_ASSERTIONS_H)
#define CORE_ASSERTIONS_H
/**
 * @file   assertions.h
 * @brief  Assertion macros.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 10, 2024
*/
#include "core/defines.h" // IWYU pragma: keep

// NOTE(alicia): C++ includes static_assert as first class macro.
#if !defined(CORE_CPLUSPLUS)
    /// @brief Assert that condition is true at compile-time.
    /// @param condition Compile-time condition to test.
    /// @param message Message to display if condition is not met.
    #define static_assert( condition, message )\
        _Static_assert( (condition), message )
#endif

#if defined(CORE_COMPILER_MSVC)
    /// @brief Unconditionally crash.
    /// @details
    /// This macro function does not return.
    #define panic() *(int*)0 = 0
    /// @brief Suppress unhandled case warning. Crashes if path is reachable.
    /// @details
    /// This macro function does not return when path is reached.
    #define unreachable() __assume(0)
    /// @brief Break debugger on the current line.
    /// @note Only generates break if CORE_ENABLE_DEBUG_BREAK is defined.
    /// @note Currently enabled in this build configuration.
    #define debug_break() __debugbreak()
#else /* CORE_COMPILER_MSVC */
    /// @brief Unconditionally crash.
    /// @details
    /// This macro function does not return.
    #define panic() __builtin_trap()
    /// @brief Suppress unhandled case warning. Crashes if path is reachable.
    /// @details
    /// This macro function does not return when path is reached.
    #define unreachable() __builtin_unreachable()
    /// @brief Break debugger on the current line.
    /// @note Only generates break if CORE_ENABLE_DEBUG_BREAK is defined.
    /// @note Currently enabled in this build configuration.
    #define debug_break() __builtin_debugtrap()
#endif /* CORE_COMPILER != MSVC */

#if !defined(CORE_ENABLE_DEBUG_BREAK)
    #undef debug_break
    /// @brief Break debugger on the current line.
    /// @note Only generates break if CORE_ENABLE_DEBUG_BREAK is defined.
    /// @note Currently disabled in this build configuration.
    #define debug_break()
#endif /* CORE_ENABLE_DEBUG_BREAK */

/// @brief Crash program if condition is not met.
/// @details
/// This macro function does not return when condition is not met.
/// @param condition (expression) Condition to check.
/// @note Currently enabled in this build configuration.
#define assert( condition ) do {\
    if( !(condition) ) {\
        panic();\
    }\
} while(0)
/// @brief Break debugger on the current line if condition is not met.
/// @param condition (expression) Condition to check.
/// @note Currently enabled in this build configuration.
#define debug_assert( condition ) do {\
    if( !(condition) ) {\
        debug_break();\
    }\
} while(0)

#if !defined(CORE_ENABLE_ASSERTIONS)
    #undef assert
    /// @brief Crash program if condition is not met.
    /// @details
    /// This macro function does not return when condition is not met.
    /// @param condition (expression) Condition to check.
    /// @note Currently disabled in this build configuration.
    #define assert(...) 
#endif

#if !defined(CORE_ENABLE_DEBUG_ASSERTIONS)
    #undef debug_assert
    /// @brief Break debugger on the current line if condition is not met.
    /// @param condition (expression) Condition to check.
    /// @note Currently disabled in this build configuration.
    #define debug_assert(...) 
#endif

#endif /* header guard */
