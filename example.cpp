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


#include <iostream>

#include "sparsematrix/sparsematrix.h"

template<size_t M, size_t N, typename T>
std::ostream& operator<<(std::ostream& os, const SparseMatrix<M, N, T>& m)
{
    for (size_t i = 0; i < M; ++i)
    {
        os << "|";
        for (size_t j = 0; j < N; ++j)
        {
            if (m.peek(i, j))
            {
                os << const_cast<SparseMatrix<M, N, T>&>(m)(i, j);
            }
            else
            {
                os << 0;
            }

            if (j < N - 1)
            {
                os << " ";
            }
        }
        os << "|\n";
    }
    std::cout << std::endl;

    return os;
}

int main(int argc, char* argv[])
{
    SparseMatrix<2, 2, int> s;
    s(1, 1) = 4;
    s(0, 1) = 2;
    std::cout << "s = \n" << s;

    SparseMatrix<2, 2, int> t;
    t(0, 0) = 1;
    t(1, 0) = 3;
    t(1, 1) = 4;
    std::cout << "t = \n" << t;

    SparseMatrix<2, 3, int> u;
    u(0, 0) = 1;
    u(0, 1) = 2;
    u(0, 2) = 3;
    u(1, 0) = 4;
    u(1, 1) = 5;
    u(1, 2) = 6;
    std::cout << "u = \n" << u;

    std::cout << "Unitary operations:\n";
    std::cout << "-s = \n" << -s;
    std::cout << "+s = \n" << +s;

    std::cout << "Addition and subtraction:\n";
    std::cout << "s + t = \n" << s + t;
    std::cout << "s - t = \n" << s - t;

    std::cout << "Multiplication:\n";
    std::cout << "t * u = \n" << s * t;

    std::cout << "Transpose:\n";
    std::cout << "u^T = \n" << u.transpose();

    return 0;
}
