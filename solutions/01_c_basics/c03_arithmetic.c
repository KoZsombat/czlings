/* c03_arithmetic -- integer division, precedence, compound assignment.
 *
 * Task: repair the expressions so each function returns the documented
 * value. Integer division truncates toward zero; use 2.0 when you want a
 * fractional average; parentheses decide precedence.
 *
 * Coming from TypeScript: in C `/` on two ints is integer division and
 * `%` follows the sign of the dividend, while JS numbers are all doubles.
 */
#include "cztest.h"

int div_trunc(int a, int b)
{
    return a / b;
}

int mod_trunc(int a, int b)
{
    return a % b;
}

double mean(int a, int b)
{
    return (a + b) / 2.0;
}

int weighted(int a, int b)
{
    return (a + b) * 2;
}

int compound(int x)
{
    x *= 2 + 3;
    return x;
}

int cast_trunc(double v)
{
    return (int)v;
}

int main(void)
{
    CZ_CHECK_INT(div_trunc(-7, 2), -3);
    CZ_CHECK_INT(mod_trunc(-7, 2), -1);
    CZ_CHECK_NEAR(mean(3, 4), 3.5, 1e-9);
    CZ_CHECK_INT(weighted(2, 3), 10);
    CZ_CHECK(compound(4) == 20 && cast_trunc(3.9) == 3);

    return cztest_summary();
}
