/* b10_max3 -- putting the pieces together.
 *
 * max2() is already written. A ternary expression `a > b ? a : b` evaluates to
 * a when the condition is true, otherwise to b.
 *
 * Task: max3() must return the largest of a, b and c, using max2() twice.
 */
#include "cztest.h"

int max2(int a, int b)
{
    return a > b ? a : b;
}

int max3(int a, int b, int c)
{
    return max2(a, b);
}

int main(void)
{
    CZ_CHECK_INT(max3(1, 2, 3), 3);
    CZ_CHECK_INT(max3(9, 2, 4), 9);
    CZ_CHECK_INT(max3(-1, -5, -2), -1);
    return cztest_summary();
}

// I AM NOT DONE
