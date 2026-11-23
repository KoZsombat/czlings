/* b08_strings -- text is an array of characters.
 *
 * In C a string is a char array whose last character is '\0' (the NUL
 * terminator). Functions that process text walk the array until they see it.
 *
 * Task: string_length() must count characters until the terminating '\0',
 * without counting it. "hello" has length 5.
 *
 * From TS: a JS string is an immutable, length-tracked object; a C string is
 * just bytes in memory, so finding its length means scanning for '\0'.
 */
#include "cztest.h"

int string_length(const char *text)
{
    int length = 0;
    while (text[length] != '\0'){
        length++;
    };
    return length;
}

int main(void)
{
    CZ_CHECK_INT(string_length(""), 0);
    CZ_CHECK_INT(string_length("C"), 1);
    CZ_CHECK_INT(string_length("hello"), 5);
    return cztest_summary();
}
