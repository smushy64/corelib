#if !defined(CORE_CONSTANTS_H)
#define CORE_CONSTANTS_H
/**
 * @file   constants.h
 * @brief  Integral type constants.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 06, 2024
*/
#include "core/defines.h"

/// @brief Largest finite f32 value
#define F32_MAX (3.40282347E+38f)
/// @brief Smallest finite f32 value
#define F32_MIN (-3.40282347E+38f)
/// @brief Not a number
#define F32_NAN ( 0.0f / 0.0f )
/// @brief Smallest positive f32 value
#define F32_MIN_POS (1.17549435E-38f)
/// @brief Positive infinity
#define F32_POS_INFINITY (1.0f / 0.0f)
/// @brief Positive infinity
#define F32_NEG_INFINITY (-(1.0f / 0.0f))
/// @brief Pi constant
#define F32_PI (3.141592741f)
/// @brief Tau constant
#define F32_TAU (F32_PI * 2.0f)
/// @brief Half Pi constant
#define F32_HALF_PI (F32_PI / 2.0f)
/// @brief Euler's number.
#define F32_EULER (2.718281828459f)
/// @brief To degrees constant.
#define F32_TO_DEG (180.0f / F32_PI)
/// @brief To radians constant.
#define F32_TO_RAD (F32_PI / 180.0f)
/// @brief Epsilon constant
#define F32_EPSILON (1.1920929E-7f)
/// @brief Approximate number of significant digits in base-10
#define F32_SIGNIFICANT_DIGITS (6)
/// @brief Number of significant digits in base-2
#define F32_MANTISSA_DIGITS (24)
/// @brief Bitmask of single precision float exponent
#define F32_EXPONENT_MASK (~(0xFFFFFFFF << 8ul) << 23ul)
/// @brief Bitmask of single precision float mantissa
#define F32_MANTISSA_MASK ((1ul << 23ul) - 1ul)

/// @brief 1 Factorial.
#define F32_ONE_FACTORIAL    ( 1.0f )
/// @brief 2 Factorial.
#define F32_TWO_FACTORIAL    ( 2.0f  * F32_ONE_FACTORIAL    )
/// @brief 3 Factorial.
#define F32_THREE_FACTORIAL  ( 3.0f  * F32_TWO_FACTORIAL    )
/// @brief 4 Factorial.
#define F32_FOUR_FACTORIAL   ( 4.0f  * F32_THREE_FACTORIAL  )
/// @brief 5 Factorial.
#define F32_FIVE_FACTORIAL   ( 5.0f  * F32_FOUR_FACTORIAL   )
/// @brief 6 Factorial.
#define F32_SIX_FACTORIAL    ( 6.0f  * F32_FIVE_FACTORIAL   )
/// @brief 7 Factorial.
#define F32_SEVEN_FACTORIAL  ( 7.0f  * F32_SIX_FACTORIAL    )
/// @brief 8 Factorial.
#define F32_EIGHT_FACTORIAL  ( 8.0f  * F32_SEVEN_FACTORIAL  )
/// @brief 9 Factorial.
#define F32_NINE_FACTORIAL   ( 9.0f  * F32_EIGHT_FACTORIAL  )
/// @brief 10 Factorial.
#define F32_TEN_FACTORIAL    ( 10.0f * F32_NINE_FACTORIAL   )
/// @brief 11 Factorial.
#define F32_ELEVEN_FACTORIAL ( 11.0f * F32_TEN_FACTORIAL    )
/// @brief 12 Factorial.
#define F32_TWELVE_FACTORIAL ( 12.0f * F32_ELEVEN_FACTORIAL )

/// @brief Largest finite f64 value
#define F64_MAX (1.7976931348623157E+308)
/// @brief Smallest finite f64 value
#define F64_MIN (-1.7976931348623157E+308)
/// @brief Not a number
#define F64_NAN (0.0 / 0.0)
/// @brief Smallest positive f32 value
#define F64_MIN_POS (2.2250738585072014E-308)
/// @brief Positive infinity
#define F64_POS_INFINITY (1.0 / 0.0)
/// @brief Positive infinity
#define F64_NEG_INFINITY (-(1.0 / 0.0))
/// @brief Pi constant
#define F64_PI (3.14159265358979323846)
/// @brief Tau constant
#define F64_TAU (F64_PI * 2.0)
/// @brief Half Pi constant
#define F64_HALF_PI (F64_PI / 2.0)
/// @brief Euler's number.
#define F64_EULER (2.718281828459045)
/// @brief To degrees constant.
#define F64_TO_DEG (180.0 / F64_PI)
/// @brief To radians constant.
#define F64_TO_RAD (F64_PI / 180.0)
/// @brief Epsilon constant
#define F64_EPSILON (2.2204460492503131E-16)
/// @brief Approximate number of significant digits in base-10
#define F64_SIGNIFICANT_DIGITS (15)
/// @brief Number of significant digits in base-2
#define F64_MANTISSA_DIGITS (54)
/// @brief bitmask of double precision float exponent
#define F64_EXPONENT_MASK (0x7FFULL << 52ull)
/// @brief bitmask of double precision float mantissa 
#define F64_MANTISSA_MASK ((1ull << 52ull) - 1ull)

