
#ifndef DFSBINARYSEARCH_H
#define DFSBINARYSEARCH_H

#include "BinSearchInterface.h"

class DFSBinarySearch : public BinSearchInterface
{
public:
	DFSBinarySearch();
	~DFSBinarySearch();
	void createDataStructure(int * arr, int arrSize);
	int binSearch(int elem); 
};

#endif

