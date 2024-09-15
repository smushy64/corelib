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

#endif /* header guard */
