#if !defined(CORE_MACROS_H)
#define CORE_MACROS_H
/**
 * @file   macros.h
 * @brief  Macro functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   January 30, 2024
*/
#include "core/defines.h"
#include "core/attributes.h"

#if defined(CORE_COMPILER_MSVC)
    #include "core/assertions.h" // IWYU pragma: keep
#endif

#if defined(CORE_CPLUSPLUS)
    #define struct_literal( identifier ) identifier
#else
    #define struct_literal( identifier ) (identifier)
#endif

attr_header attr_always_inline attr_hot
void _unused(int a, ...) { (void)(a); }

/// @brief Mark values as unused.
///
/// Silences unused warning.
/// @param ... Values to mark as unused.
#define unused(...) _unused( 0, __VA_ARGS__ )

/// @brief Write a pragma warning message.
/// @param msg (string literal) warning message.
#define pragma_warn( msg ) ___PRAGMA( message msg )

#if defined(CORE_COMPILER_MSVC)
    /// @brief Write a pragma error message.
    /// @param msg (string literal) error message.
    #define pragma_error( msg ) static_assert( false, msg )
#else
    /// @brief Write a pragma error message.
    /// @param msg (string literal) error message.
    #define pragma_error( msg ) ___PRAGMA( GCC error msg )
#endif

/// @brief Create a 32-bit unsigned integer that encodes a version of corelib.
/// @param major (u16) Major version.
/// @param minor (u8) Minor version.
/// @param patch (u8) Patch version.
/// @return Version encoded as a 32-bit unsigned integer.
#define core_create_version( major, minor, patch ) \
    ((u32)((major) << 16u | (minor) << 8u | (patch)))
/// @brief Extract major version from a corelib version integer.
/// @param version (u32) Corelib version encoded as an unsigned 32-bit integer.
/// @return Major version.
#define core_get_major( version ) ((u16)(((version) & 0xFFFF0000u) >> 16u))
/// @brief Extract minor version from a corelib version integer.
/// @param version (u32) Corelib version encoded as an unsigned 32-bit integer.
/// @return Minor version.
#define core_get_minor( version ) ((u8)(((version) & 0x0000FF00u) >> 8u))
/// @brief Extract patch version from a corelib version integer.
/// @param version (u32) Corelib version encoded as an unsigned 32-bit integer.
/// @return Patch version.
#define core_get_patch( version ) ((u8)((version) & 0x000000FFu))

/// @brief Convert identifier to string literal.
/// @param x (any) Identifier to convert.
/// @return Null-terminated string literal.
#define stringify(x) #x
/// @brief Convert macro to string literal.
/// @param x (macro) Macro to convert.
/// @return Null-terminated string literal.
#define stringify_macro(x) stringify(x)

/// @brief Concatenate macro names.
/// @param a, b Macro names to concatenate.
/// @return Concatenated macro names.
#define concat( a, b ) a##b
/// @brief Concatenate macro values.
/// @param a, b Macros to concatenate values of.
/// @return Concatenated macro values.
#define concat_macro( a, b ) concat( a, b )

/// @brief Make a unique identifier using __LINE__.
/// @warning Do not use twice in one line or in a header as it could conflict with another id with the same name.
/// @param name Name of unique identifier.
/// @return Unique identifier.
#define unique_id( name ) concat_macro( _##name, __LINE__ )

#if defined(CORE_ENABLE_COUNTER_MACRO)
    /// @brief Make a unique identifier using __COUNTER__.
    /// @param name Name of unique identifier.
    /// @note Only works if CORE_ENABLE_COUNTER_MACRO is defined.
    /// @return Unique identifier.
    #define unique_id_counted( name ) concat_macro( _##name, __COUNTER__ )
#else
    /// @brief Make a unique identifier using __COUNTER__.
    /// @param ... Name of unique identifier.
    /// @note Only works if CORE_ENABLE_COUNTER_MACRO is defined.
    /// @return Unique identifier.
    #define unique_id_counted(...)
#endif

/// @brief Calculate length of static array at compile-time.
/// @param array (any[]) Array to get length of.
/// @return Length of array.
#define static_array_len( array ) ( sizeof((array)) / sizeof((array)[0]) )

/// @brief Reinterpret cast.
/// @param type Type to reinterpret as.
/// @param ptr Pointer to value to reinterpret.
/// @return Value pointed to by ptr reinterpreted as type.
#define rcast( type, ptr ) ( *(type *)(ptr) )
/// @brief Reinterpret cast reference.
/// @param type Type to reinterpret as.
/// @param ptr Pointer to value to reinterpret.
/// @return Value pointed to by ptr reinterpreted as pointer to type.
#define rcast_ref( type, ptr ) ( (type *)(ptr) )

/// @brief Infinite loop.
#define loop() for(;;)

