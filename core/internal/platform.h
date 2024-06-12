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

void platform_query_timestamp( TimeStamp* out_timestamp );

f64 platform_query_milliseconds(void);
void platform_console_write( void* dst, usize len, const char* str );

struct Semaphore;
struct Mutex;

b32 platform_semaphore_create(
    const char* name, struct Semaphore* out_sem );
void platform_semaphore_destroy( struct Semaphore* sem );
void platform_semaphore_signal( struct Semaphore* sem );
b32 platform_semaphore_wait( struct Semaphore* sem, u32 ms );

b32 platform_mutex_create( const char* name, struct Mutex* out_mutex );
void platform_mutex_destroy( struct Mutex* mutex );
b32 platform_mutex_lock( struct Mutex* mutex, u32 ms );
void platform_mutex_unlock( struct Mutex* mutex );

void platform_sleep( u32 ms );

b32 platform_thread_create(
    ThreadMainFN* main, void* params, usize stack_size, void** handle );
void platform_thread_destroy( void* handle );
b32 platform_thread_exit_code( void* handle, int* out_exit_code );

b32 platform_path_is_file( const Path path );
b32 platform_path_is_directory( const Path path );

FileHandle* platform_file_open( const Path path, FileOpenFlags flags );
void platform_file_close( FileHandle* file );
usize platform_file_query_size( FileHandle* file );
void platform_file_truncate( FileHandle* file );
usize platform_file_query_offset( FileHandle* file );
void platform_file_set_offset( FileHandle* file, usize offset );
void platform_file_set_offset_relative( FileHandle* file, isize offset );
b32 platform_file_write( FileHandle* file, usize len, const void* buf );
b32 platform_file_read( FileHandle* file, usize len, void* buf );
b32 platform_file_copy(
    const Path dst, const Path src, b32 fail_if_dst_exists );
b32 platform_file_move(
    const Path dst, const Path src, b32 fail_if_dst_exists );
b32 platform_file_delete( const Path path );
b32 platform_file_exists( const Path path );
Path platform_working_directory(void);
b32 platform_directory_create( const Path path );
b32 platform_directory_item_count( const Path path, usize* out_count );
b32 platform_directory_exists( const Path path );

void platform_system_query_info( SystemInfo* out_info );

void* platform_library_open( const char* name );
void* platform_library_get( const char* name );
void platform_library_close( void* lib );
void* platform_library_load( void* lib, const char* function );

#if defined(CORE_PLATFORM_WINDOWS)
void* platform_win32_get_stdin(void);
void* platform_win32_get_stdout(void);
void* platform_win32_get_stderr(void);
#endif

#endif /* header guard */
