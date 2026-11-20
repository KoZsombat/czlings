// cpp03_overload_default -- default arguments vs overloading.
//
// Note: a *default argument* and a one-argument overload both match
// `scaled(5)`, so the call is ambiguous and rejected at compile time.

#include <string>

#include "cztest.h"

int scaled(int value, int factor = 2) {
    return value * factor;
}

std::string scaled(const std::string& text) {
    return "[" + text + "]";
}

int main() {
    CZ_CHECK_INT(scaled(5), 10);
    CZ_CHECK_INT(scaled(5, 3), 15);
    const std::string tagged = scaled(std::string("hi"));
    CZ_CHECK_STR(tagged.c_str(), "[hi]");

    return cztest_summary();
}
