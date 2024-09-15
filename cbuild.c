/**
 * @file   cbuild.c
 * @brief  Build system for Core Library.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   July 06, 2024
*/
#include "cbuild.h"
#define println( ... ) printf( __VA_ARGS__ ); printf( "\n" )

#define CORE_LIB_VERSION_MAJOR 0
#define CORE_LIB_VERSION_MINOR 1
#define CORE_LIB_VERSION_PATCH 2

typedef enum Mode {
    MODE_HELP,
    MODE_BUILD,
    MODE_TEST,
    MODE_DOCS,
    MODE_LSP,
    MODE_CLEAN,
} Mode;
b32 mode_from_string( string str, Mode* out_mode );
string mode_to_string( Mode mode );

typedef enum BuildCompiler {
    BC_CC,
    BC_CLANG,
    BC_GCC,
    BC_MSVC,
} BuildCompiler;
b32 build_compiler_from_string( string str, BuildCompiler* out_compiler );
string build_compiler_to_string( BuildCompiler compiler );
string build_compiler_name( BuildCompiler compiler );

typedef enum BuildTarget {
    T_NATIVE,
    T_WEB,
} BuildTarget;
b32 build_target_from_string( string str, BuildTarget* out_target );
string build_target_to_string( BuildTarget target );
string build_target_name( BuildTarget target );

string build_native_arch(void);

#define DEFAULT_OUTPUT_DIR string_text( "./build" )
#if defined( COMPILER_CLANG )
    #define DEFAULT_COMPILER BC_CLANG
#elif defined( COMPILER_GCC )
    #define DEFAULT_COMPILER BC_GCC
#elif defined( COMPILER_MSVC )
    #define DEFAULT_COMPILER BC_MSVC
#else
    #define DEFAULT_COMPILER BC_CC
#endif
#define DEFAULT_TARGET T_NATIVE
#define DEFAULT_NAME string_text( "core" )
typedef struct ParsedArguments {
    Mode mode;
    union {
        struct HelpArguments {
            Mode mode;
        } help;
        struct BuildArguments {
            string        output_dir;
            BuildCompiler compiler;
            BuildTarget   target;
            b32           release_build;
            b32           enable_optimizations;
            b32           enable_stdlib;
            b32           enable_logging;
            b32           enable_assertions;
            b32           enable_simd;
            b32           enable_simd_256;
            string        name;
            b32           use_simple_name;
            b32           static_build;
            b32           dry_build;
        } build;
        struct TestArguments {
            string        __placeholder_0;
            BuildCompiler compiler;
            BuildTarget   target;
            b32           release_build;
            b32           enable_optimizations;
            b32           enable_stdlib;
            b32           enable_logging;
            b32           enable_assertions;
            b32           enable_simd;
            b32           enable_simd_256;
        } test;
        struct DocsArguments {
            union {
                string __placeholder_0;
                struct {
                    b32 open;
                };
            };
            BuildCompiler compiler;
            BuildTarget   target;
            b32           release_build;
            b32           enable_optimizations;
            b32           enable_stdlib;
            b32           enable_logging;
            b32           enable_assertions;
            b32           enable_simd;
            b32           enable_simd_256;
            string        __placeholder_1;
            b32           __placeholder_2;
            b32           static_build;
        } docs;
        struct LspArguments {
            string        __placeholder_0;
            BuildCompiler compiler;
            BuildTarget   target;
            b32           release_build;
            b32           enable_optimizations;
            b32           enable_stdlib;
            b32           enable_logging;
            b32           enable_assertions;
            b32           enable_simd;
            b32           enable_simd_256;
        } lsp;
        struct CleanArguments {
            string output_dir;
        } clean;
    };
} ParsedArguments;
void parsed_arguments_init( Mode mode, ParsedArguments* out_args );
void print_parsed_arguments( ParsedArguments* args );
b32  parse_arguments( int argc, char** argv, ParsedArguments* out_args );

b32 generate_command_line( Command* cmd );

b32 mode_help( Mode mode );
b32 mode_build( struct BuildArguments* args );
b32 mode_test( struct TestArguments* args );
b32 mode_docs( struct DocsArguments* args );
b32 mode_lsp( struct LspArguments* args );
b32 mode_clean( struct CleanArguments* args );

