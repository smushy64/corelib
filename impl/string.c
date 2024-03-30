/**
 * Description:  Implementation of corelib String functions.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: January 31, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/macros.h"
#include "core/slice.h"
#include "core/string.h"
#include "core/memory.h"
#include "core/fmt.h"
#include "core/sync.h"

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    #include "core/internal/sse.h" // IWYU pragma: keep
#endif

#include "core/math.h"

attr_core_api usize asciiz_len( const char* ascii ) {
    if( !ascii ) {
        return 0;
    }
    usize res = 0;
    const char* str = ascii;
    while( *str++ ) {
        res++;
    }
    return res;
}
attr_core_api u64 text_buffer_hash( usize len, const char* text ) {
    // NOTE(alicia): elf-hash implementation
    // TODO(alicia): SIMD?

    unsigned char* ustr = (unsigned char*)text;

    u64 x = 0, result = 0;
    for( usize i = 0; i < len; ++i ) {
        result = ( result << 4 ) + ustr[i];
        x = result & 0xF000000000000000;
        if( x ) {
            result ^= x >> 24;
        }
        result &= ~x;
    }

    return result;
}

attr_core_api b32 string_cmp( const String a, const String b ) {
    if( a.len != b.len ) {
        return false;
    }
    return memory_cmp( a.v, b.v, a.len );
}
attr_internal b32 internal_string_find_scalar(
    const String str, const char c, usize* opt_out_pos
) {
    for( usize i = 0; i < str.len; ++i ) {
        if( str.cc[i] == c ) {
            if( opt_out_pos ) {
                *opt_out_pos = i;
            }
            return true;
        }
    }
    return false;
}
attr_internal b32 internal_string_find_last_scalar(
    const String str, const char c, usize* opt_out_pos
) {
    for( usize i = str.len; i-- > 0; ) {
        if( str.cc[i] == c ) {
            if( opt_out_pos ) {
                *opt_out_pos = i;
            }
            return true;
        }
    }
    return false;
}
attr_internal b32 internal_string_find_whitespace_scalar(
    const String str, usize* opt_out_pos
) {
    for( usize i = 0; i < str.len; ++i ) {
        if( char_is_whitespace( str.c[i] ) ) {
            if( opt_out_pos ) {
                *opt_out_pos = i;
            }
            return true;
        }
    }
    return false;
}
attr_internal void internal_string_to_upper_scalar( String str ) {
    for( usize i = 0; i < str.len; ++i ) {
        str.c[i] = char_to_upper( str.cc[i] );
    }
}
attr_internal void internal_string_to_lower_scalar( String str ) {
    for( usize i = 0; i < str.len; ++i ) {
        str.c[i] = char_to_lower( str.cc[i] );
    }
}
attr_internal usize internal_string_stream_to_upper_scalar(
    StreamBytesFN* stream, void* target, const String str
) {
    char stream_buf[16];

    usize res = 0;
    usize wide_count = str.len / 16;
    usize rem        = str.len % 16;

    for( usize i = 0; i < wide_count; ++i ) {
        const char* src = str.cc + (i * 16);
        for( usize j = 0; j < 16; ++j ) {
            stream_buf[j] = char_to_upper( src[j] );
        }
        res += stream( target, 16, stream_buf );
    }
    if( !rem ) {
        return res;
    }

    const char* src = str.cc + (wide_count * 16);

    usize stream_len = 0;
    for( usize i = 0; i < rem; ++i ) {
        stream_buf[stream_len++] = char_to_upper( src[i] );
    }
    res += stream( target, stream_len, stream_buf );
    return res;
}
attr_internal usize internal_string_stream_to_lower_scalar(
    StreamBytesFN* stream, void* target, const String str
) {
    char stream_buf[16];

    usize res = 0;
    usize wide_count = str.len / 16;
    usize rem        = str.len % 16;

    for( usize i = 0; i < wide_count; ++i ) {
        const char* src = str.cc + (i * 16);
        for( usize j = 0; j < 16; ++j ) {
            stream_buf[j] = char_to_lower( src[j] );
        }
        res += stream( target, 16, stream_buf );
    }
    if( !rem ) {
        return res;
    }

    const char* src = str.cc + (wide_count * 16);

    usize stream_len = 0;
    for( usize i = 0; i < rem; ++i ) {
        stream_buf[stream_len++] = char_to_lower( src[i] );
    }
    res += stream( target, stream_len, stream_buf );
    return res;
}

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)

#define CORE_ARCH_SSE_MIN_SEARCH_LEN (16 * 8)

#define COMP_a           _mm_set1_epi8( 'a' - 1 )
#define COMP_z           _mm_set1_epi8( 'z' + 1 )
#define COMP_A           _mm_set1_epi8( 'A' - 1 )
#define COMP_Z           _mm_set1_epi8( 'Z' + 1 )
#define UPPER_LOWER_DIFF _mm_set1_epi8( 'a' - 'A' )

attr_unused
attr_internal void internal_string_to_upper_sse( String str ) {
    if( !str.len ) {
        return;
    }
    if( str.len < CORE_ARCH_SSE_MIN_SEARCH_LEN ) {
        internal_string_to_upper_scalar( str );
        return;
    }

    const __m128i a    = COMP_a;
    const __m128i z    = COMP_z;
    const __m128i diff = UPPER_LOWER_DIFF;

    __m128i* aligned = (__m128i*)memory_align( str.cc, 16 );
    usize to_aligned = (const char*)aligned - str.cc;

    internal_string_to_upper_scalar( string_new( str.cc, to_aligned ) );
    usize aligned_len = (str.len - to_aligned) / sizeof(__m128i);
    for( usize i = 0; i < aligned_len; ++i ) {
        __m128i loaded = _mm_load_si128( aligned );

        __m128i gt = _mm_cmpgt_epi8( loaded, a );
        __m128i lt = _mm_cmplt_epi8( loaded, z );

        __m128i mask = _mm_and_si128( gt, lt );

        __m128i to_sub = _mm_and_si128( mask, diff );

        __m128i res = _mm_sub_epi8( loaded, to_sub );
        _mm_store_si128( aligned++, res );
    }

    String rem;
    rem.cc  = (const char*)(aligned - 1);
    rem.len = str.len - (rem.cc - str.cc);
    internal_string_to_upper_scalar( rem );
}
attr_unused
attr_internal usize internal_string_stream_to_upper_sse(
    StreamBytesFN* stream, void* target, const String str
) {
    if( !str.len ) {
        return 0;
    }
    if( str.len < CORE_ARCH_SSE_MIN_SEARCH_LEN ) {
        return internal_string_stream_to_upper_scalar( stream, target, str );
    }

    usize res = 0;
    const __m128i a    = COMP_a;
    const __m128i z    = COMP_z;
    const __m128i diff = UPPER_LOWER_DIFF;

    __m128i* aligned = (__m128i*)memory_align( str.cc, 16 );
    usize to_aligned = (const char*)aligned - str.cc;

    res += internal_string_stream_to_upper_scalar(
        stream, target, string_new( str.cc, to_aligned ) );
    usize offset  = to_aligned;

    usize aligned_len = (str.len - to_aligned) / sizeof(__m128i);
    attr_align(16) char buf[16];
    for( usize i = 0; i < aligned_len; ++i ) {
        __m128i loaded = _mm_load_si128( aligned++ );

        __m128i gt = _mm_cmpgt_epi8( loaded, a );
        __m128i lt = _mm_cmplt_epi8( loaded, z );

        __m128i mask = _mm_and_si128( gt, lt );

        __m128i to_sub = _mm_and_si128( mask, diff );

        __m128i sub_res = _mm_sub_epi8( loaded, to_sub );

        _mm_store_si128( (__m128i*)buf, sub_res );

        res += stream( target, sizeof(buf), buf );
        offset += sizeof(buf);
    }

    String rem;
    rem.cc  = str.cc + offset;
    rem.len = str.len - (usize)(rem.cc - str.cc);
    res += internal_string_stream_to_upper_scalar( stream, target, rem );
    return res;
}
attr_unused
attr_internal void internal_string_to_lower_sse( String str ) {
    if( !str.len ) {
        return;
    }
    if( str.len < CORE_ARCH_SSE_MIN_SEARCH_LEN ) {
        internal_string_to_lower_scalar( str );
        return;
    }

    const __m128i a    = COMP_A;
    const __m128i z    = COMP_Z;
    const __m128i diff = UPPER_LOWER_DIFF;

    __m128i* aligned = (__m128i*)memory_align( str.cc, 16 );
    usize to_aligned = (const char*)aligned - str.cc;

    internal_string_to_lower_scalar( string_new( str.cc, to_aligned ) );
    usize aligned_len = (str.len - to_aligned) / sizeof(__m128i);
    for( usize i = 0; i < aligned_len; ++i ) {
        __m128i loaded = _mm_load_si128( aligned );

        __m128i gt = _mm_cmpgt_epi8( loaded, a );
        __m128i lt = _mm_cmplt_epi8( loaded, z );

        __m128i mask = _mm_and_si128( gt, lt );

        __m128i to_add = _mm_and_si128( mask, diff );

        __m128i res = _mm_add_epi8( loaded, to_add );
        _mm_store_si128( aligned++, res );
    }

    String rem;
    rem.cc  = (const char*)(aligned - 1);
    rem.len = str.len - (rem.cc - str.cc);
    internal_string_to_lower_scalar( rem );
}
attr_unused
attr_internal usize internal_string_stream_to_lower_sse(
    StreamBytesFN* stream, void* target, const String str
) {
    if( !str.len ) {
        return 0;
    }
    if( str.len < CORE_ARCH_SSE_MIN_SEARCH_LEN ) {
        return internal_string_stream_to_lower_scalar( stream, target, str );
    }

    usize res = 0;
    const __m128i a    = COMP_A;
    const __m128i z    = COMP_Z;
    const __m128i diff = UPPER_LOWER_DIFF;

    __m128i* aligned = (__m128i*)memory_align( str.cc, 16 );
    usize to_aligned = (const char*)aligned - str.cc;

    res += internal_string_stream_to_lower_scalar(
        stream, target, string_new( str.cc, to_aligned ) );
    usize offset  = to_aligned;

    usize aligned_len = (str.len - to_aligned) / sizeof(__m128i);
    attr_align(16) char buf[16];
    for( usize i = 0; i < aligned_len; ++i ) {
        __m128i loaded = _mm_load_si128( aligned++ );

        __m128i gt = _mm_cmpgt_epi8( loaded, a );
        __m128i lt = _mm_cmplt_epi8( loaded, z );

        __m128i mask = _mm_and_si128( gt, lt );

        __m128i to_add = _mm_and_si128( mask, diff );

        __m128i add_res = _mm_add_epi8( loaded, to_add );

        _mm_store_si128( (__m128i*)buf, add_res );

        res += stream( target, sizeof(buf), buf );
        offset += sizeof(buf);
    }

    String rem;
    rem.cc  = str.cc + offset;
    rem.len = str.len - (usize)(rem.cc - str.cc);
    res += internal_string_stream_to_lower_scalar( stream, target, rem );
    return res;
}
attr_unused
attr_internal b32 internal_string_find_sse(
    const String str, const char c, usize* opt_out_pos
) {
    if( !str.len ) {
        return false;
    }
    if( str.len < CORE_ARCH_SSE_MIN_SEARCH_LEN ) {
        return internal_string_find_scalar( str, c, opt_out_pos );
    }

    const char* aligned = (const char*)memory_align( str.cc, 16 );

    usize aligned_rem_len = aligned - str.cc;
    if( aligned_rem_len ) {
        return internal_string_find_scalar( str, c, opt_out_pos );
    }

    usize result = 0;
    const char* at = str.cc;
    while( at != aligned ) {
        if( *at++ == c ) {
            if( opt_out_pos ) {
                *opt_out_pos = result;
            }
            return true;
        }
        result++;
    }

    __m128i* current   = (__m128i*)aligned;
    __m128i  wide_char = _mm_set1_epi8( c );

    loop() {
        __m128i loaded = _mm_load_si128( current );
        __m128i cmp    = _mm_cmpeq_epi8( loaded, wide_char );

        u16 mask = _mm_movemask_epi8( cmp );

        if( mask ) {
            u16 offset = 0;
            if( !(mask & 0xFF) && (mask & (0xFF << 8)) ) {
                result += 8;
                offset  = 8;
            }

            for( u16 i = 0; i < 8; ++i ) {
                if( ( mask >> (i + offset) ) & 0x1 ) {
                    break;
                }
                result++;
            }

            if( result >= str.len ) {
                return false;
            }

            if( opt_out_pos ) {
                *opt_out_pos = result;
            }
            return true;
        }

        current++;
        result += 16;
        if( result >= str.len ) {
            return false;
        }
    }
}
attr_unused
attr_internal b32 internal_string_find_whitespace_sse(
    const String str, usize* opt_out_pos
) {
    if( !str.len ) {
        return false;
    }
    if( str.len < CORE_ARCH_SSE_MIN_SEARCH_LEN ) {
        return internal_string_find_whitespace_scalar( str, opt_out_pos );
    }

    const char* aligned = (const char*)memory_align( str.cc, 16 );

    usize aligned_rem_len = aligned - str.cc;
    if( aligned_rem_len ) {
        return internal_string_find_whitespace_scalar( str, opt_out_pos );
    }

    usize result = 0;
    const char* at = str.cc;
    while( at != aligned ) {
        if( char_is_whitespace( *at++ ) ) {
            if( opt_out_pos ) {
                *opt_out_pos = result;
            }
            return true;
        }
        result++;
    }

    __m128i* current          = (__m128i*)aligned;
    __m128i  wide_whitespace0 = _mm_set1_epi8( 0x20 );
    __m128i  wide_whitespace1 = _mm_set1_epi8( 0x09 );
    __m128i  wide_whitespace2 = _mm_set1_epi8( 0x0D );
    __m128i  wide_whitespace3 = _mm_set1_epi8( 0x0A );

    loop() {
        __m128i loaded = _mm_load_si128( current );
        __m128i cmp0   = _mm_cmpeq_epi8( loaded, wide_whitespace0 );
        __m128i cmp1   = _mm_cmpeq_epi8( loaded, wide_whitespace1 );
        __m128i cmp2   = _mm_cmpeq_epi8( loaded, wide_whitespace2 );
        __m128i cmp3   = _mm_cmpeq_epi8( loaded, wide_whitespace3 );

        u16 mask = _mm_movemask_epi8( cmp0 );
        mask    |= _mm_movemask_epi8( cmp1 );
        mask    |= _mm_movemask_epi8( cmp2 );
        mask    |= _mm_movemask_epi8( cmp3 );

        if( mask ) {
            u16 offset = 0;
            if( !(mask & 0xFF) && (mask & (0xFF << 8)) ) {
                result += 8;
                offset  = 8;
            }

            for( u16 i = 0; i < 8; ++i ) {
                if( ( mask >> (i + offset) ) & 0x1 ) {
                    break;
                }
                result++;
            }

            if( result >= str.len ) {
                return false;
            }

            if( opt_out_pos ) {
                *opt_out_pos = result;
            }
            return true;
        }

        current++;
        result += 16;
        if( result >= str.len ) {
            return false;
        }
    }
}
attr_unused
attr_internal b32 internal_string_find_last_sse(
    const String str, const char c, usize* opt_out_pos
) {
    if( !str.len ) {
        return false;
    }
    if( str.len < CORE_ARCH_SSE_MIN_SEARCH_LEN ) {
        return internal_string_find_last_scalar( str, c, opt_out_pos );
    }

    usize result     = str.len - 1;
    const char* last = str.cc + result;

    const char* aligned = (const char*)memory_align( last, 16 );
    while( aligned > last ) {
        aligned -= 16;
    }

    if( aligned < str.cc ) {
        return internal_string_find_last_scalar( str, c, opt_out_pos );
    }

    if( last - aligned ) {
        String aligned_to_last;
        aligned_to_last.len = last - aligned;
        aligned_to_last.cc  = aligned;
        if( internal_string_find_last_scalar( aligned_to_last, c, opt_out_pos ) ) {
            return true;
        }
    }

    __m128i* current   = (__m128i*)aligned;
    __m128i  wide_char = _mm_set1_epi8( c );
    loop() {
        __m128i loaded = _mm_load_si128( current );
        __m128i cmp    = _mm_cmpeq_epi8( loaded, wide_char );

        u16 mask = _mm_movemask_epi8( cmp );

        if( mask ) {
            u16 offset = 0;
            if( !(mask & 0xFF) && (mask & (0xFF << 8)) ) {
                usize prev_result = result;
                result -= 8;
                if( result > prev_result ) {
                    return false;
                }
                offset  = 8;
            }

            for( u16 i = 8; i-- > 0; ) {
                if( ( mask >> (i + offset) ) & 0x1 ) {
                    break;
                }
                if( !result ) {
                    return false;
                }
                result--;
            }

            if( opt_out_pos ) {
                *opt_out_pos = result;
            }
            return true;
        }

        current--;
        if( result < 16 ) {
            break;
        }
        result -= 16;
        if( !result ) {
            break;
        }
    }

    if( result ) {
        return internal_string_find_last_scalar( str, c, opt_out_pos );
    } else {
        return false;
    }
}

#undef CORE_ARCH_SSE_MIN_SEARCH_LEN
#endif

attr_core_api b32 string_find(
    const String str, const char c, usize* opt_out_pos
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_string_find_sse( str, c, opt_out_pos );
#else // no SIMD at all
    return internal_string_find_scalar( str, c, opt_out_pos );
#endif
}
attr_core_api b32 string_find_last(
    const String str, const char c, usize* opt_out_pos
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_string_find_last_sse( str, c, opt_out_pos );
#else // no SIMD at all
    return internal_string_find_last_scalar( str, c, opt_out_pos );
#endif
}

attr_core_api b32 string_find_whitespace( const String str, usize* opt_out_pos ) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_string_find_whitespace_sse( str, opt_out_pos );
#else /* no SIMD */
    return internal_string_find_whitespace_scalar( str, opt_out_pos );
