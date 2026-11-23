// Exceptions vs error codes: a sentinel is easy to ignore, so a function that
// cannot produce a value throws, and the exception type carries the reason. A
// custom hierarchy lets a caller catch one base or one specific failure.
//
// Note: the type of the exception is the error taxonomy, `noexcept` marks
// functions that must not unwind, and a failed operation can promise to change
// nothing.
//
// Task: to_positive throws InvalidArgument for non-positive input;
// find_index throws NotFound when absent; FixedBuffer::push throws
// std::overflow_error when full and must leave the buffer untouched; is_even is
// noexcept.
//
// From TS: throw/catch is untyped at runtime and `any` can slip through. C++
// selects handlers by exception type, and `noexcept` is part of the contract.
#include "cztest.h"

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

class AppError : public std::runtime_error {
public:
    explicit AppError(const std::string& message) : std::runtime_error(message) {}
};

class InvalidArgument : public AppError {
public:
    explicit InvalidArgument(const std::string& message) : AppError(message) {}
};

class NotFound : public AppError {
public:
    explicit NotFound(const std::string& message) : AppError(message) {}
};

int to_positive(int value)
{
    // TODO: throw InvalidArgument instead of returning a sentinel.
    return value <= 0 ? 0 : value;
}

std::size_t find_index(const std::vector<int>& values, int needle)
{
    // TODO: throw NotFound when the loop ends without a match.
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (values[i] == needle) { return i; }
    }
    return values.size();
}

bool is_even(int value) { return value % 2 == 0; }

class FixedBuffer {
public:
    void push(int value)
    {
        // TODO: throw std::overflow_error when full, before touching data_.
        if (size_ == capacity_) { data_[capacity_ - 1] = value; return; }
        data_[size_++] = value;
    }

    std::size_t size() const { return size_; }
    int at(std::size_t index) const { return data_[index]; }

private:
    static constexpr std::size_t capacity_ = 3;
    int data_[capacity_] = {};
    std::size_t size_ = 0;
};

int main()
{
    bool rejected = false;
    try { (void)to_positive(-1); } catch (const InvalidArgument&) { rejected = true; }
    CZ_CHECK(rejected);

    bool caught_as_base = false;
    try { (void)find_index(std::vector<int>{1, 2, 3}, 9); } catch (const AppError&) { caught_as_base = true; }
    CZ_CHECK(caught_as_base);

    CZ_CHECK_INT(find_index(std::vector<int>{4, 5, 6}, 5), 1);

    FixedBuffer buffer;
    buffer.push(1);
    buffer.push(2);
    buffer.push(3);
    bool full = false;
    try { buffer.push(4); } catch (const std::overflow_error&) { full = true; }
    CZ_CHECK(full && buffer.size() == 3 && buffer.at(2) == 3);

    CZ_CHECK(noexcept(is_even(4)) && is_even(4));
    return cztest_summary();
}

// I AM NOT DONE
