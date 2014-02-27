
#include "matrix.h"

matrix* createMatrix(int nRows, int nCols)
{
  matrix* m = (matrix*)malloc(sizeof(matrix));
	if( m==NULL ) {
		fprintf(stderr,"error in my_matrix_alloc\n");
		return NULL;
	}
  (*m).nRows = nRows;
  (*m).nCols = nCols;
  (*m).data = (int*) malloc(nRows*nCols*sizeof(int));
  return m;
}

void destroyMatrix(matrix* m) { 
	free(m -> data); 
	free(m); 
}

void matrixPut(matrix* m, int i, int j, int value)
{
  assert(0<=i && i<(*m).nRows);
  assert(0<=j && j<(*m).nCols);
  int indx = i*(*m).nCols+j;
  (*m).data[indx]=value;
}

int matrixGet(matrix* m, int i, int j) 
{
  assert(0<=i && i<(*m).nRows);
  assert(0<=j && j<(*m).nCols);
  int indx = i*(*m).nCols+j;
  return (*m).data[indx];
}

void matrixPrint(matrix* m) {
  printf("Matrix:\n");  
  for (int i = 0; i < m->nCols; i++) {
    printf("[");
    for (int j = 0; j < m->nRows; j++) {
      printf("%d | ", matrixGet(m, i, j));
    }
    printf("]\n");
  }
  printf("\n");
}
