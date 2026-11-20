// Function templates: one body, many types.
//
// Coming from TypeScript: TS generics are erased before the code runs; C++
// templates are stamped out once per instantiation, so every operation in the
// body must be valid for the type you pass in, and each instantiation is a
// separate function in the binary.
//
// Task: max_of(a, b) returns the larger of two values of the same type T.
// clamp_value(value, lo, hi) returns value limited to [lo, hi]; the result
// keeps the type of `value`, even when the bounds have another type U.
#include "cztest.h"

#include <string>

template <typename T>
T max_of(T a, T b)
{
    (void)b;
    // TODO: return the larger value.
    return a;
}

template <typename T, typename U>
T clamp_value(T value, U lo, U hi)
{
    (void)lo;
    (void)hi;
    // TODO: clamp value into [lo, hi], converting the bound back to T.
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

// I AM NOT DONE
