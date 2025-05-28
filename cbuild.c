/**
 * @file   cbuild.c
 * @brief  Build system for Core library.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   March 02, 2025
*/
#include "cbuild.h"
#include <inttypes.h>
#include <time.h>

#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    #include <io.h>
    #define output_is_terminal() \
        _isatty(_fileno(stdout))
#else
    #include <unistd.h>
    #define output_is_terminal() \
        isatty( 1 )
#endif

#define CORE_VERSION_MAJOR 0
#define CORE_VERSION_MINOR 1
#define CORE_VERSION_PATCH 3

#define CORE_VERSION_STRING \
    CB_STRINGIFY_VALUE(CORE_VERSION_MAJOR) "." \
    CB_STRINGIFY_VALUE(CORE_VERSION_MINOR) "." \
    CB_STRINGIFY_VALUE(CORE_VERSION_PATCH)

enum Mode {
    M_HELP,
    M_BUILD,
    M_TEST,
    M_DOCS,
    M_LSP,
    M_FLAGS,

    M_COUNT,
    M_DEFAULT = M_HELP
};

enum Compiler {
    C_CLANG,
    C_GCC,

#if CB_PLATFORM_CURRENT == CB_PLATFORM_GNU_LINUX
    C_MINGW_GCC,
#endif

    C_COUNT,
    C_DEFAULT = C_CLANG,
};

enum Target {
    T_NATIVE,
    T_WINDOWS,
    T_GNU_LINUX,

    T_COUNT,
    T_DEFAULT = T_NATIVE
};

enum Optimization {
    O_NONE,
    O_SPEED,
    O_SPACE,

    O_COUNT,
    O_DEFAULT = O_NONE
};

struct Settings {
    enum Mode mode;
    union {
        struct SettingsHelp {
            enum Mode mode;
        } help;
        struct SettingsBuild {
            const char* path_output;

            struct {
                enum Target       target       : 8;
                enum Compiler     compiler     : 8;
                enum Optimization optimization : 8;
            };

            struct {
                bool is_static            : 1;
                bool is_dry               : 1;
                bool is_debug             : 1;
                bool disable_print_config : 1;
                bool enable_stdlib        : 1;
                bool enable_logging       : 1;
                bool enable_assertions    : 1;
                bool disable_simd         : 1;
                bool enable_fpic          : 1;
            } flags;
        } build;
        struct SettingsTest {
            struct {
                enum Compiler     compiler     : 16;
                enum Optimization optimization : 16;
            };

            struct {
                bool is_static         : 1;
                bool is_dry            : 1;
                bool is_debug          : 1;
                bool enable_stdlib     : 1;
                bool enable_assertions : 1;
                bool disable_simd      : 1;
                bool is_cpp            : 1;
            } flags;
        } test;
        struct SettingsDocs {
            struct {
                enum Target   target   : 16;
                enum Compiler compiler : 16;
            };

            struct {
                bool is_static           : 1;
                bool is_cpp              : 1;
                bool should_open_browser : 1;
            } flags;
        } docs;
        struct SettingsLSP {
            struct {
                enum Target   target   : 16;
                enum Compiler compiler : 16;
            };

            struct {
                bool is_static         : 1;
                bool enable_assertions : 1;
                bool disable_simd      : 1;
                bool is_downstream     : 1;
                bool disable_warnings  : 1;
            } flags;
        } lsp;
        struct SettingsFlags {
            const char* path_output;

            enum Target target;

            struct {
                bool is_cpp            : 1;
                bool is_static         : 1;
                bool enable_stdlib     : 1;
                bool enable_assertions : 1;
                bool disable_simd      : 1;
            } flags;
        } flags;
    };
};

struct Args {
    int    len;
    char** buf;
};

#define NEXT( args ) \
    CB_ADVANCE( struct Args, &args, 1 )

#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    #define EXT_EXECUTABLE ".exe"
#else
    #define EXT_EXECUTABLE ""
#endif

bool mode_from_string( const char* string, enum Mode* out_mode );
const char* string_from_mode( enum Mode mode );
const char* mode_description( enum Mode mode );

bool compiler_from_string( const char* string, enum Compiler* out_compiler );
const char* string_from_compiler( enum Compiler compiler );
const char* compiler_command_c( enum Compiler compiler );
const char* compiler_command_cpp( enum Compiler compiler );

bool target_from_string( const char* string, enum Target* out_target );
const char* string_from_target( enum Target target );
enum Target target_normalize( enum Target target );
char* target_path( bool is_static, const char* output, enum Target target );

bool optimization_from_string( const char* string, enum Optimization* out_optimization );
const char* string_from_optimization( enum Optimization optimization );

int mode_help( struct Settings* settings );
int mode_build( struct Settings* settings );
int mode_test( struct Settings* settings );
int mode_docs( struct Settings* settings );
int mode_lsp( struct Settings* settings );
int mode_flags( struct Settings* settings );

