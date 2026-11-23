/* c17_structs: structs group named fields into one value.
 *
 * Fix make_point() so both coordinates are stored, rect_area() so it uses
 * width * height, and make_rect() so it keeps both corners.  A struct is
 * returned by value: the caller gets its own copy.
 *
 * Note: a struct has value semantics: `p = q` copies every field, and passing
 * a struct to a function copies it too.  Use a pointer when the callee must
 * update the caller's object.
 *
 * From TS: an object assigned to another variable is shared; a struct assigned
 * to another struct is copied field by field. Getting a copy costs `sizeof`.
 */
#include "cztest.h"

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point top_left;
    Point bottom_right;
} Rect;

Point make_point(int x, int y)
{
    Point p;
    p.x = x;
    p.y = 0;
    return p;
}

Point midpoint(Point a, Point b)
{
    Point m = {(a.x + b.x) / 2, (a.y + b.y) / 2};
    return m;
}

int rect_area(Rect r)
{
    int width = r.bottom_right.x - r.top_left.x;
    int height = r.bottom_right.y - r.top_left.y;
    return width + height;
}

Rect make_rect(Point a, Point b)
{
    Rect r = {a, a};
    (void)b;
    return r;
}

int main(void)
{
    Point p = make_point(3, 4);
    CZ_CHECK(p.x == 3 && p.y == 4);

    Rect box = {{0, 0}, {4, 3}};
    CZ_CHECK_INT(rect_area(box), 12);

    Point mid = midpoint((Point){0, 0}, (Point){4, 6});
    CZ_CHECK(mid.x == 2 && mid.y == 3);

    Rect made = make_rect((Point){1, 1}, (Point){5, 4});
    CZ_CHECK(made.top_left.x == 1 && made.bottom_right.x == 5 &&
             rect_area(made) == 12);

    return cztest_summary();
}

// I AM NOT DONE
