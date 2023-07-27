/* Copyright 2023 Ludo Visser

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "sparsematrix.h"


TEST_CASE_TEMPLATE("multiplication 2d cases", T, int, float, double)
{
    SUBCASE("matrix with vector multiplication")
    {
        SparseMatrix<2, 3, T> s = {
            { {0, 0}, 1 },
            { {0, 1}, 2 },
            { {0, 2}, 3 },
            { {1, 0}, 4 },
            { {1, 1}, 5 },
            { {1, 2}, 6 },
        };

        SparseMatrix<3, 1, T> t = {
            { {0, 0}, 7 },
            { {1, 0}, 8 },
            { {2, 0}, 9 }
        };

        SparseMatrix<2, 1, T> u = s * t;
        CHECK(u(0, 0) == 50);
        CHECK(u(1, 0) == 122);
    }

    SUBCASE("sparse matrix with vector multiplication")
    {
        SparseMatrix<2, 3, T> s = {
            { {0, 0}, 1 },
            { {1, 2}, 6 },
        };

        SparseMatrix<3, 1, T> t = {
            { {0, 0}, 7 },
            { {1, 0}, 8 },
            { {2, 0}, 9 }
        };

        SparseMatrix<2, 1, T> u = s * t;
        CHECK(u(0, 0) == 7);
        CHECK(u(1, 0) == 54);
    }

    SUBCASE("matrix with sparse vector multiplication")
    {
        SparseMatrix<2, 3, T> s = {
            { {0, 0}, 1 },
            { {0, 1}, 2 },
            { {0, 2}, 3 },
            { {1, 0}, 4 },
            { {1, 1}, 5 },
            { {1, 2}, 6 },
        };

        SparseMatrix<3, 1, T> t = {
            { {2, 0}, 9 }
        };

        SparseMatrix<2, 1, T> u = s * t;
        CHECK(u(0, 0) == 27);
        CHECK(u(1, 0) == 54);
    }

    SUBCASE("zero matrix with vector multiplication")
    {
        SparseMatrix<2, 3, T> s;

        SparseMatrix<3, 1, T> t = {
            { {0, 0}, 7 },
            { {1, 0}, 8 },
            { {2, 0}, 9 }
        };

        SparseMatrix<2, 1, T> u = s * t;
        CHECK(u.allocated() == 0);
        CHECK(u(0, 0) == 0);
        CHECK(u(1, 0) == 0);
    }

    SUBCASE("matrix with sparse vector multiplication")
    {
        SparseMatrix<2, 3, T> s = {
            { {0, 0}, 1 },
            { {0, 1}, 2 },
            { {0, 2}, 3 },
            { {1, 0}, 4 },
            { {1, 1}, 5 },
            { {1, 2}, 6 },
        };

        SparseMatrix<3, 1, T> t;

        SparseMatrix<2, 1, T> u = s * t;
        CHECK(u.allocated() == 0);
        CHECK(u(0, 0) == 0);
        CHECK(u(1, 0) == 0);
    }

    SUBCASE("zero matrix with zero vector multiplication")
    {
        SparseMatrix<2, 3, T> s;
        SparseMatrix<3, 1, T> t;

        SparseMatrix<2, 1, T> u = s * t;
        CHECK(u.allocated() == 0);
        CHECK(u(0, 0) == 0);
        CHECK(u(1, 0) == 0);
    }
}
