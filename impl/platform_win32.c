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
#include "core/time.h"
#include "core/thread.h"
#include "core/system.h"

#include "core/string.h"
#include "core/fs.h"

#include "core/internal/platform/path.h"
#include "core/internal/platform/time.h"
#include "core/internal/platform/memory.h"
#include "core/internal/platform/thread.h"
#include "core/internal/platform/fs.h"
#include "core/internal/platform/misc.h"

#include "core/internal/logging.h"

#define NOMINMAX
#include <windows.h>
#include <intrin.h>

#if defined(CORE_COMPILER_MSVC)
    int _fltused;
#endif

struct Win32Tls {
    u32  thread_id;
    char text_buffer[CORE_PATH_NAME_LEN * 2];
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

struct Win32Platform {
    DWORD         tls;
    atomic32      running_thread_id;
    LARGE_INTEGER qpf;

    PipeRead  stdin;
    PipeWrite stdout;
    PipeWrite stderr;

    char cpu_name[255];
    char gpu_name[255];

    struct Win32Tls main_tls_storage;

    struct {
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
wchar_t* win32_make_path( _PathPOD path );
attr_internal
void win32_path_buf_push_chunk(
    usize buffer_cap, usize* buffer_len, wchar_t* buffer, _PathPOD chunk );

attr_internal
void win32_log_error( DWORD error_code );
attr_unused attr_internal
usize win32_get_error_message(
    usize buffer_size, char* buffer, DWORD error_code );

attr_internal
TimePosix win32_filetime_to_time_posix( FILETIME ft );

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

