/* c06_loops -- for, while, do-while.
 *
 * Task: sum 1..n with a for loop, count decimal digits with a do-while (zero
 * has one digit), and reverse the digits of a nonnegative int with a while.
 *
 * Note: for a nonnegative int, `n % 10` extracts the last decimal digit and
 * `n / 10` drops it; both are integer operations, with no string conversion
 * involved.
 */
#include "cztest.h"

int sum_to(int n)
{
    int total = 0;
    for (int i = 1; i < n; ++i) {
        total += i;
    }
    return total;
}

int digit_count(int n)
{
    int count = 0;
    while (n > 0) {
        ++count;
        n /= 10;
    }
    return count;
}

int reverse_int(int n)
{
    return n;
}

int main(void)
{
    CZ_CHECK_INT(sum_to(10), 55);
    CZ_CHECK_INT(digit_count(0), 1);
    CZ_CHECK_INT(digit_count(1234), 4);
    CZ_CHECK_INT(reverse_int(12345), 54321);
    CZ_CHECK_INT(reverse_int(1200), 21);

    return cztest_summary();
}

// I AM NOT DONE
