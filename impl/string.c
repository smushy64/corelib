/**
 * @file   string.c
 * @brief  String function implementations.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 10, 2024
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/macros.h"
#include "core/slice.h"
#include "core/string.h"
#include "core/memory.h"
#include "core/fmt.h"
#include "core/alloc.h"

#if defined(CORE_ENABLE_SSE_INSTRUCTIONS)
    #include "core/internal/sse.h" // IWYU pragma: keep
#endif

#include "core/math.h"

#define VECTOR_MIN_LEN (256)

attr_core_api usize cstr_len( const cstr* c_string ) {
    if( !c_string ) {
        return 0;
    }
    const cstr* end = c_string;
    while( *(++end) ) {}
    return end - c_string;
}
attr_core_api usize cstr_len_utf8( const cstr* c_string ) {
    return string_len_utf8( string_from_cstr( c_string ) );
}
attr_core_api hash64 hash_64( usize len, const void* buf ) {
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
attr_core_api hash128 hash_128( usize len, const void* buf ) {
    // TODO(alicia): 
    unused( len, buf );
    unimplemented();
}

attr_core_api usize string_len_utf8( String str ) {
    usize res = 0;
    for( usize i = 0; i < str.len; ++i ) {
        if( (str.bytes[i] & 0xC0) != 0x80 ) {
            res++;
        }
    }
    return res;
}
attr_core_api rune32 string_index_utf8( String str, usize index ) {
    // TODO(alicia): 
    unused(str, index);
    unimplemented();
}
attr_core_api b32 string_cmp( String a, String b ) {
    if( a.len != b.len ) {
        return false;
    }
    if( !a.len && !b.len ) {
        return true;
    }
    return memory_cmp( a.v, b.v, a.len );
}

attr_core_api b32 string_find( String str, char c, usize* opt_out_index ) {
    for( usize i = 0; i < str.len; ++i ) {
        if( str.cc[i] == c ) {
            if( opt_out_index ) {
                *opt_out_index = i;
            }
            return true;
        }
    }
    return false;
}
attr_core_api usize string_find_count( String str, char c ) {
    String substr = str;
    usize  res    = 0;
    while( !string_is_empty( substr ) ) {
        usize idx = 0;
        if( string_find( substr, c, &idx ) ) {
            res++;
            substr = string_advance_by( substr, idx + 1 );
        } else {
            break;
        }
    }
    return res;
}
attr_core_api b32 string_find_rev( String str, char c, usize* opt_out_index ) {
    for( usize i = str.len; i-- > 0; ) {
        if( str.cc[i] == c ) {
            if( opt_out_index ) {
                *opt_out_index = i;
            }
            return true;
        }
    }
    return false;
}
attr_core_api b32 string_find_set( String str, String set, usize* opt_out_index ) {
    for( usize i = 0; i < str.len; ++i ) {
        for( usize j = 0; j < set.len; ++j ) {
            if( str.cc[i] == set.cc[j] ) {
                if( opt_out_index ) {
                    *opt_out_index = i;
                }
                return true;
            }
        }
    }
    return false;
}
attr_core_api b32 string_find_set_rev(
    String str, String set, usize* opt_out_index
) {
    for( usize i = str.len; i-- > 0; ) {
        for( usize j = 0; j < set.len; ++j ) {
            if( str.cc[i] == set.cc[j] ) {
                if( opt_out_index ) {
                    *opt_out_index = i;
                }
                return true;
            }
        }
    }
    return false;
}
attr_core_api usize string_find_set_count( String str, String set ) {
    String substr = str;
    usize  res    = 0;
    while( !string_is_empty( substr ) ) {
        usize idx = 0;
        if( string_find_set( substr, set, &idx ) ) {
            res++;
            substr = string_advance_by( substr, idx + 1 );
        } else {
            break;
        }
    }
    return res;
}
attr_core_api b32 string_find_phrase(
    String str, String phrase, usize* opt_out_index
) {
    if( str.len < phrase.len ) {
        return false;
    }
    String substr = str;
    while( !string_is_empty( substr ) ) {
        usize start = 0;
        if( string_find( substr, phrase.cc[0], &start ) ) {
            String cmp = substr;
            cmp.len    = phrase.len;
            if( string_cmp( cmp, phrase ) ) {
                if( opt_out_index ) {
                    *opt_out_index = cmp.cc - str.cc;
                }
                return true;
            }

            substr = string_advance( substr );
        } else {
            break;
        }

        if( substr.len < phrase.len ) {
            break;
        }
    }
    return false;
}
attr_core_api b32 string_find_phrase_rev(
    String str, String phrase, usize* opt_out_index
) {
    String substr = str;
    while( !string_is_empty( substr ) ) {
        if( substr.len < phrase.len ) {
            break;
        }
        usize start = 0;
        if( string_find_rev( substr, phrase.cc[0], &start ) ) {
            String potential = string_advance_by( substr, start );
            if( potential.len < phrase.len ) {
                substr = string_truncate( substr, start );
                continue;
            }
            potential.len = phrase.len;
            if( string_cmp( potential, phrase ) ) {
                if( opt_out_index ) {
                    *opt_out_index = potential.cc - str.cc;
                }
                return true;
            }
            substr = string_truncate( substr, start );
        } else {
            break;
        }
    }
    return false;
}
attr_core_api usize string_find_phrase_count( String str, String phrase ) {
    String substr = str;
    usize  res    = 0;
    while( !string_is_empty( substr ) ) {
        usize idx = 0;
        if( string_find_phrase( substr, phrase, &idx ) ) {
            res++;
            substr = string_advance_by( substr, idx + phrase.len );
        } else {
            break;
        }
    }
    return res;
}
attr_core_api String string_trim_leading_whitespace( String str ) {
    String res = str;
    for( usize i = 0; i < res.len; ++i ) {
        if( !ascii_is_whitespace( res.cc[i] ) ) {
            res = string_advance_by( res, i );
            return res;
        }
    }
    return res;
}
attr_core_api String string_trim_trailing_whitespace( String str ) {
    String res = str;
    for( usize i = res.len; i-- > 0; ) {
        if( !ascii_is_whitespace( res.cc[i] ) ) {
            res = string_truncate( res, i + 1 );
            return res;
        }
    }
    return res;
}
attr_core_api void string_mut_reverse( String str ) {
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
attr_core_api void string_mut_set( String str, char c ) {
    memory_set( str.v, rcast( u8, &c ), str.len );
}
attr_core_api void string_mut_to_upper( String str ) {
    for( usize i = 0; i < str.len; ++i ) {
        str.c[i] = ascii_to_upper( str.c[i] );
    }
}
attr_core_api void string_mut_to_lower( String str ) {
    for( usize i = 0; i < str.len; ++i ) {
        str.c[i] = ascii_to_lower( str.c[i] );
    }
}
attr_core_api usize string_stream_to_upper(
    StreamBytesFN* stream, void* target, String str
) {
    usize res = 0;
    for( usize i = 0; i < str.len; ++i ) {
        char c = ascii_to_upper( str.cc[i] );
        res += stream( target, 1, &c );
    }
    return res;
}
attr_core_api usize string_stream_to_lower(
    StreamBytesFN* stream, void* target, String str
) {
    usize res = 0;
    for( usize i = 0; i < str.len; ++i ) {
        char c = ascii_to_lower( str.cc[i] );
        res += stream( target, 1, &c );
    }
    return res;
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
attr_core_api b32 string_parse_int( String str, i64* out_int ) {
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
        if( !ascii_is_numeric( str.cc[index] ) ) {
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
attr_core_api b32 string_parse_uint( String str, u64* out_uint ) {
    if( !str.len ) {
        return false;
    }

    u64 result = 0;
    usize index = 0;

    do {
        if( !ascii_is_numeric( str.cc[index] ) ) {
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
attr_core_api b32 string_parse_float( String str, f64* out_float ) {
    // f64 result = 0.0;
    i64 whole_part      = 0;
    u64 fractional_part = 0;

    usize dot_position = 0;
    if(
        string_find( str, '.', &dot_position ) &&
        dot_position + 1 < str.len
    ) {
        String first = string_empty(), last = string_empty();
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

        last.c   += zero_count;
        last.len -= zero_count;

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

        if( whole_part < 0 ) {
            *out_float -= fractional_part_f64;
        } else {
            *out_float += fractional_part_f64;
        }

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

attr_core_api b32 string_buf_from_alloc(
    usize size, struct AllocatorInterface* allocator, StringBuf* out_buf
) {
    // for null-terminator.
    usize _size = size + 1;
    void* ptr = allocator->alloc( _size, 0, allocator->ctx );
    if( !ptr ) {
        return false;
    }
    out_buf->cap = _size;
    out_buf->len = 0;
    out_buf->v   = ptr;
    return true;
}
attr_core_api b32 string_buf_from_string_alloc(
    String str, struct AllocatorInterface* allocator, StringBuf* out_buf
) {
    if( !string_buf_from_alloc( str.len + 16, allocator, out_buf ) ) {
        return false;
    }
    memory_copy( out_buf->v, str.cc, str.len );
    out_buf->len = str.len;
    return true;
}
attr_core_api b32 string_buf_grow(
    StringBuf* buf, usize amount, struct AllocatorInterface* allocator
) {
    void* ptr = allocator->realloc(
        buf->v, buf->cap, buf->cap + amount, 0, allocator->ctx );
    if( !ptr ) {
        return false;
    }
    buf->v    = ptr;
    buf->cap += amount;
    return true;
}
attr_core_api void string_buf_free(
    StringBuf* buf, struct AllocatorInterface* allocator
) {
    if( buf ) {
        allocator->free( buf->v, buf->cap, 0, allocator->ctx );
        memory_zero( buf, sizeof(*buf) );
    }
}
attr_core_api b32 string_buf_clone(
    StringBuf* dst, const StringBuf* src, struct AllocatorInterface* allocator
) {
    if( dst->cap != src->len + 1 ) {
        if( dst->v ) {
            usize diff = (src->len + 1) - dst->cap;
            if( !string_buf_grow( dst, diff, allocator ) ) {
                return false;
            }
        } else {
            if( !string_buf_from_alloc( src->len + 1, allocator, dst ) ) {
                return false;
            }
        }
    }

    memory_copy( dst->v, src->v, src->len );
    dst->len = src->len;
    return true;
}
attr_core_api b32 string_buf_try_push( StringBuf* buf, char c ) {
    if( string_buf_is_full( buf ) ) {
        return false;
    }
    buf->c[buf->len++] = c;
    buf->c[buf->len]   = 0;
    return true;
}
attr_core_api b32 string_buf_push(
    StringBuf* buf, char c, struct AllocatorInterface* allocator
) {
    if( string_buf_try_push( buf, c ) ) {
        return true;
    }
    if( !string_buf_grow( buf, 16, allocator ) ) {
        return false;
    }
    return string_buf_try_push( buf, c );
}
attr_core_api b32 string_buf_try_emplace( StringBuf* buf, char c, usize at ) {
    if( string_buf_is_full( buf ) ) {
        return false;
    }
    usize move = (buf->len - at) + 1;
    memory_move( buf->c + at + 1, buf->c + at, move );
    buf->c[at] = c;
    buf->len++;
    return true;
}
attr_core_api b32 string_buf_emplace(
    StringBuf* buf, char c, usize at, struct AllocatorInterface* allocator
) {
    if( string_buf_try_emplace( buf, c, at ) ) {
        return true;
    }
    if( !string_buf_grow( buf, 16, allocator ) ) {
        return false;
    }
    return string_buf_try_emplace( buf, c, at );
}
attr_core_api b32 string_buf_pop( StringBuf* buf, char* opt_out_c ) {
    if( string_buf_is_empty( buf ) ) {
        return false;
    }
    char c = buf->c[buf->len--];
    if( opt_out_c ) {
        *opt_out_c = c;
    }
    return true;
}
attr_core_api b32 string_buf_try_insert( StringBuf* buf, String insert, usize at ) {
    if( !buf->cap || ( (buf->len + insert.len) > (buf->cap - 1) ) ) {
        return false;
    }
    usize move = buf->len ? ((buf->len - at) + 1) : (0);
    memory_move( buf->c + at + insert.len, buf->c + at, move );
    memory_copy( buf->c + at, insert.v, insert.len );
    buf->len += insert.len;
    return true;
}
attr_core_api b32 string_buf_insert(
    StringBuf* buf, String insert, usize at, struct AllocatorInterface* allocator
) {
    if( string_buf_try_insert( buf, insert, at ) ) {
        return true;
    }
    if( !string_buf_grow( buf, insert.len + 16, allocator ) ) {
        return false;
    }
    return string_buf_try_insert( buf, insert, at );
}
attr_core_api void string_buf_remove( StringBuf* buf, usize at ) {
    if( string_buf_is_empty( buf ) ) {
        return;
    }
    usize move = (buf->len - at) + 1;
    memory_move( buf->c + at, buf->c + at + 1, move );
    buf->len--;
}
attr_core_api void string_buf_remove_range(
    StringBuf* buf, usize from_inclusive, usize to_exclusive
) {
    debug_assert(
        to_exclusive < from_inclusive,
        "invalid range provided! {usize} -> {usize}",
        from_inclusive, to_exclusive );
    debug_assert(
        from_inclusive >= buf->len || to_exclusive > buf->len,
        "range is out of bounds of string! len: {usize} {usize} -> {usize}",
        buf->len, from_inclusive, to_exclusive );

    usize span = to_exclusive - from_inclusive;

    memory_move(
        buf->c + from_inclusive,
        buf->c + to_exclusive, (buf->len + 1) - span );
    buf->len -= span;
}
attr_core_api usize string_buf_try_stream(
    void* target, usize count, const void* bytes
) {
    StringBuf* buf = target;
    usize max_copy = count;
    usize rem      = string_buf_remaining( buf );
    if( !rem ) {
        return count;
    }
    if( max_copy > rem ) {
        max_copy = rem;
    }

    memory_copy( buf->c + buf->len, bytes, max_copy );
    buf->len        += max_copy;
    buf->c[buf->len] = 0;

    return count - max_copy;
}
attr_core_api bsize string_buf_stream(
    void* target, usize count, const void* bytes
) {
    StringBufStreamTarget* t = target;
    String append = string_new( count, bytes );
    return string_buf_append( t->buf, append, t->allocator );
}
attr_core_api usize string_buf_try_fmt_buffer_va(
    StringBuf* buf, usize format_len, const char* format, va_list va
) {
    return stream_fmt_va( string_buf_try_stream, buf, format_len, format, va );
}
attr_core_api b32 string_buf_fmt_buffer_va(
    StringBuf* buf, struct AllocatorInterface* allocator,
    usize format_len, const char* format, va_list va
) {
    StringBufStreamTarget target;
    target.allocator = allocator;
    target.buf       = buf;

    usize res = stream_fmt_va( string_buf_stream, &target, format_len, format, va );
    return res ? true : false;
}

