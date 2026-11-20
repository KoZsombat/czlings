/* c02_types -- sizes, limits, and unsigned arithmetic.
 *
 * Task: fill in the six functions with facts about the platform's types.
 * Use sizeof for sizes, the standard macros for limits, and remember that
 * unsigned arithmetic wraps modulo 2^N.
 *
 * Coming from TypeScript: there is one number type. In C the widths of int
 * and long are NOT guaranteed; sizeof tells you what this compiler chose.
 */
#include "cztest.h"
#include <limits.h>
#include <float.h>
#include <stddef.h>

int char_bytes(void)
{
    return 0;
}

int int_bytes(void)
{
    return 0;
}

int long_bytes(void)
{
    return 0;
}

long long int_max(void)
{
    return 0;
}

unsigned int wrap_minus_one(void)
{
    return 0;
}

int float_digits(void)
{
    return 0;
}

int main(void)
{
    CZ_CHECK_INT(char_bytes(), 1);
    CZ_CHECK(int_bytes() == (int)sizeof(int) && long_bytes() == (int)sizeof(long));
    CZ_CHECK_INT(int_max(), INT_MAX);
    CZ_CHECK(wrap_minus_one() == UINT_MAX);
    CZ_CHECK(float_digits() >= 6 && float_digits() == FLT_DIG);

    return cztest_summary();
}

// I AM NOT DONE