int main( int argc, char** argv ) {
    CB_INITIALIZE( CB_LOG_ALL );

    struct Args args = { .len = argc, .buf = argv };
    args = NEXT( args );

    if( !args.len ) {
        return mode_help( NULL );
    }

    struct Settings settings = {};
    if( !mode_from_string( args.buf[0], &settings.mode ) ) {
        CB_ERROR( "unrecognized mode '%s'", args.buf[0] );
        mode_help( NULL );
        return 1;
    }
    args = NEXT( args );

    while( args.len ) {
        switch( settings.mode ) {
            case M_HELP: {
                if( !mode_from_string( args.buf[0], &settings.help.mode ) ) {
                    CB_ERROR( "unrecognized mode '%s'", args.buf[0] );
                    goto error_arg;
                }
            } goto break_args;
            case M_BUILD: {
                if( strcmp( args.buf[0], "-output" ) == 0 ) {
                    args = NEXT( args );
                    if( !args.len ) {
                        CB_ERROR( "-output requires a path after it!" );
                        goto error_arg;
                    }
                    settings.build.path_output = args.buf[0];
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-target" ) == 0 ) {
                    args = NEXT( args );
                    if( !args.len ) {
                        CB_ERROR( "-target requires a target name after it!" );
                        goto error_arg;
                    }
                    enum Target target;
                    if( target_from_string( args.buf[0], &target ) ) {
                        settings.build.target = target;
                        goto next_arg;
                    } else {
                        CB_ERROR( "unrecognized target '%s'", args.buf[0] );
                        goto error_arg;
                    }
                }
                if( strcmp( args.buf[0], "-compiler" ) == 0 ) {
                    args = NEXT( args );
                    if( !args.len ) {
                        CB_ERROR( "-compiler requires a compiler name after it!" );
                        goto error_arg;
                    }
                    enum Compiler compiler;
                    if( compiler_from_string( args.buf[0], &compiler ) ) {
                        settings.build.compiler = compiler;
                        goto next_arg;
                    } else {
                        CB_ERROR( "unrecognized compiler '%s'", args.buf[0] );
                        goto error_arg;
                    }
                }
                if( strcmp( args.buf[0], "-opt" ) == 0 ) {
                    args = NEXT( args );
                    if( !args.len ) {
                        CB_ERROR( "-opt requires a optimization name after it!" );
                        goto error_arg;
                    }
                    enum Optimization opt;
                    if( optimization_from_string( args.buf[0], &opt ) ) {
                        settings.build.optimization = opt;
                        goto next_arg;
                    } else {
                        CB_ERROR( "unrecognized optimization setting '%s'", args.buf[0] );
                        goto error_arg;
                    }
                }
                if( strcmp( args.buf[0], "-dynamic" ) == 0 ) {
                    settings.build.flags.is_static = false;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-static" ) == 0 ) {
                    settings.build.flags.is_static = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-enable-fpic" ) == 0 ) {
                    settings.build.flags.enable_fpic = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-debug" ) == 0 ) {
                    settings.build.flags.is_debug = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-dry" ) == 0 ) {
                    settings.build.flags.is_dry = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-disable-config" ) == 0 ) {
                    settings.build.flags.disable_print_config = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-enable-stdlib" ) == 0 ) {
                    settings.build.flags.enable_stdlib = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-enable-logging" ) == 0 ) {
                    settings.build.flags.enable_logging = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-enable-assertions" ) == 0 ) {
                    settings.build.flags.enable_assertions = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-disable-simd" ) == 0 ) {
                    settings.build.flags.disable_simd = true;
                    goto next_arg;
                }
            } break;
            case M_TEST: {
                if( strcmp( args.buf[0], "-compiler" ) == 0 ) {
                    args = NEXT( args );
                    if( !args.len ) {
                        CB_ERROR( "-compiler requires a compiler name after it!" );
                        goto error_arg;
                    }
                    enum Compiler compiler;
                    if( compiler_from_string( args.buf[0], &compiler ) ) {
                        settings.test.compiler = compiler;
                        goto next_arg;
                    } else {
                        CB_ERROR( "unrecognized compiler '%s'", args.buf[0] );
                        goto error_arg;
                    }
                }
                if( strcmp( args.buf[0], "-opt" ) == 0 ) {
                    args = NEXT( args );
                    if( !args.len ) {
                        CB_ERROR( "-opt requires a optimization name after it!" );
                        goto error_arg;
                    }
                    enum Optimization opt;
                    if( optimization_from_string( args.buf[0], &opt ) ) {
                        settings.test.optimization = opt;
                        goto next_arg;
                    } else {
                        CB_ERROR( "unrecognized optimization setting '%s'", args.buf[0] );
                        goto error_arg;
                    }
                }
                if( strcmp( args.buf[0], "-cpp" ) == 0 ) {
                    settings.test.flags.is_cpp = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-dynamic" ) == 0 ) {
                    settings.test.flags.is_static = false;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-static" ) == 0 ) {
                    settings.test.flags.is_static = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-debug" ) == 0 ) {
                    settings.test.flags.is_debug = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-dry" ) == 0 ) {
                    settings.test.flags.is_dry = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-enable-stdlib" ) == 0 ) {
                    settings.test.flags.enable_stdlib = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-enable-assertions" ) == 0 ) {
                    settings.test.flags.enable_assertions = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-disable-simd" ) == 0 ) {
                    settings.test.flags.disable_simd = true;
                    goto next_arg;
                }
            } break;
            case M_DOCS: {
                if( strcmp( args.buf[0], "-browser" ) == 0 ) {
                    settings.docs.flags.should_open_browser = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-target" ) == 0 ) {
                    args = NEXT( args );
                    if( !args.len ) {
                        CB_ERROR( "-target requires a target name after it!" );
                        goto error_arg;
                    }
                    enum Target target;
                    if( target_from_string( args.buf[0], &target ) ) {
                        settings.docs.target = target;
                        goto next_arg;
                    } else {
                        CB_ERROR( "unrecognized target '%s'", args.buf[0] );
                        goto error_arg;
                    }
                }
                if( strcmp( args.buf[0], "-compiler" ) == 0 ) {
                    args = NEXT( args );
                    if( !args.len ) {
                        CB_ERROR( "-compiler requires a compiler name after it!" );
                        goto error_arg;
                    }
                    enum Compiler compiler;
                    if( compiler_from_string( args.buf[0], &compiler ) ) {
                        settings.docs.compiler = compiler;
                        goto next_arg;
                    } else {
                        CB_ERROR( "unrecognized compiler '%s'", args.buf[0] );
                        goto error_arg;
                    }
                }
                if( strcmp( args.buf[0], "-dynamic" ) == 0 ) {
                    settings.docs.flags.is_static = false;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-static" ) == 0 ) {
                    settings.docs.flags.is_static = true;
                    goto next_arg;
                }
            } break;
            case M_LSP: {
                if( strcmp( args.buf[0], "-downstream" ) == 0 ) {
                    settings.lsp.flags.is_downstream = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-disable-warnings" ) == 0 ) {
                    settings.lsp.flags.disable_warnings = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-target" ) == 0 ) {
                    args = NEXT( args );
                    if( !args.len ) {
                        CB_ERROR( "-target requires a target name after it!" );
                        goto error_arg;
                    }
                    enum Target target;
                    if( target_from_string( args.buf[0], &target ) ) {
                        settings.lsp.target = target;
                        goto next_arg;
                    } else {
                        CB_ERROR( "unrecognized target '%s'", args.buf[0] );
                        goto error_arg;
                    }
                }
                if( strcmp( args.buf[0], "-compiler" ) == 0 ) {
                    args = NEXT( args );
                    if( !args.len ) {
                        CB_ERROR( "-compiler requires a compiler name after it!" );
                        goto error_arg;
                    }
                    enum Compiler compiler;
                    if( compiler_from_string( args.buf[0], &compiler ) ) {
                        settings.lsp.compiler = compiler;
                        goto next_arg;
                    } else {
                        CB_ERROR( "unrecognized compiler '%s'", args.buf[0] );
                        goto error_arg;
                    }
                }
                if( strcmp( args.buf[0], "-dynamic" ) == 0 ) {
                    settings.lsp.flags.is_static = false;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-static" ) == 0 ) {
                    settings.lsp.flags.is_static = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-enable-assertions" ) == 0 ) {
                    settings.lsp.flags.enable_assertions = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-disable-simd" ) == 0 ) {
                    settings.lsp.flags.disable_simd = true;
                    goto next_arg;
                }
            } break;
            case M_FLAGS: {
                if( strcmp( args.buf[0], "-output" ) == 0 ) {
                    args = NEXT( args );
                    if( !args.len ) {
                        CB_ERROR( "-output requires a path after it!" );
                        goto error_arg;
                    }
                    settings.flags.path_output = args.buf[0];
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-target" ) == 0 ) {
                    args = NEXT( args );
                    if( !args.len ) {
                        CB_ERROR( "-target requires a target name after it!" );
                        goto error_arg;
                    }
                    enum Target target;
                    if( target_from_string( args.buf[0], &target ) ) {
                        settings.flags.target = target;
                        goto next_arg;
                    } else {
                        CB_ERROR( "unrecognized target '%s'", args.buf[0] );
                        goto error_arg;
                    }
                }
                if( strcmp( args.buf[0], "-cpp" ) == 0 ) {
                    settings.flags.flags.is_cpp = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-dynamic" ) == 0 ) {
                    settings.flags.flags.is_static = false;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-static" ) == 0 ) {
                    settings.flags.flags.is_static = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-enable-logging" ) == 0 ) {
                    settings.build.flags.enable_logging = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-enable-assertions" ) == 0 ) {
                    settings.flags.flags.enable_assertions = true;
                    goto next_arg;
                }
                if( strcmp( args.buf[0], "-disable-simd" ) == 0 ) {
                    settings.flags.flags.disable_simd = true;
                    goto next_arg;
                }
            } break;

            case M_COUNT:
                break;
        }

        CB_ERROR( "unrecognized argument '%s'", args.buf[0] );
error_arg:
        mode_help( &settings );
        return 1;

next_arg:
        args = NEXT( args );
    }
break_args:

    switch( settings.mode ) {
        case M_HELP:  return mode_help( &settings );
        case M_BUILD: return mode_build( &settings );
        case M_TEST:  return mode_test( &settings );
        case M_DOCS:  return mode_docs( &settings );
        case M_LSP:   return mode_lsp( &settings );
        case M_FLAGS: return mode_flags( &settings );
        case M_COUNT:
            break;
    }

    CB_UNREACHABLE( "main(): unrecognized mode: %d", settings.mode );
    return 1;
}

