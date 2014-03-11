#include <iostream>
#include <conio.h>
#include <vector>

#include "CountingSort.h"

CountingSort::CountingSort(){}

CountingSort::~CountingSort(){}

void CountingSort::setup(){}
int* CountingSort::sort(int* array, int arrSize)
{
	int gKey = 0;
    int i, j;
	int* result = new int[arrSize];
	for ( int i = 0; i < arrSize; i++)
    {
		 if(array[i] > gKey)
         {
            gKey = array[i];
         }
    }
	int* C = new int[gKey];
	for(i = 0; i <= gKey; i++)
            C[i] = 0;
	for(j =0; j <= arrSize-1; j++)
            C[array[j]] = C[array[j]] + 1;
	for(i = 1; i <= gKey; i++)
            C[i] = C[i] + C[i-1];
    for(j = arrSize-1; j >= 0; j--)
    {
            result[C[array[j]]] = array[j];
            C[array[j]] = C[array[j]] - 1;
    }
	/* Print
    std::cout << "\nThe Sorted array is : ";
	for(i = 1; i <= arrSize; i++)
            std::cout << result[i] << "  " ;
			*/
	return result;
}