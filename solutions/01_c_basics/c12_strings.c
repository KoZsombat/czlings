/* c12_strings -- NUL terminator and manual string functions.
 *
 * Task: implement strlen, strcmp, and a bounded copy. The copy must write
 * the terminating '\0' and must report failure when the destination cannot
 * hold the bytes plus the terminator, leaving an empty string behind.
 *
 * Coming from TypeScript: a C string is chars up to '\0'. Forgetting that
 * byte, or copying cap bytes instead of cap - 1, writes past the buffer.
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
    size_t i = 0;
    while (a[i] != '\0' && a[i] == b[i]) {
        ++i;
    }
    return (unsigned char)a[i] - (unsigned char)b[i];
}

int my_strcpy(char *dst, size_t cap, const char *src)
{
    size_t len = my_strlen(src);
    if (len + 1 > cap) {
        if (cap > 0) {
            dst[0] = '\0';
        }
        return -1;
    }
    for (size_t i = 0; i < len; ++i) {
        dst[i] = src[i];
    }
    dst[len] = '\0';
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
