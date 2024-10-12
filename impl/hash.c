/**
 * @file   hash.c
 * @brief  Hashing algorithm implementations.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   September 25, 2024
*/
#include "core/hash.h"
#include "core/memory.h"

attr_core_api hash64 hash_elf_64( usize len, const void* buf ) {
    const u8* u = (const u8*)buf;
    hash64 x = 0, result = 0;
    for( usize i = 0; i < len; ++i ) {
        result = (result << 4) + u[i];
        x = result & 0xF000000000000000;
        if( x ) {
            result ^= x >> 24;
        }
        result &= ~x;
    }
    return result;
}
attr_core_api hash64 hash_murmur2_64( usize len, const void* buf ) {
    #define M 0xc6a4a7935bd1e995
    #define R 47
    #define SEED 0xA13F346234624

    u64 h = SEED ^ ( len * M );

    const u64* data = (const u64*)buf;
    const u64* end  = data + (len / sizeof(u64));

    while( data != end ) {
        u64 k = *data++;

        k *= M;
        k ^= k >> R;
        k *= M;

        h ^= k;
        h *= M;
    }

    const u8* data2 = (const u8*)data;

    switch( len & 7 ) {
        case 7: h ^= (u64)(data2[6]) << 48;
            attr_explicit_fallthrough();
        case 6: h ^= (u64)(data2[5]) << 40;
            attr_explicit_fallthrough();
        case 5: h ^= (u64)(data2[4]) << 32;
            attr_explicit_fallthrough();
        case 4: h ^= (u64)(data2[3]) << 24;
            attr_explicit_fallthrough();
        case 3: h ^= (u64)(data2[2]) << 16;
            attr_explicit_fallthrough();
        case 2: h ^= (u64)(data2[1]) << 8;
            attr_explicit_fallthrough();
        case 1: h ^= (u64)(data2[0]);
                h *= M;
                break;
    }

    h ^= h >> R;
    h *= M;
    h ^= h >> R;

    #undef SEED
    #undef M
    #undef R

    return h;
}

#define K0 0xc3a5c85c97cb3127ULL
#define K1 0xb492b66fbe98f273ULL
#define K2 0x9ae16a3b2f90404fULL
#define KMUL 0x9ddfea08eb382d69ULL

struct InternalHashPair { u64 x, y; };
struct InternalHashUInt128 { u64 upper, lower; };

attr_internal u64 internal_hash_rotate( u64 val, int shift ) {
    return shift == 0 ? val : ((val >> shift) | (val << (64 - shift)));
}
attr_internal u64 internal_hash_shift_mix( u64 val ) {
    return val ^ (val >> 47);
}
attr_internal u64 internal_hash_unaligned_load_64( const char* p ) {
    u64 result;
    memory_copy( &result, p, sizeof(result) );
    return result;
}
attr_internal u32 internal_hash_unaligned_load_32( const char* p ) {
    u32 result;
    memory_copy( &result, p, sizeof(result) );
    return result;
}
#define swap( a, b )\
    tmp = a;\
    a = b;\
    b = tmp