#endif
}
// TODO(alicia): dedicated SIMD?
attr_core_api b32 string_find_last_whitespace( const String str, usize* opt_out_pos ) {
    for( usize i = str.len; i-- > 0; ) {
        if( !char_is_whitespace( str.cc[i] ) ) {
            if( opt_out_pos ) {
                *opt_out_pos = i;
            }
            return true;
        }
    }
    return false;
}
attr_core_api usize string_count( const String str, const char c ) {
    usize result = 0;
    String substring = str;
    while( substring.len ) {
        usize move = 0;
        if( string_find( substring, c, &move ) ) {
            result++;
            move += 1;
            if( move > substring.len ) {
                return result;
            }
            substring.cc  += move;
            substring.len -= move;
        } else {
            return result;
        }
    }
    return result;
}
attr_core_api b32 string_find_phrase(
    const String str, const String phrase, usize* opt_out_pos
) {
    if( phrase.len > str.len ) {
        return false;
    }

    String substring = str;
    usize start = 0;

    while( substring.len ) {
        if( string_find( substring, phrase.cc[0], &start ) ) {
            substring = string_advance( substring, start );
            if( substring.len < phrase.len ) {
                break;
            }

            String potential_phrase = substring;
            potential_phrase.len    = phrase.len;

            if( string_cmp( potential_phrase, phrase ) ) {
                if( opt_out_pos ) {
                    *opt_out_pos = potential_phrase.cc - str.cc;
                }
                return true;
            }

            substring = string_advance( substring, 1 );
        } else {
            break;
        }
    }

    return false;
}
attr_core_api b32 string_find_phrase_last(
    const String str, const String phrase, usize* opt_out_pos
) {
    if( phrase.len > str.len ) {
        return false;
    }

    String substring = str;
    usize  start     = 0;

    while( substring.len ) {
        String potential_phrase;
        potential_phrase.cc  = 0;
        potential_phrase.len = 0;
        if( string_find_last( substring, phrase.cc[0], &start ) ) {
            potential_phrase.cc  = substring.cc + start;
            potential_phrase.len = phrase.len;

            usize remaining_len = str.len - start;
            if( remaining_len < phrase.len ) {
                substring.len -= remaining_len;
                if( !substring.len ) {
                    return false;
                }
                substring.len--;
                continue;
            }

            substring.len -= start;
            if( string_cmp( potential_phrase, phrase ) ) {
                if( opt_out_pos ) {
                    *opt_out_pos = start;
                }
                return true;
            }

            continue;
        }

        break;
    }

    return false;
}
// NOTE(alicia): potential optimizations?
attr_core_api void string_reverse( String str ) {
    char* begin = str.c;
    char* end   = begin + str.len - 1;

    while( begin != end ) {
        char temp = *end;
        *end--   = *begin;
        b32 should_end = begin == end;
        *begin++ = temp;

        if( should_end ) {
            return;
        }
    }
}
attr_core_api String string_trim_leading_whitespace( const String src ) {
    for( usize i = 0; i < src.len; ++i ) {
        if( !char_is_whitespace( src.cc[i] ) ) {
            String res;
            res.cc  = src.cc  + i;
            res.len = src.len - i;
            return res;
        }
    }

    return src;
}
attr_core_api String string_trim_trailing_whitespace( const String src ) {
    for( usize i = src.len; i-- > 0; ) {
        if( !char_is_whitespace( src.cc[i] ) ) {
            String res;
            res.cc  = src.cc;
            res.len = (i + 1) > src.len ? src.len : (i + 1);
            return res;
        }
    }
    return src;
}
attr_core_api void string_set( String str, const char c ) {
    memory_set( str.c, c, str.len );
}
attr_core_api void string_to_upper( String str ) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    internal_string_to_upper_sse( str );
#else
    internal_string_to_upper_scalar( str );
