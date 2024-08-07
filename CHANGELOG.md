Changelog
=========

Major version 0 means library is in development so anything
might change, including breaking changes.

## Major version
Number between 0-65535.

Indicates breaking changes to API or implementation.

## Minor version
Number between 0-255.

Indicates implementation changes or bug fixes that affect behavior.

## Patch version
Number between 0-255

Indicates additions to API, minor bug fixes and other small changes.

0.1.2
-----

- msvc: applied required changes to implementation.
- msvc: added required flags to compile project with msvc.
- test.c: removed old tests again.
- cbuild: first version written, only supports windows mingw clang and gcc currently.
- README.md: removed GNU Make requirement.
- BUILD.md: now shows instructions for using cbuild.
- gitignore: added cbuild.exe
- Doxygen_default: made sure to ignore cbuild header.
- Makefile: removed in favor of using cbuild (build system library written by me :) )
- path: removed file functions.
- string: added utf functions.
- string: rewrote string_is_empty macro.
- platform_dllmain: removed in favor of platform_sharedmain and platform_staticmain.
- platform: applied changes from fs.
- platform/win32: applied changes from fs.
- fs: rewrote all file functions.
- fs: added anonymous pipes.
- print: now uses pipes.
- print: renamed CONSOLE_COLOR_* to ANSI_COLOR_*
- print: removed str_* in favor of color_text macro.
- Makefile: sources are no longer generated, they need to be added manually.
- impl/string: rewrote cstr_len again.
- impl/string: added cstr_cmp.
- assertions: rewrote macros to allow debug message printing before crashing.
- attributes: added attr_no_return.
- fmt: made stream_fmt a library function so it can be used as a function pointer.
- fmt: bugfix: boolean arguments now properly initialized.
- impl/lib: removed unused includes.
- impl/string: applied assertions changes.
- types: renamed codepoint to rune32.
- types: added va_copy macro.
- string: added memory_set forward declaration.
- system: feature flags are now an enum.
- alloc: Implemented heap and stack functions.
- alloc: Renamed AllocStack to StackAllocator and renamed functions accordingly.
- alloc: Added AllocatorInterface structure and heap allocator interface.
- collections: applied changes from slice.h
- collections: generic_array_ functions.
- collections: removed List, replaced with darray.
- collections: rewrote queue.
- collections: rewrote hashmap.
- cstdlib: bug: memmove broken when dst is before src.
- docs: fixed warnings.
- fmt: applied changes from string.h
- fmt: rewrote API
- fmt: added path and time specifiers
- fs: removed fs_directory_delete()
- job_queue: applied changes from sync.h
- job_queue: applied changes from thread.h
- macros/assertions: moved assertion macros from core/macros.h to core/assertions.h
- macros: added struct_literal() macro for defining struct literals.
- memory: renamed memory_copy_overlapped to memory_move.
- path: applied changes from string.h
- path: rewrote many functions. path_buf_* functions now use AllocatorInterface.
- platform: applied changes from thread.h
- platform: applied changes from time.h
- platform/win32: applied changes from sync.h
- platform/win32: applied changes from string.h
- platform/win32: removed platform_directory_delete()
- platform/win32: applied changes from thread.h
- platform/win32: applied changes from time.h
- slice: renamed CoreSlice/Buffer to ByteSlice/Buffer
- slice: renamed GenericSlice/Buffer to ItemSlice/Buffer
- string: applied changes from slice.h
- string: rewrote most functions. removed SIMD "optimizations" for now.
- string: renamed asciiz_len to cstr_len. renamed char_* functions to ascii_*.
- string: string buf allocating functions now use AllocatorInterface.
- sync: renamed interlocked_* functions to atomic_* and simplified mutex/semaphore api.
- sync: renamed _fence functions to _barrier
- test: deleted old tests and replaced with only string tests for now.
- test: added collections tests
- test: added time tests
- thread: rewrote thread_create(), thread_destroy() and thread_exit_code()
- thread: added thread_free(), thread_join_timed() and thread_join()
- thread: Thread is now struct ThreadHandle
- time: rewrote all functions.
- time: streaming formatting implemented.
- types: added 128-bit integers, atomic integers, hash integers, bsize and codepoints.
- Makefile: bumped version to 0.1.2

0.1.1
-----

- time: changed time_query_ticks and time_query_ticks_per_second to functions for querying seconds and milliseconds.
- alloc: added alloc.h/.c for memory allocators.
- alloc: AllocStack stack allocator implemented.
- collections: added packed_bool_* macro functions for creating and manipulating packed boolean arrays.
- fs: win32: no longer declares GetStdHandle, platform win32 handles that.
- fs: added fs_file_write_fmt_* functions for writing formatted text to file.
- path: bug: path_file_stem() cut off last character of file stem
- path: docs: added to documentation for path_buf_set_extension() and path_extension()
- path: redefined path_text so that it can be used to initialize compile-time constants.
- lib: added core_command_line function for getting command line when project was built.
- lib: simplified logging and removed colorization and pre/suffixes from log messages.
- math:vector2/3: no longer forward declares arc_cosine
- math:mat4: bug: m4_view returning matrix with 0 in cell 15, should be 1
- memory: memory_free now checks for null pointer so platform implementation doesn't have to.
- memory: library now exports custom memcpy, memmove and memset so code that depends on corelib doesn't have to depend on cstdlib.
- memory: added memory_calculate_aligned_size() for calculating the actual size of aligned memory allocations.
- platform_dllmain:win32: added extern "C" and redefined NULL to nullptr so that it compiled in C++
- platform_win32: internal_win32_log_error now heap allocates buffer using FormatMessageA
- string: redefined string_text so that it can be used to initialize compile-time constants.
- string: bug: asciiz_len included null-terminator.
- string: bug: trim_*_whitespace functions not trimming properly
- string: bug: string_split out_last contains split index!
- string: added string_trim_whitespace, trims leading and trailing whitespace at the same time.
- sync: added semaphore and mutex sizes.
- Makefile: ```$(TARGET)``` now depends on ```$(OUTPUT_OBJ_PATH)``` instead of ```$(OUTPUT_PATH)```.
- Makefile: added linux build. NOTE: linux build is not complete.
- Makefile: test now cd's into build path before running test.
- Makefile: added lsp target for generating compile_commands.txt
- Makefile: project number generated for documentation now includes patch.
- docs:memory: added warning about realloc not returning the same base pointer.
- test: added build description and command line print-out before tests are run.
- test: test memmove/memory_copy_overlapped to make sure it still works properly.
- test: test string_trim_*_whitespace functions.
- test: added path tests.
- test: added core_logging_callback

0.1.0
------
- First version.

