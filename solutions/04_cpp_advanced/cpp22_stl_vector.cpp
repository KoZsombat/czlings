// std::vector: contiguous storage, size vs capacity, insert/erase, and the
// iterator-invalidation trap.
//
// Note: a vector iterator is a pointer into the buffer, and erase() moves every
// later element down one, so the old iterators point at the wrong slot. The fix
// is `it = values.erase(it)` without a second increment.
#include "cztest.h"

#include <cstddef>
#include <vector>

std::vector<int> keep_even(const std::vector<int>& input)
{
    std::vector<int> out;
    for (int value : input) {
        if (value % 2 == 0) {
            out.push_back(value);
        }
    }
    return out;
}

void erase_odds(std::vector<int>& values)
{
    for (auto it = values.begin(); it != values.end();) {
        if (*it % 2 != 0) {
            it = values.erase(it);
        } else {
            ++it;
        }
    }
}

int main()
{
    const std::vector<int> input{4, 1, 7, 2, 9};
    const std::vector<int> evens = keep_even(input);
    CZ_CHECK(evens.size() == 2 && evens[0] == 4 && evens[1] == 2);

    std::vector<int> values = input;
    const std::size_t before = values.size();
    values.insert(values.begin() + 1, 99);
    CZ_CHECK_INT(values.size(), before + 1);
    values.erase(values.begin() + 1);

    erase_odds(values);
    CZ_CHECK_INT(values.size(), 2);
    CZ_CHECK(values.front() == 4 && values.back() == 2);

    std::vector<int> grown;
    for (int i = 0; i < 10; ++i) {
        grown.push_back(i);
    }
    CZ_CHECK(grown.size() == 10 && grown.capacity() >= grown.size());
    return cztest_summary();
}
