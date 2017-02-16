#ifndef _MATRIX_MULTIPLY_H_
#define _MATRIX_MULTIPLY_H_

#include "CommonTools.h"

class MatrixMultiplyTester {
public:
	MatrixMultiplyTester();
	virtual ~MatrixMultiplyTester();
	void test();

private:
	void initialise_matrix_column(ne10_mat_row3f *col);
	void initialise_matrix(ne10_mat3x3f_t *mat);
};
#endif	//_MATRIX_MULTIPLY_H_
