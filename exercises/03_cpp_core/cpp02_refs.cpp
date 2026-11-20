// cpp02_refs -- references are aliases; pointers are addresses.
//
// Coming from TypeScript: objects are shared by reference, but assigning a
// variable rebinds it. A C++ reference is another name for the *same* object
// and can never be rebound or null. `const T&` passes a large value without
// copying and promises not to modify it.
//
// Task: fix swap_values() (it currently does nothing), and make shout()
// return the text uppercased with a single '!' appended, leaving its argument
// untouched.

#include <cctype>
#include <string>

#include "cztest.h"

void swap_values(int& a, int& b) {
    int tmp = a;
    a = a;  // TODO: assign through the references
    b = b;
    (void)tmp;
}

int sum(const int* data, int n) {
    int total = 0;
    for (int i = 0; i < n; ++i) {
        total += data[i];
    }
    return total;
}

std::string shout(const std::string& text) {
    return text;  // TODO: uppercase every character and append '!'
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

// I AM NOT DONE
