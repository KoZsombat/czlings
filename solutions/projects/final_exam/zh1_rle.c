/* zh1_rle -- final ZH, task 1 (10 points). BME Prog1 style string task.
 *
 * Run-length encoding: every run emits the character followed by its length
 * in decimal. The encoded form must fit in `cap` bytes including the NUL.
 */
#include "cztest.h"

#include <stddef.h>

int encode_rle(char *out, size_t cap, const char *in)
{
    if (out == NULL || cap == 0) {
        return -1;
    }
    out[0] = '\0';
    if (in == NULL || *in == '\0') {
        return 0;
    }

    size_t used = 0;
    const char *p = in;
    while (*p != '\0') {
        const char character = *p;
        const char *run_start = p;
        while (*p == character) {
            ++p;
        }
        size_t run_length = (size_t)(p - run_start);

        char digits[32];
        size_t digit_count = 0;
        do {
            digits[digit_count++] = (char)('0' + run_length % 10);
            run_length /= 10;
        } while (run_length != 0);

        /* One byte for the character, digit_count bytes, one for the NUL. */
        if (used + 1 + digit_count + 1 > cap) {
            out[0] = '\0';
            return -1;
        }

        out[used++] = character;
        while (digit_count > 0) {
            out[used++] = digits[--digit_count];
        }
    }

    out[used] = '\0';
    return (int)used;
}

int main(void)
{
    char out[64] = {0};

    CZ_CHECK_INT(encode_rle(out, sizeof out, ""), 0);
    CZ_CHECK_STR(out, "");

    CZ_CHECK_INT(encode_rle(out, sizeof out, NULL), 0);
    CZ_CHECK_STR(out, "");

    CZ_CHECK_INT(encode_rle(out, sizeof out, "abc"), 6);
    CZ_CHECK_STR(out, "a1b1c1");

    CZ_CHECK_INT(encode_rle(out, sizeof out, "aaabbc"), 6);
    CZ_CHECK_STR(out, "a3b2c1");

    CZ_CHECK_INT(encode_rle(out, sizeof out, "aaaaaaaaaaaa"), 3);
    CZ_CHECK_STR(out, "a12");

    CZ_CHECK_INT(encode_rle(out, sizeof out, "wwwwwwwwwwwwwwwwwwww"), 3);
    CZ_CHECK_STR(out, "w20");

    CZ_CHECK_INT(encode_rle(out, 7, "abc"), 6);
    CZ_CHECK_STR(out, "a1b1c1");

    out[0] = 'X';
    CZ_CHECK_INT(encode_rle(out, 6, "abc"), -1);
    CZ_CHECK_STR(out, "");

    CZ_CHECK_INT(encode_rle(out, 0, "a"), -1);
    CZ_CHECK_INT(encode_rle(NULL, 10, "a"), -1);

    return cztest_summary();
}
