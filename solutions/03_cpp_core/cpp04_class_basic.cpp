// cpp04_class_basic -- a class with private state and const member functions.
//
// Note: a const Rectangle (or a const reference to one) may only call const
// member functions, so every read-only getter must be marked const.

#include <string>

#include "cztest.h"

class Rectangle {
public:
    Rectangle(int width, int height) : width_(width), height_(height) {}

    int width() const {
        return width_;
    }

    int height() const {
        return height_;
    }

    int area() const {
        return width_ * height_;
    }

    int perimeter() const {
        return 2 * width_ + 2 * height_;
    }

    bool is_square() const {
        return width_ == height_;
    }

private:
    int width_;
    int height_;
};

int main() {
    Rectangle r(3, 4);
    CZ_CHECK_INT(r.area(), 12);
    CZ_CHECK_INT(r.perimeter(), 14);
    CZ_CHECK_INT(r.is_square(), 0);

    const Rectangle& square = Rectangle(5, 5);
    CZ_CHECK_INT(square.area(), 25);
    CZ_CHECK_INT(square.is_square(), 1);

    return cztest_summary();
}
