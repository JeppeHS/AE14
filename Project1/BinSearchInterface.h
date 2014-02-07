
#ifndef BINSEARCHINTERFACE_H
#define BINSEARCHINTERFACE_H

class BinSearchInterface
{
public:
  virtual void createDataStructure(int* arr, int arrSize) = 0;
  virtual int binSearch(int elem) = 0;
};

#endif
