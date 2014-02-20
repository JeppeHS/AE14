#include <iostream>
#include "BinSearchInterface.h"
using std::cout;

class BFSBinarySearch : public BinSearchInterface
{
private:
  int linIndex;
  int size;
  int* bfsArray;
  int* linArray;

  // Recursive method for filling out the bfs tree.
  void insert(int bfsIndex){
    if (bfsIndex >= size) return;
    insert(2*bfsIndex+1); // Left child
    bfsArray[bfsIndex] = *(linArray+linIndex);
    linIndex++;
    insert(2*bfsIndex+2); // Right child
  }

public:
  // Create the BFS structure (eg.): {1,2,3,4,5,6,7} --> {4, 2,6, 1,3,5,7}
  void createDataStructure(int* arr, int arrSize){
    size = arrSize;
    bfsArray = new int[size];
    linArray = arr;

    //remove_this printing stuff
    //    {int i;
    //for (i=0;i<size;i++) {
    // cout << linArray[i] << "\t";
    //}}
    //cout << "\n";
    
    linIndex = 0;
    insert(0);
    
    //remove_this printing stuff
    //int i;
    //for (i=0;i<size;i++) {
    //  cout << bfsArray[i] << "\t";
    //}
    //cout << "\n";
  };

  // Binary search in a BFS array. 
  int binSearch(int elem){
    int i = 0;
    int curr = bfsArray[i]; // The current node visited
    int res = -1; // The latest element smaller than 'elem'.
    while (curr != elem){
      //cout << "curr="<<curr<<", i="<<i<<"\n"; //remove_this
      i = 2*i+1; // Left child
      if (curr < elem){
	i++; // Right child (=left+1)
	res = curr; // This is now the latest known element smaller than 'elem'.
      }

      // If we have reached a bottom node, return the last element lower than 'elem'.
      if (i >= size){
	//cout << "i>=size: "<<i<<">="<<size<<"\n"; //remove_this
	return res;
      }

      curr = bfsArray[i];
    }

    // At this point, the while loop did not continue because curr==elem
    return curr;
  }
};
