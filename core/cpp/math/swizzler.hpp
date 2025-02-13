#if !defined(CORE_CPP_MATH_SWIZZLER_HPP)
#define CORE_CPP_MATH_SWIZZLER_HPP
/**
 * @file   swizzler.hpp
 * @brief  C++ template swizzling.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   November 19, 2024
*/
#include "core/attributes.h"

template<typename V, typename T, int...Indexes>
struct Swizzler {
    static const int Count = sizeof...(Indexes);
    T v[Count];

    attr_always_inline attr_header attr_hot
    constexpr V& operator=( const V& rhs ) {
        int indexes[] = { Indexes... };
        for( int i = 0; i < Count; ++i ) {
            v[indexes[i]] = rhs[i];
        }
        return *(V*)this;
    }
    attr_always_inline attr_header attr_hot
    constexpr operator V() const {
        return V{ v[Indexes]... };
    }
};

template<typename SRC, typename DST, typename T, int...Indexes>
struct SwizzlerConvert {
    static const int Count = sizeof...(Indexes);
    T v[Count];

    attr_always_inline attr_header attr_hot
    constexpr DST& operator=( const SRC& rhs ) {
        int indexes[] = { Indexes... };
        for( int i = 0; i < Count; ++i ) {
            v[indexes[i]] = rhs[i];
        }
        return *(DST*)this;
    }
    attr_always_inline attr_header attr_hot
    constexpr operator DST() const {
        DST result;
        int indexes[] = { Indexes... };
        for( int i = 0; i < Count; ++i ) {
            result[i] = v[indexes[i]];
        }
        return result;
    }
};

#endif /* header guard */
