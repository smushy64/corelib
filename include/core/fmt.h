#if !defined(CORE_FMT_H)
#define CORE_FMT_H
/**
 * @file   fmt.h
 * @brief  String formatting.
 * @see FORMAT.md
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 14, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/stream.h"

// forward declaration
struct TimeSplit;
struct _StringPOD;
attr_core_api
usize stream_fmt_time(
    StreamBytesFN* stream, void* target, const struct TimeSplit* ts,
    int padding, usize opt_format_len, const char* opt_format );

/// @brief Formatter function for any type.
/// @param[in] stream   Streaming function.
/// @param[in] target   Pointer to streaming target.
/// @param     args_len Length of format arguments.
/// @param[in] args     Pointer to format arguments.
/// @param[in] params   Pointer to format parameters.
/// @return Number of bytes that could not be streamed to target.
typedef usize AnyFormatterFN(
    StreamBytesFN* stream, void* target,
    usize args_len, const char* args, const void* params );

/// @brief Char formatting case.
typedef enum FormatCharCasing {
    /// @brief Do not convert characters.
    FMT_CASING_AS_IS,
    /// @brief Convert alphabetic characters to upper-case.
    FMT_CASING_UPPER,
    /// @brief Convert alphabetic characters to lower-case.
    FMT_CASING_LOWER,
} FormatCharCasing;
/// @brief Flags for float formatting.
typedef enum FormatFloatFlags {
    /// @brief Format float as if it is an amount of memory.
    FMT_FLOAT_MEMORY      = 0b00000001,
    /// @brief Same as #FMT_FLOAT_MEMORY, format in kibibytes instead.
    FMT_FLOAT_MEMORY_KIBI = 0b00000010,
    /// @brief Include commas in string.
    FMT_FLOAT_SEPARATE    = 0b00000100,
    /// @brief Pad left side of number with zeroes.
    FMT_FLOAT_ZERO_PAD    = 0b00001000,
    /// @brief Floats are part of a vector2.
    FMT_FLOAT_VECTOR2     = 0b00010000,
    /// @brief Floats are part of a vector3.
    FMT_FLOAT_VECTOR3     = 0b00100000,
    /// @brief Floats are part of a vector4.
    FMT_FLOAT_VECTOR4     = 0b00110000,
    /// @brief Floats are 64-bit. (doubles)
    FMT_FLOAT_F64         = 0b01000000,
} FormatFloatFlags;
/// @brief Mask for extracting which type of vector float is.
#define FMT_FLOAT_VECTOR_MASK\
    (FMT_FLOAT_VECTOR2 | FMT_FLOAT_VECTOR3 | FMT_FLOAT_VECTOR4)
/// @brief Maximum allowed float precision.
#define FMT_FLOAT_MAX_PRECISION (6)
/// Flags for integer formatting.
typedef enum FormatIntFlags {
    /// @brief Format integer as if it is an amount of memory.
    FMT_INT_MEMORY       = 0b00000001,
    /// @brief Same as #FMT_INT_MEMORY, format in kibibytes instead.
    FMT_INT_MEMORY_KIBI  = 0b00000010,
    /// @brief Include commas or ticks in string.
    FMT_INT_SEPARATE     = 0b00000100,
    /// @brief Pad left side of number with zeroes.
    FMT_INT_ZERO_PAD     = 0b00001000,
    /// @brief Integers are part of a vector2.
    FMT_INT_VECTOR2      = 0b00010000,
    /// @brief Integers are part of a vector3.
    FMT_INT_VECTOR3      = 0b00100000,
    /// @brief Integers are part of a vector4.
    FMT_INT_VECTOR4      = 0b00110000,
    /// @brief Write in upper-case hexadecimal.
    FMT_INT_HEX_UPPER    = 0b01000000,
    /// @brief Write in lower-case hexadecimal.
    FMT_INT_HEX_LOWER    = 0b10000000,
    /// @brief Write in binary.
    FMT_INT_BINARY       = 0b11000000,
    /// @brief Write all digits of integer.
    FMT_INT_FULL_WIDTH   = 0b00000001 << 8,
    /// @brief Integers are 8-bit.
    FMT_INT_BITDEPTH_8   = 0b00000010 << 8,
    /// @brief Integers are 16-bit.
    FMT_INT_BITDEPTH_16  = 0b00000100 << 8,
    /// @brief Integers are 32-bit.
    FMT_INT_BITDEPTH_32  = 0b00001000 << 8,
    /// @brief Integers are 64-bit.
    FMT_INT_BITDEPTH_64  = 0b00001100 << 8,
    /// @brief Integers are pointer sized.
    FMT_INT_BITDEPTH_PTR = 0b00010000 << 8,
    /// @brief Integers are signed.
    FMT_INT_SIGNED       = 0b00100000 << 8,
} FormatIntFlags;
/// @brief Mask for extracting integer vector flags.
#define FMT_INT_VECTOR_MASK (FMT_INT_VECTOR2 | FMT_INT_VECTOR3 | FMT_INT_VECTOR4)
/// @brief Mask for extracting integer number base flags.
#define FMT_INT_BASE_MASK (FMT_INT_HEX_UPPER | FMT_INT_HEX_LOWER | FMT_INT_BINARY)
/// @brief Mask for extracting integer bitdepth flags.
#define FMT_INT_BITDEPTH_MASK\
    (FMT_INT_BITDEPTH_8 | FMT_INT_BITDEPTH_16 |\
        FMT_INT_BITDEPTH_32 | FMT_INT_BITDEPTH_64 | FMT_INT_BITDEPTH_PTR)
/// @brief Flags for string formatting.
typedef enum FormatStringFlags {
    /// @brief String is a Path slice.
    FMT_STRING_IS_PATH                 = 0x1, // 001
    /// @brief If FMT_STRING_IS_PATH, replace path separators with platform separators.
    FMT_STRING_PATH_REPLACE_SEPARATORS = 0x2, // 010
    /// @brief If FMT_STRING_IS_PATH, canonicalize path.
    FMT_STRING_PATH_CANONICALIZE       = 0x4, // 100

    /// @brief Bit in bitfield, don't use directly.
    FMT_STRING_CASING_AS_IS_BIT = 0x0, // 0
    /// @brief Bit in bitfield, don't use directly.
    FMT_STRING_CASING_UPPER_BIT = 0x1, // 1
    /// @brief Bit in bitfield, don't use directly.
    FMT_STRING_CASING_LOWER_BIT = 0x2, // 2

    /// @brief Don't change character case.
    FMT_STRING_CASING_AS_IS = (FMT_STRING_CASING_AS_IS_BIT << 4), // 00000
    /// @brief Change characters to upper case.
    FMT_STRING_CASING_UPPER = (FMT_STRING_CASING_UPPER_BIT << 4), // 01000
    /// @brief Change characters to lower case.
    FMT_STRING_CASING_LOWER = (FMT_STRING_CASING_LOWER_BIT << 4), // 10000

    /// @brief Mask for isolating casing bits.
    FMT_STRING_CASING_MASK = ((0x2 | 0x1) << 4),
} FormatStringFlags;
/// @brief Types that formatter can handle.
typedef enum FormatType {
    /// @brief Type to format is boolean.
    FT_BOOL,
    /// @brief Type to format is character.
    FT_CHAR,
    /// @brief Type to format is string.
    FT_STRING,
    /// @brief Type to format is float.
    FT_FLOAT,
    /// @brief Type to format is integer.
    FT_INT,
    /// @brief Type to format is time split.
    FT_TIME,
    /// @brief Type to format is any.
    FT_ANY,
} FormatType;

/// @brief Boolean format arguments.
typedef struct BoolFormatArguments {
    /// @brief If boolean should be formatted as binary.
    b32 binary;
} BoolFormatArguments;

/// @brief Character format arguments.
typedef struct CharFormatArguments {
    /// @brief How many times to repeat character.
    u32 repeat;
    /// @brief What casing should character be printed in.
    FormatCharCasing casing;
} CharFormatArguments;

/// @brief String format arguments.
typedef struct StringFormatArguments {
    /// @brief String format flags.
    FormatStringFlags flags;
} StringFormatArguments;

/// @brief Float format arguments.
typedef struct FloatFormatArguments {
    /// @brief How much precision to print with.
    int precision;
    /// @brief Float formatting flags.
    enum FormatFloatFlags flags;
} FloatFormatArguments;

/// @brief Integer format arguments.
typedef struct IntFormatArguments {
    /// @brief Integer formatting flags.
    enum FormatIntFlags flags;
} IntFormatArguments;

/// @brief Time format arguments.
typedef struct TimeFormatArguments {
    /// @brief Size of format string.
    usize fmt_len;
    /// @brief Pointer to time format string.
    const char* fmt;
} TimeFormatArguments;

/// @brief Any format arguments.
typedef struct AnyFormatArguments {
    /// @brief Parameters to any formatter.
    const void* params;
    /// @brief Arguments to any formatter.
    struct {
        /// @brief Length of any formatter arguments.
        u32         len;
        /// @brief Pointer to start of any formatter arguments.
        const char* cbuf;
    } args;
} AnyFormatArguments;

/// @brief Arguments for formatter.
typedef struct FormatArguments {
    /// @brief Type of data to format.
    enum FormatType type;
    /// @brief Pointer to start of data to format buffer.
    const void*     data;
    /// @brief Number of padding characters to include in formatted string.
    int             padding;
    /// @brief Number of items to format.
    u32             count;
    /// @brief Anonymous union of format arguments.
    union {
        /// @brief Boolean format arguments.
        BoolFormatArguments boolean;
        /// @brief Character format arguments.
        CharFormatArguments character;
        /// @brief String format arguments.
        StringFormatArguments string;
        /// @brief Float format arguments.
        FloatFormatArguments floating;
        /// @brief Integer format arguments.
        IntFormatArguments integer;
        /// @brief Time format arguments.
        TimeFormatArguments time;
        /// @brief Any format arguments.
        AnyFormatArguments any;
    };
} FormatArguments;

/// @brief Stream boolean formatted string.
/// @param[in] stream   Pointer to streaming function.
/// @param[in] target   Pointer to streaming function parameters.
/// @param     padding  Padding for format string.
/// @param     count    Number of booleans in @c booleans.
/// @param[in] booleans Pointer to start of boolean array.
/// @param[in] args     Boolean format arguments.
/// @return Number of characters that could not be streamed to target.
attr_core_api
usize stream_fmt_bool(
    StreamBytesFN* stream, void* target,
    int padding, u32 count, const b32* booleans,
    struct BoolFormatArguments* args );
/// @brief Stream character formatted string.
/// @param[in] stream     Pointer to streaming function.
/// @param[in] target     Pointer to streaming function parameters.
/// @param     padding    Padding for format string.
/// @param     count      Number of booleans in @c characters.
/// @param[in] characters Pointer to start of character array.
/// @param[in] args       Character format arguments.
/// @return Number of characters that could not be streamed to target.
attr_core_api
usize stream_fmt_char(
    StreamBytesFN* stream, void* target,
    int padding, u32 count, const char* characters,
    struct CharFormatArguments* args );
/// @brief Stream string formatted string.
/// @param[in] stream     Pointer to streaming function.
/// @param[in] target     Pointer to streaming function parameters.
/// @param     padding    Padding for format string.
/// @param     string_len Length of string to format.
/// @param[in] string     Pointer to start of string.
/// @param[in] args       String format arguments.
/// @return Number of characters that could not be streamed to target.
attr_core_api
usize stream_fmt_string(
    StreamBytesFN* stream, void* target,
    int padding, usize string_len, const void* string,
    struct StringFormatArguments* args );
/// @brief Stream float formatted string.
/// @param[in] stream  Pointer to streaming function.
/// @param[in] target  Pointer to streaming function parameters.
/// @param     padding Padding for format string.
/// @param     count   Number of floats to format.
/// @param[in] floats  Pointer to start of float array.
/// @param[in] args    Float format arguments.
/// @return Number of characters that could not be streamed to target.
attr_core_api
usize stream_fmt_float(
    StreamBytesFN* stream, void* target,
    int padding, u32 count, const void* floats,
    struct FloatFormatArguments* args );
/// @brief Stream integer formatted string.
/// @param[in] stream   Pointer to streaming function.
/// @param[in] target   Pointer to streaming function parameters.
/// @param     padding  Padding for format string.
/// @param     count    Number of integers to format.
/// @param[in] integers Pointer to start of integer array.
/// @param[in] args     Integer format arguments.
/// @return Number of characters that could not be streamed to target.
attr_core_api
usize stream_fmt_int(
    StreamBytesFN* stream, void* target,
    int padding, u32 count, const void* integers,
    struct IntFormatArguments* args );
/// @brief Stream formatted string from args.
/// @param[in] stream   Pointer to streaming function.
/// @param[in] target   Pointer to streaming function parameters.
/// @param[in] args     Format arguments.
/// @return Number of characters that could not be streamed to target.
attr_core_api
usize stream_fmt_args(
    StreamBytesFN* stream, void* target, FormatArguments* args );
/// @brief Stream formatted string.
/// @param[in] stream     Pointer to streaming function.
/// @param[in] target     Pointer to streaming function parameters.
/// @param[in] format     Pointer to start of format string.
/// @param     va         Variadic format arguments.
/// @return Number of characters that could not be streamed to target.
attr_core_api
usize stream_fmt_va(
    StreamBytesFN* stream, void* target, struct _StringPOD format, va_list va );
/// @brief Stream formatted string.
/// @param[in] stream     Pointer to streaming function.
/// @param[in] target     Pointer to streaming function parameters.
/// @param[in] format     Pointer to start of format string.
/// @param     ...        Format arguments.
/// @return Number of characters that could not be streamed to target.
attr_core_api
usize stream_fmt(
    StreamBytesFN* stream, void* target, struct _StringPOD format, ... );
/// @brief Stream formatted string.
/// @param[in] stream (StreamBytesFN*) Pointer to streaming function.
/// @param[in] target (void*)          Pointer to streaming function parameters.
/// @param[in] format (string literal) Format string literal.
/// @param     va     (va_list)        Variadic format arguments.
/// @return (usize) Number of characters that could not be streamed to target.
#define stream_fmt_va_text( stream, target, format, va )\
    stream_fmt_va( stream, target, string_text(format), va )
/// @brief Stream formatted string.
/// @param[in] stream (StreamBytesFN*) Pointer to streaming function.
/// @param[in] target (void*)          Pointer to streaming function parameters.
/// @param[in] format (string literal) Format string literal.
/// @param     ...    (args)           Format arguments.
/// @return (usize) Number of characters that could not be streamed to target.
#define stream_fmt_text( stream, target, format, ... )\
    stream_fmt( stream, target, string_text(format), ##__VA_ARGS__ )

#if !defined(CORE_CPP_FMT_HPP)
    #include "core/cpp/fmt.hpp"
#endif

#endif /* header guard */
