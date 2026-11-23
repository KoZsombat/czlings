/* b01_hello -- your first C program.
 *
 * A C program starts at main(). printf prints text to the terminal, and "\n"
 * at the end of the text moves to a new line.
 *
 * Task: this file has two syntax errors. Every statement in C ends with a
 * semicolon. Fix both lines, then read the compiler error if you get stuck.
 *
 * New here? Read docs/00-start-here.md first.
 *
 * From TS: a TS file is run by Node; a C file is compiled into a native
 * program, and main() is where the operating system starts it. printf is not
 * console.log: it takes a format string and does not add a newline for you.
 */
#include <stdio.h>

int main(void)
{
    printf("Hello, world!\n");
    return 0;
}
