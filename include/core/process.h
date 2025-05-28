#if !defined(CORE_PROCESS_H)
#define CORE_PROCESS_H
/**
 * @file   process.h
 * @brief  Sub-processes.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 26, 2025
*/
#include "core/types.h"
#include "core/attributes.h"
#include "core/string.h"
#include "core/path.h"
#include "core/fs.h"

struct AllocatorInterface;

/// @brief Process handle.
typedef struct Process {
#if defined(CORE_PLATFORM_POSIX)
    int opaque;
#else
    void* opaque;
#endif
} Process;

/// @brief Slice of command arguments.
typedef struct Command {
    /// @brief Number of arguments.
    usize              len;
    /// @brief Pointer to arguments.
    struct _StringPOD* buf;
} Command;

/// @brief Buffer of command arguments.
typedef struct CommandBuf {
    /// @brief Number of arguments buffer can hold.
    usize cap;
    union {
        struct {
            /// @brief Number of arguments.
            usize              len;
            /// @brief Pointer to arguments.
            struct _StringPOD* buf;
        };
        /// @brief Command slice.
        struct Command command;
    };
    /// @brief Character buffer for command buffer.
    struct _StringBufPOD text;
} CommandBuf;

/// @brief Buffer of environment key/values.
typedef struct EnvironmentBuf {
    /// @brief Number of environment key value pairs buffer can hold.
    usize                cap;
    /// @brief Number of key value pairs in buffer.
    usize                len;
    /// @brief Pointer to environment key/value pairs.
    /// @note
    /// Keys and values are packed consecutively.
    /// Size of this buffer is sizeof(String) * 2 * cap
    struct _StringPOD*   buf;
    /// @brief Character buffer for environment buffer.
    struct _StringBufPOD text;
} EnvironmentBuf;

/// @brief Initialize allocated command builder.
/// @param      command_cap Max number of commands.
/// @param      text_cap    Max number of characters in text buf.
/// @param[in]  command_buf Pointer to buffer for command arguments.
/// @param[in]  text_buf    Pointer to buffer for command text buffer.
/// @param[out] out_buf     Pointer to write command buffer.
attr_header
void command_buf_initialize(
    usize              command_cap,
    usize              text_cap,
    struct _StringPOD* command_buf,
    char*              text_buf,
    CommandBuf*        out_buf );

/// @brief Append arguments to command buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to command buffer.
/// @param     count     Number of arguments to append.
/// @param[in] arguments Pointer to arguments to append.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
attr_core_api
b32 command_buf_append_arguments(
    struct AllocatorInterface* allocator,
    CommandBuf*                buf,
    usize                      count,
    struct _StringPOD*         arguments );
/// @brief Append arguments to command buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to command buffer.
/// @param[in] va        Variadic list of arguments.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
attr_core_api
b32 command_buf_append_va(
    struct AllocatorInterface* allocator,
    CommandBuf*                buf,
    va_list                    va );
