#include <cmath>
#include <iostream>
#include <stdio.h>
#include "TileMulti.h"

using namespace std;

static int s = 4;
static matrix* A;
static matrix* B;
static matrix* C;

void multiplyMatrix(int ArowOffset, int AcolOffset, int BcolOffset, int AnRows, int AnCols, int BnCols);

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
	C = createMatrix(A->nRows, B->nCols);	

	// Debug
	//printf("Initialized output matrix with %d rows and %d cols\n", C->nRows, C->nCols);

	if (A->nRows < s || B->nCols < s) { s = 1; }	// If can't split matrix 

	// Algorithm
	int subAnRows, subAnCols, subBnCols;
	int rowsVisitedA = 0;
	int colsVisitedA = 0;
	int colsVisitedB = 0;

	for (int i = 0; i < s; i++) {
		colsVisitedB = 0;
		subAnRows = ceil( (double) (A->nRows - rowsVisitedA) / (double) (s - i) );

		for (int j = 0; j < s; j++) {
			colsVisitedA = 0;
			subBnCols = ceil( (double) (B->nCols - colsVisitedB) / (double) (s - j) );

			for (int k = 0; k < s; k++) {
				subAnCols = ceil( (double) (A->nCols - colsVisitedA) / (double) (s - k) );

				// Debug
				/*
				printf("\ti: %d, j: %d, k: %d\n", i, j, k);
				printf("A %dx%d, B %dx%d\n", subAnRows, subAnCols, subBnRows, subBnCols);
				printf("rowsVisitedA %d, colsVisitedA %d, rowsVisitedB %d, colsVisitedB %d\n", 
					rowsVisitedA, colsVisitedA, rowsVisitedB, colsVisitedB);
				*/
				multiplyMatrix(rowsVisitedA, colsVisitedA, colsVisitedB, subAnRows, subAnCols, subBnCols);

			 	colsVisitedA += subAnCols;
			}
			colsVisitedB += subBnCols;	
		}
		rowsVisitedA += subAnRows;		
	}		

	return C;
}

void multiplyMatrix(int ArowOffset, int AcolOffset, int BcolOffset, int AnRows, int AnCols, int BnCols)
{
 	// Debug
 	/*
 	printf("multiply called. size: %d\n", size);
 	printf("ArowOffset %d, AcolOffset %d, BrowOffset %d, BcolOffset %d, CrowOffset %d, CcolOffset %d\n", 
 		ArowOffset, AcolOffset, BrowOffset, BcolOffset, CrowOffset, CcolOffset);
	*/

	int accuRes = 0;
	for (int Arow = 0; Arow < AnRows; Arow++) {
		for (int Bcol = 0; Bcol < BnCols; Bcol++) {
			
			//Calculate C (Arow, Bcol)
			for (int Acol = 0; Acol < AnCols; Acol++) {
				accuRes += matrixGet(A, Arow + ArowOffset, Acol + AcolOffset)*matrixGet(B, Acol + AcolOffset, Bcol + BcolOffset);
			}
			matrixAdd(C, Arow + ArowOffset, Bcol + BcolOffset, accuRes);
			accuRes = 0;
		}		
	}
}



