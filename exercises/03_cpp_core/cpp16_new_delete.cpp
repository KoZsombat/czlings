// cpp16_new_delete -- owning a dynamic array.
//
// Note: in C++ `new T[n]` must be matched by `delete[]`, and an owning object
// frees it in its destructor. `std::vector<int>` does exactly this and is the
// normal choice.
//
// Task: complete the destructor: delete[] the buffer and update the live
// counter. Copying is disabled so there is exactly one owner.
//
// From TS: `new` allocates an object and the GC frees it. C++ `new[]` needs a
// matching `delete[]`; `std::vector` is the managed-array equivalent.

#include "cztest.h"

class Buffer {
public:
    explicit Buffer(int size) : size_(size), data_(new int[size]()) {
        ++live_;
    }
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    ~Buffer() {
        // TODO: delete[] data_ and decrement live_
    }

    int& operator[](int index) { return data_[index]; }
    int operator[](int index) const { return data_[index]; }
    int size() const { return size_; }
    int sum() const {
        int total = 0;
        for (int i = 0; i < size_; ++i) total += data_[i];
        return total;
    }
    static int live() { return live_; }

private:
    int size_;
    int* data_;
    static int live_;
};

int Buffer::live_ = 0;

int main() {
    CZ_CHECK_INT(Buffer::live(), 0);
    {
        Buffer buffer(3);
        CZ_CHECK_INT(buffer.size(), 3);
        buffer[0] = 1;
        buffer[1] = 2;
        buffer[2] = 3;
        CZ_CHECK_INT(buffer.sum(), 6);
        CZ_CHECK_INT(Buffer::live(), 1);
    }
    CZ_CHECK_INT(Buffer::live(), 0);
    return cztest_summary();
}

// I AM NOT DONE
