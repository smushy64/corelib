/**
 * @file   platform_linux.c
 * @brief  Linux Platform Layer
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 09, 2024
*/
#include "core/defines.h"

#if defined(CORE_PLATFORM_LINUX)

#include "core/prelude.h"
#include "core/time.h"
#include "core/thread.h"
#include "core/path.h"
#include "core/fs.h"
#include "core/system.h"

#include "core/internal/platform.h"
#include "core/sync.h"
#include "core/collections.h"
#include "core/memory.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

attr_internal struct timespec platform_ms_to_ts( u32 ms ) {
    struct timespec result;
    result.tv_sec  = ms / 1000;
    result.tv_nsec = (ms % 1000) * 1000000;
    return result;
}
void* platform_heap_alloc( const usize size ) {
    int prot  = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE;
    void* result = mmap( NULL, size, prot, flags, 0, 0 );
    if( result == ((void*)(-1)) ) {
        return NULL;
    }

    return result;
}
void* platform_heap_realloc(
    void* old_buffer, const usize old_size, const usize new_size
) {
    void* result = mremap( old_buffer, old_size, new_size, MREMAP_MAYMOVE );
    if( result == ((void*)-1) ) {
        return NULL;
    }
    return result;
}
void platform_heap_free( void* buffer, const usize size ) {
    (void)munmap( buffer, size );
}

void platform_query_timestamp( TimeStamp* out_timestamp ) {
    time_t raw = time(NULL);
    struct tm tm;
    localtime_r( &raw, &tm );

    TimeStamp result;
    result.year   = tm.tm_year + 1900; // tm_year is years since 1900
    result.month  = tm.tm_mon + 1; // Range (0..11) to range (1..12)
    result.day    = tm.tm_mday;
    result.hour   = tm.tm_hour;
    result.minute = tm.tm_min;
    result.second = tm.tm_sec;

    *out_timestamp = result;
}
CoreTicks platform_query_ticks(void) {
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC_RAW, &ts );

    CoreTicks result;
    result.sec  = ts.tv_sec;
    result.nsec = ts.tv_nsec;

    return result;
}
CoreTicks platform_ticks_diff( CoreTicks lhs, CoreTicks rhs ) {
    CoreTicks result;
    result.sec  = lhs.sec - rhs.sec;
    result.nsec = lhs.nsec - rhs.nsec;
    return result;
}
f64 platform_ticks_to_seconds( CoreTicks ticks ) {
    return (f64)ticks.sec + ((f64)ticks.nsec / 1000000000.0);
}

#define MAX_WRITE (0x7ffff000)

void platform_console_write( void* dst, usize len, const char* str ) {
    i64 fd64 = (i64)dst;
    int fd   = (int)fd64;

    assert( (fd == 0) || (fd == 1) || (fd == 2) );

    usize write_size = len;
    usize offset     = 0;

#if defined(CORE_ARCH_64_BIT)
    while( write_size > MAX_WRITE ) {
        write( fd, str + offset, MAX_WRITE );

        offset     += MAX_WRITE;
        write_size -= MAX_WRITE;
    }
#endif

    write( fd, str + offset, write_size );
}

struct LinuxSemaphore {
    sem_t* handle;
};
static_assert(
    sizeof(struct LinuxSemaphore) >= sizeof(Semaphore),
    "semaphore sizes don't match!" );
#define to_sem( semaphore ) struct LinuxSemaphore* sem = (struct LinuxSemaphore*)(semaphore)

b32 platform_semaphore_create(
    const char* name, struct Semaphore* out_sem
) {
    struct LinuxSemaphore* out = (struct LinuxSemaphore*)out_sem;

    mode_t mode  = S_IRWXU;
    int    oflag = O_CREAT;
    sem_t* res   = sem_open( name, oflag, mode, 0 );

    if( res == SEM_FAILED ) {
        return false;
    }

    out->handle = res;
    return true;
}
void platform_semaphore_destroy( struct Semaphore* in_sem ) {
    to_sem( in_sem );
    sem_close( sem->handle );
}
void platform_semaphore_signal( struct Semaphore* in_sem ) {
    to_sem( in_sem );
    sem_post( sem->handle );
}
b32 platform_semaphore_wait( struct Semaphore* in_sem, u32 ms ) {
    to_sem( in_sem );
    if( ms == U32_MAX ) {
        sem_wait( sem->handle );
        return true;
    } else {
        struct timespec ts = platform_ms_to_ts( ms );
        errno = 0;
        sem_timedwait( sem->handle, &ts );

        return errno != ETIMEDOUT;
    }
}

