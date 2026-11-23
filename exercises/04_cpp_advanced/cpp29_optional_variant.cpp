// std::optional<T> is "a T or nothing"; std::variant<A, B, C> is "exactly one
// of A, B, C". Both replace the sentinel values C++ code used to invent.
//
// Note: optional stores its value plus a flag you inspect, and variant's
// alternatives are checked at runtime with holds_alternative/get, or handled
// uniformly with std::visit.
//
// Task: parse_int accepts an optional '-' plus at least one digit and returns
// std::nullopt for anything else (no exception, no sentinel). pick returns an
// int or a std::string. describe names the stored alternative and its value.
//
// From TS: `T | undefined` and discriminated unions exist only at the type
// level. optional and variant store a runtime tag alongside the value and are
// copied as values.
#include "cztest.h"

#include <cstddef>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

std::optional<int> parse_int(const std::string& text)
{
    // TODO: std::stoi accepts a numeric prefix, so "4x2" must not parse.
    return std::stoi(text);
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
            // TODO: handle double and std::string too, with if constexpr.
            if constexpr (std::is_same_v<T, int>) {
                return "int " + std::to_string(item);
            } else {
                (void)item;
                return "unknown";
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

// I AM NOT DONE
