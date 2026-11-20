/* c04_control -- if/else chains, comparisons, ternary.
 *
 * Task: grade a score (A >= 90, B >= 80, C >= 70, D >= 60, else F), compute
 * an absolute value, and clamp a value into [lo, hi] with lo <= hi.
 *
 * Note: a C condition is an int, not a dedicated boolean type: zero is false
 * and any nonzero value is true. `==` compares values.
 */
#include "cztest.h"

char grade_for(int score)
{
    if (score >= 90) {
        return 'A';
    } else if (score >= 80) {
        return 'B';
    } else if (score >= 70) {
        return 'C';
    } else if (score >= 60) {
        return 'D';
    }
    return 'F';
}

int abs_int(int v)
{
    return v < 0 ? -v : v;
}

int clamp(int v, int lo, int hi)
{
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

int main(void)
{
    CZ_CHECK_INT(grade_for(90), 'A');
    CZ_CHECK_INT(grade_for(59), 'F');
    CZ_CHECK_INT(abs_int(-7), 7);
    CZ_CHECK_INT(clamp(9, 0, 5), 5);
    CZ_CHECK_INT(clamp(-3, 0, 5), 0);

    return cztest_summary();
}
