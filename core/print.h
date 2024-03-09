#if !defined(CORE_PRINT_H)
#define CORE_PRINT_H
/**
 * @file   print.h
 * @brief  Printing to the console.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 14, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/stream.h"
#include "core/fmt.h"
#include "core/fs.h"

/// @brief Color codes for printing to the console.
///
/// Must always be a pointer.
typedef const char ConsoleColor;
/// @brief Color code black.
#define CONSOLE_COLOR_BLACK   "\033[1;30m"
/// @brief Color code white.
#define CONSOLE_COLOR_WHITE   "\033[1;37m"
/// @brief Color code red.
#define CONSOLE_COLOR_RED     "\033[1;31m"
/// @brief Color code green.
#define CONSOLE_COLOR_GREEN   "\033[1;32m"
/// @brief Color code blue.
#define CONSOLE_COLOR_BLUE    "\033[1;34m"
/// @brief Color code magenta.
#define CONSOLE_COLOR_MAGENTA "\033[1;35m"
/// @brief Color code yellow.
#define CONSOLE_COLOR_YELLOW  "\033[1;33m"
/// @brief Color code cyan.
#define CONSOLE_COLOR_CYAN    "\033[1;36m"
/// @brief Color code to reset color.
#define CONSOLE_COLOR_RESET   "\033[1;00m"

/// @brief Shorthand for making a red string literal.
/// @param str String literal to colorize.
/// @return String literal surrounded by red color code and reset color code.
#define str_red( str )\
    CONSOLE_COLOR_RED str CONSOLE_COLOR_RESET
/// @brief Shorthand for making a green string literal.
/// @param str String literal to colorize.
/// @return String literal surrounded by green color code and reset color code.
#define str_green( str )\
    CONSOLE_COLOR_GREEN str CONSOLE_COLOR_RESET
/// @brief Shorthand for making a blue string literal.
/// @param str String literal to colorize.
/// @return String literal surrounded by blue color code and reset color code.
#define str_blue( str )\
    CONSOLE_COLOR_BLUE str CONSOLE_COLOR_RESET
/// @brief Shorthand for making a magenta string literal.
/// @param str String literal to colorize.
/// @return String literal surrounded by magenta color code and reset color code.
#define str_magenta( str )\
    CONSOLE_COLOR_MAGENTA str CONSOLE_COLOR_RESET
/// @brief Shorthand for making a yellow string literal.
/// @param str String literal to colorize.
/// @return String literal surrounded by yellow color code and reset color code.
#define str_yellow( str )\
    CONSOLE_COLOR_YELLOW str CONSOLE_COLOR_RESET
/// @brief Shorthand for making a cyan string literal.
/// @param str String literal to colorize.
/// @return String literal surrounded by cyan color code and reset color code.
#define str_cyan( str )\
    CONSOLE_COLOR_CYAN str CONSOLE_COLOR_RESET
/// @brief Shorthand for making a black string literal.
/// @param str String literal to colorize
/// @return String literal surrounded by black color code and reset color code.
#define str_black( str )\
    CONSOLE_COLOR_BLACK str CONSOLE_COLOR_RESET
/// @brief Shorthand for making a white string literal.
/// @param str String literal to colorize
/// @return String literal surrounded by white color code and reset color code.
#define str_white( str )\
    CONSOLE_COLOR_BLACK str CONSOLE_COLOR_RESET

/// @brief Print formatted text to stdout using variadic argument list.
/// @param format_len Length of format string, exclusing null-terminator.
/// @param[in] format Pointer to format string.
/// @param va Variadic arguments list.
///
/// @see #va_list
/// @see #fmt_text_va()
attr_always_inline
attr_header void print_text_va( usize format_len, const char* format, va_list va ) {
    fmt_text_va( stream_console, stdout_handle(), format_len, format, va );
}
/// @brief Print formatted text to stdout.
/// @param format_len Length of format string, exclusing null-terminator.
/// @param[in] format Pointer to format string.
/// @param ... Formatting arguments.
///
/// @see #print_text_va()
attr_header void print_text( usize format_len, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    print_text_va( format_len, format, va );
    va_end( va );
}
/// @brief Print formatted text to stderr using variadic argument list.
/// @param format_len Length of format string, exclusing null-terminator.
/// @param[in] format Pointer to format string.
/// @param va Variadic arguments list.
///
/// @see #va_list
/// @see #fmt_text_va()
attr_always_inline
attr_header void print_err_text_va( usize format_len, const char* format, va_list va ) {
    fmt_text_va( stream_console, stderr_handle(), format_len, format, va );
}
/// @brief Print formatted text to stderr.
/// @param format_len Length of format string, exclusing null-terminator.
/// @param[in] format Pointer to format string.
/// @param ... - Formatting arguments.
///
/// @see #print_err_text_va()
attr_header void print_err_text( usize format_len, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    print_err_text_va( format_len, format, va );
    va_end( va );
}

/// @brief Print formatted text to stdout.
/// @param format Format string literal.
/// @param ... Formatting arguments.
///
/// @see #print_text()
#define print( format, ... )\
    print_text( sizeof(format) - 1, format, ##__VA_ARGS__ )
/// @brief Print formatted text to stderr.
/// @param format Format string literal.
/// @param ... - Formatting arguments.
///
/// @see #print_err_text()
#define print_err( format, ... )\
    print_err_text( sizeof(format) - 1, format, ##__VA_ARGS__ )
/// @brief Print formatted text to stdout using variadic argument list.
/// @param format Format string literal.
/// @param va Variadic arguments list.
///
/// @see #print_text_va()
#define print_va( format, va )\
    print_text_va( sizeof(format) - 1, format, va )
/// Print formatted text to stderr using variadic argument list.
/// @param format Format string literal.
/// @param va Variadic arguments list.
///
/// @see #print_err_text_va()
#define print_err_va( format, va )\
    print_err_text_va( sizeof(format) - 1, format, va )
/// @brief Print formatted text to stdout.
///
/// Uses a mutex to prevent crosstalk between threads.
/// @param[in] mutex Mutex to lock.
/// @param format Format string literal.
/// @param ... - Formatting arguments.
///
/// @see #Mutex
/// @see #print()
#define print_mt( mutex, format, ... ) do {\
    mutex_lock( (mutex) );\
    print( format, ##__VA_ARGS__ );\
    mutex_unlock( (mutex) );\
} while(0)
/// @brief Print formatted text to stderr.
///
/// Uses a mutex to prevent crosstalk between threads.
/// @param[in] mutex Mutex to lock.
/// @param format Format string literal.
/// @param ... - Formatting arguments.
///
/// @see #Mutex
/// @see #print_err()
#define print_err_mt( mutex, format, ... ) do {\
    mutex_lock( (mutex) );\
    print_err( format, ##__VA_ARGS__ );\
    mutex_unlock( (mutex) );\
} while(0)
/// @brief Print formatted text to stdout using variadic argument list.
///
/// Use a mutex to prevent crosstalk between threads.
/// @param[in] mutex Mutex to lock.
/// @param format Format string literal.
/// @param va Variadic formatting arguments list.
///
/// @see #Mutex
/// @see #print_va()
#define print_va_mt( mutex, format, va ) do {\
    mutex_lock( (mutex) );\
    print_va( format, va );\
    mutex_unlock( (mutex) );\
} while(0)
/// @brief Print formatted text to stderr using variadic argument list.
///
/// Use a mutex to prevent crosstalk between threads.
/// @param[in] mutex Mutex to lock.
/// @param format Format string literal.
/// @param va Variadic formatting arguments list.
///
/// @see #Mutex
/// @see #print_err_va()
#define print_err_va_mt( mutex, format, va ) do {\
    mutex_lock( (mutex) );\
    print_err_va( format, va );\
    mutex_unlock( (mutex) );\
} while(0)

/// @brief Print formatted text with a new line to stdout.
/// @param format Format string literal.
/// @param ... Formatting arguments.
///
/// @see #print()
#define println( format, ... )\
    print( format "\n", ##__VA_ARGS__ )
/// @brief Print formatted text with a new line to stderr.
/// @param format Format string literal.
/// @param ... Formatting arguments.
///
/// @see #print_err()
#define println_err( format, ... )\
    print_err( format "\n", ##__VA_ARGS__ )
/// @brief Print formatted text with a new line to stdout.
///
/// Use a mutex to prevent crosstalk between threads.
/// @param[in] mutex Mutex to lock.
/// @param format Format string literal.
/// @param ... Formatting arguments.
///
/// @see #Mutex
/// @see #println()
#define println_mt( mutex, format, ... ) do {\
    mutex_lock( (mutex) );\
    println( format, ##__VA_ARGS__ );\
    mutex_unlock( (mutex) );\
} while(0)
/// @brief Print formatted text with a new line to stderr.
///
/// Use a mutex to prevent crosstalk between threads.
/// @param[in] mutex Mutex to lock.
/// @param format Format string literal.
/// @param ... Formatting arguments.
///
/// @see #Mutex
/// @see #println_err()
#define println_err_mt( mutex, format, ... ) do {\
    mutex_lock( (mutex) );\
    println_err( format, ##__VA_ARGS__ );\
    mutex_unlock( (mutex) );\
} while(0)

#if defined(CORE_ENABLE_ASSERTIONS)
    /// @brief Assert that condition is true at run-time.
    ///
    /// Prints provided message to stderr if assertion fails.
    /// Only works if CORE_ENABLE_ASSERTIONS is defined.
    /// @param condition Run-time condition to test.
    /// @param format Format string literal.
    /// @param ... Format arguments.
    /// 
    /// @see #assert()
    /// @see #panic()
    #define assert_log( condition, format, ... ) do {\
        if( !(condition) ) {\
            println_err( str_magenta( __FILE__ ":" stringify_macro(__LINE__) " ASSERTION FAILED: " format ), ##__VA_ARGS__ );\
            panic();\
        }\
    } while(0)
    #if defined(CORE_ENABLE_DEBUG_ASSERTIONS)
        /// @brief Assert that condition is true at run-time.
        /// 
        /// Prints provided message to stderr if assertion fails.
        /// Generates #debug_break() instead of #panic().
        /// Only works if CORE_ENABLE_ASSERTIONS and CORE_ENABLE_DEBUG_ASSERTIONS is defined.
        /// @param condition Run-time condition to test.
        /// @param format Format string literal.
        /// @param ... Format arguments.
        /// 
        /// @see #debug_break()
        #define assert_debug_log( condition, format, ... ) do {\
            if( !(condition) ) {\
                println_err( str_magenta( __FILE__ ":" stringify_macro(__LINE__) " DEBUG ASSERTION FAILED: " format ), ##__VA_ARGS__ );\
                debug_break();\
            }\
        } while(0)
    #else
        /// @brief Assert that condition is true at run-time.
        /// 
        /// Prints provided message to stderr if assertion fails.
        /// Generates #debug_break() instead of #panic().
        /// Only works if CORE_ENABLE_ASSERTIONS and CORE_ENABLE_DEBUG_ASSERTIONS is defined.
        /// @param ... Ignored parameters.
        /// 
        /// @see #debug_break()
        #define assert_debug_log(...) unused(__VA_ARGS__)
    #endif
#else
    /// @brief Assert that condition is true at run-time.
    ///
    /// Prints provided message to stderr if assertion fails.
    /// Only works if CORE_ENABLE_ASSERTIONS is defined.
    /// @param ... Ignored parameters.
    /// 
    /// @see #assert()
    /// @see #panic()
    #define assert_log(...) unused(__VA_ARGS__)
    /// @brief Assert that condition is true at run-time.
    /// 
    /// Prints provided message to stderr if assertion fails.
    /// Generates #debug_break() instead of #panic().
    /// Only works if CORE_ENABLE_ASSERTIONS and CORE_ENABLE_DEBUG_ASSERTIONS is defined.
    /// @param ... Ignored parameters.
    /// 
    /// @see #debug_break()
    #define assert_debug_log(...) unused(__VA_ARGS__)
#endif

#endif /* header guard */
