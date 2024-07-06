/**
 * Description:  Path implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 26, 2024
*/
#include "core/prelude.h"
#include "core/path.h"
#include "core/string.h"
#include "core/memory.h"

attr_internal b32 internal_find_sep( Path path, usize* out_index ) {
#if defined( CORE_PLATFORM_WINDOWS )
    return string_find_set( path, string_text( "/\\" ), out_index );
#else
    return string_find( path, '/', out_index );
#endif
}
attr_internal b32 internal_find_sep_rev( Path path, usize* out_index ) {
#if defined( CORE_PLATFORM_WINDOWS )
    return string_find_set_rev( path, string_text( "/\\" ), out_index );
#else
    return string_find_rev( path, '/', out_index );
#endif
}

attr_core_api usize path_chunk_count( Path path ) {
    if( path_is_empty( path ) ) {
        return 0;
    }
    usize  res = 1;
    String substr = path;
    while( !path_is_empty( substr ) ) {
        usize sep = 0;
        if( internal_find_sep( substr, &sep ) ) {
            if( sep ) {
                res++;
            }
            substr = string_advance_by( substr, sep + 1 );
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
    String substr = path;
    while( !path_is_empty( substr ) ) {
        usize sep = 0;
        if( internal_find_sep( substr, &sep ) ) {
            Path chunk = substr;
            chunk.len  = sep;
            if( chunk.len ) {
                if( chunk_buf_cap != len ) {
                    chunk_buf[len++] = chunk;
                }
                count++;
            }
            substr = string_advance_by( substr, sep + 1 );
        } else {
            break;
        }
    }
    *out_count = len;
    return count - len;
}
attr_core_api b32 path_is_absolute( Path path ) {
    if( path_is_empty( path ) ) {
        return false;
    }
#if defined(CORE_PLATFORM_WINDOWS)
    if( path.len < 3 ) {
        return false;
    }
    return
        ascii_is_alphabetic( path.cc[0] ) &&
        path.cc[1] == ':' &&
        ascii_is_path_separator( path.cc[2] );
#else
    return path.cc[0] == '/';
#endif
}
attr_core_api b32 path_is_relative( Path path ) {
    if( path_is_empty( path ) ) {
        return false;
    }
    return path.cc[0] != '~' && !path_is_absolute( path );
}
attr_core_api b32 path_get_parent( Path path, Path* out_parent ) {
    if( path_is_empty( path ) ) {
        return false;
    }
    usize sep = 0;
    if( internal_find_sep_rev( path, &sep ) ) {
        String parent = string_truncate( path, sep );
        if( string_is_empty( parent ) ) {
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
    if( internal_find_sep_rev( path, &sep ) ) {
        String file_name = string_advance_by( path, sep + 1 );
        if( string_is_empty( file_name ) ) {
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
    if( string_find_rev( file_name, '.', &dot ) ) {
        *out_file_stem = string_truncate( file_name, dot );
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
    if( string_find( path, '.', &dot ) ) {
        if( !dot ) {
            return false;
        }

        String ext = string_advance_by( path, dot );
        if( string_is_empty( ext ) ) {
            return false;
        }

        *out_extension = ext;
        return true;
    } else {
        return false;
    }
}
attr_core_api void path_mut_set_separators( Path path, char sep ) {
    if( path_is_empty( path ) ) {
        return;
    }
    String substr = path;
    while( !string_is_empty( substr ) ) {
        usize sep_index = 0;
        if( internal_find_sep( substr, &sep_index ) ) {
            substr.c[sep_index] = sep;
            substr = string_advance_by( substr, sep_index + 1 );
        } else {
            break;
        }
    }
}
attr_core_api usize path_stream_set_separators(
    StreamBytesFN* stream, void* target, Path path, char sep
) {
    if( path_is_empty( path ) ) {
        return 0;
    }
    usize  res    = 0;
    String substr = path;
    while( !string_is_empty( substr ) ) {
        usize sep_index = 0;
        if( internal_find_sep( substr, &sep_index ) ) {
            String chunk = substr;
            chunk.len    = sep_index;
            res += stream( target, chunk.len, chunk.v );
            res += stream( target, 1, &sep );
            substr = string_advance_by( substr, chunk.len + 1 );
        } else {
            res += stream( target, substr.len, substr.v );
            break;
        }
    }
    return res;
}


attr_core_api b32 path_buf_try_push( PathBuf* buf, Path chunk ) {
    if( path_is_empty( chunk ) ) {
        return true;
    }
    b32   sep_required  = false;
    usize required_size = chunk.len;
    if( buf->len ) {
        if(
            !ascii_is_path_separator( buf->cc[buf->len - 1] ) &&
            !ascii_is_path_separator( chunk.cc[0] )
        ) {
            sep_required = true;
            required_size++;
        }
    }

    if( string_buf_remaining( buf ) < required_size ) {
        return false;
    }

    if( sep_required ) {
        string_buf_try_push( buf, '/' );
    }
    string_buf_try_append( buf, chunk );

    return true;
}
attr_core_api b32 path_buf_push(
    PathBuf* buf, Path chunk, struct AllocatorInterface* allocator
) {
    if( path_buf_try_push( buf, chunk ) ) {
        return true;
    }
    if( !string_buf_grow( buf, chunk.len + 16, allocator ) ) {
        return false;
    }
    return path_buf_try_push( buf, chunk );
}
attr_core_api b32 path_buf_pop( PathBuf* buf ) {
    Path parent = path_empty();
    if( path_get_parent( buf->slice, &parent ) ) {
        usize chunk_len = buf->len - parent.len;
        memory_zero( buf->c + parent.len, chunk_len );
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
    if( extension.cc[0] != '.' ) {
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

    if( string_buf_remaining( buf ) < required_size ) {
        return false;
    }

    if( ext_exists ) {
        memory_zero( ext.v, ext.len );
        buf->len -= ext.len;
    }

    if( !ext_has_dot ) {
        string_buf_try_push( buf, '.' );
    }
    return string_buf_try_append( buf, extension );
}
attr_core_api b32 path_buf_set_extension(
    PathBuf* buf, Path extension, struct AllocatorInterface* allocator
) {
    if( path_buf_try_set_extension( buf, extension ) ) {
        return true;
    }
    if( !string_buf_grow( buf, extension.len + 16, allocator ) ) {
        return false;
    }
    return path_buf_try_set_extension( buf, extension );
}

