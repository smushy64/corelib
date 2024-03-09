#if !defined(CORE_INTERNAL_LOGGING_H)
#define CORE_INTERNAL_LOGGING_H
/**
 * Description:  Internal logging functions.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 24, 2024
*/
#include "core/types.h"
// #include "core/attributes.h"
#include "core/lib.h"
#include "core/print.h" // IWYU pragma: keep
#include "core/macros.h" // IWYU pragma: keep

void core_log(
    enum CoreLoggingLevel level, usize format_len,
    const char* format, ... );

#if defined(CORE_ENABLE_LOGGING)

#define core_debug( format, ... )\
    core_log(\
        CORE_LOGGING_LEVEL_DEBUG,\
        sizeof( str_blue( "[CORE DEBUG] " format ) ) - 1,\
        str_blue( "[CORE DEBUG] " format ), ##__VA_ARGS__ )

#define core_info( format, ... )\
    core_log(\
        CORE_LOGGING_LEVEL_INFO,\
        sizeof( "[CORE INFO] " format ) - 1,\
        "[CORE INFO] " format, ##__VA_ARGS__ )

#define core_warn( format, ... )\
    core_log(\
        CORE_LOGGING_LEVEL_WARN,\
        sizeof( str_yellow( "[CORE WARN] " format ) ) - 1,\
        str_yellow( "[CORE WARN] " format ), ##__VA_ARGS__ )

#define core_error( format, ... )\
    core_log(\
        CORE_LOGGING_LEVEL_ERROR,\
        sizeof( str_red( "[CORE ERROR] " format ) ) - 1,\
        str_red( "[CORE ERROR] " format ), ##__VA_ARGS__ )

#else

#define core_debug(...) unused(__VA_ARGS__)
#define core_info(...) unused(__VA_ARGS__)
#define core_warn(...) unused(__VA_ARGS__)
#define core_error(...) unused(__VA_ARGS__)

#endif

#endif /* header guard */
