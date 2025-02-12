/**
 * @file   platform_staticmain.c
 * @brief  Static compilation main for Windows.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 24, 2024
*/
#include "core/defines.h"
#if defined(CORE_PLATFORM_WINDOWS) && defined(CORE_ENABLE_STATIC_BUILD)
#include "core/attributes.h"
#include "core/types.h"

extern int crt_main( int argc, char** argv );

#include <windows.h>

extern b32 win32_init(void);
extern void win32_deinit(void);

typedef LPWSTR* CommandLineToArgvWFN( LPCWSTR lpCmdLine, int* pNumArgs );

#if defined(CORE_WIN32_SUBSYSTEM_WINDOWS)
attr_no_return attr_clink
void __stdcall WinMainCRTStartup(void) {
#else
attr_no_return attr_clink
void __stdcall mainCRTStartup(void) {
#endif
    HMODULE shell_api = LoadLibraryA( "SHELL32.DLL" );
    if( !shell_api ) {
        ExitProcess(255);
    }
    CommandLineToArgvWFN* in_CommandLineToArgvW =
        (CommandLineToArgvWFN*)GetProcAddress( shell_api, "CommandLineToArgvW");
    if( !in_CommandLineToArgvW ) {
        FreeLibrary(shell_api);
        ExitProcess(255);
    }

    int    argc = 0;
    char** argv = NULL;
    HLOCAL argv_buf = NULL;
    LPWSTR* wide_argv = in_CommandLineToArgvW( GetCommandLineW(), &argc );
    FreeLibrary(shell_api); {
        usize total_size = 0;
        usize string_buffer_size = 0;
        for( int i = 0; i < argc; ++i ) {
            // TODO(alicia): what to do if this fails?
            int len = WideCharToMultiByte( CP_UTF8, 0, wide_argv[i], -1, 0, 0, 0, 0 );
            string_buffer_size += len;
        }
        total_size = string_buffer_size + (sizeof(char*) * argc);

        argv_buf = LocalAlloc( LPTR, total_size );
        if( !argv_buf ) {
            // NOTE(alicia): failed to allocate arguments buffer!
            LocalFree( wide_argv );
            ExitProcess( 255 );
        }
        argv = (char**)((char*)argv_buf + string_buffer_size);

        usize string_offset = 0;

        // TODO(alicia): make sure we never hit I32_MAX anywhere? unlikely but still
        for( int i = 0; i < argc; ++i ) {
            char* write_at = (char*)argv_buf + string_offset;

            int remaining = string_buffer_size - string_offset;

            // TODO(alicia): what to do if this fails?
            int len = WideCharToMultiByte(
                CP_UTF8, 0, wide_argv[i], -1, write_at, remaining, 0, 0 );

            string_offset += len;
            argv[i] = write_at;
        }
    }

    LocalFree( wide_argv );

    if( !win32_init() ) {
        // NOTE(alicia): failed to initialize library!
        LocalFree( argv_buf );
        ExitProcess( 254 );
    }

    int exit_code = crt_main( argc, argv );

    win32_deinit();

    LocalFree( argv_buf );
    ExitProcess( exit_code );
}

#endif

