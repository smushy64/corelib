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
#include "core/slice.h"
#include "core/string.h"

/// @brief UTF-8 Path. Same as String.
/// @details
/// Not necessarily null terminated.
/// @see #ByteSlice
/// @see #String
typedef String Path;
/// @brief UTF-8 Path buffer. Same as StringBuf.
/// @details
/// Not necessarily null terminated.
/// @see #ByteBuffer
/// @see #StringBuf
typedef StringBuf PathBuf;

/// @brief Create new path slice.
/// @param length (usize) Byte length of path.
/// @param start  (char*) Pointer to start of path.
/// @return Path slice.
#define path_new( length, start )  string_new( length, start )
/// @brief Create empty path slice.
/// @return Path slice.
#define path_empty()               string_empty()
/// @brief Create path from null terminated C string.
/// @param c_string (const char*) Null-terminated UTF-8 string.
/// @return Path slice.
#define path_from_cstr( c_string ) string_from_cstr( c_string )
/// @brief Create immutable path from string literal.
/// @param literal (string literal) String.
/// @return Path slice.
#define path_text( literal )       string_text( literal )
/// @brief Check if path is empty.
/// @note Same as string_is_empty()
/// @param path (Path) Path slice to check.
/// @return
///     - @c true  : @c path is empty.
///     - @c false : @c path is not empty.
#define path_is_empty( path )      string_is_empty( path )
/// @brief Compare two paths for equality.
/// @note Same as string_cmp()
/// @param a, b (Path) Paths to compare.
/// @return
///     - @c true  : @c a and @c b are equal in length and contents.
///     - @c false : @c a and @c b are not equal.
#define path_cmp( a, b )           string_cmp( a, b )

/// @brief Count how many path chunks are in path.
/// @param path Path to get chunk count of.
/// @return Number of chunks path has.
attr_core_api usize path_chunk_count( Path path );
/// @brief Split path by its chunks.
/// @param      path          Path to split.
/// @param      chunk_buf_cap Capacity of @c chunk_buf.
/// @param[out] chunk_buf     Pointer to store chunks. Must be able to hold @c chunk_buf_cap.
/// @param[out] out_count     Pointer to write number of chunks written to @c chunk_buf.
/// @return Number of chunks that could not be written to @c chunk_buf.
attr_core_api usize path_split_chunks(
    Path path, usize chunk_buf_cap, Path* chunk_buf, usize* out_count );
