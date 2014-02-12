#include <iostream>
#include "LinearSearch.h"

int * linearArray;


using namespace std;

LinearSearch::LinearSearch() {
}

LinearSearch::~LinearSearch() {
	delete [] linearArray;
}

void LinearSearch::createDataStructure(int * arr, int arrSize) 
{	
	linearArray = new int[arrSize];






	for (int i = 0; i < arrSize; i++) {
		cout << "linearArray i " << i << " , val " << linearArray[i] << endl;
	}
	
	
}

int LinearSearch::linSearch(int elem) 
{	
	return 1;//?
}
