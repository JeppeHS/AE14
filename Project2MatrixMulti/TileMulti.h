
#ifndef TILEMULTI_H
#define TILEMULTI_H

#include "IMatrixMulti.h"
#include "matrix.h"

class TileMulti : public IMatrixMulti
{
public:
	TileMulti();
	~TileMulti();
	void setup(matrix* A, int height, int width);
	matrix* matrixMultiplication(matrix* B);
};

#endif

