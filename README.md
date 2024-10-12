Smushy's Core Library
=====================
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

This library is a standard library replacement for my C projects.
It's a cross-platform library that provides math types & functions
(suitable for game development), string functions, io and multi-threading functions.

## Requirements
- clang, gcc or Visual Studio(cl and MASM)
- [MinGW](https://www.mingw-w64.org/) (only on Windows or if cross-compiling to it, when using clang or gcc)
- [Doxygen >= 1.9.7](https://www.doxygen.nl/) (for generating documentation)

## Limitations
- currently only win32 and linux are supported.
- wasm support is planned but not yet implemented.

## Links
- [Source Code.](https://github.com/smushy64/corelib)
- [Latest Release.](https://github.com/smushy64/corelib/releases/tag/v0.1.0)
- [Build steps and generating documentation.](./BUILD.md)
- [Changelog.](./CHANGELOG.md)
- [cbuild](https://github.com/smushy64/cbuild) (build system written in C, already included in project)

## External Code
- [Doxygen Awesome](https://jothepro.github.io/doxygen-awesome-css/) - Documentation theme.

