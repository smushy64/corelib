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
#include "core/macros.h"
#include "core/stream.h"
#include "core/memory.h"
#include "core/string.h"

struct AllocatorInterface;

#if defined(CORE_PLATFORM_WINDOWS)
    #if defined(CORE_CPLUSPLUS)
        /// @brief Current platform's path character.
        typedef wchar_t PathCharacter;
    #else
        /// @brief Current platform's path character.
        typedef c16 PathCharacter;
    #endif
    /// @brief Create a path character literal.
    /// @param literal (character literal) Character literal.
    /// @return Path character literal.
    #define path_raw_char( literal ) L##literal
    /// @brief Create a path string literal.
    /// @param literal (string literal) String literal.
    /// @return Path string literal.
    #define path_raw_string( literal ) L##literal
    /// @brief Path separator character for current platform.
    #define PATH_SEPARATOR path_raw_char( '\\' )
#else
    /// @brief Current platform's path character.
    typedef char PathCharacter;
    /// @brief Create a path character literal.
    /// @param literal (character literal) Character literal.
    /// @return Path character literal.
    #define path_raw_char( literal ) literal
    /// @brief Create a path string literal.
    /// @param literal (string literal) String literal.
    /// @return Path string literal.
    #define path_raw_string( literal ) literal
    /// @brief Path separator character for current platform.
    #define PATH_SEPARATOR path_raw_char( '/' )
#endif


/// @brief Path string slice.
typedef struct Path {
    /// @brief Number of characters in slice.
    usize len;
    /// @brief Anonymous union of slice pointers.
    union {
        /// @brief Raw path string pointer.
        PathCharacter* raw;
        /// @brief Raw path string const pointer.
        const PathCharacter* const_raw;
    };
} Path;

/// @brief Path string buffer.
typedef struct PathBuf {
    /// @brief Number of characters path buffer can hold.
    usize cap;
    /// @brief Anonymous union of path slice components and path slice.
    union {
        /// @brief Anonymous struct of path slice components.
        struct {
            /// @brief Number of characters in slice.
            usize len;
            /// @brief Anonymous union of slice pointers.
            union {
                /// @brief Raw path string pointer.
                PathCharacter* raw;
                /// @brief Raw path string const pointer.
                const PathCharacter* const_raw;
            };
        };
        /// @brief Path string slice.
        Path slice;
    };
} PathBuf;

/// @brief Create a new path slice from length and pointer.
/// @param     length (usize)          Length of raw path string.
/// @param[in] start  (PathCharacter*) Pointer to start of raw path string.
/// @return Path slice.
#define path_new( length, start )\
    struct_literal(Path){ .len=length, .const_raw=start }
/// @brief Create an empty path slice.
/// @return Path slice.
#define path_empty() path_new( 0, 0 )
/// @brief Create a path slice from path literal.
/// @param[in] literal (string literal) Path literal.
/// @return Path slice.
#define path_text( literal )\
    path_new(\
        (sizeof((path_raw_string(literal))) / sizeof(PathCharacter)) - 1,\
        path_raw_string(literal)\
    )
/// @brief Create a path slice from raw path string.
/// @details
/// If @c len is 0, calculates length of @c raw string.
/// This means @c raw must be null-terminated if @c len is 0.
/// @param len (usize)          (optional) Length of raw path string.
/// @param raw (PathCharacter*) Pointer to start of raw path string.
/// @return Path slice.
#define path_from_raw( len, raw )\
    path_new( len ? len : path_raw_len(raw), raw )
/// @brief Check if path slice is empty.
/// @param path (Path) Path to check.
/// @return
///     - @c true  : @c path is empty.
///     - @c false : @c path is not empty.
#define path_is_empty( path ) ((path).len == 0)

/// @brief Calculate length of null-terminated raw path string.
/// @param[in] raw Pointer to start of raw path string.
/// @return Number of characters in @c raw.
attr_core_api usize path_raw_len( const PathCharacter* raw );
/// @brief Compare two paths for equality.
/// @param a, b Paths to compare.
/// @return
///     - @c true  : @c a and @c b are equal in length and contents.
///     - @c false : @c a and @c b are not equal.
attr_core_api b32 path_cmp( Path a, Path b );

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
    return !path.const_raw[path.len - 1] || !path.const_raw[path.len];
}
/// @brief Set path separators to posix separators.
/// @details
/// If path contains any windows separators '\\', this function
/// converts them to posix separators '/'.
/// @param path Path to modify. @c .raw must not be a path literal.
attr_core_api void path_set_posix_separators( Path path );
/// @brief Set path separators to windows separators.
/// @details
/// If path contains any posix separators '/', this function
/// converts them to windows separators '\\'.
/// @param path Path to modify. @c .raw must not be a path literal.
attr_core_api void path_set_windows_separators( Path path );
#if defined(CORE_PLATFORM_WINDOWS)
    #define path_set_platform_separators path_set_windows_separators
#else
    #define path_set_platform_separators path_set_posix_separators
#endif

