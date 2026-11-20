/* c16_string_lib: C strings are NUL-terminated arrays with a fixed capacity.
 *
 * Fix the four helpers: compare() must order strings like strcmp(), copy_n()
 * must copy at most cap-1 bytes and always terminate, append() must report
 * failure instead of overflowing, and format_id() must build "id:<number>"
 * with snprintf, returning the length the full result would need.
 *
 * Coming from TypeScript: strings are not immutable values with automatic
 * growth.  strlen() scans for '\0', strncpy() does not promise a terminator,
 * and snprintf() returns the would-be length, which may exceed the buffer.
 */
#include "cztest.h"
#include <stdio.h>
#include <string.h>

int compare(const char *a, const char *b)
{
    return (int)(strlen(a) - strlen(b));
}

void copy_n(char *dst, size_t cap, const char *src)
{
    strncpy(dst, src, cap);
}

int append(char *dst, size_t cap, const char *src)
{
    size_t used = strlen(dst);
    size_t room = used < cap ? cap - used - 1 : 0;
    strncat(dst, src, room);
    return 0;
}

int format_id(char *dst, size_t cap, int id)
{
    return snprintf(dst, cap, "%d", id);
}

int main(void)
{
    CZ_CHECK(compare("abc", "abd") < 0 && compare("abc", "abc") == 0 &&
             compare("b", "a") > 0);

    char small[4];
    copy_n(small, sizeof small, "abcdef");
    CZ_CHECK(strcmp(small, "abc") == 0 && small[3] == '\0');

    char buf[8] = "ab";
    CZ_CHECK(append(buf, sizeof buf, "cde") == 0 && strcmp(buf, "abcde") == 0);
    CZ_CHECK(append(buf, sizeof buf, "0123456789") == -1 &&
             strcmp(buf, "abcde") == 0);

    char label[6];
    CZ_CHECK(format_id(label, sizeof label, 12345) == 8 &&
             strcmp(label, "id:12") == 0);

    return cztest_summary();
}

// I AM NOT DONE
