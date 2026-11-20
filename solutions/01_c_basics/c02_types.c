/* c02_types -- sizes, limits, and unsigned arithmetic.
 *
 * Task: fill in the six functions with facts about the platform's types.
 * Use sizeof for sizes, the standard macros for limits, and remember that
 * unsigned arithmetic wraps modulo 2^N.
 *
 * Note: the widths of int and long are not guaranteed; sizeof reports the
 * width this compiler chose for the target platform.
 */
#include "cztest.h"
#include <limits.h>
#include <float.h>
#include <stddef.h>

int char_bytes(void)
{
    return (int)sizeof(char);
}

int int_bytes(void)
{
    return (int)sizeof(int);
}

int long_bytes(void)
{
    return (int)sizeof(long);
}

long long int_max(void)
{
    return (long long)INT_MAX;
}

unsigned int wrap_minus_one(void)
{
    return 0u - 1u;
}

int float_digits(void)
{
    return FLT_DIG;
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
