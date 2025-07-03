#if !defined(CORE_INTERNAL_PLATFORM_FS_H) && defined(CORE_ENABLE_INTERNAL)
#define CORE_INTERNAL_PLATFORM_FS_H
/**
 * @file   fs.h
 * @brief  Platform file system functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 06, 2025
*/
#include "core/types.h"

struct _StringPOD;
struct FD;
enum   FileOpenFlags;
enum   FileSeek;
enum   FileType;
enum   FilePermissions;
struct FileInfo;
enum   DirectoryWalkControl;
struct DirectoryWalkInfo;

b32 platform_file_remove_by_path( struct _StringPOD path );
b32 platform_file_query_info_by_path( struct _StringPOD path, struct FileInfo* out_info );
b32 platform_file_exists_by_path( struct _StringPOD path );
enum FileType platform_file_query_type_by_path( struct _StringPOD path );
TimePosix platform_file_query_time_create_by_path( struct _StringPOD path );
TimePosix platform_file_query_time_modify_by_path( struct _StringPOD path );

b32 platform_file_open( struct _StringPOD path, enum FileOpenFlags flags, struct FD* out_fd );
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

b32 platform_directory_create( struct _StringPOD path );
b32 platform_directory_remove( struct _StringPOD path, b32 recursive );
b32 platform_directory_walk(
    struct _StringPOD path,
    enum DirectoryWalkControl (callback)( const struct DirectoryWalkInfo* info, void* params ),
    void* params );
struct _StringPOD platform_directory_current_query(void);
b32 platform_directory_current_set( struct _StringPOD path );

struct FD* platform_pipe_stdin(void);
struct FD* platform_pipe_stdout(void);
struct FD* platform_pipe_stderr(void);

b32 platform_pipe_open( struct FD* out_read, struct FD* out_write );
void platform_pipe_close( const struct FD* pipe );

#endif /* header guard */
