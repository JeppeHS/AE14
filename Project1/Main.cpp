#include <cstdlib> 
#include <ctime> 
#include <string>
#include <iostream>
#include <time.h>

using namespace std;

const int RUN_TIMES = 10;

timespec startTime, endTime;
double timeDiff;

int getRandomNumber(int low, int high, int seed);
void fillArrayWithRandom(int * array, int size, int low, int high, int seed);
void experimentLinearSearch(int * array, int elem);
void experimentBasicAlgorithm(int * array, int elem);
void experimentBFS(int * array, int elem);
void experimentDFS(int * array, int elem);
void experimentVEB(int * array, int elem);


int main(int argc, char **argv)
{
	int size, low, high, searchFor;
	for (int i = 0; i < RUN_TIMES; i++) {
		// Create random array
		size = 10;
		high = 100;
		low = 0;
		int array [size];
		
		fillArrayWithRandom(array, size, low, high, i+1);
		searchFor = getRandomNumber(low, high, i+1);	
		
		experimentLinearSearch(array, searchFor);
		experimentBasicAlgorithm(array, searchFor);
		experimentBFS(array, searchFor);
		experimentDFS(array, searchFor);
		experimentVEB(array, searchFor);
	}
	
	return 0;
}

void experimentLinearSearch(int * array, int elem) {
 	// Setup
 	
 	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
	// Do stuff here
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
	timeDiff = endTime.tv_nsec - startTime.tv_nsec;
	cout<< "Linear search took " << timeDiff << " ns" << endl;
}

void experimentBasicAlgorithm(int * array, int elem) {
	// Setup
 	
 	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
	// Do stuff here
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
	timeDiff = endTime.tv_nsec - startTime.tv_nsec;
	cout<< "Basic algorithm took " << timeDiff << " ns" << endl;
}

void experimentBFS(int * array, int elem) {
	// Setup
 	
 	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
	// Do stuff here
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
	timeDiff = endTime.tv_nsec - startTime.tv_nsec;
	cout<< "BFS took " << timeDiff << " ns" << endl;
}

void experimentDFS(int * array, int elem) {
	// Setup
 	
 	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
	// Do stuff here
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
	timeDiff = endTime.tv_nsec - startTime.tv_nsec;
	cout<< "DFS took " << timeDiff << " ns" << endl;
}

void experimentVEB(int * array, int elem) {
	// Setup
 	
 	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
	// Do stuff here
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
	timeDiff = endTime.tv_nsec - startTime.tv_nsec;
	cout<< "VEN took " << timeDiff << " ns" << endl;
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