/// @brief 1 Factorial.
#define F64_ONE_FACTORIAL    ( 1.0 )
/// @brief 2 Factorial.
#define F64_TWO_FACTORIAL    ( 2.0  * F64_ONE_FACTORIAL    )
/// @brief 3 Factorial.
#define F64_THREE_FACTORIAL  ( 3.0  * F64_TWO_FACTORIAL    )
/// @brief 4 Factorial.
#define F64_FOUR_FACTORIAL   ( 4.0  * F64_THREE_FACTORIAL  )
/// @brief 5 Factorial.
#define F64_FIVE_FACTORIAL   ( 5.0  * F64_FOUR_FACTORIAL   )
/// @brief 6 Factorial.
#define F64_SIX_FACTORIAL    ( 6.0  * F64_FIVE_FACTORIAL   )
/// @brief 7 Factorial.
#define F64_SEVEN_FACTORIAL  ( 7.0  * F64_SIX_FACTORIAL    )
/// @brief 8 Factorial.
#define F64_EIGHT_FACTORIAL  ( 8.0  * F64_SEVEN_FACTORIAL  )
/// @brief 9 Factorial.
#define F64_NINE_FACTORIAL   ( 9.0  * F64_EIGHT_FACTORIAL  )
/// @brief 10 Factorial.
#define F64_TEN_FACTORIAL    ( 10.0 * F64_NINE_FACTORIAL   )
/// @brief 11 Factorial.
#define F64_ELEVEN_FACTORIAL ( 11.0 * F64_TEN_FACTORIAL    )
/// @brief 12 Factorial.
#define F64_TWELVE_FACTORIAL ( 12.0 * F64_ELEVEN_FACTORIAL )

/// @brief Largest u8 value
#define U8_MAX (255)
/// @brief Smallest u8 value
#define U8_MIN (0)

/// @brief Largest u16 value
#define U16_MAX (65535)
/// @brief Smallest u16 value
#define U16_MIN (0)

/// @brief Largest u32 value
#define U32_MAX (4294967295ul)
/// @brief Smallest u32 value
#define U32_MIN (0ul)

/// @brief Largest u64 value
#define U64_MAX (18446744073709551615ull)
/// @brief Smallest u64 value
#define U64_MIN (0ull)

/// @brief Largest i8 value
#define I8_MAX (127)
/// @brief Smallest i8 value
#define I8_MIN (-128)
/// @brief Sign mask
#define I8_SIGN_MASK (1 << 7)

/// @brief Largest i16 value
#define I16_MAX (32767)
/// @brief Smallest i16 value
#define I16_MIN (-32768)
/// @brief Sign mask
#define I16_SIGN_MASK (1 << 15)

/// @brief Largest i32 value
#define I32_MAX (2147483647l)
/// @brief Smallest i32 value
#define I32_MIN (-2147483648l)
/// @brief Sign mask
#define I32_SIGN_MASK (1u << 31u)

/// @brief Largest i64 value
#define I64_MAX (9223372036854775807ll)
/// @brief Smallest i64 value
#define I64_MIN (-9223372036854775807ll - 1)
/// @brief Sign mask
#define I64_SIGN_MASK (1ull << 63ull)

#if defined( CORE_ARCH_64_BIT )
    /// @brief Largest pointer sized int value.
    #define ISIZE_MAX (I64_MAX)
    /// @brief Smallest pointer sized int value.
    #define ISIZE_MIN (I64_MIN)
    /// @brief Pointer sized int sign mask.
    #define ISIZE_SIGN_MASK (I64_SIGN_MASK)

    /// @brief Largest pointer sized unsigned int value.
    #define USIZE_MAX (U64_MAX)
    /// @brief Smallest pointer sized unsigned int value.
    #define USIZE_MIN (U64_MIN)

    /// @brief Size of pointer in bits.
    #define POINTER_BIT_SIZE 64
    /// @brief Size of pointer in bytes.
    #define POINTER_BYTE_SIZE 8
#else /* 32-bit */
    /// @brief Largest pointer sized int value.
    #define ISIZE_MAX (I32_MAX)
    /// @brief Smallest pointer sized int value.
    #define ISIZE_MIN (I32_MIN)
    /// @brief Pointer sized int sign mask.
    #define ISIZE_SIGN_MASK (I32_SIGN_MASK)

    /// @brief Largest pointer sized unsigned int value.
    #define USIZE_MAX (U32_MAX)
    /// @brief Smallest pointer sized unsigned int value.
    #define USIZE_MIN (U32_MIN)

    /// @brief Size of pointer in bits.
    #define POINTER_BIT_SIZE 32
    /// @brief Size of pointer in bytes.
    #define POINTER_BYTE_SIZE 4
#endif /* Intptr Constants */

#if defined(CORE_COMPILER_MSVC)
#undef F32_POS_INFINITY
#undef F32_NEG_INFINITY
#undef F64_POS_INFINITY
#undef F64_NEG_INFINITY
#undef F32_NAN
#undef F64_NAN

#if !defined(_HUGE_ENUF)
    #define _HUGE_ENUF 1e+300
#endif

#define F32_POS_INFINITY ((float)( _HUGE_ENUF * _HUGE_ENUF ))
#define F64_POS_INFINITY ((double)( _HUGE_ENUF * _HUGE_ENUF ))

#define F32_NEG_INFINITY (-F32_POS_INFINITY)
#define F64_NEG_INFINITY (-F64_POS_INFINITY)

#define F32_NAN ((float)(F32_POS_INFINITY * 0.0f))
#define F64_NAN ((float)(F64_POS_INFINITY * 0.0f))

#endif

#endif /* header guard */
