#if !defined(CORE_DEFINES_H)
#define CORE_DEFINES_H
/**
 * @file   defines.h
 * @brief  Useful conditional defines.
 * 
 * CORE_COMPILER_*, CORE_ARCH_*, CORE_PLATFORM_* etc.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   January 30, 2024
*/
#if defined(__cplusplus)
    /// @brief Check if compiler is C or C++
    ///
    /// If defined, compiling in C++, otherwise C.
    #define CORE_CPLUSPLUS
#endif

#if defined(__GNUC__) || defined(__GNUG__)
    /// @brief Compiler is GCC.
    #define CORE_COMPILER_GCC

    #if defined(CORE_COMPILER_MINGW_VERSION)
        /// @brief String literal describing version of compiler in use.
        #define CORE_COMPILER_VERSION\
            CORE_COMPILER_MINGW_VERSION "gcc " __VERSION__
    #else
        /// @brief String literal describing version of compiler in use.
        #define CORE_COMPILER_VERSION "gcc " __VERSION__
    #endif

#endif

#if defined(__clang__)
    #undef CORE_COMPILER_GCC
    /// @brief Compiler is clang.
    #define CORE_COMPILER_CLANG

    #undef CORE_COMPILER_VERSION
    #if defined(CORE_COMPILER_MINGW_VERSION)
        /// @brief String literal describing version of compiler in use.
        #define CORE_COMPILER_VERSION\
            CORE_COMPILER_MINGW_VERSION "clang " __clang_version__ 
    #else
        /// @brief String literal describing version of compiler in use.
        #define CORE_COMPILER_VERSION "clang " __clang_version__
    #endif

#endif

#if defined(_MSC_VER)
    #undef CORE_COMPILER_GCC
    #undef CORE_COMPILER_CLANG
    #undef CORE_COMPILER_MINGW_VERSION
    /// @brief Compiler is MSVC.
    #define CORE_COMPILER_MSVC

    #define ___MSVC_STRINGIFY(x) #x
    #define ___MSVC_STRINGIFY_MACRO(x) ___MSVC_STRINGIFY(x)

    #undef CORE_COMPILER_VERSION
    /// @brief String literal describing version of compiler in use.
    #define CORE_COMPILER_VERSION "MSVC " ___MSVC_STRINGIFY_MACRO( _MSC_VER ) " "
#endif

#if !defined(CORE_COMPILER_GCC) && !defined(CORE_COMPILER_CLANG) && !defined(CORE_COMPILER_MSVC)
    /// @brief Compiler is unknown.
    #define CORE_COMPILER_UNKNOWN
    /// @brief String literal describing version of compiler in use.
    #define CORE_COMPILER_VERSION "unknown "
#endif

#if !defined(CORE_COMPILER_MSVC)
    #if defined(__MINGW64__)
        /// @brief String describing version of MinGW in use.
        ///
        /// Only defined in Windows builds with MinGW clang or gcc.
        #define CORE_COMPILER_MINGW_VERSION "MinGW64 "
    #elif defined(__MINGW32__)
        /// @brief String describing version of MinGW in use.
        ///
        /// Only defined in Windows builds with MinGW clang or gcc.
        #define CORE_COMPILER_MINGW_VERSION "MinGW32 "
    #endif
#endif

// platform defines
#if defined(_WIN32) || defined(_WIN64)
    #undef CORE_PLATFORM_WINDOWS
    /// @brief Platform is Windows.
    #define CORE_PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__gnu_linux__)
    #undef CORE_PLATFORM_LINUX
    /// @brief Platform is GNU/Linux.
    #define CORE_PLATFORM_LINUX
#elif defined(__ANDROID__)
    #undef CORE_PLATFORM_ANDROID
    /// @brief Platform is Android.
    #define CORE_PLATFORM_ANDROID
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
        #undef CORE_PLATFORM_IOS
        /// @brief Platform is iOS.
        #define CORE_PLATFORM_IOS
    #elif TARGET_OS_MAC
        #undef CORE_PLATFORM_MACOS
        /// @brief Platform is MacOS.
        #define CORE_PLATFORM_MACOS
    #endif
#else
    #undef CORE_PLATFORM_UNKNOWN
    /// @brief Platform is unknown.
    #define CORE_PLATFORM_UNKNOWN
#endif

// TODO(alicia): android libc is not technically
// POSIX compliant? and im not sure about iOS :(
#if defined(CORE_PLATFORM_LINUX) || defined(CORE_PLATFORM_MACOS) || defined(CORE_PLATFORM_ANDROID) || defined(CORE_PLATFORM_IOS)
    /// @brief Platform is POSIX compliant.
    #define CORE_PLATFORM_POSIX
#endif

#if defined(__i386__) || defined(_M_IX86)
    #undef CORE_ARCH_X86
    /// @brief Architecture is x86.
    #define CORE_ARCH_X86
    #undef CORE_ARCH_32_BIT
    /// @brief Architecture is 32-bit.
    #define CORE_ARCH_32_BIT

    #if !defined(CORE_ARCH_LITTLE_ENDIAN)
        #undef CORE_ARCH_LITTLE_ENDIAN
        /// @brief Little endian.
        #define CORE_ARCH_LITTLE_ENDIAN
    #endif
#endif

