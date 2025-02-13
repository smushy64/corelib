/**
 * @file   test.c
 * @brief  Tests for core C library.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   July 20, 2024
*/
#include "core/macros.h"
#include "core/print.h"
#include "core/string.h"

int main( int argc, char** argv ) {
    unused(argc, argv);

    String foo = string_text( "foo" );
    String bar = string_text( "bar" );
    println( "'{s}' '{s}'", foo, bar );
    return 0;
}
