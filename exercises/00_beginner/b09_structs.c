/* b09_structs -- grouping related values.
 *
 *     struct Point { int x; int y; };
 *
 * A struct type holds several named members. You read a member with a dot:
 * `p.x`. A struct is passed to a function by value, as a copy.
 *
 * Task: sum_coords() must return p.x + p.y.
 *
 * From TS: think of a struct as a plain object with a fixed shape, except that
 * assignment copies the fields instead of sharing one object.
 */
#include "cztest.h"

struct Point {
    int x;
    int y;
};

int sum_coords(struct Point p)
{
    return p.x + p.y;
}

int main(void)
{
    struct Point a = {2, 3};
    struct Point b = {-1, 4};
    CZ_CHECK_INT(sum_coords(a), 5);
    CZ_CHECK_INT(sum_coords(b), 3);
    return cztest_summary();
}
