/**
 * Description:  Path implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 26, 2024
*/
#include "core/prelude.h"
#include "core/path.h"
#include "core/alloc.h"

#include "core/internal/platform/path.h"

attr_core_api
usize path_chunk_count( struct _StringPOD path ) {
    return platform_path_chunk_count( path );
}
attr_core_api
struct _StringPOD path_clip_chunk( struct _StringPOD path ) {
    return platform_path_clip_chunk( path );
}
attr_core_api
struct _StringPOD path_clip_chunk_last( struct _StringPOD path ) {
    return platform_path_clip_chunk_last( path );
}
attr_core_api
struct _StringPOD path_advance_chunk( struct _StringPOD path ) {
    return platform_path_advance_chunk( path );
}
attr_core_api
struct _StringPOD path_pop_chunk( struct _StringPOD path ) {
    return platform_path_pop_chunk( path );
}
attr_core_api
usize path_split_chunks(
    struct _StringPOD path, usize chunk_buffer_cap, struct _StringPOD* chunk_buffer
) {
    usize count = path_chunk_count( path );
    usize max   = count;
    if( max > chunk_buffer_cap ) {
        max = chunk_buffer_cap;
    }

    struct _StringPOD remaining = path;
    for( usize i = 0; i < max; ++i ) {
        struct _StringPOD chunk  = path_clip_chunk( remaining );
        chunk_buffer[i] = chunk;

        remaining = path_advance_chunk( remaining );
    }

    return count - max;
}
attr_core_api
b32 path_is_absolute( struct _StringPOD path ) {
    return platform_path_is_absolute( path );
}
attr_core_api
b32 path_parent( struct _StringPOD path, struct _StringPOD* out_parent ) {
    return platform_path_parent( path, out_parent );
}
attr_core_api
b32 path_file_name( struct _StringPOD path, struct _StringPOD* out_file_name ) {
    return platform_path_file_name( path, out_file_name );
}
attr_core_api
b32 path_file_stem( struct _StringPOD path, struct _StringPOD* out_file_stem ) {
    if( !path_file_name( path, out_file_stem ) ) {
        return false;
    }

    struct _StringPOD ext;
    if( path_extension( *out_file_stem, &ext ) ) {
        *out_file_stem = string_trim( *out_file_stem, ext.len );
    }

    return true;
}
attr_core_api
b32 path_extension( struct _StringPOD path, struct _StringPOD* out_extension ) {
    usize pos = 0;
    if( !string_find_rev( path, '.', &pos ) ) {
        return false;
    }

    *out_extension = string_advance_by( path, pos );

    return true;
}
attr_core_api
usize path_stream_set_posix_separators( StreamBytesFN* stream, void* target, struct _StringPOD path ) {
    usize    result    = 0;
    struct _StringPOD remaining = path;

    char separator = '/';
    while( !string_is_empty( remaining ) ) {
        struct _StringPOD chunk = remaining;
        string_find( remaining, '\\', &chunk.len );

        result += stream( target, chunk.len, chunk.cbuf );
        result += stream( target, 1, &separator );

        remaining = string_advance_by( remaining, chunk.len + 1 );
    }
    return result;
}
attr_core_api
usize path_stream_set_windows_separators( StreamBytesFN* stream, void* target, struct _StringPOD path ) {
    usize    result    = 0;
    struct _StringPOD remaining = path;

    char separator = '\\';
    while( !string_is_empty( remaining ) ) {
        struct _StringPOD chunk = remaining;
        string_find( remaining, '/', &chunk.len );

        result += stream( target, chunk.len, chunk.cbuf );
        result += stream( target, 1, &separator );

        remaining = string_advance_by( remaining, chunk.len + 1 );
    }
    return result;
}
attr_core_api
usize path_stream_set_native_separators( StreamBytesFN* stream, void* target, struct _StringPOD path ) {
    return platform_path_stream_set_native_separators( stream, target, path );
}
attr_core_api
void path_set_posix_separators( struct _StringPOD path ) {
    struct _StringPOD remaining = path;
    while( !string_is_empty( remaining ) ) {
        struct _StringPOD chunk = remaining;
        if( !string_find( remaining, '\\', &chunk.len ) ) {
            break;
        }

        remaining        = string_advance_by( remaining, chunk.len );
        remaining.buf[0] = '/';
        remaining        = string_advance( remaining );
    }
}
attr_core_api
void path_set_windows_separators( struct _StringPOD path ) {
    struct _StringPOD remaining = path;
    while( !string_is_empty( remaining ) ) {
        struct _StringPOD chunk = remaining;
        if( !string_find( remaining, '/', &chunk.len ) ) {
            break;
        }

        remaining        = string_advance_by( remaining, chunk.len );
        remaining.buf[0] = '\\';
        remaining        = string_advance( remaining );
    }
}
attr_core_api
void path_set_native_separators( struct _StringPOD path ) {
    platform_path_set_native_separators( path );
}
attr_core_api
usize path_stream_canonicalize( StreamBytesFN* stream, void* target, struct _StringPOD path ) {
    return platform_path_stream_canonicalize( stream, target, path );
}
attr_core_api
b32 path_buf_try_push_chunk( struct _StringBufPOD* buf, struct _StringPOD chunk ) {
    return platform_path_buf_try_push_chunk( buf, chunk );
}
attr_core_api
b32 path_buf_push_chunk(
    struct AllocatorInterface* allocator, struct _StringBufPOD* buf, struct _StringPOD chunk
) {
    if( !path_buf_try_push_chunk( buf, chunk ) ) {
        if( !string_buf_grow( allocator, buf, chunk.len + 12 ) ) {
            return false;
        }
        path_buf_try_push_chunk( buf, chunk );
    }
    return true;
}
attr_core_api
b32 path_buf_pop_chunk( struct _StringBufPOD* buf ) {
    struct _StringPOD path = path_pop_chunk( buf->slice );
    if( path.len == buf->len ) {
        return false;
    }
    buf->slice = path;
    return true;
}
attr_core_api
b32 path_buf_try_set_extension( struct _StringBufPOD* buf, struct _StringPOD extension ) {
    return platform_path_buf_try_set_extension( buf, extension );
}
attr_core_api
b32 path_buf_set_extension(
    struct AllocatorInterface* allocator, struct _StringBufPOD* buf, struct _StringPOD extension
) {
    if( !path_buf_try_set_extension( buf, extension ) ) {
        if( !string_buf_grow( allocator, buf, extension.len + 12 ) ) {
            return false;
        }
        path_buf_try_set_extension( buf, extension );
    }
    return true;
}

