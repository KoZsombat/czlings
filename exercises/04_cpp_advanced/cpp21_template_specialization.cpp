// Full specialization: Printer<T> has a generic definition, and Printer<bool>
// should have a hand-written one.
//
// Note: the generic body serves every type unless a full specialization for
// that exact type is declared, and the compiler picks one at compile time.
//
// Task: add a full specialization for bool that formats true/false instead of
// 1/0. Place it above main: a specialization must be visible before the first
// use that would instantiate the primary template.
//
// From TS: there is no specialization. C++ chooses the exact template
// definition at compile time, and a specialization must be visible before use.
#include "cztest.h"

#include <sstream>
#include <string>

template <typename T>
struct Printer {
    static std::string format(const T& value)
    {
        std::ostringstream out;
        out << value;
        return out.str();
    }
};

// TODO: specialize Printer<bool>.

int main()
{
    const std::string integer = Printer<int>::format(42);
    const std::string real = Printer<double>::format(3.5);
    const std::string yes = Printer<bool>::format(true);
    const std::string no = Printer<bool>::format(false);

    CZ_CHECK_STR(integer.c_str(), "42");
    CZ_CHECK_STR(real.c_str(), "3.5");
    CZ_CHECK_STR(yes.c_str(), "true");
    CZ_CHECK_STR(no.c_str(), "false");
    return cztest_summary();
}

// I AM NOT DONE
