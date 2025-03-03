#if !defined(CORE_LIBRARY_H)
#define CORE_LIBRARY_H
/**
 * @file   library.h
 * @brief  Dynamic library management.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 27, 2024
*/
#include "core/attributes.h"

struct _StringPOD;

/// @brief Opaque handle to a library.
///
/// Must always be a pointer.
typedef void LibraryHandle;

/// @brief Open a library.
/// @param[in] name Name of library, does not require extension.
/// @return Handle to library, NULL if failed to open.
attr_core_api
LibraryHandle* library_open( struct _StringPOD name );
/// @brief Get a handle to a previously opened library.
///
/// Do not try to close library with this handle!
/// @param[in] name Name of library, does not require extension.
/// @return Handle to library, NULL if library has not already been opened.
attr_core_api
LibraryHandle* library_get( struct _StringPOD name );
/// @brief Close a library handle.
/// @param[in] lib Library to close.
attr_core_api
void library_close( LibraryHandle* lib );
/// @brief Load a function from a library.
/// @param[in] lib Handle to library.
/// @param[in] function Null-terminated name of function to load.
/// @return Function pointer, NULL if unable to load.
attr_core_api
void* library_load( LibraryHandle* lib, struct _StringPOD function );

#endif /* header guard */
