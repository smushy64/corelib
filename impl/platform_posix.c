/**
 * @file   platform_posix.c
 * @brief  POSIX platform implementation.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 14, 2024
*/
#include "core/defines.h"

#if defined(CORE_PLATFORM_POSIX)
#include "core/internal/platform.h"
#include "core/internal/logging.h"
#include "core/memory.h"
#include "core/sync.h"
#include "core/alloc.h"

#if defined(CORE_PLATFORM_LINUX)
#include "impl/platform_linux.c"
#endif

#if !defined(_LARGEFILE64_SOURCE)
    #define _LARGEFILE64_SOURCE
#endif
#if !defined(FD_OPEN_TMP_FLAG)
    #define FD_OPEN_TMP_FLAG (O_CREAT | O_EXCL)
#endif

#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <sched.h>
#include <dlfcn.h>
#include <cpuid.h>
#include <stdlib.h>
#include <string.h>

attr_global PipeRead  global_posix_stdin_fd  = (PipeRead){.fd=(FD){0}};
attr_global PipeWrite global_posix_stdout_fd = (PipeWrite){.fd=(FD){1}};
attr_global PipeWrite global_posix_stderr_fd = (PipeWrite){.fd=(FD){2}};

attr_global atomic32 global_running_thread_id = 1;
_Thread_local u32 tls_global_thread_id = 0;

struct DirectoryWalk {
    usize len;
    DIR* dp;
    char buffer[CORE_MAX_PATH_NAME];
};

struct PosixThreadParams {
    ThreadMainFN* main;
    void*         params;
    atomic32      ready;
    atomic32*     out_atom;
};

struct PosixSemaphore {
    sem_t* sem;
};
static_assert(
    sizeof(struct PosixSemaphore) == sizeof(struct NamedSemaphore),
    "Miscalculated size of NamedSemaphore!" );
struct PosixMutex {
    pthread_mutex_t mtx;
};
static_assert(
    sizeof(struct PosixMutex) == sizeof(pthread_mutex_t),
    "Miscalculated size of OSMutex!");

#if defined(CORE_PLATFORM_MACOS) || defined(CORE_PLATFORM_IOS)
    attr_global const char* global_gpu_name_stub = "Apple GPU";
#else
    attr_global const char* global_gpu_name_stub = "Unknown";
#endif

struct PosixGlobal {
    char cwd_buf[CORE_MAX_PATH_NAME];
    u32  cwd_len;

    char cpu_name_buf[255];
    u32  cpu_name_len;
};
attr_global struct PosixGlobal global_posix;

void internal_posix_get_cpu_name(void);

b32 posix_init(void) {
    memory_zero( &global_posix, sizeof(global_posix) );
    internal_posix_get_cpu_name();

    if( !getcwd( global_posix.cwd_buf, CORE_MAX_PATH_NAME ) ) {
        return false;
    }
    global_posix.cwd_len = strlen( global_posix.cwd_buf );

    return true;
}
void posix_shutdown(void) {

}

void* platform_heap_alloc( const usize size ) {
    return mmap( 0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0 );
}
#if !defined(CORE_PLATFORM_LINUX)
void* platform_heap_realloc(
    void* old_buffer, const usize old_size, const usize new_size
) {
    void* new_buffer = platform_heap_alloc( new_size );
    if( !new_buffer ) {
        return NULL;
    }
    memory_copy( new_buffer, old_buffer, old_size );
    platform_heap_free( old_buffer, old_size );

    return new_buffer;
}
#endif
void platform_heap_free( void* buffer, const usize size ) {
    munmap( buffer, size );
}

TimePosix platform_time_posix(void) {
    return time( NULL );
}
TimeSplit platform_time_split(void) {
    time_t t = time(NULL);
    struct tm tm;
    localtime_r( &t, &tm );
    TimeSplit split;

    split.year   = tm.tm_year + 1900;
    split.month  = tm.tm_mon + 1;
    split.day    = tm.tm_mday;
    split.hour   = tm.tm_hour;
    split.minute = tm.tm_min;
    split.second = tm.tm_sec % 60;

    return split;
}
attr_internal f64 ts_to_ms( struct timespec* ts ) {
    f64 result = 0.0;
    result += (f64)ts->tv_nsec / 1000000.0;
    result += (f64)ts->tv_sec * 1000.0;
    return result;
}
void ms_to_ts( u32 ms, struct timespec* out_ts ) {
    out_ts->tv_sec  = ms / 1000;
    out_ts->tv_nsec = (ms % 1000) * 1000000;
}
void ms_to_ts_abs( u32 ms, struct timespec* out_ts ) {
    struct timespec ts;
    memory_zero( &ts, sizeof(ts) );
    ms_to_ts( ms, &ts );

    struct timespec current;
    clock_gettime( CLOCK_REALTIME, &current );

    ts.tv_nsec += current.tv_nsec;
    ts.tv_sec  += current.tv_sec;
    if( ts.tv_nsec >= 999999999 ) {
        ts.tv_sec++;
        ts.tv_nsec -= 999999999;
    }
    *out_ts = ts;
}
f64 platform_timer_milliseconds(void) {
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC_RAW, &ts );

    return ts_to_ms( &ts );
}
f64 platform_timer_seconds(void) {
    return platform_timer_milliseconds() / 1000.0;
}

