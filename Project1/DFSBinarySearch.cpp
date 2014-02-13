#include <cmath>
#include <iostream>
#include "DFSBinarySearch.h"

using namespace std;

int * dfsArr;
int dfsArrSize;
int treeHeight;
int dfsInsertAt;

void dfsInsert(int node, int * inputArr, int * outputArr, int atDepth);

DFSBinarySearch::DFSBinarySearch() {
}

DFSBinarySearch::~DFSBinarySearch() {
}

void DFSBinarySearch::createDataStructure(int * arr, int arrSize) 
{	
	dfsArrSize = arrSize;
	dfsArr = new int[arrSize];
	treeHeight = floor(log2(arrSize));
		
	// Algorithm
	int root = floor(arrSize/2);
	int depth = 0;
	dfsInsertAt = 0;
	dfsInsert(root, arr, dfsArr, depth);
	
	// TEST
	for (int i = 0; i < arrSize; i++) {
		cout << "dfsArr i " << i << " , val " << dfsArr[i] << endl;
	}
	
}

void dfsInsert(int node, int * inputArr, int * outputArr, int atDepth) {
	//Insert self
	dfsArr[dfsInsertAt] = inputArr[node];
	dfsInsertAt++;
	
	if (atDepth < treeHeight) {	
		int offset = floor(pow(2, (treeHeight - 1 - atDepth)));
		atDepth++;
		
		// Go left
		int lChild = node - offset;
		dfsInsert(lChild, inputArr, outputArr, atDepth);
		
		// Go right
		int rChild = node + offset;
		dfsInsert(rChild, inputArr, outputArr, atDepth);
	} 
}

int DFSBinarySearch::binSearch(int elem) 
{
	int max = dfsArr[0];
	int node = 0; 		// Start at root located at first pos 
	int val;
	int atDepth = 0;
	
	while (node < dfsArrSize && atDepth <= treeHeight) {
		val = dfsArr[node];
		
		if ( elem == val ) {
			return val;
		
		} else if ( elem < val ) {
			// Go left
			node = node + 1;
	
		} else {
			// Go right
			node = node + floor(pow(2, (treeHeight - atDepth)));
			max = val;
		}	
		atDepth++;
	}
	
	return max;
}

