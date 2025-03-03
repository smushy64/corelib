#if !defined(CORE_INTERNAL_PLATFORM_MEMORY_H) && defined(CORE_ENABLE_INTERNAL)
#define CORE_INTERNAL_PLATFORM_MEMORY_H
/**
 * @file   memory.h
 * @brief  Platform memory functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 06, 2025
*/
#include "core/types.h"

void* platform_heap_alloc( void* opt_old_ptr, usize opt_old_size, usize new_size );
void  platform_heap_free( void* buffer, usize size );

#endif /* header guard */
