/* p02_c_expression -- C advanced project. Reference solution. */
#include "cztest.h"

typedef struct Parser {
    const char *cursor;
} Parser;

static int is_space(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

static void skip_spaces(Parser *parser)
{
    while (is_space(*parser->cursor)) {
        ++parser->cursor;
    }
}

static int parse_expression(Parser *parser, long *out);

static int parse_factor(Parser *parser, long *out)
{
    skip_spaces(parser);

    if (*parser->cursor == '+') {
        ++parser->cursor;
        return parse_factor(parser, out);
    }
    if (*parser->cursor == '-') {
        ++parser->cursor;
        long value = 0;
        const int status = parse_factor(parser, &value);
        if (status != 0) {
            return status;
        }
        *out = -value;
        return 0;
    }
    if (*parser->cursor == '(') {
        ++parser->cursor;
        long value = 0;
        int status = parse_expression(parser, &value);
        if (status != 0) {
            return status;
        }
        skip_spaces(parser);
        if (*parser->cursor != ')') {
            return -1;
        }
        ++parser->cursor;
        *out = value;
        return 0;
    }
    if (*parser->cursor < '0' || *parser->cursor > '9') {
        return -1;
    }

    long value = 0;
    while (*parser->cursor >= '0' && *parser->cursor <= '9') {
        value = value * 10 + (*parser->cursor - '0');
        ++parser->cursor;
    }
    *out = value;
    return 0;
}

static int parse_term(Parser *parser, long *out)
{
    long value = 0;
    int status = parse_factor(parser, &value);
    if (status != 0) {
        return status;
    }

    for (;;) {
        skip_spaces(parser);
        const char operation = *parser->cursor;
        if (operation != '*' && operation != '/') {
            break;
        }
        ++parser->cursor;

        long right = 0;
        status = parse_factor(parser, &right);
        if (status != 0) {
            return status;
        }
        if (operation == '*') {
            value *= right;
        } else {
            if (right == 0) {
                return -2;
            }
            value /= right;
        }
    }
    *out = value;
    return 0;
}

static int parse_expression(Parser *parser, long *out)
{
    long value = 0;
    int status = parse_term(parser, &value);
    if (status != 0) {
        return status;
    }

    for (;;) {
        skip_spaces(parser);
        const char operation = *parser->cursor;
        if (operation != '+' && operation != '-') {
            break;
        }
        ++parser->cursor;

        long right = 0;
        status = parse_term(parser, &right);
        if (status != 0) {
            return status;
        }
        if (operation == '+') {
            value += right;
        } else {
            value -= right;
        }
    }
    *out = value;
    return 0;
}

int evaluate_expression(const char *text, long *result)
{
    if (text == NULL || result == NULL) {
        return -1;
    }
    *result = 0;

    Parser parser = {text};
    skip_spaces(&parser);
    const int status = parse_expression(&parser, result);
    if (status != 0) {
        return status;
    }
    skip_spaces(&parser);
    if (*parser.cursor != '\0') {
        return -1;
    }
    return 0;
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
