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

/// @brief Opaque file handle.
typedef void FileHandle;
/// @brief Flags for opening a file.
typedef u32 FileOpenFlags;

/// @brief Open file for reading.
///
/// @note Fails if file does not already exist
/// unless #FILE_OPEN_FLAG_CREATE is specified.
#define FILE_OPEN_FLAG_READ (1 << 0)
/// @brief Open file for writing.
#define FILE_OPEN_FLAG_WRITE (1 << 1)
/// @brief Allow other threads/processes to read file.
#define FILE_OPEN_FLAG_SHARE_ACCESS_READ (1 << 2)
/// @brief Allow other threads/processes to write to file.
#define FILE_OPEN_FLAG_SHARE_ACCESS_WRITE (1 << 3)
/// @brief Create file if it does not already exist.
///
/// If the file already exists, truncates file.
#define FILE_OPEN_FLAG_CREATE (1 << 4)
/// @brief Open an existing file and delete its contents.
///
/// Fails if file does not already exist.
#define FILE_OPEN_FLAG_TRUNCATE (1 << 5)
/// @brief Moves offset to end of file.
///
/// Fails if #FILE_OPEN_FLAG_WRITE is not specified.
#define FILE_OPEN_FLAG_APPEND (1 << 6)
/// @brief File is created as a temporary file.
///
/// On some file systems, opening a file as a temporary can
/// potentially avoid actually writing to disk if written bytes
/// do not exceed file cache, improving performance.
#define FILE_OPEN_FLAG_TEMP (1 << 7)

/// @brief Open a file.
/// @param path  Path to file.
/// @param flags Flags for opening file.
/// @return Handle to file, NULL if failed.
attr_core_api FileHandle* fs_file_open( const Path path, FileOpenFlags flags );
/// @brief Close a file.
/// @param[in] file File handle to close.
attr_core_api void fs_file_close( FileHandle* file );
/// @brief Query size of file.
/// @param[in] file File handle to query size.
/// @return Size of file.
attr_core_api usize fs_file_query_size( FileHandle* file );
/// @brief Set the size of a file to the current offset.
/// @param[in] file File to truncate.
attr_core_api void fs_file_truncate( FileHandle* file );
/// @brief Set the size of a file.
///
/// If size is smaller than current offset, offset is moved to end of file.
/// @param[in] file File to set size of.
/// @param size Size to set to, can be bigger or smaller than current file size.
attr_core_api void fs_file_set_size( FileHandle* file, usize size );
/// @brief Query offset of file pointer.
/// @param[in] file File handle to query offset.
/// @return Offset of file pointer.
attr_core_api usize fs_file_query_offset( FileHandle* file );
/// @brief Set offset of file pointer.
/// @param[in] file   File to set offset.
/// @param     offset Offset from start of file.
attr_core_api void fs_file_set_offset( FileHandle* file, usize offset );
/// @brief Set offset relative to current offset.
/// @param[in] file   File to set offset.
/// @param     offset Offset relative to current offset.
/// @return False if negative offset goes outside bounds of file.
attr_core_api b32 fs_file_set_offset_relative( FileHandle* file, isize offset );
/// @brief Write data to file.
///
/// Offset is moved by len bytes.
/// @param[in] file File to write to.
/// @param     len  Number of bytes to write.
/// @param[in] buf  Pointer to data to write.
/// @return True if write was successful.
attr_core_api b32 fs_file_write( FileHandle* file, usize len, const void* buf );
/// @brief Write data to file.
///
/// Offset does not move.
/// @param[in] file File to write to.
/// @param     len  Number of bytes to write.
/// @param[in] buf  Pointer to data to write.
/// @return True if write was successful.
attr_always_inline
attr_header b32 fs_file_write_no_offset(
    FileHandle* file, usize len, const void* buf
) {
    usize current = fs_file_query_offset( file );
    if( !fs_file_write( file, len, buf ) ) {
        return false;
    }
    fs_file_set_offset( file, current );
    return true;
}
/// @brief Read data from file.
///
/// Offset is moved by len bytes.
/// @param[in]  file File to read from.
/// @param      len  Number of bytes to read.
/// @param[out] buf  Pointer to data to read into.
/// @return True if read was successful.
attr_core_api b32 fs_file_read( FileHandle* file, usize len, void* buf );
/// @brief Read data from file.
///
/// Offset is not moved.
/// @param[in]  file File to read from.
/// @param      len  Number of bytes to read.
/// @param[out] buf  Pointer to data to read into.
/// @return True if read was successful.
attr_always_inline
attr_header b32 fs_file_read_no_offset( FileHandle* file, usize len, void* buf ) {
    usize current = fs_file_query_offset( file );
    if( !fs_file_read( file, len, buf ) ) {
        return false;
    }
    fs_file_set_offset( file, current );
    return true;
}
/// @brief Copy contents of src file to dst file.
/// @param[in] dst               Destination file.
/// @param[in] src               Source file.
/// @param     size              Number of bytes to copy.
/// @param     intermediate_size Size of intermediate buffer.
/// @param[in] intermediate      Intermediate buffer.
/// @return True if successful.
attr_core_api b32 fs_file_copy_memory(
    FileHandle* dst, FileHandle* src, usize size,
    usize intermediate_size, void* intermediate );
