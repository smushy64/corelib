/**
 * Description:  Core library function implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 06, 2024
*/
#include "core/defines.h"
#include "core/prelude.h"
#include "core/lib.h"

#if !defined(CORE_LIB_VERSION_MAJOR)
    #define CORE_LIB_VERSION_MAJOR 0
    pragma_warn( "CORE_LIB_VERSION_MAJOR is undefined!" )
#endif

#if !defined(CORE_LIB_VERSION_MINOR)
    #define CORE_LIB_VERSION_MINOR 0
    pragma_warn( "CORE_LIB_VERSION_MINOR is undefined!" )
#endif

#if !defined(CORE_LIB_VERSION_PATCH)
    #define CORE_LIB_VERSION_PATCH 0
    pragma_warn( "CORE_LIB_VERSION_PATCH is undefined!" )
#endif

attr_global u32 const global_lib_version =
    core_create_version(
        CORE_LIB_VERSION_MAJOR, CORE_LIB_VERSION_MINOR, CORE_LIB_VERSION_PATCH );

attr_core_api
u32 core_version(void) {
    return global_lib_version;
}

extern const char external_core_command_line[];
extern usize      external_core_command_line_len;

attr_core_api
const char* core_command_line( usize* opt_out_len ) {
    if( opt_out_len ) {
        *opt_out_len = external_core_command_line_len;
    }
    return external_core_command_line;
}

attr_global
enum CoreLoggingLevel global_logging_level = CORE_LOGGING_LEVEL_NONE;
attr_global
CoreLoggingCallbackFN* global_logging_callback = NULL;
attr_global
void* global_logging_callback_params = NULL;

attr_core_api
void core_set_logging_level( enum CoreLoggingLevel level ) {
    global_logging_level = level;
}
attr_core_api
enum CoreLoggingLevel core_query_logging_level(void) {
    return global_logging_level;
}
attr_core_api
void core_set_logging_callback(
    CoreLoggingCallbackFN* callback, void* params
) {
    global_logging_callback        = callback;
    global_logging_callback_params = params;
}
attr_core_api
void core_clear_logging_callback(void) {
    global_logging_callback        = NULL;
    global_logging_callback_params = NULL;
}
attr_always_inline inline attr_internal
b32 internal_logging_level_valid( CoreLoggingLevel level ) {
    if( !global_logging_level ) {
        return false;
    }
    return global_logging_level >= level;
}
void core_log(
    CoreLoggingLevel level, usize format_len,
    const char* format, ...
) {
    if( !(
        global_logging_callback               &&
        internal_logging_level_valid( level )
    )) {
        return;
    }

    va_list va;
    va_start( va, format );

    global_logging_callback(
        level, format_len, format, va, global_logging_callback_params );

    va_end( va );
}

