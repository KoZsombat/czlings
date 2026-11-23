/* c28_scope_storage: storage duration decides how long a variable lives.
 *
 * Make bump() keep its call count between calls and advance the file-scope
 * counter, make file_total() report that counter, and make shadow() return the
 * outer y.  fresh_counter() shows that an automatic local starts over each
 * call.
 *
 * Note: a block-scope local is recreated on every call; a `static` local is
 * one object with program lifetime, initialised once; an inner declaration
 * shadows the outer one until its block ends.
 *
 * From TS: module-level variables live as long as the module, and closures
 * capture locals. A C `static` local persists between calls but belongs to the
 * function; there is no closure object to capture other locals.
 */
#include "cztest.h"

static int file_counter = 0;

int bump(void)
{
    int calls = 0;
    ++calls;
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
        return y;
    }
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

// I AM NOT DONE
