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

#if !defined(CORE_ENABLE_STDLIB)
    #include "impl/cstdlib.c"
#endif

#if defined(CORE_PLATFORM_WINDOWS)
    #include "impl/platform_win32.c"
#endif
#if defined(CORE_PLATFORM_POSIX)
    #include "impl/platform_posix.c"
#endif
#if defined(CORE_ENABLE_STATIC_BUILD)
    // pragma_error( "static build option is unimplemented!" );
    #include "impl/platform_staticmain.c"
#else
    #include "impl/platform_sharedmain.c"
#endif

#include "impl/alloc.c"
#include "impl/fmt.c"
#include "impl/fs.c"
#include "impl/hash.c"
#include "impl/lib.c"
#include "impl/library.c"
#include "impl/math.c"
#include "impl/memory.c"
#include "impl/path.c"
#include "impl/rand.c"
#include "impl/sort.c"
#include "impl/string.c"
#include "impl/sync.c"
#include "impl/system.c"
#include "impl/thread.c"
#include "impl/time.c"
#include "impl/unicode.c"

#endif /* header guard */
