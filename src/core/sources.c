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
    #include "src/core/cstdlib.c"
#endif

#if defined(CORE_PLATFORM_WINDOWS)
    #include "src/core/platform_win32.c"
#endif
#if defined(CORE_PLATFORM_POSIX)
    #include "src/core/platform_posix.c"
#endif
#if defined(CORE_ENABLE_STATIC_BUILD)
    // pragma_error( "static build option is unimplemented!" );
    #include "src/core/platform_staticmain.c"
#else
    #include "src/core/platform_sharedmain.c"
#endif

#include "src/core/alloc.c"
#include "src/core/fmt.c"
#include "src/core/fs.c"
#include "src/core/hash.c"
#include "src/core/lib.c"
#include "src/core/library.c"
#include "src/core/math.c"
#include "src/core/memory.c"
#include "src/core/path.c"
#include "src/core/rand.c"
#include "src/core/sort.c"
#include "src/core/string.c"
#include "src/core/sync.c"
#include "src/core/system.c"
#include "src/core/thread.c"
#include "src/core/time.c"
#include "src/core/unicode.c"
#include "src/core/process.c"

#endif /* header guard */
