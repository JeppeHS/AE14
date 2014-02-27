
#ifndef IMATRIXMULTI_H
#define IMATRIXMULTI_H

typedef struct { int nRows, nCols; double* data;} matrix;

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
  int indx = i*(*m).nCols+j;
  (*m).data[indx]=value;
}

double matrixGet(matrix* m, int i, int j) 
{
  int indx = i*(*m).nCols+j;
  return (*m).data[indx];
}

class IMatrixMulti
{
public:
  virtual void setup(matrix* A, int height, int width) = 0;
  virtual matrix* matrixMultiplication(matrix* B) = 0;
};

#endif
