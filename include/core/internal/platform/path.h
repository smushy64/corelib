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
#include "core/path.h" // IWYU pragma: export

usize platform_path_chunk_count( struct _StringPOD path );

struct _StringPOD platform_path_clip_chunk( struct _StringPOD path );

struct _StringPOD platform_path_clip_chunk_last( struct _StringPOD path );

struct _StringPOD platform_path_advance_chunk( struct _StringPOD path );

struct _StringPOD platform_path_pop_chunk( struct _StringPOD path );

b32 platform_path_is_absolute( struct _StringPOD path );

b32 platform_path_parent( struct _StringPOD path, struct _StringPOD* out_parent );

b32 platform_path_file_name( struct _StringPOD path, struct _StringPOD* out_file_name );

b32 platform_path_stream_set_native_separators(
    StreamBytesFN* stream, void* target, struct _StringPOD path );

void platform_path_set_native_separators( struct _StringPOD path );

usize platform_path_stream_canonicalize(
    StreamBytesFN* stream, void* target, struct _StringPOD path );

b32 platform_path_buf_try_push_chunk( struct _StringBufPOD* buf, struct _StringPOD chunk );

b32 platform_path_buf_try_set_extension( struct _StringBufPOD* buf, struct _StringPOD extension );

#endif /* header guard */
