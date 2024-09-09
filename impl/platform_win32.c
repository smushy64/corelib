/**
 * Description:  Platform Win32 implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 08, 2024
*/
#include "core/defines.h"
#if defined(CORE_PLATFORM_WINDOWS)

#include "core/types.h"
#include "core/macros.h"
#include "core/constants.h"
#include "core/sync.h"
#include "core/memory.h"
#include "core/alloc.h"

#include "core/internal/platform.h"
#include "core/internal/logging.h"

#define NOMINMAX
#include <windows.h>
#include <intrin.h>

#if defined(CORE_COMPILER_MSVC)
    int _fltused;
#endif

typedef BOOL EnumDisplayDevicesAFN(
    LPCSTR lpDevice, DWORD iDevNum,
    PDISPLAY_DEVICEA lpDisplayDevice, DWORD dwFlags );
attr_global attr_unused
EnumDisplayDevicesAFN* in_EnumDisplayDevicesA = NULL;
#define EnumDisplayDevicesA in_EnumDisplayDevicesA

attr_unused
attr_internal usize win32_get_error_message(
    usize buffer_size, char* buffer, DWORD error_code );
attr_internal void win32_log_error( DWORD error_code );
attr_internal TimePosix win32_ft_to_ts( FILETIME ft );

attr_internal u32 win32_utf8_to_wide(
    u32 utf8_len, const char* utf8, u32 wide_cap, wchar_t* wide_buf );
attr_internal u32 win32_wide_to_utf8(
    u32 wide_len, const wchar_t* wide, u32 utf8_cap, char* utf8_buf );
attr_internal u32 win32_path_to_wide(
    u32 utf8_len, const char* utf8, u32 wide_cap, wchar_t* wide_buf, u32* out_len );

attr_global struct Win32Global {
    HANDLE user32;

    volatile u32 thread_id;

    String cpu_name;
    String gpu_name;

    DWORD    cwd_len;
    wchar_t* cwd;

    DWORD    home_len;
    wchar_t* home;

    PipeRead  in;
    PipeWrite out, err;

    LARGE_INTEGER qpf;

    char cpu_name_buf[255];
} global_win32;

struct DirectoryWalk {
    HANDLE           h;
    WIN32_FIND_DATAW fd;
    b32              is_first;
    u32              path_cap;
    u32              path_len;
    wchar_t          wide_path[];
};

b32 win32_init(void) {
    SetConsoleOutputCP( CP_UTF8 );
    SetConsoleCP( CP_UTF8 );

    DWORD cwd_cap   = GetCurrentDirectoryW( 0, 0 );
    DWORD drive_cap = GetEnvironmentVariableW( L"HOMEDRIVE", 0, 0 );
    DWORD hpath_cap = GetEnvironmentVariableW( L"HOMEPATH", 0, 0 );
    DWORD required_memory = cwd_cap + drive_cap + hpath_cap;
    wchar_t* paths_buffer = LocalAlloc( 0, sizeof(wchar_t) * required_memory );
    if( !paths_buffer ) {
        return false;
    }

    global_win32.cwd_len = cwd_cap - 1;
    global_win32.cwd     = paths_buffer;
    GetCurrentDirectoryW( cwd_cap, global_win32.cwd );

    global_win32.home = paths_buffer + cwd_cap;
    GetEnvironmentVariableW( L"HOMEDRIVE", global_win32.home, drive_cap );
    GetEnvironmentVariableW( L"HOMEPATH",
        global_win32.home + (drive_cap - 1), hpath_cap );
    global_win32.home_len = (drive_cap + hpath_cap) - 2;

    global_win32.in.fd.opaque  = GetStdHandle( STD_INPUT_HANDLE );
    global_win32.out.fd.opaque = GetStdHandle( STD_OUTPUT_HANDLE );
    global_win32.err.fd.opaque = GetStdHandle( STD_ERROR_HANDLE );

    global_win32.cpu_name.c = global_win32.cpu_name_buf;

    QueryPerformanceFrequency( &global_win32.qpf );

    return true;
}
void win32_deinit(void) {
    if( EnumDisplayDevicesA ) {
        if( global_win32.user32 ) {
            CloseHandle( global_win32.user32 );
            global_win32.user32 = NULL;
        }
    }

    memory_zero( &global_win32, sizeof( global_win32 ) );
}

void* platform_heap_alloc( const usize size ) {
    return HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, size );
}
void* platform_heap_realloc(
    void* old_buffer, const usize old_size, const usize new_size
) {
    unused(old_size);
    void* result = HeapReAlloc(
        GetProcessHeap(), HEAP_ZERO_MEMORY, old_buffer, new_size );

    return result;
}
void platform_heap_free( void* buffer, const usize size ) {
    unused( size );
    HeapFree( GetProcessHeap(), 0, buffer );
}

