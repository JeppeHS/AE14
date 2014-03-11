
#ifndef MSDRADIXCOUNT_H
#define MSDRADIXCOUNT_H

#include "IRadixSort.h"

class MSDRadix_countSort : public IRadixSort
{
public:
	MSDRadix_countSort();
	~MSDRadix_countSort();
	void setup(int arrSize);
	int* sort(int* array, int arrSize);
};

#endif

