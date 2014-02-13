#include <iostream>
#include "LinearSearch.h"

int * linearArray;
int linSize;

using namespace std;

LinearSearch::LinearSearch() {
}

LinearSearch::~LinearSearch() {
	//delete [] linearArray;
}

void LinearSearch::createDataStructure(int * arr, int arrSize) 
{	
	linSize = arrSize;
	linearArray = arr;
}

int LinearSearch::linSearch(int elem) 
{	
	int current  = 0;
	int curr = linearArray[current]; // The current node visited
    while (curr != elem){
      if (curr > elem && current != 0){
		  return linearArray[current-1];
	   }
	  else if(curr < elem){
		  current++;
	  }
	  else if(current == 0){
		  int result = linearArray[current];
		  return result;
	  }

      // If we have reached a bottom node, return the last element lower than 'elem'.
      if (current >= linSize){
	cout << "i>=size: "<<current<<">="<<linSize<<"\n"; //remove_this
	return linearArray[current];
      }

      curr = linearArray[current];
    }

    // At this point, the while loop did not continue because curr==elem
    return curr;
}