/// @brief Check if path looks like an absolute path for current platform.
/// @note This function does not check if @c path is a real path.
/// @details
/// - Windows : A path is absolute if it matches this pattern: [a-zA-Z]:[/\\]
/// - Posix   : A path is absolute if it starts with '/'
/// @param path Path to check.
/// @return
///     - @c true  : @c path looks like an absolute path.
///     - @c false : @c path does not look like an absolute path.
attr_core_api b32 path_is_absolute( Path path );
/// @brief Check if path looks like a relative path.
/// @note This function does not check if @c path is a real path.
/// @details
/// Currently, just checks if @c path is not absolute.
/// @param path Path to check.
/// @return
///     - @c true  : @c path looks like a relative path.
///     - @c false : @c path does not look like a relative path.
/// @see path_is_absolute()
attr_core_api b32 path_is_relative( Path path );
/// @brief Get path up the current chunk in path.
/// @details
/// For path: '/some/path/to/file'
///
/// Result:   '/some/path/to'
/// @param      path       Path to get parent of.
/// @param[out] out_parent Pointer to write parent to.
/// @return
///     - @c true  : @c path has parent path and result written to @c out_parent.
///     - @c false : @c path has no parent path.
attr_core_api b32 path_get_parent( Path path, Path* out_parent );
/// @brief Get file name of path.
/// @note This function does not check if path actually points
/// to a file or some other thing (like a directory).
/// @param      path          Path to get file name of.
/// @param[out] out_file_name Pointer to write file name to.
/// @return
///     - @c true  : @c path has a file name and it was written to @c out_file_name.
///     - @c false : @c path has no file name. (empty or ends with separator)
attr_core_api b32 path_get_file_name( Path path, Path* out_file_name );
/// @brief Get file stem of path. (file name without extension)
/// @note This function does not check if path actually points
/// to a file or some other thing (like a directory).
/// @param      path          Path to get file stem of.
/// @param[out] out_file_stem Pointer to write file stem to.
/// @return
///     - @c true  : @c path has a file stem and it was written to @c out_file_stem.
///     - @c false : @c path has no file stem. (empty or ends with separator)
attr_core_api b32 path_get_file_stem( Path path, Path* out_file_stem );
/// @brief Get file extension from path.
/// @note This function does not check if path actually points
/// to a file or some other thing (like a directory).
/// @param      path          Path to get extension from.
/// @param[out] out_extension Pointer to write extension to. (includes leading dot)
/// @return
///     - @c true  : @c path has an extension and it was written to @c out_extension.
///     - @c false : @c path has no extension.
attr_core_api b32 path_get_extension( Path path, Path* out_extension );
/// @brief Check if path is null terminated.
/// @details
/// File system functions allocate space create a null terminated path
/// if path provided is not null terminated.
/// Most platforms do not support string slices as paths so this 
/// function exists to check if allocation is necessary for doing
/// any file system operations.
/// @param path Path to check.
/// @return
///     - @c true  : @c path is null terminated.
///     - @c false : @c path is not null terminated.
attr_always_inline
attr_header b32 path_is_null_terminated( Path path ) {
    if( !path.len ) {
        return false;
    }
    return !path.c[path.len - 1] || !path.c[path.len];
}
/// @brief Change all path separators to given separator.
/// @param path Path to set separators for.
/// @param sep  Separator to set to. (should be forward or backslash)
attr_core_api void path_mut_set_separators( Path path, char sep );
/// @brief Stream path with separators set to given separator.
/// @param[in] stream Pointer to streaming function.
/// @param[in] target (optional) Pointer to streaming target.
/// @param     path   Path to stream.
/// @param     sep    Separator to stream.
/// @return Number of bytes that could not be written to stream target.
attr_core_api usize path_stream_set_separators(
    StreamBytesFN* stream, void* target, Path path, char sep );

/// @brief Create new path buffer.
/// @param capacity (usize) Size of path buffer.
/// @param start    (char*) Pointer to start of path buffer.
/// @return Path buffer.
#define path_buf_new( capacity, start ) string_buf_new( capacity, start )
/// @brief Create empty path buffer.
/// @return Path buffer.
#define path_buf_empty()                string_buf_empty()
/// @brief Initialize a path buffer from the stack.
/// @details
/// Defines a stack char buffer with name @c name\#\#_buffer
/// of given size and a PathBuf from that buffer with given name.
/// @param name (valid identifier) Name of path buffer.
/// @param size (usize)            Size of path buffer.
#define path_buf_create_from_stack( name, size )\
    string_buf_create_from_stack( name, size )
/// @brief Create path buffer using allocator.
/// @param      size      (usize)               Size of path buffer.
/// @param[in]  allocator (AllocatorInterface*) Interface for allocator to use.
/// @param[out] out_buf   (PathBuf*)            Pointer to write new buffer to.
/// @return
///     - @c true  : Allocated path buffer successfully.
///     - @c false : Failed to allocate path buffer.
#define path_buf_from_alloc( size, allocator, out_buf )\
    string_buf_from_alloc( size, allocator, out_buf )
/// @brief Reallocate path buffer.
/// @param[in] buf       (PathBuf*)            Path buffer to grow.
/// @param     amount    (usize)               Number of bytes to grow buffer by.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator.
/// @return
///     - @c true  : Reallocated buffer successfully.
///     - @c false : Failed to reallocate buffer.
#define path_buf_grow( buf, amount, allocator )\
    string_buf_grow( buf, amount, allocator )
/// @brief Free path buffer allocated with allocator.
/// @param[in] buf       (PathBuf*)            Path buffer to free.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator.
#define path_buf_free( buf, allocator )\
    string_buf_free( buf, allocator )
