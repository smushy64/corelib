#if !defined(CORE_INTERNAL_PROCESS_H) && defined(CORE_ENABLE_INTERNAL)
#define CORE_INTERNAL_PROCESS_H
/**
 * @file   process.h
 * @brief  Platform process functions.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 27, 2025
*/
#include "core/types.h"
#include "core/process.h"

struct _StringPOD platform_environment_query( struct _StringPOD key );
b32 platform_environment_set( struct _StringPOD key, struct _StringPOD value );
b32 platform_process_exec_async(
    Command                  command,
    Process*                 out_pid,
    const struct _StringPOD* opt_working_directory,
    const EnvironmentBuf*    opt_environment,
    const FD*                opt_stdin,
    const FD*                opt_stdout,
    const FD*                opt_stderr );
void platform_process_discard( Process* pid );
int  platform_process_wait( Process* pid );
b32  platform_process_wait_timed( Process* pid, u32 msec, int* opt_out_exit_code );
void platform_process_kill( Process* pid );
b32  platform_process_find( struct _StringPOD process_name );

#endif /* header guard */
