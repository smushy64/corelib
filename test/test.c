/**
 * @file   test.c
 * @brief  Tests for core C library.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   July 20, 2024
*/
#include "core/macros.h"
#include "core/print.h"

int main( int argc, char** argv ) {
    unused(argc, argv);

    println( "hello, world!" );
    return 0;
}
