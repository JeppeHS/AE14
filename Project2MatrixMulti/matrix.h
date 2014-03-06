#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>
#include <cstdio>
#include <cassert>

typedef struct {int nRows, nCols; int* data;} matrix;
matrix* createMatrix(int nRows, int nCols);
void destroyMatrix(matrix* m);
void matrixPut(matrix* m, int i, int j, int value);
void matrixAdd(matrix* m, int i, int j, int value);
int matrixGet(matrix* m, int i, int j);
void matrixPrint(matrix* m);
bool matrixEquals(matrix* m1, matrix* m2);

#endif /* MATRIX_H */
