/**
 * Description:  File system operations implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 14, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/fs.h"
#include "core/fmt.h"

#include "core/internal/platform/fs.h"
#include "core/internal/logging.h"

attr_internal
b32 internal_file_copy( FD* dst, FD* src ) {
    char buffer[CORE_FILE_COPY_BUFFER] = {};

    usize remaining = file_query_size( src );

    while( remaining ) {
        usize copy_size = remaining;
        if( copy_size > CORE_FILE_COPY_BUFFER ) {
            copy_size = CORE_FILE_COPY_BUFFER;
        }

        if( !file_read( src, copy_size, buffer, NULL ) ) {
            return false;
        }
        if( !file_write( dst, copy_size, buffer, NULL ) ) {
            return false;
        }

        remaining -= copy_size;
    }
    return true;
}

attr_core_api
b32 file_copy_by_path( struct _StringPOD dst, struct _StringPOD src, b32 create_dst ) {
    FileOpenFlags dst_flags, src_flags;
    src_flags = FOPEN_READ | FOPEN_SHARE_READ;
    dst_flags = FOPEN_WRITE;

    if( create_dst ) {
        if( file_exists_by_path( dst ) ) {
            core_error(
                "core/fs:file_copy_by_path(): "
                "attempted to copy {p} to {p} but {p} "
                "already exists and create_dst flag is true!", src, dst, dst );
            return false;
        }

        dst_flags |= FOPEN_CREATE;
    } else {
        if( file_exists_by_path( dst ) ) {
            dst_flags |= FOPEN_TRUNCATE;
        } else {
            dst_flags |= FOPEN_CREATE;
        }
    }

    FD d, s;
    if( !file_open( dst, dst_flags, &d ) ) {
        return false;
    }
    if( !file_open( src, src_flags, &s ) ) {
        file_close( &d );
        return false;
    }

    b32 result = internal_file_copy( &d, &s );

    file_close( &d );
    file_close( &s );
    return result;
}
attr_core_api
b32 file_move_by_path( struct _StringPOD dst, struct _StringPOD src, b32 create_dst ) {
    if( !file_copy_by_path( dst, src, create_dst ) ) {
        return false;
    }
    return file_remove_by_path( src );
}
attr_core_api
b32 file_remove_by_path( struct _StringPOD path ) {
    if( string_is_empty( path ) ) {
        core_error( "core/fs:file_remove(): path is empty!" );
        return false;
    }
    return platform_file_remove_by_path( path );
}
attr_core_api
b32 file_exists_by_path( struct _StringPOD path ) {
    return platform_file_exists_by_path( path );
}
attr_core_api
b32 file_query_info_by_path( struct _StringPOD path, FileInfo* out_info ) {
    if( string_is_empty( path ) ) {
        core_error( "core/fs:file_query_info_by_path(): path is empty!" );
        return false;
    }
    return platform_file_query_info_by_path( path, out_info );
}
attr_core_api
FileType file_query_type_by_path( struct _StringPOD path ) {
    if( string_is_empty( path ) ) {
        core_error( "core/fs:file_query_type_by_path(): path is empty!" );
        return FTYPE_NULL;
    }
    return platform_file_query_type_by_path( path );
}
attr_core_api
TimePosix file_query_time_create_by_path( struct _StringPOD path ) {
    if( string_is_empty( path ) ) {
        core_error( "core/fs:file_query_time_create_by_path(): path is empty!" );
        return 0;
    }
    return platform_file_query_time_create_by_path( path );
}
attr_core_api
TimePosix file_query_time_modify_by_path( struct _StringPOD path ) {
    if( string_is_empty( path ) ) {
        core_error( "core/fs:file_query_time_modify_by_path(): path is empty!" );
        return 0;
    }
    return platform_file_query_time_modify_by_path( path );
}

attr_core_api
b32 file_open( struct _StringPOD path, FileOpenFlags flags, FD* out_fd ) {
    if( string_is_empty(path) ) {
        core_error( "core/fs:file_open(): path is empty!" );
        return false;
    }
    if( bitfield_check( flags, FOPEN_CREATE ) ) {
        if( bitfield_check( flags, FOPEN_TRUNCATE ) ) {
            core_error( "core/fs:file_open(): flags CREATE and TRUNCATE cannot be combined!" );
            return false;
        }
        if( bitfield_check( flags, FOPEN_APPEND ) ) {
            core_error( "core/fs:file_open(): flags CREATE and APPEND cannot be combined!" );
            return false;
        }
        if( bitfield_check( flags, FOPEN_TEMP ) ) {
            core_error( "core/fs:file_open(): flags CREATE and TEMP cannot be combined!" );
            return false;
        }
    }

    if(
        bitfield_check( flags, FOPEN_APPEND ) &&
        bitfield_check( flags, FOPEN_TRUNCATE )
    ) {
        core_error( "core/fs:file_open(): flags APPEND and TRUNCATE cannot be combined!" );
        return false;
    }

    return platform_file_open( path, flags, out_fd );
}
attr_core_api
void file_close( FD* fd ) {
    platform_file_close( fd );
}
attr_core_api
b32 file_query_info( FD* fd, FileInfo* out_info ) {
    return platform_file_query_info( fd, out_info );
}
attr_core_api
FileType file_query_type( FD* fd ) {
    return platform_file_query_type( fd );
}
attr_core_api
TimePosix file_query_time_create( FD* fd ) {
    return platform_file_query_time_create( fd );
}
attr_core_api
TimePosix file_query_time_modify( FD* fd ) {
    return platform_file_query_time_modify( fd );
}
attr_core_api
usize file_query_size( FD* fd ) {
    return platform_file_query_size( fd );
}
attr_core_api
usize file_query_offset( FD* fd ) {
    return platform_file_query_offset( fd );
}
attr_core_api
void file_truncate( FD* fd ) {
    platform_file_truncate( fd );
}
attr_core_api
usize file_seek( FD* fd, FileSeek type, isize seek ) {
    return platform_file_seek( fd, type, seek );
}
attr_core_api
b32 file_write(
    FD* fd, usize bytes, const void* buf, usize* opt_out_write
) {
    usize write;
    usize* write_ptr = opt_out_write;
    if( !write_ptr ) {
        write_ptr = &write;
    }
    return platform_file_write( fd, bytes, buf, write_ptr );
}
attr_core_api
b32 file_read(
    FD* fd, usize buf_size, void* buf, usize* opt_out_read
) {
    usize read;
    usize* read_ptr = opt_out_read;
    if( !read_ptr ) {
        read_ptr = &read;
    }
    return platform_file_read( fd, buf_size, buf, read_ptr );
}
attr_core_api
usize file_write_fmt_va(
    FD* fd, struct _StringPOD format, va_list va
) {
    return stream_fmt_va( file_stream_write, fd, format, va );
}
attr_core_api
usize file_write_fmt( FD* fd, struct _StringPOD format, ... ) {
    va_list va;
    va_start( va, format );
    usize result = file_write_fmt_va( fd, format, va );
    va_end( va );
    return result;
}
attr_core_api
usize file_stream_write( void* struct_FD, usize count, const void* buf ) {
    FD* fd = (FD*)struct_FD;
    usize written = 0;
    if( !platform_file_write( fd, count, buf, &written ) ) {
        return count;
    }
    return count - written;
}

attr_core_api
b32 directory_create( struct _StringPOD path ) {
    if( string_is_empty(path) ) {
        core_error( "core/fs:directory_create(): path is empty!" );
        return false;
    }
    return platform_directory_create( path );
}
attr_core_api
b32 directory_remove( struct _StringPOD path, b32 recursive ) {
    if( string_is_empty(path) ) {
        core_error( "core/fs:directory_remove(): path is empty!" );
        return false;
    }
    return platform_directory_remove( path, recursive );
}
attr_core_api
b32 directory_walk( struct _StringPOD path, DirectoryWalkFN* callback, void* params ) {
    if( string_is_empty(path) ) {
        core_error( "core/fs:directory_walk(): path is empty!" );
        return false;
    }
    return platform_directory_walk( path, callback, params );
}

attr_core_api
struct _StringPOD directory_current_query(void) {
    return platform_directory_current_query();
}
attr_core_api
b32 directory_current_set( struct _StringPOD path ) {
    if( string_is_empty(path) ) {
        core_error( "core/fs:directory_current_set(): path is empty!" );
        return false;
    }
    return platform_directory_current_set( path );
}

attr_core_api
FD* pipe_stdin(void) {
    return platform_pipe_stdin();
}
attr_core_api
FD* pipe_stdout(void) {
    return platform_pipe_stdout();
}
attr_core_api
FD* pipe_stderr(void) {
    return platform_pipe_stderr();
}

attr_core_api
b32 pipe_open( FD* out_read, FD* out_write ) {
    return platform_pipe_open( out_read, out_write );
}
attr_core_api
void pipe_close( FD* pipe ) {
    platform_pipe_close( pipe );
}