    global_win32->stdin.fd.opaque  = GetStdHandle( STD_INPUT_HANDLE );
    global_win32->stdout.fd.opaque = GetStdHandle( STD_OUTPUT_HANDLE );
    global_win32->stderr.fd.opaque = GetStdHandle( STD_ERROR_HANDLE );

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
    wchar_t* path = win32_make_path( in_path );
    return DeleteFileW( path ) != FALSE;
}
b32 platform_file_query_info_by_path(
    struct _StringPOD in_path, struct FileInfo* out_info
) {
    wchar_t* path = win32_make_path( in_path );
}
enum FileType platform_file_query_type_by_path( struct _StringPOD in_path );
TimePosix platform_file_query_time_create_by_path( struct _StringPOD in_path );
TimePosix platform_file_query_time_modify_by_path( struct _StringPOD in_path );

b32 platform_file_open( struct _StringPOD in_path, enum FileOpenFlags flags, struct FD* out_fd );
void platform_file_close( struct FD* fd );
b32 platform_file_query_info( struct FD* fd, struct FileInfo* out_info );
enum FileType platform_file_query_type( struct FD* fd );
TimePosix platform_file_query_time_create( struct FD* fd );
TimePosix platform_file_query_time_modify( struct FD* fd );
usize platform_file_query_size( struct FD* fd );
usize platform_file_query_offset( struct FD* fd );
void platform_file_truncate( struct FD* fd );
usize platform_file_seek( struct FD* fd, enum FileSeek type, isize seek );
b32 platform_file_write( struct FD* fd, usize bytes, const void* buf, usize* out_write );
b32 platform_file_read( struct FD* fd, usize bytes, void* buf, usize* out_read );

b32 platform_directory_create( struct _StringPOD in_path );
b32 platform_directory_remove( struct _StringPOD in_path, b32 recursive );
b32 platform_directory_walk(
    struct _StringPOD in_path,
    enum DirectoryWalkControl (callback)( const struct DirectoryWalkInfo* info, void* params ),
    void* params );
struct _StringPOD platform_directory_current_query(void);
b32 platform_directory_current_set( struct _StringPOD in_path );

struct PipeRead* platform_pipe_stdin(void) {
    return &global_win32->stdin;
}
struct PipeWrite* platform_pipe_stdout(void) {
    return &global_win32->stdout;
}
struct PipeWrite* platform_pipe_stderr(void) {
    return &global_win32->stderr;
}

b32 platform_pipe_open( struct PipeRead* out_read, struct PipeWrite* out_write ) {
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
void platform_pipe_close( const void* pipe ) {
    struct PipeRead* p = (struct PipeRead*)pipe;
    CloseHandle( p->fd.opaque );
}
b32 platform_pipe_write(
    struct PipeWrite* pipe, usize bytes, const void* buf, usize* out_write
) {
    return platform_file_write( &pipe->fd, bytes, buf, out_write );
}
b32 platform_pipe_read( struct PipeRead* pipe, usize bytes, void* buf, usize* out_read );

usize platform_path_chunk_count( _PathPOD path ) {
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
_PathPOD platform_path_clip_chunk( _PathPOD path ) {
    if( !path.len ) {
        return path;
    }
    _PathPOD result = path;

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
_PathPOD platform_path_clip_chunk_last( _PathPOD path ) {
    if( !path.len ) {
        return path;
    }

    _PathPOD result = path;
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
_PathPOD platform_path_advance_chunk( _PathPOD path ) {
    if( !path.len ) {
        return path;
    }
    _PathPOD first_chunk = path_clip_chunk( path );
    if( first_chunk.len ) {
        return string_advance_by( path, first_chunk.len + 1 );
    } else {
        return path_empty();
    }
}
_PathPOD platform_path_pop_chunk( _PathPOD path ) {
    _PathPOD last = path_clip_chunk_last( path );
    if( last.len ) {
        return string_trim( path, last.len + 1 );
    } else {
        return path;
    }
}
b32 platform_path_is_absolute( _PathPOD path ) {
    return
        (path.len >= 3)                           &&
        (ascii_is_alphabetic( path.cbuf[0] ))     &&
        (path.cbuf[1] == ':')                     &&
        (ascii_is_path_separator( path.cbuf[2] ));
}
b32 platform_path_parent( _PathPOD path, _PathPOD* out_parent ) {
    _PathPOD parent = path_pop_chunk( path );
    if( !parent.len ) {
        return false;
    }
    *out_parent = path_clip_chunk_last( parent );
    return true;
}
b32 platform_path_file_name( _PathPOD path, _PathPOD* out_file_name ) {
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
    StreamBytesFN* stream, void* target, _PathPOD path
) {
    return path_stream_set_windows_separators( stream, target, path );
}
void platform_path_set_native_separators( _PathPOD path ) {
    path_set_windows_separators( path );
}
attr_internal
void win32_canonicalize( _PathBufPOD* buf, _PathPOD path ) {
    enum {
        WIN32_PATH_REL,
        WIN32_PATH_HOME,
        WIN32_PATH_ABS,
    } type = WIN32_PATH_REL;

    Path rem = path;

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
                buf, path_new( global_win32->home.len, global_win32->home.buf ) );
            rem = string_advance_by( rem, 2 );
        } break;
        case WIN32_PATH_ABS: break;
    }

    usize min = path_text("A:\\").len;

    while( !path_is_empty( rem ) ) {
        Path chunk = rem;
        usize sep = 0;
        if( string_find_set( rem, string_text("/\\"), &sep ) ) {
            if( !sep ) {
                rem = string_advance( rem );
                continue;
            }
            chunk.len = sep;
        }

        if( chunk.len < 3 ) {
            if( path_cmp( chunk, path_text(".") )) {
                rem = string_advance_by( rem, chunk.len + 1 );
                continue;
            }
            if( path_cmp( chunk, path_text(".."))) {
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
    StreamBytesFN* stream, void* target, _PathPOD path
) {
    _PathBufPOD buffer = path_buf_new( CORE_PATH_NAME_LEN * 2, win32_get_local_buffer() );
    win32_canonicalize( &buffer, path );
    return stream( target, buffer.len, buffer.buf );
}
b32 platform_path_buf_try_push_chunk( _PathBufPOD* buf, _PathPOD chunk ) {
    if( !chunk.len ) {
        return true;
    }

    b32 chunk_has_separator = ascii_is_path_separator( string_first_unchecked( chunk ) );
    if( chunk_has_separator && chunk.len == 1 ) {
        return true;
    }

    b32 buf_has_separator = false;
    if( !path_buf_is_empty( *buf ) ) {
        buf_has_separator = ascii_is_path_separator( string_last_unchecked( buf->slice ) );
    }

    if(
        path_buf_remaining( *buf ) <
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
b32 platform_path_buf_try_set_extension( _PathBufPOD* buf, _PathPOD extension ) {
    if( !extension.len ) {
        return true;
    }

    _PathPOD exisiting_extension = {};
    if( path_extension( buf->slice, &exisiting_extension ) ) {
        buf->len -= exisiting_extension.len;
    }

    b32 has_dot = string_first_unchecked( extension ) == '.';

    if( path_buf_remaining( *buf ) < (extension.len + !has_dot) ) {
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

void* platform_library_open( const char* name ) {
    return (void*)LoadLibraryA( name );
}
void* platform_library_get( const char* name ) {
    return (void*)GetModuleHandleA( name );
}
void platform_library_close( void* lib ) {
    FreeLibrary( (HMODULE)lib );
}
void* platform_library_load( void* lib, const char* function ) {
    void* proc = (void*)GetProcAddress( lib, function );
    if( !proc ) {
        win32_log_error( GetLastError() );
        core_error( "WIN32: failed to load function '{cc}'!", function );
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

attr_internal
char* win32_get_local_buffer(void) {
    struct Win32Tls* tls = (struct Win32Tls*)TlsGetValue( global_win32->tls );
    if( !tls ) {
        return NULL;
    }
    return tls->text_buffer;
}
attr_internal
void win32_canonicalize_ucs2( usize buffer_size, wchar_t* buffer, _PathPOD path ) {
    enum {
        WIN32_PATH_REL,
        WIN32_PATH_HOME,
        WIN32_PATH_ABS,
    } type = WIN32_PATH_REL;

    usize buffer_len = 0;

    _PathPOD rem = path; {
        _PathPOD prefix = path_text("\\\\?\\");
        b32 prefix_required = true;
        if( rem.len >= prefix.len ) {
            _PathPOD potential_prefix = path_new( prefix.len, rem.cbuf );
            if( path_cmp( potential_prefix, prefix ) ) {
                prefix_required = false;
            }
        }
        if( prefix_required ) {
            memory_copy(
                buffer, L"\\\\?\\",
                sizeof(wchar_t) * prefix.len );
            buffer_len += prefix.len;
        } else {
            rem = string_advance_by( rem, prefix.len + 1 );
        }
    }

    if( path_is_absolute( path ) ) {
        type = WIN32_PATH_ABS;
    } else if( rem.len && rem.cbuf[0] == '~' ) {
        type = WIN32_PATH_HOME;
    }

    switch( type ) {
        case WIN32_PATH_REL: {
            _PathPOD cwd = directory_current_query();
            buffer_len  += MultiByteToWideChar(
                CP_UTF8, 0, cwd.buf, cwd.len, buffer + buffer_len, buffer_size - buffer_len );
        } break;
        case WIN32_PATH_HOME: {
            _PathPOD home = path_new( global_win32->home.len, global_win32->home.buf );
            buffer_len   += MultiByteToWideChar(
                CP_UTF8, 0, home.buf, home.len, buffer + buffer_len, buffer_size - buffer_len );
            rem = string_advance_by( rem, 2 );
        } break;
        case WIN32_PATH_ABS: break;
    }

    usize min = path_text("\\\\?\\A:\\").len;

    while( !path_is_empty( rem ) ) {
        Path chunk = rem;
        usize sep = 0;
        if( string_find_set( rem, string_text("/\\"), &sep ) ) {
            if( !sep ) {
                rem = string_advance( rem );
                continue;
            }
            chunk.len = sep;
        }

        if( chunk.len < 3 ) {
            if( path_cmp( chunk, path_text(".") )) {
                rem = string_advance_by( rem, chunk.len + 1 );
                continue;
            }
            if( path_cmp( chunk, path_text(".."))) {
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
}
attr_internal
void win32_path_buf_push_chunk(
    usize buffer_cap, usize* buffer_len, wchar_t* buffer, _PathPOD chunk
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
wchar_t* win32_make_path( _PathPOD path ) {
    wchar_t* buffer = (wchar_t*)win32_get_local_buffer();
    win32_canonicalize_ucs2( CORE_PATH_NAME_LEN, buffer, path );
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



#if 0

attr_internal u32 win32_utf8_to_wide(
    u32 utf8_len, const char* utf8, u32 wide_cap, wchar_t* wide_buf );
attr_internal u32 win32_wide_to_utf8(
    u32 wide_len, const wchar_t* wide, u32 utf8_cap, char* utf8_buf );

attr_global struct Win32Global {
    volatile u32 thread_id;

    DWORD    home_len;
    wchar_t* home;

    PipeRead  in;
    PipeWrite out, err;

    LARGE_INTEGER qpf;
} global_win32;

b32 win32_init(void) {
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

    global_win32.thread_id  = 1;

    return true;
}

#if 0
Path internal_path_advance_by( Path path, usize count );
Path internal_path_advance( Path path );
b32 internal_path_find_sep_rev( Path path, usize* out_index );
b32 internal_path_find_sep( Path path, usize* out_index );
b32 internal_path_is_sep( PathCharacter c );
b32 internal_path_find_rev( Path path, PathCharacter pc, usize* out_index );
b32 internal_path_find( Path path, PathCharacter pc, usize* out_index );


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
void win32_canonicalize( PathBuf* buf, Path path ) {
    enum {
        WIN32_PATH_REL,
        WIN32_PATH_HOME,
        WIN32_PATH_ABS,
    } type = WIN32_PATH_REL;

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
                buf->raw, prefix.const_raw,
                sizeof(wchar_t) * prefix.len );
            buf->len += prefix.len;
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
            path_buf_try_push( buf, global_win32.buf_cwd.slice );
        } break;
        case WIN32_PATH_HOME: {
            path_buf_try_push(
                buf, path_new( global_win32.home_len, global_win32.home ) );
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
                for( usize i = buf->len; i-- > 0; ) {
                    if( buf->const_raw[i] == L'\\' ) {
                        buf->len = i;
                        break;
                    }
                }

                if( buf->len < min ) {
                    buf->len = min;
                }

                buf->raw[buf->len] = 0;
                rem = internal_path_advance_by( rem, chunk.len + 1 );
                continue;
            }
        }

        path_buf_try_push( buf, chunk );
        rem = internal_path_advance_by( rem, chunk.len + 1 );
    }
}
usize platform_path_stream_canonicalize(
    StreamBytesFN* stream, void* target, Path path
) {
    path_buf_from_stack( buffer, kibibytes(4) );
    win32_canonicalize( &buffer, path );

    return stream(
        target, sizeof(PathCharacter) * buffer.len, buffer.const_raw );
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

    string_buf_from_stack( buffer, kibibytes(4) );

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
usize win32_path_memory_requirement( Path path ) {
    Path prefix = path_text( "\\\\?\\" );
    b32 prefix_required = false;

    if( path.const_raw[0] == path_raw_char('~') ) {
        prefix_required = true;
    } else {
        if( path.len >= prefix.len ) {
            Path potential_prefix = path;
            potential_prefix.len  = prefix.len;
            if( !path_cmp( prefix, potential_prefix ) ) {
                prefix_required = true;
            }
        } else {
            prefix_required = true;
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

    return memory_requirement;
}
PathBuf win32_make_path( Path path ) {
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

    PathBuf p = win32_make_path( path );
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
    PathBuf d = win32_make_path( dst );
    if( path_buf_is_empty( &d ) ) {
        return false;
    }
    PathBuf s = win32_make_path( src );
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
    PathBuf d = win32_make_path( dst );
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

    PathBuf s = win32_make_path( src );
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
    PathBuf p = win32_make_path( path );
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
    PathBuf p = win32_make_path( path );
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
    PathBuf p = win32_make_path( path );
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
    PathBuf p = win32_make_path( path );
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
    PathBuf p = win32_make_path( path );
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
    path_buf_from_stack( buf, kibibytes(4) );
    win32_canonicalize( &buf, path );
    if( !path_buf_try_push( &buf, path_text( "*" ) ) ) {
        allocator->free( walk, sizeof(*walk), 0, allocator->ctx );
        core_error( "win32: path too long!" );
        return NULL;
    }

    walk->h = FindFirstFileExW(
        buf.const_raw, FindExInfoBasic,
        &walk->fd, FindExSearchNameMatch, 0, 0 );

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
#endif

#endif

#endif
