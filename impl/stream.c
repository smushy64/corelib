/**
 * Description:  Data streaming implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 14, 2024
*/
#include "core/internal/platform.h"
#include "core/stream.h"

attr_core_api usize stream_console(
    void* console_handle, usize count, const void* bytes
) {
    platform_console_write( console_handle, count, (const char*)bytes );
    return 0;
}


