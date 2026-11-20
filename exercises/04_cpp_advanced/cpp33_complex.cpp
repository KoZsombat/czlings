// Value types with operators: Complex is copied, compared with ==, combined
// with + and +=, and streamed with <<. The stream operator is a friend.
//
// Note: operator+ returns a new value, operator+= mutates and returns *this,
// and operator<< is found by argument-dependent lookup because it is declared
// next to the class.
//
// Task: fix operator+= (both components) and make operator<< print
// "re+imi", using '-' when the imaginary part is negative.
#include "cztest.h"

#include <ostream>
#include <sstream>
#include <string>

class Complex {
public:
    Complex(double re = 0.0, double im = 0.0) : re_(re), im_(im) {}
    double real() const { return re_; }
    double imag() const { return im_; }

    Complex& operator+=(const Complex& other)
    {
        // TODO: add both components.
        re_ += other.re_;
        return *this;
    }

    Complex operator+(const Complex& other) const
    {
        Complex result(*this);
        result += other;
        return result;
    }

    bool operator==(const Complex& other) const { return re_ == other.re_ && im_ == other.im_; }

    friend std::ostream& operator<<(std::ostream& out, const Complex& value)
    {
        // TODO: write the sign and the magnitude of the imaginary part.
        out << value.re_;
        out << '+' << value.im_;
        return out << 'i';
    }

private:
    double re_;
    double im_;
};

std::string to_text(const Complex& value)
{
    std::ostringstream out;
    out << value;
    return out.str();
}

int main()
{
    const Complex sum = Complex(1.0, 2.0) + Complex(3.0, -4.0);
    CZ_CHECK(sum == Complex(4.0, -2.0));
    CZ_CHECK_NEAR(sum.imag(), -2.0, 1e-12);

    Complex total(1.0, 1.0);
    total += Complex(2.0, 3.0);
    CZ_CHECK(total.real() == 3.0 && total.imag() == 4.0);

    const std::string positive = to_text(Complex(3.0, 4.0));
    const std::string negative = to_text(Complex(3.0, -4.0));
    CZ_CHECK_STR(positive.c_str(), "3+4i");
    CZ_CHECK_STR(negative.c_str(), "3-4i");
    return cztest_summary();
}

// I AM NOT DONE
