/**
 * Description:  Platform Win32 implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 08, 2024
*/
#include "core/defines.h" // IWYU pragma: keep
#if defined(CORE_PLATFORM_WINDOWS)

#include "core/types.h"
#include "core/macros.h"
#include "core/constants.h"
#include "core/sync.h"
#include "core/memory.h"
#include "core/time.h"
#include "core/thread.h"
#include "core/system.h"

#include "core/string.h"
#include "core/fs.h"
#include "core/unicode.h"

#include "core/internal/platform/path.h"
#include "core/internal/platform/time.h"
#include "core/internal/platform/memory.h"
#include "core/internal/platform/thread.h"
#include "core/internal/platform/fs.h"
#include "core/internal/platform/library.h"
#include "core/internal/platform/process.h"
#include "core/internal/platform/misc.h"

#include "core/internal/logging.h"

#define NOMINMAX
#include <windows.h>
#include <intrin.h>

#if defined(CORE_COMPILER_MSVC)
    int _fltused;
#endif

struct Win32UTF16Buf {
    usize cap;
    usize len;
    u16*  buf;
};
usize win32_utf16_buf_stream( void* Win32UTF16Buf, usize count, const void* bytes );

struct Win32Tls {
    u32  thread_id;
    char text_buffer[CORE_PATH_NAME_LEN * 3];
};

struct Win32Thread {
    HANDLE           handle;
    struct Win32Tls* tls;
};

struct Win32ThreadParams {
    atomic32*        ready;
    ThreadMainFN*    main;
    void*            params;
    struct Win32Tls* tls;
};

struct Win32Semaphore {
    HANDLE handle;
};

struct Win32Mutex {
    HANDLE handle;
};

enum Win32DirectoryWalkResult {
    WIN32_DWR_ERROR,
    WIN32_DWR_OK,
    WIN32_DWR_STOP,
};

struct Win32Platform {
    DWORD         tls;
    atomic32      running_thread_id;
    LARGE_INTEGER qpf;

    FD stdin;
    FD stdout;
    FD stderr;

    char cpu_name[255];
    char gpu_name[255];

    struct Win32Tls main_tls_storage;

    struct {
        u32   cap;
        u32   len;
        char* buf;
    } cwd;
    struct {
        u32   len;
        char* buf;
    } home;
};
attr_global struct Win32Platform* global_win32 = NULL;

attr_internal
char* win32_get_local_buffer(void);
attr_internal
void win32_canonical_from_path( struct _StringBufPOD* buf, struct _StringPOD path );
attr_internal
usize win32_canonical_from_path_ucs2( usize buffer_size, wchar_t* buffer, struct _StringPOD path );
attr_internal
wchar_t* win32_canonical_from_path_ucs2_local( struct _StringPOD path );
attr_internal
void win32_path_buf_push_chunk(
    usize buffer_cap, usize* buffer_len, wchar_t* buffer, struct _StringPOD chunk );

attr_internal
b32 win32_ucs2_cmp( const wchar_t* a, const wchar_t* b );
attr_internal
usize win32_ucs2_len( const wchar_t* p );

attr_internal
void win32_log_error( DWORD error_code );
attr_unused attr_internal
usize win32_get_error_message(
    usize buffer_size, char* buffer, DWORD error_code );

attr_internal
TimePosix win32_filetime_to_time_posix( FILETIME ft );

attr_internal
b32 win32_directory_remove( usize* path_len, wchar_t* path, WIN32_FIND_DATAW* data );

attr_internal 
void win32_get_cpu_name( char* buffer );
attr_internal
void win32_get_gpu_name( char* buffer );

b32 win32_init(void) {
    SetConsoleOutputCP( CP_UTF8 );
    SetConsoleCP( CP_UTF8 );

    global_win32 = (struct Win32Platform*)LocalAlloc( 0, sizeof(*global_win32) );
    if( global_win32 == NULL ) {
        return false;
    }
    memory_zero( global_win32, sizeof(*global_win32) );

    global_win32->home.len  = GetEnvironmentVariableW( L"HOMEDRIVE", 0, 0 );
    global_win32->home.len += GetEnvironmentVariableW( L"HOMEPATH", 0, 0 );
    global_win32->home.len *= 2;

    global_win32->home.buf = LocalAlloc( 0, global_win32->home.len );
    if( !global_win32->home.buf ) {
        LocalFree( global_win32 );
        return false;
    }
    memory_zero( global_win32->home.buf, global_win32->home.len );

    global_win32->running_thread_id = 1;

    global_win32->tls = TlsAlloc();
    TlsSetValue( global_win32->tls, &global_win32->main_tls_storage );

    win32_get_cpu_name( global_win32->cpu_name );
    win32_get_gpu_name( global_win32->gpu_name );

    global_win32->stdin.opaque  = GetStdHandle( STD_INPUT_HANDLE );
    global_win32->stdout.opaque = GetStdHandle( STD_OUTPUT_HANDLE );
    global_win32->stderr.opaque = GetStdHandle( STD_ERROR_HANDLE );

    QueryPerformanceFrequency( &global_win32->qpf );

    wchar_t* text_buffer = (wchar_t*)global_win32->main_tls_storage.text_buffer;

    DWORD home_drive_len = GetEnvironmentVariableW(
        L"HOMEDRIVE", text_buffer, CORE_PATH_NAME_LEN );
    GetEnvironmentVariableW(
        L"HOMEPATH", text_buffer + (home_drive_len * 2),
        CORE_PATH_NAME_LEN - (home_drive_len * 2) );

    global_win32->home.len = WideCharToMultiByte(
        CP_UTF8, 0, text_buffer, global_win32->home.len,
        global_win32->home.buf, global_win32->home.len, 0, 0 );

    return true;
}
void win32_deinit(void) {
    if( global_win32->cwd.buf ) {
        HeapFree( GetProcessHeap(), 0, global_win32->cwd.buf );
    }
    LocalFree( global_win32->home.buf );
    TlsFree( global_win32->tls );
    LocalFree( global_win32 );
}

void* platform_heap_alloc( void* opt_old_ptr, usize opt_old_size, usize new_size ) {
    unused(opt_old_size);
    if( opt_old_ptr ) {
        return HeapReAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, opt_old_ptr, new_size );
    } else {
        return HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, new_size );
    }
}
void platform_heap_free( void* buffer, usize size ) {
    unused(size);
    HeapFree( GetProcessHeap(), 0, buffer );
}

TimePosix platform_time_posix(void) {
    FILETIME ft;
    memory_zero( &ft, sizeof(ft) );
    GetSystemTimeAsFileTime( &ft );

    return win32_filetime_to_time_posix( ft );
}
TimeSplit platform_time_split(void) {
    SYSTEMTIME st;
    memory_zero( &st, sizeof(st) );
    GetLocalTime( &st );

    TimeSplit split;

    split.year   = st.wYear;
    split.month  = st.wMonth;
    split.day    = st.wDay;
    split.hour   = st.wHour;
    split.minute = st.wMinute;
    split.second = st.wSecond;

    return split;
}
f64 platform_timer_milliseconds(void) {
    LARGE_INTEGER qpc;
    QueryPerformanceCounter( &qpc );

    return ((f64)qpc.QuadPart / (f64)global_win32->qpf.QuadPart) * 1000.0;
}
f64 platform_timer_seconds(void) {
    LARGE_INTEGER qpc;
    QueryPerformanceCounter( &qpc );

    return (f64)qpc.QuadPart / (f64)global_win32->qpf.QuadPart;
}

b32 platform_file_remove_by_path( struct _StringPOD in_path ) {
    wchar_t* path = win32_canonical_from_path_ucs2_local( in_path );
    return DeleteFileW( path ) != FALSE;
}
enum FileType win32_file_attrib_to_file_type( DWORD attrib ) {
    if( attrib == INVALID_FILE_ATTRIBUTES ) {
        return FTYPE_NULL;
    }

    if( attrib & FILE_ATTRIBUTE_DIRECTORY ) {
        return FTYPE_DIRECTORY;
    } else {
        return FTYPE_FILE;
    }

