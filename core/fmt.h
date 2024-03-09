#if !defined(CORE_FMT_H)
#define CORE_FMT_H
/**
 * @file   fmt.h
 * @see FMT.md
 * @brief  String formatting.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 14, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/stream.h"

/// @brief Supported integer formats.
typedef enum FormatInteger {
    /// @brief Format integer in decimal.
    FORMAT_INTEGER_DECIMAL,
    /// @brief Format integer in binary.
    FORMAT_INTEGER_BINARY,
    /// @brief Format integer in upper case hexadecimal.
    FORMAT_INTEGER_HEXADECIMAL_LOWER,
    /// @brief Format integer in lower case hexadecimal.
    FORMAT_INTEGER_HEXADECIMAL_UPPER,
} FormatInteger;

/// @brief Function prototype for stream formatting functions.
/// @see #StreamBytesFN
typedef StreamBytesFN StreamFormatFN;

/// @brief Stream a formatted string to a target using variadic arguments list.
/// @note Format string must have a length.
/// @param stream Streaming function.
/// @param[in] target Target for streaming function.
/// @param format_len Length of format string.
/// @param[in] format Format string.
/// @param va Variadic arguments.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
attr_core_api usize fmt_text_va(
    StreamFormatFN* stream, void* target,
    usize format_len, const char* format, va_list va );

/// @brief Stream a formatted string to a target.
/// @note Format string must have a length.
/// @param stream Streaming function.
/// @param[in] target Target for streaming function.
/// @param format_len Length of format string.
/// @param[in] format Format string.
/// @param ... Format arguments.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
attr_always_inline
attr_header usize fmt_text(
    StreamFormatFN* stream, void* target,
    usize format_len, const char* format, ...
) {
    va_list va;
    va_start( va, format );
    usize result = fmt_text_va( stream, target, format_len, format, va );
    va_end( va );
    return result;
}

/// @brief Stream a formatted string to a target using variadic arguments list.
/// @param stream Streaming function.
/// @param[in] target Target for streaming function.
/// @param format Format string literal.
/// @param va Variadic arguments.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
#define fmt_va( stream, target, format, va )\
    fmt_text_va( stream, target, sizeof(format) - 1, format, va )

/// @brief Stream a formatted string to a target using variadic arguments list.
/// @param stream Streaming function.
/// @param[in] target Target for streaming function.
/// @param format Format string literal.
/// @param ... Format arguments.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
#define fmt( stream, target, format, ... )\
    fmt_text( stream, target, sizeof(format) - 1, format, ##__VA_ARGS__ )

/// @brief Format a signed 8-bit integer.
/// @param stream Streaming function.
/// @param target Target for streaming function.
/// @param integer Integer to format.
/// @param format Formatting style.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
attr_core_api usize fmt_i8(
    StreamFormatFN* stream, void* target, i8 integer, FormatInteger format );
/// @brief Format an unsigned 8-bit integer.
/// @param stream Streaming function.
/// @param target Target for streaming function.
/// @param integer Integer to format.
/// @param format Formatting style.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
attr_core_api usize fmt_u8(
    StreamFormatFN* stream, void* target, u8 integer, FormatInteger format );
/// @brief Format a signed 16-bit integer.
/// @param stream Streaming function.
/// @param target Target for streaming function.
/// @param integer Integer to format.
/// @param format Formatting style.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
attr_core_api usize fmt_i16(
    StreamFormatFN* stream, void* target, i16 integer, FormatInteger format );
/// @brief Format an unsigned 16-bit integer.
/// @param stream Streaming function.
/// @param target Target for streaming function.
/// @param integer Integer to format.
/// @param format Formatting style.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
attr_core_api usize fmt_u16(
    StreamFormatFN* stream, void* target, u16 integer, FormatInteger format );
/// @brief Format a signed 32-bit integer.
/// @param stream Streaming function.
/// @param target Target for streaming function.
/// @param integer Integer to format.
/// @param format Formatting style.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
attr_core_api usize fmt_i32(
    StreamFormatFN* stream, void* target, i32 integer, FormatInteger format );
/// @brief Format an unsigned 32-bit integer.
/// @param stream Streaming function.
/// @param target Target for streaming function.
/// @param integer Integer to format.
/// @param format Formatting style.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
attr_core_api usize fmt_u32(
    StreamFormatFN* stream, void* target, u32 integer, FormatInteger format );
/// @brief Format a signed 64-bit integer.
/// @param stream Streaming function.
/// @param target Target for streaming function.
/// @param integer Integer to format.
/// @param format Formatting style.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
attr_core_api usize fmt_i64(
    StreamFormatFN* stream, void* target, i64 integer, FormatInteger format );
/// @brief Format an unsigned 64-bit integer.
/// @param stream Streaming function.
/// @param target Target for streaming function.
/// @param integer Integer to format.
/// @param format Formatting style.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
attr_core_api usize fmt_u64(
    StreamFormatFN* stream, void* target, u64 integer, FormatInteger format );
/// @brief Format a floating point number.
/// @param stream Streaming function.
/// @param target Target for streaming function.
/// @param f Float to format.
/// @param precision How many decimal places to stream, can be zero.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
attr_core_api usize fmt_float(
    StreamFormatFN* stream, void* target, f64 f, u32 precision );
/// @brief Format a boolean.
/// @param stream Streaming function.
/// @param target Target for streaming function.
/// @param b Boolean to format.
/// @param binary If true, formatted as 0/1 otherwise formatted as true/false.
/// @return Zero if stream was successful, otherwise number of bytes required in stream target.
attr_core_api usize fmt_bool(
    StreamFormatFN* stream, void* target, b32 b, b32 binary );

#if defined(CORE_ARCH_64_BIT)
    /// @brief Format an unsigned pointer sized integer.
    /// @param stream Streaming function.
    /// @param target Target for streaming function.
    /// @param integer Integer to format.
    /// @param format Formatting style.
    /// @return Zero if stream was successful, otherwise number of bytes required in stream target.
    #define fmt_usize fmt_u64
    /// @brief Format a signed pointer sized integer.
    /// @param stream Streaming function.
    /// @param target Target for streaming function.
    /// @param integer Integer to format.
    /// @param format Formatting style.
    /// @return Zero if stream was successful, otherwise number of bytes required in stream target.
    #define fmt_isize fmt_i64
#else
    /// @brief Format an unsigned pointer sized integer.
    /// @param stream Streaming function.
    /// @param target Target for streaming function.
    /// @param integer Integer to format.
    /// @param format Formatting style.
    /// @return Zero if stream was successful, otherwise number of bytes required in stream target.
    #define fmt_usize fmt_u32
    /// @brief Format a signed pointer sized integer.
    /// @param stream Streaming function.
    /// @param target Target for streaming function.
    /// @param integer Integer to format.
    /// @param format Formatting style.
    /// @return Zero if stream was successful, otherwise number of bytes required in stream target.
    #define fmt_isize fmt_i32
#endif

#endif /* header guard */
