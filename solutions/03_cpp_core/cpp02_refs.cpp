// cpp02_refs -- references are aliases; pointers are addresses.
//
// Coming from TypeScript: objects are shared by reference, but assigning a
// variable rebinds it. A C++ reference is another name for the *same* object
// and can never be rebound or null. `const T&` passes a large value without
// copying and promises not to modify it.

#include <cctype>
#include <string>

#include "cztest.h"

void swap_values(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

int sum(const int* data, int n) {
    int total = 0;
    for (int i = 0; i < n; ++i) {
        total += data[i];
    }
    return total;
}

std::string shout(const std::string& text) {
    std::string out;
    for (char ch : text) {
        out += static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    out += '!';
    return out;
}

int main() {
    int x = 1;
    int y = 2;
    swap_values(x, y);
    CZ_CHECK_INT(x, 2);
    CZ_CHECK_INT(y, 1);

    const int numbers[] = {4, 5, 6};
    CZ_CHECK_INT(sum(numbers, 3), 15);

    const std::string greeting = "hi";
    const std::string shouted = shout(greeting);
    CZ_CHECK_STR(shouted.c_str(), "HI!");
    CZ_CHECK_STR(greeting.c_str(), "hi");

    return cztest_summary();
}
