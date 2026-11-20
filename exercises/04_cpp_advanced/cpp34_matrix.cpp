// Matrix over std::vector<std::vector<int>>: constructors, operator() for
// element access, operator* that refuses mismatched dimensions, const
// overloads. A const Matrix cannot hand out a mutable reference.
//
// Coming from TypeScript: arrays have no const view and no dimension contract;
// here the const overload is a different function, and a broken multiply throws
// instead of producing NaN-shaped garbage.
//
// Task: operator* must accumulate over the inner dimension and throw
// std::invalid_argument when cols() != other.rows().
#include "cztest.h"

#include <cstddef>
#include <stdexcept>
#include <vector>

class Matrix {
public:
    Matrix(std::size_t rows, std::size_t cols, int fill = 0)
        : rows_(rows), cols_(cols), data_(rows, std::vector<int>(cols, fill)) {}

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }
    int& operator()(std::size_t row, std::size_t col) { return data_.at(row).at(col); }
    const int& operator()(std::size_t row, std::size_t col) const { return data_.at(row).at(col); }

    Matrix operator*(const Matrix& other) const
    {
        // TODO: refuse mismatched dimensions with std::invalid_argument, and
        // sum data_[i][k] * other.data_[k][j] over every k.
        Matrix result(rows_, other.cols_);
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < other.cols_; ++j) {
                result.data_[i][j] = data_[i][0] * other.data_[0][j];
            }
        }
        return result;
    }

    bool operator==(const Matrix& other) const
    {
        return rows_ == other.rows_ && cols_ == other.cols_ && data_ == other.data_;
    }

private:
    std::size_t rows_;
    std::size_t cols_;
    std::vector<std::vector<int>> data_;
};

int main()
{
    Matrix a(2, 3);
    Matrix b(3, 2);
    int next = 1;
    for (std::size_t row = 0; row < a.rows(); ++row) {
        for (std::size_t col = 0; col < a.cols(); ++col) { a(row, col) = next++; }
    }
    next = 7;
    for (std::size_t row = 0; row < b.rows(); ++row) {
        for (std::size_t col = 0; col < b.cols(); ++col) { b(row, col) = next++; }
    }

    const Matrix product = a * b;
    CZ_CHECK(product.rows() == 2 && product.cols() == 2);
    CZ_CHECK(product(0, 0) == 58 && product(1, 1) == 154);
    CZ_CHECK_INT(product(1, 0), 139);

    bool mismatch = false;
    try { (void)(a * Matrix(2, 2)); } catch (const std::invalid_argument&) { mismatch = true; }
    CZ_CHECK(mismatch);
    return cztest_summary();
}

// I AM NOT DONE
