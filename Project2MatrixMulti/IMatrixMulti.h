#ifndef IMATRIXMULTI_H
#define IMATRIXMULTI_H

#include "matrix.h"

class IMatrixMulti
{

public:
  virtual void setup(matrix* A, int height, int width) = 0;
  virtual matrix* matrixMultiplication(matrix* B) = 0;
};

#endif
