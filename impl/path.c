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
usize path_chunk_count( _PathPOD path ) {
    return platform_path_chunk_count( path );
}
attr_core_api
_PathPOD path_clip_chunk( _PathPOD path ) {
    return platform_path_clip_chunk( path );
}
attr_core_api
_PathPOD path_clip_chunk_last( _PathPOD path ) {
    return platform_path_clip_chunk_last( path );
}
attr_core_api
_PathPOD path_advance_chunk( _PathPOD path ) {
    return platform_path_advance_chunk( path );
}
attr_core_api
_PathPOD path_pop_chunk( _PathPOD path ) {
    return platform_path_pop_chunk( path );
}
attr_core_api
usize path_split_chunks(
    _PathPOD path, usize chunk_buffer_cap, _PathPOD* chunk_buffer
) {
    usize count = path_chunk_count( path );
    usize max   = count;
    if( max > chunk_buffer_cap ) {
        max = chunk_buffer_cap;
    }

    _PathPOD remaining = path;
    for( usize i = 0; i < max; ++i ) {
        _PathPOD chunk  = path_clip_chunk( remaining );
        chunk_buffer[i] = chunk;

        remaining = path_advance_chunk( remaining );
    }

    return count - max;
}
attr_core_api
b32 path_is_absolute( _PathPOD path ) {
    return platform_path_is_absolute( path );
}
attr_core_api
b32 path_parent( _PathPOD path, _PathPOD* out_parent ) {
    return platform_path_parent( path, out_parent );
}
attr_core_api
b32 path_file_name( _PathPOD path, _PathPOD* out_file_name ) {
    return platform_path_file_name( path, out_file_name );
}
attr_core_api
b32 path_file_stem( _PathPOD path, _PathPOD* out_file_stem ) {
    if( !path_file_name( path, out_file_stem ) ) {
        return false;
    }

    _PathPOD ext;
    if( path_extension( *out_file_stem, &ext ) ) {
        *out_file_stem = string_trim( *out_file_stem, ext.len );
    }

    return true;
}
attr_core_api
b32 path_extension( _PathPOD path, _PathPOD* out_extension ) {
    usize pos = 0;
    if( !string_find_rev( path, '.', &pos ) ) {
        return false;
    }

    *out_extension = string_advance_by( path, pos );

    return true;
}
attr_core_api
usize path_stream_set_posix_separators( StreamBytesFN* stream, void* target, _PathPOD path ) {
    usize    result    = 0;
    _PathPOD remaining = path;

    char separator = '/';
    while( !path_is_empty( remaining ) ) {
        _PathPOD chunk = remaining;
        string_find( remaining, '\\', &chunk.len );

        result += stream( target, chunk.len, chunk.cbuf );
        result += stream( target, 1, &separator );

        remaining = string_advance_by( remaining, chunk.len + 1 );
    }
    return result;
}
attr_core_api
usize path_stream_set_windows_separators( StreamBytesFN* stream, void* target, _PathPOD path ) {
    usize    result    = 0;
    _PathPOD remaining = path;

    char separator = '\\';
    while( !path_is_empty( remaining ) ) {
        _PathPOD chunk = remaining;
        string_find( remaining, '/', &chunk.len );

        result += stream( target, chunk.len, chunk.cbuf );
        result += stream( target, 1, &separator );

        remaining = string_advance_by( remaining, chunk.len + 1 );
    }
    return result;
}
attr_core_api
usize path_stream_set_native_separators( StreamBytesFN* stream, void* target, _PathPOD path ) {
    return platform_path_stream_set_native_separators( stream, target, path );
}
attr_core_api
void path_set_posix_separators( _PathPOD path ) {
    _PathPOD remaining = path;
    while( !path_is_empty( remaining ) ) {
        _PathPOD chunk = remaining;
        if( !string_find( remaining, '\\', &chunk.len ) ) {
            break;
        }

        remaining        = string_advance_by( remaining, chunk.len );
        remaining.buf[0] = '/';
        remaining        = string_advance( remaining );
    }
}
attr_core_api
void path_set_windows_separators( _PathPOD path ) {
    _PathPOD remaining = path;
    while( !path_is_empty( remaining ) ) {
        _PathPOD chunk = remaining;
        if( !string_find( remaining, '/', &chunk.len ) ) {
            break;
        }

        remaining        = string_advance_by( remaining, chunk.len );
        remaining.buf[0] = '\\';
        remaining        = string_advance( remaining );
    }
}
attr_core_api
void path_set_native_separators( _PathPOD path ) {
    platform_path_set_native_separators( path );
}
attr_core_api
usize path_stream_canonicalize( StreamBytesFN* stream, void* target, _PathPOD path ) {
    return platform_path_stream_canonicalize( stream, target, path );
}
attr_core_api
b32 path_buf_try_push_chunk( _PathBufPOD* buf, _PathPOD chunk ) {
    return platform_path_buf_try_push_chunk( buf, chunk );
}
attr_core_api
b32 path_buf_push_chunk(
    struct AllocatorInterface* allocator, _PathBufPOD* buf, _PathPOD chunk
) {
    if( !path_buf_try_push_chunk( buf, chunk ) ) {
        if( !path_buf_grow( allocator, buf, chunk.len + 12 ) ) {
            return false;
        }
        path_buf_try_push_chunk( buf, chunk );
    }
    return true;
}
attr_core_api
b32 path_buf_pop_chunk( _PathBufPOD* buf ) {
    _PathPOD path = path_pop_chunk( buf->slice );
    if( path.len == buf->len ) {
        return false;
    }
    buf->slice = path;
    return true;
}
attr_core_api
b32 path_buf_try_set_extension( _PathBufPOD* buf, _PathPOD extension ) {
    return platform_path_buf_try_set_extension( buf, extension );
}
attr_core_api
b32 path_buf_set_extension(
    struct AllocatorInterface* allocator, _PathBufPOD* buf, _PathPOD extension
) {
    if( !path_buf_try_set_extension( buf, extension ) ) {
        if( !path_buf_grow( allocator, buf, extension.len + 12 ) ) {
            return false;
        }
        path_buf_try_set_extension( buf, extension );
    }
    return true;
}


