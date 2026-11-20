/* cztest.h -- the tiny test harness used by every czlings exercise.
 *
 * It works in both C17 and C++17.  Each check records a failure and keeps
 * going, so you see every broken assertion in one run; main() returns
 * cztest_summary(), which is 0 only when nothing failed.
 */
#ifndef CZTEST_H
#define CZTEST_H

#include <stdio.h>
#include <string.h>
#include <math.h>

#if defined(__GNUC__) || defined(__clang__)
#define CZTEST_UNUSED __attribute__((unused))
#else
#define CZTEST_UNUSED
#endif

static CZTEST_UNUSED int cztest_checks = 0;
static CZTEST_UNUSED int cztest_failures = 0;

#define CZ_CHECK(cond)                                                          \
    do {                                                                        \
        ++cztest_checks;                                                        \
        if (!(cond)) {                                                          \
            ++cztest_failures;                                                  \
            printf("  [FAIL] line %d: %s\n", __LINE__, #cond);                  \
        }                                                                       \
    } while (0)

#define CZ_CHECK_MSG(cond, ...)                                                 \
    do {                                                                        \
        ++cztest_checks;                                                        \
        if (!(cond)) {                                                          \
            ++cztest_failures;                                                  \
            printf("  [FAIL] line %d: %s -- ", __LINE__, #cond);                \
            printf(__VA_ARGS__);                                                \
            printf("\n");                                                       \
        }                                                                       \
    } while (0)

#define CZ_CHECK_INT(actual, expected)                                          \
    do {                                                                        \
        long cz_a = (long)(actual);                                             \
        long cz_e = (long)(expected);                                           \
        ++cztest_checks;                                                        \
        if (cz_a != cz_e) {                                                     \
            ++cztest_failures;                                                  \
            printf("  [FAIL] line %d: %s == %ld, expected %ld\n",               \
                   __LINE__, #actual, cz_a, cz_e);                              \
        }                                                                       \
    } while (0)

#define CZ_CHECK_STR(actual, expected)                                          \
    do {                                                                        \
        const char *cz_a = (actual);                                            \
        const char *cz_e = (expected);                                          \
        ++cztest_checks;                                                        \
        if (cz_a == NULL || cz_e == NULL || strcmp(cz_a, cz_e) != 0) {          \
            ++cztest_failures;                                                  \
            printf("  [FAIL] line %d: %s == \"%s\", expected \"%s\"\n",         \
                   __LINE__, #actual, cz_a ? cz_a : "(null)",                   \
                   cz_e ? cz_e : "(null)");                                     \
        }                                                                       \
    } while (0)

#define CZ_CHECK_NEAR(actual, expected, eps)                                    \
    do {                                                                        \
        double cz_a = (double)(actual);                                         \
        double cz_e = (double)(expected);                                       \
        ++cztest_checks;                                                        \
        if (fabs(cz_a - cz_e) > (double)(eps)) {                                \
            ++cztest_failures;                                                  \
            printf("  [FAIL] line %d: %s == %f, expected %f\n",                 \
                   __LINE__, #actual, cz_a, cz_e);                              \
        }                                                                       \
    } while (0)

#define CZ_CHECK_MEM(actual, expected, nbytes)                                  \
    do {                                                                        \
        ++cztest_checks;                                                        \
        if (memcmp((actual), (expected), (nbytes)) != 0) {                      \
            ++cztest_failures;                                                  \
            printf("  [FAIL] line %d: memory of %s differs from %s\n",          \
                   __LINE__, #actual, #expected);                               \
        }                                                                       \
    } while (0)

static CZTEST_UNUSED int cztest_summary(void)
{
    if (cztest_failures != 0) {
        printf("\n%d of %d checks failed\n", cztest_failures, cztest_checks);
        return 1;
    }
    printf("all %d checks passed\n", cztest_checks);
    return 0;
}

#endif /* CZTEST_H */
