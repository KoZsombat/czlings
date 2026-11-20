// cpp15_strings_streams -- std::string, istringstream, stoi/stod, string_view.
//
// Note: C++ `std::stoi`/`std::stod` throw on failure and stop at the first
// non-numeric character, so check how much was consumed. A
// std::string_view is a non-owning window: it does not copy and does not keep
// the buffer alive.

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
    input >> record.word >> record.count >> record.price;
    return record;
}

int to_int(const std::string& text, int fallback) {
    try {
        std::size_t used = 0;
        int value = std::stoi(text, &used);
        if (used == text.size()) {
            return value;
        }
    } catch (const std::exception&) {
    }
    return fallback;
}

double to_double(const std::string& text, double fallback) {
    try {
        std::size_t used = 0;
        double value = std::stod(text, &used);
        if (used == text.size()) {
            return value;
        }
    } catch (const std::exception&) {
    }
    return fallback;
}

std::string_view first_token(std::string_view text) {
    std::size_t space = text.find(' ');
    if (space == std::string_view::npos) {
        return text;
    }
    return text.substr(0, space);
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
