/**
 * @file   platform_posix.c
 * @brief  POSIX platform implementation.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 14, 2024
*/
#include "core/defines.h"

#if defined(CORE_PLATFORM_POSIX)
#include "core/internal/platform/library.h"
#include "core/internal/platform/path.h"
#include "core/internal/platform/time.h"
#include "core/internal/platform/memory.h"
#include "core/internal/platform/thread.h"
#include "core/internal/platform/fs.h"
#include "core/internal/platform/process.h"
#include "core/internal/platform/misc.h"

#include "core/internal/logging.h"

#include "core/memory.h"
#include "core/sync.h"
#include "core/fs.h"
#include "core/system.h"
#include "core/time.h"
#include "core/thread.h"
#include "core/constants.h"

#if defined(CORE_PLATFORM_LINUX)
    #include "src/core/platform_linux.c"
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
#include <sys/wait.h>
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
#include <ftw.h>

attr_global FD global_posix_stdin_fd  = (FD){.opaque=0};
attr_global FD global_posix_stdout_fd = (FD){.opaque=1};
attr_global FD global_posix_stderr_fd = (FD){.opaque=2};

attr_global atomic32 global_running_thread_id = 1;
_Thread_local u32 tls_global_thread_id = 0;

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
    char cwd_buf[CORE_PATH_NAME_LEN];
    u32  cwd_len;

    char cpu_name_buf[255];
    u32  cpu_name_len;
};
attr_global struct PosixGlobal global_posix;
_Thread_local char* tls_global_posix_path_buf;
union FTWState {
    struct {
        DirectoryWalkFN* function;
        void*            params;
    } directory_walk;
    struct {
        b32 success;
    } directory_remove_recursive;
}; 
_Thread_local union FTWState tls_global_ftw_state;

void internal_posix_get_cpu_name(void);

b32 posix_init(void) {
    memory_zero( &global_posix, sizeof(global_posix) );
    internal_posix_get_cpu_name();

    if( !getcwd( global_posix.cwd_buf, CORE_PATH_NAME_LEN ) ) {
        return false;
    }
    global_posix.cwd_len  = strlen( global_posix.cwd_buf );
    tls_global_posix_path_buf = (char*)calloc( 1, CORE_PATH_NAME_LEN );

    return tls_global_posix_path_buf != NULL;
}
void posix_shutdown(void) {

}

void* platform_heap_alloc( void* opt_old_ptr, usize opt_old_size, usize new_size ) {
    if( opt_old_ptr ) {
        u8* result = (u8*)realloc( opt_old_ptr, new_size );
        if( !result ) {
            return NULL;
        }
        memory_set( result + opt_old_size, 0, new_size - opt_old_size );
        return result;
    } else {
        return calloc( 1, new_size );
    }
#if 0
    void* result = mmap(
        0, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0 );
    if( result == MAP_FAILED ) {
        return NULL;
    }
    return result;
#endif
}
void platform_heap_free( void* buffer, usize size ) {
    unused(size);
    free( buffer );
#if 0
    munmap( buffer, size );
#endif
}

