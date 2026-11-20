// std::optional<T> is "a T or nothing"; std::variant<A, B, C> is "exactly one
// of A, B, C". Both replace the sentinel values C++ code used to invent.
//
// Note: optional stores its value plus a flag you inspect, and variant's
// alternatives are checked at runtime with holds_alternative/get, or handled
// uniformly with std::visit.
#include "cztest.h"

#include <cstddef>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

std::optional<int> parse_int(const std::string& text)
{
    if (text.empty()) {
        return std::nullopt;
    }
    std::size_t index = 0;
    bool negative = false;
    if (text[0] == '-') {
        negative = true;
        index = 1;
    }
    if (index == text.size()) {
        return std::nullopt;
    }
    int value = 0;
    for (; index < text.size(); ++index) {
        const char digit = text[index];
        if (digit < '0' || digit > '9') {
            return std::nullopt;
        }
        value = value * 10 + (digit - '0');
    }
    return negative ? -value : value;
}

std::variant<int, double, std::string> pick(bool number)
{
    if (number) {
        return 7;
    }
    return std::string("seven");
}

std::string describe(const std::variant<int, double, std::string>& value)
{
    return std::visit(
        [](const auto& item) -> std::string {
            using T = std::decay_t<decltype(item)>;
            if constexpr (std::is_same_v<T, int>) {
                return "int " + std::to_string(item);
            } else if constexpr (std::is_same_v<T, double>) {
                std::ostringstream out;
                out << item;
                return "double " + out.str();
            } else {
                return "string " + item;
            }
        },
        value);
}

int main()
{
    const std::optional<int> valid = parse_int("-42");
    CZ_CHECK(valid.has_value() && *valid == -42);

    CZ_CHECK(!parse_int("4x2").has_value());

    const auto number = pick(true);
    CZ_CHECK(std::holds_alternative<int>(number) && std::get<int>(number) == 7);

    const std::variant<int, double, std::string> real = 2.5;
    const std::string real_text = describe(real);
    CZ_CHECK_STR(real_text.c_str(), "double 2.5");

    const std::string word_text = describe(pick(false));
    CZ_CHECK_STR(word_text.c_str(), "string seven");
    return cztest_summary();
}
