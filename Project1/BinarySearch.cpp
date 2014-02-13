#include <iostream>
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
	binaryArray = arr;
	

	size = arrSize;
	
}

int BinarySearch::binSearch(int elem) 
{
	
	int low = 0;
	int high = size-1;
	int current  = (high + low)/2;
    int curr = binaryArray[current]; // The current node visited
	int smallestSoFar = current;
    while (curr != elem){
		current = (high + low)/2;
      if (curr > elem && high > low){
		high = current -1;
		curr = binaryArray[current];
		smallestSoFar = current;
	   }
	  else if(curr < elem && high != low){
		  curr = binaryArray[current];
		  smallestSoFar = current;
		  low = current +1;
	  }
	  else if(high == low && binaryArray[current] != elem){
		  if(binaryArray[size-1] < elem){
			  return binaryArray[size-1];
		  }
		  if(binaryArray[0] > elem){
			  return binaryArray[0];
		  }
		  int result = binaryArray[smallestSoFar];
		  return result;
	  }

      // If we have reached a bottom node, return the last element lower than 'elem'.
      if (high >= size){
	cout << "i>=size: "<<high<<">="<<size<<"\n"; //remove_this
	return binaryArray[current];
      }

      curr = binaryArray[current];
    }

    // At this point, the while loop did not continue because curr==elem
    return curr;
}