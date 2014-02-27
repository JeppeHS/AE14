#include <cstdlib> 
#include <ctime> 
#include <string>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <time.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <climits>

#include "BinSearchInterface.h"
#include "BFSBinarySearch.h"
#include "DFSBinarySearch.h"
#include "BinarySearch.h"
#include "LinearSearch.h"
#include "vEBBinarySearch.h"

using namespace std;

const int NUM_EXPERIMENTS = 100;
const int RUN_TIMES = 1000;
const int ARR_SIZE_SCALING = 1501;
const int ARR_SIZE_OFFSET = 50000;
const int MEM_MAX = 1000000000;


timespec diff(timespec start, timespec end);
int getRandomNumber(int low, int high, int seed);
void fillArrayWithRandom(int * array, int size, int low, int high, int seed);
timespec getTimeDiff(timespec start, timespec end);

void create_all_data_structures(BinSearchInterface *algo_arrray, int *array, int arrSize);
long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
		     int cpu, int group_fd, unsigned long flags);
int make_perf_event(int conf, int type);
void perf_event_array(int conf_array[], int type_array[], int* fd_array, int nStatsPerf);
void perf_event_reset(int fd_array[], int nStatsPerf);
void perf_event_enable(int fd_array[], int nStatsPerf);
void perf_event_disable(int fd_array[], int nStatsPerf);
void read_all(long long stat_row[], int* fd_array, int nStatsPerf);


const string output_dir = "Measurements";

timespec startTime, endTime, timeDiff;

