# sparsematrix

[![CMake](https://github.com/lcvisser/sparsematrix/actions/workflows/cmake.yml/badge.svg)](https://github.com/lcvisser/sparsematrix/actions/workflows/cmake.yml)

This is a sparse matrix implementation in C++11. This is primarily a project for me to explore modern C++, but perhaps
can be useful for others.

## Building

CMake presets are defined for GCC, but they should work with clang as well.

```
cmake --preset <preset>
cmake --build --preset <preset>
```

Currently defined presets:

 - `default-configure-coverage`: build with coverage