attr_unused
attr_internal u64 internal_hash_bswap_64( u64 v ) {
    u8* bytes = (u8*)&v;

    u8 tmp;

    swap( bytes[0], bytes[7] );
    swap( bytes[1], bytes[6] );
    swap( bytes[2], bytes[5] );
    swap( bytes[3], bytes[4] );

    return v;
}
attr_unused
attr_internal u32 internal_hash_bswap_32( u32 v ) {
    u8* bytes = (u8*)&v;
    u8 tmp;

    swap( bytes[0], bytes[3] );
    swap( bytes[1], bytes[2] );

    return v;
}
#undef swap
attr_internal u64 internal_hash_fetch_64( const char* p ) {
    u64 result = internal_hash_unaligned_load_64( p );
#if defined(CORE_ARCH_BIG_ENDIAN)
    result = internal_hash_bswap_64(result);
#endif
    return result;
}
attr_internal u32 internal_fetch_32( const char* p ) {
    u32 result = internal_hash_unaligned_load_32( p );
#if defined(CORE_ARCH_BIG_ENDIAN)
    result = internal_hash_bswap_32(result);
#endif
    return result;
}
attr_internal struct InternalHashPair internal_hash_weak_hash_len_32_with_seeds_0(
    u64 w, u64 x, u64 y, u64 z, u64 a, u64 b
) {
    a += w;
    b = internal_hash_rotate( b + a + z, 21 );
    u64 c = a;
    a += x;
    a += y;
    b += internal_hash_rotate( a, 44 );
    return (struct InternalHashPair){ .x=a + z, .y=b + c };
}
attr_internal struct InternalHashPair internal_hash_weak_hash_len_32_with_seeds_1(
    const char* s, u64 a, u64 b
) {
    return internal_hash_weak_hash_len_32_with_seeds_0(
        internal_hash_fetch_64(s),
        internal_hash_fetch_64(s + 8),
        internal_hash_fetch_64(s + 16),
        internal_hash_fetch_64(s + 24),
        a, b );
}
attr_internal u64 internal_hash_128_to_64( struct InternalHashUInt128 v ) {
    u64 a = (v.lower ^ v.upper) * KMUL;
    a ^= (a >> 47);
    u64 b = (v.upper ^ a) * KMUL;
    b ^= (b >> 47);
    b *= KMUL;
    return b;
}
attr_internal u64 internal_hash_len_16_0( u64 u, u64 v ) {
    return internal_hash_128_to_64(
        (struct InternalHashUInt128){ .upper=u, .lower=v } );
}
attr_internal u64 internal_hash_len_16_1( u64 u, u64 v, u64 mul ) {
    u64 a = ( u ^ v ) * mul;
    a ^= ( a >> 47 );
    u64 b = ( v ^ a ) * mul;
    b ^= ( b >> 47 );
    b *= mul;
    return b;
}
attr_internal hash64 internal_hash_len_0_to_16( usize len, const char* s ) {
    if( len >= 8 ) {
        u64 mul = K2 + len * 2;
        u64 a = internal_hash_fetch_64( s ) + K2;
        u64 b = internal_hash_fetch_64( s + len - 8 );
        u64 c = internal_hash_rotate( b, 37 ) * mul + a;
        u64 d = (internal_hash_rotate( a, 25 ) + b) * mul;
        return internal_hash_len_16_1( c, d, mul );
    }
    if( len >= 4 ) {
        u64 mul = K2 + len * 2;
        u64 a   = internal_fetch_32( s );
        return internal_hash_len_16_1(
            len + (a << 3), internal_fetch_32( s + len - 4 ), mul );
    }
    if( len > 0 ) {
        u8 a = ((u8*)s)[0];
        u8 b = ((u8*)s)[len >> 1];
        u8 c = ((u8*)s)[len - 1];
        u32 y = (u32)a + ((u32)b << 1);
        u32 z = (u32)len + ((u32)c << 2);
        return internal_hash_shift_mix( y * K2 ^ z * K0 ) * K2;
    }

    return K2;
}
attr_internal hash64 internal_hash_len_17_to_32( usize len, const char* s ) {
    u64 mul = K2 + len * 2;
    u64 a   = internal_hash_fetch_64( s ) * K1;
    u64 b   = internal_hash_fetch_64( s + 8 );
    u64 c   = internal_hash_fetch_64( s + len - 8 ) * mul;
    u64 d   = internal_hash_fetch_64( s + len - 16 ) * K2;
    return internal_hash_len_16_1(
        internal_hash_rotate( a + b, 43 ) + internal_hash_rotate( c, 30 ) + d,
        a + internal_hash_rotate( b + K2, 18 ) + c, mul );
}
attr_internal hash64 internal_hash_len_33_to_64( usize len, const char* s ) {
    u64 mul = K2 + len * 2;
    u64 a   = internal_hash_fetch_64( s ) * K2;
    u64 b   = internal_hash_fetch_64( s + 8 );
    u64 c   = internal_hash_fetch_64( s + len - 24 );
    u64 d   = internal_hash_fetch_64( s + len - 32 );
    u64 e   = internal_hash_fetch_64( s + 16 ) * K2;
    u64 f   = internal_hash_fetch_64( s + 24 ) * 9;
    u64 g   = internal_hash_fetch_64( s + len - 8 );
    u64 h   = internal_hash_fetch_64( s + len - 16 ) * mul;
    u64 u   = internal_hash_rotate( a + g, 43 ) +
        ( internal_hash_rotate( b, 30 ) + c ) * 9;
    u64 v   = ( ( a + g ) ^ d ) + f + 1;
    u64 w   = internal_hash_bswap_64( ( u + v ) * mul ) + h;
    u64 x   = internal_hash_rotate( e + f, 42 ) + c;
    u64 y   = ( internal_hash_bswap_64( ( v + w ) * mul ) + g ) * mul;
    u64 z   = e + f + c;
    a       = internal_hash_bswap_64( ( x + z ) * mul + y ) + b;
    b       = internal_hash_shift_mix( ( z + a ) * mul + d + h ) * mul;
    return b + x;
}
attr_core_api hash64 hash_city_64( usize len, const void* _buf ) {
    const char* buf = _buf;
    if( len <= 32 ) {
        if( len <= 16 ) {
            return internal_hash_len_0_to_16( len, buf );
        } else {
            return internal_hash_len_17_to_32( len, buf );
        }
    } else if( len <= 64 ) {
        return internal_hash_len_33_to_64( len, buf );
    }

    u64 x = internal_hash_fetch_64( buf + len - 40 );
    u64 y = internal_hash_fetch_64(
        buf + len - 16 ) + internal_hash_fetch_64( buf + len - 56 );
    u64 z = internal_hash_len_16_0(
        internal_hash_fetch_64( buf + len - 48 ) + len,
        internal_hash_fetch_64( buf + len - 24));

    struct InternalHashPair v =
        internal_hash_weak_hash_len_32_with_seeds_1( buf + len - 64, len, z );
    struct InternalHashPair w =
        internal_hash_weak_hash_len_32_with_seeds_1( buf + len - 32, y + K1, x );

    x = x * K1 + internal_hash_fetch_64( buf );

    len = (len - 1) & ~((usize)63);
    do {
        x = internal_hash_rotate(
            x + y + v.x + internal_hash_fetch_64( buf + 8), 37 ) * K1;
        y = internal_hash_rotate(
            y + v.y + internal_hash_fetch_64( buf + 48 ), 42 ) * K1;
        x ^= w.y;
        y += v.x + internal_hash_fetch_64( buf + 40 );
        z = internal_hash_rotate( z + w.x, 33 ) * K1;
        v = internal_hash_weak_hash_len_32_with_seeds_1( buf, v.y * K1, x + w.x );
        w = internal_hash_weak_hash_len_32_with_seeds_1(
            buf + 32, z + w.y, y + internal_hash_fetch_64( buf + 16 ) );

        {
            u64 tmp = z;
            z = x;
            x = tmp;
        }

        buf += 64;
        len -= 64;
    } while( len );

    return internal_hash_len_16_0(
        internal_hash_len_16_0( v.x, w.x ) + internal_hash_shift_mix(y) * K1 + z,
        internal_hash_len_16_0( v.y, w.y ) + x );
}

#undef K0
#undef K1
#undef K2
#undef KMUL

