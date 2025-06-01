#if !defined(CORE_CPP_PATH_HPP) && defined(__cplusplus)
#define CORE_CPP_PATH_HPP
/**
 * @file   path.hpp
 * @brief  C++ Path.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 01, 2025
*/
#if !defined(CORE_PATH_H)
    #include "core/path.h"
#endif

/// @brief Calculate how many path chunks are in the current path.
///
/// @note
/// On Windows, drive letter is not considered a chunk.
///
/// @param path Path slice.
/// @return Number of chunks in path slice.
attr_header
usize path_chunk_count( String path );
/// @brief Create a path slice of the first chunk in path.
/// @param path Path to clip.
/// @return Slice containing first chunk in path.
attr_header
String path_clip_chunk( String path );
/// @brief Create a path slice of the last chunk in path.
/// @param path Path to clip.
/// @return Slice containing last chunk in path.
attr_header
String path_clip_chunk_last( String path );
/// @brief Create a path slice of chunks after first chunk.
/// @note
/// On Windows, if path is absolute, result will no longer have drive letter.
/// @param path Path slice to advance.
/// @return Path advanced by one chunk.
attr_header
String path_advance_chunk( String path );
/// @brief Create a path slice without last chunk.
/// @note
///
/// On Windows, if path is absolute and there are
/// no more chunks to pop, result is just the drive letter.
/// Otherwise, it's an empty slice.
///
/// On POSIX, if path is absolute and there are
/// no more chunks to pop, result is /.
/// Otherwise, it's an empty slice.
/// @param path Path slice to pop from.
/// @return Path with last chunk poppped.
attr_header
String path_pop_chunk( String path );
/// @brief Fill a path slice buffer with chunks from path.
/// @param      path             Path to retrieve chunks from.
/// @param      chunk_buffer_cap Capacity of @c chunk buffer.
/// @param[out] chunk_buffer     Pointer to start of chunk buffer.
/// @return Number of chunks that could not be written to @c chunk buffer.
attr_header
usize path_split_chunks( String path, usize chunk_buffer_cap, String* chunk_buffer );
/// @brief Check if path is absolute.
/// @note
///
/// On Windows, a path is absolute if it contains a drive letter. ex: C:/
///
/// On POSIX, a path is absolute if it starts with /.
///
/// @param path Path to check.
/// @return
///     - true  : @c path is an absolute path.
///     - false : @c path is relative or malformed path.
attr_header
bool path_is_absolute( String path );
/// @brief Check if path is relative.
/// @note
///
/// On Windows, a path is relative if it does not start with drive letter.
///
/// On POSIX, a path is relative if it does not start with /.
///
/// @param path Path to check.
/// @return
///     - true  : @c path is a relative path.
///     - false : @c path is an absolute or malformed path.
attr_header
bool path_is_relative( String path );
/// @brief Try to get parent directory of given path.
/// @param      path       Path to get parent directory of.
/// @param[out] out_parent Pointer to write parent directory to.
/// @return
///     - true  : @c path contained a parent directory.
///     - false : @c path did not contain a parent directory.
attr_header
bool path_parent( String path, String* out_parent );
/// @brief Try to get file name of given path.
/// @note
///
/// This function does not check the file system,
/// it just checks if path is empty or ends with a separator.
/// If neither of those are true, it writes the last chunk to
/// @c out_file_name.
///
/// @param      path          Path to get file name of.
/// @param[out] out_file_name Pointer to write file name to.
/// @return
///     - true  : @c path contained a file name.
///     - false : @c path was empty or ended with a /.
attr_header
bool path_file_name( String path, String* out_file_name );
/// @brief Try to get file stem of given path.
/// @note
///
/// This function does not check the file system,
/// it just checks if path is empty or ends with a separator.
/// If neither of those are true, it writes the last chunk to
/// @c out_file_stem.
///
/// If the file stem contains an extension, this function
/// strips it from the result.
///
/// @param      path          Path to get file stem of.
/// @param[out] out_file_stem Pointer to write file stem to.
/// @return
///     - true  : @c path contained a file stem.
///     - false : @c path was empty or ended with a /.
attr_header
bool path_file_stem( String path, String* out_file_stem );
/// @brief Try to get file extension from path.
/// @note
/// 
/// This function does not check the file system to make
/// sure that the last chunk in @c path is a file.
/// The only check it does is if path ends with a separator
/// because that means that the last chunk is a directory.
///
/// @param      path          Path to get extension from.
/// @param[out] out_extension Pointer to write extension to.
/// @return
///     - true  : The last chunk in @c path has a file extension.
///     - false : The last chunk in @c path did not have a file extension.
attr_header
bool path_extension( String path, String* out_extension );
/// @brief Check if path is null-terminated.
/// @param path Path to check.
/// @return
///     - true  : @c path is null-terminated.
///     - false : @c path is not null-terminated.
attr_header
bool path_is_null_terminated( String path );
/// @brief Stream path with separators changed to POSIX path separators.
/// @param[in] stream Streaming function.
/// @param[in] target Pointer to streaming target.
/// @param     path   Path to stream.
/// @return Number of bytes that could not be written to target.
attr_header
usize path_stream_set_posix_separators( StreamBytesFN* stream, void* target, String path );
/// @brief Stream path with separators changed to Windows path separators.
/// @param[in] stream Streaming function.
/// @param[in] target Pointer to streaming target.
/// @param     path   Path to stream.
/// @return Number of bytes that could not be written to target.
attr_header
usize path_stream_set_windows_separators( StreamBytesFN* stream, void* target, String path );
/// @brief Stream path with separators changed to native path separators.
/// @param[in] stream Streaming function.
/// @param[in] target Pointer to streaming target.
/// @param     path   Path to stream.
/// @return Number of bytes that could not be written to target.
attr_header
usize path_stream_set_native_separators( StreamBytesFN* stream, void* target, String path );
/// @brief Set path separators to POSIX path separators.
/// @param path Path to modify.
attr_header
void path_set_posix_separators( String path );
/// @brief Set path separators to Windows path separators.
/// @param path Path to modify.
attr_header
void path_set_windows_separators( String path );
/// @brief Set path separators to native path separators.
/// @param path Path to modify.
attr_header
void path_set_native_separators( String path );
/// @brief Stream canonical path.
/// @param[in] stream Streaming function.
/// @param[in] target Pointer to streaming target.
/// @param     path   Path to stream.
/// @return Number of bytes that could not be written to target.
attr_header
usize path_stream_canonicalize( StreamBytesFN* stream, void* target, String path );
/// @brief Try to push path chunk to end of path buffer.
/// @param[in] buf   Pointer to path buffer to push to.
/// @param     chunk Path chunk to push.
/// @return
///     - true  : @c buf had enough capacity for path chunk.
///     - false : @c buf does not have enough capacity for path chunk.
attr_header
bool path_buf_try_push_chunk( StringBuf* buf, String chunk );
/// @brief Push path chunk to end of path buffer.
/// @param[in] allocator Pointer to allocator.
/// @param[in] buf       Pointer to path buffer to push to.
/// @param     chunk     Path chunk to push.
/// @return
///     - true  : Chunk pushed successfully.
///     - false : Failed to reallocate @c buf.
attr_header
bool path_buf_push_chunk( AllocatorInterface* allocator, StringBuf* buf, String chunk );
/// @brief Pop last chunk from path buffer.
/// @param[in] buf Pointer to path buffer to pop from.
/// @return
///     - true  : @c buf had at least one chunk to pop off.
///     - false : @c buf has no more chunks to pop off.
attr_header
bool path_buf_pop_chunk( StringBuf* buf );
/// @brief Try to set path buffer's extension.
/// @param[in] buf       Pointer to path buffer.
/// @param     extension Extension to set.
/// @return
///     - true  : @c buf had enough capacity to change extension.
///     - false : @c buf did not have enough capacity to change extension.
attr_header
bool path_buf_try_set_extension( StringBuf* buf, String extension );
/// @brief Set path buffer's extension.
/// @param[in] allocator Pointer to allocator.
/// @param[in] buf       Pointer to path buffer.
/// @param     extension Extension to set.
/// @return
///     - true  : Extension set successfully.
///     - false : Faileed to reallocate @c buf.
attr_header
bool path_buf_set_extension(
    AllocatorInterface* allocator, StringBuf* buf, String extension );

