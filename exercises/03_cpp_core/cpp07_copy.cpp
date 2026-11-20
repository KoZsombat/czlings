// cpp07_copy -- the rule of three: deep copies for an owning class.
//
// Note: C++ copies by value, and the compiler-generated copy is memberwise:
// two objects would own one allocation. You must write the copy constructor
// and copy assignment yourself (with the destructor: rule of three).
//
// Task: both copies allocate their own buffer, copy every element, resize the
// target, and survive `x = x`.

#include "cztest.h"

class IntArray {
public:
    explicit IntArray(int size) : size_(size), data_(new int[size]) {
        for (int i = 0; i < size_; ++i) data_[i] = 0;
    }
    IntArray(const IntArray& other)
        : size_(other.size_), data_(new int[other.size_]) {
        if (size_ > 0) data_[0] = other.data_[0];  // TODO: copy every element
    }
    IntArray& operator=(const IntArray& other) {
        if (this != &other) {
            // TODO: allocate, copy all elements, delete the old buffer
        }
        return *this;
    }
    ~IntArray() { delete[] data_; }
    int size() const { return size_; }
    int& at(int index) { return data_[index]; }
    int at(int index) const { return data_[index]; }

private:
    int size_;
    int* data_;
};

int main() {
    IntArray a(3);
    a.at(0) = 1;
    a.at(1) = 2;
    a.at(2) = 3;
    IntArray b = a;
    CZ_CHECK(b.size() == 3 && b.at(1) == 2);
    a.at(1) = 99;
    CZ_CHECK(b.at(1) == 2 && a.at(1) == 99);
    b.at(2) = 30;
    CZ_CHECK(a.at(2) == 3);
    IntArray c(2);
    c = a;
    CZ_CHECK(c.size() == 3 && c.at(1) == 99);
    IntArray* alias = &c;
    c = *alias;
    CZ_CHECK(c.at(1) == 99);
    return cztest_summary();
}

// I AM NOT DONE
