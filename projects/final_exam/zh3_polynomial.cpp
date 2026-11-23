// zh3_polynomial -- final ZH, task 3 (15 points). BME Prog2 style C++ task.
//
// Implement the missing parts of a small Polynomial value type. Coefficients
// are stored in a std::vector<double>: coeffs_[i] is the coefficient of x^i,
// and trailing zeros are trimmed so degree() is exact (-1 for the zero
// polynomial).
//
// Missing:
//   operator+=        add another polynomial in place
//   operator*         multiply two polynomials
//   operator()        evaluate at x with Horner's rule
//   operator<<        canonical text form
//
// Provided:
//   constructors, trim(), degree(), operator+, operator-, operator== / !=
//
// Text form rules:
//   - zero polynomial: "0"
//   - terms from the highest degree down, no spaces
//   - omit the coefficient when its magnitude is 1, except for the constant
//   - "x" for degree 1, "x^2" for degree 2, and so on
//   - join terms with '+' or '-', using the sign of the coefficient
//   - examples: {-1,2,3} -> "3x^2+2x-1"
//               {0,1}     -> "x"
//               {0,-1,1}  -> "x^2-x"
//               {5}       -> "5"
//               {0,0,2}   -> "2x^2"
//
// From TS: a class with an array field copies by reference unless you clone
// it. Here std::vector already deep-copies, and the operator overloads keep
// the arithmetic syntax close to the mathematics.
#include "cztest.h"

#include <cstddef>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class Polynomial {
public:
    Polynomial() = default;

    explicit Polynomial(std::vector<double> coefficients)
        : coeffs_(std::move(coefficients))
    {
        trim();
    }

    int degree() const
    {
        return coeffs_.empty() ? -1 : static_cast<int>(coeffs_.size()) - 1;
    }

    Polynomial& operator+=(const Polynomial& other)
    {
        // TODO: resize to the larger coefficient count, add element-wise, then
        // trim trailing zeros.
        (void)other;
        return *this;
    }

    Polynomial operator-() const
    {
        Polynomial result(*this);
        for (double& coefficient : result.coeffs_) {
            coefficient = -coefficient;
        }
        return result;
    }

    double operator()(double x) const
    {
        // TODO: Horner's rule from the highest coefficient down:
        //   result = result * x + coeffs_[i]
        (void)x;
        return 0.0;
    }

    bool operator==(const Polynomial& other) const
    {
        return coeffs_ == other.coeffs_;
    }

    bool operator!=(const Polynomial& other) const
    {
        return !(*this == other);
    }

    friend Polynomial operator+(Polynomial lhs, const Polynomial& rhs)
    {
        lhs += rhs;
        return lhs;
    }

    friend Polynomial operator*(const Polynomial& left, const Polynomial& right)
    {
        // TODO: return the zero polynomial if either side is zero; otherwise
        // allocate degree(left) + degree(right) + 1 coefficients, add
        // left[i] * right[j] into result[i + j], and construct a Polynomial
        // from the vector.
        (void)left;
        (void)right;
        return Polynomial();
    }

    friend std::ostream& operator<<(std::ostream& out, const Polynomial& poly)
    {
        // TODO: print the canonical form described above.
        (void)poly;
        return out;
    }

private:
    void trim()
    {
        while (!coeffs_.empty() && coeffs_.back() == 0.0) {
            coeffs_.pop_back();
        }
    }

    std::vector<double> coeffs_;
};

static std::string render(const Polynomial& poly)
{
    std::ostringstream out;
    out << poly;
    return out.str();
}

static void check_render(const Polynomial& poly, const char* expected)
{
    const std::string text = render(poly);
    CZ_CHECK_STR(text.c_str(), expected);
}

int main()
{
    const Polynomial zero;
    CZ_CHECK_INT(zero.degree(), -1);
    check_render(zero, "0");

    const Polynomial p({-1, 2, 3}); // 3x^2 + 2x - 1
    CZ_CHECK_INT(p.degree(), 2);
    CZ_CHECK_NEAR(p(2.0), 15.0, 1e-9);
    CZ_CHECK_NEAR(p(0.0), -1.0, 1e-9);

    const Polynomial q({1, 1}); // x + 1
    const Polynomial sum = p + q; // 3x^2 + 3x
    CZ_CHECK(sum == Polynomial({0, 3, 3}));
    CZ_CHECK_INT(sum.degree(), 2);
    check_render(sum, "3x^2+3x");

    const Polynomial product = p * q;
    CZ_CHECK(product == Polynomial({-1, 1, 5, 3}));
    CZ_CHECK_INT(product.degree(), 3);
    check_render(product, "3x^3+5x^2+x-1");

    CZ_CHECK((zero * p) == zero);
    CZ_CHECK((p * zero) == zero);

    Polynomial accumulated({1, 1});
    accumulated += Polynomial({2, -1, 1});
    CZ_CHECK(accumulated == Polynomial({3, 0, 1}));
    check_render(accumulated, "x^2+3");

    check_render(Polynomial({0, 1}), "x");
    check_render(Polynomial({0, -1, 1}), "x^2-x");
    check_render(Polynomial({5}), "5");
    check_render(Polynomial({-1}), "-1");
    check_render(Polynomial({0, 0, 2}), "2x^2");
    check_render(Polynomial({0, 0, 0}), "0");

    const Polynomial negative = -p;
    CZ_CHECK(negative == Polynomial({1, -2, -3}));
    CZ_CHECK(p != q);

    return cztest_summary();
}

// I AM NOT DONE