attr_internal TimePosix win32_ft_to_ts( FILETIME ft ) {
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
TimePosix platform_time_posix(void) {
    FILETIME ft;
    memory_zero( &ft, sizeof(ft) );
    GetSystemTimeAsFileTime( &ft );

    return win32_ft_to_ts( ft );
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

    return ((f64)qpc.QuadPart / (f64)global_win32.qpf.QuadPart) * 1000.0;
}
f64 platform_timer_seconds(void) {
    LARGE_INTEGER qpc;
    QueryPerformanceCounter( &qpc );

    return (f64)qpc.QuadPart / (f64)global_win32.qpf.QuadPart;
}

void platform_console_write( void* dst, usize len, const char* str ) {
    DWORD chars_to_write = len;
    DWORD chars_written;
    WriteConsoleA(
        dst, str, chars_to_write, &chars_written, NULL );
    if( len > U32_MAX ) {
        chars_to_write = len - U32_MAX;
        WriteConsoleA(
            dst, str + U32_MAX,
            chars_to_write, &chars_written, NULL );
    }
}

struct Win32Semaphore {
    HANDLE handle;
};
#define to_win32sem( s )\
    rcast_ref( struct Win32Semaphore, s )

struct Win32Mutex {
    HANDLE handle;
};
#define to_win32mut( m )\
    rcast_ref( struct Win32Mutex, m )

b32 platform_semaphore_create(
    const char* name, struct Semaphore* out_sem
) {
    HANDLE handle = CreateSemaphoreEx(
        NULL, 0, I32_MAX, name, 0, SEMAPHORE_ALL_ACCESS );
    if( handle == INVALID_HANDLE_VALUE ) {
        return false;
    }

    struct Win32Semaphore* sem = to_win32sem( out_sem );
    sem->handle = handle;

    return true;
}
void platform_semaphore_destroy( struct Semaphore* in_sem ) {
    struct Win32Semaphore* sem = to_win32sem( in_sem );
    CloseHandle( sem->handle );
}
void platform_semaphore_signal( struct Semaphore* in_sem ) {
    struct Win32Semaphore* sem = to_win32sem(in_sem);
    ReleaseSemaphore( sem->handle, 1, NULL );
}
b32 platform_semaphore_wait( struct Semaphore* in_sem, u32 ms ) {
    struct Win32Semaphore* sem = to_win32sem( in_sem );

    // NOTE(alicia): U32_MAX == INFINITE
    DWORD result = WaitForSingleObject( sem->handle, ms );

    if( ms != U32_MAX ) {
        return result != WAIT_TIMEOUT;
    }
    return true;
}

b32 platform_mutex_create( const char* name, struct Mutex* out_mutex ) {
    struct Win32Mutex* mutex = to_win32mut( out_mutex );
    HANDLE handle = CreateMutexA( NULL, false, name );

    if( handle == INVALID_HANDLE_VALUE ) {
        return false;
    }

    mutex->handle = handle;
    return true;
}
void platform_mutex_destroy( struct Mutex* in_mutex ) {
    struct Win32Mutex* mutex = to_win32mut( in_mutex );
    CloseHandle( mutex->handle );
}
b32 platform_mutex_lock( struct Mutex* in_mutex, u32 ms ) {
    struct Win32Mutex* mutex = to_win32mut( in_mutex );

    // NOTE(alicia): U32_MAX == INFINITE
    DWORD result = WaitForSingleObject( mutex->handle, ms );

    if( ms != U32_MAX ) {
        return result != WAIT_TIMEOUT;
    }
    return true;
}
void platform_mutex_unlock( struct Mutex* in_mutex ) {
    struct Win32Mutex* mutex = to_win32mut( in_mutex );
    ReleaseMutex( mutex->handle );
}

void platform_sleep( u32 ms ) {
    Sleep( (DWORD)ms );
}

struct Win32Thread {
    atomic32*     ready;
    ThreadMainFN* main;
    void*         params;
};

attr_internal DWORD internal_win32_thread_proc( void* in ) {
    struct Win32Thread thread = rcast( struct Win32Thread, in );
    read_write_barrier();

    atomic_increment32( thread.ready );

    u32 thread_id = atomic_increment32( &global_win32.thread_id );
    read_write_barrier();

    volatile int ret = thread.main( thread_id, thread.params );

    ExitThread( ret );
}

b32 platform_thread_create(
    ThreadMainFN* main, void* params,
    usize stack_size, ThreadHandle* out_handle
) {
    atomic32 ready = 0;
    struct Win32Thread thread_params;
    thread_params.ready  = &ready;
    thread_params.main   = main;
    thread_params.params = params;

    read_write_barrier();

    DWORD  id     = 0;
    HANDLE handle = CreateThread(
        NULL, stack_size, internal_win32_thread_proc, &thread_params, 0, &id );
    if( handle == NULL ) {
        DWORD error_code = GetLastError();
        win32_log_error( error_code );
        return false;
    }

    read_write_barrier();

    while( !ready ) {}

    read_write_barrier();

    out_handle->opaque = handle;
    return true;
}
void platform_thread_destroy( ThreadHandle* handle ) {
    TerminateThread( handle->opaque, -1 );
    CloseHandle( handle->opaque );
}
void platform_thread_free( ThreadHandle* handle ) {
    CloseHandle( handle->opaque );
}
b32 platform_thread_join_timed(
    ThreadHandle* handle, u32 ms, int* opt_out_exit_code
) {
    DWORD result = WaitForSingleObject( handle->opaque, ms );
    switch( result ) {
        case WAIT_OBJECT_0: break;
        case WAIT_TIMEOUT: {
            return false;
        } break;
        default: {

        } return false;
    }

    if( opt_out_exit_code ) {
        platform_thread_exit_code( handle, opt_out_exit_code );
    }
    return true;
}
b32 platform_thread_exit_code( ThreadHandle* handle, int* out_exit_code ) {
    DWORD result = WaitForSingleObject( handle->opaque, 0 );
    if( result != WAIT_OBJECT_0 ) {
        return false;
    }

    DWORD exit_code = 0;
    GetExitCodeThread( handle, &exit_code );

    *out_exit_code = rcast( int, &exit_code );
    return true;
}
attr_unused
attr_internal usize win32_get_error_message(
    usize buffer_size, char* buffer, DWORD error_code
) {
    DWORD res = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM, 0, error_code, 0, buffer, buffer_size, 0 );
    return res;
}
attr_internal void win32_log_error( DWORD error_code ) {
#if defined(CORE_ENABLE_LOGGING)
    char buf[255];
    String message = string_empty();
    message.c = buf;

    message.len = win32_get_error_message( 255, message.c, error_code );
    if( message.len ) {
        message = string_trim( message, 1 );
        core_error( "win32: [{u,xu,f}] {s}", error_code, message );
    }
#endif
    unused( error_code );
}

