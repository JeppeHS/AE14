#include <iostream>
#include "BFSBinarySearch.h"
using std::cout;

int main(){
  const int N = 9;
  int linArray[N] = {1,2,4,5,6,7,8,9,10};
  BFSBinarySearch bfsBinSearch;
  bfsBinSearch.createDataStructure(linArray, N);
  int elem = 3;
  int res =  bfsBinSearch.binSearch(elem);
  cout << "Pred("<<elem<<") = "<<res<<"\n";
  return 0;
}
