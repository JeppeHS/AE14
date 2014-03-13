#include <pthread.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>

#include "MultiCoreRadix.h"

#define NUM_OF_THREADS 2
#define PARAM_K 32
#define PARAM_D 8

using namespace std;

struct threadInfo1 {  
	int threadNum;       
    int startIdx;
    int endIdx;
};

struct threadInfo2 {  
	int threadNum;
	int msdBucketStart;
	int msdBucketEnd;
};

void parallelMSDCountingSort();
void serialComputeMSDOutputIndices();
void parallelLSDSort();
static void *parallelTask1(void *arg);
static void *parallelTask2(void *arg);

// DEBUG methods
void printThreadBucket(int threadNum, int bucketNum);
void printLocalBucketArray(int* bucketArr, int* nextArr, int numBuckets, int nElements);
void printBinary(int n);
void printArray2(int* array, int arrSize); 
// DEBUG methods - END

int msdShiftAmount = PARAM_K - PARAM_D;
int nBuckets = pow(2, PARAM_D);
pthread_t *threads = new pthread_t[NUM_OF_THREADS];
threadInfo1 *tInfos1 = new threadInfo1[NUM_OF_THREADS];
threadInfo2 *tInfos2 = new threadInfo2[NUM_OF_THREADS];
int **threadBucketArrays = new int*[NUM_OF_THREADS];
int **threadNextArrays = new int*[NUM_OF_THREADS]();
int *inputArray;
int *sortedArray;
int *bucketSizes = new int[nBuckets];
int *ouputMSDStartIndices = new int[nBuckets];
int *nElemsAssigned = new int[NUM_OF_THREADS]; 
int lsdBitMask = (1 << PARAM_D) - 1;
int arraySize;

MultiCoreRadix::MultiCoreRadix() 
{
}

MultiCoreRadix::~MultiCoreRadix() 
{
}

void MultiCoreRadix::setup(int arrSize)
{
	arraySize = arrSize;
}

int* MultiCoreRadix::sort(int* array, int arrSize)
{
	inputArray = array;
	sortedArray = new int[arrSize];

	// Part 1 - Parallel: MSD counting sort 
	parallelMSDCountingSort();
	//printf("Part 1 done\n");

	// Part 2 - Serial: Compute prefix sum
	serialComputeMSDOutputIndices();
	//printf("Part 2 done\n");

	// Part 3 - Parallel: The rest, LSD
	parallelLSDSort();
	//printf("Part 3 done\n");

	return sortedArray;
}

void parallelMSDCountingSort()
{
	int threadRes;
	int sizeSubArrs = arraySize / NUM_OF_THREADS;
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		//printf("Creating thread %d\n", i);

		tInfos1[i].threadNum = i;
		// From and including start index
		tInfos1[i].startIdx = i*sizeSubArrs;									
		// Up to and excluding end index
		tInfos1[i].endIdx = min( tInfos1[i].startIdx + sizeSubArrs, arraySize);	
		// Number of assigned elements
		nElemsAssigned[i] = tInfos1[i].endIdx - tInfos1[i].startIdx;
		// Initialize bucket array for thread
		threadBucketArrays[i] = new int[nBuckets * nElemsAssigned[i]];	
		threadNextArrays[i] = new int[nBuckets]();

   		threadRes = pthread_create(&threads[i], NULL, parallelTask1, (void *) &tInfos1[i]);
    	if (threadRes) {
      		printf("ERROR: return code from pthread_create() is %d\n", threadRes);
      		exit(-1);
   	 	}
	}		
	// Join threads
	int status;
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		pthread_join(threads[i], (void**)&status);
	}
}

void serialComputeMSDOutputIndices()
{
	ouputMSDStartIndices[0] = 0;
	int tmpNBuckets;
	for (int i = 0; i < nBuckets; i++) {
		tmpNBuckets = 0;
		for (int j = 0; j < NUM_OF_THREADS; j++) {
			tmpNBuckets += threadNextArrays[j][i]; 		
		}
		bucketSizes[i] = tmpNBuckets;

		if(tmpNBuckets > 0) {
			//printf("Bucketsize %d : %d\n", i, tmpNBuckets);
		}

		if (i != nBuckets -1) {
			ouputMSDStartIndices[i+1] = ouputMSDStartIndices[i] + tmpNBuckets;
		} 
		
		//printf("Prefix sum: %d\n", ouputMSDStartIndices[i]);
	}
}

