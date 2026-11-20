// Function templates: one body, many types.
//
// Coming from TypeScript: TS generics are erased before the code runs; C++
// templates are stamped out once per instantiation, so every operation in the
// body must be valid for the type you pass in, and each instantiation is a
// separate function in the binary.
//
// Implement max_of (two values of the same type T) and clamp_value (a value of
// type T with bounds of type U) so the checks pass.
#include "cztest.h"

#include <string>

template <typename T>
T max_of(T a, T b)
{
    return a < b ? b : a;
}

template <typename T, typename U>
T clamp_value(T value, U lo, U hi)
{
    if (value < lo) {
        return static_cast<T>(lo);
    }
    if (hi < value) {
        return static_cast<T>(hi);
    }
    return value;
}

int main()
{
    CZ_CHECK_INT(max_of(3, 7), 7);
    CZ_CHECK_NEAR(max_of(2.5, 1.5), 2.5, 1e-9);
    CZ_CHECK_INT(clamp_value(15, 0, 10), 10);
    CZ_CHECK_INT(clamp_value(-5, 0, 10), 0);
    CZ_CHECK_NEAR(max_of<double>(3, 4.5), 4.5, 1e-9);
    return cztest_summary();
}
