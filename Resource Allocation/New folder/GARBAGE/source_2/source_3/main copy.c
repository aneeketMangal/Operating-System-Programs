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
pthread_t detectionThread;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int askForResources;
int HEURISTIC_COUNT = 3;

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
FILE *fptr;

int * resourceInstances;
int * currAvailable;
int * state; // 0 -> working; 1-> ; 2-> finished;
int ** allocation;
int ** request;
Pair * currRequest;
List * timeLog;
void * interruptHandler();
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
const char * heuristicName();
int resolve(int * isDeadlocked, int * deadlockedThreads);


int main(int argc, char *argv[])
{
    if (argc < 4){
        error(fptr, "usage [thread_count] [detection interval] [resource count] [max instances of resources]", 1, 1);    
    } 
    
    // CLI input
    THREAD_COUNT = atoi(argv[1]);
    DETECTION_INTERVAL = atoi(argv[2]);
    RESOURCE_COUNT = atoi(argv[3]);

    REQUEST_INTERVAL = 5000000;
    askForResources = 1;

    threadPool = malloc(THREAD_COUNT*sizeof(pthread_t));
    currAvailable = (int*) malloc(RESOURCE_COUNT * sizeof(int));
    resourceInstances = (int*) malloc(RESOURCE_COUNT * sizeof(int));
    state = (int*) malloc(THREAD_COUNT * sizeof(int));
    for (int i = 0;i<RESOURCE_COUNT;i++){
        currAvailable[i] = atoi(argv[i+4]);
        resourceInstances[i] = currAvailable[i];
    }
    
    allocation = (int**)malloc(THREAD_COUNT * sizeof(int*));
    for (int i = 0; i < THREAD_COUNT; i++)
        allocation[i] = (int*)malloc(RESOURCE_COUNT * sizeof(int));
    
    request = (int**)malloc(THREAD_COUNT * sizeof(int*));
    for (int i = 0; i < THREAD_COUNT; i++)
        request[i] = (int*)malloc(RESOURCE_COUNT * sizeof(int));

    currRequest = (Pair*)malloc(THREAD_COUNT * sizeof(Pair));

    //initialising the mutex
    if (pthread_mutex_init(&lock, NULL) != 0) 
        error(fptr, "mutex cannot be initialised", 1, 1);
    
    // for (int a = 2;a<=HEURISTIC_COUNT;a++){
        fptr = fopen("op","a");
        HEURISTIC_INDEX = 3;
        pthread_t temp;
        push(&timeLog, time(NULL));
        spawnWorkerThreads();
        spawnDetectionThread();
        pthread_create(&temp, NULL, interruptHandler, NULL);
        pthread_join(temp, NULL);

    // }
    return 0;
}

const char * heuristicName(){
    switch(HEURISTIC_INDEX){
        case 1:
            return "RANDOM";
        case 2:
            return "MINSUM";
        default:
            return "MAXSUM";
    }
}

void assignRandomResources(int threadIndex){
    for(int i  = 0; i<RESOURCE_COUNT;i++){
        request[threadIndex][i] = generateRandomInteger(0, resourceInstances[i]);
    }
}


void resetThread(int threadIndex, int first){
    if(!first){
        int timeGap = generateRandomInteger((int)0.7*DETECTION_INTERVAL*1000000, (int)1.5*DETECTION_INTERVAL*1000000); // ! generates a random integer between 0 to 3
        usleep(timeGap);    
    }
    assignRandomResources(threadIndex);
    state[threadIndex] = 0;
    currRequest[threadIndex].index = 0;
    currRequest[threadIndex].volume = 0;
}

void deallocate(int threadIndex){
    for(int i = 0;i<RESOURCE_COUNT;i++){
        currAvailable[i]+= allocation[threadIndex][i];
        allocation[threadIndex][i] = 0;
        currRequest[threadIndex].index = 0;
        currRequest[threadIndex].volume = 0;
        request[threadIndex][i] = 0;
    }
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
        if(isFinished) state[threadIndex] = 2;

        if(state[threadIndex] == 0){
            int timeGap = generateRandomInteger(REQUEST_INTERVAL/10, REQUEST_INTERVAL); // ! generates a random integer between 0 to 3
            usleep(timeGap);  // ! make the thread suspend for timeGap second
            int randomResourceIndex;
            // TODO: improve this right here
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
        if(state[threadIndex] == 2){ // ! killing the thread
            LOCK;
            deallocate(threadIndex);
            UNLOCK;
            resetThread(threadIndex, 0); // reassigning resource requirements to thread
        }  
        continue; 
    }
}

