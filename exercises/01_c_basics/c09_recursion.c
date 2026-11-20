/* c09_recursion -- base cases and recursive calls.
 *
 * Task: implement factorial (n <= 1 is the base case), gcd via Euclid, and
 * integer power. Every recursive function needs a base case that stops the
 * descent, otherwise the call stack overflows.
 *
 * Note: C has a small, fixed call stack; runaway recursion crashes the
 * process rather than raising an exception.
 */
#include "cztest.h"

long long factorial(int n)
{
    if (n == 0) {
        return 0;
    }
    return n * factorial(n - 1);
}

int gcd(int a, int b)
{
    return a % b;
}

long long ipow(int base, int exp)
{
    return base * exp;
}

int main(void)
{
    CZ_CHECK_INT(factorial(0), 1);
    CZ_CHECK_INT(factorial(5), 120);
    CZ_CHECK_INT(gcd(48, 18), 6);
    CZ_CHECK_INT(gcd(0, 9), 9);
    CZ_CHECK_INT(ipow(2, 10), 1024);

    return cztest_summary();
}

// I AM NOT DONE
