/* b04_if -- making a decision.
 *
 * `if (condition) { ... } else { ... }` runs one block or the other. A
 * comparison such as `x < 0` produces a true/false value.
 *
 * Task: absolute(x) must return x when x is zero or positive, and -x when x is
 * negative. The current code ignores the sign.
 */
#include "cztest.h"

int absolute(int x)
{
    return x;
}

int main(void)
{
    CZ_CHECK_INT(absolute(5), 5);
    CZ_CHECK_INT(absolute(0), 0);
    CZ_CHECK_INT(absolute(-7), 7);
    return cztest_summary();
}

// I AM NOT DONE
