# Todo List
- [ ] collections: queue atomic functions.
- [ ] collections: allocating functions with alignment.
- [ ] alloc: Block allocator.
- [ ] process.h: API for creating processes and pipes.
- [ ] platform: linux support
- [ ] simd abstractions
- [ ] msvc: no cstdlib
- [ ] memory: more memory allocation functions
- [ ] fs: memory mapped file?
- [ ] math: simd (trig, linear algebra)
- [ ] math: sine_cosine MASM (for compiling with MSVC)
- [ ] math: saturating,wrapping,checked add/sub/mul/div
- [ ] math: generics/function overloads(C++)
- [ ] math: RGB <-> HEX conversions
- [ ] docs:math: vec2/3/4 quat and mat2/3/4 C++ wrappers documentation
- [ ] fmt: more accurate float formatting (grisu3 or errol?)
## Bugs
- [ ] fs: win32: always convert path to wide path. (or enable UTF-8? but that requires specific Win10 version)
## unlikely
- [ ] finish writing C tests
- [ ] write C++ tests
## Complete
- [x] build: test flag --open for opening docs in browser
- [x] platform: win32: use internal_win32_log_error to send custom error message and message from FormatMessageA
- [x] fs: traverse directory.
- [x] fs: recursive directory remove
- [x] fs: directory query item count and directory is empty
- [x] time: complete rewrite.
- [x] fmt: rewrite
- [x] fmt: time formatting
- [x] fmt: Path formatting argument
    - / to change all slashes to forward
    - \ to change all slashes to back
- [x] lib:log: fix logging new line
- [x] lib:log: remove color formatting
- [x] math: endian conversions
- [x] math: is_power_of_two
- [x] math: RGB <-> HSL conversions
- [x] thread: more functions (thread_destroy?)
- [x] Makefile: generate compile_flags.txt

