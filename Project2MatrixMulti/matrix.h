#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>
#include <cstdio>
#include <cassert>

typedef struct {int nRows, nCols; double* data;} matrix;
matrix* createMatrix(int nRows, int nCols);
void destroyMatrix(matrix* m);
void matrixPut(matrix* m, int i, int j, double value);
double matrixGet(matrix* m, int i, int j);
void matrixAdd(matrix* m, int i, int j, double value);

#endif /* MATRIX_H */
