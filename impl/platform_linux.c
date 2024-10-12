/**
 * @file   platform_linux.c
 * @brief  Linux platform implementation. Extension of platform_posix.c
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   October 11, 2024
*/
#include "core/defines.h"

#if defined(CORE_PLATFORM_LINUX)
#include "core/types.h"
#include "core/internal/platform.h"
#include "core/internal/logging.h"

#define _GNU_SOURCE
#include <sys/mman.h>
#include <pthread.h>
#include <dlfcn.h>
#include <errno.h>

void ms_to_ts( u32 ms, struct timespec* out_ts );
void ms_to_ts_abs( u32 ms, struct timespec* out_ts );

void* platform_heap_realloc(
    void* old_buffer, const usize old_size, const usize new_size
) {
    return mremap( old_buffer, old_size, new_size, MREMAP_MAYMOVE );
}
b32 posix_thread_join_timed( ThreadHandle* handle, u32 ms, int* opt_out_exit_code ) {
    struct timespec ts;
    memory_zero( &ts, sizeof(ts) );
    ms_to_ts_abs( ms, &ts );

    void* ret_code = 0;
    int ret = pthread_timedjoin_np( 
        (pthread_t)handle->opaque.handle, &ret_code, &ts );

    if( ret ) {
        switch( ret ) {
            case EINVAL: {
                core_error( "linux: pthread_timedjoin_np returned EINVAL!" );
            } break;
            case ETIMEDOUT: break;
        }
        return false;
    }

    if( opt_out_exit_code ) {
        *opt_out_exit_code = (int)(isize)ret_code;
    }
    return true;
}

void* platform_library_open( const char* name ) {
    return dlopen( name, RTLD_GLOBAL | RTLD_LAZY );
}
void* platform_library_get( const char* name ) {
    return dlopen( name, RTLD_NOLOAD );
}
void platform_library_close( void* lib ) {
    dlclose( lib );
}

#define FD_OPEN_TMP_FLAG (O_TMPFILE)

#endif /* Platform Linux */
