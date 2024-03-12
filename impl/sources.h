#if !defined(CORE_IMPL_SOURCES_H)
#define CORE_IMPL_SOURCES_H
/**
 * @file    sources.h
 * @brief   Header that includes all required sources.
 * @warning Only include if compiling in C.
 * @author  Alicia Amarilla (smushyaa@gmail.com)
 * @date    February 07, 2024
*/

#include "core/defines.h"

#include "impl/collections.c"
#include "impl/fmt.c"
#include "impl/fs.c"
#include "impl/job_queue.c"
#include "impl/lib.c"
#include "impl/library.c"
#include "impl/math.c"
#include "impl/memory.c"
#include "impl/path.c"
#include "impl/rand.c"
#include "impl/sort.c"
#include "impl/stream.c"
#include "impl/string.c"
#include "impl/sync.c"
#include "impl/system.c"
#include "impl/thread.c"
#include "impl/time.c"

#if defined(CORE_PLATFORM_WINDOWS)
    #include "impl/platform_win32.c"
#elif defined(CORE_PLATFORM_LINUX)
    #include "impl/platform_linux.c"
#endif

#include "impl/platform_dllmain.c"

#endif /* header guard */
