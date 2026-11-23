// C++17 idioms: structured bindings (auto [a, b] = pair), if/switch with an
// initializer, std::array (fixed size, no pointer decay), std::bitset (fixed
// width), and [[nodiscard]] (dropping the result warns).
//
// Note: `switch` can declare its scrutinee in C++17, attributes such as
// [[nodiscard]] turn a dropped return value into a compiler warning, and
// std::array carries its length in the type.
//
// Task: min_max returns {smallest, largest}; describe_status maps the hundreds
// digit with a switch whose scrutinee is declared in the switch (2xx success,
// 3xx redirect, 4xx client error, 5xx server error); to_bits keeps the low 8
// bits.
//
// From TS: destructuring and `switch` are familiar, but `std::array` carries its
// length in the type, `std::bitset` has a fixed width, and `[[nodiscard]]` is
// checked by the compiler.
#include "cztest.h"

#include <array>
#include <bitset>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

[[nodiscard]] std::pair<int, int> min_max(const std::vector<int>& values)
{
    int lo = values.front();
    int hi = values.front();
    for (int value : values) {
        if (value < lo) { lo = value; }
        if (value > hi) { hi = value; }
    }
    // TODO: the order is {min, max}.
    return {hi, lo};
}

std::string describe_status(int code)
{
    // TODO: rewrite as `switch (auto level = code / 100; level)`.
    if (code >= 500) { return "server error"; }
    if (code >= 400) { return "client error"; }
    return code / 100 == 2 ? "success" : "unknown";
}

std::array<int, 3> row_sums(const std::array<std::array<int, 2>, 3>& grid)
{
    std::array<int, 3> sums{};
    for (std::size_t row = 0; row < grid.size(); ++row) {
        for (int value : grid[row]) { sums[row] += value; }
    }
    return sums;
}

std::bitset<8> to_bits(unsigned value)
{
    // TODO: no shift; bitset already keeps the low 8 bits.
    return std::bitset<8>(value << 1);
}

int main()
{
    const auto [lo, hi] = min_max(std::vector<int>{3, -1, 7, 2});
    CZ_CHECK(lo == -1 && hi == 7);

    const std::string missing = describe_status(404);
    const std::string moved = describe_status(301);
    CZ_CHECK_STR(missing.c_str(), "client error");
    CZ_CHECK_STR(moved.c_str(), "redirect");

    const std::array<int, 3> sums = row_sums({{{1, 2}, {3, 4}, {5, 6}}});
    CZ_CHECK_INT(sums[0] + sums[1] + sums[2], 21);

    const std::bitset<8> bits = to_bits(5);
    const std::string text = bits.to_string();
    CZ_CHECK(text == "00000101" && bits.count() == 2);
    return cztest_summary();
}

// I AM NOT DONE
