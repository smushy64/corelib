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

Path internal_path_advance_by( Path path, usize count );
Path internal_path_advance( Path path );
b32 internal_path_find_sep_rev( Path path, usize* out_index );
b32 internal_path_find_sep( Path path, usize* out_index );
b32 internal_path_is_sep( PathCharacter c );
b32 internal_path_find_rev( Path path, PathCharacter pc, usize* out_index );
b32 internal_path_find( Path path, PathCharacter pc, usize* out_index );

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

attr_global struct Win32Global {
    HANDLE user32;

    volatile u32 thread_id;

    String cpu_name;
    String gpu_name;

    PathBuf buf_cwd;

    DWORD    home_len;
    wchar_t* home;

    PipeRead  in;
    PipeWrite out, err;

    LARGE_INTEGER qpf;

    DWORD tls_index;

    char cpu_name_buf[255];
} global_win32;

struct DirectoryWalk {
    HANDLE           h;
    WIN32_FIND_DATAW fd;
    b32              is_first;
};

b32 win32_init(void) {
    SetConsoleOutputCP( CP_UTF8 );
    SetConsoleCP( CP_UTF8 );

    global_win32.buf_cwd.len = GetCurrentDirectoryW( 0, 0 ) - 1;
    global_win32.buf_cwd.cap = global_win32.buf_cwd.len + 16;
    global_win32.buf_cwd.raw =
        LocalAlloc( 0, sizeof(wchar_t) * global_win32.buf_cwd.cap );
    memory_zero(
        global_win32.buf_cwd.raw, sizeof(wchar_t) * global_win32.buf_cwd.cap );
    if( !global_win32.buf_cwd.raw ) {
        return false;
    }

    GetCurrentDirectoryW( global_win32.buf_cwd.len + 1, global_win32.buf_cwd.raw );

    DWORD drive_cap = GetEnvironmentVariableW( L"HOMEDRIVE", 0, 0 );
    DWORD hpath_cap = GetEnvironmentVariableW( L"HOMEPATH", 0, 0 );
    DWORD required_memory = drive_cap + hpath_cap;
    wchar_t* paths_buffer = LocalAlloc( 0, sizeof(wchar_t) * required_memory );
    if( !paths_buffer ) {
        LocalFree( global_win32.buf_cwd.raw );
        return false;
    }

    global_win32.home = paths_buffer;
    GetEnvironmentVariableW( L"HOMEDRIVE", global_win32.home, drive_cap );
    GetEnvironmentVariableW( L"HOMEPATH",
        global_win32.home + (drive_cap - 1), hpath_cap );
    global_win32.home_len = (drive_cap + hpath_cap) - 2;

    global_win32.in.fd.opaque  = GetStdHandle( STD_INPUT_HANDLE );
    global_win32.out.fd.opaque = GetStdHandle( STD_OUTPUT_HANDLE );
    global_win32.err.fd.opaque = GetStdHandle( STD_ERROR_HANDLE );

    global_win32.cpu_name.c = global_win32.cpu_name_buf;
    global_win32.thread_id  = 1;

    global_win32.tls_index = TlsAlloc();
    TlsSetValue(global_win32.tls_index, 0);

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

    LocalFree( global_win32.buf_cwd.raw );
    LocalFree( global_win32.home );

    TlsFree(global_win32.tls_index);

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

    usize thread_id = atomic_increment32( &global_win32.thread_id );

    TlsSetValue( global_win32.tls_index, (void*)thread_id );

    read_write_barrier();

    volatile int ret = thread.main( thread_id, thread.params );

    ExitThread( ret );
}
u32 platform_thread_query_id(void) {
    usize value = (usize)TlsGetValue( global_win32.tls_index );
    return value;
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

attr_unused
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
usize platform_path_stream_canonicalize(
    StreamBytesFN* stream, void* target, Path path
) {
    enum {
        WIN32_PATH_REL,
        WIN32_PATH_HOME,
        WIN32_PATH_ABS,
    } type = WIN32_PATH_REL;
    usize result = 0;

    path_buf_create_from_stack( buffer, sizeof(wchar_t) * kibibytes(4) );

    Path rem = path; {
        Path prefix = path_text("\\\\?\\");
        b32 prefix_required = true;
        if( rem.len >= prefix.len ) {
            Path potential_prefix = path_new( prefix.len, rem.const_raw );
            if( path_cmp( potential_prefix, prefix ) ) {
                prefix_required = false;
            }
        }
        if( prefix_required ) {
            memory_copy(
                buffer.raw, prefix.const_raw,
                sizeof(wchar_t) * prefix.len );
            buffer.len += prefix.len;
        } else {
            rem = internal_path_advance_by( rem, prefix.len + 1 );
        }
    }

    if( path_is_absolute( path ) ) {
        type = WIN32_PATH_ABS;
    } else if( rem.len && rem.const_raw[0] == '~' ) {
        type = WIN32_PATH_HOME;
    }

    switch( type ) {
        case WIN32_PATH_REL: {
            path_buf_try_push( &buffer, global_win32.buf_cwd.slice );
        } break;
        case WIN32_PATH_HOME: {
            path_buf_try_push(
                &buffer, path_new( global_win32.home_len, global_win32.home ) );
            rem = internal_path_advance_by( rem, 2 );
        } break;
        case WIN32_PATH_ABS: break;
    }

    usize min = path_text("\\\\?\\A:\\").len;

    while( !path_is_empty( rem ) ) {
        Path chunk = rem;
        usize sep = 0;
        if( internal_path_find_sep( rem, &sep ) ) {
            if( !sep ) {
                rem = internal_path_advance( rem );
                continue;
            }
            chunk.len = sep;
        }

        if( chunk.len < 3 ) {
            if( path_cmp( chunk, path_text(".") )) {
                rem = internal_path_advance_by( rem, chunk.len + 1 );
                continue;
            }
            if( path_cmp( chunk, path_text(".."))) {
                for( usize i = buffer.len; i-- > 0; ) {
                    if( buffer.const_raw[i] == L'\\' ) {
                        buffer.len = i;
                        break;
                    }
                }

                if( buffer.len < min ) {
                    buffer.len = min;
                }

                buffer.raw[buffer.len] = 0;
                rem = internal_path_advance_by( rem, chunk.len + 1 );
                continue;
            }
        }

        path_buf_try_push( &buffer, chunk );
        rem = internal_path_advance_by( rem, chunk.len + 1 );
    }
    result += stream( target, sizeof(wchar_t) * buffer.len, buffer.const_raw );

    return result;
}
usize platform_path_stream_canonicalize_utf8(
    StreamBytesFN* stream, void* target, Path path
) {
    enum {
        WIN32_PATH_REL,
        WIN32_PATH_HOME,
        WIN32_PATH_ABS,
    } type = WIN32_PATH_REL;
    usize result = 0;

    string_buf_create_from_stack( buffer, kibibytes(4) );

    Path rem = path;

    if( path_is_absolute( path ) ) {
        type = WIN32_PATH_ABS;
    } else if( rem.len && rem.const_raw[0] == '~' ) {
        type = WIN32_PATH_HOME;
    }

    switch( type ) {
        case WIN32_PATH_REL: {
            buffer.len += win32_wide_to_utf8(
                global_win32.buf_cwd.len, global_win32.buf_cwd.const_raw,
                string_buf_remaining(&buffer), buffer.c + buffer.len );
        } break;
        case WIN32_PATH_HOME: {
            buffer.len += win32_wide_to_utf8(
                global_win32.home_len, global_win32.home,
                string_buf_remaining(&buffer), buffer.c + buffer.len );
            rem = internal_path_advance_by( rem, 2 );
        } break;
        case WIN32_PATH_ABS: break;
    }

    usize min = string_text("\\\\?\\A:").len;

    while( !path_is_empty( rem ) ) {
        Path chunk = rem;
        usize sep = 0;
        if( internal_path_find_sep( rem, &sep ) ) {
            if( !sep ) {
                rem = internal_path_advance( rem );
                continue;
            }
            chunk.len = sep;
        }

        if( chunk.len < 3 ) {
            if( path_cmp( chunk, path_text(".") )) {
                rem = internal_path_advance_by( rem, chunk.len + 1 );
                continue;
            }
            if( path_cmp( chunk, path_text(".."))) {
                for( usize i = buffer.len; i-- > 0; ) {
                    if( buffer.cc[i] == '\\' ) {
                        buffer.len = i;
                        break;
                    }
                }

                if( buffer.len < min ) {
                    buffer.len = min;
                }

                buffer.c[buffer.len] = 0;
                rem = internal_path_advance_by( rem, chunk.len + 1 );
                continue;
            }
        }

        string_buf_try_push( &buffer, PATH_SEPARATOR );

        buffer.len += win32_wide_to_utf8(
            chunk.len, chunk.const_raw,
            string_buf_remaining( &buffer ), buffer.c + buffer.len );
        rem = internal_path_advance_by( rem, chunk.len + 1 );
    }
    result += stream( target, buffer.len, buffer.cc );

    return result;

}
PathBuf win32_make_path( Path path, b32 allocate ) {
    Path prefix = path_text("\\\\?\\");

    b32 prefix_required = false;
    b32 alloc_required  = false;
    if( path.const_raw[0] == '~' ) {
        prefix_required = alloc_required = true;
    } else {
        if( !path_is_null_terminated( path ) ) {
            alloc_required = true;

            if( path.len >= prefix.len ) {
                Path potential_prefix = path;
                potential_prefix.len  = prefix.len;
                if( !path_cmp( prefix, potential_prefix ) ) {
                    prefix_required = true;
                }
            } else {
                prefix_required = true;
            }
        } else if( path.len >= MAX_PATH ) {
            Path potential_prefix = path;
            potential_prefix.len  = prefix.len;
            if( !path_cmp( prefix, potential_prefix ) ) {
                alloc_required  = true;
                prefix_required = true;
            }
        }
    }

    usize memory_requirement = prefix_required ? prefix.len : 0;
    if( global_win32.buf_cwd.len > global_win32.home_len ) {
        memory_requirement += global_win32.buf_cwd.len;
    } else {
        memory_requirement += global_win32.home_len;
    }
    memory_requirement += path.len + 1;
    memory_requirement *= sizeof(wchar_t);

    PathBuf buf = path_buf_empty();
    buf.cap = memory_requirement;

    if( !allocate ) {
        return buf;
    }

    if( !alloc_required ) {
        PathBuf result = {0};
        result.slice   = path;
        return result;
    }

    void* ptr = HeapAlloc(
        GetProcessHeap(), HEAP_ZERO_MEMORY, memory_requirement );
    if( !ptr ) {
        core_error( "win32: failed to allocate long path buffer!" );
        return path_buf_empty();
    }
    buf.raw = ptr;

    platform_path_stream_canonicalize( path_buf_stream, &buf, path );
    return buf;
}
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

    PathBuf p = win32_make_path( path, true );
    if( path_buf_is_empty( &p ) ) {
        return false;
    }

    HANDLE handle = CreateFileW(
        p.const_raw, dwDesiredAccess, dwShareMode, 0,
        dwCreationDisposition, dwFlagsAndAttributes, 0 );

    DWORD last_error = 0;
    if( !handle || handle == INVALID_HANDLE_VALUE ) {
        last_error = GetLastError();
        core_error( "win32: failed to open '{p}'", p.slice );
    }

    if( p.cap ) {
        HeapFree( GetProcessHeap(), 0, p.raw );
    }

    if( !handle || handle == INVALID_HANDLE_VALUE ) {
        win32_log_error( last_error );
        return false;
    }

    out_fd->opaque = handle;

    if( append ) {
        fd_seek( out_fd, FSEEK_END, 0 );
    }

    return true;
}
b32 platform_file_copy( Path dst, Path src, b32 create_dst ) {
    PathBuf d = win32_make_path( dst, true );
    if( path_buf_is_empty( &d ) ) {
        return false;
    }
    PathBuf s = win32_make_path( src, true );
    if( path_buf_is_empty( &s ) ) {
        if( d.cap ) {
            HeapFree( GetProcessHeap(), 0, d.raw );
        }
        return false;
    }

    b32 res = CopyFileW( d.const_raw, s.const_raw, create_dst );
    if( !res ) {
        win32_log_error( GetLastError() );
    }

    if( d.cap ) {
        HeapFree( GetProcessHeap(), 0, d.raw );
    }
    if( s.cap ) {
        HeapFree( GetProcessHeap(), 0, s.raw );
    }

    return res;
}
b32 platform_file_move( Path dst, Path src, b32 create_dst ) {
    PathBuf d = win32_make_path( dst, true );
    if( path_buf_is_empty( &d ) ) {
        return false;
    }

    if( create_dst ) {
        if( platform_file_exists( d.slice ) ) {
            if( d.cap ) {
                HeapFree( GetProcessHeap(), 0, d.raw );
            }
            core_error(
                "win32: file move: attempted to move "
                "{p} to {p} when destination already exists!", src, dst );
            return false;
        }
    }

    PathBuf s = win32_make_path( src, true );
    if( path_buf_is_empty( &s ) ) {
        if( d.cap ) {
            HeapFree( GetProcessHeap(), 0, d.raw );
        }
        return false;
    }

    b32 res = MoveFileW( d.const_raw, s.const_raw );
    if( !res ) {
        win32_log_error( GetLastError() );
    }

    if( d.cap ) {
        HeapFree( GetProcessHeap(), 0, d.raw );
    }
    if( s.cap ) {
        HeapFree( GetProcessHeap(), 0, s.raw );
    }

    return res;
}
b32 platform_file_remove( Path path ) {
    PathBuf p = win32_make_path( path, true );
    if( path_buf_is_empty( &p ) ) {
        return false;
    }
    b32 res = DeleteFileW( p.const_raw ) != FALSE;
    if( p.cap ) {
        HeapFree( GetProcessHeap(), 0, p.raw );
    }
    return res;
}
b32 platform_file_exists( Path path ) {
    PathBuf p = win32_make_path( path, true );
    if( path_buf_is_empty( &p ) ) {
        return false;
    }
    DWORD attr = GetFileAttributesW( p.const_raw );
    if( p.cap ) {
        HeapFree( GetProcessHeap(), 0, p.raw );
    }
    return (attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}
Path platform_directory_query_cwd(void) {
    return global_win32.buf_cwd.slice;
}
b32 platform_directory_set_cwd( Path path ) {
    usize canonical_len = path_stream_canonicalize( path_buf_stream, NULL, path );
    PathBuf new_path_buf = {0};
    new_path_buf.cap = canonical_len + 1;
    new_path_buf.raw = LocalAlloc( 0, sizeof(wchar_t) * (canonical_len + 1));
    memory_zero( new_path_buf.raw, new_path_buf.cap );

    if( !new_path_buf.raw ) {
        core_error( "win32: set_cwd: failed to allocate new path buffer!" );
        return false;
    }
    path_stream_canonicalize( path_buf_stream, &new_path_buf, path );

    usize advance_count = sizeof("\\\\?");
    memory_move(
        new_path_buf.raw, new_path_buf.raw + advance_count,
        new_path_buf.cap - advance_count );
    new_path_buf.len -= advance_count;
    new_path_buf.raw[new_path_buf.len] = 0;

    if( !SetCurrentDirectoryW( new_path_buf.const_raw ) ) {
        win32_log_error( GetLastError() );
        LocalFree( new_path_buf.raw );
        return false;
    }

    void* old_path_buf = global_win32.buf_cwd.raw;
    global_win32.buf_cwd = new_path_buf;

    LocalFree( old_path_buf );
    return true;
}
b32 platform_directory_create( Path path ) {
    PathBuf p = win32_make_path( path, true );
    if( path_buf_is_empty( &p ) ) {
        return false;
    }
    b32 res = CreateDirectoryW( p.const_raw, 0 ) != FALSE;
    if( p.cap ) {
        HeapFree( GetProcessHeap(), 0, p.raw );
    }
    return res;
}
b32 platform_directory_remove( Path path ) {
    PathBuf p = win32_make_path( path, true );
    if( path_buf_is_empty( &p ) ) {
        return false;
    }
    b32 result = RemoveDirectoryW( p.const_raw ) != FALSE;
    if( p.cap ) {
        HeapFree( GetProcessHeap(), 0, p.raw );
    }
    return result;
}
b32 platform_directory_exists( Path path ) {
    PathBuf p = win32_make_path( path, true );
    if( path_buf_is_empty( &p ) ) {
        return false;
    }
    DWORD attr = GetFileAttributesW( p.const_raw );
    if( p.cap ) {
        HeapFree( GetProcessHeap(), 0, p.raw );
    }
    return
        (attr != INVALID_FILE_ATTRIBUTES) &&
        (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}
DirectoryWalk* platform_directory_walk_begin(
    Path path, struct AllocatorInterface* allocator
) {
    DirectoryWalk* walk = allocator->alloc( sizeof(*walk), 0, allocator->ctx );
    if( !walk ) {
        core_error( "failed to allocate directory walk!");
        return NULL;
    }
    PathBuf buf = path_buf_empty(); {
        usize memory_requirement = win32_make_path( path, false ).cap;
        if( !path_buf_from_alloc( memory_requirement + 16, allocator, &buf ) ) {
            allocator->free( walk, sizeof(*walk), 0, allocator->ctx );
            core_error( "failed to allocate directory walk path buffer!" );
            return NULL;
        }

        path_stream_canonicalize( path_buf_stream, &buf, path );
    }
    path_buf_try_push( &buf, path_text("*") );

    walk->h = FindFirstFileExW(
        buf.const_raw, FindExInfoBasic,
        &walk->fd, FindExSearchNameMatch, 0, 0 );
    path_buf_free( &buf, allocator );

    if( walk->h == INVALID_HANDLE_VALUE ) {
        win32_log_error( GetLastError() );
        core_error( "win32: failed to open directory {p}", path );
        allocator->free( walk, sizeof(*walk), 0, allocator->ctx );
        return NULL;
    }

    walk->is_first = true;
    return walk;
}
b32 platform_directory_walk_next(
    DirectoryWalk* walk, Path* out_path, b32* opt_out_is_directory
) {

    if( walk->is_first ) {
        walk->is_first = false;
    } else {
        if( !FindNextFileW( walk->h, &walk->fd ) ) {
            return false;
        }
    }

    Path path = path_from_raw( 0, walk->fd.cFileName );
    for(;;) {
        if(
            path_cmp( path, path_text("."))  ||
            path_cmp( path, path_text("..")) ||
            path_cmp( path, path_text(".git"))
        ) {
            if( !FindNextFileW( walk->h, &walk->fd ) ) {
                return false;
            }
            path = path_from_raw( 0, walk->fd.cFileName );
            continue;
        }
        break;
    }

    if( opt_out_is_directory ) {
        *opt_out_is_directory =
            (walk->fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
    }
    *out_path = path;

    return true;
}
void platform_directory_walk_end(
    DirectoryWalk* walk, struct AllocatorInterface* allocator
) {
    if( walk ) {
        if( walk->h ) {
            FindClose( walk->h );
        }
        allocator->free( walk, sizeof(*walk), 0, allocator->ctx );
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

#endif
