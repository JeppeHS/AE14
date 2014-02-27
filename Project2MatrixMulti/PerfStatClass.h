#include <cstdlib> 
#include <ctime> 
#include <string>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <cmath>
#include <time.h>
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <climits>

long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
		     int cpu, int group_fd, unsigned long flags);
int make_perf_event(int conf, int type);
void perf_event_array();
void perf_event_reset();
void perf_event_enable();
void perf_event_disable();
void read_all(long long stat_row[]);
long long getTimeDiff();
void startTimeNow();
void endTimeNow();

  static timespec startTime, endTime;

  static int conf_array[] = {PERF_COUNT_HW_BRANCH_MISSES,
		      PERF_COUNT_HW_INSTRUCTIONS,
		      PERF_COUNT_SW_TASK_CLOCK,
		      PERF_COUNT_SW_CPU_CLOCK,
		      PERF_COUNT_HW_CACHE_REFERENCES,
		      PERF_COUNT_HW_CACHE_MISSES, 
		      PERF_COUNT_HW_CPU_CYCLES,
		      PERF_COUNT_SW_PAGE_FAULTS,
		      PERF_COUNT_SW_PAGE_FAULTS_MIN,
		      PERF_COUNT_SW_PAGE_FAULTS_MAJ};
		      //(PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16),
		      //(PERF_COUNT_HW_CACHE_L1D) | (PERF_COUNT_HW_CACHE_OP_READ << 8) | (PERF_COUNT_HW_CACHE_RESULT_MISS << 16)};


  static int HW = PERF_TYPE_HARDWARE;
  static int SW = PERF_TYPE_SOFTWARE;
  static int HW_C = PERF_TYPE_HW_CACHE;
  static int type_array[] = {HW,
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

  static const int nStats = sizeof(conf_array)/sizeof(int);
  
    static int fd_array[nStats];

  static std::string conf_labels[nStats] = {"Branch misses.csv",
				"Instructions.csv",
				"Task clock.csv",
				"CPU clock.csv",
				"Cache refs.csv",
				"Cache misses.csv", 
				"Cpu cycles.csv",
				"Page faults.csv",
				"Page faults Non-IO.csv",
				"Page faults IO.csv"};
				//"Cache_L1D accesses.csv",
				//"Cache_L1D misses.csv"};



class PerfStatClass{

 public:
  PerfStatClass(){
    perf_event_array();
  }

  int getNumberOfStats() {
    return nStats;
  }

  void startTimeNow(){
    startTime.tv_sec = 0;
    startTime.tv_nsec = 0;
    clock_gettime(CLOCK_REALTIME, &startTime);
  }
  
  void endTimeNow(){
    endTime.tv_sec = 0;
    endTime.tv_nsec = 0;
    clock_gettime(CLOCK_REALTIME, &endTime);
  }

  long long getTimeDiff()
  {
    timespec temp;
    if ((endTime.tv_nsec-startTime.tv_nsec)<0) {
      temp.tv_sec = endTime.tv_sec-startTime.tv_sec-1;
      temp.tv_nsec = 1000000000+endTime.tv_nsec-startTime.tv_nsec;
    } else {
      temp.tv_sec = endTime.tv_sec-startTime.tv_sec;
      temp.tv_nsec = endTime.tv_nsec-startTime.tv_nsec;
    }
    return (temp.tv_sec*1000000000 + temp.tv_nsec);
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

  void perf_event_array(){
    int i;
    for (i=0; i<nStats; i++) {
      fd_array[i] = make_perf_event(conf_array[i], type_array[i]);
    }
  }

  void perf_event_reset(){
    int i;
    for (i=0; i<nStats; i++) {
      ioctl(fd_array[i], PERF_EVENT_IOC_RESET, 0);
    }
  }

  void perf_event_enable(){
    int i;
    for (i=0; i<nStats; i++) {
      ioctl(fd_array[i], PERF_EVENT_IOC_ENABLE, 0);
    }
  }

  void perf_event_disable(){
    int i;
    for (i=0; i<nStats; i++) {
      ioctl(fd_array[i], PERF_EVENT_IOC_DISABLE, 0);
    }
  }

  void read_all(long long stat_row[]){
    int i;
    for (i=0; i<nStats; i++){
      long long stat = 0;
      read(fd_array[i], &stat, sizeof(long long));
      stat_row[i] = stat;
      //fprintf(file, "%lld,", stat);
    }
  }

};
