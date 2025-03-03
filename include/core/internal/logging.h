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
    CoreLoggingLevel level, usize format_len,
    const char* format, ... );

#if defined(CORE_ENABLE_LOGGING)

#define core_warn( format, ... )\
    core_log(\
        CORE_LOGGING_LEVEL_WARN,\
        sizeof( format ) - 1, format, ##__VA_ARGS__ )
#define core_error( format, ... )\
    core_log(\
        CORE_LOGGING_LEVEL_ERROR,\
        sizeof( format ) - 1, format, ##__VA_ARGS__ )

#else

#define core_warn(...)  unused(__VA_ARGS__)
#define core_error(...) unused(__VA_ARGS__)

#endif

#endif /* header guard */
