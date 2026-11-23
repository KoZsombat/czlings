/* c12_strings -- NUL terminator and manual string functions.
 *
 * Task: implement strlen, strcmp, and a bounded copy. The copy must write
 * the terminating '\0' and must report failure when the destination cannot
 * hold the bytes plus the terminator, leaving an empty string behind.
 *
 * Note: a C string is a run of chars ending at '\0'. Forgetting that byte,
 * or copying cap bytes instead of cap - 1, writes past the buffer.
 *
 * From TS: a JS string carries its length and throws when an index is out of
 * range. A C string carries neither, so you scan for '\0' and check capacity
 * yourself; overflowing a buffer is undefined behaviour, not an exception.
 */
#include "cztest.h"
#include <stddef.h>

size_t my_strlen(const char *s)
{
    size_t n = 0;
    while (s[n] != '\0') {
        ++n;
    }
    return n;
}

int my_strcmp(const char *a, const char *b)
{
    for (size_t i = 0; i < my_strlen(a); i++)
    {
        if (a[i] != b[i]) return a[i] - b[i];
    }
    
    return 0;
}

int my_strcpy(char *dst, size_t cap, const char *src)
{
    if (my_strlen(src) > cap) {
        dst[0] = '\0';
        return -1;
    }
    size_t i = 0;
    while (i < cap && src[i] != '\0') {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
    return 0;
}

int main(void)
{
    char buf[8] = "XXXXXXX";

    CZ_CHECK(my_strlen("") == 0 && my_strlen("hello") == 5);
    CZ_CHECK(my_strcmp("abc", "abc") == 0 &&
             my_strcmp("abc", "abd") < 0 &&
             my_strcmp("b", "a") > 0);

    CZ_CHECK_INT(my_strcpy(buf, 3, "hi"), 0);
    CZ_CHECK_STR(buf, "hi");

    buf[0] = 'X';
    CZ_CHECK(my_strcpy(buf, 3, "hello") == -1 && buf[0] == '\0');

    return cztest_summary();
}
