#if !defined(CORE_INTERNAL_PLATFORM_MISC_H) && defined(CORE_ENABLE_INTERNAL)
#define CORE_INTERNAL_PLATFORM_MISC_H
/**
 * @file   misc.h
 * @brief  Platform miscellaneous functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 06, 2025
*/
struct SystemInfo;

void platform_system_query_info( struct SystemInfo* out_info );

#endif /* header guard */
