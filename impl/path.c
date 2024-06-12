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
        memory_zero( parent.v, parent.len );
        buf->len -= parent.len;
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

#if 0

attr_core_api b32 path_is_absolute( const Path path ) {
#if defined(CORE_PLATFORM_POSIX)
    return path.len && path.cc[0] == '/';
#elif defined(CORE_PLATFORM_WINDOWS)
    if( path.len >= 3 ) { /* [a-zA-Z]:[/\\] */
        return path.cc[1] == ':' && (path.cc[2] == '/' || path.cc[2] == '\\');
    }
    return false;
#else
    #error "Undefined path_is_absolute for current platform!"
#endif
}
attr_core_api b32 path_is_file( const Path path ) {
    return platform_path_is_file( path );
}
attr_core_api b32 path_is_directory( const Path path ) {
    return platform_path_is_directory( path );
}
attr_core_api b32 path_parent( const Path path, Path* out_parent ) {
    b32   parent_found   = false;
    usize last_separator = 0;
#if defined(CORE_PLATFORM_WINDOWS)
    // TODO(alicia): maybe i need a multi-find function?
    parent_found               = string_find_rev( path, '/', &last_separator );
    usize other_last_separator = 0;
    b32 other_found            = string_find_rev( path, '\\', &other_last_separator );
    parent_found               = parent_found || other_found;
    if( other_found ) {
        last_separator = last_separator > other_last_separator ?
            last_separator : other_last_separator;
    }

#else
    parent_found = string_find_rev( path, '/', &last_separator );
#endif

    if( !parent_found ) {
        return false;
    }

    out_parent->cc  = path.cc;
    out_parent->len = last_separator;
    return true;
}
attr_core_api b32 path_file_name( const Path path, Path* out_file_name ) {
    if( !path.len ) {
        return false;
    }
    // nasty, nasty code
    if( path.len <= 2 ) {
        if( path.len == 1 ) {
            if( path.cc[0] == '.' || path.cc[0] == '~' ) {
                return false;
            }
        } else {
            if( path.cc[0] == '.' && path.cc[1] == '.' ) {
                return false;
            }
#if defined(CORE_PLATFORM_WINDOWS)
            if( path.cc[1] == ':' ) {
                return false;
            }
#endif
        }
    }

    b32 has_separator    = false;
    usize last_separator = 0;
    char last_char       = string_last_unchecked( path );

#if defined(CORE_PLATFORM_WINDOWS)
    if( last_char == '/' || last_char == '\\' ) {
        return false;
    }

    usize other_last_separator = 0;
    has_separator = string_find_rev( path, '/', &last_separator );
    has_separator = has_separator ||
        string_find_rev( path, '\\', &other_last_separator );

    last_separator = last_separator > other_last_separator ?
        last_separator : other_last_separator;

#else
    if( last_char == '/' ) {
        return false;
    }
    has_separator = string_find_rev( path, '/', &last_separator );
#endif

    if( has_separator ) {
        out_file_name->cc  = path.cc  + last_separator + 1;
        out_file_name->len = path.len - last_separator;
        out_file_name->len =
            out_file_name->len ? out_file_name->len - 1 : out_file_name->len;
    } else {
        *out_file_name = path;
    }

    return true;
}
attr_core_api b32 path_file_stem( const Path path, Path* out_file_stem ) {
    Path file_stem = path_new(0, 0);
    if( !path_file_name( path, &file_stem ) ) {
        return false;
    }
    usize dot_pos = 0;
    if(
        string_find_rev( file_stem, '.', &dot_pos ) &&
        dot_pos // NOTE(alicia): this is so that dot files don't get overlooked
    ) {
        file_stem.len = dot_pos;
    }
    if( !file_stem.len ) {
        return false;
    }

    *out_file_stem = file_stem;
    return true;
}
attr_core_api b32 path_extension( const Path path, Path* out_extension ) {
    Path file_name = path_new( 0, 0 );
    if( !path_file_name( path, &file_name ) ) {
        return false;
    }

    usize dot_pos = 0;
    if( !string_find_rev( file_name, '.', &dot_pos ) ) {
        return false;
    }

    out_extension->cc  = file_name.cc  + dot_pos;
    out_extension->len = file_name.len - dot_pos;
    if( out_extension->len <= 1 ) {
        return false;
    }

    return true;
}
attr_core_api b32 path_pop( const Path path, Path* out_path, Path* opt_out_chunk ) {
    Path parent;
    if( !path_parent( path, &parent ) ) {
        return false;
    }

    *out_path = parent;
    if( opt_out_chunk ) {
        opt_out_chunk->cc  = parent.cc + parent.len;
        opt_out_chunk->len = path.len - parent.len;
        if( opt_out_chunk->len ) {
            opt_out_chunk->cc++;
            opt_out_chunk->len--;
        }
    }
    return true;
}
attr_core_api void path_convert_separators( Path* path, b32 to_forward ) {
    char sep    = to_forward ? '/' : '\\';
    char search = to_forward ? '\\' : '/';
    String rem = *path;

    loop() {
        usize pos = 0;
        if( !string_find( rem, search, &pos ) ) {
            break;
        }

        rem.c[pos] = sep;

        rem.cc  += pos;
        rem.len -= pos;

        if( !rem.len ) {
            break;
        }
        rem.cc++;
        rem.len--;
    }
}
attr_core_api usize path_stream_convert_separators(
    StreamBytesFN* stream, void* target, const Path path, b32 to_forward
) {
    usize res = 0;

    char sep    = to_forward ? '/' : '\\';
    char search = to_forward ? '\\' : '/';
    String rem  = path;

    loop() {
        usize pos = 0;
        if( !string_find( rem, search, &pos ) ) {
            res += stream( target, rem.len, rem.v );
            break;
        }

        res += stream( target, pos, rem.cc );
        res += stream( target, 1, &sep );

        rem.cc  += pos;
        rem.len -= pos;

        if( !rem.len ) {
            break;
        }

        rem.cc++;
        rem.len--;
    }

    return res;
}

