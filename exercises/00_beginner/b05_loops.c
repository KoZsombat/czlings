/* b05_loops -- repeating work.
 *
 * `for (int i = 1; i <= n; ++i)` counts i from 1 through n inclusive. The
 * middle part is the condition: the loop body runs while it is true.
 *
 * Task: sum_to(n) must add 1 + 2 + ... + n. The loop currently stops before n.
 *
 * From TS: JS gives you `for..of` and array helpers; C gives you the counter
 * loop, and the loop variable's type is declared in the loop header.
 */
#include "cztest.h"

int sum_to(int n)
{
    int total = 0;
    for (int i = 1; i <= n; ++i) {
        total += i;
    }
    return total;
}

int main(void)
{
    CZ_CHECK_INT(sum_to(1), 1);
    CZ_CHECK_INT(sum_to(5), 15);
    CZ_CHECK_INT(sum_to(100), 5050);
    return cztest_summary();
}