#endif
}
attr_core_api void string_to_lower( String str ) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    internal_string_to_lower_sse( str );
#else
    internal_string_to_lower_scalar( str );
#endif
}
attr_core_api usize string_stream_to_upper(
    StreamBytesFN* stream, void* target, const String str
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_string_stream_to_upper_sse( stream, target, str );
#else
    return internal_string_stream_to_upper_scalar( stream, target, str );
#endif
}
attr_core_api usize string_stream_to_lower(
    StreamBytesFN* stream, void* target, const String str
) {
#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    return internal_string_stream_to_lower_sse( stream, target, str );
#else
    return internal_string_stream_to_lower_scalar( stream, target, str );
#endif
}
attr_always_inline inline
attr_internal b32 internal_string_read_increment( usize* value, usize max ) {
    usize increment_result = *value + 1;
    if( increment_result >= max ) {
        return false;
    }
    *value = increment_result;
    return true;
}
attr_core_api b32 string_parse_int( const String str, i64* out_int ) {
    if( !str.len ) {
        return false;
    }

    b32 is_negative = false;
    i64 result = 0;

    usize index = 0;
    if( str.cc[index] == '-' ) {
        if( !internal_string_read_increment( &index, str.len ) ) {
            return false;
        }
        is_negative = true;
    }

    do {
        if( !char_is_number( str.cc[index] ) ) {
            if( !index ) {
                return false;
            }
            break;

        }
        result *= 10;
        result += str.cc[index] - '0';
    } while( internal_string_read_increment( &index, str.len ) );

    *out_int = result * ( is_negative ? -1 : 1 );
    return true;

}
attr_core_api b32 string_parse_uint( const String str, u64* out_uint ) {
    if( !str.len ) {
        return false;
    }

    u64 result = 0;
    usize index = 0;

    do {
        if( !char_is_number( str.cc[index] ) ) {
            if( !index ) {
                return false;
            }
            break;

        }
        result *= 10;
        result += str.cc[index] - '0';
    } while( internal_string_read_increment( &index, str.len ) );

    *out_uint = result;
    return true;

}
attr_optimized
attr_internal attr_always_inline inline
u64 internal_string_places( u64 i ) {
    if( i < 10 ) {
        return 1;
    } else if( i < 100 ) {
        return 2;
    } else if( i < 1000 ) {
        return 3;
    } else if( i < 10000 ) {
        return 4;
    } else if( i < 100000 ) {
        return 5;
    } else if( i < 1000000 ) {
        return 6;
    } else if( i < 10000000 ) {
        return 7;
    } else if( i < 100000000 ) {
        return 8;
    } else if( i < 1000000000 ) {
        return 9;
    } else if( i < 10000000000 ) {
        return 10;
    } else if( i < 100000000000 ) {
        return 11;
    } else if( i < 1000000000000 ) {
        return 12;
    } else if( i < 10000000000000 ) {
        return 13;
    } else if( i < 100000000000000 ) {
        return 14;
    } else if( i < 1000000000000000 ) {
        return 15;
    } else if( i < 10000000000000000 ) {
        return 16;
    } else if( i < 100000000000000000 ) {
        return 17;
    } else if( i < 1000000000000000000 ) {
        return 18;
    }

    return 0;
}
attr_core_api b32 string_parse_float( const String str, f64* out_float ) {
    // f64 result = 0.0;
    i64 whole_part      = 0;
    u64 fractional_part = 0;

    usize dot_position = 0;
    if(
        string_find( str, '.', &dot_position ) &&
        dot_position + 1 < str.len
    ) {
        String first = {0}, last = {0};
        string_split( str, dot_position, &first, &last );
        if( !string_parse_int( first, &whole_part ) ) {
            return false;
        }

        usize zero_count = 0;
        for( usize i = 0; i < last.len; ++i ) {
            if( last.c[i] == '0' ) {
                zero_count++;
            }
        }

        last.c += zero_count;
        last.len    -= zero_count;

        if( last.len ) {
            if( !string_parse_uint( last, &fractional_part ) ) {
                return false;
            }
        }

        *out_float = (f64)whole_part;
        f64 fractional_part_f64 = (f64)fractional_part;
        u64 places = internal_string_places( fractional_part );

        u64 pow = places + zero_count;
        if( pow ) {
            fractional_part_f64 /= (f64)poweri( 10.0f, pow );
        }

        *out_float += fractional_part_f64;

        return true;
    } else {
        b32 success = string_parse_int( str, &whole_part );
        if( success ) {
            *out_float = (f64)whole_part;
            return success;
        } else {
            return success;
        }
    }
}