attr_internal b32 internal_is_sep( char c ) {
#if defined(CORE_PLATFORM_WINDOWS)
    return (c == '/') || (c == '\\');
#else
    return c == '/';
#endif
}

#if defined(CORE_PLATFORM_WINDOWS)
    #define SEP '\\'
#else
    #define SEP '/'
#endif

attr_core_api b32 path_buf_push( PathBuf* path, const Path chunk ) {
    usize additional_required = chunk.len;
    b32 chunk_has_sep = internal_is_sep( chunk.cc[0] );
    b32 path_has_sep  = path->len && internal_is_sep( path->cc[path->len - 1] );
    b32 sep_required  = path->len && !(chunk_has_sep || path_has_sep);

    if( sep_required ) {
        additional_required++;
    }

    if( (path->len + additional_required) > path->cap ) {
        return false;
    }

    if( sep_required ) {
        string_buf_push( path, SEP );
    }

    string_buf_append( path, chunk );
    return true;
}
attr_core_api b32 path_buf_set_extension( PathBuf* path, const Path new_ext ) {
    usize ext_required_cap = new_ext.len;
    b32 new_ext_has_dot    = new_ext.cc[0] == '.';
    if( !new_ext_has_dot ) {
        ext_required_cap++;
    }

    Path ext;
    b32 has_ext = path_extension( path->slice, &ext );
    if( has_ext ) {
        if( ((path->len - ext.len) + ext_required_cap) > path->cap ) {
            return false;
        }
        path->len -= ext.len;
    } else {
        if( (path->len + ext_required_cap) > path->cap ) {
            return false;
        }
    }

    if( !new_ext_has_dot ) {
        string_buf_push( path, '.' );
    }
    string_buf_append( path, new_ext );

    return true;
}

#undef SEP

#endif

