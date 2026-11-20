/* c23_file_text: text files are streams of bytes; stdio adds buffering.
 *
 * Make write_lines() put one line per string, count_lines() count lines with
 * fgets(), and count_words() count whitespace-separated words.  Both counters
 * must rewind the stream before reading and leave it rewound afterwards.
 *
 * Coming from TypeScript: file access is byte-oriented and returns counts or
 * error codes, not promises.  fgets() keeps the trailing newline, EOF is an
 * int value, and a read can fail partway through.
 */
#include "cztest.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int write_lines(FILE *f, const char *const *lines, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        if (fprintf(f, "%s", lines[i]) < 0) {
            return -1;
        }
    }
    return 0;
}

int count_lines(FILE *f)
{
    char buf[128];
    int count = 0;
    while (fgets(buf, sizeof buf, f) != NULL) {
        ++count;
    }
    rewind(f);
    return count;
}

int count_words(FILE *f)
{
    int c;
    int in_word = 0;
    int count = 0;
    rewind(f);
    while ((c = fgetc(f)) != EOF) {
        if (isspace((unsigned char)c)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            ++count;
        }
    }
    rewind(f);
    return count;
}

int main(void)
{
    FILE *f = tmpfile();
    CZ_CHECK(f != NULL);
    const char *lines[] = {"alpha beta", "gamma"};

    CZ_CHECK_INT(write_lines(f, lines, 2), 0);
    CZ_CHECK_INT((long)ftell(f), (long)strlen("alpha beta\ngamma\n"));
    CZ_CHECK_INT(count_lines(f), 2);
    CZ_CHECK_INT(count_words(f), 3);

    fclose(f);
    return cztest_summary();
}
// I AM NOT DONE