void platform_fd_close( FD* fd ) {
    if( fd && fd->opaque ) {
        CloseHandle( fd->opaque );
        memory_zero( fd, sizeof(*fd) );
    }
}
usize platform_fd_query_size( FD* fd ) {
    // TODO(alicia): error checking?
#if defined(CORE_ARCH_64_BIT)
    LARGE_INTEGER li;
    li.QuadPart = 0;
    GetFileSizeEx( fd->opaque, &li );
    return li.QuadPart;
#else
    DWORD res = 0;
    GetFileSize( fd->opaque, &res );
    return res;
#endif
}
void platform_fd_truncate( FD* fd ) {
    SetEndOfFile( fd->opaque );
}
usize platform_fd_seek( FD* fd, FileSeek type, isize seek ) {
    DWORD dwMoveMethod = FILE_BEGIN;
    switch( type ) {
        case FSEEK_CURRENT: {
            dwMoveMethod = FILE_CURRENT;
        } break;
        case FSEEK_END: {
            dwMoveMethod = FILE_END;
        } break;
        case FSEEK_SET: break;
    }

#if defined(CORE_ARCH_64_BIT)
    LARGE_INTEGER move, new_pointer;
    move.QuadPart        = seek;
    new_pointer.QuadPart = 0;
    SetFilePointerEx( fd->opaque, move, &new_pointer, dwMoveMethod );
    return new_pointer.QuadPart;
#else
    return SetFilePointer( fd->opaque, seek, NULL, dwMoveMethod );
#endif
}
attr_internal b32 win32_fd_write32(
    HANDLE handle, u32 bytes, const void* buf, u32* opt_out_write
) {
    DWORD bytes_written = 0;
    BOOL  res           = FALSE;

    if( GetFileType( handle ) == FILE_TYPE_CHAR ) {
        res = WriteConsoleA( handle, buf, bytes, &bytes_written, NULL );
    } else {
        res = WriteFile( handle, buf, bytes, &bytes_written, NULL );
    }

    if( opt_out_write ) {
        *opt_out_write = res ? bytes_written : 0;
    }

    return res != FALSE;
}
b32 platform_fd_write(
    FD* fd, usize bytes, const void* buf, usize* opt_out_write
) {
#if defined(CORE_ARCH_64_BIT)
    if( bytes > U32_MAX ) {
        u32 lo = U32_MAX;
        u32 hi = bytes - lo;

        u32 written = 0;
        if( !win32_fd_write32( fd->opaque, lo, buf, &written ) ) {
            *opt_out_write = 0;
            return false;
        }

        if( opt_out_write ) {
            *opt_out_write = written;
        }
        written = 0;

        if( !win32_fd_write32( fd->opaque, hi, (const u8*)buf + lo, &written ) ) {
            return false;
        }

        if( opt_out_write ) {
            *opt_out_write += written;
        }
        return true;
    } else {
        u32 written = 0;
        b32 res = win32_fd_write32( fd->opaque, bytes, buf, &written );
        if( opt_out_write ) {
            *opt_out_write = written;
        }
        return res;
    }
#else
    return win32_fd_write32( fd->opaque, bytes, buf, opt_out_write );
#endif
}
attr_internal b32 win32_fd_read(
    HANDLE handle, u32 buf_size, void* buf, u32* opt_out_read
) {
    DWORD read = 0;
    if( ReadFile( handle, buf, buf_size, &read, NULL ) ) {
        if( opt_out_read ) {
            *opt_out_read = read;
        }
        return true;
    } else {
        if( opt_out_read ) {
            *opt_out_read = 0;
        }
        return false;
    }
}
b32 platform_fd_read(
    FD* fd, usize buf_size, void* buf, usize* opt_out_read
) {
#if defined(CORE_ARCH_64_BIT)
    if( buf_size > U32_MAX ) {
        u32 lo = U32_MAX;
        u32 hi = buf_size - lo;

        u32 read = 0;
        if( !win32_fd_read( fd->opaque, lo, buf, &read ) ) {
            *opt_out_read = 0;
            return false;
        }

        if( opt_out_read ) {
            *opt_out_read = read;
        }
        read = 0;

        if( !win32_fd_read( fd->opaque, hi, (u8*)buf + lo, &read ) ) {
            return false;
        }

        if( opt_out_read ) {
            *opt_out_read += read;
        }
        return true;
    } else {
        u32 read = 0;
        b32 res = win32_fd_read( fd->opaque, buf_size, buf, &read );
        if( opt_out_read ) {
            *opt_out_read = read;
        }
        return res;
    }
#else
    return win32_fd_read( fd->opaque, buf_size, buf, opt_out_read );
#endif
}

b32 platform_pipe_open( PipeRead* out_read, PipeWrite* out_write ) {
    HANDLE read, write;

    SECURITY_ATTRIBUTES sa;
    memory_zero( &sa, sizeof(sa) );
    sa.nLength        = sizeof(sa);
    sa.bInheritHandle = TRUE;

    if( !CreatePipe( &read, &write, &sa, 0 ) ) {
        return false;
    }
    
    out_read->fd.opaque  = read;
    out_write->fd.opaque = write;

    return true;
}

PipeRead* platform_stdin(void) {
    return &global_win32.in;
}
PipeWrite* platform_stdout(void) {
    return &global_win32.out;
}
PipeWrite* platform_stderr(void) {
    return &global_win32.err;
}

