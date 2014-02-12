#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

#include "IBinSearch.h"
class BinarySearch : public BinSearchInterface
{
public:
	BinarySearch();
	~BinarySearch();
	void createDataStructure(int * arr, int arrSize);
	void insert(int index);
	int binSearch(int elem); 
};

#endif