/* p02_c_backtracking -- C advanced project: recursive search.
 *
 * Prog1 lecture 13 is generic algorithms and backtracking. Two classic
 * problems share the same shape: try a choice, recurse, undo the choice when it
 * fails.
 *
 *   int count_queens(int n);  number of n-queens solutions, 0 for n <= 0
 *                             and for n > 12
 *
 *   int count_paths(const char maze[][5], int rows, int cols);
 *                             number of right/down paths from (0,0) to
 *                             (rows-1, cols-1), where '#' is a wall.
 *                             rows and cols must be positive; cols is at most 5.
 *
 * For the maze, only right and down moves are allowed, so the grid is a DAG.
 * For the queens, one queen is placed per row and the recursion rejects a
 * column that attacks an earlier queen.
 *
 * From TS: a recursive function returns a number and the call stack keeps the
 * state. Here the state is an explicit array of chosen columns, and each
 * recursive call undoes its choice by returning.
 */
#include "cztest.h"

#include <stddef.h>

int count_queens(int n)
{
    /* TODO: place one queen per row. Keep chosen columns in a local array;
     * reject a column that is equal to or diagonal from an earlier queen.
     * Count complete boards. */
    (void)n;
    return 0;
}

int count_paths(const char maze[][5], int rows, int cols)
{
    /* TODO: return 0 for a wall/out-of-range cell, 1 at the destination, and
     * the sum of the right and down recursive calls otherwise. */
    (void)maze;
    (void)rows;
    (void)cols;
    return 0;
}

int main(void)
{
    CZ_CHECK_INT(count_queens(0), 0);
    CZ_CHECK_INT(count_queens(-3), 0);
    CZ_CHECK_INT(count_queens(1), 1);
    CZ_CHECK_INT(count_queens(4), 2);
    CZ_CHECK_INT(count_queens(6), 4);
    CZ_CHECK_INT(count_queens(8), 92);

    const char open3[3][5] = {
        "...",
        "...",
        "..."
    };
    CZ_CHECK_INT(count_paths(open3, 3, 3), 6);

    const char center[3][5] = {
        "...",
        ".#.",
        "..."
    };
    CZ_CHECK_INT(count_paths(center, 3, 3), 2);

    const char four[4][5] = {
        {'.', '.', '.', '.', '.'},
        {'.', '#', '.', '.', '.'},
        {'.', '.', '#', '.', '.'},
        {'.', '.', '.', '.', '.'}
    };
    CZ_CHECK_INT(count_paths(four, 4, 5), 9);

    const char blocked[4][5] = {
        {'#', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '#'}
    };
    CZ_CHECK_INT(count_paths(blocked, 4, 5), 0);

    const char one[1][5] = {"."};
    CZ_CHECK_INT(count_paths(one, 1, 1), 1);
    CZ_CHECK_INT(count_paths(NULL, 3, 3), 0);

    return cztest_summary();
}

// I AM NOT DONE