attr_internal u32 win32_utf8_to_wide(
    u32 utf8_len, const char* utf8, u32 wide_cap, wchar_t* wide_buf
) {
    return MultiByteToWideChar(
        CP_UTF8, 0, utf8, utf8_len, wide_buf, wide_cap );
}
attr_internal u32 win32_wide_to_utf8(
    u32 wide_len, const wchar_t* wide, u32 utf8_cap, char* utf8_buf
) {
    return WideCharToMultiByte(
        CP_UTF8, 0, wide, wide_len, utf8_buf, utf8_cap, 0, 0 );
}
attr_internal b32 win32_path_to_wide(
    u32 utf8_len, const char* utf8, u32 wide_cap, wchar_t* wide_buf, u32* opt_out_len
) {
    enum {
        PATH_REL,
        PATH_HOME,
        PATH_ABS
    } type = PATH_REL;

    Path path = path_new( utf8_len, utf8 );
    u32  len  = sizeof("\\\\?");
    if( len > wide_cap ) {
        return false;
    }
    memory_copy( wide_buf, L"\\\\?\\", sizeof(L"\\\\?") );

    u32 min = sizeof( "\\\\?\\A:" );
    if( path.len >= sizeof("A:") ) {
        if( ascii_is_alphabetic( path.cc[0] ) && path.cc[1] == ':' ) {
            type = PATH_ABS;
        }
    }
    if( type != PATH_ABS ) {
        if( path.cc[0] == '~' ) {
            type = PATH_HOME;
        }
    }

    switch( type ) {
        case PATH_REL: {
            if( global_win32.cwd_len + len > wide_cap ) {
                return false;
            }
            memory_copy(
                wide_buf + len, global_win32.cwd,
                sizeof(wchar_t) * global_win32.cwd_len );
            len += global_win32.cwd_len;
        } break;
        case PATH_HOME: {
            if( global_win32.home_len + len > wide_cap ) {
                return false;
            }
            memory_copy(
                wide_buf + len, global_win32.home,
                sizeof(wchar_t) * global_win32.home_len );
            len += global_win32.home_len;

            path = string_advance_by( path, 2 );
        } break;
        case PATH_ABS: break;
    }

    while( !string_is_empty( path ) ) {
        String chunk = path;
        usize sep = 0;
        if( string_find_set( path, string_text( "/\\" ), &sep ) ) {
            if( !sep ) {
                path = string_advance( path );
                continue;
            }

            chunk = string_truncate( path, sep );
        }

        if( chunk.len < 3 ) {
            if( string_cmp( chunk, string_text( "." ) ) ) {
                path = string_advance_by( path, chunk.len + 1 );
                continue;
            }
            if( string_cmp( chunk, string_text( ".." ) ) ) {
                for( usize i = len; i-- > 0; ) {
                    if( wide_buf[i] == '\\' ) {
                        len = i;
                        break;
                    }
                }

                if( len < min ) {
                    len = min;
                }

                wide_buf[len] = 0;
                path = string_advance_by( path, chunk.len + 1 );
                continue;
            }
        }

        if( wide_buf[len - 1] != '\\' ) {
            wide_buf[len++] = '\\';
        }

        len += win32_utf8_to_wide(
            chunk.len, chunk.cc, wide_cap - len, wide_buf + len );

        path = string_advance_by( path, chunk.len + 1 );
    }

    wide_buf[len] = 0;
    if( opt_out_len ) {
        *opt_out_len = len;
    }
    return true;
}
HANDLE win32_open_short(
    const char* path, DWORD dwDesiredAccess, DWORD dwShareMode,
    DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes
) {
    HANDLE handle = CreateFileA(
        path, dwDesiredAccess, dwShareMode, 0, dwCreationDisposition,
        dwFlagsAndAttributes, 0 );

    if( handle == INVALID_HANDLE_VALUE ) {
        win32_log_error( GetLastError() );
        return NULL;
    }

    return handle;
}
HANDLE win32_open_long(
    const wchar_t* path, DWORD dwDesiredAccess, DWORD dwShareMode,
    DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes
) {
    HANDLE handle = CreateFileW(
        path, dwDesiredAccess, dwShareMode, 0, dwCreationDisposition,
        dwFlagsAndAttributes, 0 );

    if( handle == INVALID_HANDLE_VALUE ) {
        win32_log_error( GetLastError() );
        return NULL;
    }

    return handle;
}
#define win32_long_path( path, buf )\
    win32_path_to_wide( path.len, path.cc, CORE_MAX_PATH_NAME, buf, 0 )
b32 platform_fd_open( Path path, FileOpenFlags flags, FD* out_fd ) {
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

    if( path.len < MAX_PATH ) {
        HANDLE handle = win32_open_short( 
            path.cc, dwDesiredAccess, dwShareMode,
            dwCreationDisposition, dwFlagsAndAttributes );
        out_fd->opaque = handle;
    } else {
        wchar_t* buf = memory_alloc( sizeof(wchar_t) * CORE_MAX_PATH_NAME );
        if( !buf ) {
            core_error( "win32: failed to allocate long path buffer!" );
            return false;
        }

        win32_long_path( path, buf );
        OutputDebugStringW( buf );

        HANDLE handle = win32_open_long(
            buf, dwDesiredAccess, dwShareMode, dwCreationDisposition,
            dwFlagsAndAttributes );

        memory_free( buf, sizeof(wchar_t) * CORE_MAX_PATH_NAME );
        out_fd->opaque = handle;
    }

    if( !out_fd->opaque ) {
        return false;
    }

    if( append ) {
        fd_seek( out_fd, FSEEK_END, 0 );
    }

    return true;
}

