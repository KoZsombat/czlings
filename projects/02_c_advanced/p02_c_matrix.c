/* p02_c_matrix -- C advanced project: a dynamic 2D matrix.
 *
 * Prog1 covers arrays, dynamic memory and 2D indexing. This project builds a
 * row-major matrix with one contiguous malloc/calloc buffer, then implements
 * addition, multiplication and transpose.
 *
 *   typedef struct Matrix { size_t rows, cols; double *data; };
 *
 *   matrix_init(m, rows, cols)       allocate zeroed storage; 0 / -1
 *   matrix_free(m)                   release and reset (provided)
 *   matrix_get(m, r, c)              value at row r, column c (provided)
 *   matrix_set(m, r, c, value)       store one value (provided)
 *   matrix_add(a, b, out)            out = a + b; 0 / -1 shape / -2 alloc
 *   matrix_multiply(a, b, out)       out = a * b; 0 / -1 shape / -2 alloc
 *   matrix_transpose(in, out)        out = in transposed; 0 / -2 alloc
 *
 * `out` may be uninitialised or already freed; each operation builds a
 * temporary and only replaces `out` on success. A matrix owns exactly one
 * buffer, so matrix_free() is the only release point.
 *
 * From TS: a 2D array is an array of arrays managed by the runtime. Here the
 * rows share one flat buffer and you compute the index as row * cols + col.
 */
#include "cztest.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Matrix {
    size_t rows;
    size_t cols;
    double *data;
} Matrix;

int matrix_init(Matrix *m, size_t rows, size_t cols)
{
    /* TODO: reject NULL, zero dimensions and rows * cols overflow; calloc the
     * flat buffer; store rows, cols and data. Return 0 or -1. */
    (void)rows;
    (void)cols;
    m->rows = 0;
    m->cols = 0;
    m->data = NULL;
    return -1;
}

void matrix_free(Matrix *m)
{
    free(m->data);
    m->rows = 0;
    m->cols = 0;
    m->data = NULL;
}

double matrix_get(const Matrix *m, size_t row, size_t col)
{
    return m->data[row * m->cols + col];
}

void matrix_set(Matrix *m, size_t row, size_t col, double value)
{
    m->data[row * m->cols + col] = value;
}

int matrix_add(const Matrix *a, const Matrix *b, Matrix *out)
{
    /* TODO: check shapes; allocate a temporary; add element-wise; replace
     * *out on success. Return -1 on a shape mismatch, -2 on allocation
     * failure. */
    (void)a;
    (void)b;
    (void)out;
    return -1;
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *out)
{
    /* TODO: require a->cols == b->rows; allocate a temporary a->rows by
     * b->cols; accumulate sum over k of a(i,k) * b(k,j); replace *out. */
    (void)a;
    (void)b;
    (void)out;
    return -1;
}

int matrix_transpose(const Matrix *in, Matrix *out)
{
    /* TODO: allocate a temporary in->cols by in->rows and copy
     * out(col, row) = in(row, col). */
    (void)in;
    (void)out;
    return -1;
}

int main(void)
{
    Matrix a = {0, 0, NULL};
    Matrix b = {0, 0, NULL};
    Matrix out = {0, 0, NULL};

    CZ_CHECK_INT(matrix_init(&a, 2, 3), 0);
    CZ_CHECK_INT(matrix_init(&b, 3, 2), 0);
    if (a.data == NULL || b.data == NULL) {
        matrix_free(&a);
        matrix_free(&b);
        return cztest_summary();
    }

    double next = 1.0;
    for (size_t row = 0; row < a.rows; ++row) {
        for (size_t col = 0; col < a.cols; ++col) {
            matrix_set(&a, row, col, next);
            next += 1.0;
        }
    }
    next = 7.0;
    for (size_t row = 0; row < b.rows; ++row) {
        for (size_t col = 0; col < b.cols; ++col) {
            matrix_set(&b, row, col, next);
            next += 1.0;
        }
    }

    CZ_CHECK_NEAR(matrix_get(&a, 0, 0), 1.0, 1e-9);
    CZ_CHECK_NEAR(matrix_get(&a, 1, 2), 6.0, 1e-9);
    CZ_CHECK_NEAR(matrix_get(&b, 2, 1), 12.0, 1e-9);

    CZ_CHECK_INT(matrix_multiply(&a, &b, &out), 0);
    CZ_CHECK_INT((long)out.rows, 2);
    CZ_CHECK_INT((long)out.cols, 2);
    if (out.data != NULL) {
        CZ_CHECK_NEAR(matrix_get(&out, 0, 0), 58.0, 1e-9);
        CZ_CHECK_NEAR(matrix_get(&out, 0, 1), 64.0, 1e-9);
        CZ_CHECK_NEAR(matrix_get(&out, 1, 0), 139.0, 1e-9);
        CZ_CHECK_NEAR(matrix_get(&out, 1, 1), 154.0, 1e-9);
    }

    CZ_CHECK_INT(matrix_add(&a, &a, &out), 0);
    CZ_CHECK_INT((long)out.rows, 2);
    CZ_CHECK_INT((long)out.cols, 3);
    if (out.data != NULL) {
        CZ_CHECK_NEAR(matrix_get(&out, 0, 0), 2.0, 1e-9);
        CZ_CHECK_NEAR(matrix_get(&out, 1, 2), 12.0, 1e-9);
    }

    CZ_CHECK_INT(matrix_transpose(&a, &out), 0);
    CZ_CHECK_INT((long)out.rows, 3);
    CZ_CHECK_INT((long)out.cols, 2);
    if (out.data != NULL) {
        CZ_CHECK_NEAR(matrix_get(&out, 0, 0), 1.0, 1e-9);
        CZ_CHECK_NEAR(matrix_get(&out, 0, 1), 4.0, 1e-9);
        CZ_CHECK_NEAR(matrix_get(&out, 2, 1), 6.0, 1e-9);
    }

    CZ_CHECK_INT(matrix_multiply(&a, &a, &out), -1);
    CZ_CHECK_INT(matrix_add(&a, &b, &out), -1);

    matrix_free(&a);
    matrix_free(&b);
    matrix_free(&out);
    CZ_CHECK(a.data == NULL && b.data == NULL && out.data == NULL);

    return cztest_summary();
}

// I AM NOT DONE
