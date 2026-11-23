/* p02_c_state_machine -- C advanced project. Reference solution. */
#include "cztest.h"

#include <stddef.h>
#include <string.h>

static int is_space(char ch)
{
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

static int is_ident_start(char ch)
{
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_';
}

static int is_ident_char(char ch)
{
    return is_ident_start(ch) || (ch >= '0' && ch <= '9');
}

static void skip_spaces(const char **cursor)
{
    while (is_space(**cursor)) {
        ++*cursor;
    }
}

int parse_config_line(const char *line, char *key, size_t key_cap,
                      char *value, size_t value_cap)
{
    if (line == NULL || key == NULL || value == NULL || key_cap == 0 ||
        value_cap == 0) {
        return -1;
    }
    key[0] = '\0';
    value[0] = '\0';

    const char *cursor = line;
    skip_spaces(&cursor);
    if (*cursor == '\0' || *cursor == '#') {
        return 1;
    }

    if (!is_ident_start(*cursor)) {
        return -1;
    }
    size_t key_length = 0;
    while (is_ident_char(*cursor)) {
        if (key_length + 1 >= key_cap) {
            return -2;
        }
        key[key_length++] = *cursor++;
    }
    key[key_length] = '\0';

    skip_spaces(&cursor);
    if (*cursor != '=') {
        return -1;
    }
    ++cursor;
    skip_spaces(&cursor);

    if (*cursor == '"') {
        ++cursor;
        size_t value_length = 0;
        int closed = 0;
        while (*cursor != '\0') {
            char ch = *cursor++;
            if (ch == '"') {
                closed = 1;
                break;
            }
            if (ch == '\\') {
                const char escape = *cursor;
                if (escape == '\0') {
                    return -1;
                }
                ++cursor;
                switch (escape) {
                case '"':
                    ch = '"';
                    break;
                case '\\':
                    ch = '\\';
                    break;
                case 'n':
                    ch = '\n';
                    break;
                case 't':
                    ch = '\t';
                    break;
                default:
                    return -1;
                }
            }
            if (value_length + 1 >= value_cap) {
                return -2;
            }
            value[value_length++] = ch;
        }
        if (!closed) {
            return -1;
        }
        value[value_length] = '\0';
        skip_spaces(&cursor);
        if (*cursor != '\0' && *cursor != '#') {
            return -1;
        }
        return 0;
    }

    const char *start = cursor;
    while (*cursor != '\0' && *cursor != '#') {
        ++cursor;
    }
    const char *end = cursor;
    while (end > start && is_space(end[-1])) {
        --end;
    }
    if (start == end) {
        return -1;
    }
    for (const char *scan = start; scan < end; ++scan) {
        if (*scan == '"') {
            return -1;
        }
    }
    const size_t value_length = (size_t)(end - start);
    if (value_length + 1 > value_cap) {
        return -2;
    }
    memcpy(value, start, value_length);
    value[value_length] = '\0';
    return 0;
}

int main(void)
{
    char key[32];
    char value[64];

    CZ_CHECK_INT(parse_config_line("name = Alice", key, sizeof key, value,
                                   sizeof value),
                 0);
    CZ_CHECK_STR(key, "name");
    CZ_CHECK_STR(value, "Alice");

    CZ_CHECK_INT(parse_config_line("  count=42  ", key, sizeof key, value,
                                   sizeof value),
                 0);
    CZ_CHECK_STR(key, "count");
    CZ_CHECK_STR(value, "42");

    CZ_CHECK_INT(parse_config_line("quote = \"a \\\"b\\\" c\"", key,
                                   sizeof key, value, sizeof value),
                 0);
    CZ_CHECK_STR(key, "quote");
    CZ_CHECK_STR(value, "a \"b\" c");

    CZ_CHECK_INT(parse_config_line("path=\"C:\\\\tmp\"", key, sizeof key, value,
                                   sizeof value),
                 0);
    CZ_CHECK_STR(key, "path");
    CZ_CHECK_STR(value, "C:\\tmp");

    CZ_CHECK_INT(parse_config_line("key=\"v\" # comment", key, sizeof key,
                                   value, sizeof value),
                 0);
    CZ_CHECK_STR(key, "key");
    CZ_CHECK_STR(value, "v");

    CZ_CHECK_INT(parse_config_line("key=value   # trailing", key, sizeof key,
                                   value, sizeof value),
                 0);
    CZ_CHECK_STR(value, "value");

    CZ_CHECK_INT(parse_config_line("empty=\"\"", key, sizeof key, value,
                                   sizeof value),
                 0);
    CZ_CHECK_STR(key, "empty");
    CZ_CHECK_STR(value, "");

    CZ_CHECK_INT(parse_config_line("# whole-line comment", key, sizeof key,
                                   value, sizeof value),
                 1);
    CZ_CHECK_STR(key, "");
    CZ_CHECK_STR(value, "");

    CZ_CHECK_INT(parse_config_line("   ", key, sizeof key, value,
                                   sizeof value),
                 1);
    CZ_CHECK_INT(parse_config_line("key=", key, sizeof key, value,
                                   sizeof value),
                 -1);
    CZ_CHECK_INT(parse_config_line("=value", key, sizeof key, value,
                                   sizeof value),
                 -1);
    CZ_CHECK_INT(parse_config_line("key value", key, sizeof key, value,
                                   sizeof value),
                 -1);
    CZ_CHECK_INT(parse_config_line("1key=value", key, sizeof key, value,
                                   sizeof value),
                 -1);
    CZ_CHECK_INT(parse_config_line("key=\"unterminated", key, sizeof key,
                                   value, sizeof value),
                 -1);
    CZ_CHECK_INT(parse_config_line("key=\"v\" trailing", key, sizeof key,
                                   value, sizeof value),
                 -1);
    CZ_CHECK_INT(parse_config_line("key=\"bad\\q\"", key, sizeof key, value,
                                   sizeof value),
                 -1);

    char tiny_key[4];
    char tiny_value[4];
    CZ_CHECK_INT(parse_config_line("longkey=1", tiny_key, sizeof tiny_key,
                                   value, sizeof value),
                 -2);
    CZ_CHECK_INT(parse_config_line("k=abcdef", key, sizeof key, tiny_value,
                                   sizeof tiny_value),
                 -2);

    return cztest_summary();
}