struct LinuxMutex {
    pthread_mutex_t handle;
};
static_assert(
    sizeof(struct LinuxMutex) >= sizeof(Mutex),
    "mutex sizes don't match!" );
#define to_mtx( mutex ) struct LinuxMutex* mtx = (struct LinuxMutex*)(mutex)

b32 platform_mutex_create( const char* name, struct Mutex* out_mutex ) {
    unused(name);
    pthread_mutexattr_t attr;
    memory_zero( &attr, sizeof(attr) );

    int error = pthread_mutexattr_init( &attr );
    if( error ) {
        return false;
    }

    struct LinuxMutex* out = (struct LinuxMutex*)out_mutex;
    error = pthread_mutex_init( &out->handle, &attr );
    pthread_mutexattr_destroy( &attr );
    if( error ) {
        return false;
    }

    return true;
}
void platform_mutex_destroy( struct Mutex* mutex ) {
    to_mtx( mutex );

    pthread_mutex_destroy( &mtx->handle );
}
b32 platform_mutex_lock( struct Mutex* mutex, u32 ms ) {
    to_mtx( mutex );

    if( ms == U32_MAX ) {
        pthread_mutex_lock( &mtx->handle );
        return true;
    } else {
        struct timespec ts = platform_ms_to_ts( ms );
        errno = 0;
        pthread_mutex_timedlock( &mtx->handle, &ts );
        return errno != ETIMEDOUT;
    }
}
void platform_mutex_unlock( struct Mutex* mutex ) {
    to_mtx( mutex );
    pthread_mutex_unlock( &mtx->handle );
}

void platform_sleep( u32 ms ) {
    struct timespec ts = platform_ms_to_ts( ms );
    nanosleep( &ts, NULL );
}

attr_global volatile u32 global_thread_id = 1;
struct LinuxThread {
    b32           is_valid;
    volatile b32  done;
    pthread_t     handle;
    ThreadMainFN* main;
    void*         params;
};

attr_internal
_Noreturn void* platform_linux_thread_proc( void* in ) {
    struct LinuxThread* thread = (struct LinuxThread*)in;
    u32 thread_id = interlocked_decrement( &global_thread_id );
    read_write_fence();

    volatile int ret = thread->main( thread_id, thread->params );

    read_write_fence();
    thread->done = true;
    pthread_exit( (void*)((isize)ret) );
}
b32 platform_thread_create(
    ThreadMainFN* main, void* params, usize stack_size, void** out_handle
) {
    struct LinuxThread* thread = memory_alloc( sizeof(struct LinuxThread) );
    if( !thread ) {
        return false;
    }

    thread->main   = main;
    thread->params = params;

    pthread_attr_t attr;
    int err = pthread_attr_init( &attr );
    if( err ) {
        memory_free( thread, sizeof( *thread ) );
        return false;
    }

    err = pthread_attr_setstacksize( &attr, stack_size );
    if( err ) {
        memory_free( thread, sizeof( *thread ) );
        return false;
    }

    read_write_fence();
    pthread_t handle;
    err = pthread_create( &handle, &attr, platform_linux_thread_proc, thread );
    pthread_attr_destroy( &attr );

    if( err ) {
        memory_free( thread, sizeof( *thread ) );
        return false;
    }

    read_write_fence();
    thread->handle   = handle;
    thread->is_valid = true;

    *out_handle = (void*)thread;
    return true;
}
// TODO(alicia): rewrite completely arggghhh
void platform_thread_destroy( void* handle ) {
    struct LinuxThread* thread = (struct LinuxThread*)handle;
    if( thread->is_valid ) {
        pthread_cancel( thread->handle );
    }
    memory_free( thread, sizeof( *thread ) );
}
b32 platform_thread_exit_code( void* handle, int* out_exit_code ) {
    struct LinuxThread* thread = (struct LinuxThread*)handle;
    if( !thread->done ) {
        return false;
    }

    void* ret = NULL;
    pthread_join( thread->handle, &ret );
    *out_exit_code = (int)((isize)ret);
    return true;
}

