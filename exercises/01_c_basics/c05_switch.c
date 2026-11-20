/* c05_switch -- switch/case/default and fallthrough.
 *
 * Task: implement a four-function calculator over a char operator. Set *err
 * to 1 for an unknown operator or division by zero; otherwise set it to 0.
 * Each case needs its own break, or execution falls into the next case.
 *
 * Coming from TypeScript: C switch is not scoped per case and does not stop
 * automatically; fallthrough is the default, and break is required.
 */
#include "cztest.h"

int apply(int a, char op, int b, int *err)
{
    int result = 0;
    *err = 0;

    switch (op) {
    case '+':
        result = a + b;
    case '-':
        result = a - b;
    case '*':
        result = a * b;
    case '/':
        if (b == 0) {
            *err = 1;
        } else {
            result = a / b;
        }
        break;
    default:
        *err = 1;
    }
    return result;
}

int main(void)
{
    int err = 0;

    err = 1;
    CZ_CHECK_INT(apply(7, '+', 5, &err), 12);
    CZ_CHECK_INT(apply(7, '-', 5, &err), 2);
    CZ_CHECK_INT(apply(6, '*', 7, &err), 42);
    CZ_CHECK(apply(7, '/', 2, &err) == 3 && err == 0);
    CZ_CHECK(apply(7, '/', 0, &err) == 0 && err == 1);

    return cztest_summary();
}

// I AM NOT DONE
