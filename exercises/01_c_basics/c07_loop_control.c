/* c07_loop_control -- break, continue, nested loops.
 *
 * Task: test primality with early exit, sum the multiples of 3 or 5 below
 * limit by skipping the rest with continue, and sum the triangle table
 * (row r contains 1..r) using nested loops.
 *
 * Note: break exits only the innermost loop, and continue jumps to the next
 * iteration of the innermost loop.
 *
 * From TS: break and continue are the same, but C has no labeled statements;
 * to leave two nested loops at once you need a flag or a goto.
 */
#include "cztest.h"

int is_prime(int n)
{
    if (n < 2) return 0;
    for (int d = 2; d < n; ++d) {
        if (n % d == 0) {
            return 0;
        }
    }
    return 1;
}

int sum_multiples(int limit)
{
    int total = 0;
    for (int i = 1; i < limit; ++i) {
        if (i % 3 == 0 || i % 5 == 0)
        total += i;
    }
    return total;
}

int triangle_sum(int rows)
{
    int total = 0;
    for (int r = 1; r <= rows; ++r) {
        for (int c = 1; c <= r; ++c) {
            total += c;
        }
    }
    return total;
}

int main(void)
{
    CZ_CHECK(is_prime(2) && !is_prime(1));
    CZ_CHECK(!is_prime(9) && is_prime(97));
    CZ_CHECK_INT(sum_multiples(10), 23);
    CZ_CHECK_INT(triangle_sum(3), 10);
    CZ_CHECK_INT(triangle_sum(4), 20);

    return cztest_summary();
}
