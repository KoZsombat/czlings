/* c25_preprocessor: macros are text substitution performed before compiling.
 *
 * Fix SQR and MIN by parenthesising every parameter and the whole result, give
 * CLAMP_ADD a do/while(0) body so it behaves as one statement, and select the
 * CZ_MODE_NAME branch for mode 2.
 *
 * Coming from TypeScript: a macro has no type checking and does not evaluate
 * its arguments once.  It is replaced literally, so missing parentheses change
 * precedence; a multi-statement macro breaks `if`/`else` unless wrapped in
 * do/while(0).  Prefer a static inline function when a function will do.
 */
#include "cztest.h"

#define SQR(x) x * x
#define MIN(a, b) a < b ? a : b
#define CLAMP_ADD(dst, amount, limit) (dst) += (amount); if ((dst) > (limit)) (dst) = (limit)

#define CZ_MODE 1
#if CZ_MODE == 1
#define CZ_MODE_NAME "fast"
#elif CZ_MODE == 2
#define CZ_MODE_NAME "safe"
#else
#define CZ_MODE_NAME "debug"
#endif

static int sqr_fn(int x)
{
    return x * x;
}

int main(void)
{
    CZ_CHECK_INT(SQR(1 + 2), 9);
    CZ_CHECK_INT(2 * MIN(3, 4), 6);

    int value = 2;
    if (value > 0)
        CLAMP_ADD(value, 5, 8);
    else
        value = -1;
    CZ_CHECK_INT(value, 7);

    CZ_CHECK_INT(sqr_fn(1 + 2), 9);
    CZ_CHECK_STR(CZ_MODE_NAME, "safe");

    return cztest_summary();
}

// I AM NOT DONE
