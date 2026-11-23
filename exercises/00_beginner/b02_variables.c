/* b02_variables -- storing a value in a named box.
 *
 * `int value = 41;` declares an integer variable and initializes it. Reading
 * the variable later returns whatever was last stored in it.
 *
 * Task: make answer() return 42 by changing the value it stores.
 *
 * From TS: `int` is a fixed 32-bit integer, while every JS number is a 64-bit
 * float. The declared type never changes, and an uninitialized local holds
 * garbage instead of `undefined`.
 */
#include "cztest.h"

int answer(void)
{
    int value = 42;
    return value;
}

int main(void)
{
    CZ_CHECK_INT(answer(), 42);
    return cztest_summary();
}