// NOTE(alicia): implementation -----------------------------------------------

attr_always_inline attr_header attr_hot
usize path_chunk_count( String path ) {
    return path_chunk_count( path.__pod );
}
attr_always_inline attr_header attr_hot
String path_clip_chunk( String path ) {
    return path_clip_chunk( path.__pod );
}
attr_always_inline attr_header attr_hot
String path_clip_chunk_last( String path ) {
    return path_clip_chunk_last( path.__pod );
}
attr_always_inline attr_header attr_hot
String path_advance_chunk( String path ) {
    return path_advance_chunk( path.__pod );
}
attr_always_inline attr_header attr_hot
String path_pop_chunk( String path ) {
    return path_pop_chunk( path.__pod );
}
attr_always_inline attr_header attr_hot
usize path_split_chunks( String path, usize chunk_buffer_cap, String* chunk_buffer ) {
    return path_split_chunks( path.__pod, chunk_buffer_cap, (_StringPOD*)chunk_buffer );
}
attr_always_inline attr_header attr_hot
bool path_is_absolute( String path ) {
    return path_is_absolute( path.__pod );
}
attr_always_inline attr_header attr_hot
bool path_is_relative( String path ) {
    return path_is_relative( path.__pod );
}
attr_always_inline attr_header attr_hot
bool path_parent( String path, String* out_parent ) {
    return path_parent( path.__pod, (_StringPOD*)out_parent );
}
attr_always_inline attr_header attr_hot
bool path_file_name( String path, String* out_file_name ) {
    return path_file_name( path.__pod, (_StringPOD*)out_file_name );
}
attr_always_inline attr_header attr_hot
bool path_file_stem( String path, String* out_file_stem ) {
    return path_file_stem( path.__pod, (_StringPOD*)out_file_stem );
}
attr_always_inline attr_header attr_hot
bool path_extension( String path, String* out_extension ) {
    return path_extension( path.__pod, (_StringPOD*)out_extension );
}
attr_always_inline attr_header attr_hot
bool path_is_null_terminated( String path ) {
    return path_is_null_terminated( path.__pod );
}
attr_always_inline attr_header attr_hot
usize path_stream_set_posix_separators( StreamBytesFN* stream, void* target, String path ) {
    return path_stream_set_posix_separators( stream, target, path.__pod );
}
attr_always_inline attr_header attr_hot
usize path_stream_set_windows_separators( StreamBytesFN* stream, void* target, String path ) {
    return path_stream_set_windows_separators( stream, target, path.__pod );
}
attr_always_inline attr_header attr_hot
usize path_stream_set_native_separators( StreamBytesFN* stream, void* target, String path ) {
    return path_stream_set_native_separators( stream, target, path.__pod );
}
attr_always_inline attr_header attr_hot
void path_set_posix_separators( String path ) {
    path_set_posix_separators( path.__pod );
}
attr_always_inline attr_header attr_hot
void path_set_windows_separators( String path ) {
    path_set_windows_separators( path.__pod );
}
attr_always_inline attr_header attr_hot
void path_set_native_separators( String path ) {
    path_set_native_separators( path.__pod );
}
attr_always_inline attr_header attr_hot
usize path_stream_canonicalize( StreamBytesFN* stream, void* target, String path ) {
    return path_stream_canonicalize( stream, target, path.__pod );
}
attr_always_inline attr_header attr_hot
bool path_buf_try_push_chunk( StringBuf* buf, String chunk ) {
    return path_buf_try_push_chunk( (_StringBufPOD*)buf, chunk.__pod );
}
attr_always_inline attr_header attr_hot
bool path_buf_push_chunk( AllocatorInterface* allocator, StringBuf* buf, String chunk ) {
    return path_buf_push_chunk( allocator, (_StringBufPOD*)buf, chunk.__pod );
}
attr_always_inline attr_header attr_hot
bool path_buf_pop_chunk( StringBuf* buf ) {
    return path_buf_pop_chunk( (_StringBufPOD*)buf );
}
attr_always_inline attr_header attr_hot
bool path_buf_try_set_extension( StringBuf* buf, String extension ) {
    return path_buf_try_set_extension( (_StringBufPOD*)buf, extension.__pod );
}
attr_always_inline attr_header attr_hot
bool path_buf_set_extension(
    AllocatorInterface* allocator, StringBuf* buf, String extension
) {
    return path_buf_set_extension( allocator, (_StringBufPOD*)buf, extension.__pod );
}

#endif /* header guard */
