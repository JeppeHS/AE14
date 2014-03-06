#include <cmath>
#include <iostream>
#include <stdio.h>
#include "RecMulti.h"
#include "matrix.h"

using namespace std;

int heightA, widthA;
matrix* A;
matrix* B;
matrix* C;

void recMult(int i0_A, int j0_A, int i0_B, int j0_B, int m, int n, int p);

RecMulti::RecMulti() 
{
}

RecMulti::~RecMulti() 
{
}

void RecMulti::setup(matrix* matrixA, int height, int width) 
{
  A = matrixA;
  heightA = height;
  widthA = width;
}

matrix * RecMulti::matrixMultiplication(matrix* matrixB)
{
  int m = heightA;
  int n = widthA;
  int p = (*matrixB).nCols;
  B = matrixB;
  
  C = createMatrix(m, p);
  
  recMult(0, 0, 0, 0, m, n, p);
  
  return C;
}

void recMult(int i_A, int j_A, int i_B, int j_B, int m, int n, int p) {
  if (m==1 && n==1 && p==1) { // base case
    int AB = matrixGet(A, i_A, j_A)*matrixGet(B, i_B, j_B);
    matrixAdd(C, i_A, j_B, AB);
  } else if (m >= max(n,p)) { // split rows of A
    recMult(i_A,     j_A,     i_B,     j_B,     m/2,   n,     p    );
    recMult(i_A+m/2, j_A,     i_B,     j_B,     m-m/2, n,     p    );
  } else if (n >= max(m,p)) { // split columns of A and rows of B
    recMult(i_A,     j_A,     i_B,     j_B,     m,     n/2,   p    );
    recMult(i_A,     j_A+n/2, i_B+n/2, j_B,     m,     n-n/2, p    );
  } else if (p >= max(m,n)) { // split columns of B
    recMult(i_A,     j_A,     i_B,     j_B,     m,     n,     p/2  );
    recMult(i_A,     j_A,     i_B,     j_B+p/2, m,     n,     p-p/2);
  }
  
  return;
}
