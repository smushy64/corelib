#if !defined(CORE_INTERNAL_PLATFORM_H)
#define CORE_INTERNAL_PLATFORM_H
/**
 * Description:  Platform functions.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 08, 2024
*/
#include "core/types.h"
#include "core/time.h"
#include "core/thread.h"
#include "core/path.h"
#include "core/fs.h"
#include "core/system.h"

void* platform_heap_alloc( const usize size );
void* platform_heap_realloc(
    void* old_buffer, const usize old_size, const usize new_size );
void platform_heap_free( void* buffer, const usize size );

TimePosix platform_time_posix(void);
TimeSplit platform_time_split(void);
f64 platform_timer_milliseconds(void);
f64 platform_timer_seconds(void);

struct NamedSemaphore;
struct OSMutex;

b32 platform_semaphore_create(
    const char* name, u32 initial_value, struct NamedSemaphore* out_sem );
void platform_semaphore_destroy( struct NamedSemaphore* sem );
void platform_semaphore_signal( struct NamedSemaphore* sem );
b32 platform_semaphore_wait( struct NamedSemaphore* sem, u32 ms );

b32 platform_mutex_create( struct OSMutex* out_mutex );
void platform_mutex_destroy( struct OSMutex* mutex );
b32 platform_mutex_lock( struct OSMutex* mutex, u32 ms );
void platform_mutex_unlock( struct OSMutex* mutex );

void platform_sleep( u32 ms );

void platform_yield(void);

b32 platform_thread_create(
    ThreadMainFN* main, void* params,
    usize stack_size, ThreadHandle* out_handle );
void platform_thread_destroy( ThreadHandle* handle );
u32 platform_thread_query_id(void);
void platform_thread_free( ThreadHandle* handle );
b32 platform_thread_join_timed( ThreadHandle* handle, u32 ms, int* opt_out_exit_code );
b32 platform_thread_exit_code( ThreadHandle* handle, int* out_exit_code );

b32 platform_fd_open( Path path, FileOpenFlags flags, FD* out_fd );
void platform_fd_close( FD* fd );
usize platform_fd_query_size( FD* fd );
void platform_fd_truncate( FD* fd );
usize platform_fd_seek( FD* fd, FileSeek type, isize seek );
b32 platform_fd_write(
    FD* fd, usize bytes, const void* buf, usize* opt_out_write );
b32 platform_fd_read(
    FD* fd, usize buf_size, void* buf, usize* opt_out_read );
b32 platform_file_copy( Path dst, Path src, b32 create_dst );
b32 platform_file_move( Path dst, Path src, b32 create_dst );
b32 platform_file_remove( Path path );
b32 platform_file_exists( Path path );

b32 platform_directory_create( Path path );
b32 platform_directory_remove( Path path );
b32 platform_directory_exists( Path path );
Path platform_directory_query_cwd(void);
b32 platform_directory_set_cwd( Path path );

DirectoryWalk* platform_directory_walk_begin(
    Path path, struct AllocatorInterface* allocator );
b32 platform_directory_walk_next(
    DirectoryWalk* walk, Path* out_path, b32* opt_out_is_directory );
void platform_directory_walk_end(
    DirectoryWalk* walk, struct AllocatorInterface* allocator );

b32 platform_pipe_open( PipeRead* out_read, PipeWrite* out_write );

PipeRead*  platform_stdin(void);
PipeWrite* platform_stdout(void);
PipeWrite* platform_stderr(void);

void platform_system_query_info( SystemInfo* out_info );

void* platform_library_open( const char* name );
void* platform_library_get( const char* name );
void platform_library_close( void* lib );
void* platform_library_load( void* lib, const char* function );

usize platform_path_stream_canonicalize(
    StreamBytesFN* stream, void* target, Path path );

#if defined(CORE_PLATFORM_WINDOWS)
usize platform_path_stream_canonicalize_utf8(
    StreamBytesFN* stream, void* target, Path path );
#endif

#endif /* header guard */
