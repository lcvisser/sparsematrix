/* Copyright 2022 Ludo Visser

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
#include <cassert>
#include <stdexcept>
#include <functional>
#include <map>
#include <tuple>
#include <utility>
#include <vector>

struct ColumnMajorOrder
{
    bool operator()(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) const
    {
        return (lhs.second < rhs.second) ? true :  // check column (<)
                (lhs.second > rhs.second) ? false : // check column (>)
                (lhs.first < rhs.first) ? true : false; // columns equal, check row
    }
};

/**
 * mxn matrix has m rows and n columns
 */
template <size_t M, size_t N, typename T>
class SparseMatrix
{
    //private:
    public:
        // Keys are pairs (i,j), which are sorted first by i and thn by j, resulting in storage in row-major order
        std::map<std::pair<int, int>, T> _values;

    public:
        SparseMatrix() = default;
        SparseMatrix(const SparseMatrix& other) = default;

        SparseMatrix(std::initializer_list<std::pair<const std::pair<int, int>, T>> m) : _values{m}
        {
            for (const auto elem : m)
            {
                int i, j;
                std::tie(i, j) = elem.first;
                if (i >= M || j >= N)
                {
                    throw std::out_of_range("index out of bounds");
                }
            }
        }

        T& operator()(int i, int j)
        {
            if (i >= M || j >= N)
            {
                throw std::out_of_range("index out of bounds");
            }

            // Keys are inserted if non-existing
            return _values[{i, j}];
        }

        int size() const
        {
            return M * N;
        }

        int allocated() const
        {
            return std::distance(_values.cbegin(), _values.cend());
        }

        bool peek(int i, int j) const
        {
            if (i >= M || j >= N)
            {
                throw std::out_of_range("index out of bounds");
            }

            bool has_value = false;
            try
            {
                auto v = _values.at({i, j});
                has_value = true;
            }
            catch (const std::out_of_range& e)
            {
                has_value = false;
            }
            return has_value;
        }

        const typename std::map<std::pair<int, int>, T>::const_iterator cbegin() const
        {
            return _values.cbegin();
        }

        const typename std::map<std::pair<int, int>, T>::const_iterator cend() const
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
                int i, j;
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

        template <size_t P>
        friend SparseMatrix<M, P, T> operator*(const SparseMatrix<M, N, T>& op1, const SparseMatrix<N, P, T>& op2)
        {
            // Create a column-major view on the second operand
            using wrapped_key = std::reference_wrapper<const std::pair<int, int>>;
            using wrapped_value = std::reference_wrapper<const T>;
            std::map<wrapped_key, wrapped_value, ColumnMajorOrder> op2_transpose;
            op2_transpose.insert(op2._values.cbegin(), op2._values.cend());

            SparseMatrix<M, P, T> lhs;

            if (op1.allocated() == 0 || op2.allocated() == 0)
            {
                return lhs;
            }

            // Iterate over each row of op1, adding element-wise product only if column of op1 matches row of op2. If
            // column number of op2 changes, reset op1 iterator to the beginning of the row. When the end of op2 is
            // reached, jump to the next row of op1. In between, increment op1 or op2 to catch up.
            int op1_row, op1_col;
            int op2_row, op2_col;

            auto op1_elem = op1.cbegin();
            auto op1_rowstart_elem = op1.cbegin();
            auto op2_elem = op2_transpose.cbegin();
            while (op1_elem != op1.cend())
            {
                while (op2_elem != op2_transpose.cend())
                {
                    op1_row = op1_elem->first.first;
                    op1_col = op1_elem->first.second;
                    op2_row = op2_elem->first.get().first;
                    op2_col = op2_elem->first.get().second;
                    T op1_value = op1_elem->second;
                    T op2_value = op2_elem->second.get();

                    if (op1_col < op2_row)
                    {
                        ++op1_elem;
                    }
                    else if (op1_col == op2_row)
                    {
                        lhs(op1_row, op2_col) += op1_value * op2_value;
                        ++op1_elem;
                        ++op2_elem;
                    }
                    else  // op1_col > op2_row
                    {
                        ++op2_elem;
                    }

                    if (op1_elem == op1.cend())
                    {
                        // End of op1
                        if (op2_col == P - 1)  // need to check previous value!
                        {
                            // Reached last column of op2, so done
                            break;
                        }
                        else
                        {
                            // Reset op1 to the beginning of the row and start op2 at the next column
                            op1_elem = op1_rowstart_elem;
                            while ((op2_elem != op2_transpose.cend()) && (op2_elem->first.get().second == op2_col))
                            {
                                ++op2_elem;
                            }
                            break;
                        }
                    }
                    else if (op1_elem->first.first > op1_row)
                    {
                        // Reached end of row of op1
                        if (op2_col == P - 1)  // need to check previous value!!=
                        {
                            // Reached last column of op2, so restart op2 iteration on next row
                            op1_rowstart_elem = op1_elem;
                            op2_elem = op2_transpose.cbegin();
                            break;
                        }
                        else
                        {
                            // Reset op1 to the beginning of the row and start op2 at the next column
                            op1_elem = op1_rowstart_elem;
                            while ((op2_elem != op2_transpose.cend()) && (op2_elem->first.get().second == op2_col))
                            {
                                ++op2_elem;
                            }
                            break;
                        }
                    }
                    else
                    {
                        if (op2_elem == op2_transpose.cend())
                        {
                            // End of op2; go to next row of op1 and restart op2 iteration
                            while ((op1_elem != op1.cend()) && (op1_elem->first.first == op1_row))
                            {
                                ++op1_elem;
                            }
                            op1_rowstart_elem = op1_elem;
                            op2_elem = op2_transpose.cbegin();
                            break;
                        }
                        else if (op2_elem->first.get().second != op2_col)
                        {
                            // Reached next column of op2, reset op1 to beginning of the row
                            op1_elem = op1_rowstart_elem;
                        }
                    }
                }
            }

            return lhs;
        }

};
