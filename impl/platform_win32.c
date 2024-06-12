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

#include "core/internal/platform.h"
#include "core/internal/logging.h"

#define NOMINMAX
#include <windows.h>
#include <intrin.h>

typedef BOOL EnumDisplayDevicesAFN( LPCSTR lpDevice, DWORD iDevNum, PDISPLAY_DEVICEA lpDisplayDevice, DWORD dwFlags );
attr_global attr_unused
EnumDisplayDevicesAFN* in_EnumDisplayDevicesA = NULL;
#define EnumDisplayDevicesA in_EnumDisplayDevicesA

attr_internal void internal_win32_log_error( DWORD error_code );

void* platform_win32_get_stdin(void) {
    return GetStdHandle( STD_INPUT_HANDLE );
}
void* platform_win32_get_stdout(void) {
    return GetStdHandle( STD_OUTPUT_HANDLE );
}
void* platform_win32_get_stderr(void) {
    return GetStdHandle( STD_ERROR_HANDLE );
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

void platform_query_timestamp( TimeStamp* out_timestamp ) {
    SYSTEMTIME systime;
    GetLocalTime( &systime );

    out_timestamp->year   = (u32)systime.wYear;
    out_timestamp->month  = (u32)systime.wMonth;
    out_timestamp->day    = (u32)systime.wDay;
    out_timestamp->hour   = (u32)systime.wHour;
    out_timestamp->minute = (u32)systime.wMinute;
    out_timestamp->second = (u32)systime.wSecond;
}

f64 platform_query_milliseconds(void) {
    LARGE_INTEGER qpf, qpc;
    QueryPerformanceFrequency( &qpf );
    QueryPerformanceCounter( &qpc );

    return ((f64)( qpc.QuadPart) / (f64)qpf.QuadPart) * 1000.0;
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
    Semaphore*    sem;
    ThreadMainFN* main;
    void*         params;
};
attr_global volatile u32 global_thread_id = 1;

attr_internal DWORD internal_win32_thread_proc( void* in ) {
    struct Win32Thread thread = rcast( struct Win32Thread, in );
    _ReadWriteBarrier();

    semaphore_signal( thread.sem );

    u32 thread_id = atomic_increment32( &global_thread_id );
    _ReadWriteBarrier();

    volatile int ret = thread.main( thread_id, thread.params );

    ExitThread( ret );
}

b32 platform_thread_create(
    ThreadMainFN* main, void* params, usize stack_size, void** handle
) {
    Semaphore sem;
    if( !semaphore_create( &sem ) ) {
        core_error( "win32: failed to create thread ready semaphore!" );
        return false;
    }

    struct Win32Thread thread;
    thread.sem    = &sem;
    thread.main   = main;
    thread.params = params;

    _ReadWriteBarrier();

    DWORD id = 0;
    HANDLE thread_handle = CreateThread(
        NULL, stack_size, internal_win32_thread_proc, &thread, 0, &id );
    if( thread_handle == NULL ) {
        DWORD error_code = GetLastError();
        internal_win32_log_error( error_code );
        semaphore_destroy( &sem );
        return false;
    }

    _ReadWriteBarrier();
    semaphore_wait( thread.sem );

    _ReadWriteBarrier();
    semaphore_destroy( thread.sem );

    *handle = thread_handle;
    return true;
}
void platform_thread_destroy( void* handle ) {
    TerminateThread( handle, 0 );
    CloseHandle( handle );
}
b32 platform_thread_exit_code( void* handle, int* out_exit_code ) {
    DWORD result = WaitForSingleObject( handle, 0 );
    if( result != WAIT_OBJECT_0 ) {
        // thread has not yet finished.
        return false;
    }

    DWORD exit_code = 0;
    GetExitCodeThread( handle, &exit_code );

    *out_exit_code = rcast( int, &exit_code );
    return true;
}
attr_internal void internal_win32_log_error( DWORD error_code ) {
#if defined(CORE_ENABLE_LOGGING)
    char* buf = NULL;
    DWORD res = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, error_code, GetSystemDefaultLangID(),
        (char*)&buf, 0, NULL );

    if( res && buf ) {
        String message;
        message.len = res - 1;
        message.cc  = buf;
        core_error( "win32: [{u,X,f}] {s}", error_code, message );

        LocalFree( buf );
    }
#endif
    unused( error_code );
}

