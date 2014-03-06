#ifndef TRANSPOSE_H
#define TRANSPOSE_H

#include "IMatrixMulti.h"
class Transpose : public IMatrixMulti
{
public:
	Transpose();
	~Transpose();
	void setup(matrix* A, int height, int width);
	matrix* transpose(matrix* B, int row, int col);
	matrix* matrixMultiplication(matrix* B);
};

#endif