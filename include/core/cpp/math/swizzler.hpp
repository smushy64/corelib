#if !defined(CORE_CPP_MATH_SWIZZLER_HPP) && defined(__cplusplus)
#define CORE_CPP_MATH_SWIZZLER_HPP
/**
 * @file   swizzler.hpp
 * @brief  C++ Math: Swizzling.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 29, 2025
*/
#include "core/types.h"
#include "core/attributes.h"

template<typename Vector, typename T, int... Indices>
struct Swizzler {
    attr_readonly usize Count = sizeof...(Indices);
    T values[Count];

    attr_always_inline attr_header attr_hot
    constexpr Vector& operator=( const Vector& rhs ) {
        int indices[Count] = { Indices... };
        for( usize i = 0; i < Count; ++i ) {
            values[indices[i]] = rhs[i];
        }
        return *(Vector*)this;
    }
    attr_always_inline attr_header attr_hot
    constexpr operator Vector() const {
        return Vector{ values[Indices]... };
    }
};

template<typename Source, typename Destination, typename T, int... Indices>
struct SwizzlerConvert {
    attr_readonly usize Count = sizeof...(Indices);
    T values[Count];

    attr_always_inline attr_header attr_hot
    constexpr Destination& operator=( const Source& rhs ) {
        int indices[Count] = { Indices... };
        for( usize i = 0; i < Count; ++i ) {
            values[indices[i]] = rhs[i];
        }
        return *(Destination*)this;
    }
    attr_always_inline attr_header attr_hot
    constexpr operator Destination() const {
        Destination result;
        int indices[Count] = { Indices... };
        for( usize i = 0; i < Count; ++i ) {
            result[i] = values[indices[i]];
        }
        return result;
    }
};

#endif /* header guard */
