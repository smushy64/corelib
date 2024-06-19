/**
 * Description:  Corelib tests.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: January 30, 2024
*/
#define CORE_ENABLE_DEBUG_TIMER_BLOCK
// IWYU pragma: begin_keep
#include "core/prelude.h"
#include "core/string.h"
#include "core/print.h"
#include "core/math.h"
#include "core/memory.h"
#include "core/alloc.h"
#include "core/collections.h"
#include "core/time.h"
// IWYU pragma: end_keep

#if defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

#define test( func )\
    do {\
        int res = func ();\
        if( res ) {\
            println( CONSOLE_COLOR_RED "test "#func" failed!" CONSOLE_COLOR_RESET );\
            return res;\
        } else {\
            println( "test "#func" completed successfully!" );\
        }\
    } while(0)\

#define expect( condition, message, ... ) do {\
    if( !(condition) ) {\
        println( CONSOLE_COLOR_RED __FILE__\
            ":{i}: condition '" #condition "' failed! "\
            message CONSOLE_COLOR_RESET, __LINE__, ##__VA_ARGS__ );\
        return -1;\
    }\
} while(0)

int string(void);
int collections(void);

int main( int argc, char** argv ) {
    unused( argc, argv );

    debug_timer_block( tests,
        test( collections );
        test( string );
    , "completed in " );

    return 0;
}

attr_optnone
int collections(void) {
    {
        #define CAPACITY 10
        AllocatorInterface heap = allocator_interface_from_heap();

        Hashmap map = hashmap_empty();
        expect( hashmap_from_alloc( sizeof(int), CAPACITY, &map, &heap ), "" );

        int item = 99;
        expect( hashmap_try_insert( &map, 10, &item ), "" );
        item = 80;
        expect( hashmap_try_insert( &map, 6, &item ), "" );
        item = 70;
        expect( hashmap_try_insert( &map, 255, &item ), "" );

        expect( hashmap_grow( &map, 2, &heap ), "" );

        int out = 0;
        expect( hashmap_index( &map, 10, &out ), "" );
        expect( out == 99, "{i32}", out );

        expect( hashmap_index( &map, 6, &out ), "" );
        expect( out == 80, "{i32}", out );

        expect( hashmap_index( &map, 255, &out ), "" );
        expect( out == 70, "{i32}", out );

        expect( hashmap_remove( &map, 6, &out ), "" );
        expect( out == 80, "{i32}", out );

        expect( hashmap_index( &map, 10, &out ), "" );
        expect( out == 99, "{i32}", out );

        expect( hashmap_index( &map, 255, &out ), "" );
        expect( out == 70, "{i32}", out );

        hashmap_free( &map, &heap );

        #undef CAPACITY
    }
    {
        char buf[hashmap_memory_requirement( sizeof(int), 10 )];
        memory_zero( buf, sizeof(buf) );

        Hashmap map = hashmap_new( sizeof(int), 10, buf );

        int item = 10;
        expect( hashmap_try_insert_text( &map, "hello, world!", &item ), "" );
        expect( hashmap_index_text( &map, "hello, world!", &item ), "" );
        expect( item == 10, "" );
    }
    {
        PackedBool packed[packed_bool_calculate_size( 10 )];
        memory_zero( packed, sizeof(packed) );
        expect( packed_bool_bytes_to_bits( static_array_len( packed ) ) == 16, "" );
        expect( packed_bool_set( packed, 0, true ), "" );
        expect( packed_bool_set( packed, 1, false ), "" );
        expect( packed_bool_index( packed, 0 ) == true, "" );
        expect( packed_bool_index( packed, 1 ) == false, "" );
        expect( packed_bool_index( packed, 9 ) == false, "" );
    }
    {
        #define CAPACITY 10
        int buf[CAPACITY];
        memory_zero( buf, sizeof(buf) );

        Queue queue = queue_new( sizeof(int), CAPACITY, buf );
        expect( queue_is_empty( &queue ), "" );

        #define enqueue( queue, item )\
            expect( queue_try_enqueue( queue, item ), "" )
        #define expect_dequeue( queue, item )\
            expect( queue_dequeue( queue, item ), "" )

        for( usize i = 0; i < CAPACITY; ++i ) {
            int item = (int)i + 10;
            enqueue( &queue, &item );
        }

        for( usize i = 0; i < 4; ++i ) {
            int item = 0;
            expect_dequeue( &queue, &item );
            expect( item == (int)i + 10, "expected item: {i32} got: {i32}", (int)i + 10, item );
        }

        #undef CAPACITY
        #undef enqueue
        #undef dequeue
    }
    {
        AllocatorInterface heap = allocator_interface_from_heap();
        Queue queue = queue_empty();
        queue_from_alloc( sizeof(int), 2, &queue, &heap );
        int item = 10;
        queue_try_enqueue( &queue, &item );
        queue_dequeue( &queue, 0 );
        queue_try_enqueue( &queue, &item );
        queue_grow( &queue, queue.cap, &heap );

        item = 99;

        queue_try_enqueue( &queue, &item );
        queue_try_enqueue( &queue, &item );

        queue_free( &queue, &heap );
    }
    {
        AllocatorInterface heap = allocator_interface_from_heap();

        darray(i32) list = darray_empty(i32);

        expect( darray_from_alloc( 10, &list, &heap ), "failed to alloc list!" );
        expect( list.cap == 10, "" );

        for( usize i = 0; i < darray_cap( &list ); ++i ) {
            i32 item = i;
            expect( darray_try_push( &list, &item ), "");
        }
        for( usize i = 0; i < darray_len( &list ); ++i ) {
            expect( list.buf[i] == (i32)i, "" );
        }

        darray_free( &list, &heap );
    }
    return 0;
}

int string(void) {
    {
        #define STRING "hello, world!"
        const char* string = STRING;

        expect( ( sizeof( STRING ) - 1 ) == cstr_len( string ), "" );
        expect( ( sizeof( STRING ) - 1 ) == cstr_len_utf8( string ), "" );
        #undef STRING
    }
    {
        String str = string_text( "hello, world" );

        expect( str.len == string_len_utf8( str ), "" );
        for( usize i = 0; i < str.len; ++i ) {
            expect( str.cc[i] == string_index( str, i ), "" );
        }
        expect( *string_first( str ) == 'h', "" );
        expect( *string_last( str )  == 'd', "" );
    }
    {
        expect( string_cmp( string_text( "hello" ), string_text( "hello" ) ), "" );
        expect( string_cmp( string_text( "hello" ), string_text( "goodbye" ) ) == false, "" );
        expect( string_cmp_min( string_text( "hello" ), string_text( "hello, world!" ) ), "" );
        expect( string_cmp_min( string_text( "hllo" ), string_text( "hello, world!" ) ) == false, "" );
    }
    {
        String str   = string_text( "abcd" );
        usize  index = 0;
        expect( string_find( str, 'a', &index ), "" );
        expect( index == 0, "" );

        expect( string_find( str, 'b', &index ), "" );
        expect( index == 1, "" );

        expect( string_find( str, 'c', &index ), "" );
        expect( index == 2, "" );

        expect( string_find( str, 'd', &index ), "" );
        expect( index == 3, "" );
    }
    {
        String str = string_text( "abbcccdddd" );
        expect( string_find_count( str, 'a' ) == 1, "" );
        expect( string_find_count( str, 'b' ) == 2, "" );
        expect( string_find_count( str, 'c' ) == 3, "" );
        expect( string_find_count( str, 'd' ) == 4, "" );
    }
    {
        String str   = string_text( "com.foo.exe" );
        usize  index = 0;
        expect( string_find_rev( str, '.', &index ), "" );
        expect( index == 7, "" );
    }
    {
        String str   = string_text( "foo/bar\\baz" );
        usize  index = 0;
        expect( string_find_set( str, string_text( "/\\" ), &index ), "" );
        expect( index == 3, "" );

        expect( string_find_set_rev( str, string_text( "/\\" ), &index ), "" );
        expect( index == 7, "" );

        expect( string_find_set_count( str, string_text( "/\\" ) ) == 2, "" );
    }
    {
        String str   = string_text( "foo bar baz" );
        usize  index = 0;
        expect( string_find_phrase( str, string_text( "bar" ), &index ), "" );
        String found = string_new( sizeof("bar") - 1, str.cc + index );
        expect(
            string_cmp( found, string_text( "bar" ) ),
            "found phrase does not match! found: '{s}' expected: '{s}'",
            found, string_text( "bar" ) );

        expect( string_find_phrase_rev( str, string_text( "bar" ), &index ), "" );
        found = string_new( sizeof("bar") - 1, str.cc + index );
        expect(
            string_cmp( found, string_text( "bar" ) ),
            "found phrase does not match! found: '{s}' expected: '{s}'",
            found, string_text( "bar" ) );
    }
    {
        String str = string_text( "foo bar bar baz baz baz" );
        expect( string_find_phrase_count( str, string_text( "foo" ) ) == 1, "" );
        expect( string_find_phrase_count( str, string_text( "bar" ) ) == 2, "" );
        expect( string_find_phrase_count( str, string_text( "baz" ) ) == 3, "" );
    }
    {
        String str = string_text( "aaa" );
        expect( string_cmp( str, string_truncate( str, 4 ) ), "" );
        expect( string_cmp( string_text( "aa" ), string_truncate( str, 2 ) ), "" );
    }
    {
        String str = string_text( "aaa" );
        expect( string_cmp( string_empty(), string_trim( str, 4 ) ), "" );
        expect( string_cmp( string_text( "aa" ), string_trim( str, 1 ) ), "" );
        expect( string_cmp( string_text( "a" ), string_trim( str, 2 ) ), "" );
    }
    {
        String str = string_text( "foo bar baz" );
        usize  start = 0;
        string_find_phrase( str, string_text( "bar" ), &start );
        usize  end   = 0;
        string_find_rev( str, 'r', &end );
        String clipped = string_clip( str, start, end + 1 );
        expect( string_cmp( clipped, string_text( "bar" ) ), "clip: '{s}' expected: '{s}'", clipped, string_text( "bar" ) );
    }
    {
        String str = string_text( "   foo   " );
        String leading  = string_trim_leading_whitespace( str );
        String trailing = string_trim_trailing_whitespace( str );
        String surround = string_trim_surrounding_whitespace( str );
        expect( string_cmp( leading, string_text( "foo   " ) ), "result: '{s}'", leading );
        expect( string_cmp( trailing, string_text( "   foo" ) ), "result: '{s}'", trailing );
        expect( string_cmp( surround, string_text( "foo" ) ), "result: '{s}'", surround );
    }
    {
        String str = string_text( "foo.bar" );
        String left = string_empty(), right = string_empty();
        string_split( str, 3, &left, &right );
        expect( string_cmp( left, string_text( "foo" ) ), "" );
        expect( string_cmp( right, string_text( "bar" ) ), "" );

        left  = string_empty();
        right = string_empty();
        string_split_ascii( str, '.', &left, &right );
        expect( string_cmp( left, string_text( "foo" ) ), "" );
        expect( string_cmp( right, string_text( "bar" ) ), "" );

        str = string_text( "foo    bar" );
        left  = string_empty();
        right = string_empty();
        string_split_whitespace( str, &left, &right );
        expect( string_cmp( left, string_text( "foo" ) ), "" );
        expect( string_cmp( right, string_text( "bar" ) ), "" );
    }
    {
        String str = string_text( "-10" );
        i64 i = 0;
        expect( string_parse_int( str, &i ), "" );
        expect( i == -10, "" );

        str   = string_text( "10" );
        u64 u = 0;
        expect( string_parse_uint( str, &u ), "" );
        expect( u == 10, "" );

        str = string_text( "-1.2" );
        f64 f = 0.0;
        expect( string_parse_float( str, &f ), "" );
        expect( (1.2 + f) < 0.0001, "f: {f}", f );
    }
    {
        char buf[4];
        memory_zero( buf, 4 );
        memory_copy(buf, "abcd", 4);
        String str;
        str.cc  = buf;
        str.len = 4;
        string_mut_reverse( str );
        expect( string_cmp( str, string_text( "dcba" ) ), "" );
        string_mut_to_upper( str );
        expect( string_cmp( str, string_text( "DCBA" ) ), "" );
        string_mut_to_lower( str );
        expect( string_cmp( str, string_text( "dcba" ) ), "" );
    }
    {
        StringBuf buf = string_buf_empty();
        AllocatorInterface heap = allocator_interface_from_heap();
        expect( string_buf_from_alloc( 10, &heap, &buf ), "" );

        string_buf_append( &buf, string_text( "hello, world!" ), &heap );
        expect( string_cmp( buf.slice, string_text( "hello, world!" ) ), "" );

        string_buf_append( &buf, string_text( " this is a message." ), &heap );
        expect( string_cmp( buf.slice, string_text( "hello, world! this is a message." ) ), "" );

        string_buf_free( &buf, &heap );
    }
    {
        StringBuf buf = string_buf_empty();
        AllocatorInterface heap = allocator_interface_from_heap();
        expect( string_buf_from_string_alloc( string_text( "hello, world!" ), &heap, &buf ), "" );
        string_buf_remove_range( &buf, 0, 7 );
        expect( string_cmp( buf.slice, string_text( "world!" ) ), "" );

        StringBuf clone = string_buf_empty();
        expect( string_buf_clone( &clone, &buf, &heap ), "failed to clone!" );
        expect( string_cmp( clone.slice, buf.slice ), "" );

        string_buf_free( &buf, &heap );
        string_buf_free( &clone, &heap );
    }
    {
        string_buf_create_from_stack( buf, 128 );
        char _buf[4] = { 'a', 'b', 'c', 'd' };
        for( usize i = 0; i < 4; ++i ) {
            expect( string_buf_try_emplace( &buf, _buf[i], 0 ), "" );
        }
        expect( string_cmp( buf.slice, string_text( "dcba" ) ), "" );
    }
    {
        string_buf_create_from_stack( buf, 128 );
        expect( string_buf_try_fmt( &buf, "hello, world! number: {u}", 10 ) == 0, "" );
        expect( string_cmp( string_text( "hello, world! number: 10" ), buf.slice ), "res: '{s}'", buf.slice );
    }

    return 0;
}

#if defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
#endif

