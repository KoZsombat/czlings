/* c11_2d_arrays -- row-major matrices.
 *
 * Task: add two 3x3 matrices and transpose one. A fixed int[3][3] is stored
 * row by row, so m[r][c] sits at offset r * 3 + c; a 2D parameter needs the
 * column count in its type, and the loops must cover every row and column.
 *
 * Note: a fixed-size 2D array is one contiguous block of rows, not an array
 * of separate row objects; it cannot be ragged or resized.
 */
#include "cztest.h"

void mat_add(const int a[3][3], const int b[3][3], int out[3][3])
{
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            out[r][c] = a[r][c] + b[r][c];
        }
    }
}

void mat_transpose(const int in[3][3], int out[3][3])
{
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            out[r][c] = in[c][r];
        }
    }
}

int mat_trace(const int m[3][3])
{
    return m[0][0] + m[1][1] + m[2][2];
}

int main(void)
{
    int a[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int b[3][3] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    int sum_expected[3][3] = {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}};
    int transposed_expected[3][3] = {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}};
    int sum[3][3] = {{0}};
    int transposed[3][3] = {{0}};

    mat_add(a, b, sum);
    CZ_CHECK_MEM(sum, sum_expected, sizeof sum);

    mat_transpose(a, transposed);
    CZ_CHECK_MEM(transposed, transposed_expected, sizeof transposed);

    CZ_CHECK_INT(mat_trace(a), 15);
    CZ_CHECK_INT(mat_trace(b), 15);

    return cztest_summary();
}
