// cpp17_exceptions -- throw, catch, custom exceptions, stack unwinding.
//
// Coming from TypeScript: `throw`/`catch` look familiar, but C++ destroys every
// fully constructed local between the throw and the matching catch, in reverse
// order. That unwinding is what makes RAII safe. Catch exceptions by const
// reference; a custom exception derives from std::runtime_error.
//
// Task: throw ValidationError("age must be positive") for negative ages and
// ValidationError("age is unrealistic") above 150; valid ages return normally.

#include <stdexcept>
#include <string>

#include "cztest.h"

class ValidationError : public std::runtime_error {
public:
    explicit ValidationError(const std::string& message) : std::runtime_error(message) {}
};

std::string g_log;

struct Tracer {
    explicit Tracer(char id) : id_(id) {
        g_log += id_;
    }

    ~Tracer() {
        g_log += static_cast<char>(id_ - 'A' + 'a');
    }

private:
    char id_;
};

void validate_age(int age) {
    (void)age;  // TODO: throw for invalid ages
}

int main() {
    int caught = 0;
    try {
        Tracer a('A');
        Tracer b('B');
        validate_age(-1);
    } catch (const ValidationError& error) {
        ++caught;
        CZ_CHECK_STR(error.what(), "age must be positive");
    }
    CZ_CHECK_INT(caught, 1);
    CZ_CHECK_STR(g_log.c_str(), "ABba");

    try {
        validate_age(200);
        CZ_CHECK_MSG(false, "200 must be rejected");
    } catch (const ValidationError& error) {
        CZ_CHECK_STR(error.what(), "age is unrealistic");
    }

    try {
        validate_age(30);
        CZ_CHECK(true);
    } catch (...) {
        CZ_CHECK_MSG(false, "30 is a valid age");
    }

    return cztest_summary();
}

// I AM NOT DONE
