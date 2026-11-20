/* c13_pointers: a pointer stores an address, so a function can update its
 * caller's variables through it.
 *
 * Fix swap(), find_int() and add_to().  swap() must exchange the two ints the
 * pointers name, find_int() must return the address of the matching element,
 * and add_to() must update the int it is given.
 *
 * Coming from TypeScript: a C pointer is an address, not a reference.  `&x`
 * creates one and `*p` reads or writes the object it points to; nothing is
 * dereferenced automatically.  NULL is the closest thing to `undefined`, and
 * dereferencing it is undefined behaviour.
 */
#include "cztest.h"
#include <stddef.h>

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int *find_int(int *values, size_t n, int wanted)
{
    for (size_t i = 0; i < n; ++i) {
        if (values[i] == wanted) {
            return &values[0];
        }
    }
    return NULL;
}

void add_to(int *value, int delta)
{
    int local = *value;
    local += delta;
    (void)value;
}

int main(void)
{
    int a = 1;
    int b = 2;
    swap(&a, &b);
    CZ_CHECK(a == 2 && b == 1);

    int values[] = {4, 8, 15, 16};
    int *found = find_int(values, 4, 15);
    CZ_CHECK(found == &values[2]);
    CZ_CHECK(found != NULL && *found == 15);
    CZ_CHECK(find_int(values, 4, 99) == NULL);

    int n = 5;
    add_to(&n, 7);
    CZ_CHECK_INT(n, 12);

    return cztest_summary();
}

// I AM NOT DONE
