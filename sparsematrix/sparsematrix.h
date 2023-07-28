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


//! Representation of a sparse matrix with M rows and N columns, of type T
/*!
 * This class represents a sparse matrix, i.e. a matrix with mostly empty (zero-valued) cells. Internally it uses a map
 * (std::map) to store non-zero values. The matrix is stored in row-major order. Indices (i,j) are used as keys for the
 * map.
 */
template <size_t M, size_t N, typename T>
class SparseMatrix
{
    private:
        //! Internal storage map; keys are pairs (i,j), which are sorted first by i and then by j (row-major order).
        std::map<std::pair<size_t, size_t>, T> _values;

    public:
        //! Default constructor.
        SparseMatrix() = default;

        //! Copy-constructor
        /*!
         * Create a copy of an existing instance.
         *
         * \param other object to be copied.
         */
        SparseMatrix(const SparseMatrix& other) = default;

        //! Construction via initializer list.
        /*!
         * Create an instance by providing a (key, value)-list for the cells to be populated. The key is a pair (i,j)
         * corresponding to the row and column indices into the matrix.
         *
         * \param m initializer list of (key, value) pairs.
         */
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

        //! Access an element at index (i,j).
        /*!
         * Access an individual element at row i and column j. If the element was empty before (i.e. (i,j) is not a key
         * in the map storage), it is created. Use peek() if to avoid this.
         * A reference to the value is return, so this operator can be used for both reading (v = A(i,j)) and writing
         * (A(i,j) = v) elements.
         * Throws std::out_of_range if either i or j exceeds the respective matrix dimension.
         *
         * \sa peek()
         *
         * \param i row index.
         * \param j column index.
         * \return a reference to the element at (i,j).
         */
        T& operator()(size_t i, size_t j)
        {
            if (i >= M || j >= N)
            {
                throw std::out_of_range("index out of bounds");
            }

            // Keys are inserted if non-existing.
            return _values[{i, j}];
        }

        //! Size of the matrix.
        /*!
         * Gives the size of the matrix as the number of elements. By definition, this is equal to M x N. Note that the
         * size is not related to the actual memory allocation. Elements are only allocated when they are assigned a
         * value.
         *
         * \sa allocated()
         *
         * \return the matrix size.
         */
        size_t size() const
        {
            return M * N;
        }

        //! Number of allocated elements.
        /*!
         * Get the number of allocated elements. This is typically equal to the number of non-zero elements, unless one
         * or more elements have been accessed before. The number of allocated elements R always satisfies
         * 0 <= R <= M x N.
         *
         * \sa size()
         *
         * \return the number of allocated elements.
         */
        size_t allocated() const
        {
            return std::distance(_values.cbegin(), _values.cend());
        }

        //! Peek if an element is allocated.
        /*!
         * Check if the element has an allocated space in the internal storage. Memory usage is proportional to the
         * number of allocated elements. Elements are only allocated when they are assigned a value.
         * Throws std::out_of_range if either i or j exceeds the respective matrix dimension.
         *
         * \param i row index.
         * \param j column index.
         * \return Boolean value indicating if the element is allocated.
         */
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

        //! Constant iterator to the beginning of the internal map storage.
        const typename std::map<std::pair<size_t, size_t>, T>::const_iterator cbegin() const
        {
            return _values.cbegin();
        }

        //! Constant iterator to the end of the internal map storage.
        const typename std::map<std::pair<size_t, size_t>, T>::const_iterator cend() const
        {
            return _values.cend();
        }

        //! Check for equality.
        /*!
         * Check for equality by comparing the internal map storage. Note that this is a strict comparison that also
         * implicitly considers sparseness. For example, if two 2x2 matrices A and B are created without any data (empty
         * matrix), but one assigns B(0,1) = 0, then A != B, because B will have one element assigned.
         *
         * \param rhs right-hand side of the equality test.
         * \return Boolean value indicating equality.
         */
        bool operator==(const SparseMatrix& rhs) const
        {
            return _values == rhs._values;
        }

        //! Check for inequality.
        /*!
         * Check for inequality by comparing the internal map storage. Note that this is a strict comparison that also
         * implicitly considers sparseness. For example, if two 2x2 matrices A and B are created without any data (empty
         * matrix), but one assigns B(0,1) = 0, then A != B, because B will have one element assigned.
         *
         * \param rhs right-hand side of the inequality test.
         * \return Boolean value indicating inequality.
         */
        bool operator!=(const SparseMatrix& rhs) const
        {
            return !(_values == rhs._values);
        }

        //! Addition.
        /*!
         * Implements A += B, with A and B of same size and type (checked at compile time).
         *
         * \param rhs Matrix to add.
         * \return A += B.
         */
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

        //! Addition.
        /*!
         * Implements A + B, with A and B of same size and type (checked at compile time).
         *
         * \param op1 First operand.
         * \param op2 Second operand.
         * \return A + B.
         */
        friend SparseMatrix operator+(SparseMatrix& op1, const SparseMatrix& op2)
        {
            SparseMatrix lhs(op1);
            lhs += op2;
            return lhs;
        }

        //! Unitary plus.
        /*!
         * Returns the same matrix unaltered.
         *
         * \param rhs Any matrix A.
         * \return A.
         */
        friend const SparseMatrix& operator+(const SparseMatrix& rhs)
        {
            return rhs;
        }

        //! Subtraction.
        /*!
         * Implements A -= B, with A and B of same size and type (checked at compile time).
         *
         * \param rhs Matrix to subtract.
         * \return A -= B.
         */
        SparseMatrix& operator-=(const SparseMatrix& rhs)
        {
            return *this += (-rhs);
        }

        //! Subtraction.
        /*!
         * Implements A - B, with A and B of same size and type (checked at compile time).
         *
         * \param op1 First operand.
         * \param op2 Second operand.
         * \return A - B.
         */
        friend SparseMatrix operator-(SparseMatrix op1, const SparseMatrix& op2)
        {
            SparseMatrix lhs(op1);
            lhs -= op2;
            return lhs;
        }

        //! Unitary minus.
        /*!
         * Returns a copy of the input matrix with every element negated.
         *
         * \param rhs Any matrix A.
         * \return -A.
         */
        friend SparseMatrix operator-(const SparseMatrix& rhs)
        {
            SparseMatrix lhs = rhs;
            for (auto elem = lhs._values.begin(); elem != lhs._values.end(); ++elem)
            {
                elem->second *= -1;
            }
            return lhs;
        }

        //! Scaling.
        /*!
         * Returns a copy of the input matrix with every element scaled.
         *
         * \param s Scaling factor.
         * \param op2 Any matrix A.
         * \return s x A.
         */
        friend SparseMatrix operator*(const T s, const SparseMatrix& op2)
        {
            SparseMatrix lhs = op2;
            for (auto elem = lhs._values.begin(); elem != lhs._values.end(); ++elem)
            {
                elem->second *= s;
            }
            return lhs;
        }

        //! Scaling.
        /*!
         * Returns a copy of the input matrix with every element scaled.
         *
         * \param op1 Any matrix A.
         * \param s Scaling factor.
         * \return A x s.
         */
        friend SparseMatrix operator*(const SparseMatrix& op1, const T s)
        {
            return s * op1;
        }

        //! Multiplication
        /*!
         * Multiplies two matrices of compatible size (checked at compile time.)
         *
         * \param op1 First operand.
         * \param op2 Second operand.
         * \return A x B.
         */
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

        //! Transpose.
        /*!
         * Returns a copy of the matrix with rows and columns swapped.
         *
         * \return A^T.
         */
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
