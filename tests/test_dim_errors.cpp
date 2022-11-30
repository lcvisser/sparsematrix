#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "sparsematrix.h"

TEST_CASE_TEMPLATE("bounds checks", T, int, float, double)
{
    SUBCASE("out of bounds initialization")
    {
        REQUIRE_THROWS_AS( (SparseMatrix<2, 3, T>({ { {3, 4}, 1 } })), const std::out_of_range& );
    }

    SUBCASE("out of bounds access")
    {
        SparseMatrix<2, 3, T> s;
        T x;
        REQUIRE_THROWS_AS( x = s(4, 5), const std::out_of_range& );
    }

    SUBCASE("out of bounds allocation")
    {
        SparseMatrix<2, 3, T> s;
        REQUIRE_THROWS_AS( s(4, 5) = 6, const std::out_of_range& );
    }

    SUBCASE("out of bounds peek")
    {
        SparseMatrix<2, 3, T> s;
        REQUIRE_THROWS_AS( s.peek(4, 5), const std::out_of_range& );
    }
}