void parallelLSDSort()
{
	int threadRes;
	int nBucketsForEachThread = nBuckets / NUM_OF_THREADS;
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		//printf("Creating thread %d\n", i);

		tInfos2[i].threadNum = i;
		// From and including start index
		tInfos2[i].msdBucketStart = i*nBucketsForEachThread;									
		// Up to and excluding end index
		tInfos2[i].msdBucketEnd = min( tInfos2[i].msdBucketStart + nBucketsForEachThread, nBuckets);	

   		threadRes = pthread_create(&threads[i], NULL, parallelTask2, (void *) &tInfos2[i]);
    	if (threadRes) {
      		printf("ERROR: return code from pthread_create() is %d\n", threadRes);
      		exit(-1);
   	 	}

   	 	//printf("Thread %d created\n", i);	
	}		
	// Join threads
	int status;
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		pthread_join(threads[i], (void**)&status);
	}
}

static void *parallelTask1(void *arg) 
{
	struct threadInfo1 *tInfo = (struct threadInfo1 *) arg;
	//printf("Task1 for %d started\n", tInfo->threadNum);
	
	// MSD counting sort 
	int *bucketArray = threadBucketArrays[tInfo->threadNum];
	int *nextArray = threadNextArrays[tInfo->threadNum];

	// DEBUG

	int putInBucket;
	for (int i = tInfo->startIdx; i < tInfo->endIdx; i++) {
		// Shift K - D to get to get the D most significant bits
		//printf("Before shift %d\n", inputArray[i]);

		putInBucket = inputArray[i] >> msdShiftAmount; 
		//printf("After shift %d\n", putInBucket);

		/*
		printf("putInBucket %d, nElemsAssigned[tInfo->threadNum] %d, nextArray[putInBucket] %d, i %d\n", 
			putInBucket, nElemsAssigned[tInfo->threadNum], nextArray[putInBucket], i);
		*/

		bucketArray[ putInBucket*nElemsAssigned[tInfo->threadNum] + nextArray[putInBucket] ] = inputArray[i];
		nextArray[putInBucket]++;
	}

	// Debug
	/*
	for (int bucket = 0; bucket < nBuckets; bucket++) {
		if (nextArray[bucket] > 0) {
			printThreadBucket(tInfo->threadNum, bucket);
		}
	}
	*/

	//printf("Task1 for %d completed\n", tInfo->threadNum);	
	pthread_exit(0);
}

