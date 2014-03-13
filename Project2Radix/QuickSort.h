#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "IRadixSort.h"

class QuickSort : public IRadixSort
{
public:
	QuickSort();
	~QuickSort();
	void setup(int arrSize);
	int* sort(int* array, int arrSize);
};

#endif