void platform_sleep( u32 ms ) {
    struct timespec ts;
    memory_zero( &ts, sizeof(ts) );
    ms_to_ts( ms, &ts );

    struct timespec rem;
    memory_zero( &rem, sizeof(rem) );

    while( clock_nanosleep( CLOCK_REALTIME, 0, &ts, &rem ) ) {
        ts = rem;
    }
}
void platform_yield(void) {
    sched_yield();
}

void* posix_thread_main( void* in_params ) {
    atomic32 finished = 0;

    struct PosixThreadParams* params = in_params;
    ThreadMainFN* main       = params->main;
    void*         usr_params = params->params;

    params->out_atom = &finished;

    read_write_barrier();

    atomic_exchange32( &params->ready, 1 );
    tls_global_thread_id = atomic_add32( &global_running_thread_id, 1 );

    int result = main( tls_global_thread_id, usr_params );

    read_write_barrier();
    atomic_exchange32( &finished, 1 );

    pthread_exit( (void*)(isize)result );
}
static_assert(
    sizeof(pthread_t) == sizeof(void*),
    "pthread_t must be the same size as a pointer!");
b32 platform_thread_create(
    ThreadMainFN* main, void* params,
    usize stack_size, ThreadHandle* out_handle
) {
    pthread_attr_t attr;
    int ret = pthread_attr_init( &attr );
    if( ret ) {
        const char* reason = "unknown";
        switch( ret ) {
            case ENOMEM: {
                reason = "out of memory.";
            } break;
        }
        core_error( "posix: thread_create: "
           "failed to initialize pthread attr! reason: {cc}", reason );
        return false;
    }

    pthread_attr_setstacksize(
        &attr, stack_size ? stack_size : CORE_DEFAULT_THREAD_STACK_SIZE );

    struct PosixThreadParams pt_params = {
        .main     = main,
        .params   = params,
        .ready    = 0,
        .out_atom = NULL
    }; 

    read_write_barrier();

    pthread_t handle = 0;
    ret = pthread_create( &handle, &attr, posix_thread_main, &pt_params );

    pthread_attr_destroy( &attr );

    if( ret ) {
        const char* reason = "unknown";
        switch( ret ) {
            case ENOMEM:
            case EAGAIN: {
                reason = "too many threads or out of memory.";
            } break;
            case EPERM:
            case EINVAL: {
                reason = "invalid attributes.";
            } break;
        }
        core_error(
            "posix: thread_create: "
            "failed to create thread! reason: {cc}", reason );
        return false;
    }

    out_handle->opaque.handle = (void*)handle;

    atomic_spinlock( &pt_params.ready, 1 );

    out_handle->opaque.atom = pt_params.out_atom;

    return true;
}
u32 platform_thread_query_id(void) {
    return tls_global_thread_id;
}
void platform_thread_destroy( ThreadHandle* handle ) {
    pthread_kill( (pthread_t)handle->opaque.handle, SIGKILL );
    pthread_detach( (pthread_t)handle->opaque.handle );
    memory_zero( handle, sizeof(*handle) );
}
void platform_thread_free( ThreadHandle* handle ) {
    pthread_detach( (pthread_t)handle->opaque.handle );
    memory_zero( handle, sizeof(*handle) );
}
void posix_thread_join( ThreadHandle* handle, int* opt_out_exit_code ) {
    // TODO(alicia): handle error?
    void* ret_code = 0;
    pthread_join( (pthread_t)handle->opaque.handle, &ret_code );

    read_write_barrier();

    if( opt_out_exit_code ) {
        *opt_out_exit_code = (int)(isize)ret_code;
    }
}
#if !defined(CORE_PLATFORM_LINUX)
b32 posix_thread_join_timed( ThreadHandle* handle, u32 ms, int* opt_out_exit_code ) {
    if( !atomic_spinlock_timed( handle->opaque.atom, 1, ms ) ) {
        return false;
    }

    void* ret_code = 0;
    pthread_join( (pthread_t)handle->opaque.handle, &ret_code );
    read_write_barrier();

    if( opt_out_exit_code ) {
        *opt_out_exit_code = (int)(isize)ret_code;
    }
    return true;
}
#endif
b32 platform_thread_join_timed(
    ThreadHandle* handle, u32 ms, int* opt_out_exit_code
) {
    if( ms == CORE_WAIT_INFINITE ) {
        posix_thread_join( handle, opt_out_exit_code );
        return true;
    } else {
        return posix_thread_join_timed( handle, ms, opt_out_exit_code );
    }
}
b32 platform_thread_exit_code( ThreadHandle* handle, int* out_exit_code ) {
    atomic_compare_exchange32( handle->opaque.atom, 1, 2 );
    if( *handle->opaque.atom != 2 ) {
        return false;
    }
    read_write_barrier();

    void* ret_code = 0;
    pthread_join( (pthread_t)handle->opaque.handle, &ret_code );

    *out_exit_code = (int)(isize)ret_code;
    return true;
}
b32 platform_semaphore_create(
    const char* name, u32 init, struct NamedSemaphore* out_sem
) {
    String name_string = string_from_cstr( name );
    string_buf_create_from_stack( sem_name, CORE_NAMED_SYNC_NAME_CAP + 16 );
    string_buf_try_append( &sem_name, string_text("./corelib_sem_") );
    if( name_string.len > string_buf_remaining( &sem_name ) ) {
        name_string = string_truncate(
            name_string, string_buf_remaining( &sem_name ) );
    }
    string_buf_try_append( &sem_name, name_string );

    struct PosixSemaphore* s = (struct PosixSemaphore*)out_sem;

    s->sem = sem_open( sem_name.cc, O_CREAT, S_IRWXU, init );
    if( s->sem == SEM_FAILED ) {
        core_error( 
            "failed to create named semaphore! reason: {cc}", strerror(errno) );
        return false;
    }
    return true;
}
void platform_semaphore_destroy( struct NamedSemaphore* sem ) {
    struct PosixSemaphore* s = (struct PosixSemaphore*)sem;
    sem_close( s->sem );
    memory_zero( s, sizeof(*s) );
}
void platform_semaphore_signal( struct NamedSemaphore* sem ) {
    struct PosixSemaphore* s = (struct PosixSemaphore*)sem;
    sem_post( s->sem );
}
b32 platform_semaphore_wait( struct NamedSemaphore* sem, u32 ms ) {
    struct PosixSemaphore* s = (struct PosixSemaphore*)sem;
    if( ms == CORE_WAIT_INFINITE ) {
        sem_wait( s->sem );
        return true;
    } else {
        struct timespec ts;
        ms_to_ts_abs( ms, &ts );
        int res = sem_timedwait( s->sem, &ts );
        if( !res ) {
            return true;
        }

        int errnum = errno;

        switch( errnum ) {
            case ETIMEDOUT: break;
            default: {
                core_error(
                    "posix: named_semaphore_wait: "
                    "failed to wait for semaphore! reason: {cc}", strerror(errnum) );
            } break;
        }

        return false;
    }
}
b32 platform_mutex_create( struct OSMutex* out_mutex ) {
    struct PosixMutex* mtx = (struct PosixMutex*)out_mutex;
    mtx->mtx = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    return true;
}
void platform_mutex_destroy( struct OSMutex* mutex ) {
    struct PosixMutex* mtx = (struct PosixMutex*)mutex;
    pthread_mutex_destroy( &mtx->mtx );
    memory_zero( mtx, sizeof(*mtx) );
}
b32 platform_mutex_lock( struct OSMutex* mutex, u32 ms ) {
    struct PosixMutex* mtx = (struct PosixMutex*)mutex;

    if( ms == CORE_WAIT_INFINITE ) {
        int res = 0;
        if( (res = pthread_mutex_lock( &mtx->mtx ) ) ) {
            core_error(
                "posix: os_mutex_lock: "
                "failed to lock! reason: {cc}", strerror(res) );
        }
        return true;
    } else {
        struct timespec ts;
        memory_zero( &ts, sizeof(ts) );
        ms_to_ts_abs( ms, &ts );
        int res = pthread_mutex_timedlock( &mtx->mtx, &ts );
        switch( res ) {
            case 0: return true;

            case ETIMEDOUT: return false;

            default: {
                core_error(
                    "posix: os_mutex_lock_timed: "
                    "failed to lock! reason: {cc}", strerror(res) );
            } return false;
        }
    }
}
void platform_mutex_unlock( struct OSMutex* mutex ) {
    struct PosixMutex* mtx = (struct PosixMutex*)mutex;
    int res = pthread_mutex_unlock( &mtx->mtx );
    if( res ) {
        core_error(
            "posix: os_mutex_unlock: "
            "failed to unlock! reason: {cc}", strerror(res) );
    }
}

