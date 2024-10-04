#if !defined(CORE_FILESYSTEM_H)
#define CORE_FILESYSTEM_H
/**
 * @file   fs.h
 * @brief  File system functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 14, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/path.h"
#include "core/macros.h" // IWYU pragma: export
#include "core/cpp/enum.hpp"

// forward declarations
struct AllocatorInterface;

/// @brief Maximum allowed path name.
#define CORE_MAX_PATH_NAME (kibibytes(4))
/// @brief Maximum allowed pipe name.
#define CORE_MAX_PIPE_NAME (128)

/// @brief Opaque file descriptor.
typedef struct FD {
#if defined(CORE_PLATFORM_POSIX)
    int opaque;
#else
    void* opaque;
#endif
} FD;
/// @brief Opaque directory walk structure.
/// @details Defined in platform code.
struct DirectoryWalk;
/// @brief Opaque directory walk structure.
/// @details Defined in platform code.
typedef struct DirectoryWalk DirectoryWalk;
/// @brief Pipe used exclusively for reading.
typedef struct PipeRead {
    /// @brief Pipe file descriptor.
    FD fd;
} PipeRead;
/// @brief Pipe used exclusively for writing.
typedef struct PipeWrite {
    /// @brief Pipe file descriptor.
    FD fd;
} PipeWrite;
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
define_bitwise_operators( enum FileOpenFlags, unsigned int );
/// @brief File seek types.
typedef enum FileSeek {
    /// @brief Seek from current file offset.
    FSEEK_CURRENT,
    /// @brief Seek from start of file.
    FSEEK_SET,
    /// @brief Seek from end of file.
    FSEEK_END,
} FileSeek;
/// @brief Open file descriptor.
/// @note
/// Allocates wide path buffer on Windows if @c path is longer than 260 characters.
/// @param      path   Path to file. Must be null-terminated and less than #CORE_MAX_PATH_NAME.
/// @param      flags  Flags for opening file.
/// @param[out] out_fd Pointer to write file descriptor to.
/// @return
///     - @c true  : File was opened successfully.
///     - @c false : Failed to open file.
attr_core_api b32 fd_open( Path path, FileOpenFlags flags, FD* out_fd );
/// @brief Close file descriptor.
/// @param[in] fd File descriptor to close.
attr_core_api void fd_close( FD* fd );
/// @brief Query size of file by file descriptor.
/// @param[in] fd File descriptor.
/// @return Size of file in bytes.
attr_core_api usize fd_query_size( FD* fd );
/// @brief Truncate file size to current file offset.
/// @param[in] fd File descriptor.
attr_core_api void fd_truncate( FD* fd );
/// @brief Seek to offset in file.
/// @param[in] fd   File descriptor.
/// @param     type Type of seek to perform.
/// @param     seek Bytes to seek by.
/// @return Offset after seek.
attr_core_api usize fd_seek( FD* fd, FileSeek type, isize seek );
/// @brief Query current file offset.
/// @param[in] fd File descriptor.
/// @return Current offset (from start of file).
attr_header usize fd_query_offset( FD* fd ) {
    return fd_seek( fd, FSEEK_CURRENT, 0 );
}
/// @brief Write bytes to file.
/// @param[in]  fd            File descriptor.
/// @param      bytes         Number of bytes in @c buf.
/// @param[in]  buf           Pointer to start of buffer to write.
/// @param[out] opt_out_write (optional) Pointer to write bytes written to file.
/// @return
///     - @c true  : Wrote to file successfully.
///     - @c false : Failed to write to file.
attr_core_api b32 fd_write(
    FD* fd, usize bytes, const void* buf, usize* opt_out_write );
/// @brief Read bytes from file.
/// @param[in]  fd           File descriptor.
/// @param      buf_size     Number of bytes in @c buf.
/// @param[in]  buf          Pointer to start of buffer to read to.
/// @param[out] opt_out_read (optional) Pointer to write bytes read from file.
/// @return
///     - @c true  : Read from file successfully.
///     - @c false : Failed to read from file.
attr_core_api b32 fd_read(
    FD* fd, usize buf_size, void* buf, usize* opt_out_read );
/// @brief Write formatted string to file.
/// @param[in]  fd            File descriptor.
/// @param[out] opt_out_write (optional) Pointer to write bytes written to file.
/// @param      format_len    Length of format string.
/// @param[in]  format        Pointer to start of format string.
/// @param      va            Variadic format arguments.
/// @return
///     - @c true  : Wrote entire formatted string to file successfully.
///     - @c false : Failed to write to file.
attr_core_api b32 fd_write_fmt_va(
    FD* fd, usize* opt_out_write, usize format_len, const char* format, va_list va );
/// @brief Write formatted string to file.
/// @param[in]  fd            File descriptor.
/// @param[out] opt_out_write (optional) Pointer to write bytes written to file.
/// @param      format_len    Length of format string.
/// @param[in]  format        Pointer to start of format string.
/// @param      ...           Format arguments.
/// @return
///     - @c true  : Wrote entire formatted string to file successfully.
///     - @c false : Failed to write to file.
attr_always_inline
attr_header b32 fd_write_fmt(
    FD* fd, usize* opt_out_write,
    usize format_len, const char* format, ...
) {
    va_list va;
    va_start( va, format );
    b32 res = fd_write_fmt_va( fd, opt_out_write, format_len, format, va );
    va_end( va );
    return res;
}
/// @brief Write formatted string to file.
/// @param[in]  fd            (FD*)            File descriptor.
/// @param[out] opt_out_write (usize*)         (optional) Pointer to write bytes written to file.
/// @param[in]  format        (string literal) Format string literal.
/// @param      va            (va_list)        Variadic format arguments.
/// @return
///     - @c true  : Wrote entire formatted string to file successfully.
///     - @c false : Failed to write to file.
#define fd_write_text_va( fd, opt_out_write, format, va )\
    fd_write_fmt_va( fd, opt_out_write, sizeof(format) - 1, format, va )
/// @brief Write formatted string to file.
/// @param[in]  fd            (FD*)            File descriptor.
/// @param[out] opt_out_write (usize*)         (optional) Pointer to write bytes written to file.
/// @param[in]  format        (string literal) Format string literal.
/// @param      ...           (args)           Format arguments.
/// @return
///     - @c true  : Wrote entire formatted string to file successfully.
///     - @c false : Failed to write to file.
#define fd_write_text( fd, opt_out_write, format, ... )\
    fd_write_fmt( fd, opt_out_write, sizeof(format) - 1, format, ##__VA_ARGS__ )
/// @brief File streaming function.
/// @param[in] struct_FD (FD*) File to stream to. Must have write access.
/// @param     count     Number of bytes to write.
/// @param[in] buf       Pointer to start of buffer to write.
/// @return Number of bytes not written to file.
attr_core_api usize fd_stream_write( void* struct_FD, usize count, const void* buf );
/// @brief Copy contents of source file to destination file.
/// @note
/// Allocates wide path buffer on Windows if @c dst or @c src
/// is longer than 260 characters.
/// @param dst        Path to destination file. Must be null terminated.
/// @param src        Path to source file. Must be null terminated.
/// @param create_dst Fail if destination already exists.
/// @return
///     - @c true  : Successfully copied source contents to destination.
///     - @c false : Failed to copy.
///     - @c false : If @c create_dst, @c dst already exists.
attr_core_api b32 file_copy( Path dst, Path src, b32 create_dst );
/// @brief Move contents of source file to destination file.
/// @note
/// Allocates wide path buffer on Windows if @c dst or @c src
/// is longer than 260 characters.
/// @param dst        Path to destination file. Must be null terminated.
/// @param src        Path to source file. Must be null terminated.
/// @param create_dst Fail if destination already exists.
/// @return
///     - @c true  : Successfully moved source to destination.
///     - @c false : Failed to move.
///     - @c false : If @c create_dst, @c dst already exists.
attr_core_api b32 file_move( Path dst, Path src, b32 create_dst );
/// @brief Remove file at path.
/// @note
/// Allocates wide path buffer on Windows if @c path is longer than 260 characters.
/// @param path Path of file to remove. Must be null terminated.
/// @return
///     - @c true  : Removed file successfully.
///     - @c false : Failed to remove file.
attr_core_api b32 file_remove( Path path );
/// @brief Check if path points to an existing file.
/// @note
/// Allocates wide path buffer on Windows if @c path is longer than 260 characters.
/// @param path Path of file to check. Must be null terminated.
/// @return
///     - @c true  : File at given path exists.
///     - @c false : File does not exists or path does not point to a file.
attr_core_api b32 file_exists( Path path );

/// @brief Create a directory at given path.
/// @details
/// If directory already exists, returns true.
/// @note
/// Allocates wide path buffer on Windows if @c path is longer than 260 characters.
/// @param path Path of new directory.
/// @return
///     - @c true  : Successfully created directory.
///     - @c false : Failed to create directory.
attr_core_api b32 directory_create( Path path );
/// @brief Remove directory from file system.
/// @note
/// Allocates wide path buffer on Windows if @c path is longer than 260 characters.
/// @param path      Path of directory to remove.
/// @param recursive Remove all files and directories in @c path.
/// @return
///     - @c true  : Successfully removed directory.
///     - @c false : Failed to remove directory.
///     - @c false : @c recursive is false and directory is not empty.
attr_core_api b32 directory_remove( Path path );
/// @brief Remove directory recursively.
/// @note
/// Allocates DirectoryWalk in order to recursively delete every item in directory.
/// @param     path      Path of directory to remove.
/// @param[in] allocator Pointer to allocator interface.
attr_core_api b32 directory_remove_recursive(
    Path path, struct AllocatorInterface* allocator );
/// @brief Check if directory exists.
/// @note
/// Allocates wide path buffer on Windows if @c path is longer than 260 characters.
/// @param path Path of directory to check for.
/// @return
///     - @c true  : Directory exists.
///     - @c false : Directory does not exist or path does not point to a directory.
attr_core_api b32 directory_exists( Path path );
/// @brief Begin directory walk.
/// @param     path      Path of directory to walk. Must be null-terminated.
/// @param[in] allocator Pointer to allocator interface.
/// @return
///     - NULL    : Could not allocate directory walk.
///     - Pointer : Directory walk structure.
attr_core_api DirectoryWalk* directory_walk_begin(
    Path path, struct AllocatorInterface* allocator );
/// @brief Get next item in directory.
/// @note
/// @c out_path is invalidated next time directory_walk_next() is called or
/// if directory_walk_end() is called.
/// @param[in]  walk                 Directory walk structure.
/// @param[out] out_path             Pointer to write path to. Does not include directory.
/// @param[out] opt_out_is_directory (optional) Pointer to write if path is a directory.
/// @return
///     - @c true  : Path written to @c out_path.
///     - @c false : There are no more items in directory.
attr_core_api b32 directory_walk_next(
    DirectoryWalk* walk, Path* out_path, b32* opt_out_is_directory );
/// @brief End directory walk.
/// @param[in] walk      Pointer to directory walk structure.
/// @param[in] allocator Pointer to allocator interface.
attr_core_api void directory_walk_end(
    DirectoryWalk* walk, struct AllocatorInterface* allocator );
/// @brief Get read-only current working directory.
/// @return Current working directory.
attr_core_api Path directory_query_cwd(void);
/// @brief Set current working directory.
/// @warning
/// This function is not thread safe! Make sure to only call it
/// when no other threads are using any file system functions!
/// @param path Path to set current working directory to.
/// @return
///     - @c true  : Set current working directory successfully.
///     - @c false : Failed to set current working directory.
attr_core_api b32 directory_set_cwd( Path path );

/// @brief Get standard in pipe.
/// @return Standard in pipe.
attr_core_api PipeRead*  pipe_stdin(void);
/// @brief Get standard out pipe.
/// @return Standard out pipe.
attr_core_api PipeWrite* pipe_stdout(void);
/// @brief Get standard error pipe.
/// @return Standard error pipe.
attr_core_api PipeWrite* pipe_stderr(void);

/// @brief Get void read pipe.
/// @details
/// Useful for when a pipe that doesn't lead anywhere is necessary.
/// @return Read pipe.
attr_core_api PipeRead  pipe_read_void(void);
/// @brief Get void write pipe.
/// @details
/// Useful for when a pipe that doesn't lead anywhere is necessary.
/// @return Write pipe.
attr_core_api PipeWrite pipe_write_void(void);

/// @brief Open read and write pipes.
/// @param[out] out_read Pointer to write read pipe.
/// @param[out] out_write Pointer to write write pipe.
/// @return
///     - @c true  : Opened pipes successfully.
///     - @c false : Failed to open pipes.
attr_core_api b32 pipe_open( PipeRead* out_read, PipeWrite* out_write );
/// @brief Close a pipe.
/// @param[in] pipe Pointer to pipe to close.
#define pipe_close( pipe ) fd_close( &(pipe)->fd )
/// @brief Write to pipe.
/// @param[in]  pipe          Pipe to write to.
/// @param      bytes         Bytes to write to pipe.
/// @param[in]  buf           Pointer to start of buffer to write.
/// @param[out] opt_out_write (optional) Pointer to write bytes written.
/// @return
///     - @c true  : Wrote to pipe successfully.
///     - @c false : Failed to write to pipe.
attr_always_inline
attr_header b32 pipe_write(
    PipeWrite* pipe, usize bytes, const void* buf, usize* opt_out_write
) {
    return fd_write( &pipe->fd, bytes, buf, opt_out_write );
}
/// @brief Read from pipe.
/// @param[in]  pipe         Pipe to read from.
/// @param      bytes        Bytes to read.
/// @param[in]  buf          Pointer to start of buffer to read into.
/// @param[out] opt_out_read (optional) Pointer to write bytes read.
/// @return
///     - @c true  : Read from pipe successfully.
///     - @c false : Failed to read from pipe.
attr_always_inline
attr_header b32 pipe_read(
    PipeRead* pipe, usize bytes, void* buf, usize* opt_out_read
) {
    return fd_read( &pipe->fd, bytes, buf, opt_out_read );
}
/// @brief Write formatted string to pipe.
/// @param[in]  pipe          Pipe to write to.
/// @param[out] opt_out_write (optional) Pointer to write bytes written.
/// @param      format_len    Length of format string.
/// @param[in]  format        Pointer to start of format string.
/// @param      va            Variadic format arguments.
/// @return
///     - @c true  : Wrote to pipe successfully.
///     - @c false : Failed to write to pipe.
attr_always_inline
attr_header b32 pipe_write_fmt_va(
    PipeWrite* pipe, usize* opt_out_write,
    usize format_len, const char* format, va_list va
) {
    return fd_write_fmt_va( &pipe->fd, opt_out_write, format_len, format, va );
}
/// @brief Write formatted string to pipe.
/// @param[in]  pipe          Pipe to write to.
/// @param[out] opt_out_write (optional) Pointer to write bytes written.
/// @param      format_len    Length of format string.
/// @param[in]  format        Pointer to start of format string.
/// @param      ...           Format arguments.
/// @return
///     - @c true  : Wrote to pipe successfully.
///     - @c false : Failed to write to pipe.
attr_always_inline
attr_header b32 pipe_write_fmt(
    PipeWrite* pipe, usize* opt_out_write,
    usize format_len, const char* format, ...
) {
    va_list va;
    va_start( va, format );
    b32 res = fd_write_fmt_va( &pipe->fd, opt_out_write, format_len, format, va );
    va_end( va );
    return res;
}
/// @brief Write formatted string to pipe.
/// @param[in]  pipe          (PipeWrite*)     Pipe to write to.
/// @param[out] opt_out_write (usize*)         (optional) Pointer to write bytes written.
/// @param      format        (string literal) Format string literal.
/// @param      va            (va_list)        Variadic format arguments.
/// @return
///     - @c true  : Wrote to pipe successfully.
///     - @c false : Failed to write to pipe.
#define pipe_write_text_va( pipe, opt_out_write, format, va )\
    pipe_write_fmt_va( pipe, opt_out_write, sizeof(format) - 1, format, va )
/// @brief Write formatted string to pipe.
/// @param[in]  pipe          (PipeWrite*)     Pipe to write to.
/// @param[out] opt_out_write (usize*)         (optional) Pointer to write bytes written.
/// @param      format        (string literal) Format string literal.
/// @param      ...           (args)           Format arguments.
/// @return
///     - @c true  : Wrote to pipe successfully.
///     - @c false : Failed to write to pipe.
#define pipe_write_text( pipe, opt_out_write, format, ... )\
    pipe_write_fmt( pipe, opt_out_write, sizeof(format) - 1, format, ##__VA_ARGS__ )
/// @brief Pipe streaming function.
/// @param[in] struct_PipeWrite Pointer to #PipeWrite.
/// @param     count            Number of bytes to stream to pipe.
/// @param[in] buf              Pointer to start of buffer to write to pipe.
/// @return Number of bytes that couldn't be streamed to pipe.
attr_core_api usize pipe_stream_write(
    void* struct_PipeWrite, usize count, const void* buf );

#endif /* header guard */
