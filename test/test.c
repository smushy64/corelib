/**
 * Description:  Corelib tests.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: January 30, 2024
*/
// IWYU pragma: begin_keep
#include "core/prelude.h"
#include "core/collections.h"
#include "core/fmt.h"
#include "core/fs.h"        
#include "core/job_queue.h"
#include "core/lib.h"
#include "core/library.h"
#include "core/math.h"      
#include "core/memory.h"    
#include "core/path.h"
#include "core/print.h"     
#include "core/rand.h"
#include "core/slice.h"
#include "core/sort.h"
#include "core/stream.h"
#include "core/string.h"    
#include "core/sync.h"      
#include "core/system.h"
#include "core/thread.h"    
#include "core/time.h"
// IWYU pragma: end_keep

#if defined(CORE_COMPILER_CLANG)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

#define test_assert( condition, format, ... ) do {\
    if( !(condition) ) {\
        println_err( str_magenta( "[ASSERTION FAILED line:" stringify_macro( __LINE__ ) "]" ) " [" #condition "] " format, ##__VA_ARGS__ );\
        return -1;\
    }\
} while(0)

#define success( message, ... )\
    println( str_green( message " tests passed successfully!" ), ##__VA_ARGS__ )

int core_lib_tests(void);
int core_collections_tests(void);
int core_math_tests(void);

int main( int argc, char** argv ) {
    unused(argc,argv);

    int result = 0;
    #define test( fn ) result = fn(); if( result ) return result

    test( core_lib_tests );
    test( core_collections_tests );
    test( core_math_tests );

    #undef test
    return 0;
}

int core_lib_tests(void) {

    {
        u32 version = core_version();
        test_assert( core_get_major( version ) == CORE_LIB_VERSION_MAJOR, "get major is incorrect! expected: {u32} result: {u32}", CORE_LIB_VERSION_MAJOR, core_get_major( version ) );
        test_assert( core_get_minor( version ) == CORE_LIB_VERSION_MINOR, "get minor is incorrect! expected: {u32} result: {u32}", CORE_LIB_VERSION_MINOR, core_get_minor( version ) );
        test_assert( core_get_patch( version ) == CORE_LIB_VERSION_PATCH, "get patch is incorrect! expected: {u32} result: {u32}", CORE_LIB_VERSION_PATCH, core_get_patch( version ) );

        String expected_version_string = string_text( stringify_macro( CORE_LIB_VERSION_MAJOR ) "." stringify_macro( CORE_LIB_VERSION_MINOR ) "." stringify_macro( CORE_LIB_VERSION_PATCH ) );
        String version_string;
        version_string.cc = core_version_string( &version_string.len );

        test_assert( string_cmp( expected_version_string, version_string ), "expected string: '{s}'({usize}) result: '{s}'({usize})", expected_version_string, expected_version_string.len, version_string, version_string.len );

        success( "lib:version" );
    }

    return 0;
}

int core_collections_tests(void) {
    {
        Stack stack;
        usize cap      = 10;
        usize buf_size = sizeof(u32) * cap;
        void* buf      = memory_alloc( buf_size );
        assert_log( buf, "failed to allocate {f,.2,m}!", (f64)buf_size );

        stack = stack_new( sizeof(u32), cap, buf );

        test_assert( stack_is_empty( &stack ), "expected stack to be empty!" );
        test_assert( !stack_is_full( &stack ), "expected stack to not be full!" );

        u32 test_values[] = {
            0, 1, 2, 3,
            4, 5, 6, 7,
            8, 9
        };
        for( usize i = 0; i < cap; ++i ) {
            test_assert( stack_push( &stack, &i ), "push should have been successful!" );
        }

        test_assert( !stack_is_empty( &stack ), "expected stack to not be empty!" );
        test_assert( stack_is_full( &stack ), "expected stack to be full!" );
        u32 dummy = 10;
        test_assert( !stack_push( &stack, &dummy ), "push should not have been successful!" );

        usize i = 9;
        u32 item = U32_MAX;
        while( stack_pop( &stack, &item ) ) {
            u32 a = test_values[i--];
            u32 b = item;
            test_assert( a == b, "expected item to match test value! {u32} == {u32}", a, b );
        }

        memory_free( buf, buf_size );
        success( "collections:stack" );
    }

    {
        Queue queue;
        usize cap = 10;
        usize buf_size = sizeof(f64) * cap;
        void* buf      = memory_alloc( buf_size );
        assert_log( buf, "failed to allocate {f,.2,m}!", (f64)buf_size );

        queue = queue_new( sizeof(f64), cap, buf );
        test_assert( queue_is_empty( &queue ), "expected queue to be empty!" );
        test_assert( !queue_is_full( &queue ), "expected queue to not be full!" );

        f64 test_values[] = {
            1.0, 1.1, 1.2, 1.3,
            1.4, 1.5, 1.6, 1.7,
            1.8, 1.9
        };
        for( usize i = 0; i < cap; ++i ) {
            test_assert( queue_enqueue( &queue, test_values + i ), "enqueue should have been successful!" );
        }

        test_assert( !queue_is_empty( &queue ), "expected queue to not be empty!" );
        test_assert( queue_is_full( &queue ), "expected queue to be full!" );
        f64 dummy = 10.1;
        test_assert( !queue_enqueue( &queue, &dummy ), "enqueue should not have been successful!" );

        usize i  = 0;
        f64 item = F64_NAN;
        while( queue_dequeue( &queue, &item ) ) {
            f64 a = test_values[i++];
            f64 b = item;
            test_assert( fcmp( a, b ), "expected item to match test value! {f} == {f}", a, b );
        }

        memory_free( buf, buf_size );
        success( "collections:queue" );
    }

    {
        Hashmap map;
        usize cap   = 4;
        void* keys  = memory_alloc( sizeof(u64) * cap );
        u64* values = (u64*)memory_alloc( sizeof(u64) * cap );
        assert_log( keys && values, "failed to allocate keys and values!" );

        map = hashmap_new( sizeof(u64), cap, values, keys );

        u64 test_values[][2] = {
            { 6, 2 },
            { 5, 3 },
            { 4, 4 },
            { 3, 5 },
        };

        for( u32 i = 0; i < cap; ++i ) {
            test_assert( hashmap_insert( &map, test_values[i][0], &test_values[i][1] ), "expected to insert into hashmap!" );
        }
        test_assert( !hashmap_insert( &map, 2463435, &test_values[0][0] ), "expected insert into hashmap to fail!" );

        for( u32 i = 0; i < cap; ++i ) {
            u64 item;
            test_assert( hashmap_get( &map, test_values[i][0], &item ), "expected key {u64} to exist in hashmap!", test_values[i][0] );
            test_assert( item == test_values[i][1], "expected item with key {u64} to be {u64}, instead it was {u64}!", test_values[i][0], test_values[i][1], item );
        }
        u64 new_value = 10;
        test_assert( hashmap_set( &map, 6, &new_value ), "expected set to work with key {u64}!", 6ULL );
        u64 out_value;
        test_assert( hashmap_get( &map, 6, &out_value ), "expected get with key {u64} to work!", 6ULL );
        test_assert( new_value == out_value, "expected values to be equal!" );

        memory_free( keys, sizeof(u64) * cap );
        memory_free( values, sizeof(u64) * cap );
        success( "collections:hashmap" );
    }

    return 0;
}

int core_math_tests(void) {
    u64 rand_state = 0;{
        TimeStamp ts;
        time_query_timestamp( &ts );
        rand_state = (((ts.day + ts.hour) % (ts.minute)) + ts.second) * 8546536385;
    }
    {
        u16 value16    = 0x1B1A;
        u16 expected16 = 0x1A1B;

        u16 result16 = u16_endian_swap( value16 );
        test_assert( result16 == expected16, "endian swap 16 failed! expected: {u16,X,f} result: {u16,X,f}", expected16, result16 );

        u32 value32    = 0x1D1C1B1A;
        u32 expected32 = 0x1A1B1C1D;

        u32 result32 = u32_endian_swap( value32 );
        test_assert( result32 == expected32, "endian swap 32 failed! expected: {u32,X,f} result: {u32,X,f}", expected32, result32 );

        u64 value64    = 0x1B1A0F0E0D0C0B0A;
        u64 expected64 = 0x0A0B0C0D0E0F1A1B;

        u64 result64 = u64_endian_swap( value64 );
        test_assert( result64 == expected64, "endian swap 64 failed! expected: {u64,X,f} result: {u64,X,f}", expected64, result64 );

        success( "math:conversions:endian_swap" );
    }
    {
        for( u32 i = 0; i < 100; ++i ) {
            rgb _rgb = rgb(
                normalize_u64( rand_xor_next( &rand_state ) ),
                normalize_u64( rand_xor_next( &rand_state ) ),
                normalize_u64( rand_xor_next( &rand_state ) ) );
            hsl _hsl = rgb_to_hsl( _rgb );
            rgb back = hsl_to_rgb( _hsl );

            test_assert( v3_cmp( _rgb, back ), "hsl:{v3} | original: {v3} != from_hsl: {v3}!", _hsl, _rgb, back );
        }

        success( "math:vector3:rgb_to_hsl && math:vector3:hsl_to_rgb" );
    }

    return 0;
}

#if defined(CORE_COMPILER_CLANG) && !defined(CORE_LSP_CLANGD)
    #pragma clang diagnostic pop
#endif

