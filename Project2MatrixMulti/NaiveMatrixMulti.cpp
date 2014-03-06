#include <iostream>
#include "NaiveMatrixMulti.h"

using namespace std;
static matrix * mA;
static matrix * product;

NaiveMatrixMulti::NaiveMatrixMulti(){
}
NaiveMatrixMulti::~NaiveMatrixMulti(){}
void NaiveMatrixMulti::setup(matrix* A, int height, int width){
	mA = A;
}

matrix * NaiveMatrixMulti::matrixMultiplication(matrix* B){
	product = createMatrix((*mA).nRows, (*B).nCols);
	for(int row=0; row<(*mA).nRows; row++)
	{
		for(int col=0; col<(*B).nCols; col++)
		{
			double value = 0;
			// Multiply the row of A by the column of B to get the row, column of product.
			for (int inner = 0; inner < (*mA).nCols; inner++) {
				value += matrixGet(mA, row, inner) * matrixGet(B, inner, col);
			}
			matrixPut(product, row, col, value);
			//Print
			//std::cout << matrixGet(product, row, col) << " ";
		}
		//Print
		//std::cout << "\n";
	}
	return product;
}
