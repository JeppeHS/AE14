#include <iostream>
#include <conio.h>
#include <vector>

#include "QuickSort.h"

QuickSort::QuickSort(){}

QuickSort::~QuickSort(){}

void QuickSort::setup(int arrSize){}

int compare (const void * a, const void * b)
{
	return ( *(int*)a - *(int*)b );
}

int* QuickSort::sort(int* array, int arrSize)
{
	std::qsort(array, arrSize, sizeof(int), compare);
	return array;
}