b32 platform_fd_open( Path path, FileOpenFlags flags, FD* out_fd ) {
    int    oflag = 0;
    mode_t mode  = S_IRWXU;
    if( bitfield_check( flags, FOPEN_READ | FOPEN_WRITE ) ) {
        oflag = O_RDWR;
    } else if( bitfield_check( flags, FOPEN_READ ) ) {
        oflag = O_RDONLY;
    } else if( bitfield_check( flags, FOPEN_WRITE ) ) {
        oflag = O_WRONLY;
    }

    if( bitfield_check( flags, FOPEN_APPEND ) ) {
        oflag |= O_APPEND;
    }

    if( bitfield_check( flags, FOPEN_CREATE ) ) {
        oflag |= O_CREAT | O_EXCL;
    }
    if( bitfield_check( flags, FOPEN_TEMP ) ) {
        oflag |= FD_OPEN_TMP_FLAG;
    }
    if( bitfield_check( flags, FOPEN_TRUNCATE ) ) {
        oflag |= O_TRUNC;
    }

    PathBuf pb;
    if( path_is_null_terminated( path ) ) {
        pb.len = path.len;
        pb.cap = 0;
        pb.raw = path.raw;
    } else {
        pb.raw = platform_heap_alloc( path.len + 1 );
        if( !pb.raw ) {
            core_error( "posix: failed to allocate path buffer!" );
            return false;
        }
        memory_copy( pb.raw, path.const_raw, path.len );
        pb.cap = path.len + 1;
        pb.len = path.len;
    }

    int fd = open( pb.const_raw, oflag, mode );
    int errnum = errno;

    if( pb.cap ) {
        platform_heap_free( pb.raw, pb.cap );
    }

    if( fd < 0 ) {
        core_error(
            "posix: fd_open: "
            "failed to open '{p}'! reason: {cc}", path, strerror(errnum));
        return false;
    }
    out_fd->opaque = fd;
    return true;
}
void platform_fd_close( FD* fd ) {
    close( fd->opaque );
    memory_zero( fd, sizeof( *fd ) );
}
usize platform_fd_query_size( FD* fd ) {
    usize current = lseek64( fd->opaque, 0, SEEK_CUR );
    usize result  = lseek64( fd->opaque, 0, SEEK_END );
    lseek64( fd->opaque, current, SEEK_SET );
    return result;
}
void platform_fd_truncate( FD* fd ) {
    ftruncate64( fd->opaque, lseek64( fd->opaque, 0, SEEK_CUR ) );
}
usize platform_fd_seek( FD* fd, FileSeek type, isize seek ) {
    int whence;
    switch( type ) {
        case FSEEK_CURRENT: {
            whence = SEEK_CUR;
        } break;
        case FSEEK_SET: {
            whence = SEEK_SET;
        } break;
        case FSEEK_END: {
            whence = SEEK_END;
        } break;
    }
    return lseek64( fd->opaque, seek, whence );
}
b32 platform_fd_write(
    FD* fd, usize bytes, const void* buf, usize* opt_out_write
) {
    isize result = write( fd->opaque, buf, bytes );
    if( result < 0 ) {
        core_error(
            "posix: failed to write {usize,mib}! reason: {cc}",
            bytes, strerror(errno) );
        return false;
    }
    if( opt_out_write ) {
        *opt_out_write = (usize)result;
    }
    return true;
}
b32 platform_fd_read(
    FD* fd, usize buf_size, void* buf, usize* opt_out_read
) {
    isize result = read( fd->opaque, buf, buf_size );
    if( result < 0 ) {
        core_error(
            "posix: failed to read {usize,mib}! reason: {cc}",
            buf_size, strerror(errno) );
        return false;
    }
    if( opt_out_read ) {
        *opt_out_read = (usize)result;
    }
    return true;
}
#define FCOPY_MAX 255
b32 platform_file_copy( Path dst, Path src, b32 create_dst ) {
    FileOpenFlags dst_flags = FOPEN_TRUNCATE | FOPEN_WRITE;
    FileOpenFlags src_flags = FOPEN_WRITE;
    if( create_dst ) {
        if( file_exists( dst ) ) {
            core_error( "posix: file_copy: dst '{p}' already exists!", dst );
            return false;
        }

        dst_flags = FOPEN_CREATE | FOPEN_WRITE;
    }

    FD fd_dst, fd_src;
    if( !fd_open( dst, dst_flags, &fd_dst ) ) {
        core_error( "posix: file_copy: failed to open dst!" );
        return false;
    }
    if( !fd_open( src, src_flags, &fd_src ) ) {
        fd_close( &fd_dst );
        core_error( "posix: file_copy: failed to open src!" );
        return false;
    }

    u8  buf[FCOPY_MAX];
    usize copy_count = fd_query_size( &fd_src );

    while( copy_count ) {
        usize max_copy = copy_count;
        if( max_copy > FCOPY_MAX) {
            max_copy = FCOPY_MAX;
        }
        if( !fd_read( &fd_src, max_copy, buf, NULL ) ) {
            core_error( "posix: file_copy: failed to read from src!" );
            fd_close( &fd_src );
            fd_close( &fd_dst );
            return false;
        }
        if( !fd_write( &fd_dst, max_copy, buf, NULL ) ) {
            core_error( "posix: file_copy: failed to write to dst!" );
            fd_close( &fd_src );
            fd_close( &fd_dst );
            return false;
        }
        copy_count -= max_copy;
    }

    fd_close( &fd_src );
    fd_close( &fd_dst );
    return true;
}
#undef FCOPY_MAX

