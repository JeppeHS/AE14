#ifndef LinearSearch_H
#define LinearSearch_H

#include "BinSearchInterface.h"
class LinearSearch : public BinSearchInterface
{
public:
	LinearSearch();
	~LinearSearch();
	void createDataStructure(int * arr, int arrSize);
	int binSearch(int elem); 
};

#endif