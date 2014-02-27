#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

#include "BinSearchInterface.h"
class BinarySearch : public BinSearchInterface
{
public:
	BinarySearch();
	~BinarySearch();
	void createDataStructure(int * arr, int arrSize);
	int binSearch(int elem); 
	char const* getLabel(){return "Inorder";};
};

#endif
