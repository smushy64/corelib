#if !defined(CORE_ASSERTIONS_H)
#define CORE_ASSERTIONS_H
/**
 * @file   assertions.h
 * @brief  Assertion macros.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 10, 2024
*/
#include "core/defines.h" // IWYU pragma: keep

#define ___helper(x) #x
#define ___helper2(x) ___helper(x)
#define ___helper3( prefix, ...)\
    eprint( ANSI_COLOR_MAGENTA __FILE__ ",{cc}:" ___helper2(__LINE__) ": " prefix, __FUNCTION__ );\
    eprint( "" __VA_ARGS__ );\
    eprintln( ANSI_COLOR_RESET )

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
    #define insert_panic() *(int*)0 = 0; __assume(0)
    /// @brief Suppress unhandled case warning. Crashes if path is reachable.
    /// @details
    /// This macro function does not return when path is reached.
    #define insert_unreachable() __assume(0)
    /// @brief Break debugger on the current line.
    /// @note Only generates break if CORE_ENABLE_DEBUG_BREAK is defined.
    /// @note Currently enabled in this build configuration.
    #define insert_debug_break() __debugbreak()
#else /* CORE_COMPILER_MSVC */
    /// @brief Unconditionally crash.
    /// @details
    /// This macro function does not return.
    #define insert_panic() __builtin_trap()
    /// @brief Suppress unhandled case warning. Crashes if path is reachable.
    /// @details
    /// This macro function does not return when path is reached.
    #define insert_unreachable() __builtin_unreachable()
    /// @brief Break debugger on the current line.
    /// @note Only generates break if CORE_ENABLE_DEBUG_BREAK is defined.
    /// @note Currently enabled in this build configuration.
    #define insert_debug_break() __builtin_debugtrap()
#endif /* CORE_COMPILER != MSVC */

/// @brief Write fatal error message and panic.
/// @param ... (string literal, args) Message to write to stderr before panic.
#define panic( ... )\
    ___helper3("Panic! ", __VA_ARGS__ );\
    insert_panic()
/// @brief Write fatal error message and panic when unimplemented code section is reached.
/// @param ... (string literal, args) Message to write to stderr before panic.
#define unimplemented( ... )\
    ___helper3("Unimplemented! ", __VA_ARGS__ );\
    insert_panic()
/// @brief Write fatal error message and crash when unreachable path is taken.
/// @param ... (string literal, args) Message to write to stderr before panic.
#define unreachable( ... )\
    ___helper3("Unreachable path taken! ", __VA_ARGS__);\
    insert_unreachable()
/// @brief Crash program if condition is not met.
/// @details
/// Checks condition regardless if assertions are enabled.
/// @param condition (expression)           Condition to check.
/// @param ...       (string literal, args) (optional) Error message format and args.
#define expect( condition, ... ) do {\
    if( !(condition) ) {\
        ___helper3( "Expected '" #condition "'! ", __VA_ARGS__ );\
        insert_panic();\
    }\
} while(0)

#if defined(CORE_ENABLE_ASSERTIONS)
    /// @brief Crash program if condition is not met.
    /// @details
    /// This macro function does not return when condition is not met.
    /// @param condition (expression)           Condition to check.
    /// @param ...       (string literal, args) (optional) Error message format and args.      
    /// @note Currently enabled in this build configuration.
    #define assertion( condition, ... ) do {\
        if( !(condition) ) {\
            ___helper3( "Assertion '" #condition "' failed! ", __VA_ARGS__ );\
            insert_panic();\
        }\
    } while(0)
#else
    /// @brief Crash program if condition is not met.
    /// @details
    /// This macro function does not return when condition is not met.
    /// @param ... (expression, string literal, args) Condition to check and optional formatted error message.
    /// @note Currently disabled in this build configuration.
    #define assertion(...) 
#endif

#if defined(CORE_ENABLE_ASSERTIONS) && defined(CORE_ENABLE_DEBUG_ASSERTIONS)
    /// @brief Break debugger on the current line if condition is not met.
    /// @param condition (expression) Condition to check.
    /// @note Currently enabled in this build configuration.
    #define debug_assert( condition, ... ) do {\
        if( !(condition) ) {\
            ___helper3( "Debug assertion '"#condition"' failed! ", __VA_ARGS__ );\
            insert_debug_break();\
        }\
    } while(0)
#else
    /// @brief Break debugger on the current line if condition is not met.
    /// @param ... (expression) Condition to check.
    /// @note Currently disabled in this build configuration.
    #define debug_assert(...) 
#endif

#endif /* header guard */
