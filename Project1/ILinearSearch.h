#ifndef ILinearSearch_H
#define ILinearSearch_H

class ILinearSearch
{
public:
  virtual void createDataStructure(int* arr, int arrSize) = 0;
  virtual int linSearch(int elem) = 0;
};

#endif