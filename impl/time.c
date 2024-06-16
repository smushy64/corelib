/**
 * Description:  Time implementation.
 * Author:       Alicia Amarilla (smushyaa@gmail.com)
 * File Created: February 13, 2024
*/
#include "core/prelude.h"
#include "core/time.h"
#include "core/internal/platform.h"

attr_core_api TimePosix time_posix(void) {
    return platform_time_posix();
}
attr_core_api TimeSplit time_split(void) {
    return platform_time_split();
}

attr_core_api b32 time_hour_24_to_12( TimeHour hr24, TimeHour* out_hr12 ) {
    TimeHour hr12 = hr24 % 12;
    b32      am   = hr24 < 12;

    *out_hr12 = hr12;
    return am;
}
attr_core_api f64 timer_milliseconds(void) {
    return platform_timer_milliseconds();
}
attr_core_api f64 timer_seconds(void) {
    return platform_timer_seconds();
}

