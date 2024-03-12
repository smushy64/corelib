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

- Makefile: ```$(TARGET)``` now depends on ```$(OUTPUT_OBJ_PATH)``` instead of ```$(OUTPUT_PATH)```.
- Makefile: added linux build. NOTE: linux build is not complete.
- lib: added core_command_line function for getting command line when project was built.
- memory: memory_free now checks for null pointer so platform implementation doesn't have to.
- docs:memory: added warning about realloc not returning the same base pointer.
- sync: added semaphore and mutex sizes.
- test: added build description and command line print-out before tests are run.
- Makefile: test now cd's into build path before running test.
- Makefile: added lsp target for generating compile_commands.txt

0.1.0
------
- First version.

