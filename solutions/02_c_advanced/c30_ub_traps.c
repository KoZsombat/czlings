/* c30_ub_traps: undefined behaviour can look like it works -- until it does not.
 *
 * Fix four defects.  sum_three() reads values[2] without initialising it;
 * fill_three() writes out[3] past a three-element array and reads it back;
 * slot() returns the address of a local; checked_add() performs signed
 * overflow.  Make all four behave as the tests require.
 *
 * Note: reading an uninitialised local, indexing past the end of an array,
 * and returning the address of a local are all undefined behaviour; the
 * returned pointer dangles once the function returns.  Signed integer
 * overflow is also undefined behaviour, not wraparound: the result cannot be
 * relied on.  <limits.h> provides INT_MAX and INT_MIN for bounds checks.
 */
#include "cztest.h"
#include <limits.h>
#include <stdlib.h>

int sum_three(void)
{
    int values[3] = {10, 20, 12};
    return values[0] + values[1] + values[2];
}

int fill_three(int *out)
{
    for (int i = 0; i < 3; ++i) {
        out[i] = i + 1;
    }
    return out[0] + out[1] + out[2];
}

int *slot(void)
{
    static int stable = 7;
    return &stable;
}

int checked_add(int a, int b, int *out)
{
    if (b > 0 && a > INT_MAX - b) {
        return -1;
    }
    if (b < 0 && a < INT_MIN - b) {
        return -1;
    }
    *out = a + b;
    return 0;
}

int main(void)
{
    CZ_CHECK_INT(sum_three(), 42);

    int *cells = malloc(3 * sizeof *cells);
    int total = cells == NULL ? -1 : fill_three(cells);
    CZ_CHECK(cells != NULL && total == 6 && cells[0] == 1 && cells[1] == 2 &&
             cells[2] == 3);
    free(cells);

    int *p = slot();
    CZ_CHECK(p != NULL && *p == 7);

    int out = 123;
    CZ_CHECK(checked_add(INT_MAX, 1, &out) == -1 && out == 123);

    int sum = 0;
    CZ_CHECK(checked_add(2, 3, &sum) == 0 && sum == 5);

    return cztest_summary();
}
