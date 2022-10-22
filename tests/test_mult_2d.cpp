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
