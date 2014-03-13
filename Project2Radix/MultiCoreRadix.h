
#ifndef MULTICORERADIX_H
#define MULTICORERADIX_H

#include "IRadixSort.h"

class MultiCoreRadix : public IRadixSort
{
public:
	MultiCoreRadix();
	~MultiCoreRadix();
	void setup(int arrSize);
	int* sort(int* array, int arrSize);
};

#endif

