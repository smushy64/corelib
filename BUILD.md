Build from Source
=================

## Tools Required

### General

- clang or gcc
- [Doxygen >= 1.9.7](https://www.doxygen.nl/) (for generating documentation)

### Windows

- [MinGW](https://www.mingw-w64.org/)
- [Latest Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/)

## Dependencies

### Posix

- libc

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
**compiler** being clang, gcc or cc(for default compiler, usually gcc).

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

