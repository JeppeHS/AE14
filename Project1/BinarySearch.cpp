#include <iostream>
#include "BinarySearch.h"

int size;
int linIndex;
int * binaryArray;
int * linArray;

using namespace std;


BinarySearch::BinarySearch() {
}

BinarySearch::~BinarySearch() {
	delete [] binaryArray;
}

void BinarySearch::createDataStructure(int * arr, int arrSize) 
{	
	binaryArray = new int[arrSize];


	size = arrSize;
    linArray = arr;

    //remove_this printing stuff
    {int i;
    for (i=0;i<size;i++) {
      cout << linArray[i] << "\t";
    }}
    cout << "\n";
    
    linIndex = 0;
    insert(0);



	for (int i = 0; i < arrSize; i++) {
		cout << "binaryArray i " << i << " , val " << binaryArray[i] << endl;
	}
	
	
}

int BinarySearch::binSearch(int elem) 
{	
	int i = 0;
    int curr = binaryArray[i]; // The current node visited
    int res = 0; // The latest element smaller than 'elem'.
    while (curr != elem){
     cout << "curr="<<curr<<", i="<<i<<"\n"; //remove_this
      i = 2*i+1; // Left child
      if (curr < elem){
	i++; // Right child (=left+1)
	res = curr; // This is now the latest known element smaller than 'elem'.
	   }

      // If we have reached a bottom node, return the last element lower than 'elem'.
      if (i >= size){
	cout << "i>=size: "<<i<<">="<<size<<"\n"; //remove_this
	return res;
      }

      curr = binaryArray[i];
    }

    // At this point, the while loop did not continue because curr==elem
    return curr;
}

void BinarySearch::insert(int binIndex)
{
	if (binIndex >= size) return;
    insert(2*binIndex+1); // Left child
    binaryArray[binIndex] = *(linArray+linIndex);
    linIndex++;
    insert(2*binIndex+2); // Right child
}