#if 0

attr_core_api usize path_stream_convert_to_utf8(
    StreamBytesFN* stream, void* target, Path path
) {
    usize result = 0;
    for( usize i = 0; i < path.len; ++i ) {
        UTF16 utf16;
        utf16.shorts[0] = path.const_raw[i];
        if( i + 1 < path.len ) {
            utf16.len = 2;
            utf16.shorts[1] = path.const_raw[i + 1];
        } else {
            utf16.len = 1;
        }

        usize read_count = 0;
        c32 codepoint = utf16_to_codepoint( utf16, &read_count );
        if( !read_count || codepoint == UTF32_REPLACEMENT_CHARACTER ) {
            break;
        }
        i += read_count - 1;

        UTF8 utf8 = codepoint_to_utf8( codepoint );
        result += stream( target, utf8.len, utf8.bytes );
    }

    return result;
}
attr_core_api usize path_stream_convert_from_utf8(
    StreamBytesFN* stream, void* target, struct _StringPOD utf8_path
) {
    usize result = 0;
    struct _StringPOD rem = utf8_path;
    while( !string_is_empty( rem ) ) {
        c32 codepoint = UTF32_REPLACEMENT_CHARACTER;
        rem = string_utf8_next( rem, &codepoint );

        UTF16 utf16 = codepoint_to_utf16( codepoint );
        if( utf16.len == 1 && utf16.shorts[0] == 0xFFFD ) {
            break;
        }

        result += stream( target, sizeof(c16) * utf16.len, utf16.bytes );
    }
    return result;
}
attr_core_api b32 path_buf_from_string(
    struct _StringPOD utf8_path, struct AllocatorInterface* allocator, PathBuf* out_buf 
) {
    if( string_is_empty(utf8_path) ) {
        return false;
    }
    usize buf_cap = path_stream_convert_from_utf8( path_buf_stream, NULL, utf8_path );
    if( !path_buf_from_alloc( buf_cap, allocator, out_buf ) ) {
        return false;
    }
    path_stream_convert_from_utf8( path_buf_stream, out_buf, utf8_path );
    return true;
}
attr_core_api b32 path_buf_copy_from_string(
    PathBuf* buf, struct _StringPOD utf8_path, struct AllocatorInterface* allocator
) {
    if( string_is_empty( utf8_path ) ) {
        return false;
    }
    usize path_cap = path_stream_convert_from_utf8( path_buf_stream, NULL, utf8_path );
    if( path_cap > buf->cap - 1 ) {
        if( !path_buf_grow( buf, path_cap - (buf->cap - 1), allocator)) {
            return false;
        }
    }
    buf->len = 0;
    path_stream_convert_from_utf8( path_buf_stream, buf, utf8_path );
    return true;
}
attr_core_api b32 path_buf_try_copy_from_string( PathBuf* buf, struct _StringPOD utf8_path ) {
    if( string_is_empty( utf8_path ) ) {
        return false;
    }
    usize buf_cap = path_stream_convert_from_utf8( path_buf_stream, NULL, utf8_path );
    if( buf_cap > buf->cap - 1 ) {
        return false;
    }
    buf->len = 0;
    path_stream_convert_from_utf8( path_buf_stream, buf, utf8_path );
    return true;
}

attr_core_api b32 path_is_absolute( Path path ) {
    if( path.len < 3 ) {
        return false;
    }
    // always ascii
    char drive_letter = path.const_raw[0];
    PathCharacter drive_sep    = path.const_raw[1];
    PathCharacter sep          = path.const_raw[2];

    if( drive_sep != path_raw_char(':')) {
        return false;
    }
    if( !(sep == path_raw_char('/') || sep == path_raw_char('\\'))) {
        return false;
    }
    
    return
        ( drive_letter >= 'A' && drive_letter <= 'Z' ) ||
        ( drive_letter >= 'a' && drive_letter <= 'z' );
}

#endif

