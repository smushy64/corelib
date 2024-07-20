#if !defined(CORE_STREAM_H)
#define CORE_STREAM_H
/**
 * @file   stream.h
 * @brief  Data streaming functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 14, 2024
*/
#include "core/types.h"
#include "core/attributes.h"

/// @brief Function prototype for a streaming function.
/// @param[in] target (optional) Where to stream data to.
/// @param count Number of bytes to stream.
/// @param[in] bytes Data to stream.
/// @return Number of bytes that could not be streamed to target. Zero if successful.
typedef usize StreamBytesFN( void* target, usize count, const void* bytes );

/// @brief Stream text to either stdout or stderr
///
/// This function is meant to be used for any functions that
/// take in a #StreamBytesFN pointer.
/// @param[in] console_handle Handle to either stdout or stderr.
/// @param count Size of string to stream.
/// @param[in] bytes String to stream, does not need to be null-terminated.
/// @return Always zero.
/// @see core/fmt.h
/// @see #pipe_stdout()
/// @see #pipe_stderr()
attr_core_api usize stream_console(
    void* console_handle, usize count, const void* bytes );

#endif /* header guard */
