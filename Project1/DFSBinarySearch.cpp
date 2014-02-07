
#include <iostream>
#include "DFSBinarySearch.h"

using namespace std;

int * dfsArr;

DFSBinarySearch::DFSBinarySearch() {
}

DFSBinarySearch::~DFSBinarySearch() {
	delete [] dfsArr;
}

void DFSBinarySearch::createDataStructure(int * arr, int arrSize) 
{	
	dfsArr = new int[arrSize];






	for (int i = 0; i < arrSize; i++) {
		cout << "dfsArr i " << i << " , val " << dfsArr[i] << endl;
	}
	
	
}

int DFSBinarySearch::binSearch(int elem) 
{
	
	
	
	return 1;
}