    return FTYPE_UNKNOWN;
}
b32 platform_file_query_info_by_path(
    struct _StringPOD in_path, struct FileInfo* out_info
) {
    wchar_t* path = win32_canonical_from_path_ucs2_local( in_path );
    WIN32_FILE_ATTRIBUTE_DATA data = {};
    if( !GetFileAttributesExW( path, GetFileExInfoStandard, &data ) ) {
        return false;
    }

#if defined(CORE_ARCH_64_BIT)
    /* Read file size */ {
        LARGE_INTEGER li;
        li.LowPart  = data.nFileSizeLow;
        li.HighPart = *(LONG*)&data.nFileSizeHigh;

        out_info->size = *(usize*)&li.HighPart;
    }
#else
    out_info->size = data.nFileSizeLow;
#endif
    
    out_info->type        = win32_file_attrib_to_file_type( data.dwFileAttributes );
    out_info->time.create = win32_filetime_to_time_posix( data.ftCreationTime );
    out_info->time.modify = win32_filetime_to_time_posix( data.ftLastWriteTime );

    HANDLE pseudo = NULL;
    out_info->permissions = 0;

    pseudo = CreateFileW(
        path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( pseudo ) {
        out_info->permissions |= FPERM_READ;
        CloseHandle( pseudo );
        pseudo = NULL;
    }
    pseudo = CreateFileW(
        path, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( pseudo ) {
        out_info->permissions |= FPERM_WRITE;
        CloseHandle( pseudo );
        pseudo = NULL;
    }
    pseudo = CreateFileW(
        path, GENERIC_EXECUTE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( pseudo ) {
        out_info->permissions |= FPERM_EXECUTE;
        CloseHandle( pseudo );
        pseudo = NULL;
    }

    return true;
}
enum FileType platform_file_query_type_by_path( struct _StringPOD in_path ) {
    wchar_t* path = win32_canonical_from_path_ucs2_local( in_path );
    DWORD attrib  = GetFileAttributesW( path );
    
    return win32_file_attrib_to_file_type( attrib );
}
b32 platform_file_exists_by_path( struct _StringPOD in_path ) {
    wchar_t* path = win32_canonical_from_path_ucs2_local( in_path );
    return GetFileAttributesW( path ) != INVALID_FILE_ATTRIBUTES;
}
TimePosix platform_file_query_time_create_by_path( struct _StringPOD in_path ) {
    wchar_t* path = win32_canonical_from_path_ucs2_local( in_path );
    WIN32_FILE_ATTRIBUTE_DATA data = {};
    if( !GetFileAttributesExW( path, GetFileExInfoStandard, &data ) ) {
        return 0;
    }

    return win32_filetime_to_time_posix( data.ftCreationTime );
}
TimePosix platform_file_query_time_modify_by_path( struct _StringPOD in_path ) {
    wchar_t* path = win32_canonical_from_path_ucs2_local( in_path );
    WIN32_FILE_ATTRIBUTE_DATA data = {};
    if( !GetFileAttributesExW( path, GetFileExInfoStandard, &data ) ) {
        return 0;
    }

    return win32_filetime_to_time_posix( data.ftLastWriteTime );
}

b32 platform_file_open(
    struct _StringPOD in_path, enum FileOpenFlags flags, struct FD* out_fd
) {
    wchar_t* path = win32_canonical_from_path_ucs2_local( in_path );

    DWORD dwDesiredAccess       = 0;
    DWORD dwShareMode           = 0;
    DWORD dwCreationDisposition = OPEN_EXISTING;
    DWORD dwFlagsAndAttributes  = 0;

    if( bitfield_check( flags, FOPEN_READ ) ) {
        dwDesiredAccess |= GENERIC_READ;
    }
    if( bitfield_check( flags, FOPEN_WRITE ) ) {
        dwDesiredAccess |= GENERIC_WRITE;
    }

    if( bitfield_check( flags, FOPEN_SHARE_READ ) ) {
        dwShareMode |= FILE_SHARE_READ;
    }
    if( bitfield_check( flags, FOPEN_SHARE_WRITE ) ) {
        dwShareMode |= FILE_SHARE_WRITE;
    }

    if( bitfield_check( flags, FOPEN_CREATE ) ) {
        dwCreationDisposition = OPEN_ALWAYS;
    } else if( bitfield_check( flags, FOPEN_TRUNCATE ) ) {
        dwCreationDisposition = TRUNCATE_EXISTING;
    } else if( bitfield_check( flags, FOPEN_TEMP ) ) {
        dwCreationDisposition = CREATE_ALWAYS;
        dwFlagsAndAttributes  = FILE_ATTRIBUTE_TEMPORARY;
    }

    b32 append = bitfield_check( flags, FOPEN_APPEND );

    HANDLE handle = CreateFileW(
        path, dwDesiredAccess, dwShareMode, 0,
        dwCreationDisposition, dwFlagsAndAttributes, 0 );

    if( !handle || handle == INVALID_HANDLE_VALUE ) {
        win32_log_error( GetLastError() );
        core_error( "WIN32: failed to open '{p}'", in_path );
        return false;
    }

    out_fd->opaque = handle;

    if( append ) {
        file_seek( out_fd, FSEEK_END, 0 );
    }

    return true;
}
void platform_file_close( struct FD* fd ) {
    if( fd && fd->opaque ) {
        CloseHandle( fd->opaque );
        fd->opaque = NULL;
    }
}
b32 platform_file_query_info( struct FD* fd, struct FileInfo* out_info ) {
    BY_HANDLE_FILE_INFORMATION data = {};
    if( !GetFileInformationByHandle( fd->opaque, &data ) ) {
        return false;
    }

#if defined(CORE_ARCH_64_BIT)
    /* Read file size */ {
        LARGE_INTEGER li;
        li.LowPart  = data.nFileSizeLow;
        li.HighPart = *(LONG*)&data.nFileSizeHigh;

        out_info->size = *(usize*)&li.HighPart;
    }
#else
    out_info->size = data.nFileSizeLow;
#endif
    
    out_info->type        = win32_file_attrib_to_file_type( data.dwFileAttributes );
    out_info->time.create = win32_filetime_to_time_posix( data.ftCreationTime );
    out_info->time.modify = win32_filetime_to_time_posix( data.ftLastWriteTime );

    // TODO(alicia): 

    /* HANDLE pseudo = NULL; */
    /* out_info->permissions = 0; */
    /**/
    /* pseudo = CreateFileW( */
    /*     path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 ); */
    /* if( pseudo ) { */
    /*     out_info->permissions |= FPERM_READ; */
    /*     CloseHandle( pseudo ); */
    /*     pseudo = NULL; */
    /* } */
    /* pseudo = CreateFileW( */
    /*     path, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 ); */
    /* if( pseudo ) { */
    /*     out_info->permissions |= FPERM_WRITE; */
    /*     CloseHandle( pseudo ); */
    /*     pseudo = NULL; */
    /* } */
    /* pseudo = CreateFileW( */
    /*     path, GENERIC_EXECUTE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 ); */
    /* if( pseudo ) { */
    /*     out_info->permissions |= FPERM_EXECUTE; */
    /*     CloseHandle( pseudo ); */
    /*     pseudo = NULL; */
    /* } */

    return true;
}
enum FileType platform_file_query_type( struct FD* fd ) {
    BY_HANDLE_FILE_INFORMATION info = {};
    if( !GetFileInformationByHandle( fd->opaque, &info ) ) {
        return FTYPE_NULL;
    }
    return win32_file_attrib_to_file_type( info.dwFileAttributes );
}
TimePosix platform_file_query_time_create( struct FD* fd ) {
    FILETIME ft = {};
    if( !GetFileTime( fd->opaque, &ft, NULL, NULL ) ) {
        return 0;
    }
    return win32_filetime_to_time_posix( ft );
}
TimePosix platform_file_query_time_modify( struct FD* fd ) {
    FILETIME ft = {};
    if( !GetFileTime( fd->opaque, NULL, NULL, &ft ) ) {
        return 0;
    }
    return win32_filetime_to_time_posix( ft );
}
usize platform_file_query_size( struct FD* fd ) {
#if defined(CORE_ARCH_64_BIT)
    LARGE_INTEGER result = {};
    GetFileSizeEx( fd->opaque, &result );
    return result.QuadPart;
#else
    DWORD result = 0;
    GetFileSize( fd->opaque, &result );
    return result;
#endif
}
usize platform_file_query_offset( struct FD* fd ) {
    return platform_file_seek( fd, FSEEK_CURRENT, 0 );
}
void platform_file_truncate( struct FD* fd ) {
    SetEndOfFile( fd->opaque );
}
usize platform_file_seek( struct FD* fd, enum FileSeek type, isize seek ) {
    DWORD dwMoveMethod = FILE_BEGIN;
    switch( type ) {
        case FSEEK_CURRENT: {
            dwMoveMethod = FILE_CURRENT;
        } break;
        case FSEEK_SET: {
            dwMoveMethod = FILE_BEGIN;
        } break;
        case FSEEK_END: {
            dwMoveMethod = FILE_END;
        } break;
    }
#if defined(CORE_ARCH_64_BIT)
    LARGE_INTEGER move = { .QuadPart = seek }, new_pointer = {};
    SetFilePointerEx( fd->opaque, move, &new_pointer, dwMoveMethod );
    return new_pointer.QuadPart;
#else
    return SetFilePointer( fd->opaque, seek, NULL, dwMoveMethod );
#endif
}
b32 win32_write32( HANDLE handle, u32 bytes, const void* buf, u32* out_write ) {
    DWORD bytes_written = 0;
    BOOL  result        = false;

    if( GetFileType( handle ) == FILE_TYPE_CHAR ) {
        result = WriteConsoleA( handle, buf, bytes, &bytes_written, NULL );
    } else {
        result = WriteFile( handle, buf, bytes, &bytes_written, NULL );
    }

    *out_write = bytes_written;
    return result != FALSE;
}
b32 platform_file_write(
    struct FD* fd, usize bytes, const void* buf, usize* out_write
) {
#if defined(CORE_ARCH_64_BIT)
    u32         part_a_size = bytes > U32_MAX ? U32_MAX : bytes;
    const void* part_a      = buf;
    u32         part_b_size = part_a_size == U32_MAX ? bytes - part_a_size : 0;
    const void* part_b      = (const u8*)buf + part_a_size;

    u32 written = 0;
    if( !win32_write32( fd->opaque, part_a_size, part_a, &written ) ) {
        return false;
    }
    if( !part_b_size ) {
        return true;
    }

    *out_write += written;
    written = 0;
    if( !win32_write32( fd->opaque, part_b_size, part_b, &written ) ) {
        return false;
    }
    *out_write += written;
    return true;
#else
    return win32_write32( fd->opaque, bytes, buf, out_write );
#endif
}
b32 win32_read32( HANDLE handle, u32 bytes, void* buf, u32* out_read ) {
    DWORD read = 0;
    if( !ReadFile( handle, buf, bytes, &read, NULL ) ) {
        return false;
    }
    *out_read = read;
    return true;
}
b32 platform_file_read( struct FD* fd, usize bytes, void* buf, usize* out_read ) {
#if defined(CORE_ARCH_64_BIT)
    u32   part_a_size = bytes > U32_MAX ? U32_MAX : bytes;
    void* part_a      = buf;
    u32   part_b_size = part_a_size == U32_MAX ? bytes - part_a_size : 0;
    void* part_b      = (u8*)buf + part_a_size;

    u32 read = 0;
    if( !win32_read32( fd->opaque, part_a_size, part_a, &read ) ) {
        return false;
    }
    if( !part_b_size ) {
        return true;
    }

    *out_read += read;
    read = 0;
    if( !win32_read32( fd->opaque, part_b_size, part_b, &read ) ) {
        return false;
    }
    *out_read += read;
    return true;
#else
    return win32_read32( fd->opaque, bytes, buf, out_read );
#endif
}

b32 platform_directory_create( struct _StringPOD in_path ) {
    wchar_t* path = win32_canonical_from_path_ucs2_local( in_path );
    return CreateDirectoryW( path, NULL ) == TRUE;
}
b32 platform_directory_remove( struct _StringPOD in_path, b32 recursive ) {
    wchar_t* path = win32_canonical_from_path_ucs2_local( in_path );
    if( recursive ) {
        usize path_len = win32_ucs2_len( path );
        path[path_len++] = '\\';
        path[path_len++] = '*';
        path[path_len]   = 0;
        WIN32_FIND_DATAW data;
        return win32_directory_remove( &path_len, path, &data );
    } else {
        return RemoveDirectoryW( path ) == TRUE;
    }
}
enum Win32DirectoryWalkResult win32_directory_walk(
    enum DirectoryWalkControl (callback)( const struct DirectoryWalkInfo* info, void* params ),
    void* params, DirectoryWalkInfo* info,
    char* utf8_buf,
    usize* ucs2_len, wchar_t* ucs2_buf,
    WIN32_FIND_DATAW* data
) {
    HANDLE handle = FindFirstFileExW(
        ucs2_buf, FindExInfoBasic, data, FindExSearchNameMatch, 0, 0 );
    if( handle == INVALID_HANDLE_VALUE ) {
        win32_log_error( GetLastError() );
        core_error( "WIN32: failed to open directory!" );
        return WIN32_DWR_ERROR;
    }

    *ucs2_len           -= 2;
    ucs2_buf[*ucs2_len]  = 0;

    usize original_len = *ucs2_len;

    for( ;; ) {
        if(
            win32_ucs2_cmp(  L".", data->cFileName ) ||
            win32_ucs2_cmp( L"..", data->cFileName )
        ) {
            if( FindNextFileW( handle, data ) ) {
                continue;
            } else {
                break;
            }
        }

        *ucs2_len           = original_len;
        ucs2_buf[*ucs2_len] = '\\';
        *ucs2_len += 1;

        usize file_name_len = win32_ucs2_len( data->cFileName );
        memory_copy( ucs2_buf + *ucs2_len, data->cFileName, sizeof(wchar_t) * file_name_len );

        *ucs2_len           += file_name_len;
        ucs2_buf[*ucs2_len]  = 0;

        usize len = (usize)WideCharToMultiByte(
            CP_UTF8, 0, ucs2_buf + sizeof("\\\\?"),
            *ucs2_len - sizeof("\\\\?"), utf8_buf, CORE_PATH_NAME_LEN, 0, 0 );

        info->path             = utf8_buf;
        info->path_len         = len;
        info->path_name_offset = len - file_name_len;
#if defined(CORE_ARCH_64_BIT)
        /* Read file size */ {
            LARGE_INTEGER li;
            li.LowPart  = data->nFileSizeLow;
            li.HighPart = *(LONG*)&data->nFileSizeHigh;

            info->size = *(usize*)&li.HighPart;
        }
#else
        info->size = data->nFileSizeLow;
#endif

        info->type = win32_file_attrib_to_file_type( data->dwFileAttributes );

        enum DirectoryWalkControl control = callback( info, params );

        switch( control ) {
            case DWC_CONTINUE: {
                if( info->type == FTYPE_DIRECTORY ) {
                    ucs2_buf[*ucs2_len] = '\\';
                    *ucs2_len += 1;
                    ucs2_buf[*ucs2_len] = '*';
                    *ucs2_len += 1;
                    ucs2_buf[*ucs2_len] = 0;

                    info->level++;

                    switch( win32_directory_walk(
                        callback, params, info,
                        utf8_buf, ucs2_len, ucs2_buf, data
                    ) ) {
                        case WIN32_DWR_OK: break;

                        case WIN32_DWR_ERROR: {
                            CloseHandle( handle );
                        } return WIN32_DWR_ERROR;

                        case WIN32_DWR_STOP: {
                            CloseHandle( handle );
                        } return WIN32_DWR_STOP;
                    }

                    info->level--;
                }
            } break;

            case DWC_STOP: {
                CloseHandle( handle );
            } return WIN32_DWR_STOP;

            case DWC_SKIP:
                break;
        }

        if( !FindNextFileW( handle, data ) ) {
            break;
        }
    }

    CloseHandle( handle );
    return WIN32_DWR_OK;
}
b32 platform_directory_walk(
    struct _StringPOD in_path,
    enum DirectoryWalkControl (callback)( const struct DirectoryWalkInfo* info, void* params ),
    void* params
) {
    char*    utf8_buf = win32_get_local_buffer();
    wchar_t* ucs2_buf = (wchar_t*)(utf8_buf + CORE_PATH_NAME_LEN);

    usize buffer_len = win32_canonical_from_path_ucs2( CORE_PATH_NAME_LEN, ucs2_buf, in_path );
    ucs2_buf[buffer_len++] = '\\';
    ucs2_buf[buffer_len++] = '*';
    ucs2_buf[buffer_len]   = 0;

    WIN32_FIND_DATAW  data = {};
    DirectoryWalkInfo info = {};

    switch( win32_directory_walk(
        callback, params, &info, utf8_buf, &buffer_len, ucs2_buf, &data
    ) ) {
        case WIN32_DWR_ERROR: return false;
        default:              return true;
    }
}

void win32_get_cwd(void) {
    wchar_t* ucs2_buf = (wchar_t*)win32_get_local_buffer();
    char*    utf8_buf = (char*)(ucs2_buf + CORE_PATH_NAME_LEN);

    DWORD ucs2_len = GetCurrentDirectoryW( CORE_PATH_NAME_LEN, ucs2_buf );
    if(
        ucs2_len >= 4 &&
        memory_cmp( ucs2_buf, L"\\\\?\\", sizeof(wchar_t) * 4 ) 
    ) {
        ucs2_buf += 4;
        ucs2_len -= 4;
    }

    u32 utf8_len = (u32)WideCharToMultiByte(
        CP_UTF8, 0, ucs2_buf, ucs2_len, utf8_buf, CORE_PATH_NAME_LEN, 0, 0 );

    if( !global_win32->cwd.buf ) {
        global_win32->cwd.buf = (char*)HeapAlloc(
            GetProcessHeap(), HEAP_ZERO_MEMORY, utf8_len + 1 );
        global_win32->cwd.cap = utf8_len + 1;
    } else if( global_win32->cwd.cap < (utf8_len + 1) ) {
        global_win32->cwd.buf = (char*)HeapReAlloc(
            GetProcessHeap(), HEAP_ZERO_MEMORY, global_win32->cwd.buf, utf8_len + 1 );
        global_win32->cwd.cap = utf8_len + 1;
    }

    global_win32->cwd.len = 0;
    memory_copy( global_win32->cwd.buf, utf8_buf, utf8_len );
    global_win32->cwd.len = utf8_len;
}
struct _StringPOD platform_directory_current_query(void) {
    if( !global_win32->cwd.buf ) {
        win32_get_cwd();
    }
    return string_new( global_win32->cwd.len, global_win32->cwd.buf );
}
b32 platform_directory_current_set( struct _StringPOD in_path ) {
    wchar_t* path = win32_canonical_from_path_ucs2_local( in_path );
    if( !SetCurrentDirectoryW( path ) ) {
        return false;
    }
    win32_get_cwd();
    return true;
}

struct FD* platform_pipe_stdin(void) {
    return &global_win32->stdin;
}
struct FD* platform_pipe_stdout(void) {
    return &global_win32->stdout;
}
struct FD* platform_pipe_stderr(void) {
    return &global_win32->stderr;
}

b32 platform_pipe_open( struct FD* out_read, struct FD* out_write ) {
    HANDLE read, write;

    SECURITY_ATTRIBUTES sa;
    memory_zero( &sa, sizeof(sa) );
    sa.nLength        = sizeof(sa);
    sa.bInheritHandle = TRUE;

    if( !CreatePipe( &read, &write, &sa, 0 ) ) {
        return false;
    }

    out_read->opaque  = read;
    out_write->opaque = write;

    return true;
}
void platform_pipe_close( const FD* pipe ) {
    CloseHandle( pipe->opaque );
}

usize platform_path_chunk_count( struct _StringPOD path ) {
    struct _StringPOD remaining = path;
    if( !remaining.len ) {
        return 0;
    }

    if(
        (remaining.len >= 3)                          &&
        (ascii_is_alphabetic( remaining.cbuf[0] ))    &&
        (remaining.cbuf[1] == ':')                    &&
        (ascii_is_path_separator( remaining.cbuf[2] ))
    ) {
        remaining = string_advance_by( remaining, 3 );
    }

    usize result = 0;
    while( !string_is_empty( remaining ) ) {
        struct _StringPOD chunk = remaining;
        string_find_set( chunk, string_text( "/\\" ), &chunk.len );

        if( chunk.len > 0 ) {
            result++;
        }
        remaining = string_advance_by( remaining, chunk.len + 1 );
    }
    return result;
}
struct _StringPOD platform_path_clip_chunk( struct _StringPOD path ) {
    if( !path.len ) {
        return path;
    }
    struct _StringPOD result = path;

    if(
        (result.len >= 3)                           &&
        (ascii_is_alphabetic( result.cbuf[0] ))     &&
        (result.cbuf[1] == ':')                     &&
        (ascii_is_path_separator( result.cbuf[2] ))
    ) {
        if( result.len == 3 ) {
            return result;
        } else {
            string_find_set(
                string_advance_by( result, 3 ), string_text("/\\"), &result.len );
        }
    } else {
        string_find_set( result, string_text("/\\"), &result.len );
    }

    return result;
}
struct _StringPOD platform_path_clip_chunk_last( struct _StringPOD path ) {
    if( !path.len ) {
        return path;
    }

    struct _StringPOD result = path;
    char last = string_last_unchecked( result );
    if( ascii_is_path_separator( last ) ) {
        result = string_trim( result, 1 );

        if( !result.len ) {
            return result;
        }
    }

    usize previous_separator = 0;
    if( string_find_set_rev( result, string_text("/\\"), &previous_separator ) ) {
        result = string_advance_by( result, previous_separator + 1 );
    }

    return result;
}
struct _StringPOD platform_path_advance_chunk( struct _StringPOD path ) {
    if( !path.len ) {
        return path;
    }
    struct _StringPOD first_chunk = path_clip_chunk( path );
    if( first_chunk.len ) {
        return string_advance_by( path, first_chunk.len + 1 );
    } else {
        return string_empty();
    }
}
struct _StringPOD platform_path_pop_chunk( struct _StringPOD path ) {
    struct _StringPOD last = path_clip_chunk_last( path );
    if( last.len ) {
        return string_trim( path, last.len + 1 );
    } else {
        return path;
    }
}
b32 platform_path_is_absolute( struct _StringPOD path ) {
    return
        (path.len >= 3)                           &&
        (ascii_is_alphabetic( path.cbuf[0] ))     &&
        (path.cbuf[1] == ':')                     &&
        (ascii_is_path_separator( path.cbuf[2] ));
}
b32 platform_path_parent( struct _StringPOD path, struct _StringPOD* out_parent ) {
    struct _StringPOD parent = path_pop_chunk( path );
    if( !parent.len ) {
        return false;
    }
    *out_parent = path_clip_chunk_last( parent );
    return true;
}
b32 platform_path_file_name( struct _StringPOD path, struct _StringPOD* out_file_name ) {
    if( !path.len ) {
        return false;
    }

    if( ascii_is_path_separator( string_last_unchecked( path ) ) ) {
        return false;
    }

    *out_file_name = path_clip_chunk_last( path );
    return true;
}
b32 platform_path_stream_set_native_separators(
    StreamBytesFN* stream, void* target, struct _StringPOD path
) {
    return path_stream_set_windows_separators( stream, target, path );
}
void platform_path_set_native_separators( struct _StringPOD path ) {
    path_set_windows_separators( path );
}
attr_internal
void win32_canonical_from_path( struct _StringBufPOD* buf, struct _StringPOD path ) {
    enum {
        WIN32_PATH_REL,
        WIN32_PATH_HOME,
        WIN32_PATH_ABS,
    } type = WIN32_PATH_REL;

    struct _StringPOD rem = path;

    if( path_is_absolute( path ) ) {
        type = WIN32_PATH_ABS;
    } else if( rem.len && rem.cbuf[0] == '~' ) {
        type = WIN32_PATH_HOME;
    }

    switch( type ) {
        case WIN32_PATH_REL: {
            string_buf_try_append( buf, directory_current_query() );
        } break;
        case WIN32_PATH_HOME: {
            path_buf_try_push_chunk(
                buf, string_new( global_win32->home.len, global_win32->home.buf ) );
            rem = string_advance_by( rem, 2 );
        } break;
        case WIN32_PATH_ABS: break;
    }

    usize min = string_text("A:\\").len;

    while( !string_is_empty( rem ) ) {
        struct _StringPOD chunk = rem;
        usize sep = 0;
        if( string_find_set( rem, string_text("/\\"), &sep ) ) {
            if( !sep ) {
                rem = string_advance( rem );
                continue;
            }
            chunk.len = sep;
        }

        if( chunk.len < 3 ) {
            if( string_cmp( chunk, string_text(".") )) {
                rem = string_advance_by( rem, chunk.len + 1 );
                continue;
            }
            if( string_cmp( chunk, string_text(".."))) {
                for( usize i = buf->len; i-- > 0; ) {
                    if( buf->cbuf[i] == '\\' ) {
                        buf->len = i;
                        break;
                    }
                }

                if( buf->len < min ) {
                    buf->len = min;
                }

                buf->buf[buf->len] = 0;
                rem = string_advance_by( rem, chunk.len + 1 );
                continue;
            }
        }

        path_buf_try_push_chunk( buf, chunk );
        rem = string_advance_by( rem, chunk.len + 1 );
    }
}
usize platform_path_stream_canonicalize(
    StreamBytesFN* stream, void* target, struct _StringPOD path
) {
    struct _StringBufPOD buffer = string_buf_new( CORE_PATH_NAME_LEN * 2, win32_get_local_buffer() );
    win32_canonical_from_path( &buffer, path );
    return stream( target, buffer.len, buffer.buf );
}
b32 platform_path_buf_try_push_chunk( struct _StringBufPOD* buf, struct _StringPOD chunk ) {
    if( !chunk.len ) {
        return true;
    }

    b32 chunk_has_separator = ascii_is_path_separator( string_first_unchecked( chunk ) );
    if( chunk_has_separator && chunk.len == 1 ) {
        return true;
    }

    b32 buf_has_separator = false;
    if( !string_buf_is_empty( *buf ) ) {
        buf_has_separator = ascii_is_path_separator( string_last_unchecked( buf->slice ) );
    }

    if(
        string_buf_remaining( *buf ) <
        (chunk.len + !(chunk_has_separator || buf_has_separator))
    ) {
        return false;
    }

    if( chunk_has_separator ) {
        chunk = string_advance( chunk );
    }

    if( !buf_has_separator ) {
        string_buf_try_push( buf, '\\' );
    }
    string_buf_try_append( buf, chunk );

    return true;
}
b32 platform_path_buf_try_set_extension( struct _StringBufPOD* buf, struct _StringPOD extension ) {
    if( !extension.len ) {
        return true;
    }

    struct _StringPOD exisiting_extension = {};
    if( path_extension( buf->slice, &exisiting_extension ) ) {
        buf->len -= exisiting_extension.len;
    }

    b32 has_dot = string_first_unchecked( extension ) == '.';

    if( string_buf_remaining( *buf ) < (extension.len + !has_dot) ) {
        return false;
    }

    if( !has_dot ) {
        string_buf_try_push( buf, '.' );
    }
    string_buf_try_append( buf, extension );
    return true;
}

b32 platform_semaphore_create(
    const char* name, u32 initial_value, struct NamedSemaphore* out_sem
) {
    HANDLE handle = CreateSemaphoreExA(
        NULL, initial_value, I32_MAX, name, 0, SEMAPHORE_ALL_ACCESS );
    if( handle == INVALID_HANDLE_VALUE ) {
        return false;
    }

    struct Win32Semaphore* sem = (struct Win32Semaphore*)out_sem;
    sem->handle = handle;

    return true;
}
void platform_semaphore_destroy( struct NamedSemaphore* in_sem ) {
    struct Win32Semaphore* sem = (struct Win32Semaphore*)in_sem;
    CloseHandle( sem->handle );
}
void platform_semaphore_signal( struct NamedSemaphore* in_sem ) {
    struct Win32Semaphore* sem = (struct Win32Semaphore*)in_sem;
    ReleaseSemaphore( sem->handle, 1, NULL );
}
b32 platform_semaphore_wait( struct NamedSemaphore* in_sem, u32 ms ) {
    struct Win32Semaphore* sem = (struct Win32Semaphore*)in_sem;

    // NOTE(alicia): U32_MAX == INFINITE
    DWORD result = WaitForSingleObject( sem->handle, ms );

    if( ms != U32_MAX ) {
        return result != WAIT_TIMEOUT;
    }
    return true;
}

b32 platform_mutex_create( struct OSMutex* out_mutex ) {
    struct Win32Mutex* mutex = (struct Win32Mutex*)out_mutex;
    HANDLE handle = CreateMutexA( NULL, false, NULL );

    if( handle == INVALID_HANDLE_VALUE ) {
        return false;
    }

    mutex->handle = handle;
    return true;
}
void platform_mutex_destroy( struct OSMutex* in_mutex ) {
    struct Win32Mutex* mutex = (struct Win32Mutex*)in_mutex;
    CloseHandle( mutex->handle );
}
b32 platform_mutex_lock( struct OSMutex* in_mutex, u32 ms ) {
    struct Win32Mutex* mutex = (struct Win32Mutex*)in_mutex;

    // NOTE(alicia): U32_MAX == INFINITE
    DWORD result = WaitForSingleObject( mutex->handle, ms );

    if( ms != U32_MAX ) {
        return result != WAIT_TIMEOUT;
    }
    return true;
}
void platform_mutex_unlock( struct OSMutex* in_mutex ) {
    struct Win32Mutex* mutex = (struct Win32Mutex*)in_mutex;
    ReleaseMutex( mutex->handle );
}

void platform_sleep( u32 ms ) {
    Sleep( (DWORD)ms );
}
void platform_yield(void) {
    SwitchToThread();
}

attr_internal
DWORD win32_thread_proc( void* in ) {
    struct Win32ThreadParams thread_params = *(struct Win32ThreadParams*)in;

    thread_params.tls->thread_id = atomic_increment32( &global_win32->running_thread_id );

    read_write_barrier();
    TlsSetValue( global_win32->tls, thread_params.tls );

    atomic_increment32( thread_params.ready );

    volatile int ret = thread_params.main( thread_params.tls->thread_id, thread_params.params );

    read_write_barrier();

    TlsSetValue( global_win32->tls, NULL );
    HeapFree( GetProcessHeap(), 0, thread_params.tls );

    ExitThread( ret );
}
b32 platform_thread_create(
    ThreadMainFN* main, void* params,
    usize stack_size, ThreadHandle* out_handle
) {
    atomic32 ready = 0;
    struct Win32ThreadParams thread_params = {};
    thread_params.ready  = &ready;
    thread_params.main   = main;
    thread_params.params = params;

    struct Win32Thread thread = {};
    thread.tls = (struct Win32Tls*)HeapAlloc(
        GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*thread.tls));

    if( !thread.tls ) {
        core_error( "WIN32: failed to allocate thread data!" );
        return false;
    }

    thread_params.tls = thread.tls;

    read_write_barrier();

    DWORD  id     = 0;
    thread.handle = CreateThread(
        NULL, stack_size, win32_thread_proc, &thread_params, 0, &id );

    if( thread.handle == NULL ) {
        win32_log_error( GetLastError() );
        HeapFree( GetProcessHeap(), 0, thread.tls );
        return false;
    }

    read_write_barrier();
    atomic_spinlock( &ready, 1 );

    out_handle->opaque  = thread.handle;
    out_handle->opaque2 = thread.tls;
    return true;
}
void platform_thread_destroy( ThreadHandle* handle ) {
    struct Win32Thread* thread = (struct Win32Thread*)handle;
    TerminateThread( thread->handle, -1 );
    CloseHandle( thread->handle );

    // TODO(alicia): how to prevent potential double free?
    read_write_barrier();

    HeapFree( GetProcessHeap(), 0, thread->tls );
}
u32 platform_thread_query_id(void) {
    struct Win32Tls* tls = TlsGetValue( global_win32->tls );
    if( !tls ) {
        core_error( "WIN32: Failed to obtain TLS value!" );
        return 0;
    }
    return tls->thread_id;
}
void platform_thread_free( ThreadHandle* handle ) {
    struct Win32Thread* thread = (struct Win32Thread*)handle;
    CloseHandle( thread->handle );
}
b32 platform_thread_join_timed( ThreadHandle* handle, u32 ms, int* opt_out_exit_code ) {
    struct Win32Thread* thread = (struct Win32Thread*)handle;
    DWORD result = WaitForSingleObject( thread->handle, ms );
    switch( result ) {
        case WAIT_OBJECT_0: break;
        case WAIT_TIMEOUT: {
            return false;
        } break;
        default: {

        } return false;
    }

    if( opt_out_exit_code ) {
        DWORD exit_code = 0;
        GetExitCodeThread( thread->handle, &exit_code );
        *opt_out_exit_code = *(int*)&exit_code;
    }
    return true;
}
b32 platform_thread_exit_code( ThreadHandle* handle, int* out_exit_code ) {
    struct Win32Thread* thread = (struct Win32Thread*)handle;
    DWORD result = WaitForSingleObject( thread->handle, 0 );
    switch( result ) {
        case WAIT_OBJECT_0: break;
        default: return false;
    }

    DWORD exit_code = 0;
    GetExitCodeThread( thread->handle, &exit_code );
    *out_exit_code = *(int*)&exit_code;
    return true;
}
void* platform_library_open( struct _StringPOD name ) {
    wchar_t* buf = (wchar_t*)win32_get_local_buffer();
    int len      = MultiByteToWideChar(
        CP_UTF8, 0, name.buf, name.len, buf, CORE_PATH_NAME_LEN );
    buf[len] = 0;

    return (void*)LoadLibraryW( buf );
}
void* platform_library_get( struct _StringPOD name ) {
    wchar_t* buf = (wchar_t*)win32_get_local_buffer();
    int len      = MultiByteToWideChar(
        CP_UTF8, 0, name.buf, name.len, buf, CORE_PATH_NAME_LEN );
    buf[len] = 0;

    return (void*)GetModuleHandleW( buf );
}
void platform_library_close( void* lib ) {
    FreeLibrary( (HMODULE)lib );
}
void* platform_library_load( void* lib, struct _StringPOD function ) {
    char* buf = win32_get_local_buffer();
    memory_copy( buf, function.cbuf, function.len );
    buf[function.len] = 0;

    void* proc = (void*)GetProcAddress( lib, buf );
    if( !proc ) {
        win32_log_error( GetLastError() );
        core_error( "WIN32: failed to load function '{s}'!", function );
    }
    return proc;
}
void platform_system_query_info( struct SystemInfo* out_info ) {
    SYSTEM_INFO info = {0};
    GetSystemInfo( &info );

    out_info->page_size = info.dwPageSize;
    out_info->cpu_count = info.dwNumberOfProcessors;

    if( IsProcessorFeaturePresent(
        PF_XMMI_INSTRUCTIONS_AVAILABLE
    ) ) {
        out_info->feature_flags |= CPU_FEATURE_SSE;
    }
    if( IsProcessorFeaturePresent(
        PF_XMMI64_INSTRUCTIONS_AVAILABLE
    ) ) {
        out_info->feature_flags |= CPU_FEATURE_SSE2;
    }
    if( IsProcessorFeaturePresent(
        PF_SSE3_INSTRUCTIONS_AVAILABLE
    ) ) {
        out_info->feature_flags |= CPU_FEATURE_SSE3;
    }
    if( IsProcessorFeaturePresent(
        PF_SSSE3_INSTRUCTIONS_AVAILABLE
    ) ) {
        out_info->feature_flags |= CPU_FEATURE_SSSE3;
    }
    if( IsProcessorFeaturePresent(
        PF_SSE4_1_INSTRUCTIONS_AVAILABLE
    ) ) {
        out_info->feature_flags |= CPU_FEATURE_SSE4_1;
    }
    if( IsProcessorFeaturePresent(
        PF_SSE4_2_INSTRUCTIONS_AVAILABLE
    ) ) {
        out_info->feature_flags |= CPU_FEATURE_SSE4_2;
    }
    if( IsProcessorFeaturePresent(
        PF_AVX_INSTRUCTIONS_AVAILABLE
    ) ) {
        out_info->feature_flags |= CPU_FEATURE_AVX;
    }
    if( IsProcessorFeaturePresent(
        PF_AVX2_INSTRUCTIONS_AVAILABLE
    ) ) {
        out_info->feature_flags |= CPU_FEATURE_AVX2;
    }
    if( IsProcessorFeaturePresent(
        PF_AVX512F_INSTRUCTIONS_AVAILABLE
    ) ) {
        out_info->feature_flags |= CPU_FEATURE_AVX_512;
    }

    MEMORYSTATUSEX memory_status = {0};
    memory_status.dwLength = sizeof( memory_status );
    GlobalMemoryStatusEx( &memory_status );

    out_info->total_memory = memory_status.ullTotalPhys;

    out_info->cpu_name = string_from_cstr( global_win32->cpu_name );
    out_info->gpu_name = string_from_cstr( global_win32->gpu_name );
}

b32 win32_directory_remove( usize* path_len, wchar_t* path, WIN32_FIND_DATAW* data ) {
    HANDLE handle = FindFirstFileExW(
        path, FindExInfoBasic, data, FindExSearchNameMatch, 0, 0 );
    if( handle == INVALID_HANDLE_VALUE ) {
        win32_log_error( GetLastError() );
        core_error( "WIN32: failed to open directory!" );
        return false;
    }
    *path_len       -= 2;
    path[*path_len]  = 0;

    usize original_len = *path_len;
    for( ;; ) {
        if(
            win32_ucs2_cmp(  L".", data->cFileName ) ||
            win32_ucs2_cmp( L"..", data->cFileName )
        ) {
            if( FindNextFileW( handle, data ) ) {
                continue;
            } else {
                break;
            }
        }

        *path_len        = original_len;
        path[*path_len]  = '\\';
        *path_len       += 1;

        usize file_name_len = win32_ucs2_len( data->cFileName );
        memory_copy( path + *path_len, data->cFileName, sizeof(wchar_t) * file_name_len );

        *path_len       += file_name_len;
        path[*path_len]  = 0;

        if( data->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
            path[*path_len] = '\\';
            *path_len += 1;
            path[*path_len] = '*';
            *path_len += 1;
            path[*path_len] = 0;

            if( win32_directory_remove( path_len, path, data ) ) {
                continue;
            } else {
                break;
            }
        }

        if( !DeleteFileW( path ) ) {
            break;
        }
    }

    CloseHandle( handle );
    return RemoveDirectoryW( path ) == TRUE;
}
usize win32_ucs2_len( const wchar_t* p ) {
    usize result = 0;
    while( *p++ ) {
        result++;
    }
    return result;
}
b32 win32_ucs2_cmp( const wchar_t* a, const wchar_t* b ) {
    while( *a && *b ) {
        if( *a != *b ) {
            return false;
        }
        a++; b++;
    }
    return *a == *b;
}
attr_internal
char* win32_get_local_buffer(void) {
    struct Win32Tls* tls = (struct Win32Tls*)TlsGetValue( global_win32->tls );
    if( !tls ) {
        return NULL;
    }
    return tls->text_buffer;
}
attr_internal
usize win32_canonical_from_path_ucs2( usize buffer_size, wchar_t* buffer, struct _StringPOD path ) {
    enum {
        WIN32_PATH_REL,
        WIN32_PATH_HOME,
        WIN32_PATH_ABS,
    } type = WIN32_PATH_REL;

    usize buffer_len = 0;

    struct _StringPOD rem = path;

    if( path_is_absolute( path ) ) {
        type = WIN32_PATH_ABS;
    } else if( rem.len && rem.cbuf[0] == '~' ) {
        type = WIN32_PATH_HOME;
    }

    switch( type ) {
        case WIN32_PATH_REL: {
            buffer_len += GetCurrentDirectoryW( buffer_size, buffer );
        } break;
        case WIN32_PATH_HOME: {
            buffer_len += GetEnvironmentVariableW(
                L"HOMEDRIVE", buffer + buffer_len, buffer_size - buffer_len );
            buffer_len += GetEnvironmentVariableW(
                L"HOMEPATH", buffer + buffer_len, buffer_size - buffer_len );
            if( buffer[buffer_len - 1] != L'\\' ) {
                buffer[buffer_len++] = L'\\';
            }
        } break;
        case WIN32_PATH_ABS: break;
    }

    // NOTE(alicia): Check if path already has "\\?\" prepended
    // and if it doesn't, prepend it. This is so that
    // the path will still work even if it's longer than MAX_PATH.
    if(
        ( buffer_len >= sizeof( "\\\\?") &&
            !memory_cmp( buffer, L"\\\\?\\", sizeof(L"\\\\?") ) ) ||
        ( buffer_len < sizeof("\\\\?") )
    ) {
        memory_move(
            buffer + sizeof("\\\\?"), buffer, sizeof(wchar_t) * buffer_len );

        memory_copy( buffer, L"\\\\?\\", sizeof(L"\\\\?") );

        buffer_len += sizeof("\\\\?");
    }

    usize min = string_text("\\\\?\\A:\\").len;

    while( !string_is_empty( rem ) ) {
        struct _StringPOD chunk = rem;
        usize sep = 0;
        if( string_find_set( rem, string_text("/\\"), &sep ) ) {
            if( !sep ) {
                rem = string_advance( rem );
                continue;
            }
            chunk.len = sep;
        }

        if( chunk.len < 3 ) {
            if( string_cmp( chunk, string_text(".") )) {
                rem = string_advance_by( rem, chunk.len + 1 );
                continue;
            }
            if( string_cmp( chunk, string_text(".."))) {
                for( usize i = buffer_len; i-- > 0; ) {
                    if( buffer[i] == '\\' ) {
                        buffer_len = i;
                        break;
                    }
                }

                if( buffer_len < min ) {
                    buffer_len = min;
                }

                buffer[buffer_len] = 0;
                rem = string_advance_by( rem, chunk.len + 1 );
                continue;
            }
        }

        win32_path_buf_push_chunk( buffer_size, &buffer_len, buffer, chunk );
        rem = string_advance_by( rem, chunk.len + 1 );
    }
    buffer[buffer_len] = 0;
    return buffer_len;
}
attr_internal
void win32_path_buf_push_chunk(
    usize buffer_cap, usize* buffer_len, wchar_t* buffer, struct _StringPOD chunk
) {
    if( !chunk.len ) {
        return;
    }

    b32 chunk_has_separator = ascii_is_path_separator( string_first_unchecked( chunk ) );
    if( chunk_has_separator && chunk.len == 1 ) {
        return;
    }

    b32 buf_has_separator = false;
    if( *buffer_len ) {
        buf_has_separator = ascii_is_path_separator( (char)buffer[*buffer_len - 1] );
    }

    if( chunk_has_separator ) {
        chunk = string_advance( chunk );
    }

    if( !buf_has_separator ) {
        buffer[*buffer_len] = L'\\';
        *buffer_len += 1;
    }
    MultiByteToWideChar(
        CP_UTF8, 0, chunk.buf, chunk.len,
        buffer + *buffer_len, buffer_cap - *buffer_len );
}
attr_internal
wchar_t* win32_canonical_from_path_ucs2_local( struct _StringPOD path ) {
    wchar_t* buffer = (wchar_t*)win32_get_local_buffer();
    win32_canonical_from_path_ucs2( CORE_PATH_NAME_LEN, buffer, path );
    return buffer;
}

attr_internal
TimePosix win32_filetime_to_time_posix( FILETIME ft ) {
    #define WIN32_TICKS_PER_SECOND (10000000)
    #define WIN32_TO_POSIX_DIFF    (11644473600ULL)

    ULARGE_INTEGER uli;
    uli.LowPart  = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    time_t res = (time_t)(
        (uli.QuadPart / WIN32_TICKS_PER_SECOND) - WIN32_TO_POSIX_DIFF );

    #undef WIN32_TICKS_PER_SECOND
    #undef WIN32_TO_POSIX_DIFF
    
    return res;
}

attr_internal 
void win32_log_error( DWORD error_code ) {
#if defined(CORE_ENABLE_LOGGING)
    char buf[255];
    String message = string_empty();
    message.c = buf;

    message.len = win32_get_error_message( 255, message.c, error_code );
    if( message.len ) {
        message = string_trim( message, 1 );
        core_error( "WIN32 {u,xu,f}: {s}", error_code, message );
    }
#endif
    unused( error_code );
}
attr_unused attr_internal
usize win32_get_error_message(
    usize buffer_size, char* buffer, DWORD error_code
) {
    DWORD res = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM, 0, error_code, 0, buffer, buffer_size, 0 );
    return res;
}

attr_internal
void win32_get_cpu_name( char* buffer ) {
#if defined(CORE_ARCH_X86)
    int cpu_info[4] = {0};
    char* chunk = buffer;

    __cpuid( cpu_info, 0x80000002 );
    memory_copy( chunk, cpu_info, sizeof(cpu_info) );
    chunk += sizeof(cpu_info);

    __cpuid( cpu_info, 0x80000003 );
    memory_copy( chunk, cpu_info, sizeof(cpu_info) );
    chunk += sizeof(cpu_info);

    __cpuid( cpu_info, 0x80000004 );
    memory_copy( chunk, cpu_info, sizeof(cpu_info) );

    chunk = buffer;
    for( u32 i = 254; i-- > 0; ) {
        if( chunk[i] && chunk[i] != ' ' ) {
            chunk[i + 1] = 0;
            break;
        }
    }
#endif /* Arch x86 */
}

typedef BOOL EnumDisplayDevicesAFN(
    LPCSTR lpDevice, DWORD iDevNum, PDISPLAY_DEVICEA lpDisplayDevice, DWORD dwFlags );

attr_internal
void win32_get_gpu_name( char* buffer ) {
    HMODULE user32 = LoadLibraryA( "USER32.DLL" );
    if( !user32 ) {
        return;
    }
    EnumDisplayDevicesAFN* EnumDisplayDevicesA =
        (void*)GetProcAddress( user32, "EnumDisplayDevicesA" );
    if( !EnumDisplayDevicesA ) {
        FreeLibrary( user32 );
        return;
    }

    DISPLAY_DEVICEA dd = {};
    dd.cb = sizeof(dd);

    EnumDisplayDevicesA( NULL, 0, &dd, EDD_GET_DEVICE_INTERFACE_NAME );

    u32 length    = cstr_len( dd.DeviceString );
    u32 copy_size = length;
    if( copy_size >= 255 ) {
        copy_size = 254;
    }
    memory_copy( buffer, dd.DeviceString, copy_size );

    FreeLibrary( user32 );
}

usize win32_utf16_buf_stream( void* target, usize count, const void* bytes ) {
    struct Win32UTF16Buf* buf = target;
    if( !buf ) {
        return count / sizeof(u16);
    }

    usize u16_count  = count / sizeof(u16);
    usize copy_count = (buf->cap - buf->len);
    if( copy_count > u16_count ) {
        copy_count = u16_count;
    }

    memory_copy( buf->buf + buf->len, bytes, sizeof(u16) * copy_count );
    buf->len += copy_count;
    return u16_count - copy_count;
}
struct _StringPOD platform_environment_query( struct _StringPOD key ) {
    u16*  wkey_buf   = (u16*)win32_get_local_buffer();
    u16*  wvalue_buf = wkey_buf + CORE_PATH_NAME_LEN;
    char* value_buf  = (char*)(wvalue_buf + CORE_PATH_NAME_LEN);

    struct Win32UTF16Buf wkey = {
        .cap = CORE_PATH_NAME_LEN,
        .len = 0,
        .buf = wkey_buf
    };
    unicode_utf16_from_utf8( win32_utf16_buf_stream, &wkey, key.len, key.bytes );

    int wvalue_len = GetEnvironmentVariableW( wkey.buf, wvalue_buf, CORE_PATH_NAME_LEN );
    if( GetLastError() == ERROR_ENVVAR_NOT_FOUND ) {
        core_error( "win32: environment_query: Key {s} not found!", key );
        return string_empty();
    }

    StringBuf value = string_buf_new( CORE_PATH_NAME_LEN, value_buf );
    unicode_utf8_from_utf16( string_buf_try_stream, &value, wvalue_len, wvalue_buf );

    return value.slice;
}
b32 platform_environment_set( struct _StringPOD key, struct _StringPOD value ) {
    u16* wkey_buf   = (u16*)win32_get_local_buffer();
    u16* wvalue_buf = wkey_buf + CORE_PATH_NAME_LEN;

    struct Win32UTF16Buf convert = {
        .cap = CORE_PATH_NAME_LEN,
        .len = 0,
        .buf = wkey_buf
    };
    unicode_utf16_from_utf8( win32_utf16_buf_stream, &convert, key.len, key.bytes );

    convert = (struct Win32UTF16Buf){
        .cap = CORE_PATH_NAME_LEN,
        .len = 0,
        .buf = wvalue_buf
    };
    unicode_utf16_from_utf8( win32_utf16_buf_stream, &convert, value.len, value.bytes );

    return SetEnvironmentVariableW( wkey_buf, wvalue_buf );
}
void win32_environment_block_size( const u16* block, usize* out_size, usize* out_pairs ) {
    usize size_counter = 0;
    usize pair_counter = 0;
    usize null_counter = 0;
    for( ;; ) {
        size_counter++;
        u16 c = *block;
        if( c ) {
            null_counter = 0;
        } else {
            null_counter++;
            if( null_counter >= 2 ) {
                break;
            } else {
                pair_counter++;
            }
        }
        block++;
    }

    *out_size  = size_counter;
    *out_pairs = pair_counter;
}
b32 win32_utf16_find( usize len, const u16* utf16, u16 c, usize* opt_out_position ) {
    for( usize i = 0; i < len; ++i ) {
        if( utf16[i] == c ) {
            if( opt_out_position ) {
                *opt_out_position = i;
            }
            return true;
        }
    }
    return false;
}
struct Win32EnvironmentBlock {
    usize cap;
    usize len;
    struct Win32EnvironmentKV {
        usize key_offset;
        usize key_len;
        usize value_offset;
        usize value_len;
    }* kv;
    struct Win32UTF16Buf text;
    u16*       complete;
    const u16* source;
    usize      source_size;
    usize      source_pairs;
};
void win32_environment_block_create( struct Win32EnvironmentBlock* out_block ) {
    out_block->source = GetEnvironmentStringsW();
    win32_environment_block_size(
        out_block->source, &out_block->source_size, &out_block->source_pairs );

    out_block->text.cap = out_block->source_size;
    out_block->text.len = 0;
    out_block->text.buf = memory_alloc( sizeof(u16) * out_block->text.cap );

    out_block->cap = out_block->source_pairs;
    out_block->len = 0;
    out_block->kv  = memory_alloc( sizeof(struct Win32EnvironmentKV) * out_block->cap );

    const u16* at = out_block->source;
    for( usize i = 0; i < out_block->source_pairs; ++i ) {
        const u16* pair = at;
        usize pair_len  = win32_ucs2_len( pair );

        const u16* key = pair;
        usize key_len;
        win32_utf16_find( pair_len, pair, '=', &key_len );

        const u16* value = key + key_len + 1;
        usize value_len  = pair_len - (key_len + 1);

        struct Win32EnvironmentKV kv;
        kv.key_len   = key_len;
        kv.value_len = value_len;

        usize key_ptr = (usize)memory_copy(
            out_block->text.buf + out_block->text.len, key, sizeof(u16) * key_len );
        out_block->text.len += key_len + 1;

        usize value_ptr = (usize)memory_copy(
            out_block->text.buf + out_block->text.len, value, sizeof(u16) * value_len );
        out_block->text.len += value_len + 1;

        kv.key_offset   = (key_ptr   - (usize)out_block->text.buf) / sizeof(u16);
        kv.value_offset = (value_ptr - (usize)out_block->text.buf) / sizeof(u16);

        out_block->kv[out_block->len++] = kv;

        at += pair_len + 1;
    }
}
void win32_environment_block_add(
    struct Win32EnvironmentBlock* block, String key, String value
) {
    struct Win32UTF16Buf buf = {};
    buf.cap = CORE_PATH_NAME_LEN;
    buf.len = 0;
    buf.buf = (u16*)win32_get_local_buffer();

    unicode_utf16_from_utf8( win32_utf16_buf_stream, &buf, key.len, key.bytes );

    // NOTE(alicia): search for key in environment block
    isize write_kv = -1;
    for( usize i = 0; i < block->len; ++i ) {
        struct Win32EnvironmentKV* kv = block->kv + i;
        const u16* this_key = block->text.buf + kv->key_offset;

        if( kv->key_len != buf.len ) {
            continue;
        }

        if( memory_cmp( this_key, buf.buf, sizeof(u16) * buf.len ) ) {
            write_kv = i;
            break;
        }
    }

    // NOTE(alicia): value is overwritten here
    if( write_kv >= 0 ) {
        struct Win32EnvironmentKV* kv = block->kv + write_kv;

        buf.len = 0;
        unicode_utf16_from_utf8( win32_utf16_buf_stream, &buf, value.len, value.bytes );
        if( (block->text.cap - block->text.len) < (buf.len + 255) ) {
            usize new_cap = block->text.cap + buf.len + 255;
            void* new_ptr = memory_realloc(
                block->text.buf, sizeof(u16) * block->text.cap, sizeof(u16) * new_cap );

            block->text.cap = new_cap;
            block->text.buf = new_ptr;
        }

        usize value_ptr = (usize)memory_copy(
            block->text.buf + block->text.len, buf.buf, sizeof(u16) * buf.len );
        block->text.len += buf.len + 1;
        kv->value_len    = buf.len;
        kv->value_offset = (value_ptr - (usize)block->text.buf) / sizeof(u16);

    } else {
        if( block->len == block->cap ) {
            usize new_cap = block->cap + 4;
            void* new_ptr = memory_realloc(
                block->kv,
                sizeof(struct Win32EnvironmentKV) * block->cap,
                sizeof(struct Win32EnvironmentKV) * new_cap );

            block->cap = new_cap;
            block->kv  = new_ptr;
        }

        struct Win32EnvironmentKV kv = {};

        if( (block->text.cap - block->text.len) < (buf.len + 255) ) {
            usize new_cap = block->text.cap + buf.len + 255;
            void* new_ptr = memory_realloc(
                block->text.buf, sizeof(u16) * block->text.cap, sizeof(u16) * new_cap );

            block->text.cap = new_cap;
            block->text.buf = new_ptr;
        }

        usize key_ptr = (usize)memory_copy(
            block->text.buf + block->text.len, buf.buf, sizeof(u16) * buf.len );
        block->text.len += buf.len + 1;
        kv.key_len = buf.len;

        buf.len = 0;
        unicode_utf16_from_utf8( win32_utf16_buf_stream, &buf, value.len, value.bytes );
        if( (block->text.cap - block->text.len) < (buf.len + 255) ) {
            usize new_cap = block->text.cap + buf.len + 255;
            void* new_ptr = memory_realloc(
                block->text.buf, sizeof(u16) * block->text.cap, sizeof(u16) * new_cap );

            block->text.cap = new_cap;
            block->text.buf = new_ptr;
        }

        usize value_ptr = (usize)memory_copy(
            block->text.buf + block->text.len, buf.buf, sizeof(u16) * buf.len );
        block->text.len += buf.len + 1;
        kv.value_len = buf.len;
        
        kv.key_offset   = (key_ptr   - (usize)block->text.buf) / sizeof(u16);
        kv.value_offset = (value_ptr - (usize)block->text.buf) / sizeof(u16);

        block->kv[block->len++] = kv;
    }
}
void win32_environment_block_complete(
    struct Win32EnvironmentBlock* block, usize* out_len, u16** out_env
) {
    struct Win32UTF16Buf buf = {};
    for( usize i = 0; i < block->len; ++i ) {
        struct Win32EnvironmentKV kv = block->kv[i];
        buf.cap += kv.key_len + kv.value_len + 2; // = and null"
    }
    buf.cap++; // second null at the end

    buf.buf = memory_alloc( sizeof(u16) * buf.cap );

    for( usize i = 0; i < block->len; ++i ) {
        struct Win32EnvironmentKV kv = block->kv[i];
        u16* key   = block->text.buf + kv.key_offset;
        u16* value = block->text.buf + kv.value_offset;

        memory_copy( buf.buf + buf.len, key, sizeof(u16) * kv.key_len );
        buf.len += kv.key_len;

        buf.buf[buf.len++] = '=';

        memory_copy( buf.buf + buf.len, value, sizeof(u16) * kv.value_len );
        buf.len += kv.value_len;

        buf.buf[buf.len++] = 0;
    }
    buf.buf[buf.len++] = 0;

    *out_len = buf.cap;
    *out_env = buf.buf;

    memory_free( block->text.buf, sizeof(u16) * block->text.cap );
    memory_free( block->kv, sizeof(struct Win32EnvironmentKV) * block->cap );
    FreeEnvironmentStringsW( (LPWCH)block->source );
}
b32 platform_process_exec_async(
    Command                  command,
    Process*                 out_pid,
    const struct _StringPOD* opt_working_directory,
    const EnvironmentBuf*    opt_environment,
    const FD*                opt_stdin,
    const FD*                opt_stdout,
    const FD*                opt_stderr
) {
    STARTUPINFOW        si = {};
    PROCESS_INFORMATION pi = {};

    si.cb      = sizeof(si);
    si.dwFlags = 0; // STARTF_USESTDHANDLES;

    DWORD dwCreationFlags = 0;
    BOOL  bInheritHandles = FALSE;
    if( opt_stdin ) {
        si.hStdInput    = (HANDLE)opt_stdin->opaque;
        bInheritHandles = TRUE;
    } else {
        si.hStdInput = GetStdHandle( STD_INPUT_HANDLE );
    }
    if( opt_stdout ) {
        si.hStdOutput   = (HANDLE)opt_stdout->opaque;
        bInheritHandles = TRUE;
    } else {
        si.hStdOutput = GetStdHandle( STD_OUTPUT_HANDLE );
    }
    if( opt_stderr ) {
        si.hStdError    = (HANDLE)opt_stderr->opaque;
        bInheritHandles = TRUE;
    } else {
        si.hStdError = GetStdHandle( STD_ERROR_HANDLE );
    }
    if( bInheritHandles ) {
        si.dwFlags |= STARTF_USESTDHANDLES;
    }

    // TODO(alicia): if environment is passed, create environment block
    // GetEnvironmentStringsW to obtain current env
    //   check for environment variables in opt_environment against this process's block and
    //   replace each variable that exists in both with the opt_environment value
    // FreeEnvironmentStringsW to free current env

    usize env_len = 0;
    u16*  env     = NULL;
    unused(env_len, env);
    if( opt_environment && opt_environment->len ) {
        const EnvironmentBuf* e = opt_environment;
        dwCreationFlags = CREATE_UNICODE_ENVIRONMENT;

        struct Win32EnvironmentBlock env_block;
        win32_environment_block_create( &env_block );

        usize pair_count = e->len * 2;
        for( usize pair = 0; pair < pair_count; pair += 2 ) {
            String key   = e->buf[pair + 0];
            String value = e->buf[pair + 1];

            win32_environment_block_add( &env_block, key, value );
        }

        win32_environment_block_complete( &env_block, &env_len, &env );
    }

    // TODO(alicia): expand command to wide string with \"\" where needed
    struct Win32UTF16Buf utf16_buf;
    utf16_buf.cap = CORE_PATH_NAME_LEN * 2;
    utf16_buf.len = 0;
    utf16_buf.buf = (u16*)win32_get_local_buffer() + CORE_PATH_NAME_LEN;

    for( usize i = 0; i < command.len; ++i ) {
        String arg = command.buf[i];

        usize space = 0;
        b32   has_space = string_find( arg, ' ', &space );
        if( has_space ) {
            if( utf16_buf.len != utf16_buf.cap ) {
                utf16_buf.buf[utf16_buf.len++] = '"';
            }
        }

        unicode_utf16_from_utf8( win32_utf16_buf_stream, &utf16_buf, arg.len, arg.bytes );

        if( has_space ) {
            if( utf16_buf.len != utf16_buf.cap ) {
                utf16_buf.buf[utf16_buf.len++] = '"';
            }
        }

        if( ((i + 1) < command.len) && utf16_buf.len != utf16_buf.cap ) {
            utf16_buf.buf[utf16_buf.len++] = ' ';
        }
    }
    if( utf16_buf.len == utf16_buf.cap ) {
        utf16_buf.buf[utf16_buf.len - 1] = 0;
    } else {
        utf16_buf.buf[utf16_buf.len++] = 0;
    }

    wchar_t* working_directory = NULL;
    if( opt_working_directory ) {
        working_directory = win32_canonical_from_path_ucs2_local( *opt_working_directory );
    }

    b32 result = CreateProcessW(
        NULL, utf16_buf.buf, NULL, NULL, bInheritHandles,
        dwCreationFlags, env, working_directory, &si, &pi );
    DWORD error_code = GetLastError();

    if( env && env_len ) {
        memory_free( env, sizeof(u16) * env_len );
    }

    if( result ) {
        out_pid->opaque = pi.hProcess;
        CloseHandle( pi.hThread );
        return true;
    }

    win32_log_error( error_code );
    core_error(
        "Windows: process_exec: "
        "failed to execute process {s}!", command.buf[0] );
    return false;
}
void platform_process_discard( Process* pid ) {
    if( pid && pid->opaque ) {
        CloseHandle( (HANDLE)pid->opaque );
        pid->opaque = NULL;
    }
}
int platform_process_wait( Process* pid ) {
    int exit_code = 0;
    if( platform_process_wait_timed( pid, INFINITE, &exit_code ) ) {
        return exit_code;
    }
    return -2;
}
b32  platform_process_wait_timed( Process* pid, u32 msec, int* opt_out_exit_code ) {
    DWORD result = WaitForSingleObject( pid->opaque, msec );
    switch( result ) {
        case WAIT_OBJECT_0:
            break;
        case WAIT_TIMEOUT:
            return false;
        default: {
            if( opt_out_exit_code ) {
                *opt_out_exit_code = -2;
            }
            win32_log_error( GetLastError() );
            core_error(
                "win32: process_wait: "
                "failed to wait for pid!" );
        } return false;
    }

    DWORD exit_code = 0;
    if( !GetExitCodeProcess( pid->opaque, &exit_code ) ) {
        win32_log_error( GetLastError() );
        core_error( "win32: process_wait: failed to get exit code!" );
        if( opt_out_exit_code ) {
            *opt_out_exit_code = -2;
        }
        return false;
    }

    if( opt_out_exit_code ) {
        *opt_out_exit_code = exit_code;
    }
    platform_process_discard( pid );
    return true;
}
void platform_process_kill( Process* pid ) {
    TerminateProcess( pid->opaque, 0 );
    platform_process_discard( pid );
}
b32 platform_process_find( struct _StringPOD process_name ) {
    char* utf8_buf  = win32_get_local_buffer();
    u16*  utf16_buf = (u16*)(utf8_buf + CORE_PATH_NAME_LEN);

    struct Win32UTF16Buf wname = {
        .cap = CORE_PATH_NAME_LEN,
        .len = 0,
        .buf = utf16_buf
    };

    StringBuf buf = string_buf_new( CORE_PATH_NAME_LEN, utf8_buf );
    string_buf_try_fmt_text( &buf, "CMD /C WHERE {s}", process_name );
    if( buf.len == buf.cap ) {
        buf.buf[buf.len - 1] = 0;
    } else {
        buf.buf[buf.len] = 0;
    }

    unicode_utf16_from_utf8( win32_utf16_buf_stream, &wname, buf.len, buf.bytes );

    STARTUPINFOW si = {};
    si.cb      = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;

    SECURITY_ATTRIBUTES sa = {};
    sa.nLength             = sizeof(sa);
    sa.bInheritHandle      = TRUE;

    HANDLE read, write;
    CreatePipe( &read, &write, &sa, 0 );

    si.hStdInput  = read;
    si.hStdOutput = write;
    si.hStdError  = write;

    b32 bInheritHandles = TRUE;

    PROCESS_INFORMATION pi = {};

    DWORD dwCreationFlags = 0;
    b32 result = CreateProcessW(
        NULL, wname.buf, NULL, NULL, bInheritHandles,
        dwCreationFlags, NULL, NULL, &si, &pi );

    CloseHandle( read );
    CloseHandle( write );

    if( !result ) {
        core_error( "win32: process_find: failed to search for processes!" );
        return false;
    }

    CloseHandle( pi.hThread );

    Process pid;
    pid.opaque = pi.hProcess;

    return platform_process_wait( &pid ) == 0;
}

#endif
