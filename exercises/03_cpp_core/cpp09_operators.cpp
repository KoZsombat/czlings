// cpp09_operators -- operator overloading.
//
// Note: C++ lets you give your own types operator syntax, but the meaning
// should stay intuitive: `+` combines, `==` compares all state, `[]` indexes.
//
// Task: complete +, ==, != and += for Vec2, and return the requested element
// from IntList::operator[] (as a reference, so assignment works).

#include "cztest.h"

struct Vec2 {
    int x;
    int y;
};

Vec2 operator+(const Vec2& a, const Vec2& b) {
    return Vec2{a.x + b.x, a.y};  // TODO: add both components
}
bool operator==(const Vec2& a, const Vec2& b) {
    return a.x == b.x;  // TODO: compare both components
}
bool operator!=(const Vec2& a, const Vec2& b) {
    (void)a;
    (void)b;
    return false;  // TODO: define as !(a == b)
}
Vec2& operator+=(Vec2& a, const Vec2& b) {
    a.x += b.x;  // TODO: also update y
    return a;
}

class IntList {
public:
    IntList(int a, int b, int c) : data_{a, b, c} {}
    int& operator[](int index) {
        (void)index;
        return data_[0];  // TODO: use index
    }
    int operator[](int index) const { return data_[index]; }

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

// I AM NOT DONE