/// @brief Stream path with windows separators changed to posix separators.
/// @details
/// If path contains any windows separators '\\', this function
/// converts them to posix separators '/'.
/// @param[in] stream Pointer to streaming function.
/// @param[in] target (optional) Pointer to streaming target.
/// @param     path   Path to stream.
/// @return Number of bytes that could not be written to stream target.
attr_core_api usize path_stream_set_posix_separators(
    StreamBytesFN* stream, void* target, Path path );
/// @brief Stream path with posix separators changed to windows separators.
/// @details
/// If path contains any posix separators '/' this function
/// converts them to windows separators '\\'.
/// @param[in] stream Pointer to streaming function.
/// @param[in] target (optional) Pointer to streaming target.
/// @param     path   Path to stream.
/// @return Number of bytes that could not be written to stream target.
attr_core_api usize path_stream_set_windows_separators(
    StreamBytesFN* stream, void* target, Path path );
#if defined(CORE_PLATFORM_WINDOWS)
    #define path_stream_set_platform_separators path_stream_set_windows_separators
#else
    #define path_stream_set_platform_separators path_stream_set_posix_separators
#endif

/// @brief Stream convert path characters to UTF-8.
/// @details
/// On Windows, paths use UTF-16 encoding so this function converts
/// path characters to UTF-8 for printing or other purposes where
/// UTF-8 encoding is required.
/// On other platforms, this function just
/// streams path to target without modifying it.
/// @param[in] stream Pointer to streaming function.
/// @param[in] target (optional) Pointer to streaming target.
/// @param     path   Path to convert.
/// @return Number of bytes that could not be written to streaming target.
attr_core_api usize path_stream_convert_to_utf8(
    StreamBytesFN* stream, void* target, Path path );
/// @brief Stream canonical path.
/// @details
/// Converts relative/home path to full path.
/// All . are removed and .. remove the previous directory.
/// ~ is converted to user's home path.
///
/// @note
/// Target should be @c PathBuf as the canonical path is streamed as
/// the current platform's PathCharacter. To stream to a UTF-8 target,
/// use path_stream_canonicalize_utf8().
/// @param[in] stream Pointer to streaming function.
/// @param[in] target (optional) Target to stream to.
/// @param     path   Path to convert.
/// @return Number of bytes that could not be written to streaming target.
attr_core_api usize path_stream_canonicalize(
    StreamBytesFN* stream, void* target, Path path );
#if defined(CORE_PLATFORM_WINDOWS)
/// @brief Stream canonical path.
/// @details
/// Converts relative/home path to full path.
/// All . are removed and .. remove the previous directory.
/// ~ is converted to user's home path.
///
/// @note
/// Target should never be @c PathBuf as the canonical path is streamed as
/// UTF-8 instead of current platform's PathCharacter. To stream to a PathBuf target,
/// use path_stream_canonicalize().
/// @param[in] stream Pointer to streaming function.
/// @param[in] target (optional) Target to stream to.
/// @param     path   Path to convert.
/// @return Number of bytes that could not be written to streaming target.
attr_core_api usize path_stream_canonicalize_utf8(
    StreamBytesFN* stream, void* target, Path path );
#else
attr_always_inline
attr_header usize path_stream_canonicalize_utf8(
    StreamBytesFN* stream, void* target, Path path
) {
    return path_stream_canonicalize( stream, target, path );
}
#endif
/// @brief Path buffer stream function.
/// @param[in] target_PathBuf Pointer to PathBuf.
/// @param     bytes          Byte size of @c buffer.
/// @param[in] buffer         Pointer to buffer to stream to @c target_PathBuf.
/// @return Number of bytes that could not be written to path buf.
attr_core_api usize path_buf_stream(
    void* target_PathBuf, usize bytes, const void* buffer );
/// @brief Create path buffer from UTF-8 String.
/// @param      utf8_path String containing UTF-8 encoded path.
/// @param[in]  allocator Pointer to allocator interface.
/// @param[out] out_buf   Pointer to write result of conversion.
/// @return
///     - @c true  : Successfully converted UTF-8 path to system path.
///     - @c false : Failed to convert UTF-8 path to system path.
attr_core_api b32 path_buf_from_string(
    String utf8_path, struct AllocatorInterface* allocator, PathBuf* out_buf );
/// @brief Copy UTF-8 path into path buffer.
/// @param[in] buf       Pointer to path buffer to copy into.
/// @param     utf8_path UTF-8 path to copy.
/// @param[in] allocator Pointer to allocator interface.
/// @return
///     - @c true  : Successfully converted UTF-8 path to system path.
///     - @c false : Failed to convert UTF-8 path to system path.
attr_core_api b32 path_buf_copy_from_string(
    PathBuf* buf, String utf8_path, struct AllocatorInterface* allocator );
