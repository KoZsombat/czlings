// cpp09_operators -- operator overloading.
//
// Coming from TypeScript: JavaScript has no operator overloading and `==`
// coerces types. In C++ you can give your own types operator syntax, but the
// meaning must stay intuitive: `+` combines, `==` compares all state, `[]`
// indexes.

#include "cztest.h"

struct Vec2 {
    int x;
    int y;
};

Vec2 operator+(const Vec2& a, const Vec2& b) {
    return Vec2{a.x + b.x, a.y + b.y};
}

bool operator==(const Vec2& a, const Vec2& b) {
    return a.x == b.x && a.y == b.y;
}

bool operator!=(const Vec2& a, const Vec2& b) {
    return !(a == b);
}

Vec2& operator+=(Vec2& a, const Vec2& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

class IntList {
public:
    IntList(int a, int b, int c) : data_{a, b, c} {}

    int& operator[](int index) {
        return data_[index];
    }

    int operator[](int index) const {
        return data_[index];
    }

private:
    int data_[3];
};

int main() {
    Vec2 a{1, 2};
    Vec2 b{3, 4};
    Vec2 c = a + b;
    CZ_CHECK(c.x == 4 && c.y == 6);
    CZ_CHECK((a == Vec2{1, 2}) && a != b);

    a += b;
    CZ_CHECK(a.x == 4 && a.y == 6);

    IntList list(10, 20, 30);
    CZ_CHECK(list[1] == 20 && list[0] == 10);
    list[2] = 99;
    CZ_CHECK(list[2] == 99 && list[0] == 10);

    return cztest_summary();
}
