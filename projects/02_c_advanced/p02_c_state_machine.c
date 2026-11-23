/* p02_c_state_machine -- C advanced project: a config-line parser.
 *
 * Prog1 lecture 12 is about state machines. This project parses one line of a
 * small config file:
 *
 *   name = Alice
 *   count=42
 *   quote = "a \"quoted\" value"
 *   path = "C:\\tmp"
 *   key=value   # comment
 *   # whole-line comment
 *
 * int parse_config_line(const char *line,
 *                       char *key, size_t key_cap,
 *                       char *value, size_t value_cap);
 *
 * Returns:
 *   0   valid key = value assignment
 *   1   blank line or comment-only line (key and value are empty)
 *  -1   syntax error
 *  -2   key or value does not fit in its output buffer
 *
 * Rules:
 *   - leading/trailing whitespace is ignored;
 *   - key is an identifier: [A-Za-z_][A-Za-z0-9_]*;
 *   - '=' is required and may be surrounded by whitespace;
 *   - an unquoted value runs to '#' or end of line and is trimmed;
 *   - a quoted value starts and ends with '"' and supports \\" \\\\ \\n \\t;
 *   - after a closing quote only whitespace and an optional '#' comment remain;
 *   - empty unquoted values are invalid; an empty quoted value "" is valid.
 *
 * From TS: a regex or a split would do this. Here you walk the bytes with an
 * explicit state (start, key, equals, unquoted, quoted, escape, comment) and
 * decide when to stop.
 */
#include "cztest.h"

#include <stddef.h>
#include <string.h>

int parse_config_line(const char *line, char *key, size_t key_cap,
                      char *value, size_t value_cap)
{
    /* TODO: walk the line with the states described above. Copy the key and
     * value as you go, respect both capacities, and handle the quoted escapes.
     * Return 0, 1, -1 or -2. */
    (void)line;
    if (key != NULL && key_cap > 0) {
        key[0] = '\0';
    }
    if (value != NULL && value_cap > 0) {
        value[0] = '\0';
    }
    return -1;
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

// I AM NOT DONE
