/* b07_arrays -- many values of the same type.
 *
 *     int data[5] = {2, 4, 6, 8, 10};
 *
 * Valid indexes are 0 through 4. C does not check the index for you, so a loop
 * that starts at 1 silently skips the first element.
 *
 * Task: sum_array() must add every element, indexes 0 through n - 1.
 *
 * From TS: `int data[5]` is a fixed block of five integers inside the current
 * scope, not a heap array object; it has no `.length` and no `push`.
 */
#include "cztest.h"

int sum_array(const int *values, int n)
{
    int total = 0;
    for (int i = 0; i < n; ++i) {
        total += values[i];
    }
    return total;
}

int main(void)
{
    int data[5] = {2, 4, 6, 8, 10};
    CZ_CHECK_INT(sum_array(data, 5), 30);
    CZ_CHECK_INT(sum_array(data, 2), 6);
    return cztest_summary();
}
