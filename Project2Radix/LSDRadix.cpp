#include <stdio.h>
#include <cstdlib> 
#include <cmath>
#include <queue>
#include <cstring>

#include "LSDRadix.h"

#define SORT_BY_N_DIGITS 3
#define NUM_BASE 10

using namespace std;


queue<int> buckets[NUM_BASE];


LSDRadix::LSDRadix() 
{
}

LSDRadix::~LSDRadix() 
{
}

void LSDRadix::setup()
{
	
}

int* LSDRadix::sort(int* array, int arrSize)
{
	// Copy array
	int* sortedArray = new int[arrSize];
	memcpy(sortedArray, array, sizeof(int[7]));

	// Sort by number of lowest signifigent digits
	for (int i = 0; i < SORT_BY_N_DIGITS; i++) {

		for (int j = 0; j < arrSize; j++) {
			// Add to bucket
			// sortedArray[j] / (int) pow(10, i) gets the i'th digit from the right
			buckets[ ( sortedArray[j] / (int) pow(10, i) ) % NUM_BASE].push(sortedArray[j]);
		}	

		// Create sorted array from buckets
		int sortArrIdx = 0;
		for (int k = 0; k < NUM_BASE; k++) {
			while(!buckets[k].empty()) {
				sortedArray[sortArrIdx] = buckets[k].front();
				buckets[k].pop();
				sortArrIdx++;
			}
		}
	}

	return sortedArray;
}
