/* b03_arithmetic -- operators and return values.
 *
 * `+ - * /` work like you expect on integers, with one C rule: division of two
 * ints truncates toward zero. `5 / 2` is 2, not 2.5.
 *
 * Reference solution.
 */
#include "cztest.h"

int add(int a, int b)
{
    return a + b;
}

int multiply(int a, int b)
{
    return a * b;
}

int main(void)
{
    CZ_CHECK_INT(add(2, 3), 5);
    CZ_CHECK_INT(add(-4, 4), 0);
    CZ_CHECK_INT(multiply(4, 5), 20);
    return cztest_summary();
}
