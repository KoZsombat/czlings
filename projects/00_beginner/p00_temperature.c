/* p00_temperature -- beginner project 1.
 *
 * After chapter 0 you know functions, if, loops, arrays and strings. This
 * project is a small temperature report:
 *
 *   celsius_to_fahrenheit(c)   convert one reading
 *   classify_temperature(c)    "freezing" / "cold" / "mild" / "hot"
 *   max_temperature(values,n)  largest reading, 0 for an empty array
 *   count_at_least(values,n,t) how many readings are >= t
 *
 * Boundaries for classify_temperature:
 *   <= 0  freezing
 *   <= 15 cold
 *   <= 25 mild
 *   > 25  hot
 *
 * From TS: `arr.map(...)` returns a new array and `Math.max(...arr)` hides the
 * loop. Here you write the loop yourself and decide what an empty array means.
 */
#include "cztest.h"

#include <stddef.h>

double celsius_to_fahrenheit(int celsius)
{
    /* TODO: F = C * 9 / 5 + 32. Keep the division in floating point. */
    (void)celsius;
    return 0.0;
}

const char *classify_temperature(int celsius)
{
    /* TODO: return the four strings above, using if / else if. */
    (void)celsius;
    return "unknown";
}

int max_temperature(const int *values, size_t count)
{
    /* TODO: walk the array and remember the largest value. Return 0 when
     * count is 0. */
    (void)values;
    (void)count;
    return 0;
}

int count_at_least(const int *values, size_t count, int threshold)
{
    /* TODO: count the readings that are greater than or equal to threshold. */
    (void)values;
    (void)count;
    (void)threshold;
    return 0;
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

// I AM NOT DONE
