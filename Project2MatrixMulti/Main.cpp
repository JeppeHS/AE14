#include "PerfStatClass.h"
#include "TileMulti.h"
#include "RecMulti.h"
#include "NaiveMatrixMulti.h"
#include "matrix.h"

using std::cout;
using std::endl;
using std::string;

void fillMatrixWithRandomNumbers(matrix* mat, int low, int high, int seed);

const int NUM_EXPERIMENTS = 100;
const int RUN_TIMES = 10;

const string output_dir = "Measurements";

int main(int argc, char **argv)
{
  TileMulti tm = TileMulti();
  RecMulti rec = RecMulti();
  NaiveMatrixMulti naive = NaiveMatrixMulti();

  PerfStatClass perf = PerfStatClass();
  const int nStats = perf.getNumberOfStats();

  IMatrixMulti *algo_array[] = {&naive, &rec};  // <-- Choose the implementations to run.
  const int nAlgos = sizeof(algo_array)/sizeof(IMatrixMulti*);
  const char *algo_labels[nAlgos] = {"Naive", "Recursive"};
  /*for (int i=0; i<nAlgos; i++) {
    algo_labels[i] = (*algo_array[i]).getLabel();
    }*/


  FILE *files[nStats];
  {int i; for (i=0; i<nStats; i++) {
      files[i] = fopen((output_dir + "/" + perf.getConfLabel(i)).c_str(), "w");
      fprintf(files[i], "Matrix size,");
      int j; for (j=0; j<nAlgos; j++) {
	fprintf(files[i], "%s,", algo_labels[j]);
      }
      fprintf(files[i], "\n");
    }}
  

  for (int i = 0; i < NUM_EXPERIMENTS; i++) {
    long long stat_array[nAlgos][RUN_TIMES][nStats]; 

    int low_A = 1;
    int high_A = i+1;
    int seed_A = i+1; // TODO
    
    const int dim_m = i+2;
    const int dim_n = dim_m;
    const int matSize = dim_m * dim_n; // TODO: something else...
    
    printf("Experiment %d/%d  \t Array size %g \n", (i+1), NUM_EXPERIMENTS, (double)matSize);


    //  perf.startTimeNow();

    matrix* matrix_A = createMatrix(dim_m, dim_n); // <-- TODO

    fillMatrixWithRandomNumbers(matrix_A, low_A, high_A, seed_A);


    const int dim_p = dim_n;

    matrix* matrix_B = createMatrix(dim_n, dim_p); // <-- TODO
    // ...to be filled in inner loop




    // Set up algorithms
    {int i;
      for (i=0; i<nAlgos; i++) {
	(*algo_array[i]).setup(matrix_A, dim_m, dim_n);
      }}

    for (int iStat=0; iStat<nStats; iStat++){
      fprintf(files[iStat], "%d,",matSize);
    }


    matrix* oldMatrix; // ???
    matrix* newMatrix; // ???

      
    //perf.perf_event_reset();
    //perf.perf_event_enable();
      
      for (int j = 0; j < RUN_TIMES; j++) {
	
	int low_B = 1;
	int high_B = j+1;
	int seed_B = j+1; // TODO

	// Fill out matrix_B with new random numbers
	fillMatrixWithRandomNumbers(matrix_B, low_B, high_B, seed_B);

	for (int iAlg=0; iAlg<nAlgos; iAlg++){

	perf.perf_event_reset();
	perf.perf_event_enable();

	//perf.startTimeNow();
	
	// Perform the matrix multiplication:
	newMatrix = (*algo_array[iAlg]).matrixMultiplication(matrix_B);
	

	//perf.endTimeNow();
  
	perf.perf_event_disable();
  
 
	perf.read_all(stat_array[iAlg][j]);
    
	//long long time = perf.getTimeDiff();

	if (iAlg != 0 && !matrixEquals(oldMatrix, newMatrix)) {
	  cout << "Results for " << algo_labels[iAlg] << " and " << algo_labels[iAlg-1] << " are not equal!" << endl;
	}
	oldMatrix = newMatrix;

	}
	//perf.perf_event_disable();
	//perf.read_all(stats[iAlg]);
      }


    {for (int iStat = 0; iStat<nStats; iStat++){
	//fprintf(files[iStat], "%d,",matSize);
	for (int iAlg=0; iAlg<nAlgos; iAlg++){
	  long long stat_sum = 0;
	    for (int j=0; j<RUN_TIMES; j++){
	    stat_sum += stat_array[iAlg][j][iStat];
	    //cout << stat_sum << endl;
	    }
	    double stat_avg = stat_sum / RUN_TIMES;
	  fprintf(files[iStat], "%g,", stat_avg);
	}
	fprintf(files[iStat], "\n");
      }}

  }
  
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
