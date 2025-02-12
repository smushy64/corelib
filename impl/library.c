/**
 * Description:  Dynamic library implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 27, 2024
*/
#include "core/prelude.h"
#include "core/library.h"
#include "core/string.h"
#include "core/fs.h"

#include "core/internal/platform/library.h"
#include "core/internal/logging.h"

attr_core_api
LibraryHandle* library_open( struct _StringPOD name ) {
    if( name.len >= CORE_PATH_NAME_LEN ) {
        core_error(
            "core/library:library_open(): "
            "library name is longer than CORE_PATH_NAME_LEN! length: {usize}",
            name.len );
        return NULL;
    }
    return platform_library_open( name );
}
attr_core_api
LibraryHandle* library_get( struct _StringPOD name ) {
    if( name.len >= CORE_PATH_NAME_LEN ) {
        core_error(
            "core/library:library_get(): "
            "library name is longer than CORE_PATH_NAME_LEN! length: {usize}",
            name.len );
        return NULL;
    }
    return platform_library_get( name );
}
attr_core_api
void library_close( LibraryHandle* lib ) {
    if( lib ) {
        platform_library_close( lib );
    }
}
attr_core_api
void* library_load( LibraryHandle* lib, struct _StringPOD function ) {
    if( function.len >= CORE_PATH_NAME_LEN ) {
        core_error(
            "core/library:library_load(): "
            "function name is longer than CORE_PATH_NAME_LEN! length: {usize}",
            function.len );
        return NULL;
    }
    return platform_library_load( lib, function );
}

