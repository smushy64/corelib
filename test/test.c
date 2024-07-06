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
#include "core/fmt.h"
#include "core/lib.h"
#include "core/fs.h"
#include "core/thread.h"
#include "core/sync.h"
#include "core/system.h"
// IWYU pragma: end_keep

#if defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

#define test( func )\
    do {\
        int res = func ();\
        if( res ) {\
            println( color_text( ANSI_COLOR_RED, "test " #func " failed!" ) );\
            return res;\
        } else {\
            println( "test "#func" completed successfully!" );\
        }\
    } while(0)\

#define test_expect( condition, message, ... ) do {\
    if( !(condition) ) {\
        println( color_text( ANSI_COLOR_RED, __FILE__\
            ":{i}: condition '" #condition "' failed! "\
            message ), __LINE__, ##__VA_ARGS__ );\
        return -1;\
    }\
} while(0)

int string(void);
int collections(void);

void logger_callback(
    CoreLoggingLevel level, usize len, const char* msg, va_list va, void* params
) {
    unused(level, params);
    print(ANSI_COLOR_RED);

    stream_fmt_va( pipe_stream_write, pipe_stdout(), len, msg, va );

    println(ANSI_COLOR_RESET);
}

char buffered_log_buffer[kibibytes(2)];
StringBuf buffered_log_buf = string_buf_new( sizeof(buffered_log_buffer), buffered_log_buffer );

usize void_stream( void* t, usize c, const void* b ) {
    unused(t,b);
    return c;
}

void buffered_log_flush(void) {
    pipe_write( pipe_stdout(), buffered_log_buf.len, buffered_log_buf.v, 0 );
    string_buf_clear( &buffered_log_buf );
}
void buffered_log( const char* format, ... ) {
    usize format_len = cstr_len( format );
    va_list va1, va2;
    va_start( va1, format );
    va_copy( va2, va1 );

    usize len = stream_fmt_va( void_stream, 0, format_len, format, va1 );

    va_end( va1 );

    if( string_buf_remaining( &buffered_log_buf ) < len ) {
        buffered_log_flush();
    }
}

int main( int argc, char** argv ) {
    unused( argc, argv );

    core_set_logging_level( CORE_LOGGING_LEVEL_WARN );
    core_set_logging_callback( logger_callback, 0 );

    AllocatorInterface heap = allocator_interface_from_heap();
    Path  path  = path_text( "~" );
    usize count = directory_query_item_count( path, true, &heap );
    println( "directory '{p}' count: {usize}", path, count );

    return 0;

    SystemInfo si;
    memory_zero( &si, sizeof(si) );

    system_query_info( &si );
    println( "CPU: {s} x {u32}", si.cpu_name, si.cpu_count );
    println( "GPU: {s}", si.gpu_name );
    println( "MEM: {usize,mib}", si.total_memory );
    println( "PAG: {usize,mib}", si.page_size );

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
        test_expect( hashmap_from_alloc( sizeof(int), CAPACITY, &map, &heap ), "" );

        int item = 99;
        test_expect( hashmap_try_insert( &map, 10, &item ), "" );
        item = 80;
        test_expect( hashmap_try_insert( &map, 6, &item ), "" );
        item = 70;
        test_expect( hashmap_try_insert( &map, 255, &item ), "" );

        test_expect( hashmap_grow( &map, 2, &heap ), "" );

        int out = 0;
        test_expect( hashmap_index( &map, 10, &out ), "" );
        test_expect( out == 99, "{i32}", out );

        test_expect( hashmap_index( &map, 6, &out ), "" );
        test_expect( out == 80, "{i32}", out );

        test_expect( hashmap_index( &map, 255, &out ), "" );
        test_expect( out == 70, "{i32}", out );

        test_expect( hashmap_remove( &map, 6, &out ), "" );
        test_expect( out == 80, "{i32}", out );

        test_expect( hashmap_index( &map, 10, &out ), "" );
        test_expect( out == 99, "{i32}", out );

        test_expect( hashmap_index( &map, 255, &out ), "" );
        test_expect( out == 70, "{i32}", out );

        hashmap_free( &map, &heap );

        #undef CAPACITY
    }
    {
        char buf[hashmap_memory_requirement( sizeof(int), 10 )];
        memory_zero( buf, sizeof(buf) );

        Hashmap map = hashmap_new( sizeof(int), 10, buf );

        int item = 10;
        test_expect( hashmap_try_insert_text( &map, "hello, world!", &item ), "" );
        test_expect( hashmap_index_text( &map, "hello, world!", &item ), "" );
        test_expect( item == 10, "" );
    }
    {
        PackedBool packed[packed_bool_calculate_size( 10 )];
        memory_zero( packed, sizeof(packed) );
        test_expect( packed_bool_bytes_to_bits( static_array_len( packed ) ) == 16, "" );
        test_expect( packed_bool_set( packed, 0, true ), "" );
        test_expect( packed_bool_set( packed, 1, false ), "" );
        test_expect( packed_bool_index( packed, 0 ) == true, "" );
        test_expect( packed_bool_index( packed, 1 ) == false, "" );
        test_expect( packed_bool_index( packed, 9 ) == false, "" );
    }
    {
        #define CAPACITY 10
        int buf[CAPACITY];
        memory_zero( buf, sizeof(buf) );

        Queue queue = queue_new( sizeof(int), CAPACITY, buf );
        test_expect( queue_is_empty( &queue ), "" );

        #define enqueue( queue, item )\
            test_expect( queue_try_enqueue( queue, item ), "" )
        #define expect_dequeue( queue, item )\
            test_expect( queue_dequeue( queue, item ), "" )

        for( usize i = 0; i < CAPACITY; ++i ) {
            int item = (int)i + 10;
            enqueue( &queue, &item );
        }

        for( usize i = 0; i < 4; ++i ) {
            int item = 0;
            expect_dequeue( &queue, &item );
            test_expect( item == (int)i + 10, "expected item: {i32} got: {i32}", (int)i + 10, item );
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

        test_expect( darray_from_alloc( 10, &list, &heap ), "failed to alloc list!" );
        test_expect( list.cap == 10, "" );

        for( usize i = 0; i < darray_cap( &list ); ++i ) {
            i32 item = i;
            test_expect( darray_try_push( &list, &item ), "");
        }
        for( usize i = 0; i < darray_len( &list ); ++i ) {
            test_expect( list.buf[i] == (i32)i, "" );
        }

        darray_free( &list, &heap );
    }
    return 0;
}

int string(void) {
    {
        #define STRING "hello, world!"
        const char* string = STRING;

        test_expect( ( sizeof( STRING ) - 1 ) == cstr_len( string ), "" );
        test_expect( ( sizeof( STRING ) - 1 ) == cstr_len_utf8( string ), "" );
        #undef STRING
    }
    {
        String str = string_text( "hello, world" );

        test_expect( str.len == string_len_utf8( str ), "" );
        for( usize i = 0; i < str.len; ++i ) {
            test_expect( str.cc[i] == string_index( str, i ), "" );
        }
        test_expect( *string_first( str ) == 'h', "" );
        test_expect( *string_last( str )  == 'd', "" );
    }
    {
        test_expect( string_cmp( string_text( "hello" ), string_text( "hello" ) ), "" );
        test_expect( string_cmp( string_text( "hello" ), string_text( "goodbye" ) ) == false, "" );
        test_expect( string_cmp_min( string_text( "hello" ), string_text( "hello, world!" ) ), "" );
        test_expect( string_cmp_min( string_text( "hllo" ), string_text( "hello, world!" ) ) == false, "" );
    }
    {
        String str   = string_text( "abcd" );
        usize  index = 0;
        test_expect( string_find( str, 'a', &index ), "" );
        test_expect( index == 0, "" );

        test_expect( string_find( str, 'b', &index ), "" );
        test_expect( index == 1, "" );

        test_expect( string_find( str, 'c', &index ), "" );
        test_expect( index == 2, "" );

        test_expect( string_find( str, 'd', &index ), "" );
        test_expect( index == 3, "" );
    }
    {
        String str = string_text( "abbcccdddd" );
        test_expect( string_find_count( str, 'a' ) == 1, "" );
        test_expect( string_find_count( str, 'b' ) == 2, "" );
        test_expect( string_find_count( str, 'c' ) == 3, "" );
        test_expect( string_find_count( str, 'd' ) == 4, "" );
    }
    {
        String str   = string_text( "com.foo.exe" );
        usize  index = 0;
        test_expect( string_find_rev( str, '.', &index ), "" );
        test_expect( index == 7, "" );
    }
    {
        String str   = string_text( "foo/bar\\baz" );
        usize  index = 0;
        test_expect( string_find_set( str, string_text( "/\\" ), &index ), "" );
        test_expect( index == 3, "" );

        test_expect( string_find_set_rev( str, string_text( "/\\" ), &index ), "" );
        test_expect( index == 7, "" );

        test_expect( string_find_set_count( str, string_text( "/\\" ) ) == 2, "" );
    }
    {
        String str   = string_text( "foo bar baz" );
        usize  index = 0;
        test_expect( string_find_phrase( str, string_text( "bar" ), &index ), "" );
        String found = string_new( sizeof("bar") - 1, str.cc + index );
        test_expect(
            string_cmp( found, string_text( "bar" ) ),
            "found phrase does not match! found: '{s}' expected: '{s}'",
            found, string_text( "bar" ) );

        test_expect( string_find_phrase_rev( str, string_text( "bar" ), &index ), "" );
        found = string_new( sizeof("bar") - 1, str.cc + index );
        test_expect(
            string_cmp( found, string_text( "bar" ) ),
            "found phrase does not match! found: '{s}' expected: '{s}'",
            found, string_text( "bar" ) );
    }
    {
        String str = string_text( "foo bar bar baz baz baz" );
        test_expect( string_find_phrase_count( str, string_text( "foo" ) ) == 1, "" );
        test_expect( string_find_phrase_count( str, string_text( "bar" ) ) == 2, "" );
        test_expect( string_find_phrase_count( str, string_text( "baz" ) ) == 3, "" );
    }
    {
        String str = string_text( "aaa" );
        test_expect( string_cmp( str, string_truncate( str, 4 ) ), "" );
        test_expect( string_cmp( string_text( "aa" ), string_truncate( str, 2 ) ), "" );
    }
    {
        String str = string_text( "aaa" );
        test_expect( string_cmp( string_empty(), string_trim( str, 4 ) ), "" );
        test_expect( string_cmp( string_text( "aa" ), string_trim( str, 1 ) ), "" );
        test_expect( string_cmp( string_text( "a" ), string_trim( str, 2 ) ), "" );
    }
    {
        String str = string_text( "foo bar baz" );
        usize  start = 0;
        string_find_phrase( str, string_text( "bar" ), &start );
        usize  end   = 0;
        string_find_rev( str, 'r', &end );
        String clipped = string_clip( str, start, end + 1 );
        test_expect( string_cmp( clipped, string_text( "bar" ) ), "clip: '{s}' expected: '{s}'", clipped, string_text( "bar" ) );
    }
    {
        String str = string_text( "   foo   " );
        String leading  = string_trim_leading_whitespace( str );
        String trailing = string_trim_trailing_whitespace( str );
        String surround = string_trim_surrounding_whitespace( str );
        test_expect( string_cmp( leading, string_text( "foo   " ) ), "result: '{s}'", leading );
        test_expect( string_cmp( trailing, string_text( "   foo" ) ), "result: '{s}'", trailing );
        test_expect( string_cmp( surround, string_text( "foo" ) ), "result: '{s}'", surround );
    }
    {
        String str = string_text( "foo.bar" );
        String left = string_empty(), right = string_empty();
        string_split( str, 3, &left, &right );
        test_expect( string_cmp( left, string_text( "foo" ) ), "" );
        test_expect( string_cmp( right, string_text( "bar" ) ), "" );

        left  = string_empty();
        right = string_empty();
        string_split_ascii( str, '.', &left, &right );
        test_expect( string_cmp( left, string_text( "foo" ) ), "" );
        test_expect( string_cmp( right, string_text( "bar" ) ), "" );

        str = string_text( "foo    bar" );
        left  = string_empty();
        right = string_empty();
        string_split_whitespace( str, &left, &right );
        test_expect( string_cmp( left, string_text( "foo" ) ), "" );
        test_expect( string_cmp( right, string_text( "bar" ) ), "" );
    }
    {
        String str = string_text( "-10" );
        i64 i = 0;
        test_expect( string_parse_int( str, &i ), "" );
        test_expect( i == -10, "" );

        str   = string_text( "10" );
        u64 u = 0;
        test_expect( string_parse_uint( str, &u ), "" );
        test_expect( u == 10, "" );

        str = string_text( "-1.2" );
        f64 f = 0.0;
        test_expect( string_parse_float( str, &f ), "" );
        test_expect( (1.2 + f) < 0.0001, "f: {f}", f );
    }
    {
        char buf[4];
        memory_zero( buf, 4 );
        memory_copy(buf, "abcd", 4);
        String str;
        str.cc  = buf;
        str.len = 4;
        string_mut_reverse( str );
        test_expect( string_cmp( str, string_text( "dcba" ) ), "" );
        string_mut_to_upper( str );
        test_expect( string_cmp( str, string_text( "DCBA" ) ), "" );
        string_mut_to_lower( str );
        test_expect( string_cmp( str, string_text( "dcba" ) ), "" );
    }
    {
        StringBuf buf = string_buf_empty();
        AllocatorInterface heap = allocator_interface_from_heap();
        test_expect( string_buf_from_alloc( 10, &heap, &buf ), "" );

        string_buf_append( &buf, string_text( "hello, world!" ), &heap );
        test_expect( string_cmp( buf.slice, string_text( "hello, world!" ) ), "" );

        string_buf_append( &buf, string_text( " this is a message." ), &heap );
        test_expect( string_cmp( buf.slice, string_text( "hello, world! this is a message." ) ), "" );

        string_buf_free( &buf, &heap );
    }
    {
        StringBuf buf = string_buf_empty();
        AllocatorInterface heap = allocator_interface_from_heap();
        test_expect( string_buf_from_string_alloc( string_text( "hello, world!" ), &heap, &buf ), "" );
        string_buf_remove_range( &buf, 0, 7 );
        test_expect( string_cmp( buf.slice, string_text( "world!" ) ), "" );

        StringBuf clone = string_buf_empty();
        test_expect( string_buf_clone( &clone, &buf, &heap ), "failed to clone!" );
        test_expect( string_cmp( clone.slice, buf.slice ), "" );

        string_buf_free( &buf, &heap );
        string_buf_free( &clone, &heap );
    }
    {
        string_buf_create_from_stack( buf, 128 );
        char _buf[4] = { 'a', 'b', 'c', 'd' };
        for( usize i = 0; i < 4; ++i ) {
            test_expect( string_buf_try_emplace( &buf, _buf[i], 0 ), "" );
        }
        test_expect( string_cmp( buf.slice, string_text( "dcba" ) ), "" );
    }
    {
        string_buf_create_from_stack( buf, 128 );
        test_expect( string_buf_try_fmt( &buf, "hello, world! number: {u}", 10 ) == 0, "" );
        test_expect( string_cmp( string_text( "hello, world! number: 10" ), buf.slice ), "res: '{s}'", buf.slice );
    }

    return 0;
}

#if defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
#endif