struct LinuxPath {
    union {
        Path    p;
        PathBuf pb;
        struct {
            const char* cc;
            usize       len;
            usize       cap;
        };
    };
    b32 heap_allocated;
};

attr_internal struct LinuxPath path_to_linux( Path p ) {
    struct LinuxPath result;
    result.heap_allocated = false;
    result.p = p;

    if( string_is_null_terminated( p ) ) {
        return result;
    }

    result.heap_allocated = true;
    char* buf = (char*)memory_alloc( p.len + 1 );
    memory_copy( buf, p.v, p.len );
    buf[p.len] = 0;

    result.cc = buf;

    return result;
}
attr_internal void path_linux_free( struct LinuxPath p ) {
    if( !p.heap_allocated ) {
        return;
    }
    memory_free( p.p.v, p.len );
}

b32 platform_path_is_file( const Path in_path ) {
    struct LinuxPath path = path_to_linux( in_path );

    struct stat pstat;
    int ret = stat( path.cc, &pstat );

    path_linux_free( path );

    return (ret == 0) && S_ISREG( pstat.st_mode );
}
b32 platform_path_is_directory( const Path in_path ) {
    struct LinuxPath path = path_to_linux( in_path );

    struct stat pstat;
    int ret = stat( path.cc, &pstat );

    path_linux_free( path );

    return (ret == 0) && S_ISDIR( pstat.st_mode );
}

FileHandle* platform_file_open( const Path in_path, FileOpenFlags flags ) {
    struct LinuxPath path = path_to_linux( in_path );

    int oflag = 0; {
        b32 read  = bitfield_check( flags, FILE_OPEN_FLAG_READ );
        b32 write = bitfield_check( flags, FILE_OPEN_FLAG_WRITE );

        if( read && write ) {
            oflag |= O_RDWR;
        } else if( read ) {
            oflag |= O_RDONLY;
        } else if( write ) {
            oflag |= O_WRONLY;
        }

        if( bitfield_check( flags, FILE_OPEN_FLAG_CREATE ) ) {
            oflag |= O_CREAT;
        }

        if( bitfield_check( flags, FILE_OPEN_FLAG_APPEND ) ) {
            oflag |= O_APPEND;
        }

        if( bitfield_check( flags, FILE_OPEN_FLAG_TRUNCATE ) ) {
            oflag |= O_TRUNC;
        }
    }

    int ret = open( path.cc, oflag );

    path_linux_free( path );

    if( ret < 0 ) {
        return NULL;
    }

    return (FileHandle*)((isize)(ret));
}

#define to_fd( file ) ((isize)file)

