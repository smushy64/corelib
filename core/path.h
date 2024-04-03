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

/// @brief Slice of a character buffer.
/// @see #CoreSlice
typedef struct CoreSlice  Path;
/// @brief Mutable character buffer.
/// @see #CoreBuffer
typedef struct CoreBuffer PathBuf;

/// @brief Create a new path from character buffer and a length.
/// @param[in] cc Pointer to character buffer.
/// @param len Length of character buffer.
/// @return Path.
attr_always_inline
attr_header Path path_new( const char* cc, usize len ) {
    Path result;
    result.cc  = cc;
    result.len = len;
    return result;
}
/// @brief Create a new const Path from string literal.
/// @param literal Null-terminated string literal.
/// @return Path
#define path_text( literal ) string_text( literal )
/// @brief Create a new path from null-terminated path.
/// @param opt_len (optional) Length of path (if 0, length is calculated).
/// @param[in] ascii Pointer to path.
/// @return Path
attr_always_inline
attr_header Path path_from_asciiz( usize opt_len, const char* ascii ) {
    Path result;
    result.cc  = ascii;
    result.len = opt_len ? opt_len : asciiz_len( ascii );
    return result;
}
/// @brief Get pointer to path's buffer.
/// @param path Path to get pointer of.
/// @return Pointer to path's buffer.
attr_always_inline
attr_header const char* path_str( const Path path ) {
    return path.cc;
}
/// @brief Get length of path.
/// @param path Path to get length of.
/// @return Length of path.
attr_always_inline
attr_header usize path_len( const Path path ) {
    return path.len;
}
/// @brief Compare two paths for equality.
/// @see #string_cmp()
/// @param a, b Paths to compare.
/// @return True if paths are the same.
attr_always_inline
attr_header b32 path_cmp( const Path a, const Path b ) {
    return string_cmp( a, b );
}
/// @brief Check if a path is an absolute path.
/// @param path Path to check.
/// @return True if path is absolute.
attr_core_api b32 path_is_absolute( const Path path );
/// @brief Check if a path is a relative path.
/// @param path Path to check.
/// @return True if path is relative.
attr_always_inline
attr_header b32 path_is_relative( const Path path ) {
    return !path_is_absolute( path );
}
/// @brief Check if path points to a file.
/// @param path Path to check.
/// @return True if path points to a file.
attr_core_api b32 path_is_file( const Path path );
/// @brief Check if path points to a directory.
/// @param path Path to check.
/// @return True if path points to a directory.
attr_core_api b32 path_is_directory( const Path path );
/// @brief Attempt to get path's parent.
/// @param path Path to get parent of.
/// @param[out] out_parent Destination to write parent of path to.
/// @return True if path had a parent.
attr_core_api b32 path_parent( const Path path, Path* out_parent );
/// @brief Get a count of how many ancestors a path has.
/// @param path Path to get ancestor count of.
/// @return Number of ancestors that path has.
attr_header usize path_ancestor_count( const Path path ) {
    Path inner_path = path;
    Path parent     = path_new( 0, 0 );
    usize result    = 0;
    while( path_parent( inner_path, &parent ) ) {
        inner_path = parent;
        result++;
    }
    return result;
}

