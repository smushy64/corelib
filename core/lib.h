#if !defined(CORE_LIB_H)
#define CORE_LIB_H
/**
 * @file   lib.h
 * @brief  Core library management.
 *
 * Get build version, description and set logging level.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   January 31, 2024
*/
#include "core/attributes.h"
#include "core/types.h"

/// @brief Logging level.
typedef enum CoreLoggingLevel {
    /// @brief Do not log messages.
    CORE_LOGGING_LEVEL_NONE,
    /// @brief Only log error messages.
    CORE_LOGGING_LEVEL_ERROR,
    /// @brief Log warning and error messages.
    CORE_LOGGING_LEVEL_WARN,
} CoreLoggingLevel;

/// @brief Function prototype for logging callback.
/// @details
/// Logging messages are streamed without formatting,
/// callback needs to use fmt_text_va or print_text_va to format messages.
/// Messages never include new-line at the end of stream.
/// To prevent cross-talk between threads, use a mutex.
/// @param     level       Level of message. Can be error or warning.
/// @param     len         Length of unformatted message.
/// @param[in] msg         Read-only pointer to unformatted message.
/// @param[in] va          Variadic argument list.
/// @param[in] user_params User parameters.
typedef void CoreLoggingCallbackFN(
    CoreLoggingLevel level, usize len, const char* msg,
    va_list va, void* user_params );

/// @brief Get version of core lib.
///
/// High short:           Major version.
/// Low short, high byte: Minor version.
/// Low short, low byte:  Patch version.
///
/// Use #core_get_major(), #core_get_minor() and #core_get_patch() macros defined
/// in core/macros.h to extract major, minor and patch version numbers.
/// @return Version integer.
attr_core_api u32 core_version(void);
/// @brief Get string that describes command line used to build library.
/// @param[out] opt_out_len (optional) Length of command line string.
/// @return Null-terminated UTF-8 command line string.
attr_core_api const char* core_command_line( usize* opt_out_len );
/// @brief Set logging level.
/// @details
/// By default, the logging level is CORE_LOGGING_LEVEL_NONE,
/// in other words, logging is disabled.
/// @param level Enum defining what logging messages are sent.
/// @note Logging can only be enabled if
/// core library is compiled with CORE_ENABLE_LOGGING defined.
attr_core_api void core_set_logging_level( CoreLoggingLevel level );
/// @brief Query current logging level.
/// @return Logging level.
attr_core_api CoreLoggingLevel core_query_logging_level(void);
/// @brief Set callback for receiving log messages.
/// @param[in] callback Function for receiving log messages.
/// @param[in] user_params User parameters for callback.
attr_core_api void core_set_logging_callback(
    CoreLoggingCallbackFN* callback, void* user_params );
/// @brief Clear logging callback.
attr_core_api void core_clear_logging_callback(void);

#endif /* header guard */