/// @brief Copy file to destination path.
/// @param dst                Destination path.
/// @param src                Path to file to copy.
/// @param fail_if_dst_exists If true, checks if dst exists and if it does, function fails.
/// @return True if successful.
attr_core_api b32 fs_file_copy(
    const Path dst, const Path src, b32 fail_if_dst_exists );
/// @brief Move file to destination path.
/// @param dst                Destination path.
/// @param src                Path to file to move.
/// @param fail_if_dst_exists If true, checks if dst exists and if so, fails.
/// @return True if successful.
attr_core_api b32 fs_file_move(
    const Path dst, const Path src, b32 fail_if_dst_exists );
/// @brief Delete a file by its path.
/// @param path Path to file to delete.
/// @return True if file was deleted successfully.
attr_core_api b32 fs_file_delete( const Path path );
/// @brief Check if file pointed to by path exists.
/// @param path Path to check.
/// @return True if file exists at given path.
attr_core_api b32 fs_file_exists( const Path path );
/// @brief Get the current working directory.
/// @return Working directory path (readonly).
attr_core_api Path fs_working_directory(void);
/// @brief Create a directory at given path.
///
/// Function succeeds even if directory already exists.
/// @param path Path to create directory at.
/// @return True if directory is successfully created.
attr_core_api b32 fs_directory_create( const Path path );
/// @brief Get count of how many items are in directory.
/// @param      path      Path of directory.
/// @param[out] out_count Pointer to receive count.
/// @return True if path is valid.
attr_core_api b32 fs_directory_item_count( const Path path, usize* out_count );
/// @brief Delete a directory at given path.
/// @param path      Path to directory to delete.
/// @param recursive If true, attempts to delete every file in directory. If directory is not empty and recursive is false, function fails.
/// @return True if directory is successfully deleted.
attr_core_api b32 fs_directory_delete( const Path path, b32 recursive );
/// @brief Check if directory pointed to by path exists.
/// @param path Path to directory.
/// @return True if directory exists.
attr_core_api b32 fs_directory_exists( const Path path );

#if defined(CORE_DOXYGEN)

/// @brief Stdin handle.
/// @return Stdin handle.
#define stdin_handle()  
/// @brief Stdout handle.
/// @return Stdout handle.
#define stdout_handle() 
/// @brief Stderr handle.
/// @return Stderr handle.
#define stderr_handle() 

#else

#if defined(CORE_PLATFORM_WINDOWS)

// NOTE(alicia): on Windows, stdin, stdout and stderr are not
// POSIX file descriptors, rather they are unique handles to a console
// so we need an actual function to retrieve the handles at run-time.

/// @brief Stdin handle.
/// @return Stdin handle.
attr_core_api void* stdin_handle(void);
/// @brief Stdout handle.
/// @return Stdout handle.
attr_core_api void* stdout_handle(void);
/// @brief Stderr handle.
/// @return Stderr handle.
attr_core_api void* stderr_handle(void);

#else /* Platform Win32 */

// NOTE(alicia): for POSIX compliant platforms, we can just
// use the default file descriptors.

/// @brief Stdin handle.
/// @return Stdin handle.
#define stdin_handle()  ((void*)0)
/// @brief Stdout handle.
/// @return Stdout handle.
#define stdout_handle() ((void*)1)
/// @brief Stderr handle.
/// @return Stderr handle.
#define stderr_handle() ((void*)2)

#endif /* Platform POSIX */

#endif /* Doxygen */

#endif /* header guard */