b32 platform_file_move( Path dst, Path src, b32 create_dst ) {
    if( !platform_file_copy( dst, src, create_dst ) ) {
        return false;
    }
    return platform_file_remove( src );
}
b32 platform_file_remove( Path path ) {
    PathBuf pb;
    if( path_is_null_terminated( path ) ) {
        pb.len = path.len;
        pb.cap = 0;
        pb.raw = path.raw;
    } else {
        pb.raw = platform_heap_alloc( path.len + 1 );
        if( !pb.raw ) {
            core_error( "posix: failed to allocate path buffer!" );
            return false;
        }
        memory_copy( pb.raw, path.const_raw, path.len );
        pb.cap = path.len + 1;
        pb.len = path.len;
    }

    int res = unlink( pb.const_raw );
    int errnum = errno;

    if( pb.cap ) {
        platform_heap_free( pb.raw, pb.cap );
    }

    if( res ) {
        core_error(
            "posix: file_remove: failed to remove '{p}'! reason: {cc}",
            path, strerror(errnum));
        return false;
    }

    return true;
}
int internal_posix_stat( Path path, struct stat* out_st ) {
    PathBuf pb;
    if( path_is_null_terminated( path ) ) {
        pb.len = path.len;
        pb.cap = 0;
        pb.raw = path.raw;
    } else {
        pb.raw = platform_heap_alloc( path.len + 1 );
        if( !pb.raw ) {
            core_error( "posix: failed to allocate path buffer!" );
            return false;
        }
        memory_copy( pb.raw, path.const_raw, path.len );
        pb.cap = path.len + 1;
        pb.len = path.len;
    }

    int res = stat( pb.const_raw, out_st );
    if( pb.cap ) {
        platform_heap_free( pb.raw, pb.cap );
    }
    return res;
}
b32 platform_file_exists( Path path ) {
    struct stat st;
    memory_zero( &st, sizeof(st) );

    int res = internal_posix_stat( path, &st );

    if( res ) {
        return false;
    }

    return !S_ISDIR( st.st_mode );
}

