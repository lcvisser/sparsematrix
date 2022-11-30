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
}
