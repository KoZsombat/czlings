// Class templates: FixedStack<T, N> stores up to N values with no heap use.
//
// Note: `FixedStack<int, 3>` and `FixedStack<std::string, 3>` are distinct
// types whose storage is sized at compile time, because type and capacity are
// template arguments. An empty pop() has no value to return, so it must throw.
//
// push() throws std::overflow_error when full; pop() and top() throw
// std::out_of_range when empty.
#include "cztest.h"

#include <array>
#include <cstddef>
#include <stdexcept>

template <typename T, std::size_t N>
class FixedStack {
public:
    void push(const T& value)
    {
        if (size_ == N) {
            throw std::overflow_error("stack is full");
        }
        data_[size_++] = value;
    }

    T pop()
    {
        if (size_ == 0) {
            throw std::out_of_range("stack is empty");
        }
        return data_[--size_];
    }

    T& top()
    {
        if (size_ == 0) {
            throw std::out_of_range("stack is empty");
        }
        return data_[size_ - 1];
    }

    const T& top() const
    {
        if (size_ == 0) {
            throw std::out_of_range("stack is empty");
        }
        return data_[size_ - 1];
    }

    std::size_t size() const { return size_; }

private:
    std::array<T, N> data_{};
    std::size_t size_ = 0;
};

int main()
{
    FixedStack<int, 3> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    CZ_CHECK_INT(stack.size(), 3);

    bool overflow = false;
    try {
        stack.push(4);
    } catch (const std::overflow_error&) {
        overflow = true;
    }
    CZ_CHECK(overflow);

    CZ_CHECK_INT(stack.pop(), 3);
    CZ_CHECK_INT(stack.top(), 2);

    stack.pop();
    stack.pop();
    bool underflow = false;
    try {
        stack.pop();
    } catch (const std::out_of_range&) {
        underflow = true;
    }
    CZ_CHECK(underflow);
    return cztest_summary();
}
