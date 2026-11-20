/* b08_strings -- text is an array of characters.
 *
 * In C a string is a char array whose last character is '\0' (the NUL
 * terminator). Functions that process text walk the array until they see it.
 *
 * Task: string_length() must count characters until the terminating '\0',
 * without counting it. "hello" has length 5.
 */
#include "cztest.h"

int string_length(const char *text)
{
    int length = 0;
    /* TODO: loop while text[length] is not '\0', incrementing length. */
    return length;
}

int main(void)
{
    CZ_CHECK_INT(string_length(""), 0);
    CZ_CHECK_INT(string_length("C"), 1);
    CZ_CHECK_INT(string_length("hello"), 5);
    return cztest_summary();
}

// I AM NOT DONE
