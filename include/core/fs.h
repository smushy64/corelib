#if !defined(CORE_FS_H)
#define CORE_FS_H
/**
 * @file   fs.h
 * @brief  File system functions.
 * @details
 *
 * When using functions that take a path,
 * it's always better to use a null-terminated path.
 *
 * If the path provided is not null-terminated,
 * the function has to copy the path to another buffer
 * (allocated at the start of the program by the run-time).
 *
 * On Windows Version 1903 or greater,
 * UTF-8 paths are used natively^.
 *
 * Versions older than 1903 always translate
 * paths to UCS-2 and canonicalize them.
 *
 * ^ - If corelib is compiled as a dynamic library. If compiled
 * as a static library, some extra work is required.
 * See BUILD.md for more info.
 *
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 14, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/path.h"   // IWYU pragma: export

/// @brief Maximum allowed path name length.
#define CORE_PATH_NAME_LEN (kibibytes(4))

/// @brief Size of file copy buffer.
#define CORE_FILE_COPY_BUFFER (kibibytes(1))

/// @brief Opaque file descriptor.
typedef struct FD {
#if defined(CORE_PLATFORM_POSIX)
    int opaque;
#else
    void* opaque;
#endif
} FD;
/// @brief Flags for opening a file.
typedef enum FileOpenFlags {
    /// @brief Open a file for reading.
    FOPEN_READ             = (1 << 0),
    /// @brief Open a file for writing.
    FOPEN_WRITE            = (1 << 1),
    /// @brief Open a file for reading and writing.
    FOPEN_READ_WRITE       = (FOPEN_READ | FOPEN_WRITE),
    /// @brief Share read access for file. Required for multi-threaded reading.
    FOPEN_SHARE_READ       = (1 << 2),
    /// @brief Share write access for file. Required for multi-threaded writing.
    FOPEN_SHARE_WRITE      = (1 << 3),
    /// @brief Share read and write access for file.
    /// Required for multi-threaded read/write.
    FOPEN_SHARE_READ_WRITE = (FOPEN_SHARE_READ | FOPEN_SHARE_WRITE),
    /// @brief Create file if it doesn't exist.
    FOPEN_CREATE           = (1 << 4),
    /// @brief Clear file when opened. File must exist.
    /// @details
    /// Cannot combine with #FOPEN_CREATE.
    /// #FOPEN_WRITE is required.
    FOPEN_TRUNCATE         = (1 << 5),
    /// @brief Seek to end of file when opened. File must exist.
    /// @details
    /// Cannot combine with #FOPEN_TRUNCATE or #FOPEN_CREATE.
    /// #FOPEN_WRITE is required.
    FOPEN_APPEND           = (1 << 6),
    /// @brief Create temporary file. File must not exist.
    /// @details
    /// Cannot combine with #FOPEN_CREATE, #FOPEN_TRUNCATE or #FOPEN_APPEND.
    FOPEN_TEMP             = (1 << 7),
} FileOpenFlags;
/// @brief File seek types.
typedef enum FileSeek {
    /// @brief Seek from current file offset.
    FSEEK_CURRENT,
    /// @brief Seek from start of file.
    FSEEK_SET,
    /// @brief Seek from end of file.
    FSEEK_END,
} FileSeek;
/// @brief Types of file.
typedef enum FileType {
    /// @brief File does not exist.
    FTYPE_NULL,
    /// @brief File path or descriptor points to a file.
    FTYPE_FILE,
    /// @brief File path or descriptor points to a directory.
    FTYPE_DIRECTORY,
    /// @brief File descriptor points to a directory.
    FTYPE_PIPE,
    /// @brief File exists but is not recognized by corelib.
    FTYPE_UNKNOWN,
} FileType;
/// @brief File permissions bitfield.
typedef enum FilePermissions {
    /// @brief Current user can read file.
    FPERM_READ    = (1 << 0),
    /// @brief Current user can write file.
    FPERM_WRITE   = (1 << 1),
    /// @brief Current user can execute file.
    FPERM_EXECUTE = (1 << 2),

    /// @brief File is readonly for current user.
    FPERM_READ_ONLY          = FPERM_READ,
    /// @brief File can be read or write for current user.
    FPERM_READ_WRITE         = (FPERM_READ | FPERM_WRITE),
    /// @brief File has all permissions set for current user.
    FPERM_READ_WRITE_EXECUTE = (FPERM_READ_WRITE | FPERM_EXECUTE)
} FilePermissions;
/// @brief Information about a file.
typedef struct FileInfo {
    /// @brief Type of file.
    FileType        type;
    /// @brief Current user's permissions regarding this file.
    FilePermissions permissions;
    /// @brief Time information about file.
    struct {
        /// @brief Time that file was created.
        TimePosix create;
        /// @brief Time that file was last modified.
        TimePosix modify;
    } time;
    /// @brief Size of file in bytes (only if it's a file).
    usize size;
} FileInfo;
/// @brief Control flow for directory walk.
typedef enum DirectoryWalkControl {
    /// @brief Directory walk should continue.
    DWC_CONTINUE,
    /// @brief Directory walk should stop.
    DWC_STOP,
    /// @brief If current is a directory,
    /// skip iterating its contents. Otherwise, same as DWC_CONTINUE.
    DWC_SKIP,
} DirectoryWalkControl;
/// @brief Directory walk current item info.
typedef struct DirectoryWalkInfo {
    /// @brief Pointer to start of current item's path.
    const char* path;
    /// @brief Length of current item's path.
    usize       path_len;
    /// @brief Offset to current item's name in path.
    usize       path_name_offset;

    /// @brief Type of current item.
    FileType type;
    /// @brief Size of current item.
    usize    size;
    /// @brief Number of subdirectories in. Starts at zero.
    usize    level;
} DirectoryWalkInfo;
/// @brief Callback function for directory walk.
/// @param[in] info   Pointer to information for current directory walk item.
/// @param[in] params (optional) Pointer to directory walk parameters.
/// @return Control flow. @see DirectoryWalkControl.
typedef DirectoryWalkControl DirectoryWalkFN( const DirectoryWalkInfo* info, void* params );

/// @brief Convert file type to string.
/// @param ft File type.
/// @return String.
attr_always_inline attr_header
struct _StringPOD file_type_to_string( FileType ft );

/// @brief Get path from directory walk info.
/// @param[in] walk_info (const DirectoryWalkInfo*) Pointer to directory walk.
/// @return (Path) Path to current item.
#define directory_walk_info_path( walk_info ) \
    string_new( (walk_info)->path_len, (walk_info)->path )
/// @brief Get file name from directory walk info.
/// @param[in] walk_info (const DirectoryWalkInfo*) Pointer to directory walk.
/// @return (Path) Name of current item.
#define directory_walk_info_file_name( walk_info ) \
    string_new( \
        (walk_info)->path_len - (walk_info)->path_name_offset, \
        (walk_info)->path + (walk_info)->path_name_offset )

/// @brief Copy contents of source file to destination file.
/// @param dst        Path to destination file. 
/// @param src        Path to source file. 
/// @param create_dst Fail if destination already exists.
/// @return
///     - @c true  : Successfully copied source contents to destination.
///     - @c false : Failed to copy.
///     - @c false : If @c create_dst, @c dst already exists.
attr_core_api
b32 file_copy_by_path( struct _StringPOD dst, struct _StringPOD src, b32 create_dst );
/// @brief Move contents of source file to destination file.
/// @param dst        Path to destination file. 
/// @param src        Path to source file. 
/// @param create_dst Fail if destination already exists.
/// @return
///     - @c true  : Successfully moved source to destination.
///     - @c false : Failed to move.
///     - @c false : If @c create_dst, @c dst already exists.
attr_core_api
b32 file_move_by_path( struct _StringPOD dst, struct _StringPOD src, b32 create_dst );
/// @brief Remove file at path.
/// @param path Path of file to remove. 
/// @return
///     - @c true  : Removed file successfully.
///     - @c false : Failed to remove file.
attr_core_api
b32 file_remove_by_path( struct _StringPOD path );
/// @brief Check if file exists by path.
/// @param path Path to file.
/// @return
///     - @c true  : File exists (file or directory).
///     - @c false : File does not exist.
attr_core_api
b32 file_exists_by_path( struct _StringPOD path );
/// @brief Query info about a file.
/// @param      path     Path to file.
/// @param[out] out_info Pointer to file info struct to fill out.
/// @return
///     - true  : Successfully obtained information about the specified file.
///     - false : Failed to obtain file information.
attr_core_api
b32 file_query_info_by_path( struct _StringPOD path, FileInfo* out_info );
/// @brief Query file descriptor file type by path.
/// @note
/// This is the same as calling file_query_info_by_path() and only reading
/// the type field.
/// @param path Path to file.
/// @return Type of file.
attr_core_api
FileType file_query_type_by_path( struct _StringPOD path );
/// @brief Query when file was created by path.
/// @note
/// This is the same as calling file_query_info_by_path() and only reading
/// the time.create field.
/// @param path Path to file.
/// @return Time created.
attr_core_api
TimePosix file_query_time_create_by_path( struct _StringPOD path );
/// @brief Query when file was last modified by path.
/// @note
/// This is the same as calling file_query_info_by_path() and only reading
/// the time.modify field.
/// @param path Path to file.
/// @return Time modified.
attr_core_api
TimePosix file_query_time_modify_by_path( struct _StringPOD path );

/// @brief Open file descriptor.
/// @param      path   Path to file.
/// @param      flags  Flags for opening file.
/// @param[out] out_fd Pointer to write file descriptor to.
/// @return
///     - @c true  : File was opened successfully.
///     - @c false : Failed to open file.
attr_core_api
b32 file_open( struct _StringPOD path, FileOpenFlags flags, FD* out_fd );
/// @brief Close file descriptor.
/// @param[in] fd File descriptor to close.
attr_core_api
void file_close( FD* fd );
/// @brief Query info about a file.
/// @param[in]  fd       Pointer to file descriptor.
/// @param[out] out_info Pointer to file info struct to fill out.
/// @return
///     - true  : Successfully obtained information about the specified file.
///     - false : Failed to obtain file information.
attr_core_api
b32 file_query_info( FD* fd, FileInfo* out_info );
/// @brief Query file descriptor file type.
/// @note
/// This is the same as calling file_query_info() and
/// reading only the type field.
/// @param[in] fd Pointer to file descriptor.
/// @return Type of file descriptor.
attr_core_api
FileType file_query_type( FD* fd );
/// @brief Query when file was created.
/// @note
/// This is the same as calling file_query_info() and only reading
/// the time.create field.
/// @param[in] fd Pointer to file descriptor.
/// @return Time created.
attr_core_api
TimePosix file_query_time_create( FD* fd );
/// @brief Query when file was last modified.
/// @note
/// This is the same as calling file_query_info() and only reading
/// the time.modify field.
/// @param[in] fd Pointer to file descriptor.
/// @return Time modified.
attr_core_api
TimePosix file_query_time_modify( FD* fd );
/// @brief Query size of file by file descriptor.
/// @note
/// On some platforms, this may be faster than calling
/// file_query_info().
/// @param[in] fd File descriptor.
/// @return Size of file in bytes.
attr_core_api
usize file_query_size( FD* fd );
/// @brief Query current file offset.
/// @param[in] fd File descriptor.
/// @return Current offset (from start of file).
attr_core_api
usize file_query_offset( FD* fd );
/// @brief Truncate file size to current file offset.
/// @param[in] fd File descriptor.
attr_core_api
void file_truncate( FD* fd );
/// @brief Seek to offset in file.
/// @param[in] fd   File descriptor.
/// @param     type Type of seek to perform.
/// @param     seek Bytes to seek by.
/// @return Offset after seek.
attr_core_api
usize file_seek( FD* fd, FileSeek type, isize seek );
/// @brief Write bytes to file.
/// @param[in]  fd            File descriptor.
/// @param      bytes         Number of bytes in @c buf.
/// @param[in]  buf           Pointer to start of buffer to write.
/// @param[out] opt_out_write (optional) Pointer to write bytes written to file.
/// @return
///     - @c true  : Wrote to file successfully.
///     - @c false : Failed to write to file.
attr_core_api
b32 file_write(
    FD* fd, usize bytes, const void* buf, usize* opt_out_write );
/// @brief Read bytes from file.
/// @param[in]  fd           File descriptor.
/// @param      buf_size     Number of bytes in @c buf.
/// @param[in]  buf          Pointer to start of buffer to read to.
/// @param[out] opt_out_read (optional) Pointer to write bytes read from file.
/// @return
///     - @c true  : Read from file successfully.
///     - @c false : Failed to read from file.
attr_core_api
b32 file_read(
    FD* fd, usize buf_size, void* buf, usize* opt_out_read );
/// @brief Write formatted string to file.
/// @param[in] fd     File descriptor.
/// @param     format Format string.
/// @param[in] va     Variadic arguments.
/// @return Number of bytes that could not be written to file.
attr_core_api
usize file_write_fmt_va( FD* fd, struct _StringPOD format, va_list va );
/// @brief Write formatted string to file.
/// @param[in] fd     File descriptor.
/// @param     format Format string.
/// @param[in] ...    Arguments.
/// @return Number of bytes that could not be written to file.
attr_core_api
usize file_write_fmt( FD* fd, struct _StringPOD format, ... );
/// @brief Write formatted string to file.
/// @param[in] fd     File descriptor.
/// @param     format Format string.
/// @param[in] va     Variadic arguments.
/// @return Number of bytes that could not be written to file.
#define file_write_fmt_va_text( fd, format, va ) \
    file_write_fmt_va( fd, string_new( format ), va )
/// @brief Write formatted string to file.
/// @param[in] fd     File descriptor.
/// @param     format Format string.
/// @param[in] ...    Arguments.
/// @return Number of bytes that could not be written to file.
#define file_write_fmt_text( fd, format, ... ) \
    file_write_fmt( fd, string_new( format ), ##__VA_ARGS__ )
/// @brief File streaming function.
/// @param[in] struct_FD (FD*) File to stream to. Must have write access.
/// @param     count     Number of bytes to write.
/// @param[in] buf       Pointer to start of buffer to write.
/// @return Number of bytes not written to file.
attr_core_api
usize file_stream_write( void* struct_FD, usize count, const void* buf );

/// @brief Create a directory at given path.
/// @details
/// If directory already exists, returns true.
/// @param path Path of new directory.
/// @return
///     - @c true  : Successfully created directory.
///     - @c false : Failed to create directory.
attr_core_api
b32 directory_create( struct _StringPOD path );
/// @brief Remove directory from file system.
/// @param path      Path of directory to remove.
/// @param recursive If false, directory must be empty.
/// @return
///     - @c true  : Successfully removed directory.
///     - @c false : Failed to remove directory.
attr_core_api
b32 directory_remove( struct _StringPOD path, b32 recursive );
/// @brief Walk directory and call callback for each item in directory.
/// @param     path     Path to directory to walk.
/// @param[in] callback Callback function to call for each item.
/// @param[in] params   (optional) Additional parameters for callback function.
/// @return
///     - true  : Successfully walked directory.
///     - false : Failed to walk directory.
attr_core_api
b32 directory_walk( struct _StringPOD path, DirectoryWalkFN* callback, void* params );

/// @brief Get read-only current working directory.
/// @return Current working directory.
attr_core_api
struct _StringPOD directory_current_query(void);
/// @brief Set current working directory.
/// @warning
/// This function is not thread safe! Make sure to only call it
/// when no other threads are using any file system functions!
/// @param path Path to set current working directory to.
/// @return
///     - @c true  : Set current working directory successfully.
///     - @c false : Failed to set current working directory.
attr_core_api
b32 directory_current_set( struct _StringPOD path );

/// @brief Get standard in pipe.
/// @return Standard in pipe.
attr_core_api
FD* pipe_stdin(void);
/// @brief Get standard out pipe.
/// @return Standard out pipe.
attr_core_api
FD* pipe_stdout(void);
/// @brief Get standard error pipe.
/// @return Standard error pipe.
attr_core_api
FD* pipe_stderr(void);

/// @brief Open read and write pipes.
/// @param[out] out_read Pointer to write read pipe.
/// @param[out] out_write Pointer to write write pipe.
/// @return
///     - @c true  : Opened pipes successfully.
///     - @c false : Failed to open pipes.
attr_core_api
b32 pipe_open( FD* out_read, FD* out_write );
/// @brief Close a pipe.
/// @param[in] pipe Pointer to pipe to close.
attr_core_api
void pipe_close( FD* pipe );

// NOTE(alicia): implementation -----------------------------------------------

#include "core/print.h"

attr_always_inline attr_header
struct _StringPOD file_type_to_string( FileType ft ) {
    switch( ft ) {
        case FTYPE_NULL:      return string_text( "NULL" );
        case FTYPE_FILE:      return string_text( "File" );
        case FTYPE_DIRECTORY: return string_text( "Directory" );
        case FTYPE_PIPE:      return string_text( "Pipe" );
        case FTYPE_UNKNOWN:   return string_text( "Unknown" );
    }
    return string_text("");
}

#if !defined(CORE_CPP_FS_HPP)
    #include "core/cpp/fs.hpp"
#endif

#endif /* header guard */
