/* c27_argv: main receives the argument count and an array of C strings.
 *
 * Finish count_flag() so it counts exact matches among argv[1]..argv[argc-1];
 * argv[0] is the program name.  Keep the required main signature and build
 * fake argument arrays in main() to test it.
 *
 * Coming from TypeScript: process.argv is an ordinary array of strings, while
 * C gives you a pointer to pointers terminated only by argc (plus a NULL
 * sentinel by convention).  Compare strings with strcmp; `==` compares
 * addresses, not contents.
 */
#include "cztest.h"
#include <stddef.h>
#include <string.h>

int count_flag(int argc, char **argv, const char *flag)
{
    int count = 0;
    for (int i = 1; i < argc; ++i) {
        if (argv[i] != NULL && strcmp(argv[i], flag) == 0) {
            ++count;
        }
    }
    return count;
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    char *fake[] = {"prog", "-v", "input.c", "-v", NULL};
    CZ_CHECK_INT(count_flag(4, fake, "-v"), 2);

    char *none[] = {"prog", "input.c", NULL};
    CZ_CHECK_INT(count_flag(2, none, "-v"), 0);

    char *similar[] = {"prog", "-verbose", "-v", "--v", NULL};
    CZ_CHECK_INT(count_flag(4, similar, "-v"), 1);

    CZ_CHECK_INT(count_flag(0, NULL, "-v"), 0);

    return cztest_summary();
}
