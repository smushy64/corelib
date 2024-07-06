/**
 * @file   platform_win32_sharedmain.c
 * @brief  Shared object main for Windows.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 26, 2024
*/
#include "core/defines.h"

#if defined(CORE_PLATFORM_WINDOWS) && !defined(CORE_ENABLE_STATIC_BUILD)
#include "core/types.h"
#include "core/macros.h"
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

attr_clink b32 win32_init(void);
attr_clink void win32_deinit(void);

BOOL WINAPI DllMainCRTStartup(
    HINSTANCE const instance,
    DWORD     const reason,
    LPVOID    const reserved
) {
    unused( instance, reserved );
    switch( reason ) {
        case DLL_PROCESS_ATTACH: {
            return win32_init();
        } break;
        case DLL_PROCESS_DETACH: {
            win32_deinit();
        } break;
        case DLL_THREAD_ATTACH: break;
        case DLL_THREAD_DETACH: break;
    }
    return TRUE;
}

#endif
