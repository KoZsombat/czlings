// cpp13_enums_constexpr -- enum class, constexpr, static_assert, auto.
//
// Note: C++ `constexpr` functions and variables are evaluated by the compiler,
// `static_assert` checks them during the build, and `enum class` values are
// scoped and do not implicitly convert to int.
//
// Task: make sum_squares() actually square its input and complete color_name().
//
// From TS: `const enum` is erased and `as const` is a type-level hint.
// `constexpr` actually computes at compile time, and `static_assert` fails the
// build.

#include <array>
#include <string>

#include "cztest.h"

enum class Color { Red, Green, Blue };

constexpr int square(int value) {
    return value * value;
}

constexpr int kSide = 5;
static_assert(square(3) == 9, "square must work in a constant expression");
static_assert(kSide * kSide == 25, "kSide must be a compile-time constant");

std::string color_name(Color color) {
    switch (color) {
    case Color::Red:
        return "red";
    default:
        return "unknown";  // TODO: handle Green and Blue
    }
}

int sum_squares(const std::array<int, 4>& values) {
    int total = 0;
    for (auto value : values) {
        total += static_cast<int>(value > 0);  // TODO: add square(value)
    }
    return total;
}

int main() {
    std::array<int, 4> values{1, 2, 3, 4};
    CZ_CHECK_INT(sum_squares(values), 30);
    CZ_CHECK_INT(square(7), 49);
    const std::string green = color_name(Color::Green);
    CZ_CHECK_STR(green.c_str(), "green");
    const std::string blue = color_name(Color::Blue);
    CZ_CHECK_STR(blue.c_str(), "blue");
    CZ_CHECK(Color::Red != Color::Green);

    return cztest_summary();
}

// I AM NOT DONE
