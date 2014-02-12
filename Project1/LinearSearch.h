#ifndef LinearSearch_H
#define LinearSearch_H

#include "ILinearSearch.h"
class LinearSearch : public ILinearSearch
{
public:
	LinearSearch();
	~LinearSearch();
	void createDataStructure(int * arr, int arrSize);
	int linSearch(int elem); 
};

#endif