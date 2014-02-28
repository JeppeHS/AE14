#ifndef NaiveMatrixMulti_H
#define NaiveMatrixMulti_H

#include "IMatrixMulti.h"
class NaiveMatrixMulti : public IMatrixMulti
{
public:
	NaiveMatrixMulti();
	~NaiveMatrixMulti();
	void setup(matrix* A, int height, int width);
	matrix* matrixMultiplication(matrix* B);
};

#endif