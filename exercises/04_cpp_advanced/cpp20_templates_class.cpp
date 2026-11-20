// FixedStack<T, N> is a class template: one definition, a distinct type and
// storage size per T and N. push() throws std::overflow_error when full;
// pop() and top() throw std::out_of_range when empty.
//
// Coming from TypeScript: class Stack<T> holds anything with one runtime shape;
// here type and capacity are compile-time facts, and an empty pop() must throw
// instead of returning undefined.
#include "cztest.h"

#include <array>
#include <cstddef>
#include <stdexcept>

template <typename T, std::size_t N>
class FixedStack {
public:
    void push(const T& value)
    {
        // TODO: reject a push into a full stack instead of dropping the value.
        if (size_ < N) { data_[size_++] = value; }
    }

    T pop()
    {
        // TODO: reject a pop from an empty stack instead of returning T{}.
        if (size_ == 0) { return T{}; }
        return data_[--size_];
    }

    T& top() { return data_[size_ - 1]; }
    const T& top() const { return data_[size_ - 1]; }
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
    try { stack.push(4); } catch (const std::overflow_error&) { overflow = true; }
    CZ_CHECK(overflow);

    CZ_CHECK_INT(stack.pop(), 3);
    CZ_CHECK_INT(stack.top(), 2);

    stack.pop();
    stack.pop();
    bool underflow = false;
    try { stack.pop(); } catch (const std::out_of_range&) { underflow = true; }
    CZ_CHECK(underflow);
    return cztest_summary();
}

// I AM NOT DONE
