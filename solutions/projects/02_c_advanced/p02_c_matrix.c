/* p02_c_matrix -- C advanced project. Reference solution. */
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
    if (m == NULL || rows == 0 || cols == 0) {
        return -1;
    }
    if (rows > SIZE_MAX / cols) {
        return -1;
    }
    double *data = calloc(rows * cols, sizeof *data);
    if (data == NULL) {
        return -1;
    }
    m->rows = rows;
    m->cols = cols;
    m->data = data;
    return 0;
}

void matrix_free(Matrix *m)
{
    if (m == NULL) {
        return;
    }
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
    if (a == NULL || b == NULL || out == NULL || a->rows != b->rows ||
        a->cols != b->cols) {
        return -1;
    }
    Matrix result = {0, 0, NULL};
    if (matrix_init(&result, a->rows, a->cols) != 0) {
        return -2;
    }
    for (size_t i = 0; i < a->rows * a->cols; ++i) {
        result.data[i] = a->data[i] + b->data[i];
    }
    matrix_free(out);
    *out = result;
    return 0;
}

int matrix_multiply(const Matrix *a, const Matrix *b, Matrix *out)
{
    if (a == NULL || b == NULL || out == NULL || a->cols != b->rows) {
        return -1;
    }
    Matrix result = {0, 0, NULL};
    if (matrix_init(&result, a->rows, b->cols) != 0) {
        return -2;
    }
    for (size_t row = 0; row < a->rows; ++row) {
        for (size_t col = 0; col < b->cols; ++col) {
            double value = 0.0;
            for (size_t k = 0; k < a->cols; ++k) {
                value += a->data[row * a->cols + k] *
                         b->data[k * b->cols + col];
            }
            result.data[row * result.cols + col] = value;
        }
    }
    matrix_free(out);
    *out = result;
    return 0;
}

int matrix_transpose(const Matrix *in, Matrix *out)
{
    if (in == NULL || out == NULL) {
        return -1;
    }
    Matrix result = {0, 0, NULL};
    if (matrix_init(&result, in->cols, in->rows) != 0) {
        return -2;
    }
    for (size_t row = 0; row < in->rows; ++row) {
        for (size_t col = 0; col < in->cols; ++col) {
            result.data[col * result.cols + row] =
                in->data[row * in->cols + col];
        }
    }
    matrix_free(out);
    *out = result;
    return 0;
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
