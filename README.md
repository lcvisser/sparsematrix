# sparsematrix

[![CMake](https://github.com/lcvisser/sparsematrix/actions/workflows/cmake.yml/badge.svg)](https://github.com/lcvisser/sparsematrix/actions/workflows/cmake.yml)

This is a sparse matrix implementation in C++11. It is a header-only library.

A sparse matrix is a matrix that only has a few non-zero valued elements. When such matrices are big (many
rows/columns), it becomes is often advantagous from a memory usage perspective to only save these non-zero elements to
memory. This of course requires some overhead to keep track of where those nonzero elements are, for example when doing
addition or subtraction.

This implementation is very simple and uses a `std::map` to store elements and uses the standard library to efficiently
perform operations. It is a very basic implementation that is probably outperformed by more advanced libraries. Its
intended use is a quick solution for projects where only the basics are needed.


## Usage

### Create a matrix

Instantiate a matrix of any type and size. The first template argument is the number of rows, the second argument is
the number of columns. The third argument is the type (typically `int`, `float` or `double`, but you can use any type
that supports the basic math operators (actually, only addition, subtraction and multiplication is needed). The
instance does not allocate memory until elements are populated.

```
SparseMatrix<1, 2, int> s;
SparseMatrix<3, 5, float> t;
SparseMatrix<7, 15, double> u;
```

You can also use initializer lists:

```
SparseMatrix<2, 3, double> v = {
    { {0, 0}, 1 },
    { {0, 1}, 2 },
    { {1, 2}, 3 }
};
```

The list consists of key-value pairs, where the key is a pair with the (row, column) indices. Note that only three out
of six elements are used.

### Operations

Instances support the basic math operations addition, subtraction and multiplication. Also transposition (swapping rows
and columns) is supported. Inversion is not supported (yet). Dimensions are checked at compile time:

```
SparseMatrix<3, 5, float> s;
SparseMatrix<3, 5, float> t;
SparseMatrix<3, 5, float> u;

t = -s;
u = s + t;
u = s - t;

SparseMatrix<5, 7, float> v;
SparseMatrix<3, 7, float> w;

w = s * v;
w = v * s;  // error, dimension mismatch
w = s * t;  // error, dimension mismatch
```

See the provided example and the tests for more usage guidelines.


## Building the example and tests

CMake presets are defined for GCC, but they should work with clang as well.

```
cmake --preset <preset>
cmake --build --preset <preset>
```

Currently defined presets:

 - `default-configure-coverage`: build with coverage

 `ctest` uses a bash script that expects that `$CC`, `$CXX` and `$GCOV` environment variables are set. See the provided
 `.env` files for sensible defaults (for most Linux distributions and MacOS).


## Generating documentation

The code is documented according to Doxygen standards. A basic `Doxyfile` is provided to allow you to generate the
documentation (builds html and LaTeX).
