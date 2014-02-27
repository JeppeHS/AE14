#include <iostream>
#include "vEBBinarySearch.h"
#include <math.h>

#include <time.h>
static timespec startTime, endTime;
static timespec diff(timespec start, timespec end);
void start();
void stop(char const* label);

using std::cout;
using std::endl;

static int linIndex;
static int size;
static int depth;
static int* vebArray;
static int* linArray;

int msb(int v);
unsigned int msb32(unsigned int v);
int findPosition(unsigned int x);

  void insert(int vebIndex, int atDepth, int depthIndex, int roots[]);

  // Create the vEB structure (eg.): {1, 2,3, 4,5,6, 7,8,9, 10,11,12, 13,14,15}
  //                             --> {8, 4,12, 2,1,3, 6,5,7, 10,9,11, 14,13,15}
void vEBBinarySearch::createDataStructure(int* arr, int arrSize){
    size = arrSize;
    delete [] vebArray;
    vebArray = new int[size];
    linArray = arr;

    //remove_this printing stuff
    /*    {int i;
    for (i=0;i<size;i++) {
     cout << linArray[i] << "\t";
    }}
    cout << "\n";
    */

    linIndex = 0;
    depth = msb32(size); // findLeftmostOne(findLeftmostOne(size))
    //cout << depth << endl;
    int roots[depth];
    {for (int i=0; i<depth; i++){
	roots[i] = 0;
      }}
    insert(0, 0, 0, roots);
    
    //remove_this printing stuff
    /*int i;
    for (i=0;i<size;i++) {
      cout << vebArray[i] << "\t";
    }
    cout << "\n";
    */
  };



// Binary search in a vEB array. ************************************************
int vEBBinarySearch::binSearch(int elem){
  //start();
  int vebIndex = 0;
  int curr = vebArray[vebIndex]; // The current node visited
  int res = 0; // The latest element smaller than 'elem'.
  int atDepth = 0;
  int depthIndex = 0;

  int roots[depth];
  {for (int i=0; i<depth; i++){
      roots[i] = 0;
    }}
  //stop("Init");
    
  while (curr != elem){
    //cout << "curr="<<curr<<", i="<<i<<"\n"; //remove_this
    /*start();
    int rootIndex0 = msb((~atDepth+1) & atDepth); // findRightmostOne(atDepth)
    int depthType0 = msb(~atDepth & (atDepth+1)); // findRightmostZero(atDepth)
    stop("msb");*/
    //start();
    //int rootIndex = msb32((~atDepth+1) & atDepth); // findRightmostOne(atDepth)
    //int depthType = msb32(~atDepth & (atDepth+1)); // findRightmostZero(atDepth)
    //stop("msb32");
    //start();
    int rootIndex = findPosition((~atDepth+1) & atDepth); // findRightmostOne(atDepth)
    int depthType = findPosition(~atDepth & (atDepth+1)); // findRightmostZero(atDepth)
    //stop("findPosition");
    
    //start();
    {for (int i=0; i<=rootIndex; i++){
	roots[i] = vebIndex;
      }}

    // Determine if we should go to left or right child.
    int goRight = curr < elem;
    //stop("roots+");
    

    //start();
    //vebIndex = (2*(depthIndex % (int) pow(2, pow(2,depthType)-1 )) +1 + goRight) * (pow(2, pow(2, depthType)) -1) + roots[depthType];
    //int amount = (int)pow(2, pow(2,depthType)-1 );
    int amount = 1<<((1<<depthType)-1);
    //stop("amount");
    // compare amount-calculation and the new vebIndex. Is amount the bottleneck?
    //start();
    //vebIndex = (2*(depthIndex % amount) +1 + goRight) * (2*amount -1) + roots[depthType];
    vebIndex = (2*(depthIndex & (amount-1)) +1 + goRight) * (2*amount -1) + roots[depthType];
    //stop("vebIndex");

    //start();
    if (goRight){
      res = curr; // This is now the latest known element smaller than 'elem'.
    }


    //cout << endl;
    // If we have reached a bottom node, return the last element lower than 'elem'.
    if (vebIndex >= size || vebIndex < 0){
      //cout << "i>=size: "<<i<<">="<<size<<"\n"; //remove_this
      //cout << endl;
      return res;
    }
      
    atDepth++;
    depthIndex = 2*depthIndex + goRight;
    curr = vebArray[vebIndex];
    
    //stop("Rest");
    //start();
    //stop("Dummy");
    //cout << endl;
  }

  // At this point, the while loop did not continue because curr==elem
  return curr;
}


