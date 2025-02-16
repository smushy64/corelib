# Todo List
- [ ] unicode:   more functions (like what ascii.h provides)
- [ ] hash:      continuous hashing function
- [ ] math:      noise functions
- [ ] time:      rename timer_* to time_*
- [ ] slice:     rewrite to match slice.hpp and strings
- [ ] math:      quat_from_angle_axis_vec3, from_angle_axis_combined, from_angle_axis (scalars)
- [ ] math:      simd (trig, linear algebra)
- [ ] math:      saturating,wrapping,checked add/sub/mul/div
- [ ] math:      RGB <-> HEX conversions
- [ ] math:      uvec2/3/4 dvec2/3/4
- [ ] math:docs: write docs for overloads and C++ vector/matrix/quaternion
- [ ] fs:        memory mapped file?
- [ ] fmt:       more accurate float formatting (grisu3 or errol?)
- [ ] memory:    more memory allocation functions
    - page protections, shared memory, allocate pages for example.
## New Features
- [ ] process.h: API for creating processes and pipes.
- [ ] search.h:  Search algorithms.
- [ ] socket.h:  Sockets and web connectivity.
- [ ] simd.h:    SIMD abstractions.
## Bugs
## unlikely
- [ ] finish writing C tests
- [ ] write C++ tests
## Complete
- [x] Remove collections
- [x] Remove job_queue
- [x] fs:posix:      directory_remove (recursive)
- [x] fmt:           custom formatter
- [x] platform:      linux support
- [x] fmt:           path to upper/lower.
- [x] fmt:           path convert separators.
- [x] math:          boolean vectors (bvec2/3/4)
- [x] fs:win32:      always convert path to wide path. (or enable UTF-8? but that requires specific Win10 version)
- [x] console:win32: SetConsoleOutputCP( CP_UTF8 ) for UTF-8 support.
- [x] math:          vector2 trig (radians/degrees/sin/cos/tan/asin/acos/atan/atan2)
- [x] math:          vector3 trig (radians/degrees/sin/cos/tan/asin/acos/atan/atan2)
- [x] math:          vector4 trig (radians/degrees/sin/cos/tan/asin/acos/atan/atan2)
- [x] math:          rewrite vec3/vec4 macro
- [x] math:          vector swizzling
- [x] math:          generics/function overloads(C++)
- [x] sync:          software/OS semaphore and mutex
    - anonymous semaphores and mutexes can be implemented with
    relatively fast atomic operations. since i haven't yet had a
    use for named semaphores/mutexes, it would be good to have
    separate structs for atomic and OS versions.
- [x] cpp:            move all C++ wrappers to their own headers
- [x] build:          test flag --open for opening docs in browser
- [x] platform:win32: use internal_win32_log_error to send custom error message and message from FormatMessageA
- [x] fs:             traverse directory.
- [x] fs:             recursive directory remove
- [x] fs:             directory query item count and directory is empty
- [x] time:           complete rewrite.
- [x] fmt:            rewrite
- [x] fmt:            time formatting
- [x] fmt:            Path formatting argument
    - / to change all slashes to forward
    - \ to change all slashes to back
- [x] lib:log:  fix logging new line
- [x] lib:log:  remove color formatting
- [x] math:     endian conversions
- [x] math:     is_power_of_two
- [x] math:     RGB <-> HSL conversions
- [x] thread:   more functions (thread_destroy?)
- [x] Makefile: generate compile_flags.txt

