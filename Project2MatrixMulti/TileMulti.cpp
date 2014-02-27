#include <cmath>
#include <iostream>
#include <stdio.h>
#include "TileMulti.h"

using namespace std;

int AHeight, AWidth; 

TileMulti::TileMulti() 
{
}

TileMulti::~TileMulti() 
{
}

void TileMulti::setup(matrix* A, int height, int width) 
{
	AHeight = height;
	AWidth = width;
}

matrix * TileMulti::matrixMultiplication(matrix* B)
{
	int CSize = min(AHeight, AWidth);
	matrix* C = createMatrix(CSize, CSize);	

	matrixPut(C, 1, 1, 12);

	printf("Get 1,1 : %f\n", matrixGet(C, 1, 1));

	return C;
}