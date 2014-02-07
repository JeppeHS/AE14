#include <cstdlib> 
#include <ctime> 
#include <string>
#include <iostream>
#include <time.h>

#include "DFSBinarySearch.h"

using namespace std;

const int NUM_EXPERIMENTS = 1;
const int RUN_TIMES = 1;

timespec startTime, endTime;
double timeDiff;
DFSBinarySearch dfs;

int getRandomNumber(int low, int high, int seed);
void fillArrayWithRandom(int * array, int size, int low, int high, int seed);
void experimentLinearSearch(int elem);
void experimentBasicAlgorithm(int elem);
void experimentBFS(int elem);
void experimentDFS(int elem);
void experimentVEB(int elem);


int main(int argc, char **argv)
{
	dfs = DFSBinarySearch();
	
	int size, low, high, searchFor;
	for (int i = 0; i < NUM_EXPERIMENTS; i++) {
		// Create random array
		size = 10;
		high = 100;
		low = 0;
		int array [size];
		fillArrayWithRandom(array, size, low, high, i+1);
		
		// Set up algorithms
		dfs.createDataStructure(array, size);
		
		// Repeat experiments
		for (int j = 0; j < RUN_TIMES; j++) {
			searchFor = getRandomNumber(low, high, j+1);	
		
			// Perform experiments
			//experimentLinearSearch(searchFor);
			//experimentBasicAlgorithm(searchFor);
			//experimentBFS(searchFor);
			experimentDFS(searchFor);
			//experimentVEB(searchFor);
		}
	}
	
	return 0;
}

void experimentLinearSearch(int elem) {
 	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
	// Do stuff here
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
	timeDiff = endTime.tv_nsec - startTime.tv_nsec;
	cout<< "Linear search took " << timeDiff << " ns" << endl;
}

void experimentBasicAlgorithm(int elem) {
 	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
	// Do stuff here
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
	timeDiff = endTime.tv_nsec - startTime.tv_nsec;
	cout<< "Basic algorithm took " << timeDiff << " ns" << endl;
}

void experimentBFS(int elem) {
 	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
	// Do stuff here
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
	timeDiff = endTime.tv_nsec - startTime.tv_nsec;
	cout<< "BFS took " << timeDiff << " ns" << endl;
}

void experimentDFS(int elem) {
 	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
	// Do stuff here
	int result = dfs.binSearch(elem);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
	timeDiff = endTime.tv_nsec - startTime.tv_nsec;
	cout<< "DFS took " << timeDiff << " ns, result " << result << endl;
}

void experimentVEB(int elem) {
 	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
	// Do stuff here
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
	timeDiff = endTime.tv_nsec - startTime.tv_nsec;
	cout<< "VEB took " << timeDiff << " ns" << endl;
}

int getRandomNumber(int low, int high, int seed) {
	srand(seed);
	return rand() % ( (high+1) - low ) + low;
}

void fillArrayWithRandom(int * array, int size, int low, int high, int seed)
{
	srand(seed);
    for(int i=0; i<size; i++){ 
        array[i] = rand() % ( (high+1) - low ) + low;
        cout << array[i] << endl; 
	}     
}
