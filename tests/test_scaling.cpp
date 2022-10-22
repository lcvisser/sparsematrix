#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "sparsematrix.h"


TEST_CASE_TEMPLATE("scaling ", T, int, float, double)
{
    SUBCASE("scaling vector")
    {
        T s = 4;

        SparseMatrix<1, 3, T> t = {
            { {0, 0}, 1 },
            { {0, 1}, 2 },
            { {0, 2}, 3 }
        };

        SparseMatrix<1, 3, T> u = s * t;
        CHECK(u.allocated() == 3);
        CHECK(u(0, 0) == 4);
        CHECK(u(0, 1) == 8);
        CHECK(u(0, 2) == 12);

        SparseMatrix<1, 3, T> v = t * s;
        CHECK(v == u);
    }

    SUBCASE("scaling matrix")
    {
        T s = 4;

        SparseMatrix<2, 3, T> t = {
            { {0, 0}, 1 },
            { {0, 1}, 2 },
            { {0, 2}, 3 },
            { {1, 0}, 4 },
            { {1, 1}, 5 },
            { {1, 2}, 6 }
        };

        SparseMatrix<2, 3, T> u = s * t;
        CHECK(u.allocated() == 6);
        CHECK(u(0, 0) == 4);
        CHECK(u(0, 1) == 8);
        CHECK(u(0, 2) == 12);
        CHECK(u(1, 0) == 16);
        CHECK(u(1, 1) == 20);
        CHECK(u(1, 2) == 24);

        SparseMatrix<2, 3, T> v = t * s;
        CHECK(v == u);
    }

    SUBCASE("scaling sparse vector")
    {
        T s = 4;

        SparseMatrix<1, 3, T> t = {
            { {0, 1}, 2 }
        };

        SparseMatrix<1, 3, T> u = s * t;
        CHECK(u.allocated() == 1);
        CHECK(u(0, 1) == 8);

        SparseMatrix<1, 3, T> v = t * s;
        CHECK(v == u);
    }

    SUBCASE("scaling sparse matrix")
    {
        T s = 4;

        SparseMatrix<2, 3, T> t = {
            { {0, 1}, 2 },
            { {1, 2}, 6 }
        };

        SparseMatrix<2, 3, T> u = s * t;
        CHECK(u.allocated() == 2);
        CHECK(u(0, 1) == 8);
        CHECK(u(1, 2) == 24);

        SparseMatrix<2, 3, T> v = t * s;
        CHECK(v == u);
    }

    SUBCASE("scaling zero vector")
    {
        T s = 4;

        SparseMatrix<1, 3, T> t;

        SparseMatrix<1, 3, T> u = s * t;
        CHECK(u.allocated() == 0);

        SparseMatrix<1, 3, T> v = t * s;
        CHECK(v == u);
    }

    SUBCASE("scaling zero matrix")
    {
        T s = 4;

        SparseMatrix<2, 3, T> t;

        SparseMatrix<2, 3, T> u = s * t;
        CHECK(u.allocated() == 0);

        SparseMatrix<2, 3, T> v = t * s;
        CHECK(v == u);
    }
}
