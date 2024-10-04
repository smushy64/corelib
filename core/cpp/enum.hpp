#if !defined(CORE_CPP_ENUM_HPP)
#define CORE_CPP_ENUM_HPP
/**
 * @file   enum.hpp
 * @brief  Operator overloading for enums.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   October 04, 2024
*/
// IWYU pragma: begin_exports
#include "core/attributes.h"
#include "core/defines.h"
#include "core/types.h"
// IWYU pragma: end_exports

#if defined(CORE_CPLUSPLUS)

/// @brief Define C++ bitwise operators for enumerator.
/// @param Enum (enum type identifier) Name of enum.
/// @param Type (type identifier) Name of underlying type of enum. Default is int.
#define define_bitwise_operators( Enum, Type )\
attr_header Enum operator~(Enum e) {\
    return (Enum)~((Type)e);\
}\
attr_header Enum operator|( Enum lhs, Enum rhs ) {\
    return (Enum)((Type)lhs | (Type)rhs);\
}\
attr_header Enum operator&( Enum lhs, Enum rhs ) {\
    return (Enum)((Type)lhs & (Type)rhs);\
}\
attr_header Enum operator^( Enum lhs, Enum rhs ) {\
    return (Enum)((Type)lhs ^ (Type)rhs);\
}\
attr_header Enum& operator|=( Enum& lhs, Enum rhs ) {\
    return (Enum&)((Type&)lhs |= (Type)rhs);\
}\
attr_header Enum& operator&=( Enum& lhs, Enum rhs ) {\
    return (Enum&)((Type&)lhs &= (Type)rhs);\
}\
attr_header Enum& operator^=( Enum& lhs, Enum rhs ) {\
    return (Enum&)((Type&)lhs ^= (Type)rhs);\
}

#else /* C++ */

/// @brief Define C++ bitwise operators for enumerator.
/// @param Enum (enum type identifier) Name of enum.
/// @param Type (type identifier) Name of underlying type of enum. Default is int.
#define define_bitwise_operators( Enum, Type )

#endif /* C */

#endif /* header guard */
