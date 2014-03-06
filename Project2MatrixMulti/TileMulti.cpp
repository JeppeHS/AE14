#include <cmath>
#include <iostream>
#include <stdio.h>
#include "TileMulti.h"

using namespace std;

static int s = 2;
static matrix* A;
static matrix* B;
static matrix* C;

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
	
	// Debug
	//printf("Initialized output matrix with %d rows and %d cols\n", C->nRows, C->nCols);

	// Assume that both intput matrices is n x n 
	int n = B->nRows;
	int sizeOfSubMat = n/s; 

	// Algorithm
	int ixSize, jxSize, kxSize;
	for (int i = 0; i < s; i++) {
		for (int j = 0; j < s; j++) {
			for (int k = 0; k < s; k++) {
						
				// Debug
				//printf("Run: i: %d, j: %d, k: %d\n", i, j, k);		

				ixSize = i*sizeOfSubMat;
				jxSize = j*sizeOfSubMat;
				kxSize = k*sizeOfSubMat;
			 	multiplyMatrix(ixSize, kxSize, kxSize, jxSize, ixSize, jxSize, sizeOfSubMat);
			}
		}
	}		

	return C;
}

void multiplyMatrix(int ArowOffset, int AcolOffset, int BrowOffset, int BcolOffset, int CrowOffset, int CcolOffset, int size)
{
 	// Debug
 	/*
 	printf("multiply called. size: %d\n", size);
 	printf("ArowOffset %d, AcolOffset %d, BrowOffset %d, BcolOffset %d, CrowOffset %d, CcolOffset %d\n", 
 		ArowOffset, AcolOffset, BrowOffset, BcolOffset, CrowOffset, CcolOffset);
	*/

	int accuRes = 0;
	for (int Arow = 0; Arow < size; Arow++) {
		for (int Bcol = 0; Bcol < size; Bcol++) {
			
			//Calculate C (Arow, Bcol)
			for (int Acol = 0; Acol < size; Acol++) {
				accuRes += matrixGet(A, Arow + ArowOffset, Acol + AcolOffset)*matrixGet(B, Acol + BrowOffset, Bcol + BcolOffset);
			}
			matrixAdd(C, Arow + CrowOffset, Bcol + CcolOffset, accuRes);
			accuRes = 0;
		}		
	}
}



