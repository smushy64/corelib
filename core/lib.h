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

/// @brief Logging level bitfield.
///
/// Bitfield that defines which log messages are allowed to pass through
/// to #CoreLoggingCallbackFN.
enum CoreLoggingLevel {
    /// @brief No logging messages.
    ///
    /// Disables formatting and streaming logging messages.
    CORE_LOGGING_LEVEL_NONE  = (0),
    /// @brief Only debug messages.
    CORE_LOGGING_LEVEL_DEBUG = (1 << 0),
    /// @brief Only info messages.
    CORE_LOGGING_LEVEL_INFO  = (1 << 1),
    /// @brief Only warning messages.
    CORE_LOGGING_LEVEL_WARN  = (1 << 2),
    /// @brief Only error messages.
    CORE_LOGGING_LEVEL_ERROR = (1 << 3),
};

/// @brief Function prototype for logging callback.
/// @param level Log level of message.
/// @param message_length Length of message.
/// @param[in] message Message.
/// @param[in] params Additional parameters.
typedef void CoreLoggingCallbackFN(
    enum CoreLoggingLevel level, usize message_length,
    const char* message, void* params );

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
/// @brief Get version of core lib as a string.
/// @param[out] opt_out_len (optional) Length of version string.
/// @return Version string.
attr_core_api const char* core_version_string( usize* opt_out_len );
/// @brief Get string that describes how library was built.
/// @param[out] opt_out_len (optional) Length of description string.
/// @return Null-terminated UTF-8 description string.
attr_core_api const char* core_build_description( usize* opt_out_len );
/// @brief Get string that describes command line used to build library.
/// @param[out] opt_out_len (optional) Length of command line string.
/// @return Null-terminated UTF-8 command line string.
attr_core_api const char* core_command_line( usize* opt_out_len );
/// @brief Set logging level.
///
/// If logging level is set to none, disables logging.
/// @note Logging can only be enabled if core library is compiled with CORE_ENABLE_LOGGING defined.
/// @param level Bitfield defining what logging messages are provided.
attr_core_api void core_set_logging_level( enum CoreLoggingLevel level );
/// @brief Query current logging level.
/// @return Bitfield of logging level.
attr_core_api enum CoreLoggingLevel core_query_logging_level(void);
/// @brief Set callback for receiving log messages.
/// @param[in] callback Function for receiving log messages.
/// @param[in] params Additional parameters for callback.
attr_core_api void core_set_logging_callback(
    CoreLoggingCallbackFN* callback, void* params );
/// @brief Clear logging callback.
attr_core_api void core_clear_logging_callback(void);

#endif /* header guard */
