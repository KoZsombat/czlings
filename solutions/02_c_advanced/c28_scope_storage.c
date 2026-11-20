/* c28_scope_storage: storage duration decides how long a variable lives.
 *
 * Make bump() keep its call count between calls and advance the file-scope
 * counter, make file_total() report that counter, and make shadow() return the
 * outer y.  fresh_counter() shows that an automatic local starts over each
 * call.
 *
 * Coming from TypeScript: module variables are not the same as C file scope,
 * and closures do not exist here.  A block-scope local is recreated on every
 * call; a `static` local is one object with program lifetime, initialised
 * once; an inner declaration shadows the outer one until its block ends.
 */
#include "cztest.h"

static int file_counter = 0;

int bump(void)
{
    static int calls = 0;
    ++calls;
    ++file_counter;
    return calls;
}

int file_total(void)
{
    return file_counter;
}

int fresh_counter(void)
{
    int n = 0;
    ++n;
    return n;
}

int shadow(int x)
{
    int y = x + 1;
    {
        int y = 100;
        (void)y;
    }
    return y;
}

int main(void)
{
    int b1 = bump();
    int b2 = bump();
    int b3 = bump();
    CZ_CHECK(b1 == 1 && b2 == 2 && b3 == 3);
    CZ_CHECK_INT(file_total(), 3);
    CZ_CHECK_INT(fresh_counter(), 1);
    CZ_CHECK_INT(shadow(5), 6);

    return cztest_summary();
}
