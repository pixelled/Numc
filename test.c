#include "matrix.h"

int main() {
	matrix *result = NULL;
	matrix *mat1 = NULL;
	matrix *mat2 = NULL;
	allocate_matrix(&result, 1, 1);
	allocate_matrix(&mat1, 1, 1);
	allocate_matrix(&mat2, 1, 1);
	set(mat1, 0, 0, 1.2);
	set(mat2, 0, 0, 1.2);
	mul_matrix(result, mat1, mat2);
	return 0;
}