attr_internal b32 win32_file_copy_short(
    const char* dst, const char* src, b32 create_dst
) {
#if defined(CORE_ENABLE_LOGGING)
    BOOL res = CopyFileA( dst, src, create_dst );
    if( !res ) {
        win32_log_error( GetLastError() );
    }
    return res;
#else
    return CopyFileA( dst, src, create_dst );
#endif
}
attr_internal b32 win32_file_copy_long(
    const wchar_t* dst, const wchar_t* src, b32 create_dst
) {
#if defined(CORE_ENABLE_LOGGING)
    BOOL res = CopyFileW( dst, src, create_dst );
    if( !res ) {
        win32_log_error( GetLastError() );
    }
    return res;
#else
    return CopyFileW( dst, src, create_dst );
#endif
}
b32 platform_file_copy( Path dst, Path src, b32 create_dst ) {

    if( dst.len > MAX_PATH || src.len > MAX_PATH ) {
        usize buf_size = (sizeof(wchar_t) * CORE_MAX_PATH_NAME) * 2;
        wchar_t* dst_long = memory_alloc( buf_size );
        if( !dst_long ) {
            core_error(
                "win32: file_copy: "
                "failed to allocate {usize,m} for long path name buffer!",
                buf_size );
            return false;
        }
        wchar_t* src_long = dst_long + CORE_MAX_PATH_NAME;

        win32_long_path( dst, dst_long );
        win32_long_path( src, src_long );

        b32 res = win32_file_copy_long( dst_long, src_long, create_dst );

        memory_free( dst_long, buf_size );

        return res;
    } else {
        return win32_file_copy_short( dst.cc, src.cc, create_dst );
    }
}
attr_internal b32 win32_file_move_short( const char* dst, const char* src ) {
#if defined(CORE_ENABLE_LOGGING)
    BOOL res = MoveFileA( dst, src );
    if( !res ) {
        win32_log_error( GetLastError() );
    }
    return res;
#else
    return MoveFileA( dst, src );
#endif
}
attr_internal b32 win32_file_move_long( const wchar_t* dst, const wchar_t* src ) {
#if defined(CORE_ENABLE_LOGGING)
    BOOL res = MoveFileW( dst, src );
    if( !res ) {
        win32_log_error( GetLastError() );
    }
    return res;
#else
    return MoveFileW( dst, src );
#endif
}
attr_internal b32 win32_file_exists_short( const char* path ) {
    DWORD attr = GetFileAttributesA( path );
    return
        ( attr != INVALID_FILE_ATTRIBUTES ) &&
        !( attr & FILE_ATTRIBUTE_DIRECTORY );
}
attr_internal b32 win32_file_exists_long( const wchar_t* path ) {
    DWORD attr = GetFileAttributesW( path );
    return
        ( attr != INVALID_FILE_ATTRIBUTES ) &&
        !( attr & FILE_ATTRIBUTE_DIRECTORY );
}
b32 platform_file_move( Path dst, Path src, b32 create_dst ) {
    if( dst.len > MAX_PATH || src.len > MAX_PATH ) {
        usize buf_size = (sizeof(wchar_t) * CORE_MAX_PATH_NAME) * 2;
        wchar_t* dst_long = memory_alloc( buf_size );
        if( !dst_long ) {
            core_error(
                "win32: file_move: "
                "failed to allocate {usize,m} for long path name buffer!",
                buf_size );
            return false;
        }
        wchar_t* src_long = dst_long + CORE_MAX_PATH_NAME;

        win32_long_path( dst, dst_long );
        win32_long_path( src, src_long );

        if( create_dst ) {
            if( win32_file_exists_long( dst_long ) ) {
                memory_free( dst_long, buf_size );
                return false;
            }
        }

        b32 res = win32_file_move_long( dst_long, src_long );

        memory_free( dst_long, buf_size );

        return res;
    } else {
        if( create_dst ) {
            if( win32_file_exists_short( dst.cc ) ) {
                return false;
            }
        }
        return win32_file_move_short( dst.cc, src.cc );
    }
}
attr_internal b32 win32_file_remove_short( const char* path ) {
    return DeleteFileA( path ) != FALSE;
}
attr_internal b32 win32_file_remove_long( const wchar_t* path ) {
    return DeleteFileW( path ) != FALSE;
}
b32 platform_file_remove( Path path ) {
    if( path.len > MAX_PATH ) {
        usize buf_size = sizeof(wchar_t) * CORE_MAX_PATH_NAME;
        wchar_t* buf = memory_alloc( buf_size );
        if( !buf ) {
            core_error(
                "win32: file_remove: "
                "failed to allocate {usize,m} for long path name buffer!",
                buf_size );
            return false;
        }

        win32_long_path( path, buf );

        b32 res = win32_file_remove_long( buf );

        memory_free( buf, buf_size );
        return res;
    } else {
        return win32_file_remove_short( path.cc );
    }
}
b32 platform_file_exists( Path path ) {
    if( path.len > MAX_PATH ) {
        usize buf_size = sizeof(wchar_t) * CORE_MAX_PATH_NAME;
        wchar_t* buf = memory_alloc( buf_size );
        if( !buf ) {
            core_error(
                "win32: file_exists: "
                "failed to allocate {usize,m} for long path name buffer!",
                buf_size );
            return false;
        }

        win32_long_path( path, buf );

        b32 res = win32_file_exists_long( buf );

        memory_free( buf, buf_size );

        return res;
    } else {
        return win32_file_exists_short( path.cc );
    }
}
attr_internal b32 win32_directory_create_short( const char* path ) {
    b32 res = CreateDirectoryA( path, 0 ) != FALSE;
#if defined(CORE_ENABLE_LOGGING)
    if( !res ) {
        win32_log_error( GetLastError() );
    }
#endif
    return res;
}
attr_internal b32 win32_directory_create_long( const wchar_t* path ) {
    b32 res = CreateDirectoryW( path, 0 ) != FALSE;
#if defined(CORE_ENABLE_LOGGING)
    if( !res ) {
        win32_log_error( GetLastError() );
    }
#endif
    return res;
}
b32 platform_directory_create( Path path ) {
    if( path.len > MAX_PATH ) {
        usize buf_size = sizeof(wchar_t) * CORE_MAX_PATH_NAME;
        wchar_t* buf = memory_alloc( buf_size );
        if( !buf ) {
            core_error(
                "win32: directory_create: "
                "failed to allocate {usize,m} for long path name buffer!",
                buf_size );
            return false;
        }

        win32_long_path( path, buf );
        b32 res = win32_directory_create_long( buf );

        return res;
    } else {
        return win32_directory_create_short( path.cc );
    }
}
attr_internal b32 win32_directory_remove_short( const char* path ) {
    b32 res = RemoveDirectoryA( path ) != FALSE;
#if defined(CORE_ENABLE_LOGGING)
    if( !res ) {
        win32_log_error( GetLastError() );
    }
#endif
    return res;
}
attr_internal b32 win32_directory_remove_long( const wchar_t* path ) {
    b32 res = RemoveDirectoryW( path ) != FALSE;
#if defined(CORE_ENABLE_LOGGING)
    if( !res ) {
        win32_log_error( GetLastError() );
    }
#endif
    return res;
}
attr_internal b32 win32_directory_remove_recursive( Path path ) {
    AllocatorInterface heap = allocator_interface_from_heap();

    DirectoryWalk* walk = directory_walk_begin( path, &heap );
    if( !walk ) {
        core_error( "win32: failed to walk directory '{p}'!", path );
        return false;
    }

    b32  result  = true;
    Path current = path_empty();
    b32  is_dir  = false;
    while( directory_walk_next( walk, &current, &is_dir ) ) {
        if( is_dir ) {
            usize path_buf_size = path.len + current.len + 16;
            StringBuf buf;
            if( !string_buf_from_alloc( path_buf_size, &heap, &buf ) ) {
                core_error( "directory_remove: win32: failed to generate next path!" );
                result = false;
                goto win32_directory_remove_recursive_end;
            }

            string_buf_try_append( &buf, path );
            path_buf_try_push( &buf, current );

            if( !win32_directory_remove_recursive( buf.slice ) ) {
                result = false;
                goto win32_directory_remove_recursive_end;
            }

            string_buf_free( &buf, &heap );
        } else {
            if( !file_remove( current ) ) {
                result = false;
                goto win32_directory_remove_recursive_end;
            }
        }
    }

win32_directory_remove_recursive_end:
    directory_walk_end( walk, &heap );
    return result;
}
b32 platform_directory_remove( Path path, b32 recursive ) {
    if( recursive ) {
        return win32_directory_remove_recursive( path );
    } else {
        if( path.len > MAX_PATH ) {
            usize buf_size = sizeof(wchar_t) * CORE_MAX_PATH_NAME;
            wchar_t* buf = memory_alloc( buf_size );
            if( !buf ) {
                core_error(
                    "win32: directory_remove: "
                    "failed to allocate {usize,m} for long path name buffer!",
                    buf_size );
                return false;
            }

            win32_long_path( path, buf );
            b32 res = win32_directory_remove_long( buf );

            return res;
        } else {
            return win32_directory_remove_short( path.cc );
        }
    }
}
attr_internal b32 win32_directory_exists_short( const char* path ) {
    DWORD attr = GetFileAttributesA( path );
    return
        ( attr != INVALID_FILE_ATTRIBUTES ) &&
        ( attr & FILE_ATTRIBUTE_DIRECTORY ) != 0;
}
attr_internal b32 win32_directory_exists_long( const wchar_t* path ) {
    DWORD attr = GetFileAttributesW( path );
    return
        ( attr != INVALID_FILE_ATTRIBUTES ) &&
        ( attr & FILE_ATTRIBUTE_DIRECTORY ) != 0;
}
b32 platform_directory_exists( Path path ) {
    if( path.len > MAX_PATH ) {
        usize buf_size = sizeof(wchar_t) * CORE_MAX_PATH_NAME;
        wchar_t* buf = memory_alloc( buf_size );
        if( !buf ) {
            core_error(
                "win32: directory_exists: "
                "failed to allocate {usize,m} for long path name buffer!",
                buf_size );
            return false;
        }

        win32_long_path( path, buf );

        b32 res = win32_directory_exists_long( buf );

        memory_free( buf, buf_size );

        return res;
    } else {
        return win32_directory_exists_short( path.cc );
    }
}
attr_internal u32 win32_wide_str_len( const wchar_t* str ) {
    u32 res = 0;
    while( *str++ ) {
        res++;
    }
    return res;
}
attr_internal b32 win32_file_is_dot( const wchar_t* file, u32* out_len ) {
    u32 len  = win32_wide_str_len( file );
    *out_len = len;
    switch( file[0] ) {
        case L'.': {
            switch( len ) {
                case 1: return true;
                case 2: {
                    if( file[1] == L'.' ) {
                        return true;
                    }
                } break;
                default: break;
            }
        } break;
    }
    return false;
}
b32 platform_directory_is_empty( Path path, b32* out_found ) {
    usize    wpath_size = sizeof(wchar_t) * (path.len + 16);
    wchar_t* wpath      = memory_alloc( wpath_size );
    if( !wpath ) {
        *out_found = false;
        core_error(
            "win32: failed to allocate wide path buffer! {usize,m}", wpath_size );
        return true;
    }

    u32 len = 0;
    win32_path_to_wide( path.len, path.cc, wpath_size, wpath, &len );
    wpath[len++] = L'\\';
    wpath[len++] = L'*';
    wpath[len]   = 0;

    WIN32_FIND_DATAW fd;
    HANDLE h = FindFirstFileExW(
        wpath, FindExInfoBasic,
        &fd, FindExSearchNameMatch, 0, 0 );

    memory_free( wpath, wpath_size );

    if( h == INVALID_HANDLE_VALUE ) {
        *out_found = false;
        win32_log_error( GetLastError() );
        core_error( "win32: failed to find directory '{p}'!", path );
        return true;
    }
    *out_found = true;

    u32 path_len = 0;
    while( win32_file_is_dot( fd.cFileName, &path_len ) ) {
        if( !FindNextFileW( h, &fd ) ) {
            FindClose( h );
            return true;
        }
    }

    b32 result = FindNextFileW( h, &fd ) == false;
    FindClose( h );
    return result;
}
DirectoryWalk* platform_directory_walk_begin(
    Path path, struct AllocatorInterface* allocator
) {
    usize wide_cap      = path.len + 260;
    usize walk_size     = ( sizeof(wchar_t) * wide_cap ) + sizeof(DirectoryWalk);
    DirectoryWalk* walk = allocator->alloc( walk_size, 0, allocator->ctx );

    if( !walk ) {
        core_error( "failed to allocate directory walk! size: {usize,m}", walk_size );
        return NULL;
    }

    walk->path_cap = wide_cap;

    if( !win32_path_to_wide(
        path.len, path.cc,
        wide_cap, walk->wide_path,
        &walk->path_len
    ) ) {
        core_error( "win32: failed to convert path '{p}' to wide characters.", path );
        allocator->free( walk, walk_size, 0, allocator->ctx );
        return NULL;
    }

    walk->wide_path[walk->path_len++] = L'\\';
    walk->wide_path[walk->path_len++] = L'*';
    walk->wide_path[walk->path_len]   = 0;

    walk->h = FindFirstFileExW(
        walk->wide_path, FindExInfoBasic,
        &walk->fd, FindExSearchNameMatch, 0, 0 );
    if( walk->h == INVALID_HANDLE_VALUE ) {
        win32_log_error( GetLastError() );
        core_error( "win32: failed to open directory '{p}'!", path );
        allocator->free( walk, walk_size, 0, allocator->ctx );
        return NULL;
    }

    memory_zero( walk->wide_path, sizeof(wchar_t) * walk->path_cap );
    walk->path_len = 0;

    walk->is_first = true;
    return walk;
}
b32 platform_directory_walk_next(
    DirectoryWalk* walk, Path* out_path, b32* opt_out_is_directory
) {
    memory_zero( walk->wide_path, sizeof(wchar_t) * walk->path_cap );
    walk->path_len = 0;

    if( walk->is_first ) {
        walk->is_first = false;
    } else {
        if( !FindNextFileW( walk->h, &walk->fd ) ) {
            return false;
        }
    }

    u32 wide_len = 0;
    while( win32_file_is_dot( walk->fd.cFileName, &wide_len ) ) {
        if( !FindNextFileW( walk->h, &walk->fd ) ) {
            return false;
        }
    }

    walk->path_len = win32_wide_to_utf8(
        wide_len, walk->fd.cFileName,
        sizeof( wchar_t ) * walk->path_cap,
        (char*)walk->wide_path );

    *out_path = string_new( walk->path_len, (char*)walk->wide_path );
    if( opt_out_is_directory ) {
        *opt_out_is_directory =
            (walk->fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
    }

    return true;
}
void platform_directory_walk_end(
    DirectoryWalk* walk, struct AllocatorInterface* allocator
) {
    if( walk ) {
        if( walk->h ) {
            FindClose( walk->h );
        }

        allocator->free(
            walk, (sizeof(wchar_t) * walk->path_cap) + sizeof(*walk),
            0, allocator->ctx );
    }
}

attr_internal void internal_win32_get_cpu_name(void) {
    if( !global_win32.cpu_name.len ) {
        memory_zero( global_win32.cpu_name.c, sizeof(global_win32.cpu_name_buf) );

#if defined(CORE_ARCH_X86)
        int cpu_info[4] = {0};
        char* chunk = global_win32.cpu_name.c;

        __cpuid( cpu_info, 0x80000002 );
        memory_copy( chunk, cpu_info, sizeof(cpu_info) );
        chunk += sizeof(cpu_info);

        __cpuid( cpu_info, 0x80000003 );
        memory_copy( chunk, cpu_info, sizeof(cpu_info) );
        chunk += sizeof(cpu_info);

        __cpuid( cpu_info, 0x80000004 );
        memory_copy( chunk, cpu_info, sizeof(cpu_info) );

        chunk = global_win32.cpu_name.c;
        for( u32 i = 255; i-- > 0; ) {
            if( chunk[i] && chunk[i] != ' ' ) {
                global_win32.cpu_name.len = i + 1;
                break;
            }
        }
#endif /* Arch x86 */

    }
}

attr_internal void internal_win32_load_enum_display_devices(void) {
    if( EnumDisplayDevicesA ) {
        return;
    }

    if( !global_win32.user32 ) {
        global_win32.user32 = LoadLibraryA( "USER32.DLL" );
    }

    EnumDisplayDevicesA =
        (EnumDisplayDevicesAFN*)(void*)GetProcAddress(
            global_win32.user32, "EnumDisplayDevicesA" );
}

attr_internal void internal_win32_get_gpu_name(void) {
    if( global_win32.gpu_name.cc ) {
        return;
    }

    internal_win32_load_enum_display_devices();
    DISPLAY_DEVICE dd = {0};
    dd.cb = sizeof(dd);

    EnumDisplayDevicesA( NULL, 0, &dd, EDD_GET_DEVICE_INTERFACE_NAME );

    usize name_len          = cstr_len( dd.DeviceString );
    global_win32.gpu_name.c =
        HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, name_len + 1 );

    memory_copy( global_win32.gpu_name.c, dd.DeviceString, name_len );
    global_win32.gpu_name.len = name_len;
}

