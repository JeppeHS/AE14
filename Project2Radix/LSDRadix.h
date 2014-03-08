
#ifndef LSDRADIX_H
#define LSDRADIX_H

#include "IRadixSort.h"

class LSDRadix : public IRadixSort
{
public:
	LSDRadix();
	~LSDRadix();
	void setup();
	int* sort(int* array, int arrSize);
};

#endif

