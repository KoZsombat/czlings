/* p01_tictactoe -- C basics project 2. Reference solution. */
#include "cztest.h"

#include <stddef.h>

int count_marks(const char board[3][3], char mark)
{
    int count = 0;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (board[row][col] == mark) {
                ++count;
            }
        }
    }
    return count;
}

char board_winner(const char board[3][3])
{
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] &&
            board[i][1] == board[i][2]) {
            return board[i][0];
        }
        if (board[0][i] != ' ' && board[0][i] == board[1][i] &&
            board[1][i] == board[2][i]) {
            return board[0][i];
        }
    }
    if (board[0][0] != ' ' && board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
        return board[0][0];
    }
    if (board[0][2] != ' ' && board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        return board[0][2];
    }
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (board[row][col] == ' ') {
                return ' ';
            }
        }
    }
    return 'D';
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
