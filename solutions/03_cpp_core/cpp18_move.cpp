// cpp18_move -- move constructor and move assignment.
//
// Coming from TypeScript: JS has no move semantics; assignment copies a
// reference. C++ `std::move(x)` is just a cast to an rvalue reference: it lets
// the compiler pick a move constructor, which steals x's buffer. The
// moved-from object must stay destructible and assignable; its value is
// unspecified. Moves should be noexcept. This class follows the rule of five.

#include <utility>

#include "cztest.h"

class Box {
public:
    explicit Box(int size) : size_(size), data_(new int[size]()) {}

    Box(const Box& other) : size_(other.size_), data_(new int[other.size_]) {
        for (int i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
        ++copies;
    }

    Box& operator=(const Box& other) {
        if (this != &other) {
            int* fresh = new int[other.size_];
            for (int i = 0; i < other.size_; ++i) {
                fresh[i] = other.data_[i];
            }
            delete[] data_;
            data_ = fresh;
            size_ = other.size_;
            ++copies;
        }
        return *this;
    }

    Box(Box&& other) noexcept : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
        ++moves;
    }

    Box& operator=(Box&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
            ++moves;
        }
        return *this;
    }

    ~Box() {
        delete[] data_;
    }

    int size() const {
        return size_;
    }

    const int* data() const {
        return data_;
    }

    int& at(int index) {
        return data_[index];
    }

    int at(int index) const {
        return data_[index];
    }

    static int copies;
    static int moves;

private:
    int size_;
    int* data_;
};

int Box::copies = 0;
int Box::moves = 0;

int main() {
    Box a(3);
    a.at(0) = 7;
    a.at(1) = 8;

    Box b(std::move(a));
    CZ_CHECK(b.size() == 3 && b.at(0) == 7);
    CZ_CHECK(a.size() == 0 && a.data() == nullptr);
    CZ_CHECK(Box::copies == 0 && Box::moves == 1);

    Box c(1);
    c = std::move(b);
    CZ_CHECK(c.size() == 3 && b.data() == nullptr);
    CZ_CHECK(Box::moves == 2 && c.at(0) == 7);

    return cztest_summary();
}
