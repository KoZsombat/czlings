// cpp15_strings_streams -- std::string, istringstream, stoi/stod, string_view.
//
// Note: C++ stoi/stod throw on failure and stop at the first non-numeric
// character, so check how much was consumed. std::string_view is a non-owning
// window: no copy, and the buffer must outlive it.
//
// Task: parse one record ("word count price"), convert with a fallback on
// malformed input, and return the first space-delimited token as a view.
//
// From TS: parseInt returns NaN and split allocates. Here stoi throws and may
// consume only a prefix, while string_view is a non-owning slice with no
// lifetime safety.

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "cztest.h"

struct Record {
    std::string word;
    int count;
    double price;
};

Record parse_record(const std::string& line) {
    Record record{"", 0, 0.0};
    std::istringstream input(line);
    input >> record.word;  // TODO: also extract count and price
    return record;
}

int to_int(const std::string& text, int fallback) {
    (void)fallback;
    try {
        return std::stoi(text);
    } catch (const std::exception&) {
        return 0;  // TODO: return fallback
    }
}

double to_double(const std::string& text, double fallback) {
    (void)fallback;
    try {
        return std::stod(text);
    } catch (const std::exception&) {
        return 0.0;  // TODO: return fallback
    }
}

std::string_view first_token(std::string_view text) {
    return text;  // TODO: cut at the first space
}

int main() {
    Record record = parse_record("apple 3 1.25");
    CZ_CHECK(record.word == "apple" && record.count == 3 &&
             record.price > 1.24 && record.price < 1.26);
    CZ_CHECK_INT(to_int("42", -1), 42);
    CZ_CHECK_INT(to_int("12x", -1), -1);
    CZ_CHECK_NEAR(to_double("abc", -1.0), -1.0, 1e-9);
    const std::string line = "hello world";
    std::string_view token = first_token(line);
    CZ_CHECK(token == "hello" && token.data() == line.data());
    return cztest_summary();
}

// I AM NOT DONE
