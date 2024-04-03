/**
 * @file   platform_dllmain.c
 * @brief  Dll main
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 04, 2024
*/
#include "core/defines.h"
#if !defined(CORE_ENABLE_STATIC_BUILD)

#if defined(CORE_PLATFORM_LINUX) && defined(CORE_ENABLE_SHARED_EXECUTABLE)
    const char interp[] __attribute__((section(".interp"))) = "/lib64/ld-linux-x86-64.so.2";
#endif

#if defined(CORE_PLATFORM_WINDOWS)
#include "core/macros.h"

#if defined(CORE_CPLUSPLUS)
#undef NULL
#define NULL nullptr

extern "C" {
#endif

#include <windows.h>

BOOL WINAPI DllMainCRTStartup(
    HINSTANCE const instance,
    DWORD     const reason,
    LPVOID    const reserved
) {
    unused(instance, reserved);
    switch( reason ) {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}

#if defined(CORE_PLUSPLUS)
} /* extern C */
#endif

#endif /* Platform Windows */

#endif /* If CORE_ENABLE_STATIC_BUILD is not defined */
