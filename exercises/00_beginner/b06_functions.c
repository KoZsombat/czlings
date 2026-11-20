/* b06_functions -- writing and calling your own function.
 *
 * A function has a return type, a name, parameters and a body:
 *
 *     int square(int x) { return x * x; }
 *
 * Once defined, it can be called anywhere below its definition.
 *
 * Task: square() must return x multiplied by itself.
 */
#include "cztest.h"

int square(int x)
{
    return x;
}

int main(void)
{
    CZ_CHECK_INT(square(0), 0);
    CZ_CHECK_INT(square(3), 9);
    CZ_CHECK_INT(square(-4), 16);
    return cztest_summary();
}

// I AM NOT DONE
