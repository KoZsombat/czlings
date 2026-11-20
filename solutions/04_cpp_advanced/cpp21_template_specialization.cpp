// Full specialization: Printer<T> has a generic definition, Printer<bool> a
// hand-written one.
//
// Note: the generic body serves every type unless a full specialization for
// that exact type is declared, and the compiler picks one at compile time.
//
// The specialization must appear before the first use that would instantiate
// the primary template: instantiation happens at the point of use, and an
// already-instantiated specialization declared later is ill-formed (and some
// compilers silently keep the generic version).
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

template <>
struct Printer<bool> {
    static std::string format(bool value)
    {
        return value ? "true" : "false";
    }
};

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
