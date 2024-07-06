/**
 * @file   platform_staticmain.c
 * @brief  Static compilation main for Windows.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 24, 2024
*/
#include "core/defines.h"
#if defined(CORE_PLATFORM_WINDOWS) && defined(CORE_ENABLE_STATIC_BUILD)
#include "core/attributes.h"

extern int main( int argc, char** argv );

#include <windows.h>

extern void win32_init(void);
extern void win32_deinit(void);

attr_clink LPSTR* WINAPI CommandLineToArgvA( LPSTR lpCmdLine, int* numargs );

#if defined(CORE_ENABLE_STATIC_WINDOW_APP)
attr_no_return attr_clink void __stdcall WinMainCRTStartup(void) {
#else
attr_no_return attr_clink void __stdcall mainCRTSTartup(void) {
#endif
    int    argc = 0;
    char** argv = CommandLineToArgvA( GetCommandLineA(), &argc );

    win32_init();

    int exit_code = main( argc, argv );

    win32_deinit();

    LocalFree( argv );
    ExitProcess( exit_code );
}

#endif

