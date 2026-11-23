/* p01_tictactoe -- C basics project 2.
 *
 * A 3x3 char board teaches 2D arrays, nested loops and control flow. A cell is
 * 'X', 'O' or ' '.
 *
 *   count_marks(board, mark)  how many cells contain mark
 *   board_winner(board)       'X' or 'O' for a completed row, column or
 *                             diagonal; 'D' when the board is full with no
 *                             winner; ' ' when the game is still running
 *
 * Assume a legal board: at most one mark has a winning line.
 *
 * From TS: `board.flat().filter(...)` builds a new array. Here you index
 * board[row][col] and write the nested loops yourself.
 */
#include "cztest.h"

#include <stddef.h>

int count_marks(const char board[3][3], char mark)
{
    /* TODO: nested loops over rows and columns, count cells equal to mark. */
    (void)board;
    (void)mark;
    return 0;
}

char board_winner(const char board[3][3])
{
    /* TODO: check the three rows, the three columns, both diagonals, then
     * decide between 'D' (full board) and ' ' (game still running). */
    (void)board;
    return ' ';
}

static int has_empty_cell(const char board[3][3])
{
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (board[row][col] == ' ') {
                return 1;
            }
        }
    }
    return 0;
}

int main(void)
{
    const char empty[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };
    CZ_CHECK_INT(count_marks(empty, 'X'), 0);
    CZ_CHECK_INT(count_marks(empty, ' '), 9);
    CZ_CHECK_INT(board_winner(empty), ' ');

    const char x_row[3][3] = {
        {'X', 'X', 'X'},
        {'O', 'O', ' '},
        {' ', ' ', ' '}
    };
    CZ_CHECK_INT(count_marks(x_row, 'X'), 3);
    CZ_CHECK_INT(count_marks(x_row, 'O'), 2);
    CZ_CHECK_INT(board_winner(x_row), 'X');
    CZ_CHECK_INT(has_empty_cell(x_row), 1);

    const char o_column[3][3] = {
        {'X', 'O', ' '},
        {'X', 'O', ' '},
        {' ', 'O', 'X'}
    };
    CZ_CHECK_INT(board_winner(o_column), 'O');

    const char o_diagonal[3][3] = {
        {'O', 'X', ' '},
        {'X', 'O', ' '},
        {' ', 'X', 'O'}
    };
    CZ_CHECK_INT(board_winner(o_diagonal), 'O');

    const char draw[3][3] = {
        {'X', 'O', 'X'},
        {'X', 'O', 'O'},
        {'O', 'X', 'X'}
    };
    CZ_CHECK_INT(board_winner(draw), 'D');
    CZ_CHECK_INT(has_empty_cell(draw), 0);

    const char running[3][3] = {
        {'X', ' ', ' '},
        {' ', 'O', ' '},
        {' ', ' ', ' '}
    };
    CZ_CHECK_INT(board_winner(running), ' ');
    CZ_CHECK_INT(count_marks(running, 'X'), 1);
    CZ_CHECK_INT(count_marks(running, 'O'), 1);

    return cztest_summary();
}

// I AM NOT DONE