int main(int argc, char **argv)
{
  //cout << ARR_SIZE_SCALING << endl;
  LinearSearch ls = LinearSearch();
  BinarySearch inorder = BinarySearch();
  BFSBinarySearch bfs = BFSBinarySearch();
  DFSBinarySearch dfs = DFSBinarySearch();
  vEBBinarySearch veb = vEBBinarySearch();
  BinSearchInterface *algo_array[] = {&inorder, &bfs, &veb, &dfs};  // <-- Choose the implementations to run.
  const int nAlgos = sizeof(algo_array)/sizeof(BinSearchInterface*);
  const char *algo_labels[nAlgos] = {};
  for (int i=0; i<nAlgos; i++) {
    algo_labels[i] = (*algo_array[i]).getLabel();
  }

  double memReq = (double)((nAlgos*NUM_EXPERIMENTS*ARR_SIZE_SCALING + ARR_SIZE_OFFSET)*sizeof(int));
  printf("The following runs will use up to %g bytes of memory.\n",memReq);
  if (memReq > MEM_MAX) {
    printf("... which is more than the maximum limit set (%ld). Aborting.\n", MEM_MAX);
    return 0;
  }

  int conf_array[] = {PERF_COUNT_HW_BRANCH_MISSES,
		      PERF_COUNT_HW_INSTRUCTIONS,
		      PERF_COUNT_SW_TASK_CLOCK,
		      PERF_COUNT_SW_CPU_CLOCK,
		      PERF_COUNT_HW_CACHE_REFERENCES,
		      PERF_COUNT_HW_CACHE_MISSES, 
		      PERF_COUNT_HW_CPU_CYCLES,
		      PERF_COUNT_SW_PAGE_FAULTS,
		      PERF_COUNT_SW_PAGE_FAULTS_MIN,
		      PERF_COUNT_SW_PAGE_FAULTS_MAJ
		      //(PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16),
		      //(PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16)
  };


  int HW = PERF_TYPE_HARDWARE;
  int SW = PERF_TYPE_SOFTWARE;
  int HW_C = PERF_TYPE_HW_CACHE;
  int type_array[] = {HW,
		      HW,
		      SW,
		      SW,
		      HW,
		      HW,
		      HW,
		      SW,
		      SW,
		      SW};
		      //HW_C,
		      //HW_C};
  const int nStats = sizeof(conf_array)/sizeof(int);

  string conf_labels[nStats] = {"Branch misses.csv",
				      "Instructions.csv",
				      "Task clock.csv",
				      "CPU clock.csv",
				      "Cache refs.csv",
				      "Cache misses.csv", 
				      "Cpu cycles.csv",
				      "Page faults.csv",
				      "Page faults Non-IO.csv",
				     "Page faults IO.csv"
				     //"Cache_L1D accesses.csv",
				     //"Cache_L1D misses.csv"
				 };

  int fd_array[nStats];
  perf_event_array(conf_array, type_array, fd_array, nStats);
  long long stat_array[nAlgos][RUN_TIMES][nStats];
	
  // Timing: Prepare file
  FILE * timeFile;
  timeFile = fopen((output_dir + "/Time in ns.csv").c_str(), "w");
  fprintf(timeFile, "Array size,");
  for (int i = 0; i < nAlgos; i++) {
  	fprintf(timeFile, "%s,", algo_labels[i]);
  }
  fprintf(timeFile, "\n");


  FILE *files[nStats];
  {int i; for (i=0; i<nStats; i++) {
      files[i] = fopen((output_dir + "/" + conf_labels[i]).c_str(), "w");
      fprintf(files[i], "Array size,");
      int j; for (j=0; j<nAlgos; j++) {
	fprintf(files[i], "%s,", algo_labels[j]);
      }
      fprintf(files[i], "\n");
    }}

	int low, high, searchFor;
	for (int i = 0; i < NUM_EXPERIMENTS; i++) {

		// Create random array
		const int arrSize = i*ARR_SIZE_SCALING + ARR_SIZE_OFFSET;

		printf("Experiment %d/%d  \t Array size %g \n", (i+1), NUM_EXPERIMENTS, (double)arrSize);
		
		high = arrSize;
		low = 1;
		//cout << "hello!"<< arrSize << endl;										
		int* array = new int[arrSize];

		fillArrayWithRandom(array, arrSize, low, high, i+1);
		//Sort array
		sort(array, array + arrSize);

		// Set up algorithms
		{int i;
		 for (i=0; i<nAlgos; i++) {
		   (*algo_array[i]).createDataStructure(array, arrSize);
		   }}

		
		// Timing: Setup array
		long tmpDiff; 
		long timeArray[nAlgos];
		for (int j = 0; j < nAlgos; j++) {
			timeArray[j] = 0;
		}		 

		// Repeat experiments
		for (int j = 0; j < RUN_TIMES; j++) {
			searchFor = getRandomNumber(low, high, INT_MAX - j);
			//searchFor = low;	
			//searchFor = high;

			int oldRes = -1;
			int newRes;
			
			// Perform experiments
			{int iAlg;
			  for (iAlg=0; iAlg<nAlgos; iAlg++){
			    
			    // Timing
				clock_gettime(CLOCK_REALTIME, &startTime); 

			    perf_event_reset(fd_array, nStats);
			    // Start all the stat-counters:
			    perf_event_enable(fd_array, nStats);

			    // Perform the binary search:
			    newRes = (*algo_array[iAlg]).binSearch(searchFor);

			    // Stop all the stat-counters:
			    perf_event_disable(fd_array, nStats);
			    
			    // Timing
			    clock_gettime(CLOCK_REALTIME, &endTime); 
			    timeDiff = getTimeDiff(startTime, endTime); 
			    tmpDiff = timeArray[iAlg];
			    timeArray[iAlg] = tmpDiff + timeDiff.tv_nsec; 


			    // Check result
			    if (oldRes != -1 && oldRes != newRes) {
					printf("Wrong result. prev:%s %d, new:%s %d, ArrSize %d, searchFor %d\n", 
						algo_labels[iAlg-1], oldRes, algo_labels[iAlg], newRes, arrSize, searchFor); 
					
			    }
		      oldRes = newRes;
		    
		    //printf("iAlg=%d done", iAlg); // TODO: remove print
		    // Store the stats in the stat_array
		    read_all(stat_array[iAlg][j], fd_array, nStats);
		    
			  }
			}
		}
	
	// Loop through all the implementations and calculate the average of every stat for searches with this array size.
		{for (int iStat = 0; iStat<nStats; iStat++){
		    fprintf(files[iStat], "%d,",arrSize);
		    for (int iAlg=0; iAlg<nAlgos; iAlg++){
		      long long stat_sum = 0;
		      for (int j=0; j<RUN_TIMES; j++){
			stat_sum += stat_array[iAlg][j][iStat];
			//cout << stat_sum << endl;
		      }
		      //TODO: go back to dividing? done...
		      double stat_avg = stat_sum / RUN_TIMES;
		      fprintf(files[iStat], "%g,", stat_avg);
		    }
			fprintf(files[iStat], "\n");
		  }}

		delete [] array;
		array = NULL;
		// Timing: Save time values
		fprintf(timeFile, "%d,", arrSize);  
		for (int j = 0; j < nAlgos; j++) {
			fprintf(timeFile, "%ld,", timeArray[j]/RUN_TIMES);
		}  
		fprintf(timeFile, "\n");
	}
	// TODO: Perhaps close the fd_array and files...
	return 0;
}

int getRandomNumber(int low, int high, int seed) {
	srand(seed + (unsigned) time(0));
	return rand() % ( (high+1) - low ) + low;
}

void fillArrayWithRandom(int * array, int size, int low, int high, int seed)
{
	srand(seed + (unsigned) time(0));
    for(int i=0; i < size; i++){ 
        array[i] = rand() % ( (high+1) - low ) + low;
	} 
}

timespec getTimeDiff(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
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

int make_perf_event(int conf, int type){
  struct perf_event_attr pe;
  int fd;

  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = type;
  pe.size = sizeof(struct perf_event_attr);
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

void perf_event_array(int conf_array[], int type_array[], int* fd_array, int nStats){
  int i;
  for (i=0; i<nStats; i++) {
    fd_array[i] = make_perf_event(conf_array[i], type_array[i]);
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

void read_all(long long stat_row[], int* fd_array, int nStats){
  int i;
  for (i=0; i<nStats; i++){
    long long stat = 0;
    read(fd_array[i], &stat, sizeof(long long));
    stat_row[i] = stat;
    //fprintf(file, "%lld,", stat);
  }
  //fprintf(file, "\n");
}
