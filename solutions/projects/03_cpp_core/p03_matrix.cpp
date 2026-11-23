// p03_matrix -- C++ core project 2. Reference solution.
//
// A small owning Matrix class: RAII, the rule of five, bounds-checked access,
// operator overloading and exceptions.
#include "cztest.h"

#include <cstddef>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

class Matrix {
public:
    Matrix() : rows_(0), cols_(0), data_(nullptr) {}

    Matrix(std::size_t rows, std::size_t cols) : Matrix(rows, cols, 0.0) {}

    Matrix(std::size_t rows, std::size_t cols, double fill)
        : rows_(rows), cols_(cols), data_(nullptr)
    {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument("matrix dimensions must be positive");
        }
        data_ = new double[rows * cols];
        for (std::size_t i = 0; i < rows * cols; ++i) {
            data_[i] = fill;
        }
    }

    Matrix(const Matrix& other)
        : rows_(other.rows_), cols_(other.cols_),
          data_(other.data_ == nullptr
                    ? nullptr
                    : new double[other.rows_ * other.cols_])
    {
        for (std::size_t i = 0; i < rows_ * cols_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    Matrix& operator=(const Matrix& other)
    {
        if (this != &other) {
            Matrix copy(other);
            std::swap(rows_, copy.rows_);
            std::swap(cols_, copy.cols_);
            std::swap(data_, copy.data_);
        }
        return *this;
    }

    Matrix(Matrix&& other) noexcept
        : rows_(other.rows_), cols_(other.cols_), data_(other.data_)
    {
        other.rows_ = 0;
        other.cols_ = 0;
        other.data_ = nullptr;
    }

    Matrix& operator=(Matrix&& other) noexcept
    {
        if (this != &other) {
            delete[] data_;
            rows_ = other.rows_;
            cols_ = other.cols_;
            data_ = other.data_;
            other.rows_ = 0;
            other.cols_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    ~Matrix() { delete[] data_; }

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

    double& operator()(std::size_t row, std::size_t col)
    {
        if (row >= rows_ || col >= cols_) {
            throw std::out_of_range("matrix index");
        }
        return data_[row * cols_ + col];
    }

    const double& operator()(std::size_t row, std::size_t col) const
    {
        if (row >= rows_ || col >= cols_) {
            throw std::out_of_range("matrix index");
        }
        return data_[row * cols_ + col];
    }

    Matrix& operator+=(const Matrix& other)
    {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            throw std::invalid_argument("matrix shape mismatch");
        }
        for (std::size_t i = 0; i < rows_ * cols_; ++i) {
            data_[i] += other.data_[i];
        }
        return *this;
    }

    Matrix operator-() const
    {
        Matrix result(*this);
        for (std::size_t i = 0; i < rows_ * cols_; ++i) {
            result.data_[i] = -result.data_[i];
        }
        return result;
    }

    Matrix transpose() const
    {
        if (rows_ == 0 || cols_ == 0) {
            return Matrix();
        }
        Matrix result(cols_, rows_);
        for (std::size_t row = 0; row < rows_; ++row) {
            for (std::size_t col = 0; col < cols_; ++col) {
                result(col, row) = (*this)(row, col);
            }
        }
        return result;
    }

    bool operator==(const Matrix& other) const
    {
        if (rows_ != other.rows_ || cols_ != other.cols_) {
            return false;
        }
        for (std::size_t i = 0; i < rows_ * cols_; ++i) {
            if (data_[i] != other.data_[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix& other) const { return !(*this == other); }

    friend Matrix operator+(Matrix lhs, const Matrix& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    friend Matrix operator*(const Matrix& left, const Matrix& right)
    {
        if (left.cols_ != right.rows_) {
            throw std::invalid_argument("matrix shape mismatch");
        }
        if (left.rows_ == 0 || right.cols_ == 0) {
            return Matrix();
        }
        Matrix result(left.rows_, right.cols_);
        for (std::size_t row = 0; row < left.rows_; ++row) {
            for (std::size_t col = 0; col < right.cols_; ++col) {
                double value = 0.0;
                for (std::size_t k = 0; k < left.cols_; ++k) {
                    value += left(row, k) * right(k, col);
                }
                result(row, col) = value;
            }
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix)
    {
        for (std::size_t row = 0; row < matrix.rows_; ++row) {
            for (std::size_t col = 0; col < matrix.cols_; ++col) {
                if (col != 0) {
                    out << ' ';
                }
                out << matrix.data_[row * matrix.cols_ + col];
            }
            out << '\n';
        }
        return out;
    }

private:
    std::size_t rows_;
    std::size_t cols_;
    double* data_;
};

int main()
{
    /* ---- Level 1: construction, copy, move ---------------------------- */
    Matrix p(2, 3);
    double next = 1.0;
    for (std::size_t row = 0; row < p.rows(); ++row) {
        for (std::size_t col = 0; col < p.cols(); ++col) {
            p(row, col) = next;
            next += 1.0;
        }
    }
    CZ_CHECK_INT((long)p.rows(), 2);
    CZ_CHECK_INT((long)p.cols(), 3);
    CZ_CHECK_NEAR(p(0, 0), 1.0, 1e-9);
    CZ_CHECK_NEAR(p(1, 2), 6.0, 1e-9);

    bool threw = false;
    try {
        (void)p(2, 0);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    CZ_CHECK(threw);

    Matrix copy = p;
    CZ_CHECK_NEAR(copy(1, 2), 6.0, 1e-9);
    copy(0, 0) = 99.0;
    CZ_CHECK_NEAR(p(0, 0), 1.0, 1e-9);
    CZ_CHECK_NEAR(copy(0, 0), 99.0, 1e-9);

    Matrix assigned;
    assigned = p;
    CZ_CHECK_INT((long)assigned.rows(), 2);
    if (assigned.rows() == 2 && assigned.cols() == 3) {
        CZ_CHECK_NEAR(assigned(1, 2), 6.0, 1e-9);
        assigned(0, 1) = 42.0;
        CZ_CHECK_NEAR(p(0, 1), 2.0, 1e-9);
    }

    Matrix moved = std::move(copy);
    CZ_CHECK_INT((long)moved.rows(), 2);
    if (moved.rows() == 2 && moved.cols() == 3) {
        CZ_CHECK_NEAR(moved(0, 0), 99.0, 1e-9);
        CZ_CHECK_NEAR(moved(1, 2), 6.0, 1e-9);
    }
    CZ_CHECK_INT((long)copy.rows(), 0);
    CZ_CHECK_INT((long)copy.cols(), 0);

    Matrix target;
    target = std::move(assigned);
    CZ_CHECK_INT((long)target.rows(), 2);
    if (target.rows() == 2 && target.cols() == 3) {
        CZ_CHECK_NEAR(target(0, 1), 42.0, 1e-9);
        CZ_CHECK_NEAR(target(1, 2), 6.0, 1e-9);
    }
    CZ_CHECK_INT((long)assigned.rows(), 0);
    CZ_CHECK_INT((long)assigned.cols(), 0);

    /* ---- Level 2: arithmetic ------------------------------------------ */
    Matrix ones(2, 2, 1.0);
    Matrix twos(2, 2, 2.0);
    Matrix sum = ones + twos;
    CZ_CHECK_INT((long)sum.rows(), 2);
    CZ_CHECK_INT((long)sum.cols(), 2);
    if (sum.rows() == 2 && sum.cols() == 2) {
        CZ_CHECK_NEAR(sum(0, 0), 3.0, 1e-9);
        CZ_CHECK_NEAR(sum(1, 1), 3.0, 1e-9);
    }

    threw = false;
    try {
        ones += Matrix(3, 1);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    CZ_CHECK(threw);

    Matrix left(2, 3);
    for (std::size_t row = 0; row < left.rows(); ++row) {
        for (std::size_t col = 0; col < left.cols(); ++col) {
            left(row, col) = static_cast<double>(row * 3 + col + 1);
        }
    }
    Matrix right(3, 2);
    for (std::size_t row = 0; row < right.rows(); ++row) {
        for (std::size_t col = 0; col < right.cols(); ++col) {
            right(row, col) = static_cast<double>(row * 2 + col + 7);
        }
    }

    Matrix product = left * right;
    CZ_CHECK_INT((long)product.rows(), 2);
    CZ_CHECK_INT((long)product.cols(), 2);
    if (product.rows() == 2 && product.cols() == 2) {
        CZ_CHECK_NEAR(product(0, 0), 58.0, 1e-9);
        CZ_CHECK_NEAR(product(0, 1), 64.0, 1e-9);
        CZ_CHECK_NEAR(product(1, 0), 139.0, 1e-9);
        CZ_CHECK_NEAR(product(1, 1), 154.0, 1e-9);
    }

    threw = false;
    try {
        (void)(left * ones);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    CZ_CHECK(threw);

    Matrix transposed = left.transpose();
    CZ_CHECK_INT((long)transposed.rows(), 3);
    CZ_CHECK_INT((long)transposed.cols(), 2);
    if (transposed.rows() == 3 && transposed.cols() == 2) {
        CZ_CHECK_NEAR(transposed(0, 0), 1.0, 1e-9);
        CZ_CHECK_NEAR(transposed(0, 1), 4.0, 1e-9);
        CZ_CHECK_NEAR(transposed(2, 1), 6.0, 1e-9);
    }

    Matrix negative = -ones;
    CZ_CHECK_INT((long)negative.rows(), 2);
    if (negative.rows() == 2 && negative.cols() == 2) {
        CZ_CHECK_NEAR(negative(1, 1), -1.0, 1e-9);
    }

    /* ---- Level 3: inspection ------------------------------------------ */
    CZ_CHECK(p == p);
    CZ_CHECK(p != right);
    CZ_CHECK(Matrix(2, 2, 5.0) == Matrix(2, 2, 5.0));

    std::ostringstream rendered;
    rendered << p;
    const std::string rendered_text = rendered.str();
    CZ_CHECK_STR(rendered_text.c_str(), "1 2 3\n4 5 6\n");

    return cztest_summary();
}