b32 platform_directory_create( Path path ) {
    PathBuf pb;
    if( path_is_null_terminated( path ) ) {
        pb.len = path.len;
        pb.cap = 0;
        pb.raw = path.raw;
    } else {
        pb.raw = platform_heap_alloc( path.len + 1 );
        if( !pb.raw ) {
            core_error( "posix: failed to allocate path buffer!" );
            return false;
        }
        memory_copy( pb.raw, path.const_raw, path.len );
        pb.cap = path.len + 1;
        pb.len = path.len;
    }

    int res = mkdir( pb.raw, S_IRWXU );
    int errnum = errno;

    if( pb.cap ) {
        platform_heap_free( pb.raw, pb.cap );
    }

    if( res ) {
        core_error(
            "posix: directory_create: "
            "failed to create directory at path '{p}'! reason: {cc}",
            path, strerror(errnum) );
        return false;
    }

    return true;
}
b32 platform_directory_remove( Path path ) {
    PathBuf pb;
    if( path_is_null_terminated( path ) ) {
        pb.len = path.len;
        pb.cap = 0;
        pb.raw = path.raw;
    } else {
        pb.raw = platform_heap_alloc( path.len + 1 );
        if( !pb.raw ) {
            core_error( "posix: failed to allocate path buffer!" );
            return false;
        }
        memory_copy( pb.raw, path.const_raw, path.len );
        pb.cap = path.len + 1;
        pb.len = path.len;
    }

    int res = rmdir( pb.const_raw );
    int errnum = errno;

    if( pb.cap ) {
        platform_heap_free( pb.raw, pb.cap );
    }

    if( res ) {
        core_error(
            "posix: directory_remove: "
            "failed to remove directory '{p}'! reason: {cc}", path, strerror(errnum) );
        return false;
    }

    return true;
}
b32 platform_directory_exists( Path path ) {
    struct stat st;
    memory_zero( &st, sizeof(st) );

    int res = internal_posix_stat( path, &st );

    if( res ) {
        return false;
    }

    return S_ISDIR( st.st_mode );
}
Path platform_directory_query_cwd(void) {
    return path_new( global_posix.cwd_len, global_posix.cwd_buf );
}
b32 platform_directory_set_cwd( Path path ) {
    if( path.len >= kibibytes(4) ) {
        core_error( "posix: cannot set cwd to a path longer than CORE_MAX_PATH_NAME!" );
        return false;
    }

    PathBuf pb;
    if( path_is_null_terminated( path ) ) {
        pb.len = path.len;
        pb.cap = 0;
        pb.raw = path.raw;
    } else {
        pb.raw = platform_heap_alloc( path.len + 1 );
        if( !pb.raw ) {
            core_error( "posix: failed to allocate path buffer!" );
            return false;
        }
        memory_copy( pb.raw, path.const_raw, path.len );
        pb.cap = path.len + 1;
        pb.len = path.len;
    }

    int res = chdir( pb.const_raw );
    int errnum = errno;

    if( pb.cap ) {
        platform_heap_free( pb.raw, pb.cap );
    }

    if( res ) {
        core_error(
            "posix: directory_set_cwd: "
            "failed to set cwd to '{p}'! reason: {cc}", path, strerror(errnum) );
        return false;
    }

    memory_zero( global_posix.cwd_buf, global_posix.cwd_len );

    getcwd( global_posix.cwd_buf, CORE_MAX_PATH_NAME );

    global_posix.cwd_len = strlen( global_posix.cwd_buf );
    return true;
}

