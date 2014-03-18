#include <cmath>
#include <iostream>
#include <stdio.h>
#include "DFSBinarySearch.h"

using namespace std;

int * dfsArr;
int dfsArrSize;
int treeHeight;
int insertFrom;

void dfsInsert(int dfsIdx, int atDepth, int * inputArr);

DFSBinarySearch::DFSBinarySearch() {
}

DFSBinarySearch::~DFSBinarySearch() {
}

void DFSBinarySearch::createDataStructure(int * arr, int arrSize) 
{	
	dfsArrSize = arrSize;
	delete [] dfsArr;
	dfsArr = new int[arrSize];
	treeHeight = floor(log2(arrSize));

	// Algorithm
	insertFrom = 0;
	dfsInsert(0, 0, arr);	
}

void dfsInsert(int dfsIdx, int atDepth, int * inputArr) {
	if (dfsIdx < dfsArrSize && atDepth <= treeHeight) {
		int offset = floor(pow(2, (treeHeight - atDepth)));
		atDepth++;
		
		// Go left;
		dfsInsert(dfsIdx + 1, atDepth, inputArr);
		// Insert self	
		dfsArr[dfsIdx] = inputArr[insertFrom];
		insertFrom++;
		// Go right
		dfsInsert(dfsIdx + offset, atDepth, inputArr);
	}
}

int DFSBinarySearch::binSearch(int elem) 
{
	int maxVal = -1;
	int node = 0; 		// Start at root located at first pos 
	int val;
	int atDepth = 0;
	
	while (node >= 0 && node < dfsArrSize) {
		val = dfsArr[node];
			
		if ( elem == val ) {
			return val;
		
		} else if (atDepth == treeHeight) {
			if (val < elem) {
				return val;
			} else {  // Added by Lau. Before, if elem<val at the bottom of the tree, it would begin incrementing the node, making it a linear search!!
			  return maxVal;
			}
					
		} else if ( elem < val ) {
			// Go left
			node = node + 1;
	
		} else {
			// Go right
			//node = node + floor(pow(2, (treeHeight - atDepth)));
       		        node = node + (1<<(treeHeight-atDepth));
			maxVal = val;
		}	
		atDepth++;
	}
	
	return maxVal;
}