void platform_file_close( FileHandle* file ) {
    int fd = to_fd( file );
    close( fd );
}
usize platform_file_query_size( FileHandle* file ) {
    int fd = to_fd( file );

    struct stat st;
    fstat( fd, &st );

    return st.st_size;
}
void platform_file_truncate( FileHandle* file ) {
    int fd    = to_fd( file );
    usize off = lseek( fd, 0, SEEK_CUR );
    ftruncate( fd, off );
}
usize platform_file_query_offset( FileHandle* file ) {
    int fd = to_fd( file );
    return lseek( fd, 0, SEEK_CUR );
}
void platform_file_set_offset( FileHandle* file, usize offset ) {
    int fd = to_fd( file );
    lseek( fd, offset, SEEK_SET );
}
void platform_file_set_offset_relative( FileHandle* file, isize offset ) {
    int fd = to_fd( file );
    lseek( fd, offset, SEEK_CUR );
}
b32 platform_file_write( FileHandle* file, usize len, const void* buf ) {
    int fd = to_fd( file );
    ssize_t written = write( fd, buf, len );

    if( written < 0 ) {
        return false;
    }

    if( (usize)written != len ) {
        return false;
    }

    return true;
}
b32 platform_file_read( FileHandle* file, usize len, void* buf ) {
    int fd = to_fd( file );
    ssize_t read_size = read( fd, buf, len );

    if( read_size < 0 ) {
        return false;
    }

    if( (usize)read_size != len ) {
        return false;
    }

    return true;
}
b32 platform_file_copy(
    const Path in_dst, const Path in_src, b32 fail_if_dst_exists
) {
    // TODO(alicia): logging, error checking!
    struct LinuxPath dst_p = path_to_linux( in_dst );
    b32 dst_exists = platform_file_exists( dst_p.p );
    if( fail_if_dst_exists && dst_exists ) {
        path_linux_free( dst_p );
        return false;
    }
    struct LinuxPath src_p = path_to_linux( in_src );

    int dst_flags = O_WRONLY | (dst_exists ? O_TRUNC : O_CREAT);
    int dst = open( dst_p.cc, dst_flags );
    int src = open( src_p.cc, O_RDONLY );

    struct stat src_st;
    fstat( src, &src_st );

    usize src_size = src_st.st_size;
    void* buf = memory_alloc( src_size );
    if( !buf ) {
        path_linux_free( src_p );
        path_linux_free( dst_p );
        close( src );
        close( dst );
        return false;
    }

    read( src, buf, src_size );
    write( dst, buf, src_size );

    path_linux_free( src_p );
    path_linux_free( dst_p );
    close( src );
    close( dst );

    return true;
}
b32 platform_file_move(
    const Path in_dst, const Path in_src, b32 fail_if_dst_exists
) {
    // TODO(alicia): logging, error checking!
    struct LinuxPath dst_p = path_to_linux( in_dst );
    b32 dst_exists = platform_file_exists( dst_p.p );
    if( fail_if_dst_exists && dst_exists ) {
        path_linux_free( dst_p );
        return false;
    }
    struct LinuxPath src_p = path_to_linux( in_src );

    int dst_flags = O_WRONLY | (dst_exists ? O_TRUNC : O_CREAT);
    int dst = open( dst_p.cc, dst_flags );
    int src = open( src_p.cc, O_RDONLY );

    struct stat src_st;
    fstat( src, &src_st );

    usize src_size = src_st.st_size;
    void* buf = memory_alloc( src_size );
    if( !buf ) {
        path_linux_free( src_p );
        path_linux_free( dst_p );
        close( src );
        close( dst );
        return false;
    }

    read( src, buf, src_size );
    write( dst, buf, src_size );
    path_linux_free( dst_p );
    close( dst );

    close( src );

    unlink( src_p.cc );
    path_linux_free( src_p );
    return true;
}
b32 platform_file_delete( const Path in_path ) {
    struct LinuxPath path = path_to_linux( in_path );
    int result = unlink( path.cc );
    path_linux_free( path );

    return result == 0;
}
b32 platform_file_exists( const Path in_path ) {
    return platform_path_is_file( in_path );
}

attr_global Path global_working_directory = (Path){0};
Path platform_working_directory(void) {
    if( !global_working_directory.cc ) {
        usize max      = pathconf( ".", _PC_PATH_MAX );
        void* path_buf = memory_alloc( max );
        getcwd( path_buf, max );

        global_working_directory.v   = path_buf;
        global_working_directory.len = asciiz_len( (const char*)path_buf );
    }

    return global_working_directory;
}
b32 platform_directory_create( const Path in_path ) {
    struct LinuxPath path = path_to_linux( in_path );

    int res = mkdir( path.cc, S_IRWXU | S_IRWXO | S_IRWXG );

    path_linux_free( path );

    return res == 0;
}
b32 platform_directory_item_count( const Path in_path, usize* out_count ) {
    struct LinuxPath path = path_to_linux( in_path );

    DIR* d = opendir( path.cc );
    if( !d ) {
        return false;
    }

    usize count = 0;
    loop() {
        struct dirent* dr = readdir( d );
        if( !dr ) {
            break;
        }
        usize len = asciiz_len( dr->d_name );
        switch( len ) {
            case 1: {
                if( dr->d_name[0] == '.' ) {
                    continue;
                }
            } break;
            case 2: {
                if( dr->d_name[1] == '.' ) {
                    continue;
                }
            } break;
            default: break;
        }
        count++;
    }
    closedir( d );

    *out_count = count;

    path_linux_free( path );
    return true;
}
b32 platform_directory_delete( const Path in_path, b32 recursive ) {
    unused(in_path, recursive);
    return false;
}
b32 platform_directory_exists( const Path in_path ) {
    return platform_path_is_directory( in_path );
}

#undef to_fd

void platform_system_query_info( SystemInfo* out_info ) {
    unused(out_info);
}

#define CORE_INTERNAL_LINUX_MIN_LIBS (64)
struct LinuxLibStore {
    Hashmap hash;
    Mutex   mtx;
    b32     initialized;
};

