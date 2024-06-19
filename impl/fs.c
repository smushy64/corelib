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

attr_core_api FileHandle* fs_file_open( const Path path, FileOpenFlags flags ) {
    b32 append = bitfield_check( flags, FILE_OPEN_FLAG_APPEND );
    if(
        append &&
        !bitfield_check( flags, FILE_OPEN_FLAG_WRITE )
    ) {
        core_error( "attempted to open file with APPEND flag without WRITE flag!" );
        return NULL;
    }

    FileHandle* handle = platform_file_open( path, flags );

    if( handle && append ) {
        usize eof = fs_file_query_size( handle );
        fs_file_set_offset( handle, eof );
    }

    return handle;
}
attr_core_api void fs_file_close( FileHandle* file ) {
    if( file ) {
        platform_file_close( file );
    }
}
attr_core_api usize fs_file_query_size( FileHandle* file ) {
    return platform_file_query_size( file );
}
attr_core_api void fs_file_truncate( FileHandle* file ) {
    platform_file_truncate( file );
}
attr_core_api void fs_file_set_size( FileHandle* file, usize size ) {
    usize previous_offset = fs_file_query_offset( file );
    fs_file_set_offset( file, size );
    fs_file_truncate( file );
    if( previous_offset < size ) {
        fs_file_set_offset( file, previous_offset );
    }
}
attr_core_api usize fs_file_query_offset( FileHandle* file ) {
    return platform_file_query_offset( file );
}
attr_core_api void fs_file_set_offset( FileHandle* file, usize offset ) {
    platform_file_set_offset( file, offset );
}
attr_core_api b32 fs_file_set_offset_relative( FileHandle* file, isize offset ) {
    if( offset < 0 ) {
        isize current = fs_file_query_offset( file );
        if( (current + offset) < 0 ) {
            return false;
        }
    }
    platform_file_set_offset_relative( file, offset );
    return true;
}
attr_core_api b32 fs_file_write( FileHandle* file, usize len, const void* buf ) {
    return platform_file_write( file, len, buf );
}
attr_core_api b32 fs_file_read( FileHandle* file, usize len, void* buf ) {
    return platform_file_read( file, len, buf );
}
attr_core_api b32 fs_file_copy_memory(
    FileHandle* dst, FileHandle* src, usize size,
    usize intermediate_size, void* intermediate
) {
    usize rem = size;
    while( rem ) {
        usize read_size = intermediate_size;
        if( read_size > rem ) {
            read_size = rem;
        }

        if( !fs_file_read( src, read_size, intermediate ) ) {
            core_error(
                "failed to copy src -> dst! failed to read {f,.2,m} from src!",
                (f64)read_size );
            return false;
        }
        if( !fs_file_write( dst, read_size, intermediate ) ) {
            core_error(
                "failed to copy src -> dst! failed to write {f,.2,m} to dst!",
                (f64)read_size );
            return false;
        }

        rem -= read_size;
    }

    return true;
}
attr_core_api b32 fs_file_copy(
    const Path dst, const Path src, b32 fail_if_dst_exists
) {
    return platform_file_copy( dst, src, fail_if_dst_exists );
}
attr_core_api b32 fs_file_move(
    const Path dst, const Path src, b32 fail_if_dst_exists
) {
    return platform_file_move( dst, src, fail_if_dst_exists );
}
attr_core_api b32 fs_file_delete( const Path path ) {
    return platform_file_delete( path );
}
attr_core_api b32 fs_file_exists( const Path path ) {
    return platform_file_exists( path );
}
attr_core_api Path fs_working_directory(void) {
    return platform_working_directory();
}
attr_core_api b32 fs_directory_create( const Path path ) {
    return platform_directory_create( path );
}
attr_core_api b32 fs_directory_item_count( const Path path, usize* out_count ) {
    return platform_directory_item_count( path, out_count );
}
attr_core_api b32 fs_directory_exists( const Path path ) {
    return platform_directory_exists( path );
}

attr_core_api usize fs_file_stream(
    void* target, usize len, const void* buf
) {
    FileHandle* file = target;
    b32 result = fs_file_write( file, len, buf );
    return !result;
}
attr_core_api b32 fs_file_write_fmt_text_va(
    FileHandle* file, usize format_len, const char* format, va_list va
) {
    return !stream_fmt_va( fs_file_stream, file, format_len, format, va );
}

#if defined(CORE_PLATFORM_WINDOWS)
attr_core_api void* stdin_handle(void) {
    return platform_win32_get_stdin();
}
attr_core_api void* stdout_handle(void) {
    return platform_win32_get_stdout();
}
attr_core_api void* stderr_handle(void) {
    return platform_win32_get_stderr();
}
#endif

