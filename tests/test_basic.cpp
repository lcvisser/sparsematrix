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

TEST_CASE_TEMPLATE("create a matrix", T, int, float, double)
{
    SparseMatrix<2, 3, T> m = {
        { {0, 0}, 1 },
        { {0, 1}, 2 },
        { {0, 2}, 3 },
        { {1, 0}, 4 },
        { {1, 1}, 5 },
        { {1, 2}, 6 },
    };

    CHECK(m(0, 0) == 1);
    CHECK(m(0, 1) == 2);
    CHECK(m(0, 2) == 3);
    CHECK(m(1, 0) == 4);
    CHECK(m(1, 1) == 5);
    CHECK(m(1, 2) == 6);
}

TEST_CASE_TEMPLATE("equality", T, int, float, double)
{
    SparseMatrix<2, 2, T> s {
        { {0, 0}, 1 },
        { {0, 1}, 2 },
        { {1, 0}, 3 },
        { {1, 1}, 4 },
    };

    SparseMatrix<2, 2, T> t {
        { {0, 0}, 5 },
        { {0, 1}, 6 },
        { {1, 0}, 7 },
        { {1, 1}, 8 },
    };

    SparseMatrix<2, 2, T> u {
        { {0, 0}, 1 },
        { {0, 1}, 2 },
        { {1, 0}, 3 },
        { {1, 1}, 4 },
    };

    SUBCASE("equal")
    {
        CHECK(s == s);
        CHECK(u == s);
        CHECK(s == u);
    }

    SUBCASE("not equal")
    {
        CHECK(s != t);
        CHECK(t != s);
    }
}

TEST_CASE_TEMPLATE("allocation only when accessed", T, int, float, double)
{
    SparseMatrix<2, 3, T> m;

    CHECK(m.size() == 6);
    CHECK(m.allocated() == 0);
    CHECK(m.peek(1, 2) == false);

    m(1, 2) = 3;

    CHECK(m.allocated() == 1);
    CHECK(m.peek(1, 2) == true);
}

TEST_CASE_TEMPLATE("arithmetic operators", T, int, float, double)
{
    SparseMatrix<2, 2, T> s {
        { {0, 0}, 1 },
        { {0, 1}, 2 },
        { {1, 0}, 3 },
        { {1, 1}, 4 },
    };

    SparseMatrix<2, 2, T> t {
        { {0, 0}, 5 },
        { {0, 1}, 6 },
        { {1, 0}, 7 },
        { {1, 1}, 8 },
    };

    SUBCASE("unitary plus")
    {
        SparseMatrix<2, 2, T> u = +s;
        CHECK(u(0, 0) == s(0, 0));
        CHECK(u(0, 1) == s(0, 1));
        CHECK(u(1, 0) == s(1, 0));
        CHECK(u(1, 1) == s(1, 1));
    }

    SUBCASE("unitary minus")
    {
        SparseMatrix<2, 2, T> u = -s;
        CHECK(u(0, 0) == -s(0, 0));
        CHECK(u(0, 1) == -s(0, 1));
        CHECK(u(1, 0) == -s(1, 0));
        CHECK(u(1, 1) == -s(1, 1));
    }

    SUBCASE("addition")
    {
        SparseMatrix<2, 2, T> u = s + t;
        CHECK(u(0, 0) == s(0, 0) + t(0, 0));
        CHECK(u(0, 1) == s(0, 1) + t(0, 1));
        CHECK(u(1, 0) == s(1, 0) + t(1, 0));
        CHECK(u(1, 1) == s(1, 1) + t(1, 1));
    }

    SUBCASE("subtraction")
    {
        SparseMatrix<2, 2, T> u = s - t;
        CHECK(u(0, 0) == s(0, 0) - t(0, 0));
        CHECK(u(0, 1) == s(0, 1) - t(0, 1));
        CHECK(u(1, 0) == s(1, 0) - t(1, 0));
        CHECK(u(1, 1) == s(1, 1) - t(1, 1));
    }

    SUBCASE("multiplication")
    {
        SparseMatrix<2, 2, T> u = s * t;
        CHECK(u(0, 0) == s(0, 0) * t(0, 0) + s(0, 1) * t(1, 0));
        CHECK(u(0, 1) == s(0, 0) * t(0, 1) + s(0, 1) * t(1, 1));
        CHECK(u(1, 0) == s(1, 0) * t(0, 0) + s(1, 1) * t(1, 0));
        CHECK(u(1, 1) == s(1, 0) * t(0, 1) + s(1, 1) * t(1, 1));
    }

    SUBCASE("transpose")
    {
        SparseMatrix<2, 3, T> m = {
            { {0, 0}, 1 },
            { {0, 1}, 2 },
            { {0, 2}, 3 },
            { {1, 0}, 4 },
            { {1, 1}, 5 },
            { {1, 2}, 6 },
        };

        auto m_transpose = m.transpose();
        CHECK(m_transpose(0, 0) == 1);
        CHECK(m_transpose(1, 0) == 2);
        CHECK(m_transpose(2, 0) == 3);
        CHECK(m_transpose(0, 1) == 4);
        CHECK(m_transpose(1, 1) == 5);
        CHECK(m_transpose(2, 1) == 6);
    }
}
