/**
 * @file   platform_dllmain.c
 * @brief  Shared object main.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 04, 2024
*/
#include "core/defines.h"

#if defined(CORE_PLATFORM_WINDOWS)
    #include "src/core/platform_win32_sharedmain.c"
#endif

#if defined(CORE_PLATFORM_POSIX)
    #include "src/core/platform_posix_sharedmain.c"
#endif


