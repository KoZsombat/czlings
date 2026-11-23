/* p02_c_backtracking -- C advanced project. Reference solution. */
#include "cztest.h"

#include <stddef.h>

static int queen_safe(const int *columns, int row, int column)
{
    for (int earlier = 0; earlier < row; ++earlier) {
        if (columns[earlier] == column) {
            return 0;
        }
        int difference = columns[earlier] - column;
        if (difference < 0) {
            difference = -difference;
        }
        if (difference == row - earlier) {
            return 0;
        }
    }
    return 1;
}

static int place_queens(int n, int row, int *columns)
{
    if (row == n) {
        return 1;
    }
    int total = 0;
    for (int column = 0; column < n; ++column) {
        if (queen_safe(columns, row, column)) {
            columns[row] = column;
            total += place_queens(n, row + 1, columns);
        }
    }
    return total;
}

int count_queens(int n)
{
    if (n <= 0 || n > 12) {
        return 0;
    }
    int columns[12];
    return place_queens(n, 0, columns);
}

static int walk(const char maze[][5], int rows, int cols, int row, int column)
{
    if (row >= rows || column >= cols || maze[row][column] == '#') {
        return 0;
    }
    if (row == rows - 1 && column == cols - 1) {
        return 1;
    }
    return walk(maze, rows, cols, row + 1, column) +
           walk(maze, rows, cols, row, column + 1);
}

int count_paths(const char maze[][5], int rows, int cols)
{
    if (maze == NULL || rows <= 0 || cols <= 0 || cols > 5) {
        return 0;
    }
    return walk(maze, rows, cols, 0, 0);
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
