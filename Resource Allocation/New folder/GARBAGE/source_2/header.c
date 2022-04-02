#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "pair.c"
#include "list.c"
#include "print.c"
#include "utility.c"


pthread_t * threadPool;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int askForResources = 0;

#define LOCK pthread_mutex_lock(&lock);\
    while(askForResources == 0)\
    { \
        pthread_cond_wait(&cond, &lock);\
    } \
    askForResources = 0; \


#define UNLOCK askForResources = 1; \
    pthread_cond_signal(&cond); \
    pthread_mutex_unlock(&lock); \

int THREAD_COUNT;
int DETECTION_INTERVAL;
int REQUEST_INTERVAL;
int RESOURCE_COUNT;
int HEURISTIC_INDEX;
int * resourceInstances;
int * currAvailable;
int * state; // 0 -> working; 1-> ; 2-> finished;
int ** allocation;
int ** request;
Pair * currRequest;
List * timeLog;

void assignRandomResources(int threadIndex); // utility function to assign random resources to a thread.
void resetThread(int threadIndex, int first); // function to reset a thread.
void deallocate(int threadIndex); // deallocate thre resources taken by a thred.
void* workerThreadLogic(void * index); 
void spawnWorkerThreads();
void * deadlockDetection();
void spawnDetectionThread();
int terminateMaxSum(int* sumOfAcquiredResources);
int terminateMinSum(int* sumOfAcquiredResources);
int terminateRandom(int* sumOfAcquiredResources);
int resolve(int * isDeadlocked, int * deadlockedThreads);
