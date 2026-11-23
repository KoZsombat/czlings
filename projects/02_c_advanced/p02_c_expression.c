/* p02_c_expression -- C advanced project: a recursive-descent expression parser.
 *
 * Prog1 lecture 14 ends with a larger program; its core is parsing. Evaluate
 * an integer arithmetic expression:
 *
 *   expression := term (('+' | '-') term)*
 *   term       := factor (('*' | '/') factor)*
 *   factor     := ['+' | '-'] factor | number | '(' expression ')'
 *   number     := one or more decimal digits
 *
 *   int evaluate_expression(const char *text, long *result);
 *
 * Returns 0 on success, -1 on a syntax error, and -2 on division by zero.
 * Whitespace is ignored. Integer division truncates toward zero.
 *
 * The parser is a small recursive descent: each grammar rule is one function,
 * whitespace is skipped at the start of every rule, and a leading sign is
 * handled by recursing into factor.
 *
 * From TS: `eval` or a parser library hides the grammar. Here each rule is a
 * function that consumes a prefix and reports the first error.
 */
#include "cztest.h"

int evaluate_expression(const char *text, long *result)
{
    /* TODO: implement expression(), term() and factor() as static recursive
     * helpers. Skip whitespace, recurse for unary +/-, and require the whole
     * input to be consumed. */
    (void)text;
    if (result != NULL) {
        *result = 0;
    }
    return -1;
}

int main(void)
{
    long result = 0;

    CZ_CHECK_INT(evaluate_expression("1+2*3", &result), 0);
    CZ_CHECK_INT(result, 7);

    CZ_CHECK_INT(evaluate_expression("(1+2)*3", &result), 0);
    CZ_CHECK_INT(result, 9);

    CZ_CHECK_INT(evaluate_expression(" 10 / 3 ", &result), 0);
    CZ_CHECK_INT(result, 3);

    CZ_CHECK_INT(evaluate_expression("-5 + 2", &result), 0);
    CZ_CHECK_INT(result, -3);

    CZ_CHECK_INT(evaluate_expression("2*(3+4)", &result), 0);
    CZ_CHECK_INT(result, 14);

    CZ_CHECK_INT(evaluate_expression("((7))", &result), 0);
    CZ_CHECK_INT(result, 7);

    CZ_CHECK_INT(evaluate_expression("+5", &result), 0);
    CZ_CHECK_INT(result, 5);

    CZ_CHECK_INT(evaluate_expression("2*-3", &result), 0);
    CZ_CHECK_INT(result, -6);

    CZ_CHECK_INT(evaluate_expression("-(3+4)*2", &result), 0);
    CZ_CHECK_INT(result, -14);

    CZ_CHECK_INT(evaluate_expression("1+", &result), -1);
    CZ_CHECK_INT(evaluate_expression("", &result), -1);
    CZ_CHECK_INT(evaluate_expression("abc", &result), -1);
    CZ_CHECK_INT(evaluate_expression("1 2", &result), -1);
    CZ_CHECK_INT(evaluate_expression("(1+2", &result), -1);

    CZ_CHECK_INT(evaluate_expression("1/0", &result), -2);
    CZ_CHECK_INT(evaluate_expression("2/(1-1)", &result), -2);

    return cztest_summary();
}

// I AM NOT DONE
