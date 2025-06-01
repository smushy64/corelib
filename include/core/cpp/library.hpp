#if !defined(CORE_CPP_LIBRARY_HPP) && defined(__cplusplus)
#define CORE_CPP_LIBRARY_HPP
/**
 * @file   library.hpp
 * @brief  C++ Library.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 01, 2025
*/
#if !defined(CORE_LIBRARY_H)
    #include "core/library.h"
#endif
#include "core/string.h"

/// @brief Open a library.
/// @param[in] name Name of library, does not require extension.
/// @return Handle to library, NULL if failed to open.
attr_header
LibraryHandle* library_open( String name );
/// @brief Get a handle to a previously opened library.
///
/// Do not try to close library with this handle!
/// @param[in] name Name of library, does not require extension.
/// @return Handle to library, NULL if library has not already been opened.
attr_header
LibraryHandle* library_get( String name );
/// @brief Load a function from a library.
/// @param[in] lib Handle to library.
/// @param[in] function Null-terminated name of function to load.
/// @return Function pointer, NULL if unable to load.
attr_header
void* library_load( LibraryHandle* lib, String function );

// NOTE(alicia): implementation -----------------------------------------------

attr_always_inline attr_header attr_hot
LibraryHandle* library_open( String name ) {
    return library_open( name.__pod );
}
attr_always_inline attr_header attr_hot
LibraryHandle* library_get( String name ) {
    return library_get( name.__pod );
}
attr_always_inline attr_header attr_hot
void* library_load( LibraryHandle* lib, String function ) {
    return library_load( lib, function.__pod );
}

#endif /* header guard */
