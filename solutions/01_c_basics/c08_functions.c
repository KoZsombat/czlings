/* c08_functions -- prototypes, pass-by-value, returning a struct.
 *
 * Task: declare and define the helpers. Notice that swap() changes only its
 * own copies, so it cannot swap the caller's variables. Fix the pairing
 * operation by returning a struct Pair with the fields exchanged.
 *
 * Note: all C arguments are passed by value, so a function only ever sees
 * copies; to send data back, return it or pass a pointer.
 */
#include "cztest.h"

struct Pair {
    int a;
    int b;
};

int triple(int v);
void bump(int v);
void swap(int a, int b);
struct Pair swap_pair(struct Pair p);

int triple(int v)
{
    return v * 3;
}

void bump(int v)
{
    ++v;
    (void)v;
}

void swap(int a, int b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

struct Pair swap_pair(struct Pair p)
{
    struct Pair swapped;
    swapped.a = p.b;
    swapped.b = p.a;
    return swapped;
}

int main(void)
{
    struct Pair p = {1, 2};
    struct Pair q;
    int x = 6;

    bump(x);
    CZ_CHECK_INT(x, 6);
    CZ_CHECK_INT(triple(4), 12);

    q = swap_pair(p);
    CZ_CHECK(q.a == 2 && q.b == 1);
    CZ_CHECK(p.a == 1 && p.b == 2);

    return cztest_summary();
}
