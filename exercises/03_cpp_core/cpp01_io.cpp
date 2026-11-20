// cpp01_io -- format text with std::ostringstream.
//
// Note: C++ has no string interpolation. `<<` inserts values into a stream,
// and formatting (fixed notation, precision) is a property of that stream.
//
// Task: make describe() return "name: score/100" and price_tag() return the
// amount with exactly two decimals followed by " EUR".

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "cztest.h"

std::string describe(const std::string& name, int score) {
    std::ostringstream out;
    out << name;  // TODO: append ": ", the score and "/100"
    return out.str();
}

std::string price_tag(double amount) {
    std::ostringstream out;
    out << amount << " EUR";  // TODO: use fixed notation with 2 decimals
    return out.str();
}

int main() {
    std::string line = describe("ada", 92);
    CZ_CHECK_STR(line.c_str(), "ada: 92/100");
    line = describe("bob", 7);
    CZ_CHECK_STR(line.c_str(), "bob: 7/100");
    line = price_tag(12.5);
    CZ_CHECK_STR(line.c_str(), "12.50 EUR");
    line = price_tag(0.0);
    CZ_CHECK_STR(line.c_str(), "0.00 EUR");

    return cztest_summary();
}

// I AM NOT DONE
