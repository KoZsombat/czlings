/* c15_const: const in a pointer declaration qualifies what is on its left.
 *
 * The declarations below put const in the wrong place.  Fix the parameter
 * types: sum() must accept a read-only array, set_first() must accept a
 * mutable array through a const pointer, and copy_into() must not modify its
 * source.
 *
 *   const int *p          pointer to const int: *p cannot be assigned
 *   int *const p          const pointer: p cannot be assigned
 *   const int *const p    neither p nor *p may be assigned
 *
 * Coming from TypeScript: C const is part of the type and does not imply a
 * deep freeze, but it is checked at compile time.  Unlike TS `readonly`, the
 * position around `*` chooses between "the pointee" and "the pointer".
 */
#include "cztest.h"
#include <stddef.h>

int sum(const int *values, size_t n)
{
    int total = 0;
    for (size_t i = 0; i < n; ++i) {
        total += values[i];
    }
    return total;
}

void set_first(const int *values, int value)
{
    values[0] = value;
}

void copy_into(int *dst, const int *src, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        dst[i] = src[i];
    }
}

int main(void)
{
    int numbers[] = {1, 2, 3, 4};
    CZ_CHECK_INT(sum(numbers, 4), 10);

    int data[] = {7, 8, 9};
    int *const fixed = data;
    set_first(fixed, 1);
    CZ_CHECK_INT(data[0], 1);

    const int *cursor = data;
    cursor = &data[1];
    CZ_CHECK_INT(*cursor, 8);

    const int *const both = &data[2];
    CZ_CHECK_INT(*both, 9);

    int copy[3] = {0, 0, 0};
    copy_into(copy, data, 3);
    CZ_CHECK_INT(copy[1], 8);

    return cztest_summary();
}

// I AM NOT DONE
