/* b02_variables -- storing a value in a named box.
 *
 * `int value = 41;` declares an integer variable and initializes it. Reading
 * the variable later returns whatever was last stored in it.
 *
 * Task: make answer() return 42 by changing the value it stores.
 */
#include "cztest.h"

int answer(void)
{
    int value = 41;
    return value;
}

int main(void)
{
    CZ_CHECK_INT(answer(), 42);
    return cztest_summary();
}

// I AM NOT DONE
