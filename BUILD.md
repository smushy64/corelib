Build from Source
=================

## Tools Required

### General

- GNU Make
- clang or gcc
- [Doxygen >= 1.9.7](https://www.doxygen.nl/) (for generating documentation)

### Windows

- [MinGW](https://www.mingw-w64.org/)
- [Visual Studio](https://visualstudio.microsoft.com/) (for MSVC compiler and MASM, optional)

## Dependencies

### Posix

- C standard library

## Steps

1) cd into root directory

2) to compile for current platform and architecture:
```console
make
```

By default, library will be in ./build/

to compile using another compiler:
```console
make COMPILER=compiler
```
**compiler** being either clang, gcc or msvc

@note if compiling with msvc, you need to run vcvarsall.bat before running make to have cl and masm in PATH.

to compile and run tests:
```console
make test
```
to generate documentation:
```console
make docs
```

additional build instructions can be obtained with:
```console
make help
```

