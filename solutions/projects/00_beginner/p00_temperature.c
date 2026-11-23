/* p00_temperature -- beginner project 1. Reference solution. */
#include "cztest.h"

#include <stddef.h>

double celsius_to_fahrenheit(int celsius)
{
    return celsius * 9.0 / 5.0 + 32.0;
}

const char *classify_temperature(int celsius)
{
    if (celsius <= 0) {
        return "freezing";
    }
    if (celsius <= 15) {
        return "cold";
    }
    if (celsius <= 25) {
        return "mild";
    }
    return "hot";
}

int max_temperature(const int *values, size_t count)
{
    if (count == 0) {
        return 0;
    }
    int maximum = values[0];
    for (size_t i = 1; i < count; ++i) {
        if (values[i] > maximum) {
            maximum = values[i];
        }
    }
    return maximum;
}

int count_at_least(const int *values, size_t count, int threshold)
{
    int matches = 0;
    for (size_t i = 0; i < count; ++i) {
        if (values[i] >= threshold) {
            ++matches;
        }
    }
    return matches;
}

int main(void)
{
    CZ_CHECK_NEAR(celsius_to_fahrenheit(0), 32.0, 1e-9);
    CZ_CHECK_NEAR(celsius_to_fahrenheit(100), 212.0, 1e-9);
    CZ_CHECK_NEAR(celsius_to_fahrenheit(-40), -40.0, 1e-9);

    CZ_CHECK_STR(classify_temperature(-5), "freezing");
    CZ_CHECK_STR(classify_temperature(0), "freezing");
    CZ_CHECK_STR(classify_temperature(1), "cold");
    CZ_CHECK_STR(classify_temperature(15), "cold");
    CZ_CHECK_STR(classify_temperature(16), "mild");
    CZ_CHECK_STR(classify_temperature(25), "mild");
    CZ_CHECK_STR(classify_temperature(26), "hot");

    const int readings[] = {-3, 12, 31, 8, 25};
    const size_t reading_count = sizeof readings / sizeof readings[0];

    CZ_CHECK_INT(max_temperature(readings, reading_count), 31);
    CZ_CHECK_INT(max_temperature(readings, 0), 0);
    CZ_CHECK_INT(count_at_least(readings, reading_count, 10), 3);
    CZ_CHECK_INT(count_at_least(readings, reading_count, 100), 0);

    return cztest_summary();
}
