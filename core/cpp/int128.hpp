#if !defined(CORE_CPP_INT128_HPP)
#define CORE_CPP_INT128_HPP
/**
 * @file   int128.hpp
 * @brief  C++ 128-bit integers.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 29, 2024
*/

struct Integer128CPP;
typedef Integer128CPP Integer128;

#if !defined(CORE_INT128_H)
    #include "core/int128.h"
#endif

struct Integer128CPP : public Integer128C {
    Integer128CPP() : Integer128C{.hi=0,.lo=0} {}
    Integer128CPP( i64 hi, u64 lo ) :
        Integer128C{ .hi=*(u64*)&hi, .lo=lo } {}
    Integer128CPP( u64 hi, u64 lo ) :
        Integer128C{ .hi=hi, .lo=lo } {}
    Integer128CPP( const Integer128C& c ) : Integer128C{c} {}

    inline Integer128CPP& operator+=( const Integer128C& rhs ) {
        return *this = int128_add( *this, rhs );
    }
    inline Integer128CPP& operator-=( const Integer128C& rhs ) {
        return *this = int128_sub( *this, rhs );
    }
};
inline Integer128C operator+( const Integer128C& lhs, const Integer128C& rhs ) {
    return int128_add( lhs, rhs );
}
inline Integer128C operator-( const Integer128C& lhs, const Integer128C& rhs ) {
    return int128_sub( lhs, rhs );
}
inline unsigned int operator==( const Integer128C& a, const Integer128C& b ) {
    return int128_cmp( a, b );
}
inline unsigned int operator!=( const Integer128C& a, const Integer128C& b ) {
    return !(a == b);
}

#endif /* header guard */
