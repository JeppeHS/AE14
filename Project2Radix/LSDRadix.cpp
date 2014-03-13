#include <stdio.h>
#include <cstdlib> 
#include <cmath>
#include <queue>
#include <cstring>

#include "LSDRadix.h"

//#define SORT_BY_N_DIGITS 3
//#define NUM_BASE 10
static const int K = 32;  // number if bits in each key (int).
static const int D = 8;  // number of bits to sort by each pass.
static const int N_PASSES = K / D;  // thus, D should divide K.
static const int N_BUCKETS = 1 << D;  // 2^D
static const int MASK_D = N_BUCKETS - 1;  // 2^D-1, which is D ones in binary.

using namespace std;


queue<int> buckets[N_BUCKETS];

LSDRadix::LSDRadix() 
{
}

LSDRadix::~LSDRadix() 
{
}

void LSDRadix::setup(int arrSize)
{
}

int* LSDRadix::sort(int* array, int arrSize)
{
	// Copy array
	int* sortedArray = new int[arrSize];
	memcpy(sortedArray, array, sizeof(int[arrSize]));

	// Sort by number of lowest signifigent digits
	for (int i = 0; i < N_PASSES; i++) {

	  //int powI = pow(10, i);
		for (int j = 0; j < arrSize; j++) {
			// Add to bucket
			// sortedArray[j] / powI gets the i'th digit from the right
			//buckets[ ( sortedArray[j] / powI ) % NUM_BASE].push(sortedArray[j]);
		  buckets[ (sortedArray[j] >> (K-i*D)) & MASK_D ].push(sortedArray[j]);
		}	

		// Create sorted array from buckets
		int sortArrIdx = 0;
		for (int k = 0; k < N_BUCKETS; k++) {
			while(!buckets[k].empty()) {
				sortedArray[sortArrIdx] = buckets[k].front();
				buckets[k].pop();
				sortArrIdx++;
			}
		}
	}

	return sortedArray;
}