DirectoryWalk* platform_directory_walk_begin(
    Path path, struct AllocatorInterface* allocator
) {
    if( path.len >= CORE_MAX_PATH_NAME ) {
        core_error(
            "posix: directory_walk_begin: path is longer than CORE_MAX_PATH_NAME!" );
        return NULL;
    }

    DirectoryWalk* walk = allocator->alloc( sizeof(*walk), 0, allocator->ctx );
    if( !walk ) {
        core_error( "posix: directory_walk_begin: failed to allocate directory walk!" );
        return NULL;
    }

    PathBuf pb;
    if( path_is_null_terminated( path ) ) {
        pb.len = path.len;
        pb.cap = 0;
        pb.raw = path.raw;
    } else {
        pb.raw = platform_heap_alloc( path.len + 1 );
        if( !pb.raw ) {
            core_error( "posix: failed to allocate path buffer!" );
            allocator->free( walk, sizeof(*walk), 0, allocator->ctx );
            return NULL;
        }
        memory_copy( pb.raw, path.const_raw, path.len );
        pb.cap = path.len + 1;
        pb.len = path.len;
    }

    walk->dp = opendir( pb.const_raw );
    int errnum = errno;

    if( pb.cap ) {
        platform_heap_free( pb.raw, pb.cap );
    }

    if( !walk->dp ) {
        core_error(
            "posix: directory_walk_begin: "
            "failed to open directory '{p}'! reason: {cc}",
            path, strerror(errnum) );
        allocator->free( walk, sizeof(*walk), 0, allocator->ctx );
        return NULL;
    }

    memory_copy( walk->buffer, path.const_raw, path.len );
    walk->len = path.len;

    return walk;
}
b32 platform_directory_walk_next(
    DirectoryWalk* walk, Path* out_path, b32* opt_out_is_directory
) {
    struct dirent* entry = readdir( (DIR*)walk );
    if( !entry ) {
        return false;
    }

    usize entry_len = strlen( entry->d_name );
    Path entry_path = path_new( entry_len, entry->d_name );
    *out_path = entry_path;

    if( !opt_out_is_directory ) {
        return true;
    }

    PathBuf walk_buf = (PathBuf){
        .cap = CORE_MAX_PATH_NAME,
        .len = walk->len,
        .raw = walk->buffer
    };

    path_buf_try_push( &walk_buf, entry_path );

    struct stat st;
    memory_zero( &st, sizeof(st) );

    if( stat( walk_buf.const_raw, &st ) ) {
        core_warn( "posix: directory_walk_next: "
            "failed to query if entry is a directory! entry: {p} reason: {cc}",
            walk_buf.slice, strerror(errno) );
    } else {
        *opt_out_is_directory = S_ISDIR( st.st_mode );
    }

    walk->buffer[walk->len] = 0;
    return true;
}
void platform_directory_walk_end(
    DirectoryWalk* walk, struct AllocatorInterface* allocator
) {
    if( !walk ) {
        return;
    }

    closedir( walk->dp );
    allocator->free( walk, sizeof(*walk), 0, allocator->ctx );
}

