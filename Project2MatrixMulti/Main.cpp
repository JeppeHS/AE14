#include "PerfStatClass.h"

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
  PerfStatClass perf = PerfStatClass();

  perf.perf_event_reset();
  perf.perf_event_enable();

  perf.startTimeNow();

  perf.endTimeNow();
  
  perf.perf_event_disable();
  
  const int nStats = perf.getNumberOfStats();
  long long stats[nStats];
  perf.read_all(stats);
  for (int i=0; i<nStats; i++){
    cout<<stats[i]<<endl;
  }

  long long time = perf.getTimeDiff();

  cout << time << endl;
  
  return 0;
}
