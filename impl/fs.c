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

#if defined( CORE_COMPILER_MSVC )
attr_global AllocatorInterface global_fs_heap_allocator = {
    .alloc=___internal_memory_alloc_,
    .realloc=___internal_memory_realloc_,
    .free=___internal_memory_free_, .ctx=NULL
};
#else
attr_global AllocatorInterface global_fs_heap_allocator =
    allocator_interface_from_heap();
#endif

attr_internal b32 check_path( Path path ) {
    if( path.len >= CORE_MAX_PATH_NAME ) {
        return false;
    }
    if( !path_is_null_terminated( path ) ) {
        return false;
    }
    return true;
}

attr_core_api b32 fd_open( Path path, FileOpenFlags flags, FD* out_fd ) {
    if( !check_path( path ) ) {
        core_error(
            "fd_open: "
            "path must be null terminated and less than CORE_MAX_PATH_NAME!" );
        return false;
    }

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
    *opt_out_write = stream_fmt_va( fd_stream_write, fd, format_len, format, va );
    return *opt_out_write == 0;
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
    if( !(check_path( dst ) && check_path( src )) ) {
        core_error( "file_copy: paths must be null terminated!" );
        return false;
    }
    return platform_file_copy( dst, src, create_dst );
}
attr_core_api b32 file_move( Path dst, Path src, b32 create_dst ) {
    if( !(check_path( dst ) && check_path( src )) ) {
        core_error( "file_move: paths must be null terminated!" );
        return false;
    }
    return platform_file_move( dst, src, create_dst );
}
attr_core_api b32 file_remove( Path path ) {
    if( !check_path( path ) ) {
        core_error(
            "file_remove: "
            "path must be null terminated and less than CORE_MAX_PATH_NAME!" );
        return false;
    }
    return platform_file_remove( path );
}
attr_core_api b32 file_exists( Path path ) {
    if( !check_path( path ) ) {
        core_error(
            "file_exists: "
            "path must be null terminated and less than CORE_MAX_PATH_NAME!" );
        return false;
    }
    return platform_file_exists( path );
}

attr_core_api b32 directory_create( Path path ) {
    if( !check_path( path ) ) {
        core_error(
            "directory_create: "
            "path must be null terminated and less than CORE_MAX_PATH_NAME!" );
        return false;
    }
    return platform_directory_create( path );
}
attr_core_api b32 directory_remove( Path path, b32 recursive ) {
    if( !check_path( path ) ) {
        core_error(
            "directory_remove: "
            "path must be null terminated and less than CORE_MAX_PATH_NAME!" );
        return false;
    }
    return platform_directory_remove( path, recursive );
}
attr_core_api b32 directory_exists( Path path ) {
    if( !check_path( path ) ) {
        core_error(
            "directory_exists: "
            "path must be null terminated and less than CORE_MAX_PATH_NAME!" );
        return false;
    }
    return platform_directory_exists( path );
}
attr_internal usize internal_directory_count_items(
    Path path, AllocatorInterface* allocator
) {
    DirectoryWalk* walk = directory_walk_begin( path, allocator );
    if( !walk ) {
        core_warn( "directory_query_item_count: cannot open directory '{p}'", path );
        return 0;
    }

    usize result  = 0;
    Path  subpath = path_empty();
    b32   is_dir  = false;
    while( directory_walk_next( walk, &subpath, &is_dir ) ) {
        if( is_dir ) {
            usize path_buf_size = subpath.len + path.len + 16;
            char* path_buf = allocator->alloc( path_buf_size, 0, allocator->ctx );
            if( !path_buf ) {
                core_warn(
                    "directory_query_item_count: "
                    "could not allocate {usize,m} for subdirectory path!",
                    path_buf_size );
                continue;
            }

            StringBuf buf = string_buf_new( path_buf_size, path_buf );
            string_buf_try_append( &buf, path );
            path_buf_try_push( &buf, subpath );

            result += internal_directory_count_items( buf.slice, allocator );

            allocator->free( path_buf, path_buf_size, 0, allocator->ctx );
        } else {
            result++;
        }
    }

    directory_walk_end( walk, allocator );
    return result;
}
attr_core_api usize directory_query_item_count(
    Path path, b32 recursive, struct AllocatorInterface* in_allocator
) {
    if( !check_path( path ) ) {
        core_error( "directory_query_item_count: "
            "path must be null terminated and less than CORE_MAX_PATH_NAME!" );
        return false;
    }

    AllocatorInterface* allocator = in_allocator;
    if( !allocator ) {
        allocator = &global_fs_heap_allocator;
    }

    usize result = 0;
    if( recursive ) {
        result = internal_directory_count_items( path, allocator );
    } else {
        DirectoryWalk* walk = directory_walk_begin( path, allocator );
        if( !walk ) {
            core_warn(
                "directory_query_item_count: could not walk directory '{p}'!", path );
            return 0;
        }

        Path subpath = path_empty();
        while( directory_walk_next( walk, &subpath, NULL ) ) {
            result++;
        }

        directory_walk_end( walk, allocator );
    }

    return result;
}
attr_core_api b32 directory_is_empty( Path path, b32* out_found ) {
    if( !check_path( path ) ) {
        core_error( "directory_is_empty: "
            "path must be null terminated and less than CORE_MAX_PATH_NAME!" );
        return false;
    }
    return platform_directory_is_empty( path, out_found );
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

