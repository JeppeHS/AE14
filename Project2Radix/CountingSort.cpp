#include <iostream>
//#include <conio.h>
#include <vector>

#include "CountingSort.h"
using std::cout;
using std::endl;

int* result;
CountingSort::CountingSort(){}

CountingSort::~CountingSort(){
}

void CountingSort::setup(int arrSize){}
int* CountingSort::sort(int* array, int arrSize)
{
	delete [] result;
	//result = nullptr;
	int gKey = 0;
	int i, j;
	result = new int[arrSize];
	for ( int i = 0; i < arrSize; i++)
	{
		if(array[i] > gKey)
		{
		  gKey = array[i];
		}
	}
	gKey++;  // added one...
	int* Histogram = new int[gKey];
	for(i = 0; i < gKey; i++)
		Histogram[i] = 0;
	for(j =0; j <= arrSize-1; j++)
		Histogram[array[j]] = Histogram[array[j]] + 1;
	for(i = 1; i < gKey; i++)
		Histogram[i] = Histogram[i] + Histogram[i-1];
	for(j = arrSize-1; j >= 0; j--)
	{
		result[Histogram[array[j]]-1] = array[j];
		Histogram[array[j]] = Histogram[array[j]] - 1;
	}
	delete [] Histogram;
	Histogram = nullptr;
	return result;
}