int mode_help( struct Settings* settings ) {
    enum Mode mode = M_HELP;

    if( settings ) {
        if( settings->mode == M_HELP ) {
            mode = settings->help.mode;
        } else {
            mode = settings->mode;
        }
    }

    printf( "OVERVIEW:     Build system for Core library.\n" );
#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
    printf( "USAGE:        .\\cbuild.exe %s [args...]\n", mode == M_HELP ? "<mode>" : string_from_mode( mode ) );
#else
    printf( "USAGE:        ./cbuild %s [args...]\n", mode == M_HELP ? "<mode>" : string_from_mode( mode ) );
#endif
    printf( "DESCRIPTION:  %s\n", mode_description( mode ) );
    printf( "ARGUMENTS:\n" );

    switch( mode ) {
        case M_HELP: {
            printf( "  <mode>   Name of mode to print help for.\n" );
            printf( "             valid: " );
            for( enum Mode m = 0; m < M_COUNT; ++m ) {
                printf( "%s", string_from_mode( m ) );
                if( (m + 1) < M_COUNT ) {
                    putchar( ',' ); putchar( ' ' );
                } else {
                    putchar( '\n' );
                }
            }
        } break;

        case M_BUILD: {
            printf( "  -output <string>     Set output directory.\n" );
            printf( "                          default: ./build\n" );
            printf( "                          note:    does not create output directory\n" );
            printf( "                                   or any subdirectories if this option is used.\n" );
            printf( "  -target <string>     Change compilation target.\n" );
            printf( "                          default: native\n" );
            printf( "                          valid:   " );
            for( enum Target t = 0; t < T_COUNT; ++t ) {
                printf( "%s", string_from_target( t ) );
                if( (t + 1) < T_COUNT ) {
                    putchar( ',' ); putchar( ' ' );
                } else {
                    putchar( '\n' );
                }
            }
            printf( "  -compiler <string>   Change compiler to use.\n" );
            printf( "                          default: clang or gcc-mingw32 if cross-compiling to Windows\n" );
            printf( "                          valid:   " );
            for( enum Compiler c = 0; c < C_COUNT; ++c ) {
                printf( "%s", string_from_compiler( c ) );
                if( (c + 1) < C_COUNT ) {
                    putchar( ',' ); putchar( ' ' );
                } else {
                    putchar( '\n' );
                }
            }
            printf( "  -opt <string>        Set optimization level.\n" );
            printf( "                          default: none\n" );
            printf( "                          valid:   " );
            for( enum Optimization o = 0; o < O_COUNT; ++o ) {
                printf( "%s", string_from_optimization( o ) );
                if( (o + 1) < O_COUNT ) {
                    putchar( ',' ); putchar( ' ' );
                } else {
                    putchar( '\n' );
                }
            }
            printf( "  -dynamic             Create dynamic library (default). Overrides -static\n" );
            printf( "  -static              Create static library. Overrides -dynamic\n" );
            printf( "  -debug               Generate debug symbols.\n" );
            printf( "  -dry                 Don't actually compile, just print configuration and command.\n" );
            printf( "  -disable-config      Don't print configuration to stdout.\n" );
            printf( "                          note: still prints compile command.\n" );
            printf( "  -enable-logging      Enable logging inside of library.\n" );
            printf( "  -enable-stdlib       Link to C stdlib.\n" );
            printf( "                          note: always enabled when targetting GNU/Linux.\n" );
            printf( "  -enable-assertions   Enable compile-time assertions in library.\n" );
            printf( "  -disable-simd        Disable SIMD instructions.\n" );
            printf( "                          note: on x86_64, SSE1-4.2 instructions are used.\n" );
            printf( "  -enable-fpic         Compile with -fPIC. Only applies for static libraries as dynamic already includes it.\n" );
        } break;

        case M_TEST: {
            printf( "  -compiler <string>   Change compiler to use.\n" );
            printf( "                          default: clang\n" );
            printf( "                          valid:   " );
            for( enum Compiler c = 0; c < C_COUNT; ++c ) {
#if CB_PLATFORM_CURRENT == CB_PLATFORM_GNU_LINUX
                if( c == C_MINGW_GCC ) {
                    if( (c + 1) >= C_COUNT ) {
                        putchar('\n');
                    }
                    continue;
                }
#endif
                printf( "%s", string_from_compiler( c ) );
                if( (c + 1) < C_COUNT ) {
                    putchar( ',' ); putchar( ' ' );
                } else {
                    putchar( '\n' );
                }
            }
            printf( "  -opt <string>        Set optimization level.\n" );
            printf( "                          default: none\n" );
            printf( "                          valid:   " );
            for( enum Optimization o = 0; o < O_COUNT; ++o ) {
                printf( "%s", string_from_optimization( o ) );
                if( (o + 1) < O_COUNT ) {
                    putchar( ',' ); putchar( ' ' );
                } else {
                    putchar( '\n' );
                }
            }
            printf( "  -cpp                 Run tests for C++\n" );
            printf( "  -dynamic             Create dynamic library (default). Overrides -static\n" );
            printf( "  -static              Create static library. Overrides -dynamic\n" );
            printf( "  -debug               Generate debug symbols.\n" );
            printf( "  -dry                 Don't actually compile, just print configuration.\n" );
            printf( "  -enable-stdlib       Link to C stdlib.\n" );
            printf( "                          note: always enabled when targetting GNU/Linux.\n" );
            printf( "  -enable-assertions   Enable compile-time assertions in library.\n" );
            printf( "  -disable-simd        Disable SIMD instructions.\n" );
            printf( "                          note: on x86_64, SSE1-4.2 instructions are used.\n" );
        } break;

        case M_DOCS: {
            printf( "  -browser             Open docs in web browser after generating.\n" );
#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
            printf( "                          note: uses Windows CMD command 'start'\n" );
#elif CB_PLATFORM_CURRENT == CB_PLATFORM_GNU_LINUX
            printf( "                          note: uses 'xdg-open'\n" );
#elif CB_PLATFORM_CURRENT == CB_PLATFORM_MACOS
            printf( "                          note: uses 'open'\n" );
#endif
            printf( "  -target <string>     Change target to define in docs.\n" );
            printf( "                          default: native\n" );
            printf( "                          valid:   " );
            for( enum Target t = 0; t < T_COUNT; ++t ) {
                printf( "%s", string_from_target( t ) );
                if( (t + 1) < T_COUNT ) {
                    putchar( ',' ); putchar( ' ' );
                } else {
                    putchar( '\n' );
                }
            }
            printf( "  -compiler <string>   Change compiler to define in docs.\n" );
            printf( "                          default: clang or gcc-mingw32 if cross-compiling to Windows\n" );
            printf( "                          valid:   " );
            for( enum Compiler c = 0; c < C_COUNT; ++c ) {
                printf( "%s", string_from_compiler( c ) );
                if( (c + 1) < C_COUNT ) {
                    putchar( ',' ); putchar( ' ' );
                } else {
                    putchar( '\n' );
                }
            }
            printf( "  -cpp                 Generate documentation for C++ headers.\n" );
            printf( "  -dynamic             Define dynamic library export macro (default). Overrides -static\n" );
            printf( "  -static              Define static library export macro. Overrides -dynamic\n" );
        } break;

        case M_LSP: {
            printf( "  -downstream          Generate LSP flags for downstream project.\n" );
            printf( "                          note: flags are output to stdout.\n" );
            printf( "  -disable-warnings    Don't generate warning flags.\n" );
            printf( "                          note: default flags are -Wall, -Wextra and -Werror=vla\n" );
            printf( "  -target <string>     Change target to define.\n" );
            printf( "                          default: native\n" );
            printf( "                          valid:   " );
            for( enum Target t = 0; t < T_COUNT; ++t ) {
                printf( "%s", string_from_target( t ) );
                if( (t + 1) < T_COUNT ) {
                    putchar( ',' ); putchar( ' ' );
                } else {
                    putchar( '\n' );
                }
            }
            printf( "  -compiler <string>   Change compiler to define.\n" );
            printf( "                          default: clang or gcc-mingw32 if cross-compiling to Windows\n" );
            printf( "                          valid:   " );
            for( enum Compiler c = 0; c < C_COUNT; ++c ) {
                printf( "%s", string_from_compiler( c ) );
                if( (c + 1) < C_COUNT ) {
                    putchar( ',' ); putchar( ' ' );
                } else {
                    putchar( '\n' );
                }
            }
            printf( "  -dynamic             Define dynamic library export macro (default). Overrides -static\n" );
            printf( "  -static              Define static library export macro. Overrides -dynamic\n" );
            printf( "  -enable-assertions   Enable compile-time assertions in library.\n" );
            printf( "  -disable-simd        Disable SIMD instructions.\n" );
            printf( "                          note: on x86_64, SSE1-4.2 instructions are used.\n" );

        } break;

        case M_FLAGS: {
            printf( "  -output <string>     Set output directory.\n" );
            printf( "                          default: ./build\n" );
            printf( "  -target <string>     Change compilation target.\n" );
            printf( "                          default: native\n" );
            printf( "                          valid:   " );
            for( enum Target t = 0; t < T_COUNT; ++t ) {
                printf( "%s", string_from_target( t ) );
                if( (t + 1) < T_COUNT ) {
                    putchar( ',' ); putchar( ' ' );
                } else {
                    putchar( '\n' );
                }
            }
            printf( "  -cpp                 Generate flags for C++ project.\n" );
            printf( "  -dynamic             Generate flags for dynamically linked library (default). Overrides -static\n" );
            printf( "  -static              Generate flags for statically linked library. Overrides -dynamic\n" );
            printf( "  -enable-stdlib       Generate enable stdlib flag.\n" );
            printf( "  -enable-assertions   Generate enable assertion flag.\n" );
            printf( "  -enable-logging      Generate enable logging flag.\n" );
            printf( "  -disable-simd        Don't generate enable SIMD flag.\n" );
        } break;

        case M_COUNT:
            break;
    }
    printf( "\n" );
    return 0;
}
bool generate_command_line( CB_Command command ) {
    #define PATH "src/generated/command_line.c"
    CB_INFO( "generating " PATH " . . ." );
    if( cb_file_exists( PATH ) ) {
        if( !cb_file_remove( PATH ) ) {
            return false;
        }
    }

    CB_Time    now     = cb_time_query();
    struct tm* tm_info = localtime( &now );

    char time_buf[64] = {};
    strftime( time_buf, sizeof(time_buf), "%B %d, %Y", tm_info );

    CB_File file;
    if( !cb_file_open( PATH, CB_FOPEN_CREATE | CB_FOPEN_WRITE, &file ) ) {
        return false;
    }

    #define write( ... ) cb_file_write_fmt( &file, __VA_ARGS__ )

    write( "/**\n" );
    write( " * @file   command_line.c\n" );
    write( " * @brief  Generated build information.\n" );
    write( " * @author Generated from cbuild.c\n" );
    write( " * @date   %s\n", time_buf );
    write( "*/\n" );
    write( "#include \"core/types.h\"\n\n" );

    write( "TimePosix external_core_build_time = 0x%" PRIX64 ";\n", now );

    write( "const char external_core_command_line[] = \"" );

    CB_StringBuilder string = {};
    cb_command_flatten( command, &string );
    CB_PUSH( &string, 0 );
    write( "%s", string.buf );
    CB_FREE( string.buf, string.cap );

    write( "\";\n" );
    write( "usize external_core_command_line_len = sizeof(external_core_command_line);\n" );

    cb_file_close( &file );
    CB_INFO( "generated " PATH " successfully!" );

    #undef PATH
    #undef write

    return true;
}
int mode_build( struct Settings* settings ) {
    double start = cb_time_msec();
    if( !settings->build.flags.is_dry ) {
        CB_INFO( "building libcore . . . " );
    }

    settings->build.target = target_normalize( settings->build.target );
#if CB_PLATFORM_CURRENT == CB_PLATFORM_GNU_LINUX
    if( settings->build.target == T_WINDOWS ) {
        settings->build.compiler = C_MINGW_GCC;
    } else if( settings->build.compiler == C_MINGW_GCC ) {
        settings->build.compiler = C_GCC;
    }
#endif

    if( settings->build.target == T_GNU_LINUX ) {
        settings->build.flags.enable_stdlib = true;
    }

    char* output_path = target_path(
        settings->build.flags.is_static,
        settings->build.path_output, settings->build.target );

    if( !settings->build.flags.disable_print_config ) {
        #define bstr( boolean ) (boolean) ? "true" : "false"
        #define bstre( boolean ) (boolean) ? "enabled" : "disabled"

        CB_INFO( "libcore build config:" );
        CB_INFO( "  output path:         %s", output_path );
        CB_INFO( "  core version:        " CORE_VERSION_STRING );
        CB_INFO( "  cbuild version:      " CB_VERSION_STRING );
        CB_INFO( "  host platform:       " CB_PLATFORM_CURRENT_NAME " " CB_ARCH_CURRENT_NAME );
        CB_INFO( "  target platform:     %s", string_from_target( settings->build.target ) );
        CB_INFO( "  compiler:            %s", string_from_compiler( settings->build.compiler ) );
        CB_INFO( "  optimization:        %s", string_from_optimization( settings->build.optimization ) );
        CB_INFO( "  static:              %s", bstr( settings->build.flags.is_static ) );
        CB_INFO( "  strip debug symbols: %s", bstr( !settings->build.flags.is_debug) );
        CB_INFO( "  link stdlib:         %s", bstr( settings->build.flags.enable_stdlib) );
        CB_INFO( "  logging:             %s", bstre( settings->build.flags.enable_logging) );
        CB_INFO( "  assertions:          %s", bstre( settings->build.flags.enable_assertions) );
        CB_INFO( "  simd:                %s", bstre( !settings->build.flags.disable_simd ) );
        CB_INFO( "  dry run:             %s", bstr( settings->build.flags.is_dry) );

        #undef bstr
        #undef bstre
    }

    if( !settings->build.flags.is_dry ) {
        bool success = false;
        switch( settings->build.target ) {
            case T_WINDOWS: {
                success = cb_make_directories( "build", "build/windows", "src/generated" );
            } break;
            case T_GNU_LINUX: {
                success = cb_make_directories( "build", "build/gnu-linux", "src/generated" );
            } break;
            case T_NATIVE:
            case T_COUNT: break;
        }
        if( !success ) {
            CB_FREE( output_path, strlen(output_path) + 1 );
            return 1;
        }
    }

    CB_CommandBuilder builder = {};
    cb_command_builder_append(
        &builder, compiler_command_c( settings->build.compiler ), "src/core/sources.c" );
    if( settings->build.flags.is_static ) {
        cb_command_builder_append( &builder, "-c" );
    }
    cb_command_builder_append(
        &builder, "-o", output_path, "-Wall", "-Wextra", "-Werror=vla", "-Werror" );
    CB_FREE( output_path, strlen(output_path) + 1 );

    cb_command_builder_append( &builder,
        "-Iinclude", "-I.", "-include", "./src/generated/command_line.c" );

    if( settings->build.flags.is_static ) {
        cb_command_builder_append( &builder, "-DCORE_ENABLE_STATIC_BUILD" );
        if( settings->build.target == T_GNU_LINUX && settings->build.flags.enable_fpic ) {
            cb_command_builder_append( &builder, "-fPIC" );
        }
    } else {
        cb_command_builder_append( &builder, "-DCORE_ENABLE_EXPORT", "-shared" );
        if( settings->build.target == T_GNU_LINUX ) {
            cb_command_builder_append( &builder, "-fPIC" );
        }
    }

    switch( settings->build.optimization ) {
        case O_NONE:
            cb_command_builder_append( &builder, "-O0" );
            break;
        case O_SPEED:
            cb_command_builder_append( &builder, "-O2", "-ffast-math" );
            break;
        case O_SPACE:
            cb_command_builder_append( &builder, "-Os" );
            break;
        case O_COUNT: break;
    }

    if( settings->build.flags.is_debug ) {
        switch( settings->build.target ) {
            case T_WINDOWS: {
                cb_command_builder_append( &builder, "-g" );
            } break;
            case T_GNU_LINUX: {
                cb_command_builder_append( &builder, "-ggdb" );
            } break;

            case T_NATIVE:
            case T_COUNT:
                break;
        }
    }

    if( settings->build.compiler == C_CLANG ) {
        cb_command_builder_append( &builder, "-mno-stack-arg-probe" );
    }

    if( settings->build.flags.enable_stdlib ) {
        cb_command_builder_append( &builder, "-DCORE_ENABLE_STDLIB" );
    } else {
        cb_command_builder_append( &builder, "-nostdlib" );
    }
    if( settings->build.flags.enable_logging ) {
        cb_command_builder_append( &builder, "-DCORE_ENABLE_LOGGING" );
    }
    if( settings->build.flags.enable_assertions ) {
        cb_command_builder_append( &builder, "-DCORE_ENABLE_ASSERTIONS" );
    }
    if( !settings->build.flags.disable_simd ) {
        cb_command_builder_append(
            &builder, "-DCORE_ENABLE_SSE_INSTRUCTIONS" );
    }
    cb_command_builder_append(
        &builder,
        "-DCORE_LIB_VERSION_MAJOR=" CB_STRINGIFY_VALUE(CORE_VERSION_MAJOR),
        "-DCORE_LIB_VERSION_MINOR=" CB_STRINGIFY_VALUE(CORE_VERSION_MINOR),
        "-DCORE_LIB_VERSION_PATCH=" CB_STRINGIFY_VALUE(CORE_VERSION_PATCH),
        "-DCORE_ENABLE_INTERNAL" );

    if( settings->build.target == T_WINDOWS ) {
#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
        cb_command_builder_append(
            &builder, "-fuse-ld=lld", "-Wl,/stack:0x100000", "-lkernel32" );
        if( settings->build.flags.is_debug ) {
            cb_command_builder_append( &builder, "-gcodeview", "-Wl,/debug" );
        }
#else
        cb_command_builder_append( &builder, "-lkernel32" );
#endif
    }

    if( settings->build.flags.is_dry ) {
        CB_StringBuilder string = {};
        cb_command_flatten( builder.cmd, &string );
        CB_PUSH( &string, 0 );
        CB_INFO( "  > %s", string.buf );

        CB_FREE( string.buf, string.cap );
        cb_command_builder_free( &builder );
        return 0;
    }

    if( !generate_command_line( builder.cmd ) ) {
        cb_command_builder_free( &builder );
        return 255;
    }

    int  exit_code = 0;
    bool success   = cb_process_exec( builder.cmd, &exit_code, NULL, NULL, NULL, NULL, NULL );

    cb_command_builder_free( &builder );

    if( !success ) {
        CB_FATAL( "failed to execute build command!" );
        return 255;
    }
    if( exit_code ) {
        CB_ERROR( "failed to build core library!" );
        return exit_code;
    }

    CB_INFO( "build finished in %fms", cb_time_msec() - start );
    return 0;
}
int mode_test( struct Settings* settings ) {
    struct Settings build = {};
    build.mode                          = M_BUILD;
    build.build.path_output             = "build/tests";
    build.build.target                  = target_normalize( T_NATIVE );
    build.build.compiler                = settings->test.compiler;
    build.build.optimization            = settings->test.optimization;
    build.build.flags.is_static         = settings->test.flags.is_static;
    build.build.flags.is_dry            = settings->test.flags.is_dry;
    build.build.flags.is_debug          = settings->test.flags.is_debug;
    build.build.flags.enable_stdlib     = settings->test.flags.enable_stdlib;
    build.build.flags.enable_assertions = settings->test.flags.enable_assertions;
    build.build.flags.enable_logging    = true;
    build.build.flags.disable_simd      = settings->test.flags.disable_simd;

    if( !settings->test.flags.is_dry ) {
        bool success = false;

        success = cb_make_directories( "build", "build/tests", "src/generated" );

        if( !success ) {
            return 1;
        }
    }

    int exit_code = 0;
    if( (exit_code = mode_build( &build )) ) {
        return exit_code;
    }

    CB_CommandBuilder builder = {};
    if( settings->test.flags.is_cpp ) {
        cb_command_builder_append(
            &builder, compiler_command_cpp( settings->test.compiler ),
            "src/tests/main.cpp", "-o", "build/tests/libcore-test-cpp" EXT_EXECUTABLE );
    } else {
        cb_command_builder_append(
            &builder, compiler_command_c( settings->test.compiler ),
            "src/tests/main.c", "-o", "build/tests/libcore-test-c" EXT_EXECUTABLE );
    }

    cb_command_builder_append(
        &builder, "-Iinclude", "-Wall", "-Wextra", "-Werror=vla", "-O0" );

    if( settings->test.flags.is_static ) {
        cb_command_builder_append(
            &builder, "build/tests/libcore.o", "-DCORE_ENABLE_STATIC_BUILD" );
    } else {
        cb_command_builder_append( &builder, "-Lbuild/tests", "-lcore" );
    }

    // NOTE(alicia): this is the current platform as it's normalized
    switch( build.build.target ) {
        case T_WINDOWS: {
#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
            cb_command_builder_append( "-fuse-ld=lld", "-Wl,/stack:0x100000" );
#endif
            if( settings->test.flags.is_debug ) {
                cb_command_builder_append( &builder, "-g" );
#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
                cb_command_builder_append( &builder, "-gcodeview", "-Wl,/debug" );
#endif
            }
        } break;
        case T_GNU_LINUX: {
            if( settings->test.flags.is_debug ) {
                cb_command_builder_append( &builder, "-ggdb" );
            }
        } break;

        case T_NATIVE:
        case T_COUNT:
            break;
    }

    if( !build.build.flags.enable_stdlib ) {
        if( settings->test.flags.is_static ) {
            cb_command_builder_append( &builder, "-nostdlib" );
        }
    }

    if( settings->build.flags.enable_logging ) {
        cb_command_builder_append( &builder, "-DCORE_ENABLE_LOGGING" );
    }
    if( settings->build.flags.enable_assertions ) {
        cb_command_builder_append( &builder, "-DCORE_ENABLE_ASSERTIONS" );
    }
    if( !settings->build.flags.disable_simd ) {
        cb_command_builder_append( &builder, "-DCORE_ENABLE_SSE_INSTRUCTIONS" );
    }

    if( settings->test.flags.is_dry ) {
        CB_StringBuilder string = {};
        cb_command_flatten( builder.cmd, &string );
        CB_PUSH( &string, 0 );

        CB_INFO( "Test command:" );
        CB_INFO( "  > %s", string.buf );

        CB_FREE( string.buf, string.cap );
        cb_command_builder_free( &builder );
        return 0;
    }

    cb_command_builder_add_null_terminator( &builder );
         exit_code = 0;
    bool success   = cb_process_exec( builder.cmd, &exit_code, NULL, NULL, NULL, NULL, NULL );

    cb_command_builder_free( &builder );

    if( !success ) {
        CB_FATAL( "failed to execute build command!" );
        return 255;
    }
    if( exit_code ) {
        CB_ERROR( "failed to build test program!" );
        return exit_code;
    }

    CB_INFO( "running tests . . ." );
    CB_EnvironmentBuilder env = {
        .cap   = 0,
        .len   = 1,
        .name  = (const char*[]){ "LD_LIBRARY_PATH" },
        .value = (const char*[]){ "build/tests" },
    };
    if( settings->test.flags.is_cpp ) {
        return cb_process_exec_quick_ex( 0, &env, 0, 0, 0, "./build/tests/libcore-test-cpp" );
    } else {
        return cb_process_exec_quick_ex( 0, &env, 0, 0, 0, "./build/tests/libcore-test-c" );
    }
}
bool generate_doxygen_settings( struct Settings* settings ) {
    settings->docs.target = target_normalize( settings->docs.target );

    CB_INFO( "generating doxygen settings for build configuration . . ." );
    char* path = cb_local_buf_fmt(
        "docs/Doxyfile_%s", string_from_target( settings->docs.target ) );
    if( cb_file_exists( path ) ) {
        if( !cb_file_remove( path ) ) {
            CB_ERROR( "failed to remove old doxygen settings!" );
            return false;
        }
    }

    if( !cb_file_copy( path, "docs/Doxyfile_default", false ) ) {
        CB_ERROR( "failed to copy default doxygen settings!" );
        return false;
    }

    CB_File file;
    if( !cb_file_open( path, CB_FOPEN_WRITE | CB_FOPEN_APPEND, &file ) ) {
        return false;
    }

    #define write( ... ) \
        cb_file_write_fmt( &file, __VA_ARGS__ )

    write( "PREDEFINED += " );
    write( "CORE_LIB_VERSION_MAJOR=" CB_STRINGIFY_VALUE(CORE_VERSION_MAJOR) " " );
    write( "CORE_LIB_VERSION_MINOR=" CB_STRINGIFY_VALUE(CORE_VERSION_MINOR) " " );
    write( "CORE_LIB_VERSION_PATCH=" CB_STRINGIFY_VALUE(CORE_VERSION_PATCH) " " );
    write( "CORE_ENABLE_DEBUG_BREAK CORE_ENABLE_DEBUG_ASSERTIONS CORE_ENABLE_ASSERTIONS " );

    if( settings->docs.flags.is_static ) {
        write( "CORE_ENABLE_STATIC_BUILD " );
    }
    if( settings->docs.flags.is_cpp ) {
        write( "__cplusplus " );
    }

    switch( settings->docs.compiler ) {
        case C_CLANG:
            write( "__clang__ " );
            break;
        case C_GCC:
            write( "__GNUC__ " );
            break;
#if CB_PLATFORM_CURRENT == CB_PLATFORM_GNU_LINUX
        case C_MINGW_GCC:
            write( "__GNUC__ " );
            break;
#endif
        case C_COUNT:
          break;
    }

    switch( settings->docs.target ) {
        case T_WINDOWS: {
            write( "_WIN32 __MINGW64__ __x86_64__ " );
        } break;
        case T_GNU_LINUX: {
            write( "__linux__ __x86_64__ " );
        } break;
        case T_NATIVE:
        case T_COUNT:
            break;
    }
    write( "CORE_ENABLE_SSE_INSTRUCTIONS " );
    write( "CORE_ENABLE_DEBUG_BREAK CORE_ENABLE_DEBUG_ASSERTIONS CORE_ENABLE_ASSERTIONS" );

    write(
        "\nPROJECT_NUMBER = "
        CB_STRINGIFY_VALUE(CORE_VERSION_MAJOR) "."
        CB_STRINGIFY_VALUE(CORE_VERSION_MINOR) "."
        CB_STRINGIFY_VALUE(CORE_VERSION_PATCH) "\n" );

    #undef write

    cb_file_close( &file );
    CB_INFO( "doxygen settings generated at %s", path );
    return true;
}
void check_doxygen_version(void) {
    CB_Pipe read, write;
    if( !cb_pipe_open( &read, &write ) ) {
        return;
    }

    CB_ProcessID pid;
    if( !cb_process_exec_async( CB_COMMAND( "doxygen", "--version"), &pid, NULL, NULL, NULL, &write, NULL ) ) {
        cb_pipe_close( &read );
        cb_pipe_close( &write );
        return;
    }
    int exit_code = cb_process_wait( &pid );
    cb_pipe_close( &write );

    if( exit_code ) {
        cb_pipe_close( &read );
    }

    char temp[64] = {};
    bool read_result = cb_file_read( &read, sizeof(temp) - 1, temp, NULL );
    cb_pipe_close( &read );

    if( !read_result ) {
        return;
    }

    int min[3]    = { 1, 9, 7 };
    int actual[3] = {};
    int counter   = 0;

    CB_StringSlice slice = CB_STRING_FROM_CSTR( temp );
    while( slice.len ) {
        int at = cb_string_find( slice, '.' );
        if( at < 0 ) {
            slice.buf[slice.len - 1] = 0;
            at = slice.len;
        } else {
            slice.buf[at] = 0;
        }

        int value = strtol( slice.buf, NULL, 10 );

        actual[counter++] = value;

        if( counter >= CB_ARRAY_LEN(min) ) {
            break;
        }

        slice = cb_string_advance( slice, at + 1 );
    }

    for( int i = 0; i < CB_ARRAY_LEN(min); ++i ) {
        if( actual[i] > min[i] ) {
            break;
        }

        if( actual[i] < min[i] ) {
            CB_WARN( "expected minimum doxygen version: %i.%i.%i",
                min[0], min[1], min[2] );
            CB_WARN( "actual doxygen version:           %i.%i.%i",
                actual[0], actual[1], actual[2] );
            CB_WARN( "expect some issues with generated doxygen documentation!" );
            return;
        }
    }
}
int mode_docs( struct Settings* settings ) {
    if( cb_process_is_in_path( "doxygen" ) ) {
        check_doxygen_version();
    } else {
        CB_FATAL( "generating docs requires 'doxygen' in path!" );
        mode_help( settings );
        return 255;
    }

    if( !generate_doxygen_settings( settings ) ) {
        return 1;
    }

    char* path = cb_local_buf_fmt(
        "Doxyfile_%s", string_from_target( settings->docs.target ) );

    CB_INFO(
        "generating documentation for %s . . .",
        string_from_target( settings->docs.target ) );

    int exit_code = cb_process_exec_quick_ex(
        "docs", NULL, NULL, NULL, NULL, "doxygen", path, "-q" );
    if( exit_code ) {
        CB_ERROR( "failed to generate documentation!" );
        return exit_code;
    }

    #define DOCS_PATH "docs/html/index.html"
    CB_INFO( "documentation generated at path " DOCS_PATH );

    if( settings->docs.flags.should_open_browser ) {
        CB_INFO( "opening " DOCS_PATH " in browser . . ." );
        switch( CB_PLATFORM_CURRENT ) {
            case CB_PLATFORM_UNKNOWN:
                break;
            case CB_PLATFORM_WINDOWS: {
                return cb_process_exec_quick( "cmd", "start", DOCS_PATH );
            } break;
            case CB_PLATFORM_GNU_LINUX: {
                return cb_process_exec_quick( "xdg-open", DOCS_PATH );
            } break;
            case CB_PLATFORM_MACOS: {
                return cb_process_exec_quick( "open", DOCS_PATH );
            } break;
        }
        CB_ERROR( "command for opening browser is not defined for current platform!" );
        return 1;
    }

    #undef DOCS_PATH
    return 0;
}
int mode_lsp_core_include( struct Settings* settings ) {
    #define PATH "include/core/compile_flags.txt"
    CB_File          file;
    CB_FileOpenFlags flags = CB_FOPEN_WRITE;
    if( cb_file_exists( PATH ) ) {
        flags |= CB_FOPEN_TRUNCATE;
    } else {
        flags |= CB_FOPEN_CREATE;
    }
    if( !cb_file_open( PATH, flags, &file ) ) {
        return 1;
    }

    #define write( ... ) cb_file_write_fmt( &file, __VA_ARGS__ )
    write( "-I..\n" );
    write( "-D_CLANGD\n" );

    if( !settings->lsp.flags.disable_warnings ) {
        write( "-Wall\n" );
        write( "-Wextra\n" );
        write( "-Werror=vla\n" );
    }

    if( settings->lsp.flags.is_static ) {
        write( "-DCORE_ENABLE_STATIC_BUILD\n" );
    } else {
        write( "-DCORE_ENABLE_EXPORT\n" );
    }
    if( settings->lsp.flags.enable_assertions ) {
        write( "-DCORE_ENABLE_ASSERTIONS\n" );
    }

    cb_file_close( &file );
    CB_INFO( "  generated " PATH " successfully." );
    #undef PATH
    #undef write
    return 0;
}
int mode_lsp_core_include_internal( struct Settings* settings ) {
    #define PATH "include/core/internal/compile_flags.txt"
    CB_File          file;
    CB_FileOpenFlags flags = CB_FOPEN_WRITE;
    if( cb_file_exists( PATH ) ) {
        flags |= CB_FOPEN_TRUNCATE;
    } else {
        flags |= CB_FOPEN_CREATE;
    }
    if( !cb_file_open( PATH, flags, &file ) ) {
        return 1;
    }

    #define write( ... ) cb_file_write_fmt( &file, __VA_ARGS__ )
    write( "-I../..\n" );
    write( "-D_CLANGD\n" );

    if( !settings->lsp.flags.disable_warnings ) {
        write( "-Wall\n" );
        write( "-Wextra\n" );
        write( "-Werror=vla\n" );
    }

    if( settings->lsp.flags.is_static ) {
        write( "-DCORE_ENABLE_STATIC_BUILD\n" );
    } else {
        write( "-DCORE_ENABLE_EXPORT\n" );
    }
    if( settings->lsp.flags.enable_assertions ) {
        write( "-DCORE_ENABLE_ASSERTIONS\n" );
    }
    write( "-DCORE_ENABLE_INTERNAL\n" );

    cb_file_close( &file );
    CB_INFO( "  generated " PATH " successfully." );
    #undef PATH
    #undef write
    return 0;
}
int mode_lsp_core_src( struct Settings* settings ) {
    #define PATH "src/core/compile_flags.txt"
    CB_File          file;
    CB_FileOpenFlags flags = CB_FOPEN_WRITE;
    if( cb_file_exists( PATH ) ) {
        flags |= CB_FOPEN_TRUNCATE;
    } else {
        flags |= CB_FOPEN_CREATE;
    }
    if( !cb_file_open( PATH, flags, &file ) ) {
        return 1;
    }

    #define write( ... ) cb_file_write_fmt( &file, __VA_ARGS__ )
    write( "-I../../include\n" );
    write( "-I../..\n" );
    write( "-D_CLANGD\n" );

    if( !settings->lsp.flags.disable_warnings ) {
        write( "-Wall\n" );
        write( "-Wextra\n" );
        write( "-Werror=vla\n" );
    }

    if( settings->lsp.flags.is_static ) {
        write( "-DCORE_ENABLE_STATIC_BUILD\n" );
    } else {
        write( "-DCORE_ENABLE_EXPORT\n" );
    }
    if( settings->lsp.flags.enable_assertions ) {
        write( "-DCORE_ENABLE_ASSERTIONS\n" );
    }
    write( "-DCORE_LIB_VERSION_MAJOR=" CB_STRINGIFY_VALUE(CORE_VERSION_MAJOR) "\n" );
    write( "-DCORE_LIB_VERSION_MINOR=" CB_STRINGIFY_VALUE(CORE_VERSION_MINOR) "\n" );
    write( "-DCORE_LIB_VERSION_PATCH=" CB_STRINGIFY_VALUE(CORE_VERSION_PATCH) "\n" );
    write( "-DCORE_ENABLE_INTERNAL\n" );

    cb_file_close( &file );
    CB_INFO( "  generated " PATH " successfully." );
    #undef PATH
    #undef write
    return 0;
}
int mode_lsp_core_tests( struct Settings* settings ) {
    #define PATH "src/tests/compile_flags.txt"
    CB_File          file;
    CB_FileOpenFlags flags = CB_FOPEN_WRITE;
    if( cb_file_exists( PATH ) ) {
        flags |= CB_FOPEN_TRUNCATE;
    } else {
        flags |= CB_FOPEN_CREATE;
    }
    if( !cb_file_open( PATH, flags, &file ) ) {
        return 1;
    }

    #define write( ... ) cb_file_write_fmt( &file, __VA_ARGS__ )
    write( "-I../../include\n" );
    write( "-D_CLANGD\n" );

    if( !settings->lsp.flags.disable_warnings ) {
        write( "-Wall\n" );
        write( "-Wextra\n" );
        write( "-Werror=vla\n" );
    }

    if( settings->lsp.flags.enable_assertions ) {
        write( "-DCORE_ENABLE_ASSERTIONS\n" );
    }

    cb_file_close( &file );
    CB_INFO( "  generated " PATH " successfully." );
    #undef PATH
    #undef write
    return 0;
}
int mode_lsp_corelib( struct Settings* settings ) {
    CB_INFO( "generating clangd files for core library source . . ." );
    int result = 0;
    result |= mode_lsp_core_include( settings );
    result |= mode_lsp_core_include_internal( settings );
    result |= mode_lsp_core_src( settings );
    result |= mode_lsp_core_tests( settings );
    
    if( result ) {
        CB_ERROR( "failed to generate one or more compile_flags.txt!" );
    } else {
        CB_INFO( "generated clangd files successfully." );
    }
    return result;
}
int mode_lsp_downstream( struct Settings* settings ) {
    char* canonical = cb_path_canonicalize( "." );

    printf( "-D_CLANGD\n" );
    printf( "-I%s/include\n", canonical );
    if( settings->lsp.flags.is_static ) {
        printf( "-DCORE_ENABLE_STATIC_BUILD\n" );
    }
    if( settings->lsp.flags.enable_assertions ) {
        printf( "-DCORE_ENABLE_ASSERTIONS\n" );
    }
    if( !settings->lsp.flags.disable_simd ) {
        printf( "-DCORE_ENABLE_SSE_INSTRUCTIONS\n" );
    }
    if( !settings->lsp.flags.disable_warnings ) {
        printf( "-Wall\n-Wextra\n-Werror=vla\n" );
    }

    free( canonical );
    return 0;
}
int mode_lsp( struct Settings* settings ) {
    settings->lsp.target = target_normalize( settings->lsp.target );
    if( settings->lsp.flags.is_downstream ) {
        return mode_lsp_downstream( settings );
    } else {
        return mode_lsp_corelib( settings );
    }
}
void mode_flags_puts( const char* string ) {
    fputs( string, stdout );
    if( output_is_terminal() ) {
        putchar( ' ' );
    } else {
        putchar( 0 );
    }
}
void mode_flags_printf( const char* fmt, ... ) {
    va_list va;
    va_start( va, fmt );
    vprintf( fmt, va );
    va_end( va );
    if( output_is_terminal() ) {
        putchar( ' ' );
    } else {
        putchar( 0 );
    }
}
int mode_flags( struct Settings* settings ) {
    settings->flags.target =
        target_normalize( settings->flags.target );
    if( settings->flags.target == T_GNU_LINUX ) {
        settings->flags.flags.enable_stdlib = true;
    }

    #define puts   mode_flags_puts
    #define printf mode_flags_printf

    char* corelib_directory = cb_path_canonicalize( "." );
    const char* output_directory = settings->flags.path_output;
    if( settings->flags.path_output ) {
        char* canonical_path_output =
            cb_path_canonicalize( settings->flags.path_output );
        output_directory = memcpy(
            cb_local_buf(), canonical_path_output, strlen( canonical_path_output ) );
        free( canonical_path_output );
    } else {
        output_directory = cb_local_buf_fmt(
            "%s/build/%s", corelib_directory,
            string_from_target( settings->flags.target ) );
    }

    if( settings->flags.flags.is_static ) {
        puts( "-DCORE_ENABLE_STATIC_BUILD" );
        printf( "%s/libcore.o", output_directory );
        if( !settings->flags.flags.disable_simd ) {
            puts( "-DCORE_ENABLE_SSE_INSTRUCTIONS" );
        }
        if( settings->flags.target == T_WINDOWS ) {
#if CB_PLATFORM_CURRENT == CB_PLATFORM_WINDOWS
            puts( "-fuse-ld=lld" );
            puts( "-Wl,/stack:0x100000" );
            puts( "-lkernel32" );
#endif
        }
    } else {
        printf( "-L%s", output_directory );
        puts( "-lcore" );
    }
    if( !settings->flags.flags.enable_stdlib ) {
        puts( "-nostdlib" );
    }

    if( settings->flags.flags.enable_assertions ) {
        puts( "-DCORE_ENABLE_ASSERTIONS" );
    }
    printf( "-I%s/include", corelib_directory );

    #undef puts
    #undef printf
    free( corelib_directory );
    return 0;
}

