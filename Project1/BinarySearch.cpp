#include <iostream>
#include <stdio.h>
#include "BinarySearch.h"

int size;
int * binaryArray;

using namespace std;


BinarySearch::BinarySearch() {
}

BinarySearch::~BinarySearch() {
	//delete [] binaryArray;
}

void BinarySearch::createDataStructure(int * arr, int arrSize) 
{	
  	size = arrSize;	
  //	binaryArray = arr;
  delete [] binaryArray;
  binaryArray = new int[size];
  for (int i=0; i<size; i++) {
    binaryArray[i] = arr[i];
  }
}

int BinarySearch::binSearch(int elem) 
{
	
	int low = 0;
	int high = size-1;
	int smallestSoFar = -1;
	
  while (high >= low)
    {
      // calculate the midpoint for roughly equal partition
      int imid = (high + low)/2;
	  int midVal = binaryArray[imid];
      if(midVal == elem) {
		return midVal; 
	  } else if (midVal < elem) {
        low = imid + 1;
        smallestSoFar = midVal;
	  } else {
        high = imid - 1;
	  }
    }
	
	return smallestSoFar;
}
