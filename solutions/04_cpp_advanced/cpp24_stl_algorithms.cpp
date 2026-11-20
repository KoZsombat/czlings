// <algorithm> and <numeric> give you loops you do not have to write, but each
// one takes a predicate or comparator with a precise meaning.
//
// Coming from TypeScript: Array#sort compares with a number-returning function
// and is stable; std::sort takes a "less than" predicate returning bool, is not
// stable, and reads the whole range through iterators.
#include "cztest.h"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <vector>

void sort_descending(std::vector<int>& values)
{
    std::sort(values.begin(), values.end(), [](int a, int b) { return a > b; });
}

std::size_t first_negative(const std::vector<int>& values)
{
    const auto it = std::find_if(values.begin(), values.end(), [](int value) { return value < 0; });
    return static_cast<std::size_t>(std::distance(values.begin(), it));
}

std::size_t count_above(const std::vector<int>& values, int limit)
{
    const auto hits = std::count_if(values.begin(), values.end(), [limit](int value) { return value > limit; });
    return static_cast<std::size_t>(hits);
}

int total(const std::vector<int>& values)
{
    return std::accumulate(values.begin(), values.end(), 0);
}

std::vector<int> doubled(const std::vector<int>& values)
{
    std::vector<int> out(values.size());
    std::transform(values.begin(), values.end(), out.begin(), [](int value) { return value * 2; });
    return out;
}

int main()
{
    std::vector<int> values{4, -1, 9, 2, 7};
    sort_descending(values);
    CZ_CHECK(values.front() == 9 && values.back() == -1);

    const std::vector<int> probe{-2, 5, -7, 8};
    CZ_CHECK_INT(first_negative(probe), 0);
    CZ_CHECK_INT(count_above(probe, 0), 2);
    CZ_CHECK_INT(total(probe), 4);
    CZ_CHECK_INT(doubled(probe)[2], -14);
    return cztest_summary();
}
