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


#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <map>
#include <tuple>
#include <utility>


/**
 * mxn matrix has m rows and n columns
 */
template <size_t M, size_t N, typename T>
class SparseMatrix
{
    private:
        // Keys are pairs (i,j), which are sorted first by i and then by j, resulting in storage in row-major order
        std::map<std::pair<size_t, size_t>, T> _values;

    public:
        SparseMatrix() = default;
        SparseMatrix(const SparseMatrix& other) = default;

        SparseMatrix(std::initializer_list<std::pair<const std::pair<size_t, size_t>, T>> m) : _values{m}
        {
            for (const auto& elem : m)
            {
                size_t i, j;
                std::tie(i, j) = elem.first;
                if (i >= M || j >= N)
                {
                    throw std::out_of_range("index out of bounds");
                }
            }
        }

        T& operator()(size_t i, size_t j)
        {
            if (i >= M || j >= N)
            {
                throw std::out_of_range("index out of bounds");
            }

            // Keys are inserted if non-existing
            return _values[{i, j}];
        }

        size_t size() const
        {
            return M * N;
        }

        size_t allocated() const
        {
            return std::distance(_values.cbegin(), _values.cend());
        }

        bool peek(size_t i, size_t j) const
        {
            if (i >= M || j >= N)
            {
                throw std::out_of_range("index out of bounds");
            }

            bool has_value = false;
            if (_values.find({i, j}) == _values.end())
            {
                has_value = false;
            }
            else
            {
                has_value = true;
            }
            return has_value;
        }

        const typename std::map<std::pair<size_t, size_t>, T>::const_iterator cbegin() const
        {
            return _values.cbegin();
        }

        const typename std::map<std::pair<size_t, size_t>, T>::const_iterator cend() const
        {
            return _values.cend();
        }

        bool operator==(const SparseMatrix& rhs) const
        {
            return _values == rhs._values;
        }

        bool operator!=(const SparseMatrix& rhs) const
        {
            return !(_values == rhs._values);
        }

        SparseMatrix& operator+=(const SparseMatrix& rhs)
        {
            auto elem = rhs.cbegin();
            while (elem != rhs.cend())
            {
                size_t i, j;
                std::tie(i, j) = elem->first;
                this->operator()(i, j) += elem->second;
                ++elem;
            }
            return *this;
        }

        friend SparseMatrix operator+(SparseMatrix& op1, const SparseMatrix& op2)
        {
            SparseMatrix lhs(op1);
            lhs += op2;
            return lhs;
        }

        friend const SparseMatrix& operator+(const SparseMatrix& rhs)
        {
            return rhs;
        }

        SparseMatrix& operator-=(const SparseMatrix& rhs)
        {
            return *this += (-rhs);
        }

        friend SparseMatrix operator-(SparseMatrix op1, const SparseMatrix& op2)
        {
            SparseMatrix lhs(op1);
            lhs -= op2;
            return lhs;
        }

        friend SparseMatrix operator-(const SparseMatrix& rhs)
        {
            SparseMatrix lhs = rhs;
            for (auto elem = lhs._values.begin(); elem != lhs._values.end(); ++elem)
            {
                elem->second *= -1;
            }
            return lhs;
        }

        friend SparseMatrix operator*(const T s, const SparseMatrix& op2)
        {
            SparseMatrix lhs = op2;
            for (auto elem = lhs._values.begin(); elem != lhs._values.end(); ++elem)
            {
                elem->second *= s;
            }
            return lhs;
        }

        friend SparseMatrix operator*(const SparseMatrix& op1, const T s)
        {
            return s * op1;
        }

        template <size_t P>
        friend SparseMatrix<M, P, T> operator*(const SparseMatrix<M, N, T>& op1, const SparseMatrix<N, P, T>& op2)
        {
            SparseMatrix<M, P, T> lhs;

            if (op1.allocated() == 0 || op2.allocated() == 0)
            {
                return lhs;
            }

            for (size_t r = 0; r < M; ++r)
            {
                for (size_t c = 0; c < P; ++c)
                {
                    // For C = A * B, element C(x,y) = sum_i A(x,i) * B(i, y)
                    T v = 0;
                    for (size_t i = 0; i < N; ++i)
                    {
                        if (op1.peek(r, i) && op2.peek(i, c))
                        {
                            v += const_cast<SparseMatrix<M, N, T>&>(op1)(r, i) * const_cast<SparseMatrix<N, P, T>&>(op2)(i, c);
                        }
                    }
                    if (v != 0)
                    {
                        lhs(r, c) = v;
                    }
                }
            }

            return lhs;
        }

        SparseMatrix<N, M, T> transpose()
        {
            SparseMatrix<N, M, T> lhs;
            for (auto elem = _values.cbegin(); elem != _values.cend(); ++elem)
            {
                lhs(elem->first.second, elem->first.first) = elem->second;
            }

            return lhs;
        }
};
