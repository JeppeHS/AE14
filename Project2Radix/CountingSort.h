#ifndef COUNTINGSORT_H
#define COUNTINGSORT_H

#include "IRadixSort.h"

class CountingSort : public IRadixSort
{
public:
	CountingSort();
	~CountingSort();
	void setup();
	int* sort(int* array, int arrSize);
};

#endif