#if !defined(CORE_TYPES_H)
#define CORE_TYPES_H
/**
 * @file   types.h
 * @brief  Integral type definitions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   January 31, 2024
*/
#include "core/defines.h"

/// @brief Unsigned 8-bit integer.
typedef unsigned char  u8;
/// @brief Unsigned 16-bit integer.
typedef unsigned short u16;
/// @brief Unsigned 32-bit integer.
typedef unsigned int   u32;

/// @brief Signed 8-bit integer.
typedef signed char  i8;
/// @brief Signed 16-bit integer.
typedef signed short i16;
/// @brief Signed 32-bit integer.
typedef signed int   i32;

#if defined(CORE_COMPILER_MSVC)
    /// @brief Unsigned 64-bit integer.
    typedef unsigned __int64 u64;
    /// @brief Signed 64-bit integer.
    typedef __int64 i64;
#else
    #if defined(__LP64__)
        /// @brief Unsigned 64-bit integer.
        typedef unsigned long u64;
        /// @brief Signed 64-bit integer.
        typedef signed   long i64;
    #else
        /// @brief Unsigned 64-bit integer.
        typedef unsigned long long u64;
        /// @brief Signed 64-bit integer.
        typedef signed   long long i64;
    #endif
#endif

#if defined(CORE_ARCH_64_BIT)
    /// @brief Unsigned pointer sized integer.
    typedef u64 usize;
    /// @brief Signed pointer sized integer.
    typedef i64 isize;
#else
    /// @brief Unsigned pointer sized integer.
    typedef u32 usize;
    /// @brief Signed pointer sized integer.
    typedef i32 isize;
#endif

/// @brief 8-bit boolean.
typedef u8  b8;
/// @brief 16-bit boolean.
typedef u16 b16;
/// @brief 32-bit boolean.
typedef u32 b32;

#undef  TRUE
#define TRUE  (1)
#undef  FALSE
#define FALSE (0)

#undef  NULL
#define NULL  ((void*)0)

// NOTE(alicia): C++ and C23 already define true, false and nullptr
#if !(defined( CORE_CPLUSPLUS ) || __STDC_VERSION__ == 202311L)
    #undef  true
    #define true    TRUE
    #undef  false
    #define false   FALSE
    #define nullptr NULL
#endif

/// @brief 32-bit floating point.
typedef float  f32;
/// @brief 64-bit floating point.
typedef double f64;

/// @brief Void pointer.
typedef void* voidp;

#if defined(CORE_COMPILER_MSVC)
#include <stdarg.h>
// #if !defined(_VA_LIST_DEFINED)
//     #define _VA_LIST_DEFINED
//     /// @brief Variadic argument list.
//     typedef char* va_list;
// #endif
// /// @brief Start variadic argument list.
// #define va_start(va, x) __crt_va_start_a(va, x)
// /// @brief End variadic argument list.
// #define va_end(va)   __crt_va_end_a(va)
// /// @brief Pull argument from variadic argument list.
// #define va_arg(va, t)   __crt_va_arg_a(va, t)
#else

/// @brief Variadic argument list.
typedef __builtin_va_list va_list;
/// @brief Start variadic argument list.
#define va_start __builtin_va_start
/// @brief End variadic argument list.
#define va_end   __builtin_va_end
/// @brief Pull argument from variadic argument list.
#define va_arg   __builtin_va_arg

#endif

#endif /* header guard */