attr_global struct LinuxLibStore global_linux_lib_store;
attr_internal void _init_store(void) {
    if( !global_linux_lib_store.initialized ) {
        assert( mutex_create( &global_linux_lib_store.mtx ) );
    }

    read_write_fence();

    mutex_lock( &global_linux_lib_store.mtx );

    usize items_size =
        sizeof( String ) *
        CORE_INTERNAL_LINUX_MIN_LIBS;
    usize keys_size =
        sizeof(void*) *
        CORE_INTERNAL_LINUX_MIN_LIBS;

    void* items_buffer = memory_alloc( items_size );
    assert( items_buffer );
    void* keys_buffer = memory_alloc( keys_size );
    assert( keys_buffer );

    global_linux_lib_store.hash = hashmap_new(
        sizeof(String), CORE_INTERNAL_LINUX_MIN_LIBS,
        items_buffer, keys_buffer );

    read_write_fence();
    mutex_unlock( &global_linux_lib_store.mtx );
}
attr_internal void _add_store( const char* name, void* handle ) {
    Hashmap* hash = &global_linux_lib_store.hash;
    u64 key = (u64)handle;
    if( hashmap_contains_key( hash, key ) ) {
        return;
    }

    read_write_fence();
    mutex_lock( &global_linux_lib_store.mtx );

    String name_s = string_from_asciiz( 0, name );
    void* name_buffer = memory_alloc( name_s.len + 1 );
    assert( name_buffer );
    memory_copy( name_buffer, name_s.cc, name_s.len );

    name_s.v = name_buffer;

    if( !hashmap_insert( hash, key, &name_s ) ) {
        usize old_size_key  = hash->cap * sizeof(void*);
        usize old_size_item = hash->cap * sizeof(String);

        usize new_size_key =
            (CORE_INTERNAL_LINUX_MIN_LIBS * sizeof(void*)) + old_size_key;
        usize new_size_item =
            (CORE_INTERNAL_LINUX_MIN_LIBS * sizeof(String)) + old_size_item;

        void* new_keys = memory_realloc( hash->keys, old_size_key, new_size_key );
        assert( new_keys );

        void* new_items = memory_realloc( hash->buf, old_size_item, new_size_item );
        assert( new_items );

        hash->cap  += CORE_INTERNAL_LINUX_MIN_LIBS;
        hash->keys = new_keys;
        hash->buf  = new_items;

        assert( hashmap_insert( hash, key, &name_s ) );
    }

    read_write_fence();
    mutex_unlock( &global_linux_lib_store.mtx );
}
attr_internal b32 _check_store_item( const char* name, void** out_key ) {
    String name_s = string_from_asciiz( 0, name );
    Hashmap* hash = &global_linux_lib_store.hash;
    if( !name_s.len ) {
        return false;
    }

    for( u32 i = 0; i < hash->cap; ++i ) {
        String* item = (String*)((u8*)hash->buf + (i * sizeof(String)));
        if( string_cmp( *item, name_s ) ) {
            *out_key = (void*)((u8*)hash->keys + (i * sizeof(u64)));
            return true;
        }
    }
    return false;
}
attr_internal void _remove_item( void* handle ) {
    u64 key = (u64)handle;
    Hashmap* hash = &global_linux_lib_store.hash;
    String* item = NULL;
    if( !hashmap_get( hash, key, &item ) ) {
        return;
    }

    read_write_fence();
    mutex_lock( &global_linux_lib_store.mtx );

    memory_free( item->v, item->len + 1 );
    hashmap_remove( hash, key, NULL );

    read_write_fence();
    mutex_unlock( &global_linux_lib_store.mtx );
}

void* platform_library_open( const char* name ) {
    _init_store();

    void* key = NULL;
    if( _check_store_item( name, &key ) ) {
        return key;
    }

    void* handle = dlopen( name, RTLD_LAZY );
    if( !handle ) {
        return NULL;
    }

    _add_store( name, handle );

    read_write_fence();
    return handle;
}
void* platform_library_get( const char* name ) {
    void* handle = NULL;
    if( _check_store_item( name, &handle ) ) {
        return handle;
    }

    return NULL;
}
void platform_library_close( void* lib ) {
    _remove_item( lib );
    dlclose( lib );
}
void* platform_library_load( void* lib, const char* function ) {
    return dlsym( lib, function );
}

#endif /* Platform Linux */

