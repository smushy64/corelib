#if !defined(CORE_CPP_FMT_HPP) && defined(__cplusplus)
#define CORE_CPP_FMT_HPP
/**
 * @file   fmt.hpp
 * @brief  C++ String formatting.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 01, 2025
*/
#if !defined(CORE_FMT_H)
    #include "core/fmt.h"
#endif
#include "core/string.h"
#include "core/cpp/bitwise.hpp"

CPP_BITWISE_OPERATORS(FormatFloatFlags, u32);
CPP_BITWISE_OPERATORS(FormatIntFlags, u32);
CPP_BITWISE_OPERATORS(FormatStringFlags, u32);

/// @brief Stream formatted string.
/// @param[in] stream     Pointer to streaming function.
/// @param[in] target     Pointer to streaming function parameters.
/// @param[in] format     Pointer to start of format string.
/// @param     va         Variadic format arguments.
/// @return Number of characters that could not be streamed to target.
attr_header
usize stream_fmt_va(
    StreamBytesFN* stream, void* target, String format, va_list va );
/// @brief Stream formatted string.
/// @param[in] stream     Pointer to streaming function.
/// @param[in] target     Pointer to streaming function parameters.
/// @param[in] format     Pointer to start of format string.
/// @param     ...        Format arguments.
/// @return Number of characters that could not be streamed to target.
attr_header
usize stream_fmt(
    StreamBytesFN* stream, void* target, String format, ... );

// NOTE(alicia): implementation -----------------------------------------------

attr_header
usize stream_fmt_va(
    StreamBytesFN* stream, void* target, String format, va_list va 
) {
    return stream_fmt_va( stream, target, format.__pod, va );
}
attr_header
usize stream_fmt(
    StreamBytesFN* stream, void* target, String format, ...
) {
    va_list va;
    va_start( va, format );
    usize result = stream_fmt_va( stream, target, format, va );
    va_end( va );
    return result;
}


#endif /* header guard */