int main( int argc, char** argv ) {
    init( LOGGER_LEVEL_INFO );

    if( argc <= 1 ) {
        mode_help( MODE_HELP );
        return 0;
    }

    ParsedArguments parsed_arguments;
    memory_zero( &parsed_arguments, sizeof(parsed_arguments) );

    if( !parse_arguments( argc, argv, &parsed_arguments ) ) {
        return 1;
    }

    switch( parsed_arguments.mode ) {
        case MODE_HELP: {
            mode_help( parsed_arguments.help.mode );
            return 0;
        } break;
        case MODE_BUILD: return !mode_build( &parsed_arguments.build );
        case MODE_TEST:  return !mode_test( &parsed_arguments.test );
        case MODE_DOCS:  return !mode_docs( &parsed_arguments.docs );
        case MODE_LSP:   return !mode_lsp( &parsed_arguments.lsp );
        case MODE_CLEAN: return !mode_clean( &parsed_arguments.clean );
    }
}
string target_extension( BuildTarget target, b32 static_build ) {
    if( static_build ) {
        return string_text( ".o" );
    }

    switch( target ) {
        case T_NATIVE: {
#if defined( PLATFORM_WINDOWS )
            return string_text(".dll");
#else
            return string_text(".so");
#endif
        } break;
        case T_WEB: return string_text(".wasm");
    }
}
b32 mode_build( struct BuildArguments* args ) {
    #define btostr( b ) ( (b) ? "true" : "false" )
    #define btostr_en( b ) ( (b) ? "enabled" : "disabled" )

    string compiler_name = build_compiler_name( args->compiler );
    string target_name   = build_target_name( args->target );

    cb_info( "Configuration: " );
    cb_info( "    output directory: %.*s", (int)args->output_dir.len, args->output_dir.cc );
    cb_info( "    project name:     %.*s", (int)args->name.len, args->name.cc );
    cb_info( "    use simple name:  %s", btostr( args->use_simple_name ) );
    cb_info( "    compiler:         %s", compiler_name.cc );
    cb_info( "    target:           %s", target_name.cc );
    cb_info( "    release:          %s", btostr( args->release_build ) );
    cb_info( "    optimized:        %s", btostr( args->enable_optimizations ) );
    cb_info( "    cstdlib:          %s", btostr_en( args->enable_stdlib ) );
    cb_info( "    logging:          %s", btostr_en( args->enable_logging ) );
    cb_info( "    assertions:       %s", btostr_en( args->enable_assertions ) );
    cb_info( "    simd:             %s", btostr_en( args->enable_simd ) );
    cb_info( "    simd 256:         %s", btostr_en( args->enable_simd_256 ) );
    cb_info( "    static build:     %s", btostr( args->static_build ) );
    #undef btostr
    #undef btostr_en

    const string prefix = string_text( "lib" );
    string arch_name    = string_empty();
    switch( args->target ) {
        case T_NATIVE: {
            arch_name = build_native_arch();
        } break;
        case T_WEB: {
            arch_name = string_text( "wasm32" );
        } break;
    }
    string ext = target_extension( args->target, args->static_build );

    usize target_path_capacity = 
        prefix.len + args->output_dir.len + args->name.len + ext.len;
    if( !args->use_simple_name ) {
        target_path_capacity += arch_name.len;
        target_path_capacity += target_name.len;
    }
    target_path_capacity += 16;

    dstring* target_path = dstring_empty( target_path_capacity );
    if( !target_path ) {
        cb_error( "build: failed to generate target path!" );
        return false;
    }

    if( args->output_dir.len ) {
        target_path = dstring_append( target_path, args->output_dir );
        if( args->output_dir.cc[args->output_dir.len - 1] != '/' ) {
            target_path = dstring_push( target_path, '/' );
        }
    }

    target_path = dstring_append( target_path, prefix );
    target_path = dstring_append( target_path, args->name );

    if( !args->use_simple_name ) {
        target_path = dstring_push( target_path, '-' );
        target_path = dstring_append( target_path, target_name );
        target_path = dstring_push( target_path, '-' );
        target_path = dstring_append( target_path, arch_name );
    }
    target_path = dstring_append( target_path, ext );

    cb_info( "target path: %s", target_path );

    #define push( arg ) do {\
        if( !command_builder_push( &builder, arg ) ) {\
            cb_error( "failed to generate build command!" );\
            dstring_free( target_path );\
            command_builder_free( &builder );\
            return false;\
        }\
    } while(0)
    CommandBuilder builder;
    if( !command_builder_new( compiler_name.cc, &builder ) ) {
        cb_error( "failed to generate build command!" );
        dstring_free( target_path );
        return false;
    }

    dstring* target_obj_path = NULL;
    switch( args->compiler ) {
        case BC_CC:
        case BC_CLANG:
        case BC_GCC: {
            push( "-std=c11" );
            push( "--include./generated/core_command_line.c");
            push( "-xc" );
            push( "./impl/sources.h" );
            if( args->static_build ) {
                push( "-c" );
            } else {
                push( "-shared" );
            }
            push( "-o" );
            push( target_path );
            push( "-Wall" );
            push( "-Wextra" );
            push( "-Werror" );
            push( "-Werror=vla" );

            if( args->compiler == BC_CLANG ) {
                push( "-mno-stack-arg-probe" );
            }

#if defined( PLATFORM_WINDOWS )
            push( "-fuse-ld=lld" );
            if( !args->static_build ) {
                push( "-Wl,/stack:0x100000" );
                push( "-lkernel32" );
            }
#endif
            if( !args->release_build ) {
                push( "-g" );

#if defined( PLATFORM_WINDOWS )
                push( "-gcodeview" );

                if( !args->static_build ) {
                    push( "-Wl,/debug" );
                }
#endif
            }

            if( args->enable_optimizations ) {
                push( "-O2" );
                push( "-ffast-math" );
            } else {
                push( "-O0" );
            }

            if( !args->enable_stdlib ) {
                push( "-nostdlib" );
            }

            if( args->enable_simd || args->enable_simd_256 ) {
                push( "-march=native" );
            }
        } break;
        case BC_MSVC: {
            push( "-std:c11" );
            push( "-nologo" );
            push( "-FIgenerated/core_command_line.c");
            push( "-Tc" );
            push( "./impl/sources.h" );
            if( args->static_build ) {
                push( "-c" );
                push( "-Fo:" );
            } else {
                target_obj_path = dstring_fmt( "%s/obj/", args->output_dir.cc );
                if( !target_obj_path ) {
                    cb_error( "failed to allocate target obj path name!" );
                    dstring_free( target_path );
                    command_builder_free( &builder );
                    return false;
                }
                push( "-Fo:" );
                push( target_obj_path );
                push( "-Fe:" );
            }
            push( target_path );

            push( "-W2" );
            push( "-external:W0" );
            push( "-external:env:INCLUDE" );
            push( "-wd4201" );
            push( "-wd4141" );
            push( "-wd4311" );
            push( "-Gm-" );
            push( "-GR-" );
            push( "-GS-" );
            push( "-Gs9999999" );
            push( "-Zc:preprocessor" );
            push( "-EHa-" );

            if( args->enable_optimizations ) {
                push( "-O2" );
                push( "-fp:fast" );
            } else {
                push( "-Od" );
                push( "-Z7" );
            }
            push( "-Oi" );

        } break;
    }
    #define def( name, ... ) push( "-D" name __VA_ARGS__ )

    push( "-I." );

    def( "CORE_LIB_VERSION_MAJOR=", macro_value_to_string(CORE_LIB_VERSION_MAJOR) );
    def( "CORE_LIB_VERSION_MINOR=", macro_value_to_string(CORE_LIB_VERSION_MINOR) );
    def( "CORE_LIB_VERSION_PATCH=", macro_value_to_string(CORE_LIB_VERSION_PATCH) );

    if( args->static_build ) {
        def( "CORE_ENABLE_STATIC_BUILD" );
    } else {
        def( "CORE_ENABLE_EXPORT" );
    }

    if( args->enable_stdlib ) {
        def( "CORE_ENABLE_STDLIB" );
    }
    if( args->enable_logging ) {
        def( "CORE_ENABLE_LOGGING" );
    }
    if( args->enable_assertions ) {
        def( "CORE_ENABLE_ASSERTIONS" );
        if( !args->release_build ) {
            def( "CORE_ENABLE_DEBUG_BREAK" );
            def( "CORE_ENABLE_DEBUG_ASSERTIONS" );
        }
    }
    if( args->target == T_NATIVE && args->enable_simd ) {
#if defined( ARCH_X86 )
        def( "CORE_ENABLE_SSE_INSTRUCTIONS" );
        if( args->enable_simd_256 ) {
            def( "CORE_ENABLE_AVX_INSTRUCTIONS" );
        }
#elif defined( ARCH_ARM )
        def( "CORE_ENABLE_NEON_INSTRUCTIONS" );
#endif
    }

    if( args->compiler == BC_MSVC ) {
        def( "INTERNAL_CORE_SINE_COSINE_NOT_IMPLEMENTED" );
        push( "-link" );
        if( !args->static_build ) {
            push( "-dll" );
        }
        push( "-stack:0x100000,0x100000" );
        push( "kernel32.lib" );
        push( "-subsystem:windows" );
        push( "-INCREMENTAL:NO" );

        if( args->release_build ) {
            push( "-opt:ref" );
        } else {
            push( "-debug:full" );
        }

        if( !args->enable_stdlib ) {
            push( "-nodefaultlib" );
        }
    }

    #undef push
    #undef def
    Command cmd = command_builder_cmd( &builder );

    if( args->dry_build ) {
        dstring* command_flat = command_flatten_dstring( &cmd );
        if( command_flat ) {
            cb_info( "%s", command_flat );
            dstring_free( command_flat );
        }

        if( target_obj_path ) {
            dstring_free( target_obj_path );
        }
        dstring_free( target_path );
        command_builder_free( &builder );
        return true;
    }

    if( !generate_command_line( &cmd ) ) {
        if( target_obj_path ) {
            dstring_free( target_obj_path );
        }
        dstring_free( target_path );
        command_builder_free( &builder );
        return false;
    }

    if( args->output_dir.len ) {
        if( !path_exists( args->output_dir.cc ) ) {
            if( !dir_create( args->output_dir.cc ) ) {
                cb_error( 
                    "failed to create directory at path '%.*s'!",
                    (int)args->output_dir.len, args->output_dir.cc );
                if( target_obj_path ) {
                    dstring_free( target_obj_path );
                }
                dstring_free( target_path );
                command_builder_free( &builder );
                return false;
            }
        }
    }
    if( args->compiler == BC_MSVC ) {
        if( target_obj_path && !path_exists( target_obj_path ) ) {
            if( !dir_create( target_obj_path ) ) {
                cb_error( "failed to create directory at path '%s'!", target_obj_path );
                dstring_free( target_obj_path );
                dstring_free( target_path );
                command_builder_free( &builder );
                return false;
            }
        }
    }
    if( !process_in_path( compiler_name.cc ) ) {
        if( args->compiler == BC_MSVC ) {
#if defined( PLATFORM_WINDOWS )
            cb_error(
                "compiler %s not found in path! "
                "run vcvarsall.bat before running cbuild!", compiler_name.cc );
#else
            cb_error( "msvc is only available on windows!" );
#endif
        } else {
            cb_error( "compiler %s not found in path!", compiler_name.cc );
        }
        if( target_obj_path ) {
            dstring_free( target_obj_path );
        }
        dstring_free( target_path );
        command_builder_free( &builder );
        return false;
    }

    PID pid = process_exec( cmd, false, NULL, NULL, NULL, NULL );
    dstring_free( target_path );
    command_builder_free( &builder );
    if( target_obj_path ) {
        dstring_free( target_obj_path );
    }

    int res = process_wait( pid );
    if( res == 0 ) {
        cb_info( "core library compiled successfully!" );
    } else {
        cb_error(
            "failed to compile core library! compiler exited with code %i.", res );
        return false;
    }

    return true;
}
b32 mode_test( struct TestArguments* args ) {
    ParsedArguments build;
    memory_zero( &build, sizeof(build) );
    parsed_arguments_init( MODE_BUILD, &build );

    #define copy_size\
        sizeof(*args) - sizeof(args->__placeholder_0)
    memory_copy( &build.build.compiler, &args->compiler, copy_size );
    #undef copy_size

    build.build.use_simple_name = true;
    build.build.name            = string_text( "core-test" );

    if( !mode_build( &build.build ) ) {
        return false;
    }

    string compiler = build_compiler_name( args->compiler );

    #define push( arg ) do {\
        if( !command_builder_push( &builder, arg ) ) {\
            cb_error( "failed to generate tests build command!" );\
            command_builder_free( &builder );\
            return false;\
        }\
    } while(0)
    #define def( name, ... ) push( "-D" name __VA_ARGS__ )
    CommandBuilder builder;
    if( !command_builder_new( compiler.cc, &builder ) ) {
        cb_error( "failed to generate tests build command!" );
        return false;
    }
