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
    if (rows < 1 || cols < 1) {
        PyErr_SetString(PyExc_TypeError, "Invalid Dimension");
        return -1;
    }
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
    if (rows < 1 || cols < 1) {
        PyErr_SetString(PyExc_TypeError, "Invalid Dimension");
        return -1;
    }
    matrix *ptr = (matrix *)malloc(sizeof(matrix));
    if (ptr == NULL) return -1;
    ptr -> rows = rows; ptr -> cols = cols;
    ptr -> data = from -> data + offset;
    ptr -> ref_cnt = 1;
    from -> ref_cnt += 1;
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
    matrix *ptr;
    while (mat) {
        mat -> ref_cnt -= 1;
        if (!mat -> ref_cnt) {
            if (!mat -> parent) free(mat -> data);
            ptr = mat -> parent;
            free(mat);
            mat = ptr;
        } else {
            break;
        }
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
    for (int i = 0; i < d / 8 * 8; i += 8) {
        result -> data[i] = mat1 -> data[i] - mat2 -> data[i];
        result -> data[i + 1] = mat1 -> data[i + 1] - mat2 -> data[i + 1];
        result -> data[i + 2] = mat1 -> data[i + 2] - mat2 -> data[i + 2];
        result -> data[i + 3] = mat1 -> data[i + 3] - mat2 -> data[i + 3];
        result -> data[i + 4] = mat1 -> data[i + 4] - mat2 -> data[i + 4];
        result -> data[i + 5] = mat1 -> data[i + 5] - mat2 -> data[i + 5];
        result -> data[i + 6] = mat1 -> data[i + 6] - mat2 -> data[i + 6];
        result -> data[i + 7] = mat1 -> data[i + 7] - mat2 -> data[i + 7];
    }
    for (int i = d / 8 * 8; i < d; i += 1) {
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
        return -1;
    }
    int rows = mat1 -> rows; int cols = mat2 -> cols; int n = mat1 -> cols; int s = n >> 2; int sl = (n + 3) >> 2;
    __m256d _res, _res1; __m256d colt[sl], colt1[sl], mat1t[rows]; double arr[4], arr1[4];
    int t = n % 4;
    __m256d vec1 = _mm256_set_pd(4, 3, 2, 1);
    __m256d vec2 = _mm256_set_pd(t, t, t, t);
    __m256d msk = _mm256_cmp_pd(vec2, vec1, 5);
    #pragma omp parallel for
    for (int r = 0; r < rows; r++) {
        mat1t[r] = _mm256_and_pd(_mm256_loadu_pd(&(mat1 -> data[r * n + s * 4])), msk);
    }
    #pragma omp parallel for private(_res, _res1, colt, colt1, arr, arr1)
    for (int c = 0; c < cols / 2 * 2; c += 2) {
        int c1 = c + 1;
        for (int i = 0; i < sl; i += 1) {
            int k = 4 * i;
            colt[i] = _mm256_set_pd(mat2 -> data[c + (k + 3) * cols], mat2 -> data[c + (k + 2) * cols],
                                    mat2 -> data[c + (k + 1) * cols],mat2 -> data[c + k * cols]);
            colt1[i] = _mm256_set_pd(mat2 -> data[c1 + (k + 3) * cols], mat2 -> data[c1 + (k + 2) * cols],
                                    mat2 -> data[c1 + (k + 1) * cols],mat2 -> data[c1 + k * cols]);
        }
        if (t) {
            colt[s] = _mm256_and_pd(colt[s], msk);
            colt1[s] = _mm256_and_pd(colt1[s], msk);
        }
        for (int r = 0; r < rows; r++) {
            _res = _mm256_set1_pd(0);
            _res1 = _mm256_set1_pd(0);
            int k = r * n;
            for (int i = 0; i < s; i += 1) {
                _res = _mm256_fmadd_pd(_mm256_loadu_pd(&(mat1 -> data[i * 4 + k])), colt[i], _res);
                _res1 = _mm256_fmadd_pd(_mm256_loadu_pd(&(mat1 -> data[i * 4 + k])), colt1[i], _res1);
            }
            if (t) {
                _res = _mm256_fmadd_pd(mat1t[r], colt[s], _res);
                _res1 = _mm256_fmadd_pd(mat1t[r], colt1[s], _res1);
            }
            _mm256_storeu_pd(&arr[0], _res);
            _mm256_storeu_pd(&arr1[0], _res1);
            result -> data[c + r * cols] = arr[0] + arr[1] + arr[2] + arr[3];;
            result -> data[c1 + r * cols] = arr1[0] + arr1[1] + arr1[2] + arr1[3];;
        }
    }
    for (int c = cols / 2 * 2; c < cols; c++) {
        for (int i = 0; i < sl; i += 1) {
            int k = 4 * i;
            colt[i] = _mm256_set_pd(mat2 -> data[c + (k + 3) * cols], mat2 -> data[c + (k + 2) * cols],
                                    mat2 -> data[c + (k + 1) * cols],mat2 -> data[c + k * cols]);
        }
        if (t) {
            colt[s] = _mm256_and_pd(colt[s], msk);
        }
        for (int r = 0; r < rows; r++) {
            _res = _mm256_set1_pd(0);
            int k = r * n;
            for (int i = 0; i < s; i += 1) {
                _res = _mm256_fmadd_pd(_mm256_loadu_pd(&(mat1 -> data[i * 4 + k])), colt[i], _res);
            }
            if (t) {
                _res = _mm256_fmadd_pd(mat1t[r], colt[s], _res);
            }
            _mm256_storeu_pd(&arr[0], _res);
            result -> data[c + r * cols] = arr[0] + arr[1] + arr[2] + arr[3];;
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
    if (rows != cols || pow < 0) return -1;
    int size = rows * cols;
    matrix *res, *mat0;
    allocate_matrix(&res, rows, cols);
    allocate_matrix(&mat0, rows, cols);
    for (int i = 0; i < size; i++) {
        result -> data[i] = 0;
        mat0 -> data[i] = mat -> data[i];
    }
    for (int i = 0; i < mat -> rows; i++) {
        result -> data[i + i * mat -> cols] = 1;
    }
    while (pow > 0) {
        if (pow % 2 == 0) {
            mul_matrix(res, mat0, mat0);
            for  (int i = 0; i < size; i++) {
                mat0 -> data[i] = res -> data[i];
            }
            pow >>= 1;
        } else {
            for (int i = 0; i < size; i++) {
                res -> data[i] = result -> data[i];
            }
            mul_matrix(result, res, mat0);
            pow--;
        }
    }
    deallocate_matrix(res);
    deallocate_matrix(mat0);
    return 0;
}

/*
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int neg_matrix(matrix *result, matrix *mat) {
    int d = result -> rows * result -> cols;
    #pragma omp parallel for
    for (int i = 0; i < d; i++) {
        result -> data[i] = -mat -> data[i];
    }
    return 0;
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int abs_matrix(matrix *result, matrix *mat) {
    int d = mat -> rows * mat -> cols;
    #pragma omp parallel for
    for (int i = 0; i < d; i++) {
        double k = mat -> data[i];
        result -> data[i] = k >= 0 ? k : -k;
    }
    return 0;
}