attr_core_api b32 string_buf_new_alloc(
    const usize size, const char* opt_copy_buffer, StringBuf* out_buf
) {
    StringBuf result;
    result.c   = nullptr;
    result.len = 0;
    result.cap = 0;

    usize allocated_size = size;

    if( opt_copy_buffer ) {
        allocated_size += string_buf_query_realloc_size();
    }

    result.cap = allocated_size;
    result.v   = memory_alloc( allocated_size );
    if( !result.v ) {
        return false;
    }

    if( opt_copy_buffer ) {
        memory_copy( result.v, opt_copy_buffer, size );
        result.len = size;
    }

    *out_buf = result;
    return true;
}
attr_core_api b32 string_buf_realloc( StringBuf* buf, const usize new_size ) {
    if( new_size < buf->cap ) {
        return true;
    }
    void* new_buffer = memory_realloc( buf->v, buf->cap, new_size );
    if( !new_buffer ) {
        return false;
    }

    buf->v   = new_buffer;
    buf->cap = new_size;
    return true;
}

#if !defined(CORE_DEFAULT_STRING_REALLOC_SIZE)
    #define CORE_DEFAULT_STRING_REALLOC_SIZE (16)
#endif

attr_global usize global_string_buf_realloc_size =
    CORE_DEFAULT_STRING_REALLOC_SIZE < 16 ? 16 : CORE_DEFAULT_STRING_REALLOC_SIZE;