#if !defined(offsetof)
    #if defined(CORE_ARCH_64_BIT)
        #if defined(__LP64__)
            /// @brief Calculate offset of field in struct.
            /// @param struct Struct to get field offset of.
            /// @param field Field to get offset of.
            /// @return Offset of field in struct.
            #define offsetof( struct, field )\
                (unsigned long)((&(((struct*)0)->field)))
        #else
            /// @brief Calculate offset of field in struct.
            /// @param struct Struct to get field offset of.
            /// @param field Field to get offset of.
            /// @return Offset of field in struct.
            #define offsetof( struct, field )\
                (unsigned long long)((&(((struct*)0)->field)))
        #endif
    #else
        /// @brief Calculate offset of field in struct.
        /// @param struct Struct to get field offset of.
        /// @param field Field to get offset of.
        /// @return Offset of field in struct.
        #define offsetof( struct, field )\
            (unsigned int)((&(((struct*)0)->field)))
    #endif
#endif

/// @brief Check if bits in bitmask are enabled in bitfield.
/// @param bitfield (unsigned integer) Bitfield to test.
/// @param bitmask (unsigned integer) Mask of bits to test.
/// @return True if bits in bitmask are enabled in bitfield.
#define bitfield_check( bitfield, bitmask )\
    (((bitfield) & (bitmask)) == (bitmask))
/// @brief Check if ONLY bits in bitmask are enabled in bitfield.
/// @param bitfield (unsigned integer) Bitfield to test.
/// @param bitmask (unsigned integer) Mask of bits to test.
/// @return True if ONLY bits in bitmask are enabled in bitfield.
#define bitfield_check_exact( bitfield, bitmask )\
    (((bitfield) & (bitmask)) == (bitfield))
/// @brief Toggle bits in bitfield using a bitmask.
/// @param bitfield (unsigned integer) Bitfield to toggle bits in.
/// @param bitmask (unsigned integer) Mask of bits to toggle.
/// @return Bitfield with bits from bitmask toggled.
#define bitfield_toggle( bitfield, bitmask )\
    ((bitfield) ^ (bitmask))
/// @brief Set bits in bitfield using a bitmask.
/// @param bitfield (unsigned integer) Bitfield to set bits in.
/// @param bitmask (unsigned integer) Mask of bits to set.
/// @return Bitfield with bits from bitmask set.
#define bitfield_set( bitfield, bitmask )\
    ((bitfield) | (bitmask))
/// @brief Clear bits in bitfield using a bitmask.
/// @param bitfield (unsigned integer) Bitfield to clear bits in.
/// @param bitmask (unsigned integer) Mask of bits to clear.
/// @return Bitfield with bits from bitmask cleared.
#define bitfield_clear( bitfield, bitmask )\
    ((bitfield) & ~(bitmask))

/// @brief Calculate number of bytes in kb kilobytes.
/// @param kb (usize) Kilobytes.
/// @return kb in bytes.
#define kilobytes(kb) ( (kb) * 1000ULL )
/// @brief Calculate number of bytes in mb megabytes.
/// @param mb (usize) Megabytes.
/// @return mb in bytes.
#define megabytes(mb) ( kilobytes( (mb) ) * 1000ULL )
/// @brief Calculate number of bytes in gb gigabytes.
/// @param gb (usize) Gigabytes.
/// @return gb in bytes.
#define gigabytes(gb) ( megabytes( (gb) ) * 1000ULL )
/// @brief Calculate number of bytes in tb terabytes.
/// @param tb Terabytes.
/// @return tb in bytes.
#define terabytes(tb) ( gigabytes( (tb) ) * 1000ULL )

/// @brief Calculate number of bytes in kb kibibytes.
/// @param kb (usize) Kibibytes.
/// @return kb in bytes.
#define kibibytes(kb) ( (kb) * 1024ULL )
/// @brief Calculate number of bytes in mb mebibytes.
/// @param mb (usize) Mebibytes.
/// @return mb in bytes.
#define mebibytes(mb) ( kibibytes( (mb) ) * 1024ULL )
/// @brief Calculate number of bytes in gb gibibytes.
/// @param gb (usize) Gibibytes.
/// @return gb in bytes.
#define gibibytes(gb) ( mebibytes( (gb) ) * 1024ULL )
/// @brief Calculate number of bytes in tb tebibytes.
/// @param tb Tebibytes.
/// @return tb in bytes.
#define tebibytes(tb) ( gibibytes( (tb) ) * 1024ULL )

// NOTE(alicia): C23 has typeof macro
#if __STDC_VERSION__ != 202311L
    /// @brief Get type of x.
    /// @param x (Any) Value to get type of.
    /// @return Type.
    #define typeof(x) __typeof__(x)
#endif

#endif /* header guard */
