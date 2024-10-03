/**
 * Description:  Path implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 26, 2024
*/
#include "core/prelude.h"
#include "core/path.h"
#include "core/memory.h"
#include "core/alloc.h"
#include "core/internal/platform.h"
#include "core/unicode.h"

attr_core_api usize path_raw_len( const PathCharacter* raw ) {
    if( !raw ) {
        return 0;
    }
    usize res = 0;
    const PathCharacter* at = raw;
    while( *at++ ) {
        res++;
    }
    return res;
}
attr_core_api b32 path_cmp( Path a, Path b ) {
    if( a.len != b.len ) {
        return false;
    }
    return memory_cmp( a.raw, b.raw, sizeof(PathCharacter) * a.len );
}

#if defined(CORE_PLATFORM_WINDOWS)

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
    StreamBytesFN* stream, void* target, String utf8_path
) {
    usize result = 0;
    String rem = utf8_path;
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
    String utf8_path, struct AllocatorInterface* allocator, PathBuf* out_buf 
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
    PathBuf* buf, String utf8_path, struct AllocatorInterface* allocator
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
attr_core_api b32 path_buf_try_copy_from_string( PathBuf* buf, String utf8_path ) {
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

#else
attr_core_api usize path_stream_convert_to_utf8(
    StreamBytesFN* stream, void* target, Path path
) {
    return stream( target, path.len, path.const_raw );
}
attr_core_api usize path_stream_convert_from_utf8(
    StreamBytesFN* stream, void* target, String utf8_path 
) {
    return stream( target, utf8_path.len, utf8_path.cc );
}
attr_core_api b32 path_buf_from_string(
    String utf8_path, struct AllocatorInterface* allocator, PathBuf* out_buf 
) {
    PathBuf result = {0};
    if( !path_buf_from_alloc( utf8_path.len, allocator, &result ) ) {
        return false;
    }
    memory_copy( result.raw, utf8_path.cc, utf8_path.len );
    result.len = utf8_path.len;
    *out_buf = result;
    return true;
}
attr_core_api b32 path_buf_copy_from_string(
    PathBuf* buf, String utf8_path, AllocatorInterface* allocator
) {
    if( (buf->cap - 1) < utf8_path.len ) {
        if( !path_buf_grow( buf, utf8_path.len - (buf->cap - 1), allocator )) {
            return false;
        }
    }
    memory_copy( buf->raw, utf8_path.cc, utf8_path.len );
    buf->len = utf8_path.len;
    buf->raw[buf->len] = 0;

    return true;
}
attr_core_api b32 path_buf_try_copy_from_string( PathBuf* buf, String utf8_path ) {
    if( buf->cap < utf8_path.len ) {
        return false;
    }
    memory_copy( buf->raw, utf8_path.cc, utf8_path.len );
    buf->len = utf8_path.len;
    buf->raw[buf->len] = 0;
    return true;
}
#endif

attr_core_api void path_set_posix_separators( Path path ) {
    for( usize i = 0; i < path.len; ++i ) {
        if( path.raw[i] == path_raw_char('\\') ) {
            path.raw[i] = path_raw_char('/');
        }
    }
}
attr_core_api void path_set_windows_separators( Path path ) {
    for( usize i = 0; i < path.len; ++i ) {
        if( path.raw[i] == path_raw_char('/') ) {
            path.raw[i] = path_raw_char('\\');
        }
    }
}
attr_core_api usize path_stream_set_posix_separators(
    StreamBytesFN* stream, void* target, Path path 
) {
    usize result = 0;
    for( usize i = 0; i < path.len; ++i ) {
        PathCharacter c = path.const_raw[i];
        if( c == path_raw_char('\\') ) {
            c = path_raw_char('/');
        }
        result += stream( target, sizeof(c), &c );
    }
    return result;
}
attr_core_api usize path_stream_set_windows_separators(
    StreamBytesFN* stream, void* target, Path path 
) {
    usize result = 0;
    for( usize i = 0; i < path.len; ++i ) {
        PathCharacter c = path.const_raw[i];
        if( c == path_raw_char('/') ) {
            c = path_raw_char('\\');
        }
        result += stream( target, sizeof(c), &c );
    }
    return result;
}

b32 internal_path_find( Path path, PathCharacter pc, usize* out_index ) {
    for( usize i = 0; i < path.len; ++i ) {
        PathCharacter c = path.const_raw[i];
        if( c == pc ) {
            *out_index = i;
            return true;
        }
    }
    return false;
}
b32 internal_path_find_rev(
    Path path, PathCharacter pc, usize* out_index
) {
    for( usize i = path.len; i-- > 0; ) {
        PathCharacter c = path.const_raw[i];
        if( c == pc ) {
            *out_index = i;
            return true;
        }
    }
    return false;
}
#if defined(CORE_PLATFORM_WINDOWS)
b32 internal_path_is_sep( PathCharacter c ) {
    return c == path_raw_char('/') || c == path_raw_char('\\');
}
b32 internal_path_find_sep( Path path, usize* out_index ) {
    for( usize i = 0; i < path.len; ++i ) {
        PathCharacter c = path.const_raw[i];
        if( internal_path_is_sep( c ) ) {
            *out_index = i;
            return true;
        }
    }
    return false;
}
b32 internal_path_find_sep_rev( Path path, usize* out_index ) {
    for( usize i = path.len; i-- > 0; ) {
        PathCharacter c = path.const_raw[i];
        if( internal_path_is_sep( c ) ) {
            *out_index = i;
            return true;
        }
    }
    return false;
}
#else
b32 internal_path_is_sep( PathCharacter c ) {
    return c == path_raw_char( '/' );
}
b32 internal_path_find_sep( Path path, usize* out_index ) {
    return internal_path_find( path, path_raw_char('/'), out_index );
}
b32 internal_path_find_sep_rev( Path path, usize* out_index ) {
    return internal_path_find_rev( path, path_raw_char('/'), out_index );
}
#endif

Path internal_path_advance_by( Path path, usize count ) {
    Path result = path;
    if( path.len < count ) {
        result.const_raw += result.len;
        result.len = 0;
    } else {
        result.const_raw += count;
        result.len -= count;
    }
    return result;
}
Path internal_path_advance( Path path ) {
    return internal_path_advance_by( path, 1 );
}

attr_core_api usize path_chunk_count( Path path ) {
    if( path_is_empty( path ) ) {
        return 0;
    }
    usize  res = 1;
    Path subpath = path;
    while( !path_is_empty( subpath ) ) {
        usize sep = 0;
        if( internal_path_find_sep( subpath, &sep ) ) {
            if( sep ) {
                res++;
            }
            subpath = internal_path_advance_by( subpath, sep + 1 );
        } else {
            break;
        }
    }
    return res;
}
attr_core_api usize path_split_chunks(
    Path path, usize chunk_buf_cap, Path* chunk_buf, usize* out_count
) {
    if( path_is_empty( path ) ) {
        return 0;
    }
    usize  count  = 0;
    usize  len    = 0;
    Path subpath = path;
    while( !path_is_empty( subpath ) ) {
        usize sep = 0;
        if( internal_path_find_sep( subpath, &sep ) ) {
            Path chunk = subpath;
            chunk.len  = sep;
            if( chunk.len ) {
                if( chunk_buf_cap != len ) {
                    chunk_buf[len++] = chunk;
                }
                count++;
            }
            subpath = internal_path_advance_by( subpath, sep + 1 );
        } else {
            break;
        }
    }
    *out_count = len;
    return count - len;
}
attr_core_api b32 path_is_absolute( Path path ) {
#if defined(CORE_PLATFORM_WINDOWS)
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
#else
    if( path_is_empty( path ) ) {
        return false;
    }
    return path.const_raw[0] == path_raw_char('/');
#endif
}
attr_core_api b32 path_is_relative( Path path ) {
    if( path_is_empty( path ) ) {
        return false;
    }
    return path.const_raw[0] != path_raw_char('~') && !path_is_absolute( path );
}
attr_core_api b32 path_get_parent( Path path, Path* out_parent ) {
    if( path_is_empty( path ) ) {
        return false;
    }
    usize sep = 0;
    if( internal_path_find_sep_rev( path, &sep ) ) {
        Path parent = path;
        parent.len  = sep;
        if( path_is_empty( parent ) ) {
            return false;
        }

        *out_parent = parent;
        return true;
    }

    return false;
}
attr_core_api b32 path_get_file_name( Path path, Path* out_file_name ) {
    if( path_is_empty( path ) ) {
        return false;
    }
    usize sep = 0;
    if( internal_path_find_sep_rev( path, &sep ) ) {
        Path file_name = internal_path_advance_by( path, sep + 1 );
        if( path_is_empty( file_name ) ) {
            return false;
        }
        *out_file_name = file_name;
        return true;
    } else {
        *out_file_name = path;
        return true;
    }
}
attr_core_api b32 path_get_file_stem( Path path, Path* out_file_stem ) {
    Path file_name = path_empty();
    if( !path_get_file_name( path, &file_name ) ) {
        return false;
    }
    usize dot = 0;
    if( internal_path_find_rev( file_name, '.', &dot ) ) {
        Path file_stem = file_name;
        file_stem.len = dot;
        *out_file_stem = file_stem;
        return true;
    } else {
        *out_file_stem = file_name;
        return true;
    }
}
attr_core_api b32 path_get_extension( Path path, Path* out_extension ) {
    if( path_is_empty( path ) ) {
        return false;
    }
    usize dot = 0;
    if( internal_path_find_rev( path, '.', &dot ) ) {
        if( !dot ) {
            return false;
        }

        Path ext = internal_path_advance_by( path, dot );
        if( path_is_empty( ext ) ) {
            return false;
        }

        *out_extension = ext;
        return true;
    } else {
        return false;
    }
}
attr_core_api b32 path_buf_from_alloc(
    usize capacity, struct AllocatorInterface* allocator, PathBuf* out_buf
) {
    usize _size = capacity + 1;
    void* ptr   = allocator->alloc( sizeof(PathCharacter) * _size, 0, allocator->ctx );
    if( !ptr ) {
        return false;
    }
    out_buf->cap = _size;
    out_buf->len = 0;
    out_buf->raw = ptr;
    return true;
}
attr_core_api b32 path_buf_from_path(
    Path path, struct AllocatorInterface* allocator, PathBuf* out_buf
) {
    if( !path_buf_from_alloc( path.len + 16, allocator, out_buf ) ) {
        return false;
    }
    memory_copy( out_buf->raw, path.const_raw, sizeof(PathCharacter) * path.len );
    out_buf->len = path.len;
    return true;
}
attr_core_api b32 path_buf_copy_from_path(
    PathBuf* buf, Path path, struct AllocatorInterface* allocator
) {
    if( !buf->cap || (buf->cap - 1) < path.len ) {
        usize diff = !buf->cap ? path.len : (path.len - (buf->cap - 1));
        if( !path_buf_grow( buf, diff, allocator ) ) {
            return false;
        }
    }
    buf->len = 0;
    memory_copy( buf->raw, path.const_raw, sizeof(PathCharacter) * path.len );
    buf->len = path.len;
    buf->raw[buf->len] = path_raw_char('\0');
    return true;
}
attr_core_api b32 path_buf_try_copy_from_path( PathBuf* buf, Path path ) {
    if( !buf->cap || (buf->cap - 1) < path.len ) {
        return false;
    }
    buf->len = 0;
    memory_copy( buf->raw, path.const_raw, sizeof(PathCharacter) * path.len );
    buf->len = path.len;
    buf->raw[buf->len] = path_raw_char('\0');
    return true;
}
attr_core_api b32 path_buf_grow(
    PathBuf* buf, usize amount, struct AllocatorInterface* allocator
) {
    usize old_size = sizeof(PathCharacter) * buf->cap;
    usize new_size = sizeof(PathCharacter) * (buf->cap + amount);
    void* ptr = allocator->realloc( buf->raw, old_size, new_size, 0, allocator->ctx );
    if( !ptr ) {
        return false;
    }
    buf->raw = ptr;
    buf->cap += amount;
    return true;
}
attr_core_api void path_buf_free(
    PathBuf* buf, struct AllocatorInterface* allocator
) {
    if( buf && buf->raw ) {
        allocator->free( buf->raw, sizeof(PathCharacter) * buf->cap, 0, allocator->ctx );
        memory_zero( buf, sizeof(*buf) );
    }
}
attr_core_api b32 path_buf_try_push( PathBuf* buf, Path chunk ) {
    if( path_is_empty( chunk ) ) {
        return true;
    }
    b32   sep_required  = false;
    usize required_size = chunk.len;
    if( buf->len ) {
        if(
            !internal_path_is_sep( buf->const_raw[buf->len - 1] ) &&
            !internal_path_is_sep( chunk.const_raw[0] )
        ) {
            sep_required = true;
            required_size++;
        }
    }

    if( path_buf_remaining( buf ) < required_size ) {
        return false;
    }

    if( sep_required ) {
        buf->raw[buf->len++] = PATH_SEPARATOR;
    }
    memory_copy(
        buf->raw + buf->len, chunk.const_raw,
        sizeof(PathCharacter) * chunk.len );
    buf->len += chunk.len;
    buf->raw[buf->len] = path_raw_char( '\0' );

    return true;
}
attr_core_api b32 path_buf_push(
    PathBuf* buf, Path chunk, struct AllocatorInterface* allocator
) {
    if( path_buf_try_push( buf, chunk ) ) {
        return true;
    }
    if( !path_buf_grow( buf, chunk.len + 16, allocator ) ) {
        return false;
    }
    return path_buf_try_push( buf, chunk );
}
attr_core_api b32 path_buf_pop( PathBuf* buf ) {
    Path parent = path_empty();
    if( path_get_parent( buf->slice, &parent ) ) {
        usize chunk_len = buf->len - parent.len;
        memory_zero( buf->raw + parent.len, sizeof(PathCharacter) * chunk_len );
        buf->len -= chunk_len;
        return true;
    } else {
        return false;
    }
}
attr_core_api b32 path_buf_try_set_extension( PathBuf* buf, Path extension ) {
    if( path_is_empty( buf->slice ) ) {
        return false;
    }
    if( path_is_empty( extension ) ) {
        // nothing to do . . .
        return true;
    }

    b32   ext_has_dot = true;
    usize required_size = extension.len;
    if( extension.const_raw[0] != path_raw_char('.') ) {
        required_size++;
        ext_has_dot = false;
    }

    b32  ext_exists = false;
    Path ext        = path_empty();
    if( path_get_extension( buf->slice, &ext ) ) {
        if( ext.len > required_size ) {
            required_size = 0;
        } else {
            required_size -= ext.len;
        }

        ext_exists = true;
    }

    if( path_buf_remaining( buf ) < required_size ) {
        return false;
    }

    if( ext_exists ) {
        memory_zero( ext.raw, sizeof(PathCharacter) * ext.len );
        buf->len -= ext.len;
    }

    if( !ext_has_dot ) {
        buf->raw[buf->len++] = path_raw_char( '.' );
        buf->raw[buf->len] = path_raw_char( '\0' );
    }

    memory_copy( buf->raw, extension.const_raw, sizeof(PathCharacter) * extension.len );
    buf->len += extension.len;
    buf->raw[buf->len] = path_raw_char( '\0' );

    return true;
}
attr_core_api b32 path_buf_set_extension(
    PathBuf* buf, Path extension, struct AllocatorInterface* allocator
) {
    if( path_buf_try_set_extension( buf, extension ) ) {
        return true;
    }
    if( !path_buf_grow( buf, extension.len + 16, allocator ) ) {
        return false;
    }
    return path_buf_try_set_extension( buf, extension );
}

attr_core_api usize path_stream_canonicalize(
    StreamBytesFN* stream, void* target, Path path
) {
    return platform_path_stream_canonicalize( stream, target, path );
}
#if defined(CORE_PLATFORM_WINDOWS)
attr_core_api usize path_stream_canonicalize_utf8(
    StreamBytesFN* stream, void* target, Path path 
) {
    return platform_path_stream_canonicalize_utf8( stream, target, path );
}
#endif

attr_core_api usize path_buf_stream(
    void* target_PathBuf, usize bytes, const void* buffer
) {
    if( !target_PathBuf ) {
        return bytes;
    }

    PathBuf* buf = target_PathBuf;
    if( !bytes ) {
        return 0;
    }

    usize characters = bytes / sizeof(PathCharacter);
    usize remaining  = path_buf_remaining( buf );

    usize push_count = 0;
    if( remaining < characters ) {
        push_count = remaining;
    } else {
        push_count = characters;
    }

    memory_copy( buf->raw + buf->len, buffer, sizeof(PathCharacter) * push_count );
    buf->len += push_count;
    buf->raw[buf->len] = path_raw_char('\0');

    return bytes - (sizeof(PathCharacter) * push_count);
}

