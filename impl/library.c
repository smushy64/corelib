/**
 * Description:  Dynamic library implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 27, 2024
*/
#include "core/prelude.h"
#include "core/library.h"

#include "core/internal/platform/library.h"

attr_core_api LibraryHandle* library_open( const char* name ) {
    return platform_library_open( name );
}
attr_core_api LibraryHandle* library_get( const char* name ) {
    return platform_library_get( name );
}
attr_core_api void library_close( LibraryHandle* lib ) {
    if( lib ) {
        platform_library_close( lib );
    }
}
attr_core_api void* library_load( LibraryHandle* lib, const char* function ) {
    return platform_library_load( lib, function );
}

