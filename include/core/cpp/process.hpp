#if !defined(CORE_CPP_PROCESS_HPP) && defined(__cplusplus)
#define CORE_CPP_PROCESS_HPP
/**
 * @file   process.hpp
 * @brief  C++ Process.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   June 01, 2025
*/
#if !defined(CORE_PROCESS_H)
    #include "core/process.h"
#endif
#include "core/string.h"

/// @brief Initialize allocated command builder.
/// @param      command_cap Max number of commands.
/// @param      text_cap    Max number of characters in text buf.
/// @param[in]  command_buf Pointer to buffer for command arguments.
/// @param[in]  text_buf    Pointer to buffer for command text buffer.
/// @param[out] out_buf     Pointer to write command buffer.
attr_header
void command_buf_initialize(
    usize       command_cap,
    usize       text_cap,
    String*     command_buf,
    char*       text_buf,
    CommandBuf* out_buf );
/// @brief Append arguments to command buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to command buffer.
/// @param     count     Number of arguments to append.
/// @param[in] arguments Pointer to arguments to append.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
attr_header
bool command_buf_append_arguments(
    AllocatorInterface* allocator,
    CommandBuf*         buf,
    usize               count,
    String*             arguments );
#undef command_buf_append
/// @brief Append arguments to command buffer.
/// @param[in] allocator Pointer to allocator interface.
/// @param[in] buf       Pointer to command buffer.
/// @param     args      Arguments.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
template<typename... Args> attr_header
bool command_buf_append( AllocatorInterface* allocator, CommandBuf* buf, Args... args );
#undef command_buf_append_text
/// @brief Append arguments to command buffer.
/// @param[in] allocator (AllocatorInterface*) Pointer to allocator interface.
/// @param[in] buf       (CommandBuf*)         Pointer to command buffer.
/// @param     ...       (const char*)         Arguments.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
#define command_buf_append_text( allocator, buf, ... ) \
    command_buf_append( allocator, buf, ##__VA_ARGS__ )
/// @brief Try to append arguments to command buffer.
/// @param[in] buf       Pointer to command buffer.
/// @param     count     Number of arguments to append.
/// @param[in] arguments Pointer to arguments to append.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
attr_header
bool command_buf_try_append_arguments(
    CommandBuf* buf,
    usize       count,
    String*     arguments );
#undef command_buf_try_append
/// @brief Try to append arguments to command buffer.
/// @param[in] buf  Pointer to command buffer.
/// @param     args Arguments.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
template<typename... Args> attr_header
bool command_buf_try_append( CommandBuf* buf, Args... args );
#undef command_buf_try_append_text
/// @brief Try to append arguments to command buffer.
/// @param[in] buf (CommandBuf*) Pointer to command buffer.
/// @param[in] ... (const char*) Arguments.
/// @return
///     - @c true  : Arguments were appended successfully.
///     - @c false : Failed to append arguments.
#define command_buf_try_append_text( buf, ... ) \
    command_buf_try_append( buf, ##__VA_ARGS__ )

/// @brief Initialize allocated environment buffer.
/// @param      key_value_pair_cap Max number of key/value pairs in key_value_buf.
/// @param      text_cap           Max number of characters in text_buf.
/// @param[in]  key_value_buf      Pointer to buffer for key/value pairs.
///                                  Must be able to hold key_value_pair_cap * 2 strings.
/// @param[in]  text_buf           Pointer to buffer for environment text buffer.
/// @param[out] out_buf            Pointer to write environment buffer.
attr_header
void environment_buf_initialize(
    usize           key_value_pair_cap,
    usize           text_cap,
    String*         key_value_buf,
    char*           text_buf,
    EnvironmentBuf* out_buf );

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
attr_header
bool environment_buf_add(
    AllocatorInterface* allocator,
    EnvironmentBuf*     buf,
    String              key,
    String              value );
#undef environment_buf_add_text
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
    environment_buf_add( allocator, buf, key, value )
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
attr_header
bool environment_buf_set(
    AllocatorInterface* allocator,
    EnvironmentBuf*     buf,
    String              key,
    String              value );
#undef environment_buf_set_text
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
    environment_buf_set( allocator, buf, key, value )
/// @brief Remove key/value pair in environment buffer.
/// @param[in] buf Pointer to environment buffer.
/// @param     key Key of key/value pair to remove.
attr_header
void environment_buf_remove( EnvironmentBuf* buf, String key );
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
attr_header
bool environment_buf_try_add( EnvironmentBuf* buf, String key, String value );
#undef environment_buf_try_add_text
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
    environment_buf_try_add( buf, key, value )
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
attr_header
bool environment_buf_try_set( EnvironmentBuf* buf, String key, String value );
#undef environment_buf_try_set_text
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

/// @brief Query value for given environment key.
/// @warning
/// The returned string should be used immediately after calling this function!
/// Otherwise, store it in a buffer.
/// @param key Key of environment variable.
/// @return Read-only value of environment variable. Returns empty string if not set.
attr_header
String environment_query( String key );
/// @brief Set environment value.
/// @param key   Key of environment variable.
/// @param value Value of environment variable.
/// @return
///     - @c true  : Successfully set environment variable.
///     - @c false : Failed to set environment variable.
attr_header
bool environment_set( String key, String value );

#undef process_exec
#undef process_exec_async
/// @brief Synchronously execute process.
/// @param     command               (Command)         Command to execute.
/// @param[in] opt_working_directory (Path*)           Path to execute process in.
/// @param[in] opt_environment       (EnvironmentBuf*) Environment variables to execute with.
/// @param[in] opt_stdin             (FD*)             Pipe for sub-process stdin.
/// @param[in] opt_stdout            (FD*)             Pipe for sub-process stdout.
/// @param[in] opt_stderr            (FD*)             Pipe for sub-process stderr.
/// @return
///     - @c -2    : Failed to execute process.
///     - @c -1    : Sub-process executed but exited abnormally.
///     - @c 0-255 : Exit code of sub-process.
attr_header
int process_exec(
    Command               command,
    const String*         opt_working_directory = nullptr,
    const EnvironmentBuf* opt_environment       = nullptr,
    const FD*             opt_stdin             = nullptr,
    const FD*             opt_stdout            = nullptr,
    const FD*             opt_stderr            = nullptr );
/// @brief Asynchronously execute process.
/// @note
/// Process ID must be freed with any of the following functions:
/// - process_discard()
/// - process_wait()
/// - process_wait_timed() *process is only freed if it did not time out*
/// - process_kill()
/// @param      command               (Command)         Command to execute.
/// @param[out] out_pid               (Process*)        Pointer to write PID to.
/// @param[in]  opt_working_directory (String*)         Path to execute process in.
/// @param[in]  opt_environment       (EnvironmentBuf*) Environment variables to execute with.
/// @param[in]  opt_stdin             (FD*)             Pipe for sub-process stdin.
/// @param[in]  opt_stdout            (FD*)             Pipe for sub-process stdout.
/// @param[in]  opt_stderr            (FD*)             Pipe for sub-process stderr.
/// @return
///     - @c true  : Successfully started sub-process.
///     - @c false : Failed to start sub-process.
attr_header
bool process_exec_async(
    Command               command,
    Process*              out_pid,
    const String*         opt_working_directory = nullptr,
    const EnvironmentBuf* opt_environment       = nullptr,
    const FD*             opt_stdin             = nullptr,
    const FD*             opt_stdout            = nullptr,
    const FD*             opt_stderr            = nullptr );
/// @brief Wait for process to exit.
/// @param[in]  pid  Pointer to process ID.
/// @param      msec Number of milliseconds to wait. U32_MAX for infinite wait.
/// @return
///     - @c true  : Sub-process exited within specified time.
///     - @c false : Sub-process timed out.
attr_header
bool process_wait_timed( Process* pid, u32 msec );

/// @brief Search in PATH for process.
/// @param process_name Name of process to search for.
/// @return
///     - @c true  : Process found in PATH.
///     - @c false : Failed to find process.
attr_header
bool process_find( String process_name );

// NOTE(alicia): implementation -----------------------------------------------

attr_header
void command_buf_initialize(
    usize       command_cap,
    usize       text_cap,
    String*     command_buf,
    char*       text_buf,
    CommandBuf* out_buf
) {
    return command_buf_initialize(
        command_cap, text_cap, (_StringPOD*)command_buf, text_buf, out_buf );
}
attr_header
bool command_buf_append_arguments(
    AllocatorInterface* allocator,
    CommandBuf*         buf,
    usize               count,
    String*             arguments
) {
    return command_buf_append_arguments( allocator, buf, count, (_StringPOD*)arguments );
}
attr_header
bool command_buf_try_append_arguments(
    CommandBuf* buf,
    usize       count,
    String*     arguments
) {
    return command_buf_try_append_arguments( buf, count, (_StringPOD*)arguments );
}
attr_header
void environment_buf_initialize(
    usize           key_value_pair_cap,
    usize           text_cap,
    String*         key_value_buf,
    char*           text_buf,
    EnvironmentBuf* out_buf
) {
    return environment_buf_initialize(
        key_value_pair_cap, text_cap, (_StringPOD*)key_value_buf, text_buf, out_buf );
}
attr_header
bool environment_buf_add(
    AllocatorInterface* allocator,
    EnvironmentBuf*     buf,
    String              key,
    String              value
) {
    return environment_buf_add( allocator, buf, key.__pod, value.__pod );
}
attr_header
bool environment_buf_set(
    AllocatorInterface* allocator,
    EnvironmentBuf*     buf,
    String              key,
    String              value
) {
    return environment_buf_set( allocator, buf, key.__pod, value.__pod );
}
attr_header
void environment_buf_remove( EnvironmentBuf* buf, String key ) {
    environment_buf_remove( buf, key.__pod );
}
attr_header
bool environment_buf_try_add( EnvironmentBuf* buf, String key, String value ) {
    return environment_buf_try_add( buf, key.__pod, value.__pod );
}
attr_header
bool environment_buf_try_set( EnvironmentBuf* buf, String key, String value ) {
    return environment_buf_try_set( buf, key.__pod, value.__pod );
}
attr_header
String environment_query( String key ) {
    return environment_query( key.__pod );
}
attr_header
bool environment_set( String key, String value ) {
    return environment_set( key.__pod, value.__pod );
}
attr_header
bool process_wait_timed( Process* pid, u32 msec ) {
    return process_wait_timed( pid, msec, nullptr );
}
attr_header
bool process_find( String process_name ) {
    return process_find( process_name.__pod );
}
template<typename... Args> attr_header attr_always_inline attr_hot
bool command_buf_append( AllocatorInterface* allocator, CommandBuf* buf, Args... args ) {
    String strings[sizeof...(args)] = { args... };
    return command_buf_append_arguments( allocator, buf, sizeof...(args), strings );
}
template<typename... Args> attr_header attr_always_inline attr_hot
bool command_buf_try_append( CommandBuf* buf, Args... args ) {
    String strings[sizeof...(args)] = { args... };
    return command_buf_try_append_arguments( buf, sizeof...(args), strings );
}
attr_header
int process_exec(
    Command               command,
    const String*         opt_working_directory,
    const EnvironmentBuf* opt_environment,
    const FD*             opt_stdin,
    const FD*             opt_stdout,
    const FD*             opt_stderr           
) {
    return _internal_process_exec(
        command, (_StringPOD*)opt_working_directory,
        opt_environment, opt_stdin, opt_stdout, opt_stderr );
}
attr_header
bool process_exec_async(
    Command               command,
    Process*              out_pid,
    const String*         opt_working_directory,
    const EnvironmentBuf* opt_environment,
    const FD*             opt_stdin,
    const FD*             opt_stdout,
    const FD*             opt_stderr
) {
    return _internal_process_exec_async(
        command, out_pid,
        (_StringPOD*)opt_working_directory,
        opt_environment, opt_stdin, opt_stdout, opt_stderr );
}

#endif /* header guard */