void platform_system_query_info( SystemInfo* out_info ) {
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

    internal_win32_get_cpu_name();
    out_info->cpu_name = global_win32.cpu_name;

    internal_win32_get_gpu_name();
    out_info->gpu_name = global_win32.gpu_name;

}

void* platform_library_open( const char* name ) {
    HMODULE module = LoadLibraryA( name );
    if( !module ) {
        win32_log_error( GetLastError() );
        core_error( "win32: failed to load library '{cc}'!", name );
        return NULL;
    }

    return module;
}
void* platform_library_get( const char* name ) {
    return (HMODULE)GetModuleHandleA( name );
}
void platform_library_close( void* lib ) {
    FreeLibrary( (HMODULE)lib );
}
void* platform_library_load( void* lib, const char* function ) {
    void* addr = (void*)GetProcAddress( lib, function );
    if( !addr ) {
        win32_log_error( GetLastError() );
        core_error( "win32: failed to load function '{cc}'!", function );
        return NULL;
    }
    return addr;
}

LPSTR* WINAPI CommandLineToArgvA( LPSTR lpCmdline, int* numargs ) {
    DWORD argc;
    LPSTR *argv;
    LPSTR s;
    LPSTR d;
    LPSTR cmdline;
    int qcount, bcount;

    if( !numargs || *lpCmdline == 0 ) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }

    /* --- First count the arguments */
    argc = 1;
    s = lpCmdline;
    /* The first argument, the executable path, follows special rules */
    if( *s == '"' ) {
        /* The executable path ends at the next quote, no matter what */
        s++;
        while( *s ) {
            if( *s++ == '"' ) {
                break;
            }
        }
    } else {
        /* The executable path ends at the next space, no matter what */
        while( *s && *s != ' ' && *s != '\t' ) {
            s++;
        }
    }
    /* skip to the first argument, if any */
    while( *s == ' ' || *s == '\t' ) {
        s++;
    }
    if( *s ) {
        argc++;
    }

    /* Analyze the remaining arguments */
    qcount = bcount=0;
    while( *s ) {
        if( ( *s == ' ' || *s == '\t' ) && qcount == 0 ) {
            /* skip to the next argument and count it if any */
            while( *s == ' ' || *s == '\t' ) {
                s++;
            }
            if( *s ) {
                argc++;
            }
            bcount = 0;
        } else if( *s == '\\' ) {
            /* '\', count them */
            bcount++;
            s++;
        } else if( *s == '"' ) {
            /* '"' */
            if( ( bcount & 1 ) == 0 ) {
                qcount++; /* unescaped '"' */
            }
            s++;
            bcount = 0;
            /* consecutive quotes, see comment in copying code below */
            while( *s == '"' ) {
                qcount++;
                s++;
            }
            qcount = qcount % 3;
            if( qcount == 2 ) {
                qcount=0;
            }
        } else {
            /* a regular character */
            bcount = 0;
            s++;
        }
    }

    /* Allocate in a single lump, the string array, and the strings that go
     * with it. This way the caller can make a single LocalFree() call to free
     * both, as per MSDN.
     */

    usize lpCmdline_len = cstr_len( lpCmdline );
    argv = (LPSTR*)LocalAlloc(
        LMEM_FIXED,
        ( argc + 1 ) *
        sizeof(LPSTR) +
        ( lpCmdline_len + 1 ) *
        sizeof(char)
    );
    if( !argv ) {
        return NULL;
    }
    cmdline = (LPSTR)( argv + argc + 1 );
    memory_copy( cmdline, lpCmdline, lpCmdline_len + 1 );

    /* --- Then split and copy the arguments */
    argv[0] = d = cmdline;
    argc = 1;
    /* The first argument, the executable path, follows special rules */
    if( *d == '"' ) {
        /* The executable path ends at the next quote, no matter what */
        s = d + 1;
        while( *s ) {
            if( *s == '"' ) {
                s++;
                break;
            }
            *d++ = *s++;
        }
    } else {
        /* The executable path ends at the next space, no matter what */
        while( *d && *d != ' ' && *d != '\t' ) {
            d++;
        }
        s = d;
        if( *s ) {
            s++;
        }
    }
    /* close the executable path */
    *d++ = 0;
    /* skip to the first argument and initialize it if any */
    while( *s == ' ' || *s == '\t' ) {
        s++;
    }
    if( !*s ) {
        /* There are no parameters so we are all done */
        argv[argc] = NULL;
        *numargs = argc;
        return argv;
    }

    /* Split and copy the remaining arguments */
    argv[argc++] = d;
    qcount = bcount = 0;
    while( *s ) {
        if( ( *s == ' ' || *s == '\t' ) && qcount == 0 ) {
            /* close the argument */
            *d++ = 0;
            bcount = 0;

            /* skip to the next one and initialize it if any */
            do {
                s++;
            } while( *s == ' ' || *s == '\t' );
            if (*s) {
                argv[argc++] = d;
            }
        } else if( *s == '\\' ) {
            *d++ = *s++;
            bcount++;
        } else if( *s == '"' ) {
            if( ( bcount & 1 ) == 0 ) {
                /* Preceded by an even number of '\', this is half that
                 * number of '\', plus a quote which we erase.
                 */
                d -= bcount/2;
                qcount++;
            } else {
                /* Preceded by an odd number of '\', this is half that
                 * number of '\' followed by a '"'
                 */
                d = d - (bcount / 2) - 1;
                *d++ = '"';
            }
            s++;
            bcount = 0;
            /* Now count the number of consecutive quotes. Note that qcount
             * already takes into account the opening quote if any, as well as
             * the quote that lead us here.
             */
            while( *s == '"' ) {
                if( ++qcount == 3 ) {
                    *d++ = '"';
                    qcount = 0;
                }
                s++;
            }
            if( qcount == 2 ) {
                qcount = 0;
            }
        } else {
            /* a regular character */
            *d++ = *s++;
            bcount = 0;
        }
    }
    *d = '\0';
    argv[argc] = NULL;
    *numargs = argc;

    return argv;
}


#endif