void spawnWorkerThreads(){
    prompt(fptr, "Spawning Worker Threads.........", 1);
    for (int i = 0;i<THREAD_COUNT;i++){
        int *j = malloc(sizeof(*j));
        *j = i;
        int fd = pthread_create(&threadPool[i], NULL, workerThreadLogic, (void *)j);
        if(fd != 0) error(fptr, "Some worker thread cannot be spawned", 1, 1);
    }
}


void spawnDetectionThread(){
    prompt(fptr, "Spawning detection Thread.........", 1);
    int fd = pthread_create(&detectionThread, NULL, deadlockDetection, NULL);
    if(fd != 0) error(fptr, "Deadlock Detection thread cannot be spawned", 1, 1);
}

void * interruptHandler(){
    int interrupt;
    while(1){
        scanf("%d", &interrupt);
        printf("\ndsfadfsa\n");
        pthread_cancel(detectionThread);
        for(int i = 0;i<THREAD_COUNT;i++){
            pthread_cancel(threadPool[i]);
        }
        printf("Average Time taken between deadlocks using %s heuristic:-> %f\n", heuristicName(), calculateAverage(timeLog));
        free(timeLog);
        
        break;
    }
}

void * deadlockDetection(){
    while(1){
        sleep(DETECTION_INTERVAL); // to give time interval detection of deadlock
        LOCK;
        prompt(fptr, "Looking for deadlock.....", 0);
        int printFlag = 1;
        int killed[THREAD_COUNT];
        for(int i = 0;i<THREAD_COUNT;i++) {
            killed[i] = 0;
        }

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

            int countDeadlockedThreads = 0;
            for(int i = 0;i<THREAD_COUNT;i++){
                if(finish[i] == 0) countDeadlockedThreads += 1;
            }


            if(countDeadlockedThreads > 0){
                int deadlockedThreads[countDeadlockedThreads];
                int j = 0;
                for(int i = 0;i<THREAD_COUNT;i++){
                    if(finish[i] == 0) deadlockedThreads[j++] = i;
                }
                if(printFlag){
                    push(&timeLog, time(NULL));
                    error(fptr, "Deadlock detected......\nThe deadlocked Threads are", 0, 1);
                    error(fptr, "Deadlock detected......\nThe deadlocked Threads are", 0, 0);
                    printArray(fptr, deadlockedThreads, countDeadlockedThreads, 0);
                    printArray(fptr, deadlockedThreads, countDeadlockedThreads, 1);
                    printFlag = 0;
                }
                
                int terminatedThreadIndex = resolve(finish, deadlockedThreads);
                killed[terminatedThreadIndex] = 1;
                deallocate(terminatedThreadIndex);
                continue;
            }
            if(printFlag)
                success(fptr, "No Deadlock detected.", 0);
            else
                success(fptr, "Deadlock Resolved.", 1);
            break;
            
        }
        for(int i = 0;i<THREAD_COUNT;i++){
            if(killed[i]) resetThread(i, 0);
        }
        UNLOCK;

    }
}


int terminateMaxSum(int* sumOfAcquiredResources){
    int maximumVal = 0;
    int maximumValIndex = 0;
    for (int i = 0;i<THREAD_COUNT;i++){
        if(sumOfAcquiredResources[i] != -1 && sumOfAcquiredResources[i] >= maximumVal){
            sumOfAcquiredResources[i] = maximumVal;
            maximumValIndex = i;
        }
    }

    return maximumValIndex;
}

int terminateMinSum(int* sumOfAcquiredResources){
    int minimumVal = INT_MAX;
    int minimumValIndex = 0;
    for (int i = 0;i<THREAD_COUNT;i++){
        if(sumOfAcquiredResources[i] != -1 && sumOfAcquiredResources[i] <= minimumVal){
            sumOfAcquiredResources[i] = minimumVal;
            minimumValIndex = i;
        }
    }

    return minimumValIndex;
}

int terminateRandom(int* deadlockedThreads){
    int randomThreadIndex = generateRandomInteger(0, sizeof(deadlockedThreads)/sizeof(int));
    return randomThreadIndex;
}



int resolve(int * isDeadlocked, int * deadlockedThreads){
    int sumOfAcquiredResources[THREAD_COUNT]; // finding sum of acquired resources by all threads
    for(int i = 0;i<THREAD_COUNT;i++){        // sum = -1 implies that given thread is not deadlocked
        if(isDeadlocked[i] == 1)
            sumOfAcquiredResources[i] = -1;
        else{
            for(int j = 0;j<RESOURCE_COUNT;j++){
                sumOfAcquiredResources[i] += allocation[i][j];
            }
        }
    }
    
    switch(HEURISTIC_INDEX){
        case 1:
            return terminateRandom(deadlockedThreads);
        case 2:
            return terminateMinSum(sumOfAcquiredResources);
        default:
            return terminateMaxSum(sumOfAcquiredResources);
    }

}