/* c09_recursion -- base cases and recursive calls.
 *
 * Task: implement factorial (n <= 1 is the base case), gcd via Euclid, and
 * integer power. Every recursive function needs a base case that stops the
 * descent, otherwise the call stack overflows.
 *
 * Coming from TypeScript: recursion is the same idea, but C has a small,
 * fixed stack; deep recursion crashes instead of throwing RangeError.
 */
#include "cztest.h"

long long factorial(int n)
{
    if (n <= 1) {
        return 1;
    }
    return (long long)n * factorial(n - 1);
}

int gcd(int a, int b)
{
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

long long ipow(int base, int exp)
{
    if (exp == 0) {
        return 1;
    }
    return (long long)base * ipow(base, exp - 1);
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
