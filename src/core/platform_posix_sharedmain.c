/**
 * @file   platform_posix_sharedmain.c
 * @brief  Posix shared object main implementation.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   October 11, 2024
*/
#include "core/defines.h"
#if defined(CORE_PLATFORM_POSIX)
#include "core/types.h"
#include <stdlib.h>

extern b32 posix_init(void);
extern void posix_shutdown(void);

__attribute__((__constructor__))
void posix_constructor(void) {
    if( !posix_init() ) {
        exit(1);
    }
}
__attribute__((__destructor__))
void posix_destructor(void) {
    posix_shutdown();
}

#endif /* Platform Posix */
