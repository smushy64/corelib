#if !defined(CORE_INTERNAL_PLATFORM_LIBRARY_H) && defined(CORE_ENABLE_INTERNAL)
#define CORE_INTERNAL_PLATFORM_LIBRARY_H
/**
 * @file   library.h
 * @brief  Platform library functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 06, 2025
*/

void* platform_library_open( const char* name );
void* platform_library_get( const char* name );
void platform_library_close( void* lib );
void* platform_library_load( void* lib, const char* function );

#endif /* header guard */
