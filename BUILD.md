Build from Source
=================

## Tools Required

### General

- clang or gcc
- [Doxygen >= 1.9.7](https://www.doxygen.nl/) (for generating documentation)

### Windows

- [MinGW](https://www.mingw-w64.org/) clang (preferred) or gcc
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

3) compile project for current platform:
```console
./cbuild build
```

By default, result will be in ./build/<platform-name>

to compile using a different compiler:
```console
./cbuild build -compiler gcc
```
**compiler** being clang, gcc or gcc-mingw32 if cross-compiling to Windows.

to compile and run tests:
```console
./cbuild test
```

to generate documentation:
```console
./cbuild docs
```

to generate clangd files:
```console
./cbuild lsp
```

to generate clangd flags for downstream project:
```console
./cbuild lsp -downstream
```
flags are output to stdout

to generate compilation flags for downstream project:
```console
./cbuild flags
```
flags are output to stdout

additional build options can be obtained with:
```console
./cbuild help
```

