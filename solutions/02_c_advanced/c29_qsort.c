/* c29_qsort: qsort and bsearch take a comparator through a function pointer.
 *
 * Fix cmp_int_asc() so it returns a negative/zero/positive result for a total
 * order, and cmp_str() so it orders the pointed-to strings.  bsearch uses the
 * same comparator, so the array must already be sorted by it.
 *
 * Coming from TypeScript: you pass a function pointer, not a closure, and it
 * receives const void * arguments.  The comparator must describe a consistent
 * total order; returning a - b can overflow, so compare with > and < instead.
 */
#include "cztest.h"
#include <stdlib.h>
#include <string.h>

int cmp_int_asc(const void *a, const void *b)
{
    const int *left = (const int *)a;
    const int *right = (const int *)b;
    return (*left > *right) - (*left < *right);
}

int cmp_int_desc(const void *a, const void *b)
{
    const int *left = (const int *)a;
    const int *right = (const int *)b;
    return (*right > *left) - (*right < *left);
}

int cmp_str(const void *a, const void *b)
{
    const char *const *left = (const char *const *)a;
    const char *const *right = (const char *const *)b;
    return strcmp(*left, *right);
}

int main(void)
{
    int two = 2;
    int one = 1;
    CZ_CHECK(cmp_int_asc(&two, &one) > 0 && cmp_int_asc(&one, &one) == 0);

    int numbers[] = {40, 10, 30, 20};
    qsort(numbers, 4, sizeof numbers[0], cmp_int_asc);
    CZ_CHECK(numbers[0] == 10 && numbers[1] == 20 &&
             numbers[2] == 30 && numbers[3] == 40);

    int key = 30;
    int *hit = (int *)bsearch(&key, numbers, 4, sizeof numbers[0],
                              cmp_int_asc);
    CZ_CHECK(hit != NULL && *hit == 30);

    const char *words[] = {"pear", "apple", "fig"};
    qsort(words, 3, sizeof words[0], cmp_str);
    CZ_CHECK(strcmp(words[0], "apple") == 0 &&
             strcmp(words[1], "fig") == 0 &&
             strcmp(words[2], "pear") == 0);

    return cztest_summary();
}