attr_core_api usize string_buf_query_realloc_size(void) {
    return global_string_buf_realloc_size;
}
attr_core_api usize string_buf_query_default_realloc_size(void) {
    return CORE_DEFAULT_STRING_REALLOC_SIZE < 16 ?
        16 : CORE_DEFAULT_STRING_REALLOC_SIZE;
}
attr_core_api void string_buf_set_default_realloc_size( const usize size ) {
    usize new_size = size;
    if( new_size < 16 ) {
        new_size = 16;
    }
    interlocked_exchange( (volatile long*)&global_string_buf_realloc_size, new_size );
}
attr_core_api void string_buf_free( StringBuf* buf ) {
    memory_free( buf->v, buf->cap );
    memory_zero( buf, sizeof(*buf) );
}
attr_core_api void string_buf_set(
    StringBuf* buf, const char c, const usize opt_max
) {
    usize max = buf->cap;
    if( opt_max && opt_max < max ) {
        max = opt_max;
    }

    memory_set( buf->v, c, max );
}
attr_core_api usize string_buf_copy( StringBuf* dst, const String src ) {
    usize max_copy = src.len;
    usize rem      = dst->cap - dst->len;
    max_copy       = max_copy > rem ? rem : max_copy;

    memory_copy( dst->c + dst->len, src.v, max_copy );
    dst->len += max_copy;
    return max_copy;
}
attr_core_api b32 string_buf_prepend( StringBuf* dst, const String src ) {
    if( src.len + dst->len > dst->cap ) {
        return false;
    }

    memory_copy_overlapped( dst->c + src.len, dst->c, dst->len );
    memory_copy( dst->c, src.c, src.len );
    dst->len += src.len;

    return true;
}
attr_core_api b32 string_buf_append( StringBuf* dst, const String src ) {
    if( dst->len + src.len > dst->cap ) {
        return false;
    }

    memory_copy( dst->c + dst->len, src.v, src.len );
    dst->len += src.len;
    return true;
}
attr_core_api b32 string_buf_insert(
    StringBuf* dst, const String src, const usize index
) {
    if( (dst->len + src.len) > dst->cap ) {
        return false;
    }
    if( !dst->len || index == (dst->len - 1) ) {
        return string_buf_append( dst, src );
    }

    usize move_src = index + 1;
    usize move_dst = move_src + src.len;
    usize move_len = dst->len - move_src;
    memory_copy_overlapped( dst->c + move_dst, dst->c + move_src, move_len );
    memory_copy( dst->c + move_src, src.c, src.len );
    dst->len += src.len;

    return true;
}
attr_core_api usize string_buf_stream(
    void* in_dst, usize count, const void* src
) {
    StringBuf* dst = (StringBuf*)in_dst;

    usize max_copy = count;
    usize rem      = dst->cap - dst->len;
    max_copy       = max_copy > rem ? rem : max_copy;

    memory_copy( dst->c + dst->len, src, max_copy );
    dst->len += max_copy;
    return count - max_copy;
}
attr_core_api usize string_buf_fmt_text_buffer_va(
    StringBuf* dst, const usize format_len, const char* format, va_list va
) {
    return fmt_text_va( string_buf_stream, dst, format_len, format, va );
}

