#if !defined(CORE_ATTRIBUTES_H)
#define CORE_ATTRIBUTES_H
/**
 * @file   attributes.h
 * @brief  Function and variable attributes.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   January 30, 2024
*/
#include "core/defines.h"

/// @brief Attribute for functions that are internal to a translation unit.
#define attr_internal static
/// @brief Attribute for global variables.
#define attr_global static
/// @brief Attribute for static variables.
#define attr_local static

#if defined( CORE_CPLUSPLUS )
    /// @brief C/C++ restrict keyword
    #define attr_restrict __restrict
    /// @brief Attribute for header-only functions.
    #define attr_header inline
    /// @brief Attribute for functions with C-linkage.
    #define attr_clink extern "C"
    /// @brief Attribute for declaring values that are always readonly.
    #define attr_readonly static constexpr const
#else
    /// @brief C/C++ restrict keyword
    #define attr_restrict restrict
    /// @brief Attribute for header-only functions.
    #define attr_header static inline
    /// @brief Attribute for functions with C-linkage.
    #define attr_clink extern
    /// @brief Attribute for declaring values that are always readonly.
    #define attr_readonly static const
#endif

#if defined(CORE_PLATFORM_WINDOWS) || defined(CORE_COMPILER_MSVC)
    /// @brief Attribute for functions to be exported by dll.
    #define attr_export __declspec(dllexport)
    /// @brief Attribute for functions to be imported from dll.
    #define attr_import __declspec(dllimport) attr_clink
#else
    /// @brief Attribute for functions to be exported by dll.
    #define attr_export __attribute__((visibility("default")))
    /// @brief Attribute for functions to be imported from dll.
    #define attr_import attr_clink
#endif

#if !defined(attr_core_api)
    #if defined(CORE_ENABLE_STATIC_BUILD)
        #define attr_core_api attr_clink
    #else
        #if defined(CORE_ENABLE_EXPORT)
            #define attr_core_api attr_export
        #else
            #define attr_core_api attr_import
        #endif
    #endif
#endif

// TODO(alicia): test on MSVC

/// @brief Attribute for pushing packing value.
///
/// Structs defined after this are packed to given packing value.
/// @note Must use #attr_pack_pop() after to reset packing.
/// @see #attr_pack_pop()
#define attr_pack_push(n) ___PRAGMA( pack(push,n) )
/// @brief Attribute for resetting packing value.
///
/// @note Must be used after #attr_pack_push()
/// @see #attr_pack_push()
#define attr_pack_pop()  _Pragma( "pack(pop)" )

// TODO(alicia): test on MSVC
#if defined(CORE_COMPILER_MSVC)

/// @brief Attribute for forcing optimizations for function.
/// @warning does not do anything on MSVC
#define attr_hot
/// @brief Attribute for forcing optimizations off for function.
/// @warning does not do anything on MSVC
#define attr_cold

/// @brief Attribute for forcing function to be inlined.
#define attr_always_inline __forceinline
/// @brief Attribute for preventing inlining.
#define attr_no_inline __declspec(noinline)

/// @brief Attribute for marking functions as deprecated.
#define attr_deprecate __declspec(deprecated)

#if defined(CORE_CPLUSPLUS)
    /// @brief Attribute for marking variables/functions as unused.
    /// @note Does not do anything on MSVC if C version is < C23.
    #define attr_unused [[maybe_unused]]
#else
    /// @brief Attribute for marking variables/functions as unused.
    /// @note Does not do anything on MSVC if C version is < C23.
    #define attr_unused
#endif

/// @brief Attribute for always aligning variable to given alignment.
#define attr_align(x) __declspec( align( x ) )

/// @brief Attribute for displaying warning if return is not used.
#define attr_no_discard _Check_return_

/// @brief Attribute for functions that do not return.
#define attr_no_return __declspec( noreturn )

/// @brief Mark switch case that falls through as intentional.
#define attr_fallthrough() [[fallthrough]]

#else /* MSVC */

/// @brief Mark switch case that falls through as intentional.
#define attr_fallthrough() __attribute__ ((fallthrough))

/// @brief Attribute for forcing optimizations for function.
#define attr_hot __attribute__((__hot__))
/// @brief Attribute for forcing optimizations off for function.
#define attr_cold __attribute__((__optnone__))

/// @brief Attribute for forcing function to be inlined.
#define attr_always_inline __attribute__((__always_inline__))
/// @brief Attribute for preventing inlining.
#define attr_no_inline __attribute__((__noinline__))

/// @brief Attribute for marking functions as deprecated.
#define attr_deprecate  __attribute__((__deprecated__))

/// @brief Attribute for marking variables/functions as unused.
/// @note Does not do anything on MSVC if C version is < C23.
#define attr_unused __attribute__((__unused__))

/// @brief Attribute for always aligning variable to given alignment.
#define attr_align(x) __attribute__((__aligned__( x )))

/// @brief Attribute for displaying warning if return is not used.
#define attr_no_discard __attribute__((__warn_unused_result__))

/// @brief Attribute for functions that do not return.
#define attr_no_return _Noreturn

#endif /* GCC || CLANG */

#endif /* header guard */
