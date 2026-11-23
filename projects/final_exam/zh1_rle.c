/* zh1_rle -- final ZH, task 1 (10 points). BME Prog1 style string task.
 *
 * Write encode_rle(out, cap, in), which run-length encodes a C string:
 *
 *   "aaabbc"  -> "a3b2c1"
 *   "abc"     -> "a1b1c1"
 *   "a" * 12  -> "a12"
 *
 * Rules:
 *   - Every run emits the character followed by its length in decimal.
 *   - `out` is NUL-terminated and must fit in at most `cap` bytes including
 *     the terminator.
 *   - Return the number of characters written, excluding the terminator.
 *   - Return -1 when `in` is non-empty and the encoded form does not fit, or
 *     when `out` is NULL / `cap` is 0. On failure, write an empty string when
 *     possible (out != NULL && cap > 0).
 *   - NULL or empty `in` produces an empty string and returns 0.
 *   - `in` and `out` must not overlap.
 *
 * Implement it with pointer arithmetic. Do not allocate memory and do not use
 * snprintf or strlen; a small local buffer for the reversed digits is enough.
 *
 * From TS: `"aaabbc".replace(/.../)` builds a new string for you. Here you
 * own the destination buffer and must check capacity before every write.
 */
#include "cztest.h"

#include <stddef.h>

int encode_rle(char *out, size_t cap, const char *in)
{
    /* TODO: walk one run at a time, write the character, then the run length
     * digits in reverse from a small local buffer. Check capacity before each
     * write and return -1 if the encoded string plus NUL does not fit. */
    (void)out;
    (void)cap;
    (void)in;
    return -1;
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

    /* Exactly enough room for "a1b1c1" plus the terminator. */
    CZ_CHECK_INT(encode_rle(out, 7, "abc"), 6);
    CZ_CHECK_STR(out, "a1b1c1");

    out[0] = 'X';
    CZ_CHECK_INT(encode_rle(out, 6, "abc"), -1);
    CZ_CHECK_STR(out, "");

    CZ_CHECK_INT(encode_rle(out, 0, "a"), -1);
    CZ_CHECK_INT(encode_rle(NULL, 10, "a"), -1);

    return cztest_summary();
}

// I AM NOT DONE
