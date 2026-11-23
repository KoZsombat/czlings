/* c01_hello -- printf/snprintf formatting.
 *
 * Task: implement format_greeting so it writes "<name> (<age>)" into out and
 * returns the number of characters snprintf would have written, even when the
 * buffer is too small to hold the whole string.
 *
 * Note: a C string is a fixed char buffer whose capacity the caller supplies.
 * snprintf never writes past that capacity, and its return value is the
 * length the full, untruncated output would have had.
 *
 * From TS: a template string allocates the result for you; snprintf formats
 * into a buffer the caller allocated, so the capacity is a separate argument
 * and the return value is not the number of bytes actually written.
 */
#include "cztest.h"
#include <stddef.h>

int format_greeting(char *out, size_t cap, const char *name, int age)
{
    return snprintf(out, cap, "%s (%d)", name, age);
}

int main(void)
{
    char buf[64];
    char small[6];
    char tiny[1] = {0};

    CZ_CHECK_INT(format_greeting(buf, sizeof buf, "Ada", 36), 8);
    CZ_CHECK_STR(buf, "Ada (36)");

    CZ_CHECK_INT(format_greeting(small, sizeof small, "Ada", 36), 8);
    CZ_CHECK_STR(small, "Ada (");

    CZ_CHECK_INT(format_greeting(tiny, 0, "Ada", 36), 8);

    return cztest_summary();
}
