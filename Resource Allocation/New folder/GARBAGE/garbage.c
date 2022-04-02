void * printing(){
    for(int i = 0;i<50;i++){
        usleep(2);
        printf("\n");
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            for (int j = 0; j < RESOURCE_COUNT; j++)
            {
                printf("%d\t", request[i][j]);
            }
            printf("\n");
        }
    }
}

void spawnDummyThread(){
    pthread_t detectionThreadId;
    int fd = pthread_create(&detectionThreadId, NULL, printing, NULL);
    if(fd == 0) {
        pthread_join(detectionThreadId, NULL); // for thread safety, ensuring that the thread ends
        prompt("Dummy thread spawned");
    }
    else error("Deadlock Detection thread cannot be spawned", 1);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <pthread.h>
#include <dlfcn.h>
#include <errno.h>



#include "error.h"
int THREAD_COUNT;
int DETECTION_INTERVAL;
int RESOURCE_COUNT;
// int * RESOURCE_INSTANCES;
int * available;
int ** allocation;
int ** request;
pthread_t * threadPool;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int askForResources = 0;
// TODO: learn about pthread_cond_t
// TODO: learn about pthread_join
// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// int play = 0;

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
        request[threadIndex][i] = generateRandomInteger(0, available[i]);
    }
}


void* workerThreadLogic(void * index){
    
    int threadIndex = *((int*) index);
    assignRandomResources(threadIndex);

    while(1){
        int timeGap = generateRandomInteger(0, 1000); // ! generates a random integer between 0 to 1000 
        usleep(timeGap);  // ! make the thread suspend for timeGap second
        int randomResourceIndex = generateRandomInteger(1, RESOURCE_COUNT)-1;

        if(available[randomResourceIndex]>0){  // ! acquiring resource instance if its available
            available[randomResourceIndex]-=1;
        }
    }
    
}


void spawnWorkerThreads(){
    for (int i = 0;i<THREAD_COUNT;i++){
        int *j = malloc(sizeof(*j));
        *j = i;
        pthread_create(&threadPool[i], NULL, workerThreadLogic, (void *)j);
        pthread_join(threadPool[i], NULL);
    }
}

void * deadlockDetection(){
    while(1){
        sleep(DETECTION_INTERVAL);
        pthread_mutex_lock(&lock);
        askForResources = 0;

        // TODO: detection lock

        pthread_mutex_unlock(&lock);
    }
}

void spawnDetectionThread(){
    pthread_t detectionThreadId;
    int fd = pthread_create(&detectionThreadId, NULL, deadlockDetection, NULL);
    if(fd == 0) {
        pthread_join(detectionThreadId, NULL); // for thread safety, ensuring that the thread ends
        prompt("Deadlock Detection thread spawned");
    }
    else error("Deadlock Detection thread cannot be spawned", 1);
}



int main(int argc, char *argv[])
{
    
    // ! to seed the random numbers
    // ! srand(time(0));

    if (argc < 4){
        printf("usage [thread_count] [detection interval] [resource count] [max instances of resources]\n");    
    } 
    
    // CLI input
    THREAD_COUNT = atoi(argv[1]);
    DETECTION_INTERVAL = atoi(argv[2]);
    RESOURCE_COUNT = atoi(argv[3]);
    threadPool = malloc(THREAD_COUNT*sizeof(pthread_t));
    
    askForResources = 1;
    available = (int*) malloc(RESOURCE_COUNT * sizeof(int));
    for (int i = 0;i<RESOURCE_COUNT;i++){
        available[i] = atoi(argv[i+4]);
        printf("%d\n", available[i]);}
    
    allocation = (int**)malloc(THREAD_COUNT * sizeof(int*));
    for (int i = 0; i < THREAD_COUNT; i++)
        allocation[i] = (int*)malloc(RESOURCE_COUNT * sizeof(int));
    
    request = (int**)malloc(THREAD_COUNT * sizeof(int*));
    for (int i = 0; i < THREAD_COUNT; i++)
        request[i] = (int*)malloc(RESOURCE_COUNT * sizeof(int));


    //initialising the mutex
    if (pthread_mutex_init(&lock, NULL) != 0) 
        error("mutex cannot be initialised", 1);


    spawnDetectionThread();
    spawnWorkerThreads();
    return 0;
}
