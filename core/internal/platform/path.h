#if !defined(CORE_INTERNAL_PLATFORM_PATH_H) && defined(CORE_ENABLE_INTERNAL)
#define CORE_INTERNAL_PLATFORM_PATH_H
/**
 * @file   path.h
 * @brief  Platform-specific path functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 06, 2025
*/
#include "core/types.h"
#include "core/stream.h"
#include "core/path.h"

usize platform_path_chunk_count( _PathPOD path );

_PathPOD platform_path_clip_chunk( _PathPOD path );

_PathPOD platform_path_clip_chunk_last( _PathPOD path );

_PathPOD platform_path_advance_chunk( _PathPOD path );

_PathPOD platform_path_pop_chunk( _PathPOD path );

b32 platform_path_is_absolute( _PathPOD path );

b32 platform_path_parent( _PathPOD path, _PathPOD* out_parent );

b32 platform_path_file_name( _PathPOD path, _PathPOD* out_file_name );

b32 platform_path_stream_set_native_separators(
    StreamBytesFN* stream, void* target, _PathPOD path );

void platform_path_set_native_separators( _PathPOD path );

usize platform_path_stream_canonicalize(
    StreamBytesFN* stream, void* target, _PathPOD path );

b32 platform_path_buf_try_push_chunk( _PathBufPOD* buf, _PathPOD chunk );

b32 platform_path_buf_try_set_extension( _PathBufPOD* buf, _PathPOD extension );

#endif /* header guard */
