#include "PerfStatClass.h"
#include "TileMulti.h"
#include "RecMulti.h"
#include "matrix.h"

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
	TileMulti tm = TileMulti();
	RecMulti rec = RecMulti();

	int m = 2;
	int n = 3;
	int p = 2;
	matrix* A = createMatrix(m, n);
	matrixPut(A, 0,0, 1);
	matrixPut(A, 0,1, 1);
	matrixPut(A, 1,0, 1);
	matrixPut(A, 1,1, 1);
	matrixPut(A, 0,2, 1);
	matrixPut(A, 1,2, 1);

	matrix* B = createMatrix(n, p);
	matrixPut(B, 0,0, 1);
	matrixPut(B, 0,1, 1);
	matrixPut(B, 1,0, 1);
	matrixPut(B, 1,1, 1);
	matrixPut(B, 2,0, 1);
	matrixPut(B, 2,1, 1);

	rec.setup(A, m, n);
	matrix* C = rec.matrixMultiplication(B);

	matrixPrint(A);
	matrixPrint(B);
	matrixPrint(C);


  PerfStatClass perf = PerfStatClass();

  perf.perf_event_reset();
  perf.perf_event_enable();

  perf.startTimeNow();

  perf.endTimeNow();
  
  perf.perf_event_disable();
  
  const int nStats = perf.getNumberOfStats();
  long long stats[nStats];
  perf.read_all(stats);
  /*for (int i=0; i<nStats; i++){
    cout<<stats[i]<<endl;
  }

  long long time = perf.getTimeDiff();

  cout << time << endl;*/
  
  return 0;
}
