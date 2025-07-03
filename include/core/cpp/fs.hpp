#if !defined(CORE_CPP_FS_HPP) && defined(__cplusplus)
#define CORE_CPP_FS_HPP
/**
 * @file   fs.hpp
 * @brief  C++ File system.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 01, 2025
*/
#if !defined(CORE_FS_H)
    #include "core/fs.h"
#endif
#include "core/cpp/bitwise.hpp"

CPP_BITWISE_OPERATORS(FileOpenFlags, u32);
CPP_BITWISE_OPERATORS(FilePermissions, u32);

/// @brief Copy contents of source file to destination file.
/// @param dst        Path to destination file. 
/// @param src        Path to source file. 
/// @param create_dst Fail if destination already exists.
/// @return
///     - @c true  : Successfully copied source contents to destination.
///     - @c false : Failed to copy.
///     - @c false : If @c create_dst, @c dst already exists.
attr_header
bool file_copy_by_path( String dst, String src, bool create_dst );
/// @brief Move contents of source file to destination file.
/// @param dst        Path to destination file. 
/// @param src        Path to source file. 
/// @param create_dst Fail if destination already exists.
/// @return
///     - @c true  : Successfully moved source to destination.
///     - @c false : Failed to move.
///     - @c false : If @c create_dst, @c dst already exists.
attr_header
bool file_move_by_path( String dst, String src, bool create_dst );
/// @brief Remove file at path.
/// @param path Path of file to remove. 
/// @return
///     - @c true  : Removed file successfully.
///     - @c false : Failed to remove file.
attr_header
bool file_remove_by_path( String path );
/// @brief Query info about a file.
/// @param      path     Path to file.
/// @param[out] out_info Pointer to file info struct to fill out.
/// @return
///     - true  : Successfully obtained information about the specified file.
///     - false : Failed to obtain file information.
attr_header
bool file_query_info_by_path( String path, FileInfo* out_info );
/// @brief Check if file exists by path.
/// @param path Path to file.
/// @return
///     - @c true  : File exists (file or directory).
///     - @c false : File does not exist.
attr_header
bool file_exists_by_path( String path );
/// @brief Query file descriptor file type by path.
/// @note
/// This is the same as calling file_query_info_by_path() and only reading
/// the type field.
/// @param path Path to file.
/// @return Type of file.
attr_header
FileType file_query_type_by_path( String path );
/// @brief Query when file was created by path.
/// @note
/// This is the same as calling file_query_info_by_path() and only reading
/// the time.create field.
/// @param path Path to file.
/// @return Time created.
attr_header
TimePosix file_query_time_create_by_path( String path );
/// @brief Query when file was last modified by path.
/// @note
/// This is the same as calling file_query_info_by_path() and only reading
/// the time.modify field.
/// @param path Path to file.
/// @return Time modified.
attr_header
TimePosix file_query_time_modify_by_path( String path );

/// @brief Open file descriptor.
/// @param      path   Path to file.
/// @param      flags  Flags for opening file.
/// @param[out] out_fd Pointer to write file descriptor to.
/// @return
///     - @c true  : File was opened successfully.
///     - @c false : Failed to open file.
attr_header
bool file_open( String path, FileOpenFlags flags, FD* out_fd );
/// @brief Write bytes to file.
/// @param[in]  fd            File descriptor.
/// @param      bytes         Number of bytes in @c buf.
/// @param[in]  buf           Pointer to start of buffer to write.
/// @return
///     - @c true  : Wrote to file successfully.
///     - @c false : Failed to write to file.
attr_header
bool file_write( FD* fd, usize bytes, const void* buf );
/// @brief Read bytes from file.
/// @param[in]  fd           File descriptor.
/// @param      buf_size     Number of bytes in @c buf.
/// @param[in]  buf          Pointer to start of buffer to read to.
/// @param[out] opt_out_read (optional) Pointer to write bytes read from file.
/// @return
///     - @c true  : Read from file successfully.
///     - @c false : Failed to read from file.
attr_header
bool file_read( FD* fd, usize buf_size, void* buf );
/// @brief Write formatted string to file.
/// @param[in] fd     File descriptor.
/// @param     format Format string.
/// @param[in] va     Variadic arguments.
/// @return Number of bytes that could not be written to file.
attr_header
usize file_write_fmt_va( FD* fd, String format, va_list va );
/// @brief Write formatted string to file.
/// @param[in] fd     File descriptor.
/// @param     format Format string.
/// @param[in] ...    Arguments.
/// @return Number of bytes that could not be written to file.
attr_header
usize file_write_fmt( FD* fd, String format, ... );

