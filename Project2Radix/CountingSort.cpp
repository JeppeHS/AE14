#include <iostream>
#include <conio.h>
#include <vector>

#include "CountingSort.h"

int* result;
CountingSort::CountingSort(){}

CountingSort::~CountingSort(){
}

void CountingSort::setup(int arrSize){}
int* CountingSort::sort(int* array, int arrSize)
{
	delete [] result;
	result = nullptr;
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
	gKey++;
	int* C = new int[gKey];
	for(i = 0; i < gKey; i++)
            C[i] = 0;
	for(j =0; j < arrSize; j++)
            C[array[j]] = C[array[j]] + 1;
	for(i = 1; i < gKey; i++)
            C[i] = C[i] + C[i-1];
    for(j = arrSize-1; j >= 0; j--)
    {
		result[C[array[j]]-1] = array[j];
		C[array[j]] = C[array[j]] - 1;
    }
    std::cout << "\nThe Sorted array is : ";
	for(i = 0; i < arrSize; i++)
            std::cout << result[i] << "  " ;
	delete [] C;
	C = nullptr;
	return result;
}