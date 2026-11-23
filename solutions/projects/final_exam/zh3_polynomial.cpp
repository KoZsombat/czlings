// zh3_polynomial -- final ZH, task 3 (15 points). Reference solution.
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
        if (other.coeffs_.size() > coeffs_.size()) {
            coeffs_.resize(other.coeffs_.size(), 0.0);
        }
        for (std::size_t i = 0; i < other.coeffs_.size(); ++i) {
            coeffs_[i] += other.coeffs_[i];
        }
        trim();
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
        double result = 0.0;
        for (int i = degree(); i >= 0; --i) {
            result = result * x + coeffs_[static_cast<std::size_t>(i)];
        }
        return result;
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
        if (left.degree() < 0 || right.degree() < 0) {
            return Polynomial();
        }

        std::vector<double> result(
            static_cast<std::size_t>(left.degree() + right.degree() + 1), 0.0);
        for (std::size_t i = 0; i < left.coeffs_.size(); ++i) {
            for (std::size_t j = 0; j < right.coeffs_.size(); ++j) {
                result[i + j] += left.coeffs_[i] * right.coeffs_[j];
            }
        }
        return Polynomial(std::move(result));
    }

    friend std::ostream& operator<<(std::ostream& out, const Polynomial& poly)
    {
        if (poly.coeffs_.empty()) {
            return out << '0';
        }

        bool first = true;
        for (int degree = poly.degree(); degree >= 0; --degree) {
            const double coefficient =
                poly.coeffs_[static_cast<std::size_t>(degree)];
            if (coefficient == 0.0) {
                continue;
            }

            const bool negative = coefficient < 0.0;
            const double magnitude = negative ? -coefficient : coefficient;
            if (first) {
                if (negative) {
                    out << '-';
                }
            } else {
                out << (negative ? '-' : '+');
            }

            if (magnitude != 1.0 || degree == 0) {
                out << magnitude;
            }
            if (degree >= 1) {
                out << 'x';
            }
            if (degree >= 2) {
                out << '^' << degree;
            }
            first = false;
        }

        if (first) {
            out << '0';
        }
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

    const Polynomial p({-1, 2, 3});
    CZ_CHECK_INT(p.degree(), 2);
    CZ_CHECK_NEAR(p(2.0), 15.0, 1e-9);
    CZ_CHECK_NEAR(p(0.0), -1.0, 1e-9);

    const Polynomial q({1, 1});
    const Polynomial sum = p + q;
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
