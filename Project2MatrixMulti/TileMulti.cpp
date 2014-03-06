#include <cmath>
#include <iostream>
#include <stdio.h>
#include "TileMulti.h"

using namespace std;

int s = 2;
matrix* A;
matrix* B;
matrix* C;

void multiplyMatrix(int ArowOffset, int AcolOffset, int BrowOffset, int BcolOffset, int CrowOffset, int CcolOffset, int size); 

TileMulti::TileMulti() 
{
}

TileMulti::~TileMulti() 
{
}

void TileMulti::setup(matrix* matA, int height, int width) 
{
	A = matA;
}

matrix * TileMulti::matrixMultiplication(matrix* matB)
{
	B = matB;
	
	// Create output matrix
	C = createMatrix(B->nRows, A->nCols);	
	printf("Initialized output matrix with %d rows and %d cols\n", C->nRows, C->nCols);
	
	// Assume that both intput matrices is n x n 
	int n = B->nRows;
	int sizeOfSubMat = n/s; 
	
	// Algorithm
	for (int i = 0; i < s; i++) {
		for (int j = 0; j < s; j++) {
			for (int k = 0; k < s; k++) {
						
				printf("Run: i: %d, j: %d, k: %d\n", i, j, k);		

				int ArowOffset = i*sizeOfSubMat;	
			 	int AcolOffset = k*sizeOfSubMat;
			 	int BrowOffset = k*sizeOfSubMat;	
			 	int BcolOffset = j*sizeOfSubMat;
			 	int CrowOffset = i*sizeOfSubMat;	
			 	int CcolOffset = j*sizeOfSubMat;

			 	multiplyMatrix(ArowOffset, AcolOffset, BrowOffset, BcolOffset, CrowOffset, CcolOffset, sizeOfSubMat);
			}
		}
	}		

	return C;
}

void multiplyMatrix(int ArowOffset, int AcolOffset, int BrowOffset, int BcolOffset, int CrowOffset, int CcolOffset, int size)
{
 	printf("multiply called. size: %d\n", size);
 	printf("ArowOffset %d, AcolOffset %d, BrowOffset %d, BcolOffset %d, CrowOffset %d, CcolOffset %d\n", 
 		ArowOffset, AcolOffset, BrowOffset, BcolOffset, CrowOffset, CcolOffset);

	
	int accuRes = 0;
	for (int Arow = 0; Arow < size; Arow++) {
		for (int Bcol = 0; Bcol < size; Bcol++) {
			
			//Calculate C (Arow, Bcol)
			for (int Acol = 0; Acol < size; Acol++) {
				accuRes += matrixGet(A, Arow + ArowOffset, Acol + AcolOffset)*matrixGet(B, Acol + BrowOffset, Bcol + BcolOffset);
			}
			matrixPut(C, Arow + CrowOffset, Bcol + CcolOffset, accuRes);
			accuRes = 0;
		}		
	}
}



