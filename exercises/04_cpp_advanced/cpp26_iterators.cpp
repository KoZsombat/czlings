// Iterators are the seam between containers and algorithms: a half-open range
// [first, last). A function template that only uses ++, *, != and == works on
// vector, list and set alike.
//
// Note: the iterator is the cursor, so std::distance may walk a list in O(n)
// while a vector does it in O(1), and reverse iterators are a separate type.
//
// Task: count_value counts every match in [first, last); reversed copies the
// values in reverse with reverse iterators; sorted_unique deduplicates through
// std::set and writes through std::back_inserter.
//
// From TS: the iterator protocol yields values one at a time. C++ splits that
// into begin/end iterator pairs, and algorithms work on any container that
// supports the operations they use.
#include "cztest.h"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <list>
#include <set>
#include <vector>

template <typename Iter, typename T>
std::size_t count_value(Iter first, Iter last, const T& value)
{
    std::size_t count = 0;
    for (Iter it = first; it != last; ++it) {
        if (*it == value) {
            // TODO: count all matches; do not stop at the first one.
            ++count;
            break;
        }
    }
    return count;
}

std::vector<int> reversed(const std::vector<int>& values)
{
    // TODO: copy rbegin()..rend() into a new vector.
    return values;
}

std::vector<int> sorted_unique(const std::vector<int>& values)
{
    // TODO: route the input through std::set before copying it out.
    return values;
}

std::size_t list_size()
{
    const std::list<int> items{1, 2, 3, 4};
    return static_cast<std::size_t>(std::distance(items.begin(), items.end()));
}

int main()
{
    const std::vector<int> values{1, 2, 2, 3};
    const std::list<int> linked{2, 2, 2};
    CZ_CHECK_INT(count_value(values.begin(), values.end(), 2), 2);
    CZ_CHECK_INT(count_value(linked.begin(), linked.end(), 2), 3);

    const std::vector<int> back = reversed(values);
    CZ_CHECK(back.size() == 4 && back[0] == 3 && back[1] == 2 && back[3] == 1);

    const std::vector<int> deduped = sorted_unique(std::vector<int>{3, 1, 3, 2});
    CZ_CHECK(deduped.size() == 3 && deduped[0] == 1 && deduped[2] == 3);

    CZ_CHECK_INT(list_size(), 4);
    return cztest_summary();
}

// I AM NOT DONE
