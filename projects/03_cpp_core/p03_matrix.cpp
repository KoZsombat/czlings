// p03_matrix -- C++ core project 2.
//
// A small owning Matrix class: RAII, the rule of five, bounds-checked access,
// operator overloading and exceptions. It complements p03_bank (polymorphism,
// unique_ptr, STL) with manual resource ownership.
//
// Level 1 -- resource management
//   copy constructor        deep copy the buffer
//   copy assignment        copy-and-swap or an equivalent safe implementation
//   move constructor        steal the buffer, leave the source empty
//   move assignment         same, releasing the old buffer first
//   destructor              already deletes[] the buffer
//
// Level 2 -- arithmetic
//   operator+=              shape check + element-wise addition
//   operator+               provided; reuses +=
//   operator*               shape check + matrix product
//   transpose()             rows and columns swap
//   operator-()             provided; element-wise negation
//
// Level 3 -- inspection
//   operator()              provided; throws std::out_of_range
//   operator== / !=         provided; shapes and values
//   operator<<              one row per line, values separated by spaces
//
// A Matrix owns a `new double[]` buffer. Every copy allocates a fresh buffer;
// every move transfers ownership and leaves the source at 0x0. Double-free and
// use-after-move are the two bugs this project is about.
//
// From TS: arrays are garbage-collected and copying is implicit. Here a raw
// pointer has no owner, so copy and move must be written explicitly.
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
          data_(other.rows_ == 0
                    ? nullptr
                    : new double[other.rows_ * other.cols_]())
    {
        // TODO: copy every element from other.data_ into this buffer. The
        // value-initialised allocation above is only a safe starting point.
    }

    Matrix& operator=(const Matrix& other)
    {
        // TODO: release the current buffer and deep-copy other. Guard
        // self-assignment. Copy-and-swap is the shortest safe version.
        (void)other;
        return *this;
    }

    Matrix(Matrix&& other) noexcept : rows_(0), cols_(0), data_(nullptr)
    {
        // TODO: steal other.rows_, other.cols_ and other.data_, then leave
        // other at 0x0 with data_ == nullptr.
        (void)other;
    }

    Matrix& operator=(Matrix&& other) noexcept
    {
        // TODO: release the current buffer, steal other's fields, and leave
        // other at 0x0. Guard self-assignment.
        (void)other;
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
        // TODO: add other.data_[i] to data_[i] for every element.
        return *this;
    }

    Matrix operator-() const
    {
        // TODO: return a copy with every element negated.
        return *this;
    }

    Matrix transpose() const
    {
        Matrix result(cols_, rows_);
        // TODO: result(col, row) = (*this)(row, col) for every element.
        return result;
    }

    bool operator==(const Matrix& other) const
    {
        // TODO: compare shapes and then every element.
        (void)other;
        return false;
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
        Matrix result(left.rows_, right.cols_);
        // TODO: for each (row, col), sum left(row, k) * right(k, col) over k.
        return result;
    }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix)
    {
        // TODO: one row per line, values separated by single spaces, and a
        // newline after each row. An empty 0x0 matrix prints nothing.
        (void)matrix;
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

// I AM NOT DONE
