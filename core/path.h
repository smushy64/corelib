#if !defined(CORE_PATH_H)
#define CORE_PATH_H
/**
 * @file   path.h
 * @brief  File system path functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 24, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/defines.h" // IWYU pragma: keep
#include "core/stream.h"
#include "core/string.h"

struct AllocatorInterface;

typedef struct    _StringPOD   _PathPOD;
typedef struct _StringBufPOD   _PathBufPOD;

#if defined(CORE_CPLUSPLUS)
    /// @brief UTF-8 Path.
    /// @details
    /// Not necessarily null-terminated.
    typedef _StringCPP    Path;
    /// @brief UTF-8 Path buffer.
    /// @details
    /// Always accounts for null terminator.
    typedef _StringBufCPP PathBuf;
#else
    /// @brief UTF-8 Path.
    /// @details
    /// Not necessarily null-terminated.
    typedef _PathPOD    Path;
    /// @brief UTF-8 Path buffer.
    /// @details
    /// Always accounts for null terminator.
    typedef _PathBufPOD PathBuf;
#endif

/// @Brief Target for PathBuf stream.
typedef StringBufStreamTarget PathBufStreamTarget;

/// @brief Create new path slice.
/// @param     length Length of path slice.
/// @param[in] start  Pointer to start of slice.
/// @return Path slice.
#define path_new( length, start ) \
    string_new( length, start )
/// @brief Create empty path slice.
/// @return Path slice.
#define path_empty() \
    string_empty()
/// @brief Create new path slice from null terminated C string.
/// @param[in] cstr (const cstr*) C string.
/// @return Path slice.
#define path_from_cstr( cstr ) \
    string_from_cstr( cstr )
/// @brief Create new path slice from string literal.
/// @param literal (const char* literal) String literal.
/// @return Path slice.
#define path_text( literal ) \
    string_text( literal )
/// @brief Check if path is empty.
/// @param path Path to check.
/// @return
///     - @c true  : @c str is empty.
///     - @c false : @c str is not empty.
attr_always_inline attr_header
b32 path_is_empty( _PathPOD path ) {
    return string_is_empty( path );
}
/// @brief Calculate UTF-8 length of path.
/// @note This function does not check if string is valid UTF-8!
/// @param path Path to calculate length of.
/// @return UTF-8 length.
attr_always_inline attr_header
usize path_len_utf8( _PathPOD path ) {
    return string_len_utf8( path );
}
/// @brief Compare paths for equality.
/// @param a, b Paths to compare.
/// @return
///     - @c true  : @c a and @c b are equal in length and contents.
///     - @c false : @c a and @c b are not equal.
attr_always_inline attr_header
b32 path_cmp( _PathPOD a, _PathPOD b ) {
    return string_cmp( a, b );
}

/// @brief Calculate how many path chunks are in the current path.
///
/// @note
/// On Windows, drive letter is not considered a chunk.
///
/// @param path Path slice.
/// @return Number of chunks in path slice.
attr_core_api
usize path_chunk_count( _PathPOD path );

/// @brief Create a path slice of the first chunk in path.
/// @param path Path to clip.
/// @return Slice containing first chunk in path.
attr_core_api
_PathPOD path_clip_chunk( _PathPOD path );

/// @brief Create a path slice of the last chunk in path.
/// @param path Path to clip.
/// @return Slice containing last chunk in path.
attr_core_api
_PathPOD path_clip_chunk_last( _PathPOD path );

/// @brief Create a path slice of chunks after first chunk.
/// @note
/// On Windows, if path is absolute, result will no longer have drive letter.
/// @param path Path slice to advance.
/// @return Path advanced by one chunk.
attr_core_api
_PathPOD path_advance_chunk( _PathPOD path );

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
attr_core_api
_PathPOD path_pop_chunk( _PathPOD path );

/// @brief Fill a path slice buffer with chunks from path.
/// @param      path             Path to retrieve chunks from.
/// @param      chunk_buffer_cap Capacity of @c chunk buffer.
/// @param[out] chunk_buffer     Pointer to start of chunk buffer.
/// @return Number of chunks that could not be written to @c chunk buffer.
attr_core_api
usize path_split_chunks(
    _PathPOD path, usize chunk_buffer_cap, _PathPOD* chunk_buffer );

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
attr_core_api
b32 path_is_absolute( _PathPOD path );

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
attr_always_inline attr_header
b32 path_is_relative( _PathPOD path ) {
    return !path_is_absolute( path );
}

/// @brief Try to get parent directory of given path.
/// @param      path       Path to get parent directory of.
/// @param[out] out_parent Pointer to write parent directory to.
/// @return
///     - true  : @c path contained a parent directory.
///     - false : @c path did not contain a parent directory.
attr_core_api
b32 path_parent( _PathPOD path, _PathPOD* out_parent );

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
attr_core_api
b32 path_file_name( _PathPOD path, _PathPOD* out_file_name );

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
attr_core_api
b32 path_file_stem( _PathPOD path, _PathPOD* out_file_stem );

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
attr_core_api
b32 path_extension( _PathPOD path, _PathPOD* out_extension );

/// @brief Check if path is null-terminated.
/// @param path Path to check.
/// @return
///     - true  : @c path is null-terminated.
///     - false : @c path is not null-terminated.
attr_always_inline attr_header
b32 path_is_null_terminated( _PathPOD path ) {
    if( !path.len ) {
        return false;
    }
    return !path.cbuf[path.len] || !path.cbuf[path.len - 1];
}
/// @brief Stream path with separators changed to POSIX path separators.
/// @param[in] stream Streaming function.
/// @param[in] target Pointer to streaming target.
/// @param     path   Path to stream.
/// @return Number of bytes that could not be written to target.
attr_core_api
usize path_stream_set_posix_separators( StreamBytesFN* stream, void* target, _PathPOD path );
/// @brief Stream path with separators changed to Windows path separators.
/// @param[in] stream Streaming function.
/// @param[in] target Pointer to streaming target.
/// @param     path   Path to stream.
/// @return Number of bytes that could not be written to target.
attr_core_api
usize path_stream_set_windows_separators( StreamBytesFN* stream, void* target, _PathPOD path );
/// @brief Stream path with separators changed to native path separators.
/// @param[in] stream Streaming function.
/// @param[in] target Pointer to streaming target.
/// @param     path   Path to stream.
/// @return Number of bytes that could not be written to target.
attr_core_api
usize path_stream_set_native_separators( StreamBytesFN* stream, void* target, _PathPOD path );
/// @brief Set path separators to POSIX path separators.
/// @param path Path to modify.
attr_core_api
void path_set_posix_separators( _PathPOD path );
/// @brief Set path separators to Windows path separators.
/// @param path Path to modify.
attr_core_api
void path_set_windows_separators( _PathPOD path );
/// @brief Set path separators to native path separators.
/// @param path Path to modify.
attr_core_api
void path_set_native_separators( _PathPOD path );
/// @brief Stream canonical path.
/// @param[in] stream Streaming function.
/// @param[in] target Pointer to streaming target.
/// @param     path   Path to stream.
/// @return Number of bytes that could not be written to target.
attr_core_api
usize path_stream_canonicalize( StreamBytesFN* stream, void* target, _PathPOD path );

/// @brief Create new path buffer.
/// @param     capacity Capacity of path buffer.
/// @param[in] start    Pointer to start of path buffer.
/// @return Path buffer.
attr_always_inline attr_header
_PathBufPOD path_buf_new( usize capacity, const void* start ) {
    return string_buf_new( capacity, start );
}
/// @brief Create empty path buffer.
/// @return Path buffer.
attr_always_inline attr_header
_PathBufPOD path_buf_empty(void) {
    return string_buf_empty();
}
/// @brief Initialize a path buffer from the stack.
/// @details
/// Defines a stack char buffer with name @c name\#\#_buffer
/// of given size and a PathBuf from that buffer with given name.
/// @param name (valid identifier) Name of path buffer.
/// @param size (usize)            Size of path buffer.
#define path_buf_from_stack( name, size ) \
    string_buf_from_stack( name, size )
/// @brief Create path buffer with given allocator.
/// @param[in]  allocator Pointer to allocator interface.
/// @param      size      Size of allocation.
/// @param[out] out_buf   Pointer to write new path buffer to.
/// @return
///     - @c true  : Path buffer successfully allocated.
///     - @c false : Failed to allocate path buffer.
attr_always_inline attr_header
b32 path_buf_from_alloc(
    struct AllocatorInterface* allocator, usize size, _PathBufPOD* out_buf
) {
    return string_buf_from_alloc( allocator, size, out_buf );
}
/// @brief Create path buffer with given allocator.
/// @param[in]  allocator Pointer to allocator interface.
/// @param      src       Path to copy to buffer.
/// @param[out] out_buf   Pointer to write new path buffer to.
/// @return
///     - @c true  : Path buffer successfully allocated.
///     - @c false : Failed to allocate path buffer.
attr_always_inline attr_header
b32 path_buf_from_path_alloc(
    struct AllocatorInterface* allocator, _PathPOD src, _PathBufPOD* out_buf
) {
    return string_buf_from_string_alloc( allocator, src, out_buf );
}
/// @brief Grow path buffer capacity.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to path buffer to grow.
/// @param     amount    Number of bytes to grow by.
/// @return
///     - @c true  : Path buffer successfully reallocated.
///     - @c false : Failed to reallocate path buffer.
attr_always_inline attr_header
b32 path_buf_grow( struct AllocatorInterface* allocator, _PathBufPOD* buf, usize amount ) {
    return string_buf_grow( allocator, buf, amount );
}
/// @brief Free path buffer from allocator.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to path buffer to free.
attr_always_inline attr_header
void path_buf_free( struct AllocatorInterface* allocator, _PathBufPOD* buf ) {
    string_buf_free( allocator, buf );
}
/// @brief Calculate remaining space in path buffer.
/// @param buf Path buffer.
/// @return Number of bytes remaining in path buffer.
attr_always_inline attr_header
usize path_buf_remaining( _PathBufPOD buf ) {
    return string_buf_remaining( buf );
}
/// @brief Check if path buffer is empty.
/// @param buf Pointer to path buffer.
/// @return
///     - @c true  : Path buffer is empty.
///     - @c false : Path buffer is not empty.
attr_always_inline attr_header
b32 path_buf_is_empty( _PathBufPOD buf ) {
    return string_buf_is_empty( buf );
}
/// @brief Check if path buffer is full.
/// @note Path buffer always tries to have space for null terminator.
/// @param buf Pointer to path buffer.
/// @return
///     - @c true  : Path buffer is full.
///     - @c false : Path buffer still has space.
attr_always_inline attr_header
b32 path_buf_is_full( _PathBufPOD buf ) {
    return string_buf_is_full( buf );
}
/// @brief Set path buffer length to zero and zero out memory.
/// @param[in] buf Pointer to path buffer.
attr_always_inline attr_header
void path_buf_clear( _PathBufPOD* buf ) {
    string_buf_clear( buf );
}
/// @brief Clone path buffer.
/// @details
/// Allocates @c src.len + 1 in @c dst buffer.
/// @param[in]  allocator Pointer to allocator interface.
/// @param[out] dst       Destination path buffer.
/// @param[in]  src       Source path buffer.
/// @return
///     - @c true  : Allocated @c dst buffer and copied contents of @c src to it.
///     - @c false : Failed to allocate @c dst buffer.
attr_always_inline attr_header
b32 path_buf_clone(
    struct AllocatorInterface* allocator, _PathBufPOD* dst, _PathPOD src
) {
    return string_buf_clone( allocator, dst, src );
}
/// @brief Stream format function for path buffer.
/// @param[in] PathBuf Pointer to path buffer.
/// @param     count   Number of bytes to stream to buffer.
/// @param[in] bytes   Pointer to bytes to stream to buffer.
/// @return
///     - Zero   : @c buf had enough capacity to write path.
///     - > Zero : Number of characters not written to @c buf.
attr_always_inline attr_header
usize path_buf_try_stream(
    void* PathBuf, usize count, const void* bytes 
) {
    return string_buf_try_stream( PathBuf, count, bytes );
}
/// @brief Stream format function for path buffer.
/// @param[in] PathBufStreamTarget Pointer to #PathBufStreamTarget.
/// @param     count               Number of bytes to stream to buffer.
/// @param[in] bytes               Pointer to bytes to stream to buffer.
/// @return
///     - @c true  : Wrote entire path to @c buf. If allocation was required, it was successful.
///     - @c false : Failed to reallocate @c buf.
attr_always_inline attr_header
bsize path_buf_stream(
    void* PathBufStreamTarget, usize count, const void* bytes 
) {
    return string_buf_stream( PathBufStreamTarget, count, bytes );
}
/// @brief Try to push path chunk to end of path buffer.
/// @param[in] buf   Pointer to path buffer to push to.
/// @param     chunk Path chunk to push.
/// @return
///     - true  : @c buf had enough capacity for path chunk.
///     - false : @c buf does not have enough capacity for path chunk.
attr_core_api
b32 path_buf_try_push_chunk( _PathBufPOD* buf, _PathPOD chunk );
/// @brief Push path chunk to end of path buffer.
/// @param[in] allocator Pointer to allocator.
/// @param[in] buf       Pointer to path buffer to push to.
/// @param     chunk     Path chunk to push.
/// @return
///     - true  : Chunk pushed successfully.
///     - false : Failed to reallocate @c buf.
attr_core_api
b32 path_buf_push_chunk(
    struct AllocatorInterface* allocator, _PathBufPOD* buf, _PathPOD chunk );
/// @brief Pop last chunk from path buffer.
/// @param[in] buf Pointer to path buffer to pop from.
/// @return
///     - true  : @c buf had at least one chunk to pop off.
///     - false : @c buf has no more chunks to pop off.
attr_core_api
b32 path_buf_pop_chunk( _PathBufPOD* buf );
/// @brief Try to set path buffer's extension.
/// @param[in] buf       Pointer to path buffer.
/// @parma     extension Extension to set.
/// @return
///     - true  : @c buf had enough capacity to change extension.
///     - false : @c buf did not have enough capacity to change extension.
attr_core_api
b32 path_buf_try_set_extension( _PathBufPOD* buf, _PathPOD extension );
/// @brief Set path buffer's extension.
/// @param[in] allocator Pointer to allocator.
/// @param[in] buf       Pointer to path buffer.
/// @parma     extension Extension to set.
/// @return
///     - true  : Extension set successfully.
///     - false : Faileed to reallocate @c buf.
attr_core_api
b32 path_buf_set_extension(
    struct AllocatorInterface* allocator, _PathBufPOD* buf, _PathPOD extension );

#endif /* header guard */
