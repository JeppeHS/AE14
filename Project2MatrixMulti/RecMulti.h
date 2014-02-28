#ifndef RECMULTI_H
#define RECMULTI_H

#include "IMatrixMulti.h"

class RecMulti : public IMatrixMulti
{
public:
	RecMulti();
	~RecMulti();
	void setup(matrix* A, int height, int width);
	matrix* matrixMultiplication(matrix* B);
};

#endif
