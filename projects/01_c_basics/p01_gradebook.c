/* p01_gradebook -- C basics project 1.
 *
 * After chapter 1 you know arrays, loops, functions, switch and integer
 * arithmetic. Build the small grading helpers used by a teacher:
 *
 *   average_score(scores, count)   mean as a double; 0.0 for an empty class
 *   letter_grade(score)            A >= 90, B >= 80, C >= 70, D >= 60, else F
 *   count_passing(scores, count)   scores >= 60
 *   best_score(scores, count)      largest score, or -1 for an empty class
 *
 * Scores are ordinary ints. The array does not carry its length, so every
 * helper takes `count` separately.
 *
 * From TS: an array has `.length` and `.reduce`; here the length is a separate
 * parameter and a loop updates an accumulator you declare yourself.
 */
#include "cztest.h"

#include <stddef.h>

double average_score(const int *scores, size_t count)
{
    /* TODO: sum the scores and divide by count. Guard count == 0. */
    (void)scores;
    (void)count;
    return 0.0;
}

char letter_grade(int score)
{
    /* TODO: return 'A', 'B', 'C', 'D' or 'F' using the boundaries above. */
    (void)score;
    return '?';
}

size_t count_passing(const int *scores, size_t count)
{
    /* TODO: count scores >= 60. */
    (void)scores;
    (void)count;
    return 0;
}

int best_score(const int *scores, size_t count)
{
    /* TODO: return the largest score, or -1 when count == 0. */
    (void)scores;
    (void)count;
    return -1;
}

int main(void)
{
    const int scores[] = {95, 82, 71, 60, 59, 100, 45};
    const size_t score_count = sizeof scores / sizeof scores[0];

    CZ_CHECK_NEAR(average_score(scores, score_count), 512.0 / 7.0, 1e-9);
    CZ_CHECK_NEAR(average_score(scores, 0), 0.0, 1e-9);

    CZ_CHECK_INT(letter_grade(90), 'A');
    CZ_CHECK_INT(letter_grade(89), 'B');
    CZ_CHECK_INT(letter_grade(80), 'B');
    CZ_CHECK_INT(letter_grade(79), 'C');
    CZ_CHECK_INT(letter_grade(70), 'C');
    CZ_CHECK_INT(letter_grade(69), 'D');
    CZ_CHECK_INT(letter_grade(60), 'D');
    CZ_CHECK_INT(letter_grade(59), 'F');

    CZ_CHECK_INT((long)count_passing(scores, score_count), 5);
    CZ_CHECK_INT((long)count_passing(scores, 0), 0);
    CZ_CHECK_INT(best_score(scores, score_count), 100);
    CZ_CHECK_INT(best_score(scores, 0), -1);

    return cztest_summary();
}

// I AM NOT DONE
