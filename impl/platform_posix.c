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

#include <unistd.h>
#include <sys/mman.h>

attr_global PipeRead  global_posix_stdin_fd  = (PipeRead){.fd=(FD){0}};
attr_global PipeWrite global_posix_stdout_fd = (PipeWrite){.fd=(FD){1}};
attr_global PipeWrite global_posix_stderr_fd = (PipeWrite){.fd=(FD){2}};

void* platform_heap_alloc( const usize size ) {
    void* result = mmap(
        NULL, size, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANON, -1, 0 );
    return result;
}
void* platform_heap_realloc(
    void* old_buffer, const usize old_size, const usize new_size
) {
    void* result = platform_heap_alloc( new_size );
    if( result ) {
        memory_copy( result, old_buffer, old_size );
        platform_heap_free( old_buffer, old_size );
    }
    return result;
}
void platform_heap_free( void* buffer, const usize size ) {
    munmap( buffer, size );
}

TimePosix platform_time_posix(void) {
    // TODO(alicia): 
    return 0;
}
TimeSplit platform_time_split(void) {
    // TODO(alicia): 
    return (TimeSplit){0};
}
f64 platform_timer_milliseconds(void) {
    // TODO(alicia): 
    return 0;
}
f64 platform_timer_seconds(void) {
    // TODO(alicia): 
    return 0;
}

b32 platform_semaphore_create(
    const char* name, struct Semaphore* out_sem
) {
    // TODO(alicia): 
    unused(name,out_sem);
    return false;
}
void platform_semaphore_destroy( struct Semaphore* sem ) {
    // TODO(alicia): 
    unused(sem);
}
void platform_semaphore_signal( struct Semaphore* sem ) {
    // TODO(alicia): 
    unused(sem);
}
b32 platform_semaphore_wait( struct Semaphore* sem, u32 ms ) {
    // TODO(alicia): 
    unused(sem,ms);
    return false;
}

b32 platform_mutex_create( const char* name, struct Mutex* out_mutex ) {
    // TODO(alicia): 
    unused(name,out_mutex);
    return false;
}
void platform_mutex_destroy( struct Mutex* mutex ) {
    // TODO(alicia): 
    unused(mutex);
}
b32 platform_mutex_lock( struct Mutex* mutex, u32 ms ) {
    // TODO(alicia): 
    unused(mutex,ms);
    return false;
}
void platform_mutex_unlock( struct Mutex* mutex ) {
    // TODO(alicia): 
    unused(mutex);
}

void platform_sleep( u32 ms ) {
    // TODO(alicia): 
    unused(ms);
}

b32 platform_thread_create(
    ThreadMainFN* main, void* params,
    usize stack_size, ThreadHandle* out_handle 
) {
    // TODO(alicia): 
    unused(main,params,stack_size,out_handle);
    return false;
}
void platform_thread_destroy( ThreadHandle* handle ) {
    // TODO(alicia): 
    unused(handle);
}
void platform_thread_free( ThreadHandle* handle ) {
    // TODO(alicia): 
    unused(handle);
}
b32 platform_thread_join_timed(
    ThreadHandle* handle, u32 ms, int* opt_out_exit_code
) {
    // TODO(alicia): 
    unused(handle,ms,opt_out_exit_code);
    return false;
}
b32 platform_thread_exit_code( ThreadHandle* handle, int* out_exit_code ) {
    // TODO(alicia): 
    unused(handle,out_exit_code);
    return false;
}

b32 platform_fd_open( Path path, FileOpenFlags flags, FD* out_fd ) {
    // TODO(alicia): 
    unused(path,flags,out_fd);
    return false;
}
void platform_fd_close( FD* fd ) {
    close(fd->opaque);
    memory_zero( fd, sizeof(*fd) );
}
usize platform_fd_query_size( FD* fd ) {
    // TODO(alicia): 
    unused(fd);
    return 0;
}
void platform_fd_truncate( FD* fd ) {
    // TODO(alicia): 
    unused(fd);
}
usize platform_fd_seek( FD* fd, FileSeek type, isize seek ) {
    // TODO(alicia): 
    unused(fd,type,seek);
    return 0;
}
b32 platform_fd_write(
    FD* fd, usize bytes, const void* buf, usize* opt_out_write
) {
    ssize_t result = write( fd->opaque, buf, bytes );
    if( result < 0 ) {
        return false;
    }
    if( opt_out_write ) {
        *opt_out_write = result;
    }
    return true;
}
b32 platform_fd_read(
    FD* fd, usize buf_size, void* buf, usize* opt_out_read
) {
    // TODO(alicia): 
    unused(fd,buf_size,buf,opt_out_read);
    return false;
}
b32 platform_file_copy( Path dst, Path src, b32 create_dst ) {
    // TODO(alicia): 
    unused(dst,src,create_dst);
    return false;
}
b32 platform_file_move( Path dst, Path src, b32 create_dst ) {
    // TODO(alicia): 
    unused(dst,src,create_dst);
    return false;
}
b32 platform_file_remove( Path path ) {
    // TODO(alicia): 
    unused(path);
    return false;
}
b32 platform_file_exists( Path path ) {
    // TODO(alicia): 
    unused(path);
    return false;
}

b32 platform_directory_create( Path path ) {
    // TODO(alicia): 
    unused(path);
    return false;
}
b32 platform_directory_remove( Path path, b32 recursive ) {
    // TODO(alicia): 
    unused(path,recursive);
    return false;
}
b32 platform_directory_exists( Path path ) {
    // TODO(alicia): 
    unused(path);
    return false;
}
b32 platform_directory_is_empty( Path path, b32* out_found ) {
    // TODO(alicia): 
    unused(path,out_found);
    return false;
}

DirectoryWalk* platform_directory_walk_begin(
    Path path, struct AllocatorInterface* allocator 
) {
    // TODO(alicia): 
    unused(path,allocator);
    return NULL;
}
b32 platform_directory_walk_next(
    DirectoryWalk* walk, Path* out_path, b32* opt_out_is_directory 
) {
    // TODO(alicia): 
    unused(walk,out_path,opt_out_is_directory);
    return false;
}
void platform_directory_walk_end(
    DirectoryWalk* walk, struct AllocatorInterface* allocator
) {
    // TODO(alicia): 
    unused(walk,allocator);
}

b32 platform_pipe_open( PipeRead* out_read, PipeWrite* out_write ) {
    int fd[2];
    int result = pipe( fd ); 

    if( result ) {
        core_error( "posix: failed to open pipes!" );
        return false;
    }

    if( out_read ) {
        out_read->fd.opaque = fd[0];
    }
    if( out_write ) {
        out_write->fd.opaque = fd[1];
    }
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

void platform_system_query_info( SystemInfo* out_info ) {
    // TODO(alicia): 
    unused(out_info);
}

void* platform_library_open( const char* name ) {
    // TODO(alicia): 
    unused(name);
    return NULL;
}
void* platform_library_get( const char* name ) {
    // TODO(alicia): 
    unused(name);
    return NULL;
}
void platform_library_close( void* lib ) {
    // TODO(alicia): 
    unused(lib);
}
void* platform_library_load( void* lib, const char* function ) {
    // TODO(alicia): 
    unused(lib,function);
    return NULL;
}

#endif /* Platform POSIX*/

