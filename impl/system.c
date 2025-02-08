/**
 * Description:  Query information.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 27, 2024
*/
#include "core/prelude.h"
#include "core/system.h"

#include "core/internal/platform/misc.h"

attr_core_api void system_query_info( SystemInfo* out_info ) {
    platform_system_query_info( out_info );
}


