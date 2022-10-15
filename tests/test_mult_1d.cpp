#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "sparsematrix.h"


TEST_CASE_TEMPLATE("multiplication 1d cases", T, int, float, double)
{
    SUBCASE("vector to scalar multiplication")
    {
        SparseMatrix<1, 3, T> s = {
            { {0, 0}, 1 },
            { {0, 1}, 2 },
            { {0, 2}, 3 }
        };

        SparseMatrix<3, 1, T> t = {
            { {0, 0}, 4 },
            { {1, 0}, 5 },
            { {2, 0}, 6 }
        };

        SparseMatrix<1, 1, T> u = s * t;
        CHECK(u(0, 0) == 32);
    }

    SUBCASE("sparse vector to scalar multiplication")
    {
        SparseMatrix<1, 3, T> s = {
            { {0, 1}, 3 }
        };

        SparseMatrix<3, 1, T> t = {
            { {1, 0}, 2 }
        };

        SparseMatrix<1, 1, T> u = s * t;
        CHECK(u.allocated() == 1);
        CHECK(u(0, 0) == 6);
    }

    SUBCASE("sparse vector to zero scalar multiplication")
    {
        SparseMatrix<1, 3, T> s = {
            { {0, 2}, 3 }
        };

        SparseMatrix<3, 1, T> t = {
            { {1, 0}, 2 }
        };

        SparseMatrix<1, 1, T> u = s * t;
        CHECK(u.allocated() == 0);
        CHECK(u(0, 0) == 0);
    }

    SUBCASE("zero vector to scalar multiplication")
    {
        SparseMatrix<1, 3, T> s = {
            { {0, 0}, 1 },
            { {0, 1}, 2 },
            { {0, 2}, 3 }
        };

        SparseMatrix<3, 1, T> t;

        SparseMatrix<1, 1, T> u = s * t;
        CHECK(u.allocated() == 0);
        CHECK(u(0, 0) == 0);
    }

    SUBCASE("vector to matrix multiplication")
    {
        SparseMatrix<3, 1, T> s = {
            { {0, 0}, 1 },
            { {1, 0}, 2 },
            { {2, 0}, 3 }
        };

        SparseMatrix<1, 3, T> t = {
            { {0, 0}, 4 },
            { {0, 1}, 5 },
            { {0, 2}, 6 }
        };

        SparseMatrix<3, 3, T> u = s * t;
        CHECK(u(0, 0) == 4);
        CHECK(u(0, 1) == 5);
        CHECK(u(0, 2) == 6);
        CHECK(u(1, 0) == 8);
        CHECK(u(1, 1) == 10);
        CHECK(u(1, 2) == 12);
        CHECK(u(2, 0) == 12);
        CHECK(u(2, 1) == 15);
        CHECK(u(2, 2) == 18);
    }
}