bool mode_from_string( const char* string, enum Mode* out_mode ) {
    for( enum Mode m = 0; m < M_COUNT; ++m ) {
        if( strcmp( string, string_from_mode( m ) ) == 0 ) {
            *out_mode = m;
            return true;
        }
    }
    return false;
}
const char* string_from_mode( enum Mode mode ) {
    switch( mode ) {
        case M_HELP:  return "help";
        case M_BUILD: return "build";
        case M_TEST:  return "test";
        case M_DOCS:  return "docs";
        case M_LSP:   return "lsp";
        case M_FLAGS: return "flags";
        case M_COUNT:
            break;
    }
    return "";
}
const char* mode_description( enum Mode mode ) {
    switch( mode ) {
        case M_HELP:
            return "Print help and exit.";
        case M_BUILD:
            return "Build core library.";
        case M_TEST:
            return "Build core library and run tests.";
        case M_DOCS:
            return "Generate doxygen documentation. Requires 'doxygen' in path.";
        case M_LSP:
            return "Generate clangd compile flags.";
        case M_FLAGS:
            return "Output null-separated list of compile flags for downstream project.";
        case M_COUNT:
            break;
    }
    return "";
}

bool compiler_from_string( const char* string, enum Compiler* out_compiler ) {
    for( enum Compiler c = 0; c < C_COUNT; ++c ) {
        if( strcmp( string, string_from_compiler( c ) ) == 0 ) {
            *out_compiler = c;
            return true;
        }
    }
    return false;
}
const char* string_from_compiler( enum Compiler compiler ) {
    switch( compiler ) {
        case C_CLANG:     return "clang";
        case C_GCC:       return "gcc";
#if CB_PLATFORM_CURRENT == CB_PLATFORM_GNU_LINUX
        case C_MINGW_GCC: return "gcc-mingw32";
#endif
        case C_COUNT:
            break;
    }
    return "";
}

