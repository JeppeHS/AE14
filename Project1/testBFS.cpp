#include <iostream>
#include "BFSBinarySearch.h"
using std::cout;
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>

//int perf_event_open(struct perf_event_attr *attr,
//                    pid_t pid, int cpu, int group_fd,
//                    unsigned long flags);


int main(){
  const int N = 9;
  int linArray[N] = {1,2,4,5,6,7,8,9,10};
  BFSBinarySearch bfsBinSearch;
  bfsBinSearch.createDataStructure(linArray, N);
  int elem = 3;

  perf_event_open();
  int res =  bfsBinSearch.binSearch(elem);
  cout << "Pred("<<elem<<") = "<<res<<"\n";
  return 0;
}
