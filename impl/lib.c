/**
 * Description:  Core library function implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 06, 2024
*/
#include "core/defines.h"
#include "core/prelude.h"
#include "core/lib.h"
#include "core/sync.h"
#include "core/print.h"
#include "core/fmt.h"

#if !defined(CORE_LIB_VERSION_MAJOR)
    #define CORE_LIB_VERSION_MAJOR 0
    pragma_warning( "CORE_LIB_VERSION_MAJOR is undefined!" )
#endif

#if !defined(CORE_LIB_VERSION_MINOR)
    #define CORE_LIB_VERSION_MINOR 0
    pragma_warning( "CORE_LIB_VERSION_MINOR is undefined!" )
#endif

#if !defined(CORE_LIB_VERSION_PATCH)
    #define CORE_LIB_VERSION_PATCH 0
    pragma_warning( "CORE_LIB_VERSION_PATCH is undefined!" )
#endif

attr_global u32 const global_lib_version =
    core_create_version(
        CORE_LIB_VERSION_MAJOR, CORE_LIB_VERSION_MINOR, CORE_LIB_VERSION_PATCH );

#define CORE_LIB_VERSION_STRING stringify_macro(CORE_LIB_VERSION_MAJOR) "." stringify_macro(CORE_LIB_VERSION_MINOR) "." stringify_macro(CORE_LIB_VERSION_PATCH)

attr_core_api u32 core_version(void) {
    return global_lib_version;
}
attr_core_api const char* core_version_string( usize* opt_out_len ) {
    if( opt_out_len ) {
        *opt_out_len = sizeof( CORE_LIB_VERSION_STRING ) - 1;
    }
    return CORE_LIB_VERSION_STRING;
}

attr_global const char global_build_description[] =
#if defined(CORE_STATIC_BUILD)
    "Statically compiled. "
#else
    "Dynamic library. "
#endif
    "Compiled with "
    CORE_COMPILER_VERSION
    "targetting "

#if defined(CORE_PLATFORM_WINDOWS)
    "win32 "
#elif defined(CORE_PLATFORM_LINUX)
    "linux "
#elif defined(CORE_PLATFORM_ANDROID)
    "android "
#elif defined(CORE_PLATFORM_IOS)
    "iOS "
#elif defined(CORE_PLATFORM_MACOS)
    "macOS "
#else
    "unknown platform "
#endif
#if defined(CORE_ARCH_X86)
    "x86"
    #if defined(CORE_ARCH_64_BIT)
        "-64"
    #else
        ""
    #endif
    #if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
        " with SSE instructions. "
    #elif defined(CORE_ENABLE_AVX_INSTRUCTIONS)
        " with AVX instructions. "
    #else
        ". "
    #endif /* no x86 simd */
#elif defined(CORE_ARCH_ARM)
    "Arm "
    #if defined(CORE_ARCH_64_BIT)
        "32-bit "
    #else
        "64-bit "
    #endif
    #if defined(CORE_ARCH_LITTLE_ENDIAN)
        "little endian"
    #else
        "big endian"
    #endif
    #if defined(CORE_ENABLE_NEON_INSTRUCTIONS)
        " with NEON instructions. "
    #else
        " . "
    #endif
#else
    "unknown architecture. "
#endif
    "Compiled on " __DATE__ "."
;

attr_core_api const char* core_build_description( usize* opt_out_len ) {
    if( opt_out_len ) {
        *opt_out_len = sizeof( global_build_description ) - 1;
    }
    return global_build_description;
}

attr_global enum CoreLoggingLevel global_logging_level = CORE_LOGGING_LEVEL_NONE;
attr_global Mutex global_logging_mutex;
attr_global b32 global_logging_mutex_created = false;
attr_global CoreLoggingCallbackFN* global_logging_callback = NULL;
attr_global void* global_logging_callback_params = NULL;

attr_core_api void core_set_logging_level( enum CoreLoggingLevel level ) {
    if( !global_logging_mutex_created ) {
        assert_log(
            mutex_create( &global_logging_mutex ),
            "failed to create logging mutex!" );
        global_logging_mutex_created = true;
    }

    mutex_lock( &global_logging_mutex );

    // TODO(alicia): check if level is valid?
    global_logging_level = level;

    mutex_unlock( &global_logging_mutex );
}
attr_core_api enum CoreLoggingLevel core_query_logging_level(void) {
    return global_logging_level;
}
attr_core_api void core_set_logging_callback(
    CoreLoggingCallbackFN* callback, void* params
) {
    if( !global_logging_mutex_created ) {
        assert_log(
            mutex_create( &global_logging_mutex ),
            "failed to create logging mutex!" );
        global_logging_mutex_created = true;
    }

    mutex_lock( &global_logging_mutex );

    global_logging_callback        = callback;
    global_logging_callback_params = params;

    mutex_unlock( &global_logging_mutex );
}
attr_core_api void core_clear_logging_callback(void) {
    if( !global_logging_mutex_created ) {
        assert_log(
            mutex_create( &global_logging_mutex ),
            "failed to create logging mutex!" );
        global_logging_mutex_created = true;
    }

    mutex_lock( &global_logging_mutex );

    global_logging_callback        = NULL;
    global_logging_callback_params = NULL;

    mutex_unlock( &global_logging_mutex );

}

attr_internal usize internal_stream_logging_message(
    void* target, usize count, const void* bytes
) {
    enum CoreLoggingLevel level = rcast( enum CoreLoggingLevel, target );
    global_logging_callback(
        level, count, (const char*)bytes,
        global_logging_callback_params );

    return 0;
}

attr_always_inline inline
attr_internal b32 internal_logging_level_valid( enum CoreLoggingLevel level ) {
    return bitfield_check( global_logging_level, level ) != 0;
}

void core_log(
    enum CoreLoggingLevel level, usize format_len,
    const char* format, ...
) {
    if( !global_logging_mutex_created ) {
        return;
    }
    mutex_lock( &global_logging_mutex );

    if( !internal_logging_level_valid( level ) || !global_logging_callback ) {
        mutex_unlock( &global_logging_mutex );
        return;
    }

    va_list va;
    va_start( va, format );

    fmt_text_va( internal_stream_logging_message, &level, format_len, format, va );

    va_end( va );

    mutex_unlock( &global_logging_mutex );
}

