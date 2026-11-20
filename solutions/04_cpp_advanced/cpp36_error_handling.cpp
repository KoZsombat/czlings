// Exceptions vs error codes: a return-value sentinel is easy to ignore, so
// functions that cannot produce a value throw, and the exception type carries
// the reason. A custom hierarchy lets a caller catch one base class or one
// specific failure. noexcept promises that leaving the function by exception
// terminates the program, which is what move constructors and destructors need.
//
// Note: the type of the exception is the error taxonomy, and the strong
// guarantee (a failed operation changes nothing) is a per-class promise.
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
    if (value <= 0) {
        throw InvalidArgument("value must be positive");
    }
    return value;
}

std::size_t find_index(const std::vector<int>& values, int needle)
{
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (values[i] == needle) {
            return i;
        }
    }
    throw NotFound("value not present");
}

bool is_even(int value) noexcept { return value % 2 == 0; }

class FixedBuffer {
public:
    void push(int value)
    {
        if (size_ == capacity_) {
            throw std::overflow_error("buffer is full");
        }
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
    try {
        (void)to_positive(-1);
    } catch (const InvalidArgument&) {
        rejected = true;
    }
    CZ_CHECK(rejected);

    bool caught_as_base = false;
    try {
        (void)find_index(std::vector<int>{1, 2, 3}, 9);
    } catch (const AppError&) {
        caught_as_base = true;
    }
    CZ_CHECK(caught_as_base);

    CZ_CHECK_INT(find_index(std::vector<int>{4, 5, 6}, 5), 1);

    FixedBuffer buffer;
    buffer.push(1);
    buffer.push(2);
    buffer.push(3);
    bool full = false;
    try {
        buffer.push(4);
    } catch (const std::overflow_error&) {
        full = true;
    }
    CZ_CHECK(full && buffer.size() == 3 && buffer.at(2) == 3);

    CZ_CHECK(noexcept(is_even(4)) && is_even(4));
    return cztest_summary();
}