#if CB_PLATFORM_CURRENT == CB_PLATFORM_GNU_LINUX
const char* global_mingw_c_compiler   = NULL;
const char* global_mingw_cpp_compiler = NULL;
const char* get_mingw_c_compiler(void) {
    if( global_mingw_c_compiler ) {
        return global_mingw_c_compiler;
    }

    if( cb_process_is_in_path( "x86_64-mingw32-w64-gcc" ) ) {
        global_mingw_c_compiler = "x86_64-mingw32-w64-gcc";
    } else if( cb_process_is_in_path( "x86_64-w64-mingw32-gcc" ) ) {
        global_mingw_c_compiler = "x86_64-w64-mingw32-gcc";
    } else {
        global_mingw_c_compiler = "";
    }

    return global_mingw_c_compiler;
}
const char* get_mingw_cpp_compiler(void) {
    if( global_mingw_cpp_compiler ) {
        return global_mingw_cpp_compiler;
    }

    if( cb_process_is_in_path( "x86_64-mingw32-w64-g++" ) ) {
        global_mingw_cpp_compiler = "x86_64-mingw32-w64-g++";
    } else if( cb_process_is_in_path( "x86_64-w64-mingw32-g++" ) ) {
        global_mingw_cpp_compiler = "x86_64-w64-mingw32-g++";
    } else {
        global_mingw_cpp_compiler = "";
    }

    return global_mingw_cpp_compiler;
}
#endif

