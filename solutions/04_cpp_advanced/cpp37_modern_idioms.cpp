// Modern C++17 idioms in one place: structured bindings (auto [a, b] = pair),
// if/switch with an initializer (keeps a helper variable's scope tiny),
// std::array (fixed size, no decay to a pointer, knows its size), std::bitset
// (fixed-width bit operations), and [[nodiscard]] (the compiler warns when a
// return value is dropped).
//
// Coming from TypeScript: there is no attribute that turns a dropped return
// value into a warning, `switch` cannot declare its scrutinee, and arrays are
// the same object as any other Array — std::array carries its length in the
// type.
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
        if (value < lo) {
            lo = value;
        }
        if (value > hi) {
            hi = value;
        }
    }
    return {lo, hi};
}

std::string describe_status(int code)
{
    switch (auto level = code / 100; level) {
    case 2:
        return "success";
    case 3:
        return "redirect";
    case 4:
        return "client error";
    case 5:
        return "server error";
    default:
        return "unknown";
    }
}

std::array<int, 3> row_sums(const std::array<std::array<int, 2>, 3>& grid)
{
    std::array<int, 3> sums{};
    for (std::size_t row = 0; row < grid.size(); ++row) {
        for (int value : grid[row]) {
            sums[row] += value;
        }
    }
    return sums;
}

std::bitset<8> to_bits(unsigned value) { return std::bitset<8>(value); }

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
