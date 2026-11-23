/* p01_gradebook -- C basics project 1. Reference solution. */
#include "cztest.h"

#include <stddef.h>

double average_score(const int *scores, size_t count)
{
    if (count == 0) {
        return 0.0;
    }
    double total = 0.0;
    for (size_t i = 0; i < count; ++i) {
        total += scores[i];
    }
    return total / (double)count;
}

char letter_grade(int score)
{
    if (score >= 90) {
        return 'A';
    }
    if (score >= 80) {
        return 'B';
    }
    if (score >= 70) {
        return 'C';
    }
    if (score >= 60) {
        return 'D';
    }
    return 'F';
}

size_t count_passing(const int *scores, size_t count)
{
    size_t passing = 0;
    for (size_t i = 0; i < count; ++i) {
        if (scores[i] >= 60) {
            ++passing;
        }
    }
    return passing;
}

int best_score(const int *scores, size_t count)
{
    if (count == 0) {
        return -1;
    }
    int best = scores[0];
    for (size_t i = 1; i < count; ++i) {
        if (scores[i] > best) {
            best = scores[i];
        }
    }
    return best;
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
