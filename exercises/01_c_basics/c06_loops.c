/* c06_loops -- for, while, do-while.
 *
 * Task: sum 1..n with a for loop, count decimal digits with a do-while (zero
 * has one digit), and reverse the digits of a nonnegative int with a while.
 *
 * Note: for a nonnegative int, `n % 10` extracts the last decimal digit and
 * `n / 10` drops it; both are integer operations, with no string conversion
 * involved.
 *
 * From TS: JS has no `do..while` and gives you `String(n).length`; here digit
 * extraction is arithmetic on the integer itself.
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
    int output = 0;
    while (n != 0){
        int digit = n % 10;
        output = output * 10 + digit;
        n /= 10;
    };
    return output;
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
