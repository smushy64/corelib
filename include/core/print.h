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
#include "core/string.h"

// forward declarations
struct FD;
attr_core_api
struct FD* pipe_stdout(void);
attr_core_api
struct FD* pipe_stderr(void);
attr_core_api
usize file_write_fmt( struct FD* file, struct _StringPOD format, ... );

/// @brief Color codes for printing to the console.
///
/// Must always be a pointer.
typedef const char AnsiColor;
/// @brief Color code black.
#define ANSI_COLOR_BLACK   "\033[1;30m"
/// @brief Color code white.
#define ANSI_COLOR_WHITE   "\033[1;37m"
/// @brief Color code red.
#define ANSI_COLOR_RED     "\033[1;31m"
/// @brief Color code green.
#define ANSI_COLOR_GREEN   "\033[1;32m"
/// @brief Color code blue.
#define ANSI_COLOR_BLUE    "\033[1;34m"
/// @brief Color code magenta.
#define ANSI_COLOR_MAGENTA "\033[1;35m"
/// @brief Color code yellow.
#define ANSI_COLOR_YELLOW  "\033[1;33m"
/// @brief Color code cyan.
#define ANSI_COLOR_CYAN    "\033[1;36m"
/// @brief Color code to reset color.
#define ANSI_COLOR_RESET   "\033[1;00m"

/// @brief Shorthand for making a colored string literal.
/// @param[in] color   (AnsiColor)      Color code to wrap text with.
/// @param[in] literal (string literal) Text to wrap with color.
/// @return (string literal) Text wrapped with given color.
#define ansi_color( color, literal ) ANSI_COLOR_##color literal ANSI_COLOR_RESET

#if defined(__cplusplus)
    #include "core/fs.h"

    /// @brief Print message to stdout.
    /// @param format (string literal) Format string.
    /// @param ...    (args)           Format arguments.
    #define print( fmt, ... ) \
        file_write_fmt( pipe_stdout(), fmt, ##__VA_ARGS__ )
    /// @brief Print message to stderr.
    /// @param format (string literal) Format string.
    /// @param ...    (args)           Format arguments.
    #define eprint( fmt, ... ) \
        file_write_fmt( pipe_stderr(), fmt, ##__VA_ARGS__ )
#else
    /// @brief Print message to stdout.
    /// @param format (string literal) Format string.
    /// @param ...    (args)           Format arguments.
    #define print( fmt, ... ) \
        file_write_fmt( pipe_stdout(), string_text(fmt), ##__VA_ARGS__ )
    /// @brief Print message to stderr.
    /// @param format (string literal) Format string.
    /// @param ...    (args)           Format arguments.
    #define eprint( fmt, ... ) \
        file_write_fmt( pipe_stderr(), string_text(fmt), ##__VA_ARGS__ )
#endif
/// @brief Print message to stdout.
/// @param format (string literal) Format string.
/// @param ...    (args)           Format arguments.
#define println( fmt, ... ) \
    print( fmt "\n", ##__VA_ARGS__ )
/// @brief Print message to stderr.
/// @param format (string literal) Format string.
/// @param ...    (args)           Format arguments.
#define eprintln( fmt, ... ) \
    eprint( fmt "\n", ##__VA_ARGS__ )

#endif /* header guard */
