#include "PerfStatClass.h"
#include "TileMulti.h"
#include "matrix.h"

using std::cout;
using std::endl;

void fillMatrixWithRandomNumbers(matrix* mat, int low, int high, int seed);

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


void fillMatrixWithRandomNumbers(matrix* mat, int low, int high, int seed) {
  // Rows then cols
  srand(seed + (unsigned) time(0));
  int ranVal;
  for (int i = 0; i < mat->nCols; i++) {
    for (int j = 0; j < mat->nRows; j++) {
      ranVal = rand() % ( (high+1) - low ) + low;
      matrixPut(mat, i, j, ranVal);
    }
  }
}
