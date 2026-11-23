/* c02_types -- sizes, limits, and unsigned arithmetic.
 *
 * Task: fill in the six functions with facts about the platform's types.
 * Use sizeof for sizes, the standard macros for limits, and remember that
 * unsigned arithmetic wraps modulo 2^N.
 *
 * Note: the widths of int and long are not guaranteed; sizeof reports the
 * width this compiler chose for the target platform.
 *
 * From TS: JS has one `number` (a double). C has separate integer types whose
 * width and overflow rules differ; `sizeof` is the compile-time byte count.
 */
#include "cztest.h"
#include <limits.h>
#include <float.h>
#include <stddef.h>

int char_bytes(void)
{
    return sizeof(char);
}

int int_bytes(void)
{
    return sizeof(int);
}

int long_bytes(void)
{
    return sizeof(long);
}

long long int_max(void)
{
    return __INT_MAX__;
}

unsigned int wrap_minus_one(void)
{
    return (__INT_MAX__  *2U +1U);
}

int float_digits(void)
{
    return __FLT_DIG__;
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
