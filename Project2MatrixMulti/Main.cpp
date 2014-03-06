#include "PerfStatClass.h"
#include "TileMulti.h"
#include "RecMulti.h"
#include "matrix.h"

using std::cout;
using std::endl;

void fillMatrixWithRandomNumbers(matrix* mat, int low, int high, int seed);

int main(int argc, char **argv)
{
	TileMulti tm = TileMulti();
	RecMulti rec = RecMulti();

	int m = 1;
	int n = 3;
	int p = 1;
	matrix* A = createMatrix(m, n);
	//fillMatrix

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
