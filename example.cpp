#include <iostream>

#include "sparsematrix.h"

template<size_t M, size_t N, typename T>
std::ostream& operator<<(std::ostream& os, const SparseMatrix<M, N, T>& m)
{
    for (int i = 0; i < M; ++i)
    {
        os << "|";
        for (int j = 0; j < N; ++j)
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
    SparseMatrix<2, 2, int> t;
    SparseMatrix<2, 3, int> u;

    s(1, 1) = 4;
    s(0, 1) = 2;
    std::cout << "s = \n" << s;
    std::cout << "-s = \n" << -s;
    std::cout << "+s = \n" << +s;

    t(0, 0) = 1;
    t(1, 0) = 3;
    t(1, 1) = 4;
    std::cout << "t = \n" << t;

    std::cout << "s + t = \n" << s + t;
    std::cout << "s - t = \n" << s - t;

    u(0, 0) = 1;
    u(0, 1) = 2;
    u(0, 2) = 3;
    u(1, 0) = 4;
    u(1, 1) = 5;
    u(1, 2) = 6;
    std::cout << "t = \n" << t;
    std::cout << "u = \n" << u;
    std::cout << "t * u = \n" << s * t;

    for (auto iter = t.cbegin(); iter != t.cend(); ++iter)
    {
        std::cout << iter->second << ", ";
    }
    std::cout << std::endl;
    return 0;
}
