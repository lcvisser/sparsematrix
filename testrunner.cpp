#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "sparsematrix.h"

TEST_CASE_TEMPLATE("create a matrix", T, int, float, double)
{
    SparseMatrix<2, 3, T> m = {
        { {0, 0}, 0 },
        { {0, 1}, 1 },
        { {0, 2}, 2 },
        { {1, 0}, 3 },
        { {1, 1}, 4 },
        { {1, 2}, 5 },
    };

    CHECK(m(0, 0) == 0);
    CHECK(m(0, 1) == 1);
    CHECK(m(0, 2) == 2);
    CHECK(m(1, 0) == 3);
    CHECK(m(1, 1) == 4);
    CHECK(m(1, 2) == 5);
}

TEST_CASE("allocation only when accessed")
{
    SparseMatrix<2, 3, int> m;

    CHECK(m.size() == 6);
    CHECK(m.allocated() == 0);
    CHECK(m.peek(1, 2) == false);

    m(1, 2) = 3;

    CHECK(m.allocated() == 1);
    CHECK(m.peek(1, 2) == true);
}
