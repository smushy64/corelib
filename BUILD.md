Build from Source
=================

## Tools Required

### General

- clang, gcc or msvc (Windows only)
- [Doxygen >= 1.9.7](https://www.doxygen.nl/) (for generating documentation)

### Windows

- [MinGW](https://www.mingw-w64.org/)
- [Visual Studio](https://visualstudio.microsoft.com/) (for MSVC compiler and MASM, optional)
- [Latest Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/)

## Dependencies

### Posix

- C standard library

## Steps

1) cd into root directory

2) compile cbuild.c with desired compiler:
```console
clang cbuild.c -o cbuild
```

3) compile project for current platform and architecture:
```console
./cbuild build
```

By default, result will be in ./build

to compile using a compiler different from the one used to compile cbuild:
```console
./cbuild build --compiler=gcc
```
**compiler** being either clang, gcc, cc(for default compiler, usually gcc) or msvc on windows.

@note if compiling with msvc, you need to run vcvarsall.bat before running cbuild to have cl and masm in PATH.

to compile and run tests:
```console
./cbuild test
```
to generate documentation:
```console
./cbuild docs
```

additional build flags can be obtained with:
```console
./cbuild help
```

