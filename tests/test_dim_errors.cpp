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
        (void) x;  // Suppress warning on unused variable 'x'
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