#if defined(PLATFORM_WINDOWS)
    #define test_output "./build/core-test.exe"
#else
    #define test_output "./build/core-test"
#endif

    switch( args->compiler ) {
        case BC_CC:
        case BC_CLANG:
        case BC_GCC: {
            push( "-std=c11" );
            push( "./test/test.c" );
            push( "-o" );
            push( test_output );
            push( "-L./build" );
            push( "-lcore-test" );
            push( "-Wall" );
            push( "-Wextra" );
            push( "-Werror=vla" );
            push( "-O0" );
            push( "-g" );
            push( "-march=native" );
#if defined(PLATFORM_WINDOWS)
            push( "-gcodeview" );
            push( "-fuse-ld=lld" );
            push( "-Wl,/stack:0x100000" );
            push( "-Wl,/debug" );
#endif
        } break;
        case BC_MSVC: {
            push( "-std:c11" );
            push( "-nologo" );
            push( "./test/test.c" );
            push( "-Fe:" );
            push( test_output );
            push( "-Fo./build/obj/" );
            push( "-Od" );
            push( "-Oi" );
            push( "-Z7" );
        } break;
    }
    push( "-I." );
    if( args->enable_assertions ) {
        def( "CORE_ENABLE_ASSERTIONS" );
        if( !args->release_build ) {
            def( "CORE_ENABLE_DEBUG_ASSERTIONS" );
            def( "CORE_ENABLE_DEBUG_BREAK" );
        }
    }
    if( args->target == T_NATIVE && args->enable_simd ) {
#if defined( ARCH_X86 )
        def( "CORE_ENABLE_SSE_INSTRUCTIONS" );
        if( args->enable_simd_256 ) {
            def( "CORE_ENABLE_AVX_INSTRUCTIONS" );
        }
#elif defined( ARCH_ARM )
        def( "CORE_ENABLE_NEON_INSTRUCTIONS" );
#endif
    }

    if( args->compiler == BC_MSVC ) {
        push( "-link" );
        push( "-INCREMENTAL:NO" );
        push( "-stack:0x100000,0x100000" );
        push( "./build/libcore-test.lib" );
    }

    #undef push
    #undef def
    Command cmd = command_builder_cmd( &builder );
    PID pid = process_exec( cmd, false, NULL, NULL, NULL, NULL );
    command_builder_free( &builder );

    int res = process_wait( pid );

    if( res != 0 ) {
        cb_error( "failed to compile tests! exited with code %i", res );
        return false;
    }
    cb_info( "tests compiled successfully!" );

    cmd = command_new( test_output );

    cb_info( "running tests . . ." );
    pid = process_exec( cmd, false, NULL, NULL, NULL, NULL );

    res = process_wait( pid );

    if( res != 0 ) {
        cb_error( "tests failed with code %i!", res );
        return false;
    }

    cb_info( "all tests succeeded!" );
    #undef test_output
    return true;
}
b32 mode_docs( struct DocsArguments* args ) {
    if( !process_in_path( "doxygen" ) ) {
        cb_error( "doxygen was not found in path, required to generate docs!" );
        return false;
    }

    cb_info( "generating doxygen settings for build configuration . . ." );

    dstring* settings = dstring_empty( kibibytes(4) );
    if( !settings ) {
        cb_error( "failed to allocate doxygen settings buffer!" );
        return false;
    }
    #define write( ... ) do {\
        const char* formatted = local_fmt( __VA_ARGS__ );\
        settings = dstring_append_cstr( settings, formatted );\
    } while(0)

    write( "PREDEFINED += " );
    write( "CORE_LIB_VERSION_MAJOR=%i ", CORE_LIB_VERSION_MAJOR );
    write( "CORE_LIB_VERSION_MINOR=%i ", CORE_LIB_VERSION_MINOR );
    write( "CORE_LIB_VERSION_PATCH=%i ", CORE_LIB_VERSION_PATCH );
    if( args->release_build ) {
        write( "CORE_ENABLE_DEBUG_BREAK " );
        if( args->enable_assertions ) {
            write( "CORE_ENABLE_DEBUG_ASSERTIONS " );
        }
    }
    if( args->enable_assertions ) {
        write( "CORE_ENABLE_ASSERTIONS " );
    }
    if( args->enable_stdlib ) {
        write( "CORE_ENABLE_STDLIB " );
    }
    if( args->static_build ) {
        write( "CORE_ENABLE_STATIC_BUILD " );
    }
    switch( args->compiler ) {
        case BC_GCC:
        case BC_CC: {
            write( "__GNUC__" );
        } break;
        case BC_CLANG: {
            write( "__clang__ " );
        } break;
        case BC_MSVC: {
            write( "_MSC_VER " );
        } break;
    }

    const char* target_arch_name = "unknown";
    switch( args->target ) {
        case T_NATIVE: {
#if defined( PLATFORM_WINDOWS )
            write( "_WIN32 " );
#elif defined( PLATFORM_LINUX )
            write( "__linux__ " );
#elif defined( PLATFORM_MACOS )
            write( "__APPLE__ TARGET_OS_MAC " );
#endif

#if defined( ARCH_64BIT )
    #if defined( PLATFORM_MINGW )
            write( "__MINGW64__ " );
    #endif
#else
    #if defined( PLATFORM_MINGW )
            write( "__MINGW32__ " );
    #endif
#endif

#if defined( ARCH_X86 )
    #if defined( ARCH_64BIT )
            write( "__x86_64__ " );
    #else
            write( "__i386__ " );
    #endif
            if( args->enable_simd ) {
                write( "CORE_ENABLE_SSE_INSTRUCTIONS " );
            }
#elif defined( ARCH_ARM )
    #if defined( ARCH_64BIT )
            write( "__aarch64__ " );
    #else
            write( "__arm__ " );
    #endif
            if( args->enable_simd ) {
                write( "CORE_ENABLE_NEON_INSTRUCTIONS " );
            }
#endif
            target_arch_name = build_native_arch().cc;
        } break;
        case T_WEB: {
            target_arch_name = "wasm32";
        } break;
    }

    write(
        "\nPROJECT_NUMBER = %i.%i.%i",
        CORE_LIB_VERSION_MAJOR, CORE_LIB_VERSION_MINOR, CORE_LIB_VERSION_PATCH );
    #undef write

    dstring* doxygen_settings_path = dstring_fmt( 
        "./docs/Doxyfile_%s_%s",
        build_target_name( args->target ).cc, target_arch_name );
    if( !doxygen_settings_path ) {
        cb_error( "failed to generate doxygen settings file name!" );
        dstring_free( settings );
        return false;
    }

    if( path_exists( doxygen_settings_path ) ) {
        if( !file_remove( doxygen_settings_path ) ) {
            dstring_free( settings );
            dstring_free( doxygen_settings_path );
            return false;
        }
    }
    if( !file_copy( doxygen_settings_path, "./docs/Doxyfile_default" ) ) {
        dstring_free( settings );
        dstring_free( doxygen_settings_path );
        return false;
    }

    FD file;
    if( !fd_open( doxygen_settings_path, FOPEN_WRITE, &file ) ) {
        dstring_free( doxygen_settings_path );
        dstring_free( settings );
        return false;
    }

    fd_seek( &file, FSEEK_END, 0 );

    b32 file_write_success = fd_write( &file, dstring_len( settings ), settings, NULL );
    dstring_free( settings );
    fd_close( &file );

    if( !file_write_success ) {
        cb_error( "failed to write to generated doxygen settings file!" );
        dstring_free( doxygen_settings_path );
        return false;
    }

    cb_info( "settings successfully created at path '%s'", doxygen_settings_path );
    const char* doxygen_settings_file_name =
        string_adv_by( 
            string_from_dstring( doxygen_settings_path ), sizeof( "./docs" ) ).cc;

    Command cmd = command_new( "doxygen", doxygen_settings_file_name, "-q" );
    PID pid = process_exec( cmd, false, NULL, NULL, NULL, "./docs" );

    dstring_free( doxygen_settings_path );

    int result = process_wait( pid );
    if( result != 0 ) {
        if( result < 0 ) {
            cb_error(
                "doxygen exited abnormally! failed to generate documentation!" );
            return false;
        } else {
            cb_error(
                "failed to generate documentation! exited with code: %i", result );
            return false;
        }
    }

    cb_info( "documentation generated at ./docs/html/index.html successfully!" );

    if( args->open ) {
        Command open_docs_cmd;
        b32 browser_found = false;
#if defined( PLATFORM_WINDOWS )
        if( process_in_path( "pwsh" ) ) {
            open_docs_cmd = command_new(
                "pwsh", "-Command", "Invoke-Expression", "./docs/html/index.html" );
            browser_found = true;
        } 
#endif
        if( !browser_found ) {
            if( process_in_path( "firefox" ) ) {
                open_docs_cmd = command_new( "firefox", "./docs/html/index.html" );
                browser_found = true;
            } else if( process_in_path( "google-chrome" ) ) {
                open_docs_cmd = command_new(
                    "google-chrome", "./docs/html/index.html" );
                browser_found = true;
            }
        }

        if( !browser_found ) {
            cb_warn( "no browser was found in path to open docs!" );
            return true;
        }

        PID browser_pid = process_exec( open_docs_cmd, true, NULL, NULL, NULL, NULL );
        process_discard( browser_pid );
    }
    return true;
}
b32 mode_lsp( struct LspArguments* args ) {
    if( path_exists( "./core/compile_flags.txt" ) ) {
        if( !file_remove( "./core/compile_flags.txt" ) ) {
            cb_error( "failed to remove ./core/compile_flags.txt!" );
            return false;
        }
        cb_info( "removed old ./core/compile_flags.txt" );
    }
    if( path_exists( "./impl/compile_flags.txt" ) ) {
        if( !file_remove( "./impl/compile_flags.txt" ) ) {
            cb_error( "failed to remove ./impl/compile_flags.txt!" );
            return false;
        }
        cb_info( "removed old ./impl/compile_flags.txt" );
    }

    FD core_fd;
    if( !fd_open( "./core/compile_flags.txt", FOPEN_WRITE | FOPEN_CREATE, &core_fd ) ) {
        return false;
    }

    #define write( fd, ... ) do {\
        if( !fd_write_fmt( fd, __VA_ARGS__ ) ) {\
            fd_close( fd );\
            return false;\
        }\
    } while(0)
    write( &core_fd, "%s\n", build_compiler_name( args->compiler ).cc );
    write( &core_fd, "-std=c11\n" );
    write( &core_fd, "-I..\n" );
    write( &core_fd, "-D_CLANGD\n" );
    write( &core_fd, "-DCORE_LIB_VERSION_MAJOR=%i\n", CORE_LIB_VERSION_MAJOR );
    write( &core_fd, "-DCORE_LIB_VERSION_MINOR=%i\n", CORE_LIB_VERSION_MINOR );
    write( &core_fd, "-DCORE_LIB_VERSION_PATCH=%i\n", CORE_LIB_VERSION_PATCH );
    write( &core_fd, "-Wall\n" );
    write( &core_fd, "-Wextra\n" );
    write( &core_fd, "-Werror=vla\n" );
    write( &core_fd, "-DCORE_ENABLE_EXPORT\n" );
    if( !args->release_build ) {
        write( &core_fd, "-DCORE_ENABLE_DEBUG_BREAK\n" );
        if( args->enable_assertions ) {
            write( &core_fd, "-DCORE_ENABLE_DEBUG_ASSERTIONS\n" );
        }
    }
    if( args->enable_assertions ) {
        write( &core_fd, "-DCORE_ENABLE_ASSERTIONS\n" );
    }
    if( args->enable_simd ) {
        switch( args->target ) {
            case T_NATIVE: {
#if defined( ARCH_X86 )
                write( &core_fd, "-DCORE_ENABLE_SSE_INSTRUCTIONS\n" );
#elif defined( ARCH_ARM )
                write( &core_fd, "-DCORE_ENABLE_NEON_INSTRUCTIONS\n" );
#endif
            } break;
            case T_WEB: break;
        }
    }

    fd_close( &core_fd );
    cb_info( "wrote new ./core/compile_flags.txt" );

    if( !file_copy( "./impl/compile_flags.txt", "./core/compile_flags.txt" ) ) {
        return false;
    }
    cb_info( "wrote new ./impl/compile_flags.txt" );

    if( path_exists( "./test/compile_flags.txt" ) ) {
        if( file_remove( "./test/compile_flags.txt" ) ) {
            cb_info( "removed old ./test/compile_flags.txt" );
            if( file_copy( "./test/compile_flags.txt", "./core/compile_flags.txt" ) ) {
                cb_info( "wrote new ./test/compile_flags.txt" );
            }
        }
    } else {
        if( file_copy( "./test/compile_flags.txt", "./core/compile_flags.txt" ) ) {
            cb_info( "wrote new ./test/compile_flags.txt" );
        }
    }
    #undef write
    return true;
}
b32 mode_clean( struct CleanArguments* args ) {
    if( !path_exists( args->output_dir.cc ) ) {
        cb_info(
            "path '%s' does not exist, nothing to clean.", 
            args->output_dir.cc );
        return true;
    }
    if( dir_remove( args->output_dir.cc, true ) ) {
        cb_info(
            "removed directory '%s' successfully.",
            args->output_dir.cc );
        return true;
    } else {
        cb_error(
            "failed to remove path '%s'!",
            args->output_dir.cc );
        return false;
    }
}
b32 parse_arguments( int argc, char** argv, ParsedArguments* out_args ) {
    if( !mode_from_string( string_from_cstr( argv[1] ), &out_args->mode ) ) {
        cb_error( "unrecognized mode '%s'", argv[1] );
        mode_help( MODE_HELP );
        return false;
    }

    parsed_arguments_init( out_args->mode, out_args );

    if( out_args->mode == MODE_HELP ) {
        if( argc == 2 ) {
            return true;
        }

        return mode_from_string( string_from_cstr( argv[2] ), &out_args->help.mode );
    }

    for( int i = 2; i < argc; ++i ) {
        string arg = string_from_cstr( argv[i] );

        switch( out_args->mode ) {
            case MODE_DOCS: {
                if( string_cmp( arg, string_text( "--open" ) ) ) {
                    out_args->docs.open = true;
                    continue;
                }
            } break;
            case MODE_TEST:
            case MODE_LSP:
            case MODE_HELP: break;
            case MODE_BUILD:
            case MODE_CLEAN: {
                if( string_cmp(
                    string_truncate( arg, sizeof("--output") ), 
                    string_text( "--output=" )
                ) ) {
                    out_args->build.output_dir =
                        string_adv_by( arg, sizeof("--output") );
                    if( out_args->build.output_dir.len ) {
                        continue;
                    } else {
                        cb_error( "no output directory provided in --output= argument!" );
                        mode_help( out_args->mode );
                        return false;
                    }
                }
            } break;
        }

        switch( out_args->mode ) {
            case MODE_HELP:
            case MODE_CLEAN: break;
            case MODE_BUILD:
            case MODE_TEST:
            case MODE_DOCS:
            case MODE_LSP: {
                if( string_cmp( arg, string_text( "--release" ) ) ) {
                    out_args->build.release_build = true;
                    continue;
                }
                if( string_cmp( arg, string_text( "--optimized" ) ) ) {
                    out_args->build.enable_optimizations = true;
                    continue;
                }
                if( string_cmp(
                    string_truncate( arg, sizeof("--compiler") ),
                    string_text( "--compiler=" )
                ) ) {
                    if( build_compiler_from_string( 
                        string_adv_by( arg, sizeof("--compiler") ),
                        &out_args->build.compiler
                    ) ) {
                        continue;
                    }
                }
                if( string_cmp(
                    string_truncate( arg, sizeof("--target") ),
                    string_text( "--target=" )
                ) ) {
                    if( build_target_from_string( 
                        string_adv_by( arg, sizeof("--target") ),
                        &out_args->build.target
                    ) ) {
                        continue;
                    }
                }
            } break;
        }

        switch( out_args->mode ) {
            case MODE_HELP:
            case MODE_CLEAN: break;
            case MODE_DOCS: 
            case MODE_BUILD:
            case MODE_TEST:
            case MODE_LSP: {
                if( string_cmp( arg, string_text( "--enable-stdlib" ) ) ) {
                    out_args->build.enable_stdlib = true;
                    continue;
                }
                if( string_cmp( arg, string_text( "--enable-logging" ) ) ) {
                    out_args->build.enable_logging = true;
                    continue;
                }
                if( string_cmp( arg, string_text( "--enable-assertions" ) ) ) {
                    out_args->build.enable_assertions = true;
                    continue;
                }
                if( string_cmp( arg, string_text( "--enable-simd" ) ) ) {
                    out_args->build.enable_simd = true;
                    continue;
                }
                if( string_cmp( arg, string_text( "--enable-simd-256" ) ) ) {
                    out_args->build.enable_simd     = true;
                    out_args->build.enable_simd_256 = true;
                    continue;
                }
            } break;
        }
        
        switch( out_args->mode ) {
            case MODE_HELP:
            case MODE_TEST:
            case MODE_LSP:
            case MODE_CLEAN: break;
            case MODE_BUILD:
            case MODE_DOCS: {
                if( string_cmp( arg, string_text( "--static" ) ) ) {
                    out_args->build.static_build = true;
                    continue;
                }
            } break;
        }

        if( out_args->mode == MODE_BUILD ) {
            if( string_cmp(
                string_truncate( arg, sizeof("--name") ),
                string_text( "--name=" )
            ) ) {
                out_args->build.name = string_adv_by( arg, sizeof( "--name" ) );
                if( out_args->build.name.len ) {
                    continue;
                } else {
                    cb_error( "no name provided in --name= argument!" );
                    mode_help( out_args->mode );
                    return false;
                }
            }
            if( string_cmp( arg, string_text( "--simple-name" ) ) ) {
                out_args->build.use_simple_name = true;
                continue;
            }
            if( string_cmp( arg, string_text( "--dry" ) ) ) {
                out_args->build.dry_build = true;
                continue;
            }
        }

        cb_error( "unrecognized argument: '%s'", arg.cc );
        mode_help( out_args->mode );
        return false;
    }
    return true;
}
void print_parsed_arguments( ParsedArguments* args ) {
    cb_info( "mode: %s", mode_to_string( args->mode ).cc );
    switch( args->mode ) {
        case MODE_HELP: {
            cb_info( "\tmode: %s", mode_to_string( args->help.mode ).cc );
            return;
        } break;
        case MODE_BUILD:
        case MODE_CLEAN: {
            cb_info( "\toutput:            '%s'", args->build.output_dir.cc );
            if( args->mode == MODE_CLEAN ) {
                return;
            }
        } break;
        case MODE_DOCS: {
            cb_info( "\topen browser:      %s", args->docs.open ? "true" : "false" );
        } break;
        case MODE_TEST:
        case MODE_LSP: break;
    }

    switch( args->mode ) {
        case MODE_HELP:
        case MODE_CLEAN: break;
        case MODE_BUILD:
        case MODE_TEST:
        case MODE_DOCS:
        case MODE_LSP: {
            cb_info( "\tcompiler:          %s", build_compiler_to_string( args->build.compiler ).cc );
            cb_info( "\ttarget:            %s", build_target_to_string( args->build.target ).cc );
            cb_info( "\trelease:           %s", args->build.release_build ? "true" : "false" );
            cb_info( "\toptimized:         %s", args->build.enable_optimizations ? "true" : "false" );
            if( args->mode == MODE_DOCS ) {
                return;
            }
        } break;
    }

    switch( args->mode ) {
        case MODE_HELP:
        case MODE_CLEAN: break;
        case MODE_DOCS: 
        case MODE_BUILD:
        case MODE_TEST:
        case MODE_LSP: {
            cb_info( "\tenable stdlib:     %s", args->build.enable_stdlib ? "true" : "false" );
            cb_info( "\tenable logging:    %s", args->build.enable_logging ? "true" : "false" );
            cb_info( "\tenable assertions: %s", args->build.enable_assertions ? "true" : "false" );
            cb_info( "\tenable simd:       %s", args->build.enable_simd ? "true" : "false" );
            cb_info( "\tenable simd 256:   %s", args->build.enable_simd_256 ? "true" : "false" );
            if( args->mode != MODE_BUILD || args->mode != MODE_DOCS ) {
                return;
            }
        } break;
    }

    cb_info( "\tstatic build:      %s", args->build.static_build ? "true" : "false" );

    if( args->mode != MODE_DOCS ) {
        return;
    }

    cb_info( "\tname:              '%s'", args->build.name.cc );
    cb_info( "\tuse simple name:   %s", args->build.use_simple_name ? "true" : "false" );
    cb_info( "\tdry build:         %s", args->build.dry_build ? "true" : "false" );
}
void parsed_arguments_init( Mode mode, ParsedArguments* out_args ) {
    out_args->mode = mode;
    switch( mode ) {
        case MODE_HELP: {
            out_args->help.mode = MODE_HELP;
            return;
        } break;
        case MODE_DOCS: {
            out_args->docs.open = false;
        } break;
        case MODE_BUILD:
        case MODE_TEST:
        case MODE_LSP:
        case MODE_CLEAN: {
            out_args->build.output_dir = DEFAULT_OUTPUT_DIR;
            if( mode == MODE_CLEAN ) {
                return;
            }
        } break;
    }

    switch( mode ) {
        case MODE_HELP:
        case MODE_CLEAN: break;
        case MODE_BUILD:
        case MODE_TEST:
        case MODE_LSP:
        case MODE_DOCS: {
            out_args->build.compiler             = DEFAULT_COMPILER;
            out_args->build.target               = DEFAULT_TARGET;
            out_args->build.release_build        = false;
            out_args->build.enable_optimizations = false;
            if( mode == MODE_DOCS ) {
                return;
            }
        } break;
    }

    switch( mode ) {
        case MODE_HELP:
        case MODE_CLEAN: break;
        case MODE_DOCS: 
        case MODE_BUILD:
        case MODE_TEST:
        case MODE_LSP: {
            out_args->build.enable_stdlib     = false;
            out_args->build.enable_logging    = false;
            out_args->build.enable_assertions = false;
            out_args->build.enable_simd       = false;
            out_args->build.enable_simd_256   = false;
            if( mode != MODE_BUILD ) {
                return;
            }
        } break;
    }

    out_args->build.name            = DEFAULT_NAME;
    out_args->build.use_simple_name = false;
    out_args->build.static_build    = false;
    out_args->build.dry_build       = false;
}