// Recursive method for filling out the veb tree.
void insert(int vebIndex, int atDepth, int depthIndex, int roots[]){
  if (vebIndex >= size || vebIndex < 0) {
    //cout << "vInd = " << vebIndex << ", returning..." << endl;
    return;
  }
  int rootIndex = findPosition((~atDepth+1) & atDepth); // findRightmostOne(atDepth)
  int depthType = findPosition(~atDepth & (atDepth+1)); // findRightmostZero(atDepth)
  /*
    cout << "vInd = " << (vebIndex) << "\t";
    cout << "atDep  = " << atDepth << "\t";
    cout << "depInd = " << depthIndex << "\t";
    cout << "dType = " << depthType << "\t";
    cout << "rInd = " << rootIndex << "\t";
    cout << "root = " << (roots[depthType]) << "\t";
    cout << endl;
  */
    
  int rootsNew[depth];
  {for (int i=0; i<depth; i++){
      if (i <= rootIndex) rootsNew[i] = vebIndex;
      else rootsNew[i] = roots[i];
      //	cout << rootsNew[i] << "\t";
      }}
  //cout << endl;

    //int depthTypeNext = msb(~(atDepth+1) & (atDepth+2));
    //cout << "Next root: " << rootsNew[depthTypeNext] << endl;
    //cout << "First factor = " << (2*(depthIndex % (int) pow(2, pow(2,depthType)-1 )) +1) << endl;
    //cout << "Second factor = " <<  (pow(2, pow(2, depthType)) -1) << endl;


   int amount = 1<<((1<<depthType)-1);
   int vebLeft = (2*(depthIndex & (amount-1)) +1) * (2*amount -1) + rootsNew[depthType];

  //    if (vebLeft < vebIndex) cout << "vebLeft ("<<vebLeft<<") < vebIndex ("<<vebIndex<<")"<<endl;

  insert(vebLeft, atDepth+1, 2*depthIndex, rootsNew); // Left child
    
  vebArray[vebIndex] = *(linArray+linIndex);
  linIndex++;

  int vebRight = vebLeft + 2*amount -1;
  insert(vebRight, atDepth+1, 2*depthIndex+1, rootsNew); // Right child
}

int findPosition(unsigned int x){
  static const int hashPos[37] = {0, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4, 7, 17, 0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5, 20, 8, 19, 18};
  return hashPos[x % 37];
}

unsigned int msb32(unsigned int v){
  static const char LogTable256[256] = 
    {
#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
      -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
      LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
      LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
    };

  unsigned r;     // r will be lg(v)
  register unsigned int t, tt; // temporaries

  if (tt = v >> 16)
    {
      r = (t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt];
    }
  else 
    {
      r = (t = v >> 8) ? 8 + LogTable256[t] : LogTable256[v];
    }
  
  return r;
}

/*unsigned int msb32(unsigned int v){
  v |= (v >> 1);
  v |= (v >> 2);
  v |= (v >> 4);
  v |= (v >> 8);
  v |= (v >> 16);
  return (v & ~(v >> 1));
  }*/

int msb(int v){
  int r = 0;
  while (v >>= 1) {
    r++;
  }
  return r;
}


timespec diff(timespec start, timespec end)
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

void start(){
  startTime.tv_sec = 0;
  startTime.tv_nsec = 0;
  endTime.tv_sec = 0;
  endTime.tv_nsec = 0;			    
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTime);
}

void stop(char const* label){
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTime);
  timespec diffTime = diff(startTime, endTime);
  long long nanoTime = 1000000000*diffTime.tv_sec + diffTime.tv_nsec;
  printf("%s: %g\t", label, (double)nanoTime);
}