struct Win32Path {
    union {
        Path    p;
        PathBuf pb;
        struct {
            const char* cc;
            usize       len;
            usize       cap;
        };
    };
    b32  heap_allocated;
};

attr_internal struct Win32Path path_to_win32( const Path in_path ) {
    struct Win32Path path = {0};
    path.p = in_path;

    // TODO(alicia): any way to not heap allocate?
    // Or allocate more intelligently?
    HANDLE process_heap = GetProcessHeap();

    b32 is_nt = false;
    if( path.p.len ) {
        if( !path.p.c[path.p.len - 1] || !path.p.c[path.p.len] ) {
            is_nt = true;
        }
    }
    b32 is_lteq_max_path = path.len <= MAX_PATH;

    if( is_nt && is_lteq_max_path ) {
        return path;
    }

    usize nt_size = path.len + 1;
    char* nt_path = HeapAlloc( process_heap, HEAP_ZERO_MEMORY, nt_size );
    memory_copy( nt_path, path.cc, path.len );

    path.heap_allocated = true;
    if( !is_nt && is_lteq_max_path ) {
        path.cc  = nt_path;
        path.cap = nt_size;
        path.len = path.cap - 1;

        return path;
    }

    #define PATH_PREPEND "\\\\?\\"

    DWORD full_size = GetFullPathNameA( nt_path, 0, 0, 0 );
    usize buf_size  = path.len + full_size + (sizeof(PATH_PREPEND) - 1);
    char* buf = HeapAlloc( process_heap, HEAP_ZERO_MEMORY, buf_size );

    memory_copy( buf, PATH_PREPEND, sizeof(PATH_PREPEND) - 1 );
    GetFullPathNameA(
        nt_path,
        buf_size - (sizeof(PATH_PREPEND) - 1),
        buf + (sizeof(PATH_PREPEND) - 1),
        NULL );

    HeapFree( process_heap, 0, nt_path );
    path.cc  = buf;
    path.cap = buf_size;
    path.len = path.cap - 1;

    #undef PATH_PREPEND
    return path;
}
attr_internal void path_win32_free( struct Win32Path path ) {
    if( !path.heap_allocated ) {
        return;
    }

    HeapFree( GetProcessHeap(), 0, (void*)path.cc );
}

b32 platform_path_is_directory( const Path in_path ) {
    struct Win32Path path = path_to_win32( in_path );

    DWORD attributes = GetFileAttributesA( path.cc );

    path_win32_free( path );

    return
        attributes != INVALID_FILE_ATTRIBUTES &&
        bitfield_check( attributes, FILE_ATTRIBUTE_DIRECTORY );
}
b32 platform_path_is_file( const Path in_path ) {
    struct Win32Path path = path_to_win32( in_path );

    DWORD attributes = GetFileAttributesA( path.cc );

    path_win32_free( path );

    return
        attributes != INVALID_FILE_ATTRIBUTES &&
        !bitfield_check( attributes, FILE_ATTRIBUTE_DIRECTORY );
}