const char* compiler_command_c( enum Compiler compiler ) {
    switch( compiler ) {
        case C_CLANG:     return "clang";
        case C_GCC:       return "gcc";
#if CB_PLATFORM_CURRENT == CB_PLATFORM_GNU_LINUX
        case C_MINGW_GCC: {
            return get_mingw_c_compiler();
        } break;
#endif
        case C_COUNT:
          break;
    }
    return "";
}
const char* compiler_command_cpp( enum Compiler compiler ) {
    switch( compiler ) {
        case C_CLANG:     return "clang++";
        case C_GCC:       return "g++";
#if CB_PLATFORM_CURRENT == CB_PLATFORM_GNU_LINUX
        case C_MINGW_GCC: {
            return get_mingw_cpp_compiler();
        } break;
#endif
        case C_COUNT:
          break;
    }
    return "";
}

bool target_from_string( const char* string, enum Target* out_target ) {
    for( enum Target t = 0; t < T_COUNT; ++t ) {
        if( strcmp( string, string_from_target( t ) ) == 0 ) {
            *out_target = t;
            return true;
        }
    }
    return false;
}
const char* string_from_target( enum Target target ) {
    switch( target ) {
        case T_NATIVE:
            return "native";
        case T_WINDOWS:
            return "windows";
        case T_GNU_LINUX:
            return "gnu-linux";
        case T_COUNT:
            break;
    }
    return "";
}
enum Target target_normalize( enum Target target ) {
    if( target != T_NATIVE ) {
        return target;
    }
    switch( CB_PLATFORM_CURRENT ) {
        case CB_PLATFORM_MACOS:
        case CB_PLATFORM_UNKNOWN: {
            CB_UNIMPLEMENTED( CB_PLATFORM_CURRENT_NAME " is currently not implemented!" );
        } break;
        case CB_PLATFORM_GNU_LINUX: return T_GNU_LINUX;
        case CB_PLATFORM_WINDOWS:   return T_WINDOWS;
    }
    CB_UNREACHABLE();
}
char* target_path( bool is_static, const char* output, enum Target target ) {
    const char* file_name = is_static ? "libcore.o" : "libcore.so";
    if( !is_static && target == T_WINDOWS ) {
        file_name = "libcore.dll";
    }

    if( output ) {
        int output_len = strlen( output );
        bool append_path_separator = false;
        if( !output_len || (output[output_len - 1] != '/') ) {
            append_path_separator = true;
        }

        return cb_alloc_fmt( "%s%s%s", output, append_path_separator ? "/" : "", file_name );
    } else {
        return cb_alloc_fmt( "./build/%s/%s", string_from_target( target ), file_name );
    }
}

bool optimization_from_string( const char* string, enum Optimization* out_optimization ) {
    for( enum Optimization o = 0; o < O_COUNT; ++o ) {
        if( strcmp( string, string_from_optimization( o ) ) == 0 ) {
            *out_optimization = o;
            return true;
        }
    }
    return false;
}
const char* string_from_optimization( enum Optimization optimization ) {
    switch( optimization ) {
        case O_NONE:  return "none";
        case O_SPEED: return "speed";
        case O_SPACE: return "space";
        case O_COUNT:
            break;
    }
    return "";
}

#define CB_IMPLEMENTATION
#include "cbuild.h"

