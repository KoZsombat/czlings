/* c12_strings -- NUL terminator and manual string functions.
 *
 * Task: implement strlen, strcmp, and a bounded copy. The copy must write
 * the terminating '\0' and must report failure when the destination cannot
 * hold the bytes plus the terminator, leaving an empty string behind.
 *
 * Note: a C string is a run of chars ending at '\0'. Forgetting that byte,
 * or copying cap bytes instead of cap - 1, writes past the buffer.
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
    return a[0] - b[0];
}

int my_strcpy(char *dst, size_t cap, const char *src)
{
    size_t i = 0;
    while (i < cap && src[i] != '\0') {
        dst[i] = src[i];
        ++i;
    }
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

// I AM NOT DONE
