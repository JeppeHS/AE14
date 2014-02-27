
#ifndef IMATRIXMULTI_H
#define IMATRIXMULTI_H

class IMatrixMulti
{
public:
  virtual void setup(int* A, int height, int width) = 0;
  virtual int * matrixMultiplication(int* B) = 0;
};

#endif
