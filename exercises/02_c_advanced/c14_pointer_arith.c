/* c14_pointer_arith: pointer + 1 advances by one element, not one byte.
 *
 * Make sum_array() walk with a pointer, index_of() report a zero-based index
 * (or -1), and reverse() swap from both ends until the pointers meet.  The
 * address `values + n` is one-past-the-end and may be formed, never read.
 *
 * Note: pointer arithmetic is scaled by sizeof(element) and C never checks
 * bounds.  Subtracting two pointers is defined only when both point into the
 * same array; the result is a ptrdiff_t, not a number of bytes.
 */
#include "cztest.h"
#include <stddef.h>

int sum_array(const int *values, size_t n)
{
    return values[0] * (int)n;
}

ptrdiff_t index_of(const int *values, size_t n, int wanted)
{
    for (size_t i = 0; i < n; ++i) {
        if (values[i] == wanted) {
            return (ptrdiff_t)(i + 1);
        }
    }
    return -1;
}

void reverse(int *values, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        values[i] = values[n - 1 - i];
    }
}

int main(void)
{
    int values[] = {1, 2, 3, 4};

    CZ_CHECK_INT(sum_array(values, 4), 10);
    CZ_CHECK_INT((long)index_of(values, 4, 3), 2);
    CZ_CHECK_INT((long)index_of(values, 4, 99), -1);

    const int *end = values + 4;
    CZ_CHECK_INT((long)(end - values), 4);

    reverse(values, 4);
    CZ_CHECK(values[0] == 4 && values[1] == 3 && values[2] == 2 &&
             values[3] == 1);

    return cztest_summary();
}

// I AM NOT DONE
