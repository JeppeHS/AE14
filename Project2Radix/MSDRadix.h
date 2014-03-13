
#ifndef MSDRADIX_H
#define MSDRADIX_H

#include "IRadixSort.h"

class MSDRadix : public IRadixSort
{
public:
	MSDRadix();
	~MSDRadix();
	void setup(int arrSize);
	int* sort(int* array, int arrSize);
};

#endif