b32 platform_pipe_open( PipeRead* out_read, PipeWrite* out_write ) {
    int pd[2];
    memory_zero( pd, sizeof(pd) );
    int res = pipe( pd );
    if( res ) {
        core_error(
            "posix: pipe_open: failed to open pipes! reason: {cc}",
            strerror(errno) );
        return false;
    }
    out_read->fd.opaque  = pd[0];
    out_write->fd.opaque = pd[1];
    return true;
}

PipeRead* platform_stdin(void) {
    return &global_posix_stdin_fd;
}
PipeWrite* platform_stdout(void) {
    return &global_posix_stdout_fd;
}
PipeWrite* platform_stderr(void) {
    return &global_posix_stderr_fd;
}

#if defined(CORE_ARCH_X86)
void internal_posix_get_cpu_name(void) {
    unsigned int cpu_info[4] = {0};
    char* chunk = global_posix.cpu_name_buf;

    __get_cpuid( 0x80000002, cpu_info + 0, cpu_info + 1, cpu_info + 2, cpu_info + 3 );
    memory_copy( chunk, cpu_info, sizeof(cpu_info) );
    chunk += sizeof(cpu_info);

    __get_cpuid( 0x80000003, cpu_info + 0, cpu_info + 1, cpu_info + 2, cpu_info + 3 );
    memory_copy( chunk, cpu_info, sizeof(cpu_info) );
    chunk += sizeof(cpu_info);

    __get_cpuid( 0x80000004, cpu_info + 0, cpu_info + 1, cpu_info + 2, cpu_info + 3 );
    memory_copy( chunk, cpu_info, sizeof(cpu_info) );
    chunk += sizeof(cpu_info);

    for( u32 i = sizeof(global_posix.cpu_name_buf); i-- > 0; ) {
        if( global_posix.cpu_name_buf[i] && global_posix.cpu_name_buf[i] != ' ' ) {
            global_posix.cpu_name_len = i + 1;
            break;
        }
    }
}

CPUFeatureFlags cpu_feature_flags(void) {
    CPUFeatureFlags feature_flags = 0;
    unsigned int eax, ebx, ecx, edx;
    __get_cpuid( 1, &eax, &ebx, &ecx, &edx );

    if( edx & (1 << 25) ) {
        feature_flags |= CPU_FEATURE_SSE;
    }
    if( edx & (1 << 26) ) {
        feature_flags |= CPU_FEATURE_SSE2;
    }
    if( ecx & (1 << 0) ) {
        feature_flags |= CPU_FEATURE_SSE3;
    }
    if( ecx & (1 << 9) ) {
        feature_flags |= CPU_FEATURE_SSSE3;
    }
    if( ecx & (1 << 19) ) {
        feature_flags |= CPU_FEATURE_SSE4_1;
    }
    if( ecx & (1 << 20) ) {
        feature_flags |= CPU_FEATURE_SSE4_2;
    }
    if( ecx & (1 << 28) ) {
        feature_flags |= CPU_FEATURE_AVX;
    }

    __get_cpuid_count( 7, 0, &eax, &ebx, &ecx, &edx );

    if( ebx & (1 << 5) ) {
        feature_flags |= CPU_FEATURE_AVX2;
    }

    if(
        (ebx & (1 << 16)) &&
        (ebx & (1 << 30)) &&
        (ecx & (1 << 24)) &&
        (ecx & (1 << 27)) &&
        (ecx & (1 << 25))
    ) {
        feature_flags |= CPU_FEATURE_AVX_512;
    }

    return feature_flags;
}
#endif /* Arch x86_64 */