b32 mode_help( Mode mode ) {
    println( "OVERVIEW: Core Library Build System.\n" );
    switch( mode ) {
        case MODE_HELP: {
            println( "USAGE:    ./cbuild help <str:mode,opt>\n" );
            println( "MODES:" );
            println( "  build   Build core library shared object." );
            println( "  test    Build library, build tests and run tests." );
            println( "  docs    Generate documentation." );
            println( "  lsp     Generate compile_flags.txt for clangd." );
            println( "  clean   Clean output directory." );
            println( "  help    Print help message for mode." );
        } break;
        case MODE_BUILD: {
            println( "USAGE:    ./cbuild build [args,opt]\n" );
            println( "ARGUMENTS:" );
            println( "  --compiler=<str:compiler>   Set compiler to build with. (default = %s)", build_compiler_to_string( DEFAULT_COMPILER ).cc );
            println( "                                 valid: clang, gcc, msvc, cc" );
            println( "  --target=<str:target>       Set target platform. (default = native)" );
            println( "                                 valid: native" );
            println( "  --name=<str>                Set name of output. (default = core)" );
            println( "                                 note: final output name matches this format: lib<name>-<platform>-<arch>.<ext>" );
            println( "  --simple-name               Don't format final output name. (default = false)" );
            println( "                                 note: final output becomes lib<name>.<ext>" );
            println( "  --output=<dir-path>         Set output directory. (default = ./build)" );
            println( "  --release                   Build in release mode. (default = false)" );
            println( "  --optimized                 Build with optimizations turned on. (default = false)" );
            println( "  --enable-stdlib             Link with C standard library. (default = win32:false posix:always true web:false)" );
            println( "  --enable-logging            Build with logging from library. (default = false)" );
            println( "  --enable-assertions         Build with assertions from library. (default = false)" );
            println( "  --enable-simd               Build with SSE instructions on x86, NEON instructions on ARM. (default = false)" );
            println( "  --enable-simd-256           Build with AVX/AVX2 on x86, does nothing on ARM. (default = false)" );
            println( "  --static                    Build static lib instead of dynamic. (default = false)" );
            println( "                                 note: final output becomes <format-name>.o" );
            println( "  --dry                       Don't actually build, just output configuration." );
        } break;
        case MODE_TEST: {
            println( "USAGE:    ./cbuild test [args,opt]\n" );
            println( "  --open                      Open documentation in browser. (if available)" );
            println( "  --compiler=<str:compiler>   Generate documentation for compiler. (default = %s)", build_compiler_to_string( DEFAULT_COMPILER ).cc );
            println( "                                 valid: clang, gcc, msvc, cc" );
            println( "  --target=<str:target>       Generate documentation for target platform. (default = native)" );
            println( "                                 valid: native" );
            println( "  --release                   Generate documentation for release mode. (default = false)" );
            println( "  --optimized                 Generate documentation for optimized functions. (default = false)" );
            println( "  --enable-stdlib             Generate documentation with C standard library enabled. (default = win32:false posix:always true web:false)" );
            println( "  --enable-logging            Generate documentation with logging macros enabled. (default = false)" );
            println( "  --enable-assertions         Generate documentation with assertion macros enabled. (default = false)" );
            println( "  --enable-simd               Generate documentation with simd functions enabled. (default = false)" );
            println( "  --enable-simd-256           Generate documentation with simd 256 functions enabled. (default = false)" );
            println( "  --static                    Generate documentation for static version of library. (default = false)" );
        } break;
        case MODE_DOCS: {
            println( "USAGE:    ./cbuild docs [args,opt]\n" );
            println( "  --compiler=<str:compiler>   Set compiler to build with. (default = %s)", build_compiler_to_string( DEFAULT_COMPILER ).cc );
            println( "                                 valid: clang, gcc, msvc, cc" );
            println( "  --target=<str:target>       Set target platform. (default = native)" );
            println( "                                 valid: native" );
            println( "  --release                   Enable release mode. (default = false)" );
            println( "  --optimized                 Enable optimizations turned on. (default = false)" );
            println( "  --enable-logging            Enable logging from library. (default = false)" );
            println( "  --enable-assertions         Enable assertions from library. (default = false)" );
        } break;
        case MODE_LSP: {
            println( "USAGE:    ./cbuild lsp [args,opt]\n" );
            println( "  --compiler=<str:compiler>   Set compiler to build with. (default = %s)", build_compiler_to_string( DEFAULT_COMPILER ).cc );
            println( "                                 valid: clang, gcc, msvc, cc" );
            println( "  --target=<str:target>       Set target platform. (default = native)" );
            println( "                                 valid: native" );
            println( "  --release                   Enable release mode. (default = false)" );
            println( "  --optimized                 Enable optimizations turned on. (default = false)" );
            println( "  --enable-stdlib             Link with C standard library. (default = win32:false posix:always true web:false)" );
            println( "  --enable-logging            Enable logging from library. (default = false)" );
            println( "  --enable-assertions         Enable assertions from library. (default = false)" );
            println( "  --enable-simd               Enable SSE instructions on x86, NEON instructions on ARM. (default = false)" );
            println( "  --enable-simd-256           Enable AVX/AVX2 on x86, does nothing on ARM. (default = false)" );
        } break;
        case MODE_CLEAN: {
            println( "USAGE:    ./cbuild clean [args,opt]\n" );
            println( "  --output=<dir-path>   Set output directory. (default = ./build)" );
        } break;
    }
    return true;
}
b32 mode_from_string( string str, Mode* out_mode ) {
    switch( str.cc[0] ) {
        case 'b': {
            if( string_cmp( str, string_text( "build" ) ) ) {
                *out_mode = MODE_BUILD;
                return true;
            }
        } break;
        case 't': {
            if( string_cmp( str, string_text( "test" ) ) ) {
                *out_mode = MODE_TEST;
                return true;
            }
        } break;
        case 'd': {
            if( string_cmp( str, string_text( "docs" ) ) ) {
                *out_mode = MODE_DOCS;
                return true;
            }
        } break;
        case 'l': {
            if( string_cmp( str, string_text( "lsp" ) ) ) {
                *out_mode = MODE_LSP;
                return true;
            }
        } break;
        case 'c': {
            if( string_cmp( str, string_text( "clean" ) ) ) {
                *out_mode = MODE_CLEAN;
                return true;
            }
        } break;
        case 'h': {
            if( string_cmp( str, string_text( "help" ) ) ) {
                *out_mode = MODE_HELP;
                return true;
            }
        } break;
    }

    return false;
}
string mode_to_string( Mode mode ) {
    switch( mode ) {
        case MODE_HELP  : return string_text( "help" );
        case MODE_BUILD : return string_text( "build" );
        case MODE_TEST  : return string_text( "test" );
        case MODE_DOCS  : return string_text( "docs" );
        case MODE_LSP   : return string_text( "lsp" );
        case MODE_CLEAN : return string_text( "clean" );
    }
}

