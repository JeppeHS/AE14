#include <cstdlib> 
#include <ctime> 
#include <string>
#include <iostream>
#include <time.h>
#include <algorithm>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

#include "BinSearchInterface.h"
#include "BFSBinarySearch.h"
//#include "DFSBinarySearch.h"

using namespace std;

const int NUM_EXPERIMENTS = 1;
const int RUN_TIMES = 1;

timespec startTime, endTime;
double timeDiff;
//DFSBinarySearch dfs;

int getRandomNumber(int low, int high, int seed);
void fillArrayWithRandom(int * array, int size, int low, int high, int seed);
void experimentLinearSearch(int elem);
void experimentBasicAlgorithm(int elem);
void experimentBFS(int elem);
void experimentDFS(int elem);
void experimentVEB(int elem);

void create_all_data_structures(BinSearchInterface *algo_arrray, int *array, int arrSize);
long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
		     int cpu, int group_fd, unsigned long flags);
int make_perf_event(int conf);
void perf_event_array(int conf_array[], int* fd_array, int nStats);
void perf_event_reset(int fd_array[], int nStats);
void perf_event_enable(int fd_array[], int nStats);
void perf_event_disable(int fd_array[], int nStats);
void read_all(FILE *file, int* fd_array, int nStats);

const int nAlgos = 1;
const char *algo_labels[nAlgos] = {"BFS.csv"};

int main(int argc, char **argv)
{
  BFSBinarySearch bfs = BFSBinarySearch();
  BinSearchInterface *algo_array[nAlgos] = {&bfs};
  

  int conf_array[] = {PERF_COUNT_HW_BRANCH_MISSES,
		  PERF_COUNT_HW_INSTRUCTIONS};
  const int nStats = sizeof(conf_array)/sizeof(int);
  const char *conf_labels[nStats] = {"Branch misses",
				     "Instructions"};
  int fd_array[nStats];
  perf_event_array(conf_array, fd_array, nStats);

  //int nRuns = 1;
  //  long long stats[nRuns][nStats];

  //dfs = DFSBinarySearch();
	
  FILE *files[nAlgos];
  {int i; for (i=0; i<nAlgos; i++) {
      files[i] = fopen(algo_labels[i], "w");
      int j; for (j=0; j<nStats; j++) {
	fprintf(files[i], "%s,", conf_labels[i]);
      }
      fprintf(files[i], "\n");
    }}

	int arrSize, low, high, searchFor;
	for (int i = 0; i < NUM_EXPERIMENTS; i++) {
		// Create random array
		arrSize = 10;
		high = 100;
		low = 0;
		int array [arrSize];
		fillArrayWithRandom(array, arrSize, low, high, i+1);
		// Sort array
		sort(array, array + arrSize);
		
		// Set up algorithms
		{int i;
		 for (i=0; i<nAlgos; i++){
		   (*algo_array[i]).createDataStructure(array, arrSize);
		 }}
		
		//dfs.createDataStructure(array, arrSize);
		
		// Repeat experiments
		for (int j = 0; j < RUN_TIMES; j++) {
			searchFor = getRandomNumber(low, high, j+1);	
		
			
			
			// Perform experiments
			{int iAlg;
			  for (iAlg=0; iAlg<nAlgos; iAlg++){
			    perf_event_reset(fd_array, nStats);
			    perf_event_enable(fd_array, nStats);
			    (*algo_array[iAlg]).binSearch(searchFor);
			    perf_event_disable(fd_array, nStats);
			    read_all(files[iAlg], fd_array, nStats);
			  }}
			//experimentLinearSearch(searchFor);
			//experimentBasicAlgorithm(searchFor);
			//experimentBFS(searchFor);
			//			experimentDFS(searchFor);
			//experimentVEB(searchFor);
		}
	}
	
	// TODO: Perhaps close the fd_array and files...
	return 0;
}
/*
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
*/
int getRandomNumber(int low, int high, int seed) {
	srand(seed + (unsigned) time(0));
	return rand() % ( (high+1) - low ) + low;
}

void fillArrayWithRandom(int * array, int size, int low, int high, int seed)
{
	srand(seed + (unsigned) time(0));
    for(int i=0; i<size; i++){ 
        array[i] = rand() % ( (high+1) - low ) + low;
        cout << array[i] << endl; 
	}     
}

/*
void create_all_data_structures(BinSearchInterface *algo_array, int *array, int arrSize) {
  int i;
  for (i=0; i<nAlgos; i++){
    (*algo_array[i]).createDataStructure(array, arrSize);
  }
}
*/


long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
		int cpu, int group_fd, unsigned long flags)
{
  int ret;

  ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
		group_fd, flags);
  return ret;
}

int make_perf_event(int conf){
  struct perf_event_attr pe;
  int fd;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  //pe.config = PERF_COUNT_HW_INSTRUCTIONS;
  pe.config = conf;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  fd = perf_event_open(&pe, 0, -1, -1, 0);
  if (fd == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
    exit(EXIT_FAILURE);
  }
  return fd;
}

void perf_event_array(int conf_array[], int* fd_array, int nStats){
  int i;
  for (i=0; i<nStats; i++) {
    fd_array[i] = make_perf_event(conf_array[i]);
  }
}

void perf_event_reset(int fd_array[], int nStats){
  int i;
  for (i=0; i<nStats; i++) {
    ioctl(fd_array[i], PERF_EVENT_IOC_RESET, 0);
  }
}

void perf_event_enable(int fd_array[], int nStats){
  int i;
  for (i=0; i<nStats; i++) {
    ioctl(fd_array[i], PERF_EVENT_IOC_ENABLE, 0);
  }
}

void perf_event_disable(int fd_array[], int nStats){
  int i;
  for (i=0; i<nStats; i++) {
    ioctl(fd_array[i], PERF_EVENT_IOC_DISABLE, 0);
  }
}

void read_all(FILE *file, int* fd_array, int nStats){
  int i;
  for (i=0; i<nStats; i++){
    long long stat = 0;
    read(fd_array[i], &stat, sizeof(long long));
    fprintf(file, "%lld,", stat);
  }
  fprintf(file, "\n");
}