/// @brief Calculate remaining capacity in path buffer.
/// @param[in] buf (PathBuf*) Path buffer.
/// @return Number of bytes remaining. Does not include null terminator.
#define path_buf_remaining( buf )\
    string_buf_remaining( buf )
/// @brief Check if path buffer is empty.
/// @param[in] buf (PathBuf*) Path buffer to check.
/// @return
///     - @c true  : @c buf is empty.
///     - @c false : @c buf is not empty.
#define path_buf_is_empty( buf )\
    string_buf_is_empty( buf )
/// @brief Check if path buffer still has space.
/// @note Path buffer always tries to have space for null terminator.
/// @param[in] buf (PathBuf*) Path to check.
/// @return
///     - @c true : @c buf is full.
#define path_buf_is_full( buf )\
    string_buf_is_full( buf )
/// @brief Set path buffer length to zero and zero out memory.
/// @param[in] buf (PathBuf*) Path to clear.
#define path_buf_clear( buf )\
    string_buf_clear( buf )

/// @brief Attempt to push chunk to end of path buffer.
/// @details
/// It always uses / as a path separator regardless of platform.
/// @param[in] buf   Path buffer to push chunk to.
/// @param     chunk Path chunk to push.
/// @return
///     - @c true  : @c buf has enough space for chunk and it was pushed successfully.
///     - @c false : @c buf does not have enough space for chunk.
attr_core_api b32 path_buf_try_push( PathBuf* buf, Path chunk );
/// @brief Push chunk to end of path buffer.
/// @details
/// It always uses / as a path separator regardless of platform.
/// @param[in] buf       Path buffer to push chunk to.
/// @param     chunk     Path chunk to push.
/// @param[in] allocator Pointer to allocator.
/// @return
///     - @c true  : Pushed chunk successfully. If allocation was required, it was successful.
///     - @c false : Failed to reallocate @c buf.
attr_core_api b32 path_buf_push(
    PathBuf* buf, Path chunk, struct AllocatorInterface* allocator );
/// @brief Remove last chunk from path.
/// @param[in] buf Path buffer to pop chunk off of.
/// @return
///     - @c true  : @c buf had chunk to pop and it was popped off.
///     - @c false : @c buf has no more chunks to pop.
attr_core_api b32 path_buf_pop( PathBuf* buf );
/// @brief Attempt to set path extension.
/// @details
/// Does not check if path is a directory, just blindly sets extension.
///
/// If path has no extension, appends extension.
///
/// If path has an extension, removes that extension and sets new extension.
///
/// If extension does not begin with dot, also appends dot before extension.
/// @param[in] buf       Path buffer to set extension of.
/// @param     extension Extension to set.
/// @return
///     - @c true  : @c buf has enough capacity to set new extension.
///     - @c false : @c buf does not have capacity to set new extension.
attr_core_api b32 path_buf_try_set_extension( PathBuf* buf, Path extension );
/// @brief Set path extension.
/// @details
/// Does not check if path is a directory, just blindly sets extension.
///
/// If path has no extension, appends extension.
///
/// If path has an extension, removes that extension and sets new extension.
///
/// If extension does not begin with dot, also appends dot before extension.
/// @param[in] buf       Path buffer to set extension of.
/// @param     extension Extension to set.
/// @param[in] allocator Pointer to allocator.
/// @return
///     - @c true  : Set extension. If allocation was required, it was successful.
///     - @c false : Failed to reallocate path.
attr_core_api b32 path_buf_set_extension(
    PathBuf* buf, Path extension, struct AllocatorInterface* allocator );

// TODO(alicia): file_path_is_file, file_path_is_directory
// /// @brief Check if path points to a file.
// /// @param path Path to check.
// /// @return True if path points to a file.
// attr_core_api b32 path_is_file( const Path path );
// /// @brief Check if path points to a directory.
// /// @param path Path to check.
// /// @return True if path points to a directory.
// attr_core_api b32 path_is_directory( const Path path );

#endif /* header guard */
