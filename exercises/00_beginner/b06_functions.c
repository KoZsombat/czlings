/* b06_functions -- writing and calling your own function.
 *
 * A function has a return type, a name, parameters and a body:
 *
 *     int square(int x) { return x * x; }
 *
 * Once defined, it can be called anywhere below its definition.
 *
 * Task: square() must return x multiplied by itself.
 *
 * From TS: parameter and return types are part of the signature and are
 * checked when the program is compiled, not when it runs. There are no
 * optional parameters and no closures.
 */
#include "cztest.h"

int square(int x)
{
    return x * x;
}

int main(void)
{
    CZ_CHECK_INT(square(0), 0);
    CZ_CHECK_INT(square(3), 9);
    CZ_CHECK_INT(square(-4), 16);
    return cztest_summary();
}
