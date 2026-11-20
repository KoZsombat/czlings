// Lambdas capture by value (a snapshot) or by reference (an alias); `mutable`
// lets a value capture change, and std::function stores any matching callable.
//
// Note: a lambda capturing by value snapshots the variable at creation time,
// while capturing by reference keeps a pointer, so a returned lambda that
// captured a local by reference dangles.
//
// Task: make_counter counts 1, 2, 3... across calls; make_adder adds a fixed
// base; sort_by_abs orders by absolute value; the reference capture must
// actually update `total`.
#include "cztest.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>

std::function<int()> make_counter()
{
    // TODO: a value capture without `mutable` is read-only, so this never moves.
    return [count = 0]() { return count; };
}

std::function<int(int)> make_adder(int base)
{
    return [base](int value) { return base + value; };
}

void sort_by_abs(std::vector<int>& values)
{
    // TODO: compare absolute values, not the raw numbers.
    std::sort(values.begin(), values.end(), [](int a, int b) { return a < b; });
}

int add_twice_through_reference(int start, int delta)
{
    int total = start;
    // TODO: capture by reference; with a value capture the caller sees nothing.
    auto add = [total](int amount) mutable { total += amount; };
    add(delta);
    add(delta);
    return total;
}

int sum_above(const std::vector<int>& values, int limit)
{
    const auto above = [limit](int value) { return value > limit; };
    int sum = 0;
    for (int value : values) { if (above(value)) { sum += value; } }
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

// I AM NOT DONE
