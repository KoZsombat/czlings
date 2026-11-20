// cpp16_new_delete -- owning a dynamic array.
//
// Note: in C++ `new T[n]` must be matched by `delete[]`, and an object that
// owns that buffer must free it in its destructor. `std::vector<int>` does
// exactly this and is the normal choice; here you build the mechanism once.

#include "cztest.h"

class Buffer {
public:
    explicit Buffer(int size) : size_(size), data_(new int[size]()) {
        ++live_;
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    ~Buffer() {
        delete[] data_;
        --live_;
    }

    int& operator[](int index) {
        return data_[index];
    }

    int operator[](int index) const {
        return data_[index];
    }

    int size() const {
        return size_;
    }

    int sum() const {
        int total = 0;
        for (int i = 0; i < size_; ++i) {
            total += data_[i];
        }
        return total;
    }

    static int live() {
        return live_;
    }

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
