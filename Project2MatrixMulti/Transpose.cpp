#include <iostream>
#include "Transpose.h"

using namespace std;
matrix * mC;
matrix * p;

Transpose::Transpose(){
}
Transpose::~Transpose(){}

void Transpose::setup(matrix* A, int rows, int cols){
	mC = transpose(A, rows, cols);
}

matrix* Transpose::transpose(matrix* B, int rows, int cols){
	matrix* result = createMatrix(cols, rows);
	for ( int i = 0; i < rows; i++ )
    {
       for ( int j = 0; j < cols; j++ )
       {
		   matrixPut(result, j, i, matrixGet(B, i, j));
		   //std::cout << matrixGet(result, j, i) << " ";
       }
	   //std::cout << "\n";
    }
	//std::cout << "\n";
	return result;
}

matrix* Transpose::matrixMultiplication(matrix* B){
	p = createMatrix((*mC).nCols, (*B).nCols);
	for(int colA=0; colA<(*mC).nCols; colA++)
	{
		for(int colB=0; colB<(*B).nCols; colB++)
		{
			int value = 0;
			// Multiply the row of A by the column of B to get the row, column of product.
			for (int inner = 0; inner < (*mC).nRows; inner++) {
				value += matrixGet(mC, inner, colA) * matrixGet(B, inner, colB);
			}
			matrixPut(p, colA, colB, value);
			//Print
			std::cout << matrixGet(p, colA, colB) << " ";
		}
		//Print
		std::cout << "\n";
	}
	std::cout << "\n";
	return p;
}
