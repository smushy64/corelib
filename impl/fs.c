/**
 * Description:  File system operations implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 14, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/fs.h"
#include "core/internal/platform.h"
#include "core/internal/logging.h"
#include "core/fmt.h"
#include "core/sync.h"
#include "core/alloc.h"

volatile b32 global_void_pipes_created = false;
attr_global FD global_void_pipes[2];

attr_core_api b32 fd_open( Path path, FileOpenFlags flags, FD* out_fd ) {
    if( bitfield_check( flags, FOPEN_CREATE ) ) {
        if( bitfield_check( flags, FOPEN_TRUNCATE ) ) {
            core_error( "fd_open: flags create and truncate cannot be combined!" );
            return false;
        }
        if( bitfield_check( flags, FOPEN_APPEND ) ) {
            core_error( "fd_open: flags create and append cannot be combined!" );
            return false;
        }
        if( bitfield_check( flags, FOPEN_TEMP ) ) {
            core_error( "fd_open: flags create and temp cannot be combined!" );
            return false;
        }
    }

    if(
        bitfield_check( flags, FOPEN_APPEND ) &&
        bitfield_check( flags, FOPEN_TRUNCATE )
    ) {
        core_error( "fd_open: flags append and truncate cannot be combined!" );
        return false;
    }

    return platform_fd_open( path, flags, out_fd );
}
attr_core_api void fd_close( FD* fd ) {
    platform_fd_close( fd );
}
attr_core_api usize fd_query_size( FD* fd ) {
    return platform_fd_query_size( fd );
}
attr_core_api void fd_truncate( FD* fd ) {
    platform_fd_truncate( fd );
}
attr_core_api usize fd_seek( FD* fd, FileSeek type, isize seek ) {
    return platform_fd_seek( fd, type, seek );
}
attr_core_api b32 fd_write(
    FD* fd, usize bytes, const void* buf, usize* opt_out_write
) {
    return platform_fd_write( fd, bytes, buf, opt_out_write );
}
attr_core_api b32 fd_read(
    FD* fd, usize buf_size, void* buf, usize* opt_out_read
) {
    return platform_fd_read( fd, buf_size, buf, opt_out_read );
}
attr_core_api b32 fd_write_fmt_va(
    FD* fd, usize* opt_out_write, usize format_len, const char* format, va_list va
) {
    usize write = stream_fmt_va( fd_stream_write, fd, format_len, format, va );
    if( opt_out_write ) {
        *opt_out_write = format_len - write;
    }
    return write == 0;
}
attr_core_api usize fd_stream_write(
    void* struct_FD, usize count, const void* buf
) {
    FD* fd = struct_FD;

    usize write_size = 0;
    if( fd_write( fd, count, buf, &write_size ) ) {
        return count - write_size;
    } else {
        return count;
    }
}

attr_core_api b32 file_copy( Path dst, Path src, b32 create_dst ) {
    return platform_file_copy( dst, src, create_dst );
}
attr_core_api b32 file_move( Path dst, Path src, b32 create_dst ) {
    return platform_file_move( dst, src, create_dst );
}
attr_core_api b32 file_remove( Path path ) {
    return platform_file_remove( path );
}
attr_core_api b32 file_exists( Path path ) {
    return platform_file_exists( path );
}

attr_core_api Path directory_query_cwd(void) {
    return platform_directory_query_cwd();
}
attr_core_api b32 directory_set_cwd( Path path ) {
    return platform_directory_set_cwd( path );
}
attr_core_api b32 directory_create( Path path ) {
    return platform_directory_create( path );
}
attr_core_api b32 directory_remove( Path path ) {
    return platform_directory_remove( path );
}
attr_internal b32 internal_directory_remove_recursive(
    PathBuf* buf, struct AllocatorInterface* allocator
) {
    usize original_len = buf->len;
    DirectoryWalk* walk = directory_walk_begin( buf->slice, allocator );
    if( !walk ) {
        core_error( "directory_remove_recursive: failed to walk '{p}'!", buf->slice );
        return false;
    }

    Path path = path_empty();
    b32 is_dir = false;
    b32 result = true;
    while( directory_walk_next( walk, &path, &is_dir )) {
        if( !path_buf_push( buf, path, allocator ) ) {
            core_error( "directory_remove_recursive: "
               "failed to push new path to path buffer!");
            result = false;
            goto internal_directory_remove_recursive_end;
        }

        if( is_dir ) {
            if( !internal_directory_remove_recursive( buf, allocator ) ) {
                result = false;
                goto internal_directory_remove_recursive_end;
            }
            if( !directory_remove( buf->slice ) ) {
                result = false;
                goto internal_directory_remove_recursive_end;
            }
            continue;
        }

        if( !file_remove( buf->slice ) ) {
            result = false;
            goto internal_directory_remove_recursive_end;
        }

        buf->len = original_len;
        buf->raw[buf->len] = 0;
    }

internal_directory_remove_recursive_end:
    directory_walk_end( walk, allocator );
    buf->len = original_len;
    buf->raw[buf->len] = 0;
    return result;
}
attr_core_api b32 directory_remove_recursive(
    Path path, struct AllocatorInterface* allocator
) {
    PathBuf buf = path_buf_empty();
    if( !path_buf_from_path( path, allocator, &buf ) ) {
        core_error( "directory_remove_recursive: failed to create path buffer!" );
        return false;
    }
    b32 result = internal_directory_remove_recursive( &buf, allocator );

    path_buf_free( &buf, allocator );

    if( result ) {
        return directory_remove( path );
    } else {
        return false;
    }
}
attr_core_api b32 directory_exists( Path path ) {
    return platform_directory_exists( path );
}
attr_core_api DirectoryWalk* directory_walk_begin(
    Path path, struct AllocatorInterface* allocator
) {
    return platform_directory_walk_begin( path, allocator );
}
attr_core_api b32 directory_walk_next(
    DirectoryWalk* walk, Path* out_path, b32* opt_out_is_directory
) {
    return platform_directory_walk_next( walk, out_path, opt_out_is_directory );
}
attr_core_api void directory_walk_end(
    DirectoryWalk* walk, struct AllocatorInterface* allocator
) {
    platform_directory_walk_end( walk, allocator );
}

attr_core_api PipeRead* pipe_stdin(void) {
    return platform_stdin();
}
attr_core_api PipeWrite* pipe_stdout(void) {
    return platform_stdout();
}
attr_core_api PipeWrite* pipe_stderr(void) {
    return platform_stderr();
}
attr_internal void internal_check_pipes(void) {
    if( !global_void_pipes_created ) {
        PipeRead  r;
        PipeWrite w;
        pipe_open( &r, &w );
        global_void_pipes[0] = r.fd;
        global_void_pipes[1] = w.fd;
        atomic_add32( &global_void_pipes_created, 1 );
    }
}
attr_core_api PipeRead pipe_read_void(void) {
    internal_check_pipes();
    PipeRead r;
    r.fd = global_void_pipes[0];
    return r;
}
attr_core_api PipeWrite pipe_write_void(void) {
    internal_check_pipes();
    PipeWrite w;
    w.fd = global_void_pipes[1];
    return w;
}

attr_core_api b32 pipe_open( PipeRead* out_read, PipeWrite* out_write ) {
    return platform_pipe_open( out_read, out_write );
}
attr_core_api usize pipe_stream_write(
    void* struct_PipeWrite, usize count, const void* buf
) {
    PipeWrite* pw = struct_PipeWrite;

    usize write_size = 0;
    if( pipe_write( pw, count, buf, &write_size ) ) {
        return count - write_size;
    } else {
        return count;
    }
}

