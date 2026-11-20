// cpp07_copy -- the rule of three: deep copies for an owning class.
//
// Note: C++ copies by value, and the compiler-generated copy is memberwise:
// for a raw pointer that means two objects own one allocation (double free,
// double mutation). You must write the copy constructor and copy assignment
// yourself.

#include "cztest.h"

class IntArray {
public:
    explicit IntArray(int size) : size_(size), data_(new int[size]) {
        for (int i = 0; i < size_; ++i) {
            data_[i] = 0;
        }
    }

    IntArray(const IntArray& other) : size_(other.size_), data_(new int[other.size_]) {
        for (int i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    IntArray& operator=(const IntArray& other) {
        if (this != &other) {
            int* fresh = new int[other.size_];
            for (int i = 0; i < other.size_; ++i) {
                fresh[i] = other.data_[i];
            }
            delete[] data_;
            data_ = fresh;
            size_ = other.size_;
        }
        return *this;
    }

    ~IntArray() {
        delete[] data_;
    }

    int size() const {
        return size_;
    }

    int& at(int index) {
        return data_[index];
    }

    int at(int index) const {
        return data_[index];
    }

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
