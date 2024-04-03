Changelog
=========

## Major version
Number between 0-65535.

Indicates breaking changes to API or implementation.

## Minor version
Number between 0-255.

Indicates implementation changes or bug fixes that affect behavior.

## Patch version
Number between 0-255

Indicates additions to API, minor bug fixes and other small changes.

0.1.1
-----

- collections: added packed_bool_* macro functions for creating and manipulating packed boolean arrays.
- fs: win32: no longer declares GetStdHandle, platform win32 handles that.
- lib: added core_command_line function for getting command line when project was built.
- math:vector2/3: no longer forward declares arc_cosine
- math:mat4: bug: m4_view returning matrix with 0 in cell 15, should be 1
- memory: memory_free now checks for null pointer so platform implementation doesn't have to.
- memory: library now exports custom memcpy, memmove and memset so code that depends on corelib doesn't have to depend on cstdlib.
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

0.1.0
------
- First version.

