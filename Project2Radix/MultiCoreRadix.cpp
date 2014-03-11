#include <pthread.h>
#include <stdio.h>
#include <algorithm>

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

int shiftAmount = PARAM_K - PARAM_D;
int nBuckets;
pthread_t *threads = new pthread_t[NUM_OF_THREADS];
threadInfo1 *tInfos1 = new threadInfo1[NUM_OF_THREADS];
threadInfo2 *tInfos2 = new threadInfo2[NUM_OF_THREADS];
int **threadBucketArrays = new int*[NUM_OF_THREADS];
int **threadNextArrays = new int*[NUM_OF_THREADS];
int *inputArray;
int nInputElements;
int *bucketSizes;
int *prefixSumArr; 

MultiCoreRadix::MultiCoreRadix() 
{
}

MultiCoreRadix::~MultiCoreRadix() 
{
}

void MultiCoreRadix::setup(int arrSize)
{
	nInputElements = arrSize;
	nBuckets = pow(2, PARAM_D);
	bucketSizes = new int[nBuckets];	
	prefixSumArr = new int[nBuckets];	 
}

static void *parallelTask1(void *arg) {
	struct threadInfo1 *tInfo = (struct threadInfo1 *) arg;
	printf("Task1 for %d started\n", tInfo->threadNum);
	
	printf("startIdx %d, endIdx %d \n", tInfo->startIdx, tInfo->endIdx);

	// MSD counting sort 
	int *bucketArray = threadBucketArrays[tInfo->threadNum];
	int *nextArray = threadNextArrays[tInfo->threadNum];
	int putInBucket;
	for (int i = tInfo->startIdx; i < tInfo->endIdx; i++) {
		// Shift K - D to get to get the D most significant bits
		//printf("Before shift %d\n", inputArray[i]);

		putInBucket = inputArray[i] >> shiftAmount; 
		//printf("After shift %d\n", putInBucket);

		bucketArray[ putInBucket*nInputElements + nextArray[putInBucket] ];
		nextArray[putInBucket]++;
	}

	printf("Task1 for %d completed\n", tInfo->threadNum);	
	pthread_exit(0);
}

static void *parallelTask2(void *arg) {
	struct threadInfo2 *tInfo = (struct threadInfo2 *) arg;
	
	pthread_exit(0);
}

int* MultiCoreRadix::sort(int* array, int arrSize)
{
	inputArray = array;

	// Part 1 - Parallel: MSD counting sort 
	int threadRes;
	int sizeSubArrs = arrSize / NUM_OF_THREADS;
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		printf("Creating thread %d\n", i);

		tInfos1[i].threadNum = i;
		// From and including start index
		tInfos1[i].startIdx = i*sizeSubArrs;									
		// Up to and excluding end index
		tInfos1[i].endIdx = min( tInfos1[i].startIdx + sizeSubArrs, arrSize);	
		// Initialize bucket array for thread
		threadBucketArrays[i] = new int[nBuckets * (tInfos1[i].endIdx - tInfos1[i].startIdx)];	
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
	printf("Part 1 done\n");

	// Part 2 - Serial: Compute prefix sum
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
		if (i == 0) {
			prefixSumArr[i] = tmpNBuckets;
		} else {
			prefixSumArr[i] = prefixSumArr[i-1] + tmpNBuckets;
		} 
		
		//printf("Prefix sum: %d\n", prefixSumArr[i]);
	}
	printf("Part 2 done\n");

	// Part 3 - Parallel: The rest, LSD 
	int* sortedArray = new int[arrSize];
	int threadRes;
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		printf("Creating thread %d\n", i);

		/*
		tInfos1[i].threadNum = i;
		// From and including start index
		tInfos1[i].msdBucketStart = i*sizeSubArrs;									
		// Up to and excluding end index
		tInfos1[i].endIdx = min( tInfos1[i].startIdx + sizeSubArrs, arrSize);	
		*/

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
	printf("Part 3 done\n");

	return sortedArray;
}