void platform_system_query_info( SystemInfo* out_info ) {
    out_info->page_size     = sysconf( _SC_PAGE_SIZE );
    out_info->total_memory  = sysconf( _SC_PHYS_PAGES ) * out_info->page_size;
    out_info->cpu_count     = sysconf( _SC_NPROCESSORS_ONLN );
    out_info->feature_flags = cpu_feature_flags();
    out_info->gpu_name      = string_from_cstr( global_gpu_name_stub );
    out_info->cpu_name      = string_new(
        global_posix.cpu_name_len, global_posix.cpu_name_buf );

}

#if !defined(CORE_PLATFORM_LINUX)
void* platform_library_open( const char* name ) {
    return dlopen( name, RTLD_GLOBAL | RTLD_LAZY );
}
void* platform_library_get( const char* name ) {
    // TODO(alicia): this is linux only!
    // implement library reference counting for non-linux posix!
    return dlopen( name, RTLD_NOLOAD );
}
void platform_library_close( void* lib ) {
    dlclose( lib );
}
#endif

void* platform_library_load( void* lib, const char* function ) {
    return dlsym( lib, function );
}

void posix_canonicalize( PathBuf* buf, Path path ) {
    enum {
        POSIX_PATH_REL,
        POSIX_PATH_HOME,
        POSIX_PATH_ABS,
    } type = POSIX_PATH_REL;

    if( path_is_absolute( path ) ) {
        type = POSIX_PATH_ABS;
    } else if( path.const_raw[0] == '~' ) {
        if( path.len >= 2 ) {
            if( path.const_raw[1] == '/' ) {
                type = POSIX_PATH_HOME;
            }
        } else {
            type = POSIX_PATH_HOME;
        }
    }

    String rem = string_new( path.len, path.const_raw );
    switch( type ) {
        case POSIX_PATH_REL: {
            usize max_copy = global_posix.cwd_len;
            if( max_copy > buf->cap - 1 ) {
                max_copy = buf->cap - 1;
            }
            memory_copy( buf->raw, global_posix.cwd_buf, max_copy );
            buf->len = max_copy;
        } break;
        case POSIX_PATH_HOME: {
            char* home = getenv( "HOME" );
            if( home ) {
                usize home_len = strlen( home );
                memory_copy( buf->raw, home, home_len );
                buf->len += home_len;

                if( path.len >= 2 ) {
                    rem = string_advance_by( rem, 2 );
                } else {
                    return;
                }
            } else {
                core_warn( "posix: canonicalize: getenv(\"HOME\") returned NULL!" );
                buf->raw[buf->len++] = '~';
                buf->raw[buf->len++] = '/';

                if( path.len >= 2 ) {
                    rem = string_advance_by( rem, 2 );
                } else {
                    return;
                }
            }
        } break;
        case POSIX_PATH_ABS: {
            buf->raw[buf->len++] = '/';
        } break;
    }

    // leading forward-slash.
    usize min = 1;

    while( !string_is_empty(rem) ) {
        String chunk_str = rem;
        usize  sep       = 0;
        if( string_find( chunk_str, '/', &sep ) ) {
            if( !sep ) {
                rem = string_advance( rem );
                continue;
            }
            chunk_str.len = sep;
        }

        if( chunk_str.len < 3 ) {
            if( string_cmp( chunk_str, string_text("."))) {
                rem = string_advance_by( rem, chunk_str.len + 1 );
                continue;
            }
            if( string_cmp( chunk_str, string_text(".."))) {
                for( usize i = buf->len; i-- > 0; ) {
                    if( buf->const_raw[i] == '/' ) {
                        buf->len = i;
                        break;
                    }
                }

                if( buf->len < min ) {
                    buf->len = min;
                }

                buf->raw[buf->len] = 0;
                rem = string_advance_by( rem, chunk_str.len + 1 );
                continue;
            }
        }

        Path chunk = path_new( chunk_str.len, chunk_str.cc );
        path_buf_try_push( buf, chunk );
        rem = string_advance_by( rem, chunk.len + 1 );
    }
}

usize platform_path_stream_canonicalize(
    StreamBytesFN* stream, void* target, Path path 
) {
    if( !path.len ) {
        return 0;
    }
    Path p = path_new(
        path.len >= CORE_MAX_PATH_NAME ? CORE_MAX_PATH_NAME - 1 : path.len,
        path.const_raw );

    path_buf_create_from_stack( buffer, CORE_MAX_PATH_NAME );
    posix_canonicalize( &buffer, p );

    return stream( target, buffer.len, buffer.const_raw );
}

#endif /* Platform POSIX */

