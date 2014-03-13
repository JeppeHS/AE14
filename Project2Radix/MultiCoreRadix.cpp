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
void serialComputePrefixSum();
void parallelLSDSort();
static void *parallelTask1(void *arg);
static void *parallelTask2(void *arg);

// DEBUG methods
void printThreadBucket(int threadNum, int bucketNum);
void printLocalBucketArray(int* bucketArr, int* nextArr, int numBuckets, int nElements);
void printBinary(int n);
// DEBUG methods - END

int shiftAmount = PARAM_K - PARAM_D;
int nBuckets;
pthread_t *threads = new pthread_t[NUM_OF_THREADS];
threadInfo1 *tInfos1 = new threadInfo1[NUM_OF_THREADS];
threadInfo2 *tInfos2 = new threadInfo2[NUM_OF_THREADS];
int **threadBucketArrays = new int*[NUM_OF_THREADS];
int **threadNextArrays = new int*[NUM_OF_THREADS];
int *inputArray;
int* sortedArray;
int arraySize;
int *bucketSizes;
int *outputStartIndices;

int* nElemsAssigned = new int[NUM_OF_THREADS]; 
int lsdBitMask = (1 << PARAM_D) - 1;

MultiCoreRadix::MultiCoreRadix() 
{
}

MultiCoreRadix::~MultiCoreRadix() 
{
}

void MultiCoreRadix::setup(int arrSize)
{
	arraySize = arrSize;
	nBuckets = pow(2, PARAM_D);
	bucketSizes = new int[nBuckets];	
	outputStartIndices = new int[nBuckets];	 
}

int* MultiCoreRadix::sort(int* array, int arrSize)
{
	inputArray = array;
	sortedArray = new int[arrSize];

	// Part 1 - Parallel: MSD counting sort 
	parallelMSDCountingSort();
	printf("Part 1 done\n");

	// Part 2 - Serial: Compute prefix sum
	serialComputePrefixSum();
	printf("Part 2 done\n");

	// Part 3 - Parallel: The rest, LSD
	parallelLSDSort();
	printf("Part 3 done\n");

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
		threadNextArrays[i] = new int[nBuckets];

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

void serialComputePrefixSum()
{
	int tmpNBuckets;
	for (int i = 0; i < nBuckets; i++) {
		tmpNBuckets = 0;
		for (int j = 0; j < NUM_OF_THREADS; j++) {
			tmpNBuckets += threadNextArrays[j][i]; 		
		}
		bucketSizes[i] = tmpNBuckets;

		if(tmpNBuckets > 0) {
			printf("Bucketsize %d : %d\n", i, tmpNBuckets);
		}

		// Prefix sum
		if (i != nBuckets -1) {
			outputStartIndices[i+1] = outputStartIndices[i] + tmpNBuckets;
		} 
		
		//printf("Prefix sum: %d\n", outputStartIndices[i]);
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
	int putInBucket;
	for (int i = tInfo->startIdx; i < tInfo->endIdx; i++) {
		// Shift K - D to get to get the D most significant bits
		//printf("Before shift %d\n", inputArray[i]);

		putInBucket = inputArray[i] >> shiftAmount; 
		//printf("After shift %d\n", putInBucket);

		bucketArray[ putInBucket*nElemsAssigned[tInfo->threadNum] + nextArray[putInBucket] ] = inputArray[i];
		nextArray[putInBucket]++;
	}

	// Debug
	for (int bucket = 0; bucket < nBuckets; bucket++) {
		if (nextArray[bucket] > 0) {
			printThreadBucket(tInfo->threadNum, bucket);
		}
	}

	//printf("Task1 for %d completed\n", tInfo->threadNum);	
	pthread_exit(0);
}

static void *parallelTask2(void *arg) 
{
	struct threadInfo2 *tInfo = (struct threadInfo2 *) arg;
	//printf("Task2 for %d started\n", tInfo->threadNum);

	// Assigned buckets
	for (int msdBucket = tInfo->msdBucketStart; msdBucket < tInfo->msdBucketEnd; msdBucket++) {
		int startIdxForMSDBucket = outputStartIndices[msdBucket];

		int* localBuckets = new int[nBuckets * arraySize];	
		int* localNextArr = new int[nBuckets];
		// First pass, sort assigned global buckets into local buckets based on the lowest D bits
		// For each thread
		for (int threadNum = 0; threadNum < NUM_OF_THREADS; threadNum++) {
			
			int* threadBucketArray = threadBucketArrays[threadNum];
			int* threadNextArray = threadNextArrays[threadNum];
			int bucketStartIdx = msdBucket*nElemsAssigned[threadNum];
			// Each element in thread's bucket
			for (int i = 0; i < threadNextArray[msdBucket]; i++) {
				// Put in this thread's local bucket
				int elem = threadBucketArray[bucketStartIdx + i];
				int putInBucket = elem & lsdBitMask;
				localBuckets[putInBucket*arraySize + localNextArr[putInBucket]] = elem;
				localNextArr[putInBucket]++;
			}
		}
		
		// DEBUG
		//printLocalBucketArray(localBuckets, localNextArr, nBuckets, arraySize); 

		// TODO passes in between first and second to last

		// Second to last pass: sort and make histogram 
		int* lastLSDhistogram = new int[nBuckets];
		int* tmpBuckets = new int[nBuckets * arraySize];	
		int* tmpNextArr = new int[nBuckets];

		// TODO make general	
		int shiftRight2nLast = PARAM_D;	
		int shiftRightLast = 2*PARAM_D;
		// TODO make general - END

		// For each bucket and each element in said bucket
		for (int bucket = 0; bucket < nBuckets; bucket++) {
			for (int i = 0; i < localNextArr[bucket]; i++) {
				
				int elem = localBuckets[bucket*arraySize + i];

				// DEBUG
				printf("2nd last phase: Elem: ");
				printBinary(elem);
				printf(", shifted elem: ");
				printBinary((elem >> shiftRight2nLast));
				printf(", masked: ");
				printBinary((elem >> shiftRight2nLast) & lsdBitMask);
				printf("\n");

				// Put in bucket
				int putInBucket = (elem >> shiftRight2nLast) & lsdBitMask;
				tmpBuckets[putInBucket*arraySize + tmpNextArr[putInBucket]] = elem;
				tmpNextArr[putInBucket]++;

				// Histogram
				int histIdx = (elem >> shiftRightLast) & lsdBitMask;
				lastLSDhistogram[histIdx]++;
			}
		}
		localBuckets = tmpBuckets;
		localNextArr = tmpNextArr;

		int* d2prefixSum = new int[nBuckets];
		for (int i = 0; i < nBuckets; i++) {
			// Prefix sum
			if (i != nBuckets -1) {
				d2prefixSum[i+1] = d2prefixSum[i] + lastLSDhistogram[i];
			} 
		}

		// Last pass: write to output array
		for (int bucket = 0; bucket < nBuckets; bucket++) {
			for (int i = 0; i < localNextArr[bucket]; i++) {
				
				int elem = localBuckets[bucket*arraySize + i];

				int putInBucket = (elem >> shiftRightLast) & lsdBitMask;

				int outputIdx = startIdxForMSDBucket + d2prefixSum[putInBucket];

				printf("\tElem: %d, new bucket %d, d1Psum %d, d2Psum %d, output idx %d\n", elem, putInBucket, 
						startIdxForMSDBucket, d2prefixSum[putInBucket], outputIdx);

				d2prefixSum[putInBucket]++;
				sortedArray[outputIdx] = elem;
			}
		}
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