b32 build_compiler_from_string( string str, BuildCompiler* out_compiler ) {
    if( string_is_empty( str ) ) {
        return false;
    }

    switch( str.cc[0] ) {
        case 'c': {
            if( str.len == 2 && string_cmp( str, string_text( "cc" ) ) ) {
                *out_compiler = BC_CC;
                return true;
            } else if( string_cmp( str, string_text( "clang" ) ) ) {
                *out_compiler = BC_CLANG;
                return true;
            }
        } break;
        case 'g': {
            if( string_cmp( str, string_text( "gcc" ) ) ) {
                *out_compiler = BC_GCC;
                return true;
            }
        } break;
        case 'm': {
            if( string_cmp( str, string_text( "msvc" ) ) ) {
                *out_compiler = BC_MSVC;
                return true;
            }
        } break;
    }

    return false;
}
string build_compiler_to_string( BuildCompiler compiler ) {
    switch( compiler ) {
        case BC_CC    : return string_text( "cc" );
        case BC_CLANG : return string_text( "clang" );
        case BC_GCC   : return string_text( "gcc" );
        case BC_MSVC  : return string_text( "msvc" );
    }
}
string build_compiler_name( BuildCompiler compiler ) {
    switch( compiler ) {
        case BC_CC    : return string_text( "cc" );
        case BC_CLANG : return string_text( "clang" );
        case BC_GCC   : return string_text( "gcc" );
        case BC_MSVC  : return string_text( "cl" );
    }
}