FileHandle* platform_file_open( const Path in_path, FileOpenFlags flags ) {
    struct Win32Path path = path_to_win32( in_path );

    DWORD dwDesiredAccess = 0;
    if( bitfield_check( flags, FILE_OPEN_FLAG_READ ) ) {
        dwDesiredAccess |= GENERIC_READ;
    }
    if( bitfield_check( flags, FILE_OPEN_FLAG_WRITE ) ) {
        dwDesiredAccess |= GENERIC_WRITE;
    }
    DWORD dwShareMode = 0;
    if( bitfield_check( flags, FILE_OPEN_FLAG_SHARE_ACCESS_WRITE ) ) {
        dwShareMode |= FILE_SHARE_WRITE;
    }
    if( bitfield_check( flags, FILE_OPEN_FLAG_SHARE_ACCESS_READ ) ) {
        dwShareMode |= FILE_SHARE_READ;
    }

    LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;

    DWORD dwCreationDisposition = OPEN_EXISTING;
    if( bitfield_check( flags, FILE_OPEN_FLAG_CREATE ) ) {
        dwCreationDisposition = CREATE_ALWAYS;
    }
    if( bitfield_check( flags, FILE_OPEN_FLAG_TRUNCATE ) ) {
        dwCreationDisposition = TRUNCATE_EXISTING;
    }

    DWORD dwFlagsAndAttributes = 0;
    if( bitfield_check( flags, FILE_OPEN_FLAG_TEMP ) ) {
        dwFlagsAndAttributes |= FILE_ATTRIBUTE_TEMPORARY;
    }

    HANDLE hTemplateFile = NULL;

    HANDLE handle = CreateFileA(
        path.cc, dwDesiredAccess, dwShareMode,
        lpSecurityAttributes, dwCreationDisposition,
        dwFlagsAndAttributes, hTemplateFile );
    path_win32_free( path );

    if( handle == INVALID_HANDLE_VALUE ) {
        internal_win32_log_error( GetLastError() );
        core_error( "win32: failed to open file '{p}'!", in_path );
        return NULL;
    } else {
        return handle;
    }
}
void platform_file_close( FileHandle* file ) {
    CloseHandle( (HANDLE)file );
}
usize platform_file_query_size( FileHandle* file ) {
#if defined(CORE_ARCH_64_BIT)
    LARGE_INTEGER result = {0};
    GetFileSizeEx( file, &result );
    return result.QuadPart;
#else
    return GetFileSize( file, NULL );
#endif
}
void platform_file_truncate( FileHandle* file ) {
    SetEndOfFile( file );
}
usize platform_file_query_offset( FileHandle* file ) {
#if defined(CORE_ARCH_64_BIT)
    LARGE_INTEGER offset = {0};
    LARGE_INTEGER result = {0};
    SetFilePointerEx( file, offset, &result, FILE_CURRENT );
    return result.QuadPart;
#else
    LONG result = 0;
    SetFilePointer( file, 0, &result, FILE_CURRENT );
    return result;
#endif
}
void platform_file_set_offset( FileHandle* file, usize offset ) {
    DWORD dwMoveMethod = FILE_BEGIN;
#if defined(CORE_ARCH_64_BIT)
    LARGE_INTEGER large_offset = {0};
    large_offset.QuadPart = offset;

    SetFilePointerEx( file, large_offset, NULL, dwMoveMethod );
#else
    LONG small_offset = offset;
    SetFilePointer( file, small_offset, NULL, dwMoveMethod );
#endif
}
void platform_file_set_offset_relative( FileHandle* file, isize offset ) {
    DWORD dwMoveMethod = FILE_CURRENT;
#if defined(CORE_ARCH_64_BIT)
    LARGE_INTEGER large_offset = {0};
    large_offset.QuadPart = offset;

    SetFilePointerEx( file, large_offset, NULL, dwMoveMethod );
#else
    LONG small_offset = offset;
    SetFilePointer( file, small_offset, NULL, dwMoveMethod );
#endif
}

attr_internal b32 internal_win32_write32(
    FileHandle* file, u32 len, const void* buf
) {
    DWORD written = 0;
    BOOL result = WriteFile( file, buf, len, &written, NULL );

    if( !result ) {
        internal_win32_log_error( GetLastError() );
        core_error( "win32: failed to write to file!" );
        return false;
    }

    if( written != len ) {
        core_error(
            "win32: failed to write requested bytes! "
            "requested: {u32} written: {u32}", len, written );
        return false;
    }

    return true;
}
attr_internal b32 internal_win32_read32(
    FileHandle* file, u32 len, void* buf
) {
    DWORD read  = 0;
    BOOL result = ReadFile( file, buf, len, &read, NULL );

    if( !result ) {
        internal_win32_log_error( GetLastError() );
        core_error( "win32: failed to read {f,.2,m} from file!", (f64)len );
        return false;
    }

    if( read != len ) {
        core_error(
            "win32: failed to read requested bytes! "
            "requested: {u32} read: {u32}", len, read );
        return false;
    }

    return true;
}

