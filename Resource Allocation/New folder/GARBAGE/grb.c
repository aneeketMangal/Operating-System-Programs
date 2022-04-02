#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include "error.h"
#define max(X, Y) (((X) > (Y)) ? (X) : (Y))


int THREAD_COUNT;
int DETECTION_INTERVAL;
int RESOURCE_COUNT;

int * maxAvailable;
int * available;
int ** allocation;
int ** request;
int ** currRequest;

pthread_t * threadPool;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int askForResources = 0;

/* 
    utility function to generate a random integer 
    between lowerLimit and UpperLimit (inclusive)
*/
int generateRandomInteger(int lowerLimit, int upperLimit){
    return (rand()%(1+upperLimit-lowerLimit))+lowerLimit;
}
/*
    utility function to assign random resources 
    to a thread.
*/


void assignRandomResources(int threadIndex){
    for(int i  = 0; i<RESOURCE_COUNT;i++){
        request[threadIndex][i] = generateRandomInteger(0, maxAvailable[i]);
    }
}


void* workerThreadLogic(void * index){
    
    int threadIndex = *((int*) index);
    assignRandomResources(threadIndex);

    while(1){
        int timeGap = generateRandomInteger(0, 3); // ! generates a random integer between 0 to 1000 
        sleep(timeGap);  // ! make the thread suspend for timeGap second
        int randomResourceIndex = generateRandomInteger(1, RESOURCE_COUNT)-1;


        pthread_mutex_lock(&lock);
        while(askForResources == 0){
            pthread_cond_wait(&cond, &lock);
        }
        askForResources = 0;
        if(available[randomResourceIndex]>0 && request[threadIndex][randomResourceIndex]>0){  // ! acquiring resource instance if its available
            available[randomResourceIndex]-=1;
            allocation[threadIndex][randomResourceIndex]+=1;
            request[threadIndex][randomResourceIndex]-=1;
        }
        askForResources = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }
    
}


void spawnWorkerThreads(){
    for (int i = 0;i<THREAD_COUNT;i++){
        int *j = malloc(sizeof(*j));
        *j = i;
        pthread_create(&threadPool[i], NULL, workerThreadLogic, (void *)j);
    }
}

int compare(int * a, int * b, int sizeOfArrays){
    for(int i = 0;i< sizeOfArrays;i++){
        if(a[i]<b[i]) return 0;
    }
    return 1;
}

void * deadlockDetection(){
    while(1){
        sleep(DETECTION_INTERVAL);
        printf("Icame\n");
        // TODO: detection lock detection;
        pthread_mutex_lock(&lock);
        while(askForResources == 0){
            pthread_cond_wait(&cond, &lock);
        }
        askForResources = 0;
        int work[RESOURCE_COUNT];
        int finish[THREAD_COUNT];
        for(int i = 0;i<THREAD_COUNT;i++) { // * initialisation
            finish[i] = 0;
            work[i] = available[i];
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
            if(finish[i] == 0){
                isUnfinished  = 1;
            }
        }
        if(isUnfinished){
            printf("deadlock\n");
            break;
        }
        else{
            printf("sab chungus si\n");
        }
        askForResources = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }
}

void spawnDetectionThread(){
    pthread_t detectionThreadId;
    int fd = pthread_create(&detectionThreadId, NULL, deadlockDetection, NULL);
    if(fd == 0) {
        // pthread_join(detectionThreadId, NULL); // for thread safety, ensuring that the thread ends
        prompt("Deadlock Detection thread spawned");
    }
    else error("Deadlock Detection thread cannot be spawned", 1);
}



int main(int argc, char *argv[])
{
    
    // ! to seed the random numbers
    srand(time(0));

    if (argc < 4){
        error("usage [thread_count] [detection interval] [resource count] [max instances of resources]", 1);    
    } 
    
    // CLI input
    THREAD_COUNT = atoi(argv[1]);
    DETECTION_INTERVAL = atoi(argv[2]);
    RESOURCE_COUNT = atoi(argv[3]);
    threadPool = malloc(THREAD_COUNT*sizeof(pthread_t));
    
    askForResources = 1;
    available = (int*) malloc(RESOURCE_COUNT * sizeof(int));
    maxAvailable = (int*) malloc(RESOURCE_COUNT * sizeof(int));
    for (int i = 0;i<RESOURCE_COUNT;i++){
        available[i] = atoi(argv[i+4]);
        maxAvailable[i] = available[i];
    }
    
    allocation = (int**)malloc(THREAD_COUNT * sizeof(int*));
    for (int i = 0; i < THREAD_COUNT; i++)
        allocation[i] = (int*)malloc(RESOURCE_COUNT * sizeof(int));
    
    request = (int**)malloc(THREAD_COUNT * sizeof(int*));
    for (int i = 0; i < THREAD_COUNT; i++)
        request[i] = (int*)malloc(RESOURCE_COUNT * sizeof(int));

    currRequest = (int**)malloc(THREAD_COUNT * sizeof(int*));
    for (int i = 0; i < THREAD_COUNT; i++)
        currRequest[i] = (int*)malloc(RESOURCE_COUNT * sizeof(int));

    //initialising the mutex
    if (pthread_mutex_init(&lock, NULL) != 0) 
        error("mutex cannot be initialised", 1);


    spawnWorkerThreads();
    // spawnDummyThread();
    spawnDetectionThread();
    int a;
    scanf("%d", &a);
    return 0;
}