b32 build_target_from_string( string str, BuildTarget* out_target ) {
    if( string_is_empty( str ) ) {
        return false;
    }

    switch( str.cc[0] ) {
        case 'n': {
            if( string_cmp( str, string_text( "native") ) ) {
                *out_target = T_NATIVE;
                return true;
            }
        } break;
        case 'w': {
            if( string_cmp( str, string_text( "web" ) ) ) {
                *out_target = T_WEB;
                return true;
            }
        } break;
    }

    return false;
}
string build_target_to_string( BuildTarget target ) {
    switch( target ) {
        case T_NATIVE: return string_text( "native" );
        case T_WEB:    return string_text( "web" );
    }
}
string build_target_name( BuildTarget target ) {
    switch( target ) {
        case T_NATIVE: {
#if defined( PLATFORM_WINDOWS )
            return string_text( "win32" );
#elif defined( PLATFORM_LINUX )
            return string_text( "linux" );
#elif defined( PLATFORM_MACOS )
            return string_text( "macos" );
#else
            return string_text( "unknown" );
#endif
        } break;
        case T_WEB: return string_text( "web" );
    }
}
string build_native_arch(void) {
#if defined( ARCH_64BIT )
    #if defined( ARCH_X86 )
        return string_text( "x86_64" );
    #elif defined( ARCH_ARM )
        return string_text( "arm64" );
    #else
        return string_text( "unknown64" );
    #endif
#else
    #if defined(ARCH_64BIT)
        return string_text( "x86" );
    #elif defined( ARCH_ARM )
        return string_text( "arm" );
    #else
        return string_text( "unknown32" );
    #endif
#endif
}

