#include <stdio.h>
#include <cstdlib> 
#include <cmath>
#include <queue>
#include <cstring>
#include <iostream>

#include "MSDRadix.h"


static const int K = 32;  // number if bits in each key (int).
static const int D = 4;  // number of bits to sort by each pass.
static const int N_PASSES = K / D;  // thus, D should divide K.
static const int N_BUCKETS = 1 << D;  // 2^D
static const int MASK_D = N_BUCKETS - 1;  // 2^D-1, which is D ones in binary.

static int* sortedArray;
static int sortArrIdx;

using std::cout;
using std::endl;
using std::queue;

static void msdRecursive(queue<int> bucket, int pass);

MSDRadix::MSDRadix() 
{
}

MSDRadix::~MSDRadix() 
{
}

void MSDRadix::setup(int arrSize)
{
	
}

int* MSDRadix::sort(int* array, int arrSize)
{
  delete [] sortedArray;
  // Copy array
  sortedArray = new int[arrSize];
  memcpy(sortedArray, array, sizeof(int[arrSize]));
  sortArrIdx = 0;

  // Pass 0
  queue<int> buckets[N_BUCKETS];
  for (int i = 0; i < arrSize; i++) {
    // Add to bucket
    buckets[ sortedArray[i] >> (K-D) ].push(sortedArray[i]);
  }

  // Pass 1 (and the rest, recursively)
  for (int b = 0; b < N_BUCKETS; b++) {
    msdRecursive(buckets[b], 1);
  }

  return sortedArray;
}

void msdRecursive(queue<int> bucket, int pass) {
  if (bucket.empty()) return;

  // Base case (this bucket is already fully sorted).
  if (pass == N_PASSES) {
    while(!bucket.empty()) {
      sortedArray[sortArrIdx] = bucket.front();
      bucket.pop();
      sortArrIdx++;
    }
    return;
  }

  // Sort this bucket into some new buckets.
  queue<int> buckets[N_BUCKETS];
  while(!bucket.empty()) {
    const int key = bucket.front();
    bucket.pop();
    // Add to bucket
    buckets[ (key >> (K - (pass+1)*D)) & MASK_D ].push(key);
  }
    // Example (K=8, D=2, pass=1, so we have already sorted by AB, and now we look at CD):
    // (ABCDEFGH >> (8 - (1+1)*2)) & 00000011
    // (ABCDEFGH >> 4) & 00000011
    //  0000ABCD & 00000011
    //  000000CD

  // Now, sort all buckets recursively.
  for (int b = 0; b < N_BUCKETS; b++) {
    msdRecursive(buckets[b], pass+1);
  }
}
