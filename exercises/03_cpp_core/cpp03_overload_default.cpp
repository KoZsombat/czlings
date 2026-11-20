// cpp03_overload_default -- default arguments vs overloading.
//
// Coming from TypeScript: `f(a, b = 2)` is one function with an optional
// parameter. C++ also allows that, but a *default argument* and a one-argument
// overload both match `scaled(5)`, so the call is ambiguous and rejected at
// compile time.
//
// Task: delete the redundant one-argument int overload (the default already
// covers it) and make the string overload return "[text]".

#include <string>

#include "cztest.h"

int scaled(int value) {
    return value;
}

int scaled(int value, int factor = 2) {
    return value * factor;
}

std::string scaled(const std::string& text) {
    return text;
}

int main() {
    CZ_CHECK_INT(scaled(5), 10);
    CZ_CHECK_INT(scaled(5, 3), 15);
    const std::string tagged = scaled(std::string("hi"));
    CZ_CHECK_STR(tagged.c_str(), "[hi]");

    return cztest_summary();
}

// I AM NOT DONE