b32 generate_command_line( Command* cmd ) {
    if( !path_exists( "generated" ) ) {
        if( !dir_create( "generated" ) ) {
            cb_error( "failed to create 'generated' directory!" );
            return false;
        }
    }

    const char* core_command_line_path = "generated/core_command_line.c";
    const char* core_command_line_path_temp = "generated/core_command_line_temp.c";

    FD core_command_line;
    if( !fd_open(
        core_command_line_path_temp,
        FOPEN_CREATE | FOPEN_WRITE, &core_command_line
    ) ) {
        cb_error( "failed to create generated core_command_line!" );
        return false;
    }

    dstring* flat = command_flatten_dstring( cmd );
    if( !flat ) {
        cb_error(
            "failed to generate command line! "
            "failed to allocate flat command buffer!");
        fd_close( &core_command_line );
        return false;
    }

    #define write( format, args... ) do {\
        if( !fd_write_fmt( &core_command_line, format, ##args ) ) {\
            cb_error( "failed to write to core_command_line!");\
            dstring_free(flat);\
            fd_close(&core_command_line);\
            file_remove( core_command_line_path_temp );\
            return false;\
        }\
    } while(0)

    write( "// generated file\n" );
    write( "#include \"core/types.h\"\n");
    write( "const char external_core_command_line[] = \"%s\";\n", flat );
    write( "usize external_core_command_line_len = sizeof(external_core_command_line);\n");
    write( "\n" );

    fd_close( &core_command_line );
    if( path_exists( core_command_line_path ) ) {
        if( !file_remove(core_command_line_path ) ) {
            cb_error( "failed to remove old core_command_line!" );
            dstring_free( flat );
            return false;
        }
    }
    if( !file_move( core_command_line_path, core_command_line_path_temp ) ) {
        cb_error( "failed to rename temp core command line!");
        dstring_free( flat );
        return false;
    }

    cb_info( "generated core_command_line at %s!", core_command_line_path );
    #undef write
    dstring_free( flat );
    return true;
}

#undef println
#define CBUILD_IMPLEMENTATION
#include "cbuild.h"
