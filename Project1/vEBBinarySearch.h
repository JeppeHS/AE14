#include <iostream>
#include "BinSearchInterface.h"
#include <math.h>
using std::cout;
using std::endl;

class vEBBinarySearch : public BinSearchInterface
{
private:
  int linIndex;
  int size;
  int depth;
  int* vebArray;
  int* linArray;

  // Recursive method for filling out the veb tree.
  void insert(int vebIndex, int atDepth, int depthIndex, int roots[]){
    if (vebIndex >= size) {
      //cout << "vInd = " << vebIndex << ", returning..." << endl;
      return;
    }
    int rootIndex = msb((~atDepth+1) & atDepth); // findRightmostOne(atDepth)
    int depthType = msb(~atDepth & (atDepth+1)); // findRightmostZero(atDepth)
    /*
    cout << "vInd = " << (vebIndex) << "\t";
    cout << "atDep  = " << atDepth << "\t";
    cout << "depInd = " << depthIndex << "\t";
    cout << "dType = " << depthType << "\t";
    cout << "rInd = " << rootIndex << "\t";
    cout << "root = " << (roots[depthType]) << "\t";
    cout << endl;
    */
    
    int rootsNew[depth];
    {for (int i=0; i<depth; i++){
	if (i <= rootIndex) rootsNew[i] = vebIndex;
	else rootsNew[i] = roots[i];
	//	cout << rootsNew[i] << "\t";
      }}
    //cout << endl;

    //int depthTypeNext = msb(~(atDepth+1) & (atDepth+2));
    //cout << "Next root: " << rootsNew[depthTypeNext] << endl;
    //cout << "First factor = " << (2*(depthIndex % (int) pow(2, pow(2,depthType)-1 )) +1) << endl;
    //cout << "Second factor = " <<  (pow(2, pow(2, depthType)) -1) << endl;

    int vebLeft = (2*(depthIndex % (int) pow(2, pow(2,depthType)-1 )) +1) * (pow(2, pow(2, depthType)) -1) + rootsNew[depthType];

    insert(vebLeft, atDepth+1, 2*depthIndex, rootsNew); // Left child
    
    vebArray[vebIndex] = *(linArray+linIndex);
    linIndex++;

    int vebRight = vebLeft + pow(2, pow(2, depthType)) -1;
    insert(vebRight, atDepth+1, 2*depthIndex+1, rootsNew); // Right child
  }

  int msb(int v){
    int r = 0;
    while (v >>= 1) {
      r++;
    }
    return r;
  }



public:
  // Create the vEB structure (eg.): {1, 2,3, 4,5,6, 7,8,9, 10,11,12, 13,14,15}
  //                             --> {8, 4,12, 2,1,3, 6,5,7, 10,9,11, 14,13,15}
  void createDataStructure(int* arr, int arrSize){
    size = arrSize;
    vebArray = new int[size];
    linArray = arr;

    //remove_this printing stuff
    /*    {int i;
    for (i=0;i<size;i++) {
     cout << linArray[i] << "\t";
    }}
    cout << "\n";
    */

    linIndex = 0;
    depth = msb(size); // findLeftmostOne(findLeftmostOne(size))
    //cout << depth << endl;
    int roots[depth];
    {for (int i=0; i<depth; i++){
	roots[i] = 0;
      }}
    insert(0, 0, 0, roots);
    
    //remove_this printing stuff
    /*int i;
    for (i=0;i<size;i++) {
      cout << vebArray[i] << "\t";
    }
    cout << "\n";
    */
  };



  // Binary search in a vEB array. ************************************************
  int binSearch(int elem){
    int vebIndex = 0;
    int curr = vebArray[vebIndex]; // The current node visited
    int res = 0; // The latest element smaller than 'elem'.
    int atDepth = 0;
    int depthIndex = 0;

    int roots[depth];
    {for (int i=0; i<depth; i++){
	roots[i] = 0;
      }}
    
    while (curr != elem){
      //cout << "curr="<<curr<<", i="<<i<<"\n"; //remove_this

      int rootIndex = msb((~atDepth+1) & atDepth); // findRightmostOne(atDepth)
      int depthType = msb(~atDepth & (atDepth+1)); // findRightmostZero(atDepth)
    
      {for (int i=0; i<=rootIndex; i++){
	  roots[i] = vebIndex;
	}}
      
      // Determine if we should go to left or right child.
      int goRight = curr < elem;

      vebIndex = (2*(depthIndex % (int) pow(2, pow(2,depthType)-1 )) +1 + goRight) * (pow(2, pow(2, depthType)) -1) + roots[depthType];

      if (goRight){
	res = curr; // This is now the latest known element smaller than 'elem'.
      }

      // If we have reached a bottom node, return the last element lower than 'elem'.
      if (vebIndex >= size){
	//cout << "i>=size: "<<i<<">="<<size<<"\n"; //remove_this
	return res;
      }
      
      atDepth++;
      depthIndex = 2*depthIndex + goRight;
      curr = vebArray[vebIndex];
    }

    // At this point, the while loop did not continue because curr==elem
    return curr;
  }
};
