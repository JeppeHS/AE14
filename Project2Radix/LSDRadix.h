
#ifndef LSDRADIX_H
#define LSDRADIX_H

#include "IRadixSort.h"

class LSDRadix : public IRadixSort
{
public:
	LSDRadix();
	~LSDRadix();
	void setup(int arrSize);
	int* sort(int* array, int arrSize);
};

#endif