static void *parallelTask2(void *arg) 
{
	struct threadInfo2 *tInfo = (struct threadInfo2 *) arg;
	//printf("Task2 for %d started\n", tInfo->threadNum);

	int* localBuckets = new int[nBuckets * arraySize]; 
	int* localNextArr = new int[nBuckets]();
	int* lastLSDhistogram = new int[nBuckets];
	int* tmpBuckets = new int[nBuckets * arraySize];	
	int* tmpNextArr = new int[nBuckets]();
	int* d2prefixSum = new int[nBuckets];

	int shiftRight2nLast = PARAM_D;	
	int shiftRightLast = 2*PARAM_D;
	int* threadBucketArray;
	int* threadNextArray;
	int startIdxForMSDBucket, elem, putInBucket, histIdx, outputIdx, bucketStartIdx;

	// Assigned buckets
	for (int msdBucket = tInfo->msdBucketStart; msdBucket < tInfo->msdBucketEnd; msdBucket++) {
		//printf("Thread %d look at msd bucket %d\n", tInfo->threadNum, msdBucket);
		startIdxForMSDBucket = ouputMSDStartIndices[msdBucket];
		
		// Reset	
		fill(localNextArr, localNextArr + nBuckets, 0);

		// First pass, sort assigned global buckets into local buckets based on the lowest D bits
		// For each thread
		for (int threadNum = 0; threadNum < NUM_OF_THREADS; threadNum++) {

			threadBucketArray = threadBucketArrays[threadNum];
			threadNextArray = threadNextArrays[threadNum];
			bucketStartIdx = msdBucket*nElemsAssigned[threadNum];

			// Each element in thread's bucket
			for (int i = 0; i < threadNextArray[msdBucket]; i++) {
				// Put in this thread's local bucket
				elem = threadBucketArray[bucketStartIdx + i];
				putInBucket = elem & lsdBitMask;
				localBuckets[putInBucket*arraySize + localNextArr[putInBucket]] = elem;
				localNextArr[putInBucket]++;
			}
		}
		//printf("Phase 1 for %d ended\n", tInfo->threadNum);
		
		// DEBUG
		//printLocalBucketArray(localBuckets, localNextArr, nBuckets, arraySize); 

		// TODO passes in between first and second to last

		// Second to last pass: sort and make histogram

		// Reset	
		fill(tmpNextArr, tmpNextArr + nBuckets, 0);

		// For each bucket and each element in said bucket
		for (int bucket = 0; bucket < nBuckets; bucket++) {
			for (int i = 0; i < localNextArr[bucket]; i++) {
				
				elem = localBuckets[bucket*arraySize + i];

				// DEBUG
				/*
				printf("2nd last phase: Elem: ");
				printBinary(elem);
				printf(", shifted elem: ");
				printBinary((elem >> shiftRight2nLast));
				printf(", masked: ");
				printBinary((elem >> shiftRight2nLast) & lsdBitMask);
				printf("\n");
				*/

				// Put in bucket
				putInBucket = (elem >> shiftRight2nLast) & lsdBitMask;
				tmpBuckets[putInBucket*arraySize + tmpNextArr[putInBucket]] = elem;
				tmpNextArr[putInBucket]++;

				// Histogram
				histIdx = (elem >> shiftRightLast) & lsdBitMask;
				lastLSDhistogram[histIdx]++;
			}
		}
		localBuckets = tmpBuckets;
		localNextArr = tmpNextArr;

		//printf("Phase 2 for %d ended\n", tInfo->threadNum);

		d2prefixSum[0] = 0;		
		for (int i = 0; i < nBuckets; i++) {
			// Prefix sum
			if (i != nBuckets -1) {
				d2prefixSum[i+1] = d2prefixSum[i] + lastLSDhistogram[i];
			} 
		}

		// Last pass: write to output array
		for (int bucket = 0; bucket < nBuckets; bucket++) {
			for (int i = 0; i < localNextArr[bucket]; i++) {
				
				elem = localBuckets[bucket*arraySize + i];

				putInBucket = (elem >> shiftRightLast) & lsdBitMask;

				outputIdx = startIdxForMSDBucket + d2prefixSum[putInBucket];

				/*
				printf("\tElem: %d, new bucket %d, d1Psum %d, d2Psum %d, output idx %d\n", elem, putInBucket, 
						startIdxForMSDBucket, d2prefixSum[putInBucket], outputIdx);
				*/

				d2prefixSum[putInBucket]++;
				sortedArray[outputIdx] = elem;
			}
		}
		//printf("Phase 3 for %d ended\n", tInfo->threadNum);
	}

	//printf("Task2 for %d completed\n", tInfo->threadNum);	
	pthread_exit(0);
}

void printThreadBucket(int threadNum, int bucketNum) 
{
	int* bucketArr = threadBucketArrays[threadNum];
	int* nextArr = threadNextArrays[threadNum];

	printf("Bucket %d for thread %d, has size %d\n", bucketNum, threadNum, nextArr[bucketNum]);
	for (int i = 0; i < nextArr[bucketNum]; i++) {
		int elem = bucketArr[bucketNum*nElemsAssigned[threadNum] + i];
		printf("%d, ", elem);
	}
	printf("\n");
}

void printLocalBucketArray(int* bucketArr, int* nextArr, int numBuckets, int nElements) 
{
	printf("Bucket array:\n");	
	for (int bucket = 0; bucket < numBuckets; bucket++) {
		if (nextArr[bucket] > 0) {
			printf("\tBucket %d (%d): ", bucket, nextArr[bucket]);
			for (int i = 0; i < nextArr[bucket]; i++) {
				int elem = bucketArr[bucket*nElements + i];
				printf("%d (", elem);
				printBinary(elem);
				printf("), ");
			}
			printf("\n");
		}
	}
	printf("\n");
}

void printBinary(int n)
{
	int j = 0;
	for (int i = 31; i >= 0; i--) {
    	if (j % 8 == 0 && i != 31) {
    		printf(" ");
    	}
    	printf("%d", ((n >> i) & 1));
    	j++;
    }
}

void printArray2(int* array, int arrSize) 
{
  printf("[");  
  for (int i = 0; i < arrSize; i++) {
    printf(" %d ", array[i]);
    if (i != arrSize - 1) {
      printf("|");
    }  
  }
  printf("]\n");
}