/// @brief Write ancestors of given path to path array.
/// @param path Path to get ancestors of.
/// @param max Max number of ancestors in given ancestors array.
/// @param[out] out_ancestors Array to hold ancestors.
/// @param[out] out_written Number of ancestors written to ancestors array.
/// @param[out] out_ancestor_count Number of ancestors that path actually has.
attr_header void path_ancestors(
    const Path path, usize max, Path* out_ancestors,
    usize* out_written, usize* out_ancestor_count
) {
    Path inner_path = path;
    Path parent     = path_new(0, 0);
    usize count   = 0;
    usize written = 0;

    while( path_parent( inner_path, &parent ) ) {
        if( written < max ) {
            out_ancestors[written++] = parent;
        }
        inner_path = parent;
        count++;
    }

    *out_written        = written;
    *out_ancestor_count = count;
}
/// @brief Get file name of path.
/// @param path Path to get file name of.
/// @param[out] out_file_name Path slice containing just file name (including extension).
/// @return True if path points to a file.
/// @note Path might not actually point to a file, this function only checks for trailing path separator.
/// @see #path_is_file() to check if path points to a real file.
attr_core_api b32 path_file_name( const Path path, Path* out_file_name );
/// @brief Get file stem of path.
/// @param path Path to get file stem of.
/// @param[out] out_file_stem Path slice containing just file name (excludes extension).
/// @return True if path points to a file.
/// @note Path might not actually point to a file, this function only checks for trailing path separator.
/// @see #path_is_file() to check if path points to a real file.
attr_core_api b32 path_file_stem( const Path path, Path* out_file_stem );
/// @brief Get file extension of path.
/// @param path Path to get file extension of.
/// @param[out] out_extension Path slice containing file extension with leading dot.
/// @return True if path points to a file and has an extension.
attr_core_api b32 path_extension( const Path path, Path* out_extension );
/// @brief Pop off last chunk of path.
/// @param path Path to pop chunk off of.
/// @param[out] out_path Path with last chunk popped.
/// @param[out] opt_out_chunk (optional) Chunk that was popped off.
/// @return True if path had a chunk to pop off.
attr_core_api b32 path_pop( const Path path, Path* out_path, Path* opt_out_chunk );
/// @brief Convert chunk separators from either back or forward slashes to opposite.
/// @param[in] path Path to convert separators of.
/// @param to_forward If true, convert back slashes to forward slashes, if false vice versa.
attr_core_api void path_convert_separators( Path* path, b32 to_forward );
/// @brief Stream convert chunk separators from either back or forward slashes to opposite.
/// @param stream Streaming function.
/// @param[in] target Target to write result to.
/// @param path Path to convert separators of.
/// @param to_forward If true, convert back slashes to forward slashes, if false vice versa.
/// @return Number of bytes required to complete conversion.
/// @see #StreamBytesFN
attr_core_api usize path_stream_convert_separators(
    StreamBytesFN* stream, void* target, const Path path, b32 to_forward );

/// @brief Create a new path buffer.
/// @param size Size of buffer.
/// @param[in] buffer Pointer to start of buffer.
/// @return Path buffer.
attr_always_inline
attr_header PathBuf path_buf_new( const usize size, char* buffer ) {
    PathBuf result;
    result.c   = buffer;
    result.len = 0;
    result.cap = size;
    return result;
}
/// @brief Create a new path buffer from static array.
/// @param array Static, local character array.
/// @return Path buffer.
#define path_buf_from_array( array )\
    path_buf_new( static_array_len( (array) ), (array) )
/// @brief Get pointer to path buffer's buffer.
/// @param[in] buf Path buffer.
/// @return Pointer to start of buffer.
attr_always_inline
attr_header char* path_buf_str( PathBuf* buf ) {
    return buf->c;
}
/// @brief Get path buffer's length.
/// @param[in] buf Path buffer.
/// @return Length of path buffer.
attr_always_inline
attr_header usize path_buf_len( const PathBuf* buf ) {
    return buf->len;
}
/// @brief Get path buffer's capacity in bytes.
/// @param[in] buf Path buffer.
/// @return Capacity of path buffer in bytes.
attr_always_inline
attr_header usize path_buf_cap( const PathBuf* buf ) {
    return buf->cap;
}
/// @brief Check if path buffer is empty.
/// @param[in] buf Path buffer.
/// @return True if path buffer is empty.
attr_always_inline
attr_header b32 path_buf_is_empty( const PathBuf* buf ) {
    return buf->len == 0;
}
/// @brief Check if path buffer is full.
/// @param[in] buf Path buffer.
/// @return True if path buffer is full.
attr_always_inline
attr_header b32 path_buf_is_full( const PathBuf* buf ) {
    return buf->len == buf->cap;
}
/// @brief Clear a path buffer.
///
/// Sets length to zero.
/// @param[in] buf Buffer to clear.
attr_always_inline
attr_header void path_buf_clear( PathBuf* buf ) {
    buf->len = 0;
}
/// @brief Push a path chunk to end of path buffer.
/// @param[in] path Path buffer to push to.
/// @param chunk Path chunk to push.
/// @return True if path buffer had enough space to push chunk.
attr_core_api b32 path_buf_push( PathBuf* path, const Path chunk );
/// @brief Pop the last chunk off of path buffer.
/// @param[in] path Path to pop chunk off of.
/// @param[out] opt_out_chunk (optional) Pointer to receive popped chunk.
/// @return True if path had a chunk to pop off.
attr_always_inline
attr_header b32 path_buf_pop( PathBuf* path, Path* opt_out_chunk ) {
    Path new_path;
    if( !path_pop( path->slice, &new_path, opt_out_chunk ) ) {
        return false;
    }

    path->len = new_path.len;
    return true;
}
/// @brief Set the extension of a path buffer.
/// @param[in] path Path to modify.
/// @param ext Extension to set. Accepts extensions with or without leading dot.
/// @return True if path had enough capacity for new extension.
attr_core_api b32 path_buf_set_extension( PathBuf* path, const Path ext );

#endif /* header guard */