b32 platform_file_write( FileHandle* file, usize len, const void* buf ) {
#if defined(CORE_ARCH_64_BIT)
    if( len > U32_MAX ) {
        u32 rem = len - U32_MAX;
        if( !internal_win32_write32( file, U32_MAX, buf ) ) {
            return false;
        }
        return internal_win32_write32( file, rem, (const u8*)buf + U32_MAX );
    }
#endif
    return internal_win32_write32( file, len, buf );
}
b32 platform_file_read( FileHandle* file, usize len, void* buf ) {
#if defined(CORE_ARCH_64_BIT)
    if( len > U32_MAX ) {
        u32 rem = len - U32_MAX;
        if( !internal_win32_read32( file, U32_MAX, buf ) ) {
            return false;
        }
        return internal_win32_read32( file, rem, (u8*)buf + U32_MAX );
    }
#endif
    return internal_win32_read32( file, len, buf );
}
b32 platform_file_copy(
    const Path in_dst, const Path in_src, b32 fail_if_dst_exists
) {
    struct Win32Path dst = path_to_win32( in_dst );
    struct Win32Path src = path_to_win32( in_src );

    int result = CopyFileA( src.cc, dst.cc, fail_if_dst_exists );

    path_win32_free( dst );
    path_win32_free( src );

    if( !result ) {
        internal_win32_log_error( GetLastError() );
        core_error( "win32: failed to copy '{p}' to '{p}'!", in_src, in_dst );
        return false;
    }

    return true;
}
b32 platform_file_move(
    const Path in_dst, const Path in_src, b32 fail_if_dst_exists
) {
    struct Win32Path dst = path_to_win32( in_dst );
    struct Win32Path src = path_to_win32( in_src );

    if( fail_if_dst_exists ) {
        if( fs_file_exists( dst.p ) ) {
            core_error(
                "win32: failed to move {p} to {p}, destination already exists!",
                in_src, in_dst );
            path_win32_free( dst );
            path_win32_free( src );
            return false;
        }
    }

    DWORD dwFlags  = MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH;
    int result = MoveFileExA( src.cc, dst.cc, dwFlags );

    path_win32_free( dst );
    path_win32_free( src );

    if( !result ) {
        internal_win32_log_error( GetLastError() );
        core_error( "win32: failed to move '{p}' to '{p}'!", in_src, in_dst );
        return false;
    }

    return true;
}
b32 platform_file_delete( const Path in_path ) {
    struct Win32Path path = path_to_win32( in_path );

    BOOL result = DeleteFileA( path.cc );
    path_win32_free( path );

    if( !result ) {
        internal_win32_log_error( GetLastError() );
        core_error( "win32: failed to delete '{p}'!", in_path );
        return false;
    }

    return true;
}
b32 platform_file_exists( const Path in_path ) {
    struct Win32Path path = path_to_win32( in_path );
    DWORD attributes = GetFileAttributesA( path.cc );
    path_win32_free( path );

    if(
        attributes == INVALID_FILE_ATTRIBUTES                  ||
        bitfield_check( attributes, FILE_ATTRIBUTE_DIRECTORY ) ||
        GetLastError() == ERROR_FILE_NOT_FOUND
    ) {
        return false;
    }

    return true;
}
b32 platform_directory_exists( const Path in_path ) {
    struct Win32Path path = path_to_win32( in_path );
    DWORD attributes = GetFileAttributesA( path.cc );
    path_win32_free( path );

    if(
        attributes == INVALID_FILE_ATTRIBUTES                   ||
        !bitfield_check( attributes, FILE_ATTRIBUTE_DIRECTORY ) ||
        // TODO(alicia): test to see if this is valid?
        GetLastError() == ERROR_FILE_NOT_FOUND
    ) {
        return false;
    }

    return true;
}
b32 platform_directory_create( const Path in_path ) {
    struct Win32Path path = path_to_win32( in_path );
    
    BOOL result = CreateDirectoryA( path.cc, NULL );
    path_win32_free( path );

    if( !result ) {
        DWORD err = GetLastError();
        if( err == ERROR_ALREADY_EXISTS ) {
            return true;
        }

        internal_win32_log_error( err );
        core_error( "win32: failed to create directory at path '{p}'!", in_path );
        return false;
    }

    return true;
}

