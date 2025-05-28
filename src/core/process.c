/**
 * @file   process.c
 * @brief  Functions for starting sub-processes.
 * @author Alicia Amarilla (smushyaa@gmail.com)
 * @date   May 26, 2025
*/
#include "core/process.h"
#include "core/alloc.h"
#include "core/internal/platform/process.h"

attr_core_api
b32 command_buf_append_arguments(
    AllocatorInterface* allocator,
    CommandBuf*         buf,
    usize               count,
    struct _StringPOD*  arguments
) {
    if( !count ) {
        return true;
    }

    if( (buf->cap - buf->len) < count ) {
        usize new_cap = buf->cap + count + 4;
        void* ptr     = allocator_realloc(
            allocator, buf->buf, sizeof(String) * buf->cap, sizeof(String) * new_cap );

        if( !ptr ) {
            return false;
        }

        buf->cap = new_cap;
        buf->buf = ptr;
    }

    usize total_size = 0;
    for( usize i = 0; i < count; ++i ) {
        total_size += arguments[i].len + 1;
    }
    if( buf->text.buf ) {
        if( string_buf_remaining( buf->text ) < total_size ) {
            // NOTE(alicia): convert string pointers to offsets.
            for( usize i = 0; i < buf->len; ++i ) {
                buf->buf[i]._void = (void*)((usize)(buf->buf[i].buf) - (usize)(buf->text.buf));
            }

            usize realloc_size = total_size - string_buf_remaining( buf->text );

            b32 realloc_result = string_buf_grow( allocator, &buf->text, realloc_size );

            // NOTE(alicia): convert string offset back to pointers.
            for( usize i = 0; i < buf->len; ++i ) {
                buf->buf[i]._void = (void*)((usize)(buf->text.buf) + (usize)(buf->buf[i].buf));
            }

            if( !realloc_result ) {
                return false;
            }
        }
    } else {
        if( !string_buf_from_alloc( allocator, total_size, &buf->text ) ) {
            return false;
        }
    }

    for( usize i = 0; i < count; ++i ) {
        String arg     = arguments[i];
        String new_arg = {};

        new_arg.len = arg.len;
        new_arg.buf = buf->text.buf + buf->text.len;

        string_buf_try_append( &buf->text, arg );
        string_buf_try_push( &buf->text, 0 );

        buf->buf[buf->len++] = new_arg;
    }

    return true;
}
attr_core_api
b32 command_buf_append_va(
    AllocatorInterface* allocator,
    CommandBuf*         buf,
    va_list             va
) {
    String current = va_arg( va, String );
    while( current.len ) {
        if( !command_buf_append_arguments( allocator, buf, 1, &current ) ) {
            return false;
        }

        current = va_arg( va, String );
    }
    return true;
}
attr_core_api
b32 command_buf_try_append_arguments(
    CommandBuf*        buf,
    usize              count,
    struct _StringPOD* arguments
) {
    if( !count ) {
        return true;
    }

    if( (buf->cap - buf->len) < count ) {
        return false;
    }

    usize total_size = 0;
    for( usize i = 0; i < count; ++i ) {
        total_size += arguments[i].len + 1;
    }

    if( string_buf_remaining( buf->text ) < total_size ) {
        return false;
    }

    for( usize i = 0; i < count; ++i ) {
        String arg     = arguments[i];
        String new_arg = {};

        new_arg.len = arg.len;
        new_arg.buf = buf->text.buf + buf->text.len;

        string_buf_try_append( &buf->text, arg );
        string_buf_try_push( &buf->text, 0 );

        buf->buf[buf->len++] = new_arg;
    }

    return true;
}
attr_core_api
b32 command_buf_try_append_va( CommandBuf* buf, va_list va ) {
    String current = va_arg( va, String );
    while( current.len ) {
        if( !command_buf_try_append_arguments( buf, 1, &current ) ) {
            return false;
        }

        current = va_arg( va, String );
    }
    return true;
}
attr_core_api
void command_buf_reset( CommandBuf* buf ) {
    buf->len      = 0;
    buf->text.len = 0;
}
attr_core_api
void command_buf_free( AllocatorInterface* allocator, CommandBuf* buf ) {
    if( !buf ) {
        return;
    }

    if( buf->buf ) {
        allocator_free( allocator, buf->buf, sizeof(String) * buf->cap );
    }
    string_buf_free( allocator, &buf->text );

    memory_zero( buf, sizeof(*buf) );
}
attr_core_api
b32 environment_buf_add(
    AllocatorInterface* allocator,
    EnvironmentBuf*     buf,
    struct _StringPOD   key,
    struct _StringPOD   value
) {
    if( !(buf->cap - buf->len) ) {
        usize new_cap = buf->cap + 4;
        void* ptr = allocator_realloc(
            allocator, buf->buf, sizeof(String) * 2 * buf->cap, sizeof(String) * 2 * new_cap );

        if( !ptr ) {
            return false;
        }

        buf->cap = new_cap;
        buf->buf = ptr;
    }

    // NOTE(alicia): search for key and change it if it already exists
    isize pair_index = -1;
    for( usize pair = 0; pair < (buf->len * 2); pair += 2 ) {
        if( string_cmp( buf->buf[pair + 0], key ) ) {
            pair_index = pair;
            break;
        }
    }

    usize total_size;
    if( pair_index >= 0 ) {
        total_size = value.len + 1;
    } else {
        total_size = key.len + value.len + 2;
    }
    if( buf->text.buf ) {
        if( string_buf_remaining( buf->text ) < total_size ) {
            // NOTE(alicia): convert string pointers to offsets.
            for( usize i = 0; i < (buf->len * 2); ++i ) {
                buf->buf[i]._void = (void*)((usize)(buf->buf[i].buf) - (usize)(buf->text.buf));
            }

            usize realloc_size = total_size - string_buf_remaining( buf->text );
            b32 realloc_result = string_buf_grow( allocator, &buf->text, realloc_size );

            // NOTE(alicia): convert string offset back to pointers.
            for( usize i = 0; i < (buf->len * 2); ++i ) {
                buf->buf[i]._void = (void*)((usize)(buf->text.buf) + (usize)(buf->buf[i].buf));
            }

            if( !realloc_result ) {
                return false;
            }
        }
    } else {
        if( !string_buf_from_alloc( allocator, total_size, &buf->text ) ) {
            return false;
        }
    }

    if( pair_index >= 0 ) {
        String new_value;
        new_value.len = value.len;

        if( (value.len + 1) <= (buf->buf[pair_index + 1].len + 1) ) {
            new_value.buf = memory_copy( buf->buf[pair_index + 1].buf, value.buf, value.len );
            new_value.buf[new_value.len] = 0;
        } else {
            new_value.buf = buf->text.buf + buf->text.len;

            string_buf_try_append( &buf->text, value );
            string_buf_try_push( &buf->text, 0 );
        }

        buf->buf[pair_index + 1] = new_value;
    } else {
        String new_key, new_value;
        new_key.len = key.len;
        new_key.buf = buf->text.buf + buf->text.len;

        string_buf_try_append( &buf->text, key );
        string_buf_try_push( &buf->text, 0 );

        new_value.len = value.len;
        new_value.buf = buf->text.buf + buf->text.len;

        string_buf_try_append( &buf->text, value );
        string_buf_try_push( &buf->text, 0 );

        usize write_index = buf->len * 2;

        buf->buf[write_index + 0] = new_key;
        buf->buf[write_index + 1] = new_value;

        buf->len++;
    }

    return true;
}
attr_core_api
b32 environment_buf_set(
    AllocatorInterface* allocator,
    EnvironmentBuf*     buf,
    struct _StringPOD   key,
    struct _StringPOD   value
) {
    if( !buf->buf || !buf->text.buf ) {
        return false;
    }

    isize pair_index = -1;
    for( usize pair = 0; pair < (buf->len * 2); pair += 2 ) {
        if( string_cmp( buf->buf[pair + 0], key ) ) {
            pair_index = pair;
            break;
        }
    }

    if( pair_index < 0 ) {
        return false;
    }

    String* existing_value = buf->buf + (pair_index + 1);

    usize total_size = value.len + 1;
    if( (value.len + 1) <= (existing_value->len + 1) ) {
        total_size = (existing_value->len - value.len) + 1;
    }

    if( string_buf_remaining( buf->text ) < total_size ) {
        // NOTE(alicia): convert string pointers to offsets.
        for( usize i = 0; i < buf->len; ++i ) {
            buf->buf[i]._void = (void*)((usize)(buf->buf[i].buf) - (usize)(buf->text.buf));
        }

        usize realloc_size = total_size - string_buf_remaining( buf->text );
        b32 realloc_result = string_buf_grow( allocator, &buf->text, realloc_size );

        // NOTE(alicia): convert string offset back to pointers.
        for( usize i = 0; i < buf->len; ++i ) {
            buf->buf[i]._void = (void*)((usize)(buf->text.buf) + (usize)(buf->buf[i].buf));
        }

        if( !realloc_result ) {
            return false;
        }
    }

    existing_value->len = value.len;
    if( (value.len + 1) <= (existing_value->len + 1) ) {
        memory_copy( existing_value->buf, value.buf, value.len );
        existing_value->buf[existing_value->len] = 0;
    } else {
        existing_value->buf = buf->text.buf + buf->text.len;
        string_buf_try_append( &buf->text, value );
        string_buf_try_push( &buf->text, 0 );
    }

    return true;
}
attr_core_api
void environment_buf_remove(
    EnvironmentBuf* buf, struct _StringPOD key
) {
    isize pair_index = -1;
    for( usize pair = 0; pair < (buf->len * 2); pair += 2 ) {
        if( string_cmp( buf->buf[pair + 0], key ) ) {
            pair_index = pair;
            break;
        }
    }
    if( pair_index < 0 ) {
        return;
    }

    if( buf->len >= 1 ) {
        buf->buf[pair_index + 0] = buf->buf[(buf->len * 2) - 2];
        buf->buf[pair_index + 1] = buf->buf[(buf->len * 2) - 1];

        buf->len--;
    } else {
        buf->len = 0;
    }
}
attr_core_api
b32 environment_buf_try_add(
    EnvironmentBuf*   buf,
    struct _StringPOD key,
    struct _StringPOD value
) {
    if( !(buf->cap - buf->len) || !buf->text.buf ) {
        return false;
    }

    // NOTE(alicia): search for key and change it if it already exists
    isize pair_index = -1;
    for( usize pair = 0; pair < (buf->len * 2); pair += 2 ) {
        if( string_cmp( buf->buf[pair + 0], key ) ) {
            pair_index = pair;
            break;
        }
    }

    usize total_size;
    if( pair_index >= 0 ) {
        String* existing_value = buf->buf + (pair_index + 1);
        if( (value.len + 1) <= (existing_value->len + 1) ) {
            total_size = (existing_value->len - value.len) + 1;
        } else {
            total_size = value.len + 1;
        }
    } else {
        total_size = key.len + value.len + 2;
    }
    if( string_buf_remaining( buf->text ) < total_size ) {
        return false;
    }

    if( pair_index >= 0 ) {
        String* existing_value = buf->buf + (pair_index + 1);
        String new_value;
        new_value.len = value.len;

        if( (value.len + 1) <= (existing_value->len + 1) ) {
            new_value.buf = memory_copy( existing_value->buf, value.buf, value.len );
            new_value.buf[new_value.len] = 0;
        } else {
            new_value.buf = buf->text.buf + buf->text.len;

            string_buf_try_append( &buf->text, value );
            string_buf_try_push( &buf->text, 0 );
        }

        buf->buf[pair_index + 1] = new_value;
    } else {
        String new_key, new_value;
        new_key.len = key.len;
        new_key.buf = buf->text.buf + buf->text.len;

        string_buf_try_append( &buf->text, key );
        string_buf_try_push( &buf->text, 0 );

        new_value.len = value.len;
        new_value.buf = buf->text.buf + buf->text.len;

        string_buf_try_append( &buf->text, value );
        string_buf_try_push( &buf->text, 0 );

        usize write_index = buf->len * 2;

        buf->buf[write_index + 0] = new_key;
        buf->buf[write_index + 1] = new_value;

        buf->len++;
    }

    return true;
}
attr_core_api
b32 environment_buf_try_set(
    EnvironmentBuf*   buf,
    struct _StringPOD key,
    struct _StringPOD value
) {
    if( !buf->buf || !buf->text.buf ) {
        return false;
    }

    isize pair_index = -1;
    for( usize pair = 0; pair < (buf->len * 2); pair += 2 ) {
        if( string_cmp( buf->buf[pair + 0], key ) ) {
            pair_index = pair;
            break;
        }
    }

    if( pair_index < 0 ) {
        return false;
    }

    String* existing_value = buf->buf + (pair_index + 1);

    usize total_size = value.len + 1;
    if( (value.len + 1) <= (existing_value->len + 1) ) {
        total_size = (existing_value->len - value.len) + 1;
    }

    if( string_buf_remaining( buf->text ) < total_size ) {
        return false;
    }

    existing_value->len = value.len;
    if( (value.len + 1) <= (existing_value->len + 1) ) {
        memory_copy( existing_value->buf, value.buf, value.len );
        existing_value->buf[existing_value->len] = 0;
    } else {
        existing_value->buf = buf->text.buf + buf->text.len;
        string_buf_try_append( &buf->text, value );
        string_buf_try_push( &buf->text, 0 );
    }

    return true;
}
attr_core_api
void environment_buf_reset( EnvironmentBuf* buf ) {
    buf->len      = 0;
    buf->text.len = 0;
}
attr_core_api
void environment_buf_free( AllocatorInterface* allocator, EnvironmentBuf* buf ) {
    if( !buf ) {
        return;
    }
    if( buf->buf ) {
        allocator_free( allocator, buf->buf, sizeof(String) * 2 * buf->len );
    }
    string_buf_free( allocator, &buf->text );
    memory_zero( buf, sizeof(*buf) );
}
attr_core_api
struct _StringPOD environment_query( struct _StringPOD key ) {
    return platform_environment_query( key );
}
attr_core_api
b32 environment_set( struct _StringPOD key, struct _StringPOD value ) {
    return platform_environment_set( key, value );
}
attr_core_api
int _internal_process_exec(
    Command                command,
    const _PathPOD*        opt_working_directory,
    const EnvironmentBuf*  opt_environment,
    const PipeRead*        opt_stdin,
    const PipeWrite*       opt_stdout,
    const PipeWrite*       opt_stderr,
    ...
) {
    Process pid;
    if( !process_exec_async(
        command, &pid, opt_working_directory,
        opt_environment, opt_stdin, opt_stdout, opt_stderr
    ) ) {
        return -2;
    }
    return process_wait( &pid );
}
attr_core_api
b32 _internal_process_exec_async(
    Command                command,
    Process*               out_pid,
    const _PathPOD*        opt_working_directory,
    const EnvironmentBuf*  opt_environment,
    const PipeRead*        opt_stdin,
    const PipeWrite*       opt_stdout,
    const PipeWrite*       opt_stderr,
    ...
) {
    return platform_process_exec_async(
        command, out_pid, opt_working_directory,
        opt_environment, opt_stdin, opt_stdout, opt_stderr );
}
attr_core_api
void process_discard( Process* pid ) {
    platform_process_discard( pid );
}
attr_core_api
int process_wait( Process* pid ) {
    return platform_process_wait( pid );
}
attr_core_api
b32 process_wait_timed( Process* pid, u32 msec, int* opt_out_exit_code ) {
    return platform_process_wait_timed( pid, msec, opt_out_exit_code );
}
attr_core_api
void process_kill( Process* pid ) {
    platform_process_kill( pid );
}
attr_core_api
b32 process_find( struct _StringPOD process_name ) {
    return platform_process_find( process_name );
}


