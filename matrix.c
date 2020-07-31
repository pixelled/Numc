#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
*/

/* Generates a random double between low and high */
double rand_double(double low, double high) {
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
    srand(seed);
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. `parent` should be set to NULL to indicate that
 * this matrix is not a slice. You should also set `ref_cnt` to 1.
 * You should return -1 if either `rows` or `cols` or both have invalid values, or if any
 * call to allocate memory in this function fails. Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols) {
    if (rows < 1 || cols < 1) return -1;
    matrix *ptr = (matrix *)malloc(sizeof(matrix));
    if (ptr == NULL) return -1;
    ptr -> rows = rows; ptr -> cols = cols;
    ptr -> data = (double *)calloc(rows * cols, sizeof(double));
    if (ptr -> data == NULL) return -1;
    ptr -> ref_cnt = 1;
    ptr -> parent = NULL;
    *mat = ptr;
    return 0;
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * Its data should point to the `offset`th entry of `from`'s data (you do not need to allocate memory)
 * for the data field. `parent` should be set to `from` to indicate this matrix is a slice of `from`.
 * You should return -1 if either `rows` or `cols` or both are non-positive or if any
 * call to allocate memory in this function fails. Return 0 upon success.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows, int cols) {
    if (rows < 1 || cols < 1) return -1;
    matrix *ptr = (matrix *)malloc(sizeof(matrix));
    if (ptr == NULL) return -1;
    ptr -> rows = rows; ptr -> cols = cols;
    ptr -> data = from -> data + offset;
    ptr -> ref_cnt = 1;
    from ->ref_cnt += 1;
    ptr -> parent = from;
    *mat = ptr;
    return 0;
}

/*
 * This function frees the matrix struct pointed to by `mat`. However, you need to make sure that
 * you only free the data if `mat` is not a slice and has no existing slices, or if `mat` is the
 * last existing slice of its parent matrix and its parent matrix has no other references.
 * You cannot assume that mat is not NULL.
 */
void deallocate_matrix(matrix *mat) {
    if (mat == NULL) return;
    if (mat -> ref_cnt == 1) {
        if (mat -> parent) {
            mat -> parent -> ref_cnt -= 1;
        } else {
            free(mat -> data);
        }
        free(mat);
    }
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid.
 */
double get(matrix *mat, int row, int col) {
    return mat -> data[col + row * mat -> cols];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid
 */
void set(matrix *mat, int row, int col, double val) {
    mat -> data[col + row * mat -> cols] = val;
}

/*
 * Sets all entries in mat to val
 */
void fill_matrix(matrix *mat, double val) {
    for (int r = 0; r < mat -> rows; r++) {
        for (int c = 0; c < mat -> cols; c++) {
            mat -> data[c + r * mat -> cols] = val;
        }
    }
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    if (mat1 -> rows != mat2 -> rows || mat1 -> cols != mat2 -> cols) { return 1; }
    int d = mat1 -> rows * mat1 -> cols;
    #pragma omp parallel for
    for (int i = 0; i < d / 8 * 8; i += 8) {
        result -> data[i] = mat1 -> data[i] + mat2 -> data[i];
        result -> data[i + 1] = mat1 -> data[i + 1] + mat2 -> data[i + 1];
        result -> data[i + 2] = mat1 -> data[i + 2] + mat2 -> data[i + 2];
        result -> data[i + 3] = mat1 -> data[i + 3] + mat2 -> data[i + 3];
        result -> data[i + 4] = mat1 -> data[i + 4] + mat2 -> data[i + 4];
        result -> data[i + 5] = mat1 -> data[i + 5] + mat2 -> data[i + 5];
        result -> data[i + 6] = mat1 -> data[i + 6] + mat2 -> data[i + 6];
        result -> data[i + 7] = mat1 -> data[i + 7] + mat2 -> data[i + 7];
    }
    for (int i = d / 8 * 8; i < d; i += 1) {
        result -> data[i] = mat1 -> data[i] + mat2 -> data[i];
    }
    return 0;
}

/*
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    if (mat1 -> rows != mat2 -> rows || mat1 -> cols != mat2 -> cols) { return 1; }
    int d = mat1 -> rows * mat1 -> cols;
    #pragma omp parallel for
    for (int i = 0; i < d; i++) {
        result -> data[i] = mat1 -> data[i] - mat2 -> data[i];
    }
    return 0;
}

/*
 * Store the result of multiplying mat1 and mat2 to result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 */
int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    if (mat1 -> cols != mat2 -> rows) {
        return 1;
    }
    int rows = mat1 -> rows;
    int cols = mat2 -> cols;
    int n = mat1 -> cols;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            double val = 0;
            for (int i = 0; i < n; i++) {
                val += mat1 -> data[i + r * n] * mat2 -> data[c + i * cols];
            }
            result -> data[c + r * cols] = val;
        }
    }
    return 0;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 */
int pow_matrix(matrix *result, matrix *mat, int pow) {
    int rows = mat -> rows; int cols = mat -> cols;
    int size = rows * cols;
    matrix *tmp, *exp;
    allocate_matrix(&tmp, rows, cols);
    allocate_matrix(&exp, rows, cols);
    for (int i = 0; i < size; i++) {
        result -> data[i] = 0;
        exp -> data[i] = mat -> data[i];
    }
    for (int i = 0; i < mat -> rows; i++) {
        result -> data[i + i * mat -> cols] = 1;
    }
    while (pow > 0) {
        if (pow % 2 == 0) {
            mul_matrix(tmp, exp, exp);
            for  (int i = 0; i < size; i++) {
                exp -> data[i] = tmp -> data[i];
            }
            pow /= 2;
        } else {
            for (int i = 0; i < size; i++) {
                tmp -> data[i] = result -> data[i];
            }
            mul_matrix(result, tmp, exp);
            pow--;
        }
    }
    deallocate_matrix(tmp);
    deallocate_matrix(exp);
    return 0;
}

/*
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int neg_matrix(matrix *result, matrix *mat) {
    int rows = result -> rows;
    int cols = result -> cols;
    if (mat -> rows != rows || mat -> cols != cols) {
        return 1;
    }
    for (int i = 0; i < rows * cols; i++) {
        result -> data[i] = -mat -> data[i];
    }
    return 0;
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int abs_matrix(matrix *result, matrix *mat) {
    int rows = result -> rows;
    int cols = result -> cols;
    if (mat -> rows != rows || mat -> cols != cols) {
        return 1;
    }
    for (int i = 0; i < rows * cols; i++) {
        double k = mat -> data[i];
        result -> data[i] = k >= 0 ? k : -k;
    }
    return 0;
}

