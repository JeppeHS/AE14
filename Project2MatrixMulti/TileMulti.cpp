#include <cmath>
#include <iostream>
#include <stdio.h>
#include "TileMulti.h"

using namespace std;

matrix* mA;

TileMulti::TileMulti() 
{
}

TileMulti::~TileMulti() 
{
}

void TileMulti::setup(matrix* A, int height, int width) 
{
	mA = A;
}

matrix * TileMulti::matrixMultiplication(matrix* B)
{
	// Create output matrix
	matrix* C = createMatrix((*B).nRows, (*mA).nCols);	
	printf("Initialized output matrix with %d rows and %d cols\n", (*C).nRows, (*C).nCols);
	



	matrixPut(C, 1, 1, 12);

	printf("Get 1,1 : %d\n", matrixGet(C, 1, 1));

	return C;
}