b32 platform_directory_item_count( const Path in_path, usize* out_count ) {
    usize count = 0;

    struct Win32Path path = path_to_win32( in_path );

    WIN32_FIND_DATAA fd = {0};
    HANDLE find = FindFirstFileA( path.cc, &fd );
    path_win32_free( path );

    if( find == INVALID_HANDLE_VALUE ) {
        internal_win32_log_error( GetLastError() );
        core_error( "win32: Path '{p}' is not a valid directory!", in_path.cc );
        return false;
    }

    // NOTE(alicia): this is to skip '.' and '..'
    #define SKIP_COUNT (2)
    int skip = 0;
    while( FindNextFileA( find, &fd ) ) {
        skip++;
        if( skip < SKIP_COUNT ) {
            continue;
        }
        count++;
    }

    #undef SKIP_COUNT
    *out_count = count;
    return true;
}

attr_global char* global_working_directory     = NULL;
attr_global usize global_working_directory_len = 0;

attr_internal void internal_win32_pwd(void) {
    if( !global_working_directory ) {
        DWORD size = GetCurrentDirectoryA( 0, 0 ) + 1;

        global_working_directory =
            HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, size );
        assert( global_working_directory );

        GetCurrentDirectoryA( size, global_working_directory );
        global_working_directory_len = size - 1;
    }
}

Path platform_working_directory(void) {
    internal_win32_pwd();

    Path working_directory;
    working_directory.cc  = global_working_directory;
    working_directory.len = global_working_directory_len;

    return working_directory;
}

attr_global char global_cpu_name[255] = {0};
attr_global usize global_cpu_name_len = 0;

attr_internal void internal_win32_get_cpu_name(void) {
    if( !global_cpu_name_len ) {
        memory_zero( global_cpu_name, sizeof(global_cpu_name) );

#if defined(CORE_ARCH_X86)
        int cpu_info[4] = {0};
        char* chunk = global_cpu_name;

        __cpuid( cpu_info, 0x80000002 );
        memory_copy( chunk, cpu_info, sizeof(cpu_info) );
        chunk += sizeof(cpu_info);

        __cpuid( cpu_info, 0x80000003 );
        memory_copy( chunk, cpu_info, sizeof(cpu_info) );
        chunk += sizeof(cpu_info);

        __cpuid( cpu_info, 0x80000004 );
        memory_copy( chunk, cpu_info, sizeof(cpu_info) );

        chunk = global_cpu_name;
        for( u32 i = 255; i-- > 0; ) {
            if( chunk[i] && chunk[i] != ' ' ) {
                global_cpu_name_len = i + 1;
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

    HMODULE user32 = GetModuleHandleA( "USER32.DLL" );
    if( !user32 ) {
        user32 = LoadLibraryA( "USER32.DLL" );
    }

    EnumDisplayDevicesA =
        (EnumDisplayDevicesAFN*)(void*)GetProcAddress( user32, "EnumDisplayDevicesA" );
}

attr_global char* global_gpu_name     = NULL;
attr_global usize global_gpu_name_len = 0;

attr_internal void internal_win32_get_gpu_name(void) {
    if( global_gpu_name ) {
        return;
    }

    internal_win32_load_enum_display_devices();
    DISPLAY_DEVICE dd = {0};
    dd.cb = sizeof(dd);

    EnumDisplayDevicesA( NULL, 0, &dd, EDD_GET_DEVICE_INTERFACE_NAME );

    usize name_len  = cstr_len( dd.DeviceString );
    global_gpu_name = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, name_len + 1 );

    memory_copy( global_gpu_name, dd.DeviceString, name_len );
    global_gpu_name_len = name_len;
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
    out_info->cpu_name.cc  = global_cpu_name;
    out_info->cpu_name.len = global_cpu_name_len;

    internal_win32_get_gpu_name();
    out_info->gpu_name.cc  = global_gpu_name;
    out_info->gpu_name.len = global_gpu_name_len;

}

void* platform_library_open( const char* name ) {
    HMODULE module = LoadLibraryA( name );
    if( !module ) {
        internal_win32_log_error( GetLastError() );
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
        internal_win32_log_error( GetLastError() );
        core_error( "win32: failed to load function '{cc}'!", function );
        return NULL;
    }
    return addr;
}

#endif
