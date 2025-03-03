#if !defined(CORE_INTERNAL_PLATFORM_TIME_H) && defined(CORE_ENABLE_INTERNAL)
#define CORE_INTERNAL_PLATFORM_TIME_H
/**
 * @file   time.h
 * @brief  Platform time functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   February 06, 2025
*/
#include "core/types.h"
#include "core/time.h"

TimePosix platform_time_posix(void);
TimeSplit platform_time_split(void);
f64 platform_timer_milliseconds(void);
f64 platform_timer_seconds(void);

#endif /* header guard */
