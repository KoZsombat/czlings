/* c21_dynamic_string: return a heap copy sized exactly for its contents.
 *
 * dup_string() must return a new string the caller frees, or NULL when src is
 * NULL or allocation fails.  join() must concatenate into one exact-size
 * buffer.  The caller owns every returned pointer.
 *
 * Note: a heap copy needs strlen(src) + 1 bytes because the terminator is part
 * of the allocation, and the caller must free it.  Returning a pointer into
 * src would not be a copy.
 */
#include "cztest.h"
#include <stdlib.h>
#include <string.h>

char *dup_string(const char *src)
{
    if (src == NULL) {
        return NULL;
    }

    size_t n = strlen(src);
    char *out = malloc(n > 0 ? n : 1);
    if (out == NULL) {
        return NULL;
    }
    if (n == 0) {
        out[0] = '\0';
        return out;
    }

    memcpy(out, src, n);
    out[n - 1] = '\0';
    return out;
}

char *join(const char *a, const char *b)
{
    size_t la = strlen(a);
    size_t lb = strlen(b);
    char *out = malloc(la + lb + 1);
    if (out == NULL) {
        return NULL;
    }
    memcpy(out, a, la);
    memcpy(out + la, b, lb + 1);
    return out;
}

int main(void)
{
    char *copy = dup_string("hello");
    CZ_CHECK(copy != NULL && strcmp(copy, "hello") == 0);
    free(copy);

    CZ_CHECK(dup_string(NULL) == NULL);

    char *empty = dup_string("");
    CZ_CHECK(empty != NULL && empty[0] == '\0');
    free(empty);

    char *both = join("ab", "cde");
    CZ_CHECK(both != NULL && strcmp(both, "abcde") == 0);
    free(both);

    return cztest_summary();
}

// I AM NOT DONE
