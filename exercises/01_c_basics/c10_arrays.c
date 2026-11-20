/* c10_arrays -- fixed arrays, traversal, in-place reverse.
 *
 * Task: implement sum, max, and reverse for arrays passed with an explicit
 * length. In main, ARRAY_LEN works because the array is still an array; once
 * passed to a function it decays to a pointer and the idiom returns garbage.
 *
 * Coming from TypeScript: arrays do not carry their length in C, and there
 * are no bounds checks. The callee must receive the element count.
 */
#include "cztest.h"
#include <stddef.h>

#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

int array_sum(const int *arr, size_t n)
{
    int total = 0;
    for (size_t i = 0; i + 1 < n; ++i) {
        total += arr[i];
    }
    return total;
}

int array_max(const int *arr, size_t n)
{
    int best = 0;
    for (size_t i = 1; i < n; ++i) {
        if (arr[i] > best) {
            best = arr[i];
        }
    }
    return best;
}

void reverse_array(int *arr, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        int tmp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = tmp;
    }
}

int main(void)
{
    int data[] = {3, -1, 7, 2};
    int neg[] = {-5, -2, -9};
    int odd[] = {1, 2, 3, 4, 5};
    int even[] = {1, 2, 3, 4};
    int expect_odd[] = {5, 4, 3, 2, 1};
    int expect_even[] = {4, 3, 2, 1};

    CZ_CHECK_INT(ARRAY_LEN(data), 4);
    CZ_CHECK_INT(array_sum(data, ARRAY_LEN(data)), 11);
    CZ_CHECK(array_max(data, ARRAY_LEN(data)) == 7 &&
             array_max(neg, ARRAY_LEN(neg)) == -2);

    reverse_array(odd, ARRAY_LEN(odd));
    CZ_CHECK_MEM(odd, expect_odd, sizeof odd);

    reverse_array(even, ARRAY_LEN(even));
    CZ_CHECK_MEM(even, expect_even, sizeof even);

    return cztest_summary();
}

// I AM NOT DONE