TimePosix platform_time_posix(void) {
    return time(NULL);
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

    // TODO(alicia): what to do if this fails?
    tls_global_posix_path_buf = calloc( 1, CORE_PATH_NAME_LEN );

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

    free( tls_global_posix_path_buf );

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
    string_buf_from_stack( sem_name, CORE_NAMED_SYNC_NAME_CAP + 16 );
    string_buf_try_append( &sem_name, string_text("./corelib_sem_") );
    if( name_string.len > string_buf_remaining( sem_name ) ) {
        name_string = string_truncate(
            name_string, string_buf_remaining( sem_name ) );
    }
    string_buf_try_append( &sem_name, name_string );

    struct PosixSemaphore* s = (struct PosixSemaphore*)out_sem;

    s->sem = sem_open( sem_name.cbuf, O_CREAT, S_IRWXU, init );
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

char* posix_get_local_buffer(void) {
    return memory_zero( tls_global_posix_path_buf, CORE_PATH_NAME_LEN );
}
const char* posix_path_null_terminated( struct _StringPOD p ) {
    if( path_is_null_terminated( p ) ) {
        return p.cbuf;
    }

    char* buf = posix_get_local_buffer();

    memory_copy( buf, p.cbuf, p.len );
    buf[p.len] = 0;

    return buf;
}

b32 platform_file_remove_by_path( struct _StringPOD path ) {
    const char* p = posix_path_null_terminated( path );

    int result = unlink( p );
    int errnum = errno;
    if( result ) {
        core_error( 
            "POSIX: file_remove(): failed to remove '{p}'! reason: {cc}",
            path, strerror(errnum) );
        return false;
    }
    return true;
}

FileType internal_posix_file_type_from_stat( const struct stat* st ) {
    if( S_ISREG( st->st_mode ) ) {
        return FTYPE_FILE;
    } else if( S_ISDIR( st->st_mode ) ) {
        return FTYPE_DIRECTORY;
    } else {
        return FTYPE_UNKNOWN;
    }
}

b32 platform_file_query_info_by_path( struct _StringPOD path, struct FileInfo* out_info ) {
    const char* p = posix_path_null_terminated( path );
    struct stat st;
    if( stat( p, &st ) != 0 ) {
        int errnum = errno;
        core_error(
            "POSIX: file_query_info(): failed to stat '{p}'! reason: {cc}",
            path, strerror(errnum) );
        return false;
    }

    out_info->size        = st.st_size;
    out_info->time.create = st.st_ctime;
    out_info->time.modify = st.st_mtime;
    out_info->type        = internal_posix_file_type_from_stat( &st );

    out_info->permissions = 0;
    if( access( p, R_OK ) == 0 ) {
        out_info->permissions |= FPERM_READ;
    } else {
        int errnum = errno;
        if( errnum != EACCES ) {
            core_warn(
                "POSIX: file_query_info(): "
                "failed to query file access for '{p}'! reason: {cc}",
                path, strerror(errnum) );
        }
    }
    if( access( p, W_OK ) == 0 ) {
        out_info->permissions |= FPERM_WRITE;
    } else {
        int errnum = errno;
        if( errnum != EACCES ) {
            core_warn(
                "POSIX: file_query_info(): "
                "failed to query file access for '{p}'! reason: {cc}",
                path, strerror(errnum) );
        }
    }
    if( access( p, X_OK ) == 0 ) {
        out_info->permissions |= FPERM_EXECUTE;
    } else {
        int errnum = errno;
        if( errnum != EACCES ) {
            core_warn(
                "POSIX: file_query_info(): "
                "failed to query file access for '{p}'! reason: {cc}",
                path, strerror(errnum) );
        }
    }

    return true;
}
enum FileType platform_file_query_type_by_path( struct _StringPOD path ) {
    const char* p = posix_path_null_terminated( path );
    struct stat st;
    if( stat( p, &st ) ) {
        int errnum = errno;
        core_error(
            "POSIX: file_query_type_by_path(): failed to stat '{p}'! reason: {cc}",
            path, strerror(errnum) );
        return FTYPE_NULL;
    }

    return internal_posix_file_type_from_stat( &st );
}
b32 platform_file_exists_by_path( struct _StringPOD path ) {
    const char* p = posix_path_null_terminated( path );
    struct stat st;
    if( stat( p, &st ) ) {
        return false;
    } else {
        return true;
    }
}
TimePosix platform_file_query_time_create_by_path( struct _StringPOD path ) {
    const char* p = posix_path_null_terminated( path );
    struct stat st;
    if( stat( p, &st ) ) {
        int errnum = errno;
        core_error(
            "POSIX: file_query_time_create_by_path(): failed to stat '{p}'! reason: {cc}",
            path, strerror(errnum) );
        return 0;
    }
    return st.st_ctime;
}
TimePosix platform_file_query_time_modify_by_path( struct _StringPOD path ) {
    const char* p = posix_path_null_terminated( path );
    struct stat st;
    if( stat( p, &st ) ) {
        int errnum = errno;
        core_error(
            "POSIX: file_query_time_create_by_path(): failed to stat '{p}'! reason: {cc}",
            path, strerror(errnum) );
        return 0;
    }
    return st.st_mtime;
}

b32 platform_file_open(
    struct _StringPOD path, enum FileOpenFlags flags, struct FD* out_fd 
) {
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

    const char* p = posix_path_null_terminated( path );
    int fd     = open( p, oflag, mode );
    int errnum = errno;

    if( fd < 0 ) {
        core_error(
            "POSIX: file_open(): "
            "failed to open '{p}'! reason: {cc}", path, strerror(errnum));
        return false;
    }
    out_fd->opaque = fd;
    return true;
}
void platform_file_close( struct FD* fd ) {
    close( fd->opaque );
    memory_zero( fd, sizeof(*fd) );
}
b32 platform_file_query_info( struct FD* fd, struct FileInfo* out_info ) {
    struct stat st;
    if( fstat( fd->opaque, &st ) != 0 ) {
        int errnum = errno;
        core_error(
            "POSIX: file_query_info(): failed to stat! reason: {cc}",
            strerror(errnum) );
        return false;
    }

    out_info->size        = st.st_size;
    out_info->time.create = st.st_ctime;
    out_info->time.modify = st.st_mtime;
    out_info->type        = internal_posix_file_type_from_stat( &st );

    // TODO(alicia): 
    out_info->permissions = 0;

    return true;
}
enum FileType platform_file_query_type( struct FD* fd ) {
    struct stat st;
    if( fstat( fd->opaque, &st ) ) {
        int errnum = errno;
        core_error(
            "POSIX: file_query_type(): failed to stat! reason: {cc}",
            strerror(errnum) );
        return FTYPE_NULL;
    }
    return internal_posix_file_type_from_stat( &st );
}
TimePosix platform_file_query_time_create( struct FD* fd ) {
    struct stat st;
    if( fstat( fd->opaque, &st ) ) {
        int errnum = errno;
        core_error(
            "POSIX: file_query_time_create(): failed to stat! reason: {cc}",
            strerror(errnum) );
        return 0;
    }
    return st.st_ctime;
}
TimePosix platform_file_query_time_modify( struct FD* fd ) {
    struct stat st;
    if( fstat( fd->opaque, &st ) ) {
        int errnum = errno;
        core_error(
            "POSIX: file_query_time_modify(): failed to stat! reason: {cc}",
            strerror(errnum) );
        return 0;
    }
    return st.st_mtime;
}
usize platform_file_query_size( struct FD* fd ) {
    usize offset = platform_file_query_offset( fd );
    usize result = platform_file_seek( fd, FSEEK_END, 0 );
    platform_file_seek( fd, FSEEK_SET, offset );
    return result;
}
usize platform_file_query_offset( struct FD* fd ) {
    return platform_file_seek( fd, FSEEK_CURRENT, 0 );
}
void platform_file_truncate( struct FD* fd ) {
    ftruncate64( fd->opaque, platform_file_query_offset( fd ) );
}
usize platform_file_seek( struct FD* fd, enum FileSeek type, isize seek ) {
    int whence = SEEK_CUR;
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
b32 platform_file_write( struct FD* fd, usize bytes, const void* buf, usize* out_write ) {
    isize result = write( fd->opaque, buf, bytes );
    if( result < 0 ) {
        core_error(
            "POSIX: failed to write {usize,mib}! reason: {cc}",
            bytes, strerror(errno) );
        return false;
    }
    *out_write = (usize)result;
    return true;
}
b32 platform_file_read( struct FD* fd, usize bytes, void* buf, usize* out_read ) {
    isize result = read( fd->opaque, buf, bytes );
    if( result < 0 ) {
        core_error(
            "POSIX: failed to read {usize,mib}! reason: {cc}",
            bytes, strerror(errno) );
        return false;
    }
    *out_read = (usize)result;
    return true;
}

b32 platform_directory_create( struct _StringPOD path ) {
    const char* p = posix_path_null_terminated( path );
    if( mkdir( p, S_IRWXU ) ) {
        int errnum = errno;
        core_error(
            "POSIX: directory_create(): failed to create '{p}'! reason: {cc}",
            path, strerror(errnum) );
        return false;
    }
    return true;
}
b32 internal_posix_directory_remove_recursive( const char* path );
int internal_posix_directory_remove_nftw(
    const char* filename, const struct stat* st, int flag, struct FTW* _info
) {
    unused(flag,_info);
    if( S_ISDIR( st->st_mode ) ) {
        if( !internal_posix_directory_remove_recursive( filename ) ) {
            return FTW_STOP;
        }
        if( rmdir( filename ) ) {
            int errnum = errno;
            core_error(
                "POSIX: directory_remove(): "
                "failed to remove directory '{cc}'! reason: {cc}",
                filename, strerror(errnum) );

            tls_global_ftw_state.directory_remove_recursive.success = false;
            return FTW_STOP;
        }
        return FTW_SKIP_SUBTREE;
    } else {
        if( unlink( filename ) ) {
            int errnum = errno;
            core_error(
                "POSIX: directory_remove(): "
                "failed to remove file '{cc}'! reason: {cc}",
                filename, strerror(errnum) );
            tls_global_ftw_state.directory_remove_recursive.success = false;
            return FTW_STOP;
        }
        return FTW_CONTINUE;
    }
}
b32 internal_posix_directory_remove_recursive( const char* path ) {
    struct stat st;
    if( stat( path, &st ) ) {
        int errnum = errno;
        core_error(
            "POSIX: directory_remove(): failed to stat '{cc}'! reason: {cc}",
            path, strerror(errnum) );
        return false;
    }

    if( !S_ISDIR( st.st_mode ) ) {
        core_error(
            "POSIX: directory_remove(): path '{cc}' does not point to a directory!", path );
        return false;
    }

    tls_global_ftw_state.directory_remove_recursive.success = true;

    if( nftw( path, internal_posix_directory_remove_nftw, 5, FTW_ACTIONRETVAL ) < 0 ) {
        core_error(
            "POSIX: directory_remove(): failed to walk '{cc}'! reason: {cc}",
            path, strerror(errno) );
        return false;
    }

    return tls_global_ftw_state.directory_remove_recursive.success;
}
b32 platform_directory_remove( struct _StringPOD path, b32 recursive ) {
    if( recursive ) {
        const char* p = posix_path_null_terminated( path );
        return internal_posix_directory_remove_recursive( p );
    } else {
        const char* p = posix_path_null_terminated( path );
        if( rmdir( p ) ) {
            int errnum = errno;
            core_error(
                "POSIX: directory_remove(): "
                "failed to remove directory '{p}'! reason: {cc}",
                path, strerror(errnum));
            return false;
        }
        return true;
    }
}
int internal_posix_nftw(
    const char* filename, const struct stat* st, int flag, struct FTW* _info
) {
    unused(flag);

    DirectoryWalkInfo info = {};
    info.type             = internal_posix_file_type_from_stat( st );
    info.size             = st->st_size;
    info.path             = filename;
    info.path_len         = strlen( filename );
    info.path_name_offset = _info->base;
    info.level            = _info->level;

    switch( tls_global_ftw_state.directory_walk.function(
        &info, tls_global_ftw_state.directory_walk.params
    ) ) {
        case DWC_CONTINUE: return FTW_CONTINUE;
        case DWC_STOP:     return FTW_STOP;
        case DWC_SKIP:     return FTW_SKIP_SUBTREE;
    }
    return FTW_STOP;
}
b32 platform_directory_walk(
    struct _StringPOD path,
    enum DirectoryWalkControl (callback)( const struct DirectoryWalkInfo* info, void* params ),
    void* params
) {
    const char* p = posix_path_null_terminated( path );
    tls_global_ftw_state.directory_walk.function = callback;
    tls_global_ftw_state.directory_walk.params   = params;

    if( nftw( p, internal_posix_nftw, 5, FTW_ACTIONRETVAL ) < 0 ) {
        core_error(
            "POSIX: directory_walk(): failed to walk '{p}'! reason: {cc}",
            path, strerror(errno) );
        return false;
    }
    return true;
}
struct _StringPOD platform_directory_current_query(void) {
    return string_new( global_posix.cwd_len, global_posix.cwd_buf );
}
b32 platform_directory_current_set( struct _StringPOD path ) {
    const char* p = posix_path_null_terminated( path );
    if( chdir( p ) ) {
        int errnum = errno;
        core_error(
            "POSIX: directory_current_set(): failed to change to '{p}'! reason: {cc}",
            path, strerror(errnum) );
        return false;
    }

    memory_copy( global_posix.cwd_buf, path.cbuf, path.len );
    global_posix.cwd_len           = path.len;
    global_posix.cwd_buf[path.len] = 0;

    return true;
}

struct FD*  platform_pipe_stdin(void) {
    return &global_posix_stdin_fd;
}
struct FD* platform_pipe_stdout(void) {
    return &global_posix_stdout_fd;
}
struct FD* platform_pipe_stderr(void) {
    return &global_posix_stderr_fd;
}

b32 platform_pipe_open( FD* out_read, FD* out_write ) {
    int fd[2];
    if( pipe( fd ) ) {
        int errnum = errno;
        core_error(
            "POSIX: pipe_open(): failed to open pipes! reason: {cc}", strerror(errnum) );
        return false;
    }
    out_read->opaque  = fd[0];
    out_write->opaque = fd[1];
    return true;
}
void platform_pipe_close( const FD* pipe ) {
    close( pipe->opaque );
    memory_zero( (FD*)pipe, sizeof(*pipe) );
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
void* platform_library_get( struct _StringPOD in_name ) {
    const char* name = posix_path_null_terminated( in_name );
    // TODO(alicia): this is linux only!
    // implement library reference counting for non-linux posix!
    return dlopen( name, RTLD_NOLOAD );
}
#endif

void* platform_library_open( struct _StringPOD in_name ) {
    const char* name = posix_path_null_terminated( in_name );
    return dlopen( name, RTLD_GLOBAL | RTLD_LAZY );
}
void* platform_library_load( void* lib, struct _StringPOD in_fn_name ) {
    const char* fn_name = posix_path_null_terminated( in_fn_name );
    return dlsym( lib, fn_name );
}
void platform_library_close( void* lib ) {
    dlclose( lib );
}

void posix_canonicalize( struct _StringBufPOD* buf, struct _StringPOD path ) {
    enum {
        POSIX_PATH_REL,
        POSIX_PATH_HOME,
        POSIX_PATH_ABS,
    } type = POSIX_PATH_REL;

    if( path_is_absolute( path ) ) {
        type = POSIX_PATH_ABS;
    } else if( path.cbuf[0] == '~' ) {
        if( path.len >= 2 ) {
            if( path.cbuf[1] == '/' ) {
                type = POSIX_PATH_HOME;
            }
        } else {
            type = POSIX_PATH_HOME;
        }
    }

    String rem = string_new( path.len, path.cbuf );
    switch( type ) {
        case POSIX_PATH_REL: {
            usize max_copy = global_posix.cwd_len;
            if( max_copy > buf->cap - 1 ) {
                max_copy = buf->cap - 1;
            }
            memory_copy( buf->buf, global_posix.cwd_buf, max_copy );
            buf->len = max_copy;
        } break;
        case POSIX_PATH_HOME: {
            char* home = getenv( "HOME" );
            if( home ) {
                usize home_len = strlen( home );
                memory_copy( buf->buf, home, home_len );
                buf->len += home_len;

                if( path.len >= 2 ) {
                    rem = string_advance_by( rem, 2 );
                } else {
                    return;
                }
            } else {
                core_warn( "posix: canonicalize: getenv(\"HOME\") returned NULL!" );
                buf->buf[buf->len++] = '~';
                buf->buf[buf->len++] = '/';

                if( path.len >= 2 ) {
                    rem = string_advance_by( rem, 2 );
                } else {
                    return;
                }
            }
        } break;
        case POSIX_PATH_ABS: {
            buf->buf[buf->len++] = '/';
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
                    if( buf->cbuf[i] == '/' ) {
                        buf->len = i;
                        break;
                    }
                }

                if( buf->len < min ) {
                    buf->len = min;
                }

                buf->buf[buf->len] = 0;
                rem = string_advance_by( rem, chunk_str.len + 1 );
                continue;
            }
        }

        struct _StringPOD chunk = string_new( chunk_str.len, chunk_str.cbuf );
        path_buf_try_push_chunk( buf, chunk );
        rem = string_advance_by( rem, chunk.len + 1 );
    }
}

usize platform_path_chunk_count( struct _StringPOD path ) {
    struct _StringPOD remaining = path;
    if( !remaining.len ) {
        return 0;
    }

    if( remaining.buf[0] == '/' ) {
        remaining = string_advance( remaining );
    }

    usize result = 0;
    while( !string_is_empty( remaining ) ) {
        struct _StringPOD chunk = remaining;
        string_find( chunk, '/', &chunk.len );

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
    if( path.buf[0] == '/' ) {
        if( path.len == 1 ) {
            return result;
        } else {
            string_find( string_advance( result ), '/', &result.len );
        }
    } else {
        string_find( result, '/', &result.len );
    }
    return result;
}

struct _StringPOD platform_path_clip_chunk_last( struct _StringPOD path ) {
    if( !path.len ) {
        return path;
    }

    struct _StringPOD result = path;
    if( string_last_unchecked( result ) == '/' ) {
        result = string_trim( result, 1 );

        if( !result.len ) {
            return result;
        }
    }

    usize previous_separator = 0;
    if( string_find_rev( result, '/', &previous_separator ) ) {
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
    return path.buf[0] == '/';
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

    if( string_last_unchecked( path ) == '/' ) {
        return false;
    }

    *out_file_name = path_clip_chunk_last( path );
    return true;
}

b32 platform_path_stream_set_native_separators(
    StreamBytesFN* stream, void* target, struct _StringPOD path
) {
    return path_stream_set_posix_separators( stream, target, path );
}

void platform_path_set_native_separators( struct _StringPOD path ) {
    path_set_posix_separators( path );
}

usize platform_path_stream_canonicalize(
    StreamBytesFN* stream, void* target, struct _StringPOD path
) {
    string_buf_from_stack( buffer, CORE_PATH_NAME_LEN );
    posix_canonicalize( &buffer, path );
    return stream( target, buffer.len, buffer.cbuf );
}

b32 platform_path_buf_try_push_chunk( struct _StringBufPOD* buf, struct _StringPOD chunk ) {
    if( !chunk.len ) {
        return true;
    }

    b32 chunk_has_separator = string_first_unchecked( chunk ) == '/';
    if( chunk_has_separator && chunk.len == 1 ) {
        return true;
    }

    b32 buf_has_separator = false;
    if( !string_buf_is_empty( *buf ) ) {
        buf_has_separator = string_last_unchecked( buf->slice ) == '/';
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
        string_buf_try_push( buf, '/' );
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

struct _StringPOD platform_environment_query( struct _StringPOD key ) {
    const char* key_nt = posix_path_null_terminated( key );
    const char* result = getenv( key_nt );
    if( !result ) {
        return string_empty();
    }
    return string_from_cstr( result );
}
b32 platform_environment_set( struct _StringPOD key, struct _StringPOD value ) {
    char* local = posix_get_local_buffer();

    const char* key_nt   = memory_copy( local, key.buf, key.len );
    local[key.len] = 0;
    const char* value_nt = memory_copy( local + key.len + 1, value.buf, value.len );

    if( setenv( key_nt, value_nt, true ) ) {
        int errnum = errno;
        core_error(
            "posix: environment_set: "
            "failed to set environment variable! reason: {cc}", strerror(errnum) );
        return false;
    }
    return true;
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
    if( !command.len ) {
        core_error( "posix: process_exec: command is empty!" );
        return false;
    }

    int _pipe_stdin  =
        opt_stdin  ? opt_stdin->opaque : STDIN_FILENO;
    int _pipe_stdout =
        opt_stdout ? opt_stdout->opaque : STDOUT_FILENO;
    int _pipe_stderr =
        opt_stderr ? opt_stderr->opaque : STDERR_FILENO;

    pid_t pid = fork();
    if( pid < 0 ) {
        core_error( "posix: process_exec_async: failed to fork process!" );
        return false;
    }

    if( pid ) { // thread that ran process_exec
        out_pid->opaque = pid;
        return true;
    }

    // thread where process will run

    if( opt_working_directory ) {
        platform_directory_current_set( *opt_working_directory );
    }
    dup2(  _pipe_stdin, STDIN_FILENO );
    dup2( _pipe_stdout, STDOUT_FILENO );
    dup2( _pipe_stderr, STDERR_FILENO );

    if( opt_environment ) {
        StringBuf temp = {};

        usize string_count = opt_environment->len * 2;
        for( usize pair = 0; pair < string_count; pair += 2 ) {
            String key   = opt_environment->buf[pair + 0];
            String value = opt_environment->buf[pair + 1];

            b32 value_has_semicolon = string_find( value, ';', NULL );

            if( value_has_semicolon ) {
                if( temp.buf ) {
                    temp.len = 0;
                    if( temp.cap < value.len ) {
                        temp.buf = memory_realloc(
                            temp.buf, temp.cap, temp.cap + value.len + 16 );
                    }
                } else {
                    temp.cap = value.len + 1;
                    temp.len = 0;
                    temp.buf = memory_alloc( temp.cap );
                }

                value.buf = memory_copy( temp.buf, value.buf, value.len );
                for( usize i = 0; i < value.len; ++i ) {
                    if( value.buf[i] == ';' ) {
                        value.buf[i] = ':';
                    }
                }
            }

            platform_environment_set( key, value );
        }

        if( temp.buf ) {
            memory_free( temp.buf, temp.cap );
        }
    }

    usize command_buffer_size = 0;
    for( usize i = 0; i < command.len; ++i ) {
        command_buffer_size += command.buf[i].len + 1;
    }

    char* command_buffer  = calloc( 1, command_buffer_size );
    const char** commands = calloc( command.len + 1, sizeof(const char*) );
    usize offset = 0;
    for( usize i = 0; i < command.len; ++i ) {
        String current = command.buf[i];
        commands[i] = memory_copy( command_buffer + offset, current.buf, current.len );
        offset += current.len + 1;
    }

    // NOTE(alicia): execvp should only return if command failed to execute.
    execvp( commands[0], (char* const*)(commands) );
    abort();
}
void platform_process_discard( Process* pid ) {
    // NOTE(alicia): no need to discard on posix systems
    unused(pid);
}
int platform_process_wait( Process* pid ) {
    int wstatus = 0;
    pid_t value = waitpid( pid->opaque, &wstatus, 0 );
    if( value < 0 ) {
        int errnum = errno;
        core_error(
            "posix: process_wait: "
            "failed to wait for pid! reason: %s", strerror(errnum) );
        return -2;
    }
    if( WIFEXITED( wstatus ) ) {
        pid->opaque = 1;
        return WEXITSTATUS( wstatus );
    } else {
        return -1;
    }
}
b32 platform_process_wait_timed( Process* pid, u32 msec, int* opt_out_exit_code ) {
    if( msec == U32_MAX ) {
        int res = platform_process_wait( pid );
        if( res < 0 ) {
            return false;
        }
        if( opt_out_exit_code ) {
            *opt_out_exit_code = res;
        }
        pid->opaque = 1;
        return true;
    }

    for( u32 i = 0; i < msec; ++i ) {
        int wstatus = 0;
        pid_t value = waitpid( pid->opaque, &wstatus, WNOHANG );
        if( value == 0 ) {
            sleep(1);
            continue;
        }

        if( value < 0 ) {
            int errnum = errno;
            core_error(
                "posix: process_wait_timed: "
                "failed to wait for pid! reason: %s", strerror(errnum) );
            return false;
        }

        if( opt_out_exit_code ) {
            if( WIFEXITED( wstatus ) ) {
                *opt_out_exit_code = WEXITSTATUS( wstatus );
            } else {
                *opt_out_exit_code = -1;
            }
        }

        pid->opaque = 1;
        return true;
    }

    return false;
}
void platform_process_kill( Process* pid ) {
    if( kill( pid->opaque, SIGKILL ) ) {
        int errnum = errno;
        core_error(
            "posix: process_kill: failed to kill pid! reason: %s",
            strerror(errnum) );
    }
    pid->opaque = 1;
}
b32 platform_process_find( struct _StringPOD process_name ) {
    // TODO(alicia): replace use of 'which' and system!
    StringBuf buf = string_buf_new( CORE_PATH_NAME_LEN, posix_get_local_buffer() );

    string_buf_try_fmt_text( &buf, "which {s} > /dev/null 2>&1{c}", process_name, 0 );

    return system( buf.buf ) == 0;
}

#endif /* Platform POSIX */