#if defined(__x86_64__) || defined(_M_X64)
    #undef CORE_ARCH_X86
    /// @brief Architecture is x86.
    #define CORE_ARCH_X86

    #undef CORE_ARCH_32_BIT
    #undef CORE_ARCH_64_BIT
    /// @brief Architecture is 64-bit.
    #define CORE_ARCH_64_BIT

    #undef CORE_ARCH_LITTLE_ENDIAN
    /// @brief Little endian.
    #define CORE_ARCH_LITTLE_ENDIAN
#endif

#if defined(__arm__) || defined(_M_ARM)
    #undef CORE_ARCH_ARM
    /// @brief Architecture is ARM.
    #define CORE_ARCH_ARM
    #undef CORE_ARCH_32_BIT
    /// @brief Architecture is 32-bit.
    #define CORE_ARCH_32_BIT
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
    #undef CORE_ARCH_ARM
    /// @brief Architecture is ARM.
    #define CORE_ARCH_ARM
    #undef CORE_ARCH_32_BIT
    #undef CORE_ARCH_64_BIT
    /// @brief Architecture is 64-bit.
    #define CORE_ARCH_64_BIT
#endif

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS) && !defined(CORE_ARCH_X86)
    #error "SSE instructions cannot be enabled on non-x86 architectures!"
#endif
#if defined(CORE_ENABLE_AVX_INSTRUCTIONS) && !defined(CORE_ARCH_X86)
    #error "AVX instructions cannot be enabled on non-x86 architectures!"
#endif
#if defined(CORE_ENABLE_NEON_INSTRUCTIONS) && !defined(CORE_ARCH_ARM)
    #error "NEON instructions cannot be enabled on non-ARM architectures!"
#endif

#if defined(_CLANGD)
    /// @brief LSP is clangd.
    #define CORE_LSP_CLANGD
#endif

#if defined(CORE_DOXYGEN)

/// @brief Check if compiler is C or C++
///
/// If defined, compiling in C++, otherwise C.
#define CORE_CPLUSPLUS

/// @brief Useful for suppressing erroneous warnings and errors from clangd.
#define CORE_CLANGD

/// @brief Useful for generating doxygen documentation in some unsupported circumstances.
#define CORE_DOXYGEN

/// @brief Defines #attr_core_api as #attr_export
///
/// Default = enabled.
///
/// @note Defined by compiler.
#define CORE_ENABLE_EXPORT

/// @brief Defines #attr_core_api as #attr_clink.
///
/// Default = disabled.
///
/// Used for statically compiling library in another project.
/// @note Defined by compiler.
#define CORE_ENABLE_STATIC_BUILD

/// @brief Enable logging messages from corelib.
///
/// Default = disabled.
///
/// Logging messages are not sent if corelib is compiled without this flag.
/// @note Defined by compiler
#define CORE_ENABLE_LOGGING

/// @brief Enable regular assertions.
///
/// Default = disabled.
///
/// @see #assert()
/// @note Defined by compiler.
#define CORE_ENABLE_ASSERTIONS

/// @brief Enable debug breaks.
///
/// Default = disabled.
///
/// @see #insert_debug_break()
/// @see #debug_assert()
/// @note Defined by compiler.
#define CORE_ENABLE_DEBUG_BREAK

/// @brief Enable debug breaks.
///
/// Default = disabled.
///
/// @see #debug_assert()
/// @note Defined by compiler.
/// @note Does nothing if #CORE_ENABLE_DEBUG_BREAK is not defined.
/// @note Does nothing if #CORE_ENABLE_ASSERTIONS is not defined.
#define CORE_ENABLE_DEBUG_ASSERTIONS

/// @brief Enable debug timer blocks.
///
/// Default = disabled.
///
/// @note C++ only.
/// @note Defined by compiler.
#define CORE_ENABLE_DEBUG_TIMER_BLOCK

/// @brief Enable generic macros/function overloads
/// 
/// Default = disabled.
/// 
/// @note Defined by compiler.
#define CORE_ENABLE_GENERICS

/// @brief Enable global time function aliases.
/// @see core/time.h
///
/// Default = disabled.
///
/// @note Defined by compiler.
#define CORE_ENABLE_TIME_GLOBAL_ALIAS

/// @brief Enable x86 SSE instructions.
/// 
/// Default = enabled (x86_64 only).
///
/// @note Defined by compiler.
#define CORE_ENABLE_SSE_INSTRUCTIONS

/// @brief Enable x86 AVX instructions.
///
/// Default = disabled.
///
/// @note Defined by compiler.
#define CORE_ENABLE_AVX_INSTRUCTIONS

/// @brief Enable ARM NEON instructions.
///
/// Default = enabled (ARM only).
///
/// @note Defined by compiler.
#define CORE_ENABLE_NEON_INSTRUCTIONS

/// @brief Enable compiling as a shared executable.
///
/// Default = disabled.
///
/// Only takes effect when targetting Linux currently.
///
/// @note Defined by compiler.
#define CORE_ENABLE_SHARED_EXECUTABLE

/// @brief Enable use of __COUNTER__ in macros.
///
/// Default = disabled.
///
/// @see #unique_id_counted
/// @note Defined by compiler.
#define CORE_ENABLE_COUNTER_MACRO

#endif /* Doxygen */

// NOTE(alicia): Helper macro.
#define ___PRAGMA(x) _Pragma(#x)

#endif /* header guard */
