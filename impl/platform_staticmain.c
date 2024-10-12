/**
 * @file   platform_staticmain.c
 * @brief  Static compilation main.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 24, 2024
*/
#include "core/defines.h"

#if defined(CORE_PLATFORM_WINDOWS)
    #include "impl/platform_win32_staticmain.c"
#endif

#if defined(CORE_PLATFORM_POSIX)
    #include "impl/platform_posix_staticmain.c"
#endif
