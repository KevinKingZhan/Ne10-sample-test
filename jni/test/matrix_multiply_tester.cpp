#include "matrix_multiply_tester.h"

#define LOG_TAG "Ne10_MatrixMultiplyTester"

#define MATRICES 3

MatrixMultiplyTester::MatrixMultiplyTester() {
}

MatrixMultiplyTester::~MatrixMultiplyTester() {
}

void MatrixMultiplyTester::test() {
	ne10_mat3x3f_t src[MATRICES]; // A source array of `MATRICES` input matrices
	ne10_mat3x3f_t mul[MATRICES]; // An array of matrices to multiply those in `src` by
	ne10_mat3x3f_t dst[MATRICES]; // A destination array for the multiplication results

	// Initialise Ne10, using hardware auto-detection to set library function pointers.
	if (ne10_init() != NE10_OK) {
		LOGE("Failed to initialise Ne10.\n");
	}

	// Generate test input values
	for (int i = 0; i < MATRICES; i++) {
		initialise_matrix(&src[i]);
		initialise_matrix(&mul[i]);
	}

	// Perform the multiplication of the matrices in `src` by those in `mul`
	ne10_mulmat_3x3f(dst, src, mul, MATRICES);

	// Display the results (src[i] * mul[i] == dst[i])
	for (int i = 0; i < MATRICES; i++) {
		LOGI("[ %5.2f %5.2f %5.2f     [ %5.2f %5.2f %5.2f     [ %5.2f %5.2f %5.2f\n", src[i].c1.r1, src[i].c2.r1, src[i].c3.r1, mul[i].c1.r1, mul[i].c2.r1, mul[i].c3.r1,
				dst[i].c1.r1, dst[i].c2.r1, dst[i].c3.r1);
		LOGI("  %5.2f %5.2f %5.2f   *   %5.2f %5.2f %5.2f   =   %5.2f %5.2f %5.2f\n", src[i].c1.r2, src[i].c2.r2, src[i].c3.r2, mul[i].c1.r2, mul[i].c2.r2, mul[i].c3.r2,
				dst[i].c1.r2, dst[i].c2.r2, dst[i].c3.r2);
		LOGI("  %5.2f %5.2f %5.2f ]     %5.2f %5.2f %5.2f ]     %5.2f %5.2f %5.2f ]\n", src[i].c1.r3, src[i].c2.r3, src[i].c3.r3, mul[i].c1.r3, mul[i].c2.r3, mul[i].c3.r3,
				dst[i].c1.r3, dst[i].c2.r3, dst[i].c3.r3);
		LOGI("\n");
	}
}

void MatrixMultiplyTester::initialise_matrix(ne10_mat3x3f_t *mat) {
	initialise_matrix_column(&mat->c1);
	initialise_matrix_column(&mat->c2);
	initialise_matrix_column(&mat->c3);
}

void MatrixMultiplyTester::initialise_matrix_column(ne10_mat_row3f *col) {
	col->r1 = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
	col->r2 = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
	col->r3 = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
}
