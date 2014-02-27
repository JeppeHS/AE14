#include "BinSearchInterface.h"

class vEBBinarySearch : public BinSearchInterface
{
  
 public:
  void createDataStructure(int *arr, int arrSize);
  int binSearch(int elem);
  char const* getLabel(){return "vEB";};
};
