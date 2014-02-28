#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>
#include <cstdio>
#include <cassert>

typedef struct {int nRows, nCols; int* data;} matrix;
matrix* createMatrix(int nRows, int nCols);
void destroyMatrix(matrix* m);
<<<<<<< HEAD
void matrixPut(matrix* m, int i, int j, double value);
double matrixGet(matrix* m, int i, int j);
void matrixAdd(matrix* m, int i, int j, double value);
=======
void matrixPut(matrix* m, int i, int j, int value);
int matrixGet(matrix* m, int i, int j);
void matrixPrint(matrix* m);
>>>>>>> 5d2c49ce6398694763e7b0a2c7d239b6f0db4b97

#endif /* MATRIX_H */
