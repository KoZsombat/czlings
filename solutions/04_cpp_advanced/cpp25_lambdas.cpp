// Lambdas: capture by value copies at creation time, capture by reference
// aliases the local, `mutable` lets a value capture be modified, and
// std::function stores any callable with the matching signature (type erasure).
//
// Coming from TypeScript: closing over a `let` keeps a live binding to the
// variable. A C++ lambda capturing by value snapshots it; capturing by
// reference keeps a pointer, so a returned lambda that captured a local by
// reference dangles exactly like a JS closure over a loop variable would not.
#include "cztest.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>

std::function<int()> make_counter()
{
    return [count = 0]() mutable { return ++count; };
}

std::function<int(int)> make_adder(int base)
{
    return [base](int value) { return base + value; };
}

void sort_by_abs(std::vector<int>& values)
{
    std::sort(values.begin(), values.end(), [](int a, int b) { return std::abs(a) < std::abs(b); });
}

int add_twice_through_reference(int start, int delta)
{
    int total = start;
    const auto add = [&total](int amount) { total += amount; };
    add(delta);
    add(delta);
    return total;
}

int sum_above(const std::vector<int>& values, int limit)
{
    const auto above = [limit](int value) { return value > limit; };
    int sum = 0;
    for (int value : values) {
        if (above(value)) {
            sum += value;
        }
    }
    return sum;
}

int main()
{
    auto counter = make_counter();
    CZ_CHECK(counter() == 1 && counter() == 2);

    const auto add10 = make_adder(10);
    CZ_CHECK_INT(add10(5), 15);

    std::vector<int> values{-5, 2, -1, 3};
    sort_by_abs(values);
    CZ_CHECK(values.front() == -1 && values.back() == -5);

    CZ_CHECK_INT(add_twice_through_reference(10, 5), 20);
    CZ_CHECK_INT(sum_above(values, 0), 5);
    return cztest_summary();
}
