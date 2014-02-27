#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>
#include <cstdio>
#include <cassert>

typedef struct {int nRows, nCols; double* data;} matrix;

matrix* createMatrix(int nRows, int nCols)
{
  matrix* m = (matrix*)malloc(sizeof(matrix));
	if( m==NULL ) {
		fprintf(stderr,"error in my_matrix_alloc\n");
		return NULL;
	}
  (*m).nRows = nRows;
  (*m).nCols = nCols;
  (*m).data = (double*) malloc(nRows*nCols*sizeof(double));
  return m;
}

void destroyMatrix(matrix* m) { 
	free(m -> data); 
	free(m); 
}

void matrixPut(matrix* m, int i, int j, double value)
{
  assert(0<=i && i<(*m).nRows);
  assert(0<=j && j<(*m).nCols);
  int indx = i*(*m).nCols+j;
  (*m).data[indx]=value;
}

double matrixGet(matrix* m, int i, int j) 
{
  assert(0<=i && i<(*m).nRows);
  assert(0<=j && j<(*m).nCols);
  int indx = i*(*m).nCols+j;
  return (*m).data[indx];
}

#endif /* MATRIX_H */