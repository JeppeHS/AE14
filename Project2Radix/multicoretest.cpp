#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstring>

#define NUM_OF_THREADS 2
const int arrSize = 10;

struct threadInfo1 {  
  int threadNum;       
  int startIdx;
  int endIdx;
  int* bucketArray;
  int numOfBuckets;
};

//void task(int id);
//void *threaded_task(void *t);
static int* outputArray;
static pthread_t *threads = new pthread_t[NUM_OF_THREADS];
static threadInfo1 *tInfos1 = new threadInfo1[NUM_OF_THREADS];

static void task(int id);
static void *threaded_task(void *t);

class multicoretest {
public:




  /* Run 'task' num_tasks times serially. */
  void serial(int num_tasks) {
    int i;
    for (i = 0; i < num_tasks; i++) {
      task(i);
    }
  };

  /* Run 'task' num_tasks times, creating a separate thread for each
     call to 'task'. */
  void parallel(int num_tasks)
  {
    outputArray = new int[arrSize];
  
    int threadRes;
    for (int i = 0; i < NUM_OF_THREADS; i++) {
      printf("Creating thread %d\n", i);
  
      tInfos1[i].threadNum = i;
      tInfos1[i].startIdx = i;
      tInfos1[i].endIdx = i;
      tInfos1[i].bucketArray = new int[i];
      tInfos1[i].numOfBuckets = i;

      threadRes = pthread_create(&threads[i], NULL, threaded_task, (void *) &tInfos1[i]);
      if (threadRes) {
	printf("ERROR: return code from pthread_create() is %d\n", threadRes);
	exit(-1);
      }
    } 

    /*
      int rc;
      long t;
      for (t = 0; t < NUM_OF_THREADS; t++) {
      printf("Creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, threaded_task, (void *)t);
      if (rc) {
      printf("ERROR: return code from pthread_create() is %d\n", rc);
      exit(-1);
      }
      }
    */
  };

  void *print_usage(int argc, char *argv[]) {
    printf("Usage: %s serial|parallel num_tasks\n", argv[0]);
    exit(1);
  };


};

/*int main(int argc, char *argv[]) {

  multicoretest mcTest = multicoretest();
  mcTest.parallel(NUM_OF_THREADS);

  printf("Main completed\n");
  pthread_exit(NULL);
  }*/

  /* A task that takes some time to complete. The id identifies distinct
     tasks for printed messages. */
  static void task(int id) {
    printf("Task %d started\n", id);
    int i;
    double result = 0.0;
    for (i = 0; i < 100000000; i++) {
      result = result + sin(i) * tan(i);
      if (i%1000 == 0) outputArray[id] = result;
    }
  
    printf("Task %d completed with result %e\n", id, result);
  };


  /* Same as 'task', but meant to be called from different threads. */
  static void *threaded_task(void *t) {
    struct threadInfo1 *tInfo = (struct threadInfo1 *) t;
    int id = tInfo->threadNum;
    printf("Thread %d started\n", id);
    task(id);
    printf("Thread %d done\n", id);
    pthread_exit(0);
  };
