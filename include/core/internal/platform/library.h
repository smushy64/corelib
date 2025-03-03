#if !defined(CORE_INTERNAL_PLATFORM_LIBRARY_H) && defined(CORE_ENABLE_INTERNAL)
#define CORE_INTERNAL_PLATFORM_LIBRARY_H
/**
 * @file   library.h
 * @brief  Platform library functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 06, 2025
*/

struct _StringPOD;

void* platform_library_open( struct _StringPOD name );
void* platform_library_get( struct _StringPOD name );
void platform_library_close( void* lib );
void* platform_library_load( void* lib, struct _StringPOD function );

#endif /* header guard */
