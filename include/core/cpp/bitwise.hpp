#if !defined(CORE_CPP_BITWISE_HPP) && defined(__cplusplus)
#define CORE_CPP_BITWISE_HPP
/**
 * @file   bitwise.hpp
 * @brief  C++ Bitwise operator implementation.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 31, 2025
*/
// IWYU pragma: begin_exports
#include "core/types.h"
#include "core/attributes.h"
// IWYU pragma: end_exports

/// @brief Define bitwise operators for enum.
/// @param EnumType (type) Name of enumerator type.
/// @param Type     (type) Name of underlying enumerator type.
#define CPP_BITWISE_OPERATORS( EnumType, Type )          \
attr_header attr_hot                                     \
EnumType operator~( EnumType e ) {                       \
    Type value = (Type)e;                                \
    value = ~value;                                      \
    EnumType result = (EnumType)value;                   \
    return result;                                       \
}                                                        \
attr_header attr_hot                                     \
EnumType operator-( EnumType e ) {                       \
    Type value = (Type)e;                                \
    value = -value;                                      \
    EnumType result = (EnumType)value;                   \
    return result;                                       \
}                                                        \
attr_header attr_hot                                     \
EnumType operator!( EnumType e ) {                       \
    Type value = (Type)e;                                \
    value = !value;                                      \
    EnumType result = (EnumType)value;                   \
    return result;                                       \
}                                                        \
attr_header attr_hot                                     \
EnumType operator|( EnumType in_lhs, EnumType in_rhs ) { \
    Type lhs = (Type)in_lhs;                             \
    Type rhs = (Type)in_rhs;                             \
    EnumType result = (EnumType)(lhs | rhs);             \
    return result;                                       \
}                                                        \
attr_header attr_hot                                     \
EnumType operator&( EnumType in_lhs, EnumType in_rhs ) { \
    Type lhs = (Type)in_lhs;                             \
    Type rhs = (Type)in_rhs;                             \
    EnumType result = (EnumType)(lhs & rhs);             \
    return result;                                       \
}                                                        \
attr_header attr_hot                                     \
EnumType operator^( EnumType in_lhs, EnumType in_rhs ) { \
    Type lhs = (Type)in_lhs;                             \
    Type rhs = (Type)in_rhs;                             \
    EnumType result = (EnumType)(lhs ^ rhs);             \
    return result;                                       \
}                                                        \
attr_header attr_hot                                     \
EnumType& operator|=( EnumType& lhs, EnumType rhs ) {    \
    return lhs = lhs | rhs;                              \
}                                                        \
attr_header attr_hot                                     \
EnumType& operator&=( EnumType& lhs, EnumType rhs ) {    \
    return lhs = lhs & rhs;                              \
}                                                        \
attr_header attr_hot                                     \
EnumType& operator^=( EnumType& lhs, EnumType rhs ) {    \
    return lhs = lhs ^ rhs;                              \
}                                                        \

#endif /* header guard */
