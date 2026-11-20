// cpp10_friend -- friend functions and friend classes.
//
// Coming from TypeScript: `private` is enforced by the class body, and a
// subclass cannot read private fields either. C++ friendship is an explicit
// one-way grant: the friend may touch private members. It is *not* inherited,
// so a class derived from Calibrator could not read Meter::value_ either.
//
// Task: make operator<< print "value m" (for example "42 m") and let
// Calibrator::raw() return the private value.

#include <ostream>
#include <sstream>

#include "cztest.h"

class Meter {
public:
    explicit Meter(int value) : value_(value) {}

    friend std::ostream& operator<<(std::ostream& os, const Meter& meter);
    friend class Calibrator;

private:
    int value_;
};

std::ostream& operator<<(std::ostream& os, const Meter&) {
    return os << "meter";  // TODO: print value_ and " m"
}

class Calibrator {
public:
    static int raw(const Meter&) {
        return 0;  // TODO: read the private value
    }
};

int main() {
    Meter meter(42);
    std::ostringstream out;
    out << meter;
    const std::string printed = out.str();
    CZ_CHECK_STR(printed.c_str(), "42 m");
    CZ_CHECK_INT(Calibrator::raw(meter), 42);

    Meter negative(-3);
    std::ostringstream below;
    below << negative;
    const std::string below_text = below.str();
    CZ_CHECK_STR(below_text.c_str(), "-3 m");

    return cztest_summary();
}

// I AM NOT DONE