/// @brief Create a directory at given path.
/// @details
/// If directory already exists, returns true.
/// @param path Path of new directory.
/// @return
///     - @c true  : Successfully created directory.
///     - @c false : Failed to create directory.
attr_header
bool directory_create( String path );
/// @brief Remove directory from file system.
/// @param path      Path of directory to remove.
/// @param recursive If false, directory must be empty.
/// @return
///     - @c true  : Successfully removed directory.
///     - @c false : Failed to remove directory.
attr_header
bool directory_remove( String path, bool recursive );
/// @brief Walk directory and call callback for each item in directory.
/// @param     path     Path to directory to walk.
/// @param[in] callback Callback function to call for each item.
/// @param[in] params   (optional) Additional parameters for callback function.
/// @return
///     - true  : Successfully walked directory.
///     - false : Failed to walk directory.
attr_header
bool directory_walk( String path, DirectoryWalkFN* callback, void* params );
/// @brief Set current working directory.
/// @warning
/// This function is not thread safe! Make sure to only call it
/// when no other threads are using any file system functions!
/// @param path Path to set current working directory to.
/// @return
///     - @c true  : Set current working directory successfully.
///     - @c false : Failed to set current working directory.
attr_header
bool directory_current_set( String path );

// NOTE(alicia): implementation -----------------------------------------------

attr_always_inline attr_header attr_hot
bool file_copy_by_path( String dst, String src, bool create_dst ) {
    return file_copy_by_path( dst.__pod, src.__pod, (b32)create_dst );
}
attr_always_inline attr_header attr_hot
bool file_move_by_path( String dst, String src, bool create_dst ) {
    return file_move_by_path( dst.__pod, src.__pod, (b32)create_dst );
}
attr_always_inline attr_header attr_hot
bool file_remove_by_path( String path ) {
    return file_remove_by_path( path.__pod );
}
attr_always_inline attr_header attr_hot
bool file_query_info_by_path( String path, FileInfo* out_info ) {
    return file_query_info_by_path( path.__pod, out_info );
}
attr_always_inline attr_header attr_hot
FileType file_query_type_by_path( String path ) {
    return file_query_type_by_path( path.__pod );
}
attr_always_inline attr_header attr_hot
bool file_exists_by_path( String path ) {
    return file_exists_by_path( path.__pod );
}
attr_always_inline attr_header attr_hot
TimePosix file_query_time_create_by_path( String path ) {
    return file_query_time_create_by_path( path.__pod );
}
attr_always_inline attr_header attr_hot
TimePosix file_query_time_modify_by_path( String path ) {
    return file_query_time_modify_by_path( path.__pod );
}
attr_always_inline attr_header attr_hot
bool file_open( String path, FileOpenFlags flags, FD* out_fd ) {
    return file_open( path.__pod, flags, out_fd );
}
attr_always_inline attr_header attr_hot
bool file_write( FD* fd, usize bytes, const void* buf ) {
    return file_write( fd, bytes, buf, nullptr );
}
attr_always_inline attr_header attr_hot
bool file_read( FD* fd, usize buf_size, void* buf ) {
    return file_read( fd, buf_size, buf, nullptr );
}
attr_always_inline attr_header attr_hot
usize file_write_fmt_va( FD* fd, String format, va_list va ) {
    return file_write_fmt_va( fd, format.__pod, va );
}
attr_always_inline attr_header attr_hot
usize file_write_fmt( FD* fd, String format, ... ) {
    va_list va;
    va_start( va, format );
    usize result = file_write_fmt_va( fd, format, va );
    va_end( va );
    return result;
}
attr_always_inline attr_header attr_hot
bool directory_create( String path ) {
    return directory_create( path.__pod );
}
attr_always_inline attr_header attr_hot
bool directory_remove( String path, bool recursive ) {
    return directory_remove( path.__pod, (b32)recursive );
}
attr_always_inline attr_header attr_hot
bool directory_walk( String path, DirectoryWalkFN* callback, void* params ) {
    return directory_walk( path.__pod, callback, params );
}
attr_always_inline attr_header attr_hot
bool directory_current_set( String path ) {
    return directory_current_set( path.__pod );
}


#endif /* header guard */
