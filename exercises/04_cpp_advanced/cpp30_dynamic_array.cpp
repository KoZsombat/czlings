// cpp30_dynamic_array -- capstone: an owning dynamic array.
//
// The class already has the rule of five (destructor, deep copy, move) and a
// working push_back. Your job is the reallocation and shifting API:
//
//   reserve(n)        ensure capacity >= n, preserve the elements
//   insert(i, value)  shift elements right, insert at i, may reallocate
//   erase(i)          shift elements left, drop the element at i
//
// Both insert and erase throw std::out_of_range on an invalid index, and
// insert must accept index == size() (append).
//
// Note: reserve(n) only allocates storage and leaves the size alone. Growing
// allocates a new buffer and moves the elements into it, which invalidates
// every pointer, reference and iterator into the old buffer. insert() and
// erase() shift the tail by one, so both are O(n) and invalidate iterators at
// or after the position they touch.
//
// From TS: push and splice grow the array for you. This class manages capacity
// itself: reserve does not change size, insert and erase shift elements, and
// reallocation invalidates every pointer or iterator into the buffer.
#include "cztest.h"

#include <cstddef>
#include <stdexcept>
#include <utility>

class IntArray {
public:
    IntArray() : data_(new int[1]) {}

    IntArray(const IntArray& other)
        : size_(other.size_), capacity_(other.capacity_),
          data_(new int[other.capacity_ ? other.capacity_ : 1])
    {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    IntArray& operator=(const IntArray& other)
    {
        if (this != &other) {
            IntArray copy(other);
            swap(copy);
        }
        return *this;
    }

    IntArray(IntArray&& other) noexcept
        : size_(other.size_), capacity_(other.capacity_), data_(other.data_)
    {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    IntArray& operator=(IntArray&& other) noexcept
    {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = other.data_;
            other.size_ = 0;
            other.capacity_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    ~IntArray() { delete[] data_; }

    void push_back(int value)
    {
        if (size_ == capacity_) {
            grow(capacity_ ? capacity_ * 2 : 2);
        }
        data_[size_++] = value;
    }

    // TODO: allocate when min_capacity exceeds the current capacity, keeping
    // the first size_ elements.
    void reserve(std::size_t min_capacity)
    {
        (void)min_capacity;
    }

    // TODO: validate the index, grow if full, shift [index, size_) one slot
    // right, store the value, then increment size_.
    void insert(std::size_t index, int value)
    {
        (void)index;
        (void)value;
    }

    // TODO: validate the index, shift (index, size_) one slot left, then
    // decrement size_.
    void erase(std::size_t index)
    {
        (void)index;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    int& operator[](std::size_t index) { return data_[index]; }
    const int& operator[](std::size_t index) const { return data_[index]; }
    int* begin() { return data_; }
    int* end() { return data_ + size_; }

private:
    void swap(IntArray& other) noexcept
    {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

    void grow(std::size_t fresh_capacity)
    {
        int* fresh = new int[fresh_capacity];
        for (std::size_t i = 0; i < size_; ++i) {
            fresh[i] = data_[i];
        }
        delete[] data_;
        data_ = fresh;
        capacity_ = fresh_capacity;
    }

    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
    int* data_ = nullptr;
};

int main()
{
    IntArray values;
    for (int i = 1; i <= 5; ++i) {
        values.push_back(i);
    }

    values.reserve(16);
    CZ_CHECK(values.capacity() >= 16);
    CZ_CHECK_INT(values.size(), 5);
    CZ_CHECK_INT(values[4], 5);

    values.insert(2, 99);
    CZ_CHECK_INT(values.size(), 6);
    CZ_CHECK_INT(values[2], 99);
    CZ_CHECK_INT(values[5], 5);

    values.insert(0, 0);
    values.insert(values.size(), 6);
    CZ_CHECK_INT(values[0], 0);
    CZ_CHECK_INT(values[7], 6);

    values.erase(1);
    CZ_CHECK_INT(values.size(), 7);
    CZ_CHECK_INT(values[1], 2);
    CZ_CHECK_INT(values[6], 6);

    bool threw = false;
    try {
        values.erase(values.size());
    } catch (const std::out_of_range&) {
        threw = true;
    }
    CZ_CHECK(threw);

    threw = false;
    try {
        values.insert(100, 1);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    CZ_CHECK(threw);

    IntArray copy = values;
    copy[0] = 42;
    CZ_CHECK(values[0] == 0 && copy[0] == 42);
    return cztest_summary();
}

// I AM NOT DONE
