#include <iostream>
//#include <conio.h>
#include <vector>

#include "CountingSort.h"
using std::cout;
using std::endl;

CountingSort::CountingSort(){}

CountingSort::~CountingSort(){}

void CountingSort::setup(int arrSize){}
int* CountingSort::sort(int* array, int arrSize)
{
	int gKey = 0;
    int i, j;
    //cout << "MARK start" << endl;
	int* result = new int[arrSize];
	//cout << "MARK 0" << endl;
	for ( int i = 0; i < arrSize; i++)
    {
		 if(array[i] > gKey)
         {
            gKey = array[i];
         }
    }
	//cout << "MARK 1" << endl;
	gKey++;  // added one...
	int* C = new int[gKey];
	for(i = 0; i < gKey; i++)
            C[i] = 0;
	//cout << "MARK 2" << endl;
	for(j =0; j <= arrSize-1; j++)
            C[array[j]] = C[array[j]] + 1;
	//cout << "MARK 3" << endl;
	for(i = 1; i < gKey; i++)
            C[i] = C[i] + C[i-1];
	//cout << "MARK 4" << endl;
    for(j = arrSize-1; j >= 0; j--)
    {
      //      cout << "C[array[" << j << "]] = " << C[array[j]] << endl;
            result[C[array[j]]-1] = array[j];
            C[array[j]] = C[array[j]] - 1;
    }
    //cout << "MARK 5" << endl;
	/* Print
    std::cout << "\nThe Sorted array is : ";
	for(i = 1; i <= arrSize; i++)
            std::cout << result[i] << "  " ;
			*/
    delete [] C;
	return result;
}
