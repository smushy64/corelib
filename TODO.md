# Todo List
- [ ] Makefile: no longer check for compiler when generating docs.
- [ ] platform: linux support
- [ ] simd abstractions
- [ ] msvc: no cstdlib
- [ ] memory: more memory allocation functions
- [ ] fs:platform: not reallocating when path needs to be null-terminated or full path is necessary.
- [ ] fs: traverse directory.
- [ ] fs: memory mapped file?
- [ ] math: simd (trig, linear algbra)
- [ ] math: sine_cosine MASM (for compiling with MSVC)
- [ ] math: saturating,wrapping,checked add/sub/mul/div
- [ ] math: generics/function overloads(C++)
- [ ] math: RGB <-> HEX conversions
- [ ] docs:math: vec2/3/4 quat and mat2/3/4 C++ wrappers documentation
- [ ] fmt: rewrite
- [ ] fmt: time formatting
- [ ] fmt: more accurate float formatting (grisu3 or errol?)
- [ ] fmt: Path formatting argument
    - / to change all slashes to forward
    - \ to change all slashes to back
## unlikely
- [ ] finish writing C tests
- [ ] write C++ tests
## Complete
- [x] math: endian conversions
- [x] math: is_power_of_two
- [x] math: RGB <-> HSL conversions
- [x] thread: more functions (thread_destroy?)
- [x] Makefile: generate compile_flags.txt

