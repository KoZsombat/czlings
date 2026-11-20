/* c06_loops -- for, while, do-while.
 *
 * Task: sum 1..n with a for loop, count decimal digits with a do-while (zero
 * has one digit), and reverse the digits of a nonnegative int with a while.
 *
 * Coming from TypeScript: the loops look familiar, but integer division
 * `n / 10` drops the last digit and `n % 10` extracts it; there is no
 * Number.prototype.toString trick here.
 */
#include "cztest.h"

int sum_to(int n)
{
    int total = 0;
    for (int i = 1; i <= n; ++i) {
        total += i;
    }
    return total;
}

int digit_count(int n)
{
    int count = 0;
    do {
        ++count;
        n /= 10;
    } while (n > 0);
    return count;
}

int reverse_int(int n)
{
    int reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + n % 10;
        n /= 10;
    }
    return reversed;
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
