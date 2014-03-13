#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>

#include "PerfStatClass.h"

#include "LSDRadix.h"
#include "MSDRadix.h"
#include "MSDRadix_countSort.h"
#include "CountingSort.h"
//#include "MultiCoreRadix.h"

void fillArrayWithRandom(int * array, int size, int low, int high, int seed);
void printArray(int* array, int arrSize);
bool arrayEquals(int* oldArray, int* newArray, int arrSize);

using std::cout;
using std::endl;
using std::string;


const int NUM_ORDERS = 6;
const int RUNS_PER_ORDER = 10;

const int NUM_EXPERIMENTS = NUM_ORDERS*RUNS_PER_ORDER;
const int RUN_TIMES = 20;
const int SIZE_MIN = 10;

const string output_dir = "Measurements";

int main(int argc, char **argv)
{
  LSDRadix lsd = LSDRadix();
  MSDRadix msd = MSDRadix();
  MSDRadix_countSort msdCS = MSDRadix_countSort();
  CountingSort cs = CountingSort();
  //MultiCoreRadix mcr = MultiCoreRadix();

  PerfStatClass perf = PerfStatClass();
  const int nStats = perf.getNumberOfStats();

  IRadixSort *algo_array[] = {&msd, &msdCS};  // <-- Choose the implementations to run.
  const int nAlgos = sizeof(algo_array)/sizeof(IRadixSort*);
  const char *algo_labels[nAlgos] = {"MSD", "MSD_cs"};
  /*for (int i=0; i<nAlgos; i++) {
    algo_labels[i] = (*algo_array[i]).getLabel();
    }*/

  FILE *files[nStats];
  {int i; for (i=0; i<nStats; i++) {
      files[i] = fopen((output_dir + "/" + perf.getConfLabel(i)).c_str(), "w");
      fprintf(files[i], "Array size,");
      int j; for (j=0; j<nAlgos; j++) {
	fprintf(files[i], "%s,", algo_labels[j]);
      }
      fprintf(files[i], "\n");
    }}

  for (int i = 0; i < NUM_EXPERIMENTS; i++) {
    long long stat_array[nAlgos][RUN_TIMES][nStats]; 




    const int order = i / RUNS_PER_ORDER;
    //cout << order << endl;
    const int run_number = (i % RUNS_PER_ORDER)+1;
    // Create random array
    const int size_exp = (int) pow(10,order) + (pow(10,order+1)-pow(10,order))*run_number/RUNS_PER_ORDER;
    
    
    const int arrSize = size_exp + SIZE_MIN;
    
    int* array = new int[arrSize];

    
    
    //  perf.startTimeNow();


    printf("Experiment %d/%d  \t Array size %g \n", (i+1), NUM_EXPERIMENTS, (double)arrSize);




    // Set up algorithms
    {int i;
      for (i=0; i<nAlgos; i++) {
	//cout << "setting up alg: " << algo_labels[i] << endl;
	(*algo_array[i]).setup(arrSize);
      }}

    for (int iStat=0; iStat<nStats; iStat++){
      fprintf(files[iStat], "%d,",arrSize);
    }


    int* oldArray; // ???
    int* newArray; // ???

      
    //perf.perf_event_reset();
    //perf.perf_event_enable();
      
      for (int j = 0; j < RUN_TIMES; j++) {


	const int low = 1 << 9;
	const int high = 1 << 10;
	const int seed = j+1; // TODO
	
	fillArrayWithRandom(array, arrSize, low, high, seed);	


	for (int iAlg=0; iAlg<nAlgos; iAlg++){

	perf.perf_event_reset();
	perf.perf_event_enable();

	//perf.startTimeNow();
	
	// Perform the matrix multiplication:
	//cout << "starting alg: " << algo_labels[iAlg] << endl;
	newArray = (*algo_array[iAlg]).sort(array, arrSize);


	//perf.endTimeNow();
  
	perf.perf_event_disable();
  
	perf.read_all(stat_array[iAlg][j]);

	
	//printf("Sorted array:\n");
	//printArray(newArray, arrSize);
    
	//long long time = perf.getTimeDiff();

	if (iAlg != 0 && !arrayEquals(oldArray, newArray, arrSize)) {
	  cout << "Results for " << algo_labels[iAlg] << " and " << algo_labels[iAlg-1] << " are not equal!" << endl;
	  cout << algo_labels[iAlg-1] << " sorted array: " << endl;
	  printArray(oldArray, arrSize);
	  cout << algo_labels[iAlg] << " sorted array: " << endl;
	  printArray(newArray, arrSize);
	  delete [] oldArray;
	}
	if (iAlg < nAlgos) {oldArray = newArray;}
	
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

}


void fillArrayWithRandom(int * array, int size, int low, int high, int seed)
{
  srand(seed + (unsigned) time(0));
    for(int i=0; i < size; i++){ 
        array[i] = rand() % ( (high+1) - low ) + low;
  } 
}

void printArray(int* array, int arrSize) 
{
  printf("[");  
  for (int i = 0; i < arrSize; i++) {
    printf(" %d ", array[i]);
    if (i != arrSize - 1) {
      printf("|");
    }  
  }
  printf("]\n");
}

bool arrayEquals(int* oldArray, int* newArray, int arrSize){
  for (int i=0; i < arrSize; i++) {
    if (oldArray[i] != newArray[i]) return false;
  }
  return true;
}
