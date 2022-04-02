#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include "print.c"
#include "pair.c"
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
int RESOURCE_COUNT;
int HEURISTIC_INDEX;

int * resourceInstances;
int * currAvailable;
int * state; // 0 -> working; 1-> ; 2-> finished;
int ** allocation;
int ** request;
Pair * currRequest;




void assignRandomResources(int threadIndex); // utility function to assign random resources to a thread.
void resetThread(int threadIndex, int first); // function to reset a thread.

void* workerThreadLogic(void * index); 
void spawnWorkerThreads();

void * deadlockDetection();
void spawnDetectionThread();

void resolve(){
    
}

int main(int argc, char *argv[])
{
    if (argc < 5){
        error("usage [thread_count] [detection interval (microseconds)] [resource count] [heuristic technique index] [max instances of resources]", 1);    
    } 
    
    // CLI input
    THREAD_COUNT = atoi(argv[1]);
    DETECTION_INTERVAL = atoi(argv[2]);
    RESOURCE_COUNT = atoi(argv[3]);
    HEURISTIC_INDEX = atoi(argv[4]);
    askForResources = 1;

    threadPool = malloc(THREAD_COUNT*sizeof(pthread_t));
    currAvailable = (int*) malloc(RESOURCE_COUNT * sizeof(int));
    resourceInstances = (int*) malloc(RESOURCE_COUNT * sizeof(int));
    state = (int*) malloc(THREAD_COUNT * sizeof(int));
    for (int i = 0;i<RESOURCE_COUNT;i++){
        currAvailable[i] = atoi(argv[i+5]);
        resourceInstances[i] = currAvailable[i];
    }
    for (int i = 0;i<THREAD_COUNT;i++){
        state[i] = 0;
    }
    
    allocation = (int**)malloc(THREAD_COUNT * sizeof(int*));
    for (int i = 0; i < THREAD_COUNT; i++)
        allocation[i] = (int*)malloc(RESOURCE_COUNT * sizeof(int));
    
    request = (int**)malloc(THREAD_COUNT * sizeof(int*));
    for (int i = 0; i < THREAD_COUNT; i++)
        request[i] = (int*)malloc(RESOURCE_COUNT * sizeof(int));

    currRequest = (Pair*)malloc(THREAD_COUNT * sizeof(Pair));
    for (int i = 0; i < THREAD_COUNT; i++){
        currRequest[i].index = 0;
        currRequest[i].volume = 0;
    }

    //initialising the mutex
    if (pthread_mutex_init(&lock, NULL) != 0) 
        error("mutex cannot be initialised", 1);


    spawnWorkerThreads();
    spawnDetectionThread();
    int interrupt; scanf("%d", &interrupt); // program will wait for a user input to 
    return 0;
}


void assignRandomResources(int threadIndex){
    for(int i  = 0; i<RESOURCE_COUNT;i++){
        request[threadIndex][i] = generateRandomInteger(0, resourceInstances[i]);
    }
}


void resetThread(int threadIndex, int first){
    if(first == 0){
        int timeGap = generateRandomInteger((int)0.7*DETECTION_INTERVAL, (int)1.5*DETECTION_INTERVAL); // ! generates a random integer between 0 to 3
        usleep(timeGap);    
    }
    assignRandomResources(threadIndex);
    state[threadIndex] = 0;
}

void* workerThreadLogic(void * index){
    
    int threadIndex = *((int*) index);
    resetThread(threadIndex, 1);

    while(1){
        /*
            checking if all requests have been served
            and killing the thread followed by reassigning a new thread
        */
        int isFinished = 1;
        for(int i = 0;i<RESOURCE_COUNT;i++){
            if(request[threadIndex][i] >0) isFinished = 0;
        }
        if(isFinished) 
            state[threadIndex] = 2;

        if(state[threadIndex] == 0){
            int timeGap = generateRandomInteger(10, 5000000); // ! generates a random integer between 0 to 3
            usleep(timeGap);  // ! make the thread suspend for timeGap second
            int randomResourceIndex;
            while(1){
                randomResourceIndex = generateRandomInteger(1, RESOURCE_COUNT)-1;
                if(request[threadIndex][randomResourceIndex]>0) break;
            }
            int randomResourceVolume = generateRandomInteger(1, request[threadIndex][randomResourceIndex]);
            currRequest[threadIndex].index = randomResourceIndex;
            currRequest[threadIndex].volume = randomResourceVolume;
            state[threadIndex] = 1;
        }

        else if(state[threadIndex] == 1){
            LOCK;
            if(currAvailable[currRequest[threadIndex].index]>= currRequest[threadIndex].volume){  // ! acquiring resource instance if its currAvailable
                currAvailable[currRequest[threadIndex].index] -= currRequest[threadIndex].volume;
                allocation[threadIndex][currRequest[threadIndex].index] += currRequest[threadIndex].volume;
                request[threadIndex][currRequest[threadIndex].index]-=currRequest[threadIndex].volume;
                currRequest[threadIndex].index = 0;
                currRequest[threadIndex].volume = 0;
                state[threadIndex] = 0;
            }        
            UNLOCK;
        }
        if(state[threadIndex] == 2){
            LOCK;
            for(int i = 0;i<RESOURCE_COUNT;i++){
                currAvailable[i]+= allocation[threadIndex][i];
                allocation[threadIndex][i] = 0;
            }
            UNLOCK;
            resetThread(threadIndex, 0); // reassigning resource requirements to thread
        }  // ! killing the thread
        continue; 
    }
}

void spawnWorkerThreads(){
    for (int i = 0;i<THREAD_COUNT;i++){
        int *j = malloc(sizeof(*j));
        *j = i;
        pthread_create(&threadPool[i], NULL, workerThreadLogic, (void *)j);
    }
}


void spawnDetectionThread(){
    pthread_t detectionThreadId;
    int fd = pthread_create(&detectionThreadId, NULL, deadlockDetection, NULL);
    if(fd != 0) error("Deadlock Detection thread cannot be spawned", 1);
}



void * deadlockDetection(){
    while(1){
        sleep(DETECTION_INTERVAL);
        LOCK;
        while(1){
            int work[RESOURCE_COUNT];
            int finish[THREAD_COUNT];
            for(int i = 0;i<THREAD_COUNT;i++) { // * initialisation
                finish[i] = 0;
                work[i] = currAvailable[i]; // initialise work matrix equal to currAvailable
            }
            for(int i = 0;i<THREAD_COUNT;i++) {
                int allZeroes = 1;
                for(int j = 0;j<RESOURCE_COUNT;j++){
                    if(allocation[i][j] > 0){
                        allZeroes = 0;
                    }
                }
                finish[i] = allZeroes;
            }
            /* finding and index which hasn't finished acquiring
                and request[i]<= work
            */

            while(1){
                int temp = -1;
                for(int i = 0;i<THREAD_COUNT;i++) { 
                    if(finish[i] == 0 && compare(work, request[i], RESOURCE_COUNT) == 1){
                        temp = i;
                        break;
                    }
                }
                if(temp!=-1){
                    for(int i = 0;i<RESOURCE_COUNT;i++){
                        work[i]+=allocation[temp][i];
                    }
                    finish[temp] = 1;
                    continue;
                }
                break;
            }
            int isUnfinished = 0;
            for(int i = 0;i<THREAD_COUNT;i++){
                if(finish[i] == 0) isUnfinished = 1;
            }
            if(isUnfinished){
                printf("DeadLock\n");
                break;
            }
            else{
                printf("No DeadLock\n");
                continue;
            }
        }
        UNLOCK;
    }
}
