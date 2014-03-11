#include <stdio.h>
#include <cstdlib> 
#include <cmath>
#include <queue>
#include <cstring>
#include <iostream>

#include "MSDRadix_countSort.h"


static const int K = 32;  // number if bits in each key (int).
static const int D = 8;  // number of bits to sort by each pass.
static const int N_PASSES = K / D;  // thus, D should divide K.
static const int N_BUCKETS = 1 << D;  // 2^D
static const int MASK_D = N_BUCKETS - 1;  // 2^D-1, which is D ones in binary.

static int N_KEYS;
static int* sortedArray;
static int sortArrIdx;
static int* storage_pointers[N_PASSES];
static int* next_pointers[N_PASSES];

using std::cout;
using std::endl;
using std::queue;

static void msdRecursiveCS(int pass, int startIdx, int bucketSize);

MSDRadix_countSort::MSDRadix_countSort() 
{
}

MSDRadix_countSort::~MSDRadix_countSort() 
{
}

void MSDRadix_countSort::setup(int arrSize)  // TODO...
{
  for (int i=0; i < N_PASSES; i++) {
    delete [] storage_pointers[i];
    delete [] next_pointers[i];
  }
  N_KEYS = arrSize;

  //delete [] storage_pointers;
  //delete [] next_pointers;
  //storage_pointers = new int[N_PASSES];
  //next_pointers = new int [N_PASSES];
  for (int i=0; i < N_PASSES; i++) {
    int* storage_tmp = new int[N_KEYS*N_BUCKETS];
    storage_pointers[i] = storage_tmp;
    next_pointers[i] = new int[N_BUCKETS];
  }
}

int* MSDRadix_countSort::sort(int* array, int arrSize)
{
  delete [] sortedArray;
  // Copy array
  sortedArray = new int[arrSize];
  memcpy(sortedArray, array, sizeof(int[arrSize]));
  sortArrIdx = 0;
  //cout << "MARK 1" << endl;
  int* storage = storage_pointers[0];
  int* next = next_pointers[0];
  for (int i=0; i < N_BUCKETS; i++) {
    next[i] = i*N_KEYS;
  }
  //cout << "MARK 2" << endl;
  // Pass 0
  //queue<int> buckets[N_BUCKETS];
  for (int i = 0; i < N_KEYS; i++) {
    // Add to bucket
    //buckets[ sortedArray[i] >> (K-D) ].push(sortedArray[i]);
    const int value = sortedArray[i];
    const int key = value >> (K-D);
    storage[next[key]] = value;
    next[key]++;
  }
  //cout << "MARK 3" << endl;
  // Pass 1 (and the rest, recursively)
  for (int b = 0; b < N_BUCKETS; b++) {
    int startIdx = b*N_KEYS;
    int bucketSize = next[b] - startIdx;
    //cout << "Recursion, bucket " << b << ", startIdx = " << startIdx << ", bucketSize = " << bucketSize << endl;
    msdRecursiveCS(1, startIdx, bucketSize);
  }

  return sortedArray;
}

void msdRecursiveCS(int pass, int startIdx, int bucketSize) {
  //cout << "pass = " << pass << endl;

  if (bucketSize == 0) return;

  const int* storageOld = storage_pointers[pass-1];

  // Base case (this bucket is already fully sorted).
  if (pass == N_PASSES) {
    for (int i=startIdx; i<(startIdx+bucketSize); i++) {
      sortedArray[sortArrIdx] = storageOld[i];
      sortArrIdx++;
    }
    return;
  }


  // Sort this bucket into some new buckets.
  int* storageNew = storage_pointers[pass];
  int* nextNew = next_pointers[pass];

  for (int i=0; i < N_BUCKETS; i++) {
    nextNew[i] = i*N_KEYS;
  }

  for (int i = startIdx; i < (startIdx+bucketSize); i++) {
    //cout << "i = " << i << ", startIdx = " << startIdx << ", bucketSize = " << bucketSize << endl;
    // Add to bucket
    const int value = storageOld[i];
    const int key = (value >> (K - (pass+1)*D)) & MASK_D;
    storageNew[nextNew[key]] = value;
    nextNew[key]++;
  }  
    // Example (K=8, D=2, pass=1, so we have already sorted by AB, and now we look at CD):
    // (ABCDEFGH >> (8 - (1+1)*2)) & 00000011
    // (ABCDEFGH >> 4) & 00000011
    //  0000ABCD & 00000011
    //  000000CD


  // The rest, recursively
  for (int b = 0; b < N_BUCKETS; b++) {
    int startIdxNew = b*N_KEYS;
    int bucketSizeNew = nextNew[b] - startIdxNew;
    msdRecursiveCS(pass+1, startIdxNew, bucketSizeNew);
  }
}