/// @brief Append arguments to command buffer.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator interface.
/// @param[in] buf       (CommandBuf*)         Pointer to command buffer.
/// @param     ...       (String)              Arguments.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
#define command_buf_append( allocator, buf, ... ) \
    _internal_command_buf_append( allocator, buf, ##__VA_ARGS__, string_empty() )
/// @brief Append arguments to command buffer.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator interface.
/// @param[in] buf       (CommandBuf*)         Pointer to command buffer.
/// @param     ...       (const char*)         Arguments.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
#define command_buf_append_text( allocator, buf, ... ) \
    _internal_command_buf_append_text( allocator, buf, ##__VA_ARGS__, NULL )

/// @brief Try to append arguments to command buffer.
/// @param[in] buf       Pointer to command buffer.
/// @param     count     Number of arguments to append.
/// @param[in] arguments Pointer to arguments to append.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
attr_core_api
b32 command_buf_try_append_arguments(
    CommandBuf*        buf,
    usize              count,
    struct _StringPOD* arguments );
/// @brief Try to append arguments to command buffer.
/// @param[in] buf Pointer to command buffer.
/// @param[in] va  Variadic list of arguments.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
attr_core_api
b32 command_buf_try_append_va( CommandBuf* buf, va_list va );
/// @brief Try to append arguments to command buffer.
/// @param[in] buf (CommandBuf*) Pointer to command buffer.
/// @param[in] ... (String)      Arguments.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
#define command_buf_try_append( buf, ... ) \
    _internal_command_buf_try_append( buf, ##__VA_ARGS__, string_empty() )
/// @brief Try to append arguments to command buffer.
/// @param[in] buf (CommandBuf*) Pointer to command buffer.
/// @param[in] ... (const char*) Arguments.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
#define command_buf_try_append_text( buf, ... ) \
    _internal_command_buf_try_append_text( buf, ##__VA_ARGS__, NULL )

/// @brief Reset command buffer.
/// @param[in] buf Pointer to command buffer.
attr_core_api
void command_buf_reset( CommandBuf* buf );
/// @brief Free command buffer allocated with allocator interface.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to command buffer.
attr_core_api
void command_buf_free( struct AllocatorInterface* allocator, CommandBuf* buf );

/// @brief Initialize allocated environment buffer.
/// @param      key_value_pair_cap Max number of key/value pairs in key_value_buf.
/// @param      text_cap           Max number of characters in text_buf.
/// @param[in]  key_value_buf      Pointer to buffer for key/value pairs.
///                                  Must be able to hold key_value_pair_cap * 2 strings.
/// @param[in]  text_buf           Pointer to buffer for environment text buffer.
/// @param[out] out_buf            Pointer to write environment buffer.
attr_header
void environment_buf_initialize(
    usize              key_value_pair_cap,
    usize              text_cap,
    struct _StringPOD* key_value_buf,
    char*              text_buf,
    EnvironmentBuf*    out_buf );

/// @brief Add environment key/value pair to environment buffer.
/// @warning
/// When adding a value containing multiple paths,
/// always use @c ; as a separator rather than @c :
/// When the environment buffer is interpreted, it will change
/// path separators to the appropriate separator.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to environment buffer.
/// @param     key       Key to add.
/// @param     value     Value to add.
/// @return
///     - @c true  : Key/value pair added to environment buffer.
///     - @c false : Failed to add key/value pair to environment buffer.
attr_core_api
b32 environment_buf_add(
    struct AllocatorInterface* allocator,
    EnvironmentBuf*            buf,
    struct _StringPOD          key,
    struct _StringPOD          value );
/// @brief Add environment key/value pair to environment buffer.
/// @warning
/// When adding a value containing multiple paths,
/// always use @c ; as a separator rather than @c :
/// When the environment buffer is interpreted, it will change
/// path separators to the appropriate separator.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to environment buffer.
/// @param     key       Key to add.
/// @param     value     Value to add.
/// @return
///     - @c true  : Key/value pair added to environment buffer.
///     - @c false : Failed to add key/value pair to environment buffer.
#define environment_buf_add_text( allocator, buf, key, value ) \
    environment_buf_add( allocator, buf, string_text(key), string_text(value) )
/// @brief Set environment value pair of key.
/// @warning
/// When setting a value containing multiple paths,
/// always use @c ; as a separator rather than @c :
/// When the environment buffer is interpreted, it will change
/// path separators to the appropriate separator.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to environment buffer.
/// @param     key       Key of key/value pair to set.
/// @param     value     Value to set.
/// @return
///     - @c true  : Value changed successfully.
///     - @c false : Failed to set value.
attr_core_api
b32 environment_buf_set(
    struct AllocatorInterface* allocator,
    EnvironmentBuf*            buf,
    struct _StringPOD          key,
    struct _StringPOD          value );
/// @brief Set environment value pair of key.
/// @warning
/// When setting a value containing multiple paths,
/// always use @c ; as a separator rather than @c :
/// When the environment buffer is interpreted, it will change
/// path separators to the appropriate separator.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to environment buffer.
/// @param     key       Key of key/value pair to set.
/// @param     value     Value to set.
/// @return
///     - @c true  : Value changed successfully.
///     - @c false : Failed to set value.
#define environment_buf_set_text( allocator, buf, key, value ) \
    environment_buf_set( allocator, buf, string_text(key), string_text(value) )
/// @brief Remove key/value pair in environment buffer.
/// @param[in] buf Pointer to environment buffer.
/// @param     key Key of key/value pair to remove.
attr_core_api
void environment_buf_remove(
    EnvironmentBuf*   buf,
    struct _StringPOD key );
/// @brief Remove key/value pair in environment buffer.
/// @param[in] buf Pointer to environment buffer.
/// @param     key Key of key/value pair to remove.
#define environment_buf_remove_text( buf, key ) \
    environment_buf_remove( buf, string_text(key) )

/// @brief Try to add key/value pair to environment buffer.
/// @warning
/// When adding a value containing multiple paths,
/// always use @c ; as a separator rather than @c :
/// When the environment buffer is interpreted, it will change
/// path separators to the appropriate separator.
/// @param[in] buf   Pointer to environment buffer.
/// @param     key   Key to add.
/// @param     value Value to add.
/// @return
///     - @c true  : Value changed successfully.
///     - @c false : Failed to set value.
attr_core_api
b32 environment_buf_try_add(
    EnvironmentBuf*   buf,
    struct _StringPOD key,
    struct _StringPOD value );
/// @brief Try to add key/value pair to environment buffer.
/// @warning
/// When adding a value containing multiple paths,
/// always use @c ; as a separator rather than @c :
/// When the environment buffer is interpreted, it will change
/// path separators to the appropriate separator.
/// @param[in] buf   Pointer to environment buffer.
/// @param     key   Key to add.
/// @param     value Value to add.
/// @return
///     - @c true  : Value changed successfully.
///     - @c false : Failed to set value.
#define environment_buf_try_add_text( buf, key, value ) \
    environment_buf_try_add( buf, string_text(key), string_text(value) )
/// @brief Try to set environment value pair of key.
/// @warning
/// When setting a value containing multiple paths,
/// always use @c ; as a separator rather than @c :
/// When the environment buffer is interpreted, it will change
/// path separators to the appropriate separator.
/// @param[in] buf   Pointer to environment buffer.
/// @param     key   Key of key/value pair to set.
/// @param     value Value to set.
/// @return
///     - @c true  : Value changed successfully.
///     - @c false : Failed to set value.
attr_core_api
b32 environment_buf_try_set(
    EnvironmentBuf*   buf,
    struct _StringPOD key,
    struct _StringPOD value );
/// @brief Try to set environment value pair of key.
/// @warning
/// When setting a value containing multiple paths,
/// always use @c ; as a separator rather than @c :
/// When the environment buffer is interpreted, it will change
/// path separators to the appropriate separator.
/// @param[in] buf   Pointer to environment buffer.
/// @param     key   Key of key/value pair to set.
/// @param     value Value to set.
/// @return
///     - @c true  : Value changed successfully.
///     - @c false : Failed to set value.
#define environment_buf_try_set_text( buf, key, value ) \
    environment_buf_try_set( buf, key, value )

/// @brief Reset environment buffer.
/// @param[in] buf Pointer to environment buffer to reset.
attr_core_api
void environment_buf_reset( EnvironmentBuf* buf );
/// @brief Free environment buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to environment buffer.
attr_core_api
void environment_buf_free( struct AllocatorInterface* allocator, EnvironmentBuf* buf );

/// @brief Query value for given environment key.
/// @warning
/// The returned string should be used immediately after calling this function!
/// Otherwise, store it in a buffer.
/// @param key Key of environment variable.
/// @return Read-only value of environment variable. Returns empty string if not set.
attr_core_api
struct _StringPOD environment_query( struct _StringPOD key );
/// @brief Set environment value.
/// @param key   Key of environment variable.
/// @param value Value of environment variable.
/// @return
///     - @c true  : Successfully set environment variable.
///     - @c false : Failed to set environment variable.
attr_core_api
b32 environment_set( struct _StringPOD key, struct _StringPOD value );

/// @brief Synchronously execute process.
/// @param     command               (Command)         Command to execute.
/// @param[in] opt_working_directory (Path*)           Path to execute process in.
/// @param[in] opt_environment       (EnvironmentBuf*) Environment variables to execute with.
/// @param[in] opt_stdin             (PipeRead*)       Pipe for sub-process stdin.
/// @param[in] opt_stdout            (PipeWrite*)      Pipe for sub-process stdout.
/// @param[in] opt_stderr            (PipeWrite*)      Pipe for sub-process stderr.
/// @return
///     - @c -2    : Failed to execute process.
///     - @c -1    : Sub-process executed but exited abnormally.
///     - @c 0-255 : Exit code of sub-process.
#define process_exec( ... ) \
    _internal_process_exec( __VA_ARGS__, NULL, NULL, NULL, NULL, NULL )
/// @brief Asynchronously execute process.
/// @note
/// Process ID must be freed with any of the following functions:
/// - process_discard()
/// - process_wait()
/// - process_wait_timed() *process is only freed if it did not time out*
/// - process_kill()
/// @param      command               (Command)         Command to execute.
/// @param[out] out_pid               (Process*)        Pointer to write PID to.
/// @param[in]  opt_working_directory (Path*)           Path to execute process in.
/// @param[in]  opt_environment       (EnvironmentBuf*) Environment variables to execute with.
/// @param[in]  opt_stdin             (PipeRead*)       Pipe for sub-process stdin.
/// @param[in]  opt_stdout            (PipeWrite*)      Pipe for sub-process stdout.
/// @param[in]  opt_stderr            (PipeWrite*)      Pipe for sub-process stderr.
/// @return
///     - @c true  : Successfully started sub-process.
///     - @c false : Failed to start sub-process.
#define process_exec_async( ... ) \
    _internal_process_exec_async( __VA_ARGS__, NULL, NULL, NULL, NULL, NULL )
/// @brief Free process ID.
/// @param[in] pid Pointer to process ID to discard.
attr_core_api
void process_discard( Process* pid );
/// @brief Wait for process to exit.
/// @param[in] pid Pointer to process ID.
/// @return
///     - @c -2    : Invalid process ID.
///     - @c -1    : Sub-process exited abnormally.
///     - @c 0-255 : Exit code of sub-process.
attr_core_api
int  process_wait( Process* pid );
/// @brief Wait for process to exit.
/// @param[in]  pid               Pointer to process ID.
/// @param      msec              Number of milliseconds to wait. U32_MAX for infinite wait.
/// @param[out] opt_out_exit_code Pointer to write exit code to.
/// @return
///     - @c true  : Sub-process exited within specified time.
///     - @c false : Sub-process timed out.
attr_core_api
b32  process_wait_timed( Process* pid, u32 msec, int* opt_out_exit_code );
/// @brief Force shutdown of sub-process.
/// @param[in] pid Pointer to process ID.
attr_core_api
void process_kill( Process* pid );

/// @brief Search in PATH for process.
/// @param process_name Name of process to search for.
/// @return
///     - @c true  : Process found in PATH.
///     - @c false : Failed to find process.
attr_core_api
b32 process_find( struct _StringPOD process_name );

// NOTE(alicia): implementation -----------------------------------------------

attr_core_api
int _internal_process_exec(
    Command                command,
    const _PathPOD*        opt_working_directory,
    const EnvironmentBuf*  opt_environment,
    const PipeRead*        opt_stdin,
    const PipeWrite*       opt_stdout,
    const PipeWrite*       opt_stderr,
    ... );
attr_core_api
b32 _internal_process_exec_async(
    Command                command,
    Process*               out_pid,
    const _PathPOD*        opt_working_directory,
    const EnvironmentBuf*  opt_environment,
    const PipeRead*        opt_stdin,
    const PipeWrite*       opt_stdout,
    const PipeWrite*       opt_stderr,
    ... );

attr_header
void command_buf_initialize(
    usize              command_cap,
    usize              text_cap,
    struct _StringPOD* command_buf,
    char*              text_buf,
    CommandBuf*        out_buf
) {
    out_buf->cap      = command_cap;
    out_buf->len      = 0;
    out_buf->buf      = command_buf;
    out_buf->text.cap = text_cap;
    out_buf->text.len = 0;
    out_buf->text.buf = text_buf;
}

attr_header
b32 _internal_command_buf_append(
    struct AllocatorInterface* allocator,
    CommandBuf*                buf, ...
) {
    va_list va;
    va_start( va, buf );
    b32 result = command_buf_append_va( allocator, buf, va );
    va_end( va );
    return result;
}
attr_header
b32 _internal_command_buf_append_text(
    struct AllocatorInterface* allocator,
    CommandBuf*                buf, ...
) {
    va_list va;
    va_start( va, buf );
    const char* current = va_arg( va, const char* );
    while( current ) {
        struct _StringPOD arg;
        arg.len = cstr_len( current );
        arg.buf = (char*)current;
        if( !command_buf_append_arguments( allocator, buf, 1, &arg ) ) {
            va_end( va );
            return false;
        }
        current = va_arg( va, const char* );
    }
    va_end( va );
    return true;
}
attr_header
b32 _internal_command_buf_try_append( CommandBuf* buf, ... ) {
    va_list va;
    va_start( va, buf );
    b32 result = command_buf_try_append_va( buf, va );
    va_end( va );
    return result;
}
attr_header
b32 _internal_command_buf_try_append_text(
    CommandBuf* buf, ...
) {
    va_list va;
    va_start( va, buf );
    const char* current = va_arg( va, const char* );
    while( current ) {
        struct _StringPOD arg;
        arg.len = cstr_len( current );
        arg.buf = (char*)current;
        if( !command_buf_try_append_arguments( buf, 1, &arg ) ) {
            va_end( va );
            return false;
        }
        current = va_arg( va, const char* );
    }
    va_end( va );
    return true;
}
attr_header
void environment_buf_initialize(
    usize              key_value_pair_cap,
    usize              text_cap,
    struct _StringPOD* key_value_buf,
    char*              text_buf,
    EnvironmentBuf*    out_buf
) {
    out_buf->cap      = key_value_pair_cap;
    out_buf->len      = 0;
    out_buf->buf      = key_value_buf;
    out_buf->text.cap = text_cap;
    out_buf->text.len = 0;
    out_buf->text.buf = text_buf;
}


#endif /* header guard */