/// @brief Try to copy UTF-8 path into existing path buffer.
/// @param[in] buf       Pointer to write result of conversion.
/// @param     utf8_path String containing UTF-8 encoded path.
/// @return
///     - @c true  : Successfully converted UTF-8 path to system path.
///     - @c false : Failed to convert UTF-8 path to system path.
attr_core_api b32 path_buf_try_copy_from_string( PathBuf* buf, String utf8_path );
/// @brief Stream conversion UTF-8 path to system path.
/// @param[in] stream    Pointer to streaming function.
/// @param[in] target    (optional) Pointer to target to receive result.
/// @param     utf8_path String containing UTF-8 encoded path.
/// @return Number of bytes that could not be written to stream target.
attr_core_api usize path_stream_convert_from_utf8(
    StreamBytesFN* stream, void* target, String utf8_path );

/// @brief Create new path buffer.
/// @param capacity (usize)          Size of path buffer.
/// @param start    (PathCharacter*) Pointer to start of path buffer.
/// @return Path buffer.
#define path_buf_new( capacity, start )\
    struct_literal(PathBuf){ .cap=capacity, .len=0, .const_raw=start }
/// @brief Create empty path buffer.
/// @return Path buffer.
#define path_buf_empty() path_buf_new( 0, 0 )
/// @brief Initialize a path buffer from the stack.
/// @details
/// Defines a stack PathCharacter buffer with name @c name.
/// of given size and a PathBuf from that buffer with given name.
/// @param name (valid identifier) Name of path buffer.
/// @param size (usize)            Size of path buffer.
#define path_buf_create_from_stack( name, size )\
    PathCharacter name##_buffer[size];\
    memory_zero( name##_buffer, sizeof(name##_buffer) );\
    PathBuf name = path_buf_new( size, name##_buffer )
/// @brief Create path buffer using allocator.
/// @param      capacity  Number of characters in buffer.
/// @param[in]  allocator Interface for allocator to use.
/// @param[out] out_buf   Pointer to write new buffer to.
/// @return
///     - @c true  : Allocated path buffer successfully.
///     - @c false : Failed to allocate path buffer.
attr_core_api b32 path_buf_from_alloc(
    usize capacity, struct AllocatorInterface* allocator, PathBuf* out_buf );
/// @brief Create path buffer from existing path.
/// @param      path      Path to create buffer from.
/// @param[in]  allocator Pointer to allocator interface.
/// @param[out] out_buf   Pointer to write new buffer to.
/// @return
///     - @c true  : Allocated path buffer successfully.
///     - @c false : Failed to allocate path buffer.
attr_core_api b32 path_buf_from_path(
    Path path, struct AllocatorInterface* allocator, PathBuf* out_buf );
/// @brief Copy path into path buffer.
/// @param[in] buf       Pointer to path buffer to copy to.
/// @param     path      Path to copy.
/// @param[in] allocator Pointer to allocator interface.
/// @return
///     - @c true  : Copied path successfully.
///     - @c false : Failed to allocate space to copy path.
attr_core_api b32 path_buf_copy_from_path(
    PathBuf* buf, Path path, struct AllocatorInterface* allocator );
/// @brief Attempt to copy path into path buffer.
/// @param[in] buf  Pointer to path buffer to copy to.
/// @param     path Path to copy.
/// @return
///     - @c true  : Buffer had enough space to copy.
///     - @c false : Buffer did not have enough space to copy.
attr_core_api b32 path_buf_try_copy_from_path( PathBuf* buf, Path path );
/// @brief Reallocate path buffer.
/// @param[in] buf       Path buffer to grow.
/// @param     amount    Number of bytes to grow buffer by.
/// @param[in] allocator Pointer to allocator.
/// @return
///     - @c true  : Reallocated buffer successfully.
///     - @c false : Failed to reallocate buffer.
attr_core_api b32 path_buf_grow(
    PathBuf* buf, usize amount, struct AllocatorInterface* allocator );
/// @brief Free path buffer allocated with allocator.
/// @param[in] buf       Path buffer to free.
/// @param[in] allocator Pointer to allocator.
attr_core_api void path_buf_free(
    PathBuf* buf, struct AllocatorInterface* allocator );
/// @brief Calculate remaining capacity in path buffer.
/// @param[in] buf Path buffer.
/// @return Number of bytes remaining. Does not include null terminator.
attr_always_inline
attr_header usize path_buf_remaining( const PathBuf* buf ) {
    return buf->cap ? ( buf->cap - 1 ) - buf->len : 0;
}
/// @brief Check if path buffer is empty.
/// @param[in] buf Path buffer to check.
/// @return
///     - @c true  : @c buf is empty.
///     - @c false : @c buf is not empty.
attr_always_inline
attr_header b32 path_buf_is_empty( const PathBuf* buf ) {
    return buf->len == 0;
}
/// @brief Check if path buffer still has space.
/// @note Path buffer always tries to have space for null terminator.
/// @param[in] buf Path to check.
/// @return
///     - @c true : @c buf is full.
attr_always_inline
attr_header b32 path_buf_is_full( const PathBuf* buf ) {
    return buf->len == ( buf->cap - 1 );
}
/// @brief Set path buffer length to zero and zero out memory.
/// @param[in] buf Path to clear.
attr_always_inline
attr_header void path_buf_clear( PathBuf* buf ) {
    memory_zero( buf->raw, sizeof(PathCharacter) * buf->len );
    buf->len = 0;
}
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

#endif /* header guard */
