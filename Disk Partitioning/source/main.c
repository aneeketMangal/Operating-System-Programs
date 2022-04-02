#include "main.h"

ConstantSet constants; // struct of all constants used in the program
struct RequestQueue* requestQueue; // queue of waiting requests
int * memory; // memory
Metrics* metrics; // data structure maintained to calculate various matrices for the program execution

pthread_mutex_t memoryMutex = PTHREAD_MUTEX_INITIALIZER; // mutex for memory resource

pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER; // mutex for request queue
pthread_cond_t queueCond = PTHREAD_COND_INITIALIZER;

#define LOCK_MEMORY pthread_mutex_lock(&memoryMutex);
#define UNLOCK_MEMORY pthread_mutex_unlock(&memoryMutex);

void * process(void * D);
void * processSimulator();
void * processGenerator(void * rate);


int main(int argc, char *argv[])
{
    if (argc < 7){
        error("usage: [p] [q] [m] [n] [t] [allocation type] [T]", 1);    
    } 
    srand(time(0));
    
    int T; // execution time of the program
    pthread_t generatorThread; // this thread generates processes at processArrivalRate r;
    pthread_t interruptThread; // this thread exits after T time
    pthread_t simulatorThread; // this process looks for waiting processes and allocates memory to them

    constants.p = atoi(argv[1]);
    constants.q = atoi(argv[2]); //space allocated to OS
    constants.n = atoi(argv[3]);
    constants.m = atoi(argv[4]);
    constants.t = atoi(argv[5]);
    constants.allocationType = atoi(argv[6]);
    T = atoi(argv[7]);
    metrics = malloc(sizeof(Metrics));
    metrics->totalTurnAroundTime = 0;
    metrics->processServed = 0;
    metrics->memoryUtilization = 0.0;
    metrics-> processGenerated = 0;
    
    requestQueue = buildQueue(); // q linked-list based queue for waiting processes

    memory = malloc(constants.p*sizeof(int));
    for(int i = 0;i<constants.p;i++){ memory[i] = 0;}
    for(int i = 0;i<constants.q;i++){ memory[i] = 1;}
    success("Program has started execution");
    printf("Wait for the execution to finish.....\n");
    pthread_create(&generatorThread, NULL, processGenerator, (void*)&constants.n);
    pthread_create(&simulatorThread, NULL, processSimulator, NULL);
    pthread_create(&interruptThread, NULL, interruptHandler, (void*)&T);
    pthread_join(interruptThread, NULL);
    calculateMemoryUtilization(memory, metrics, constants.p);
    printf("Memory Utilisation     : %f%%\n", metrics->memoryUtilization);
    printf("Average turnaround time: %f\n", averageTurnAround(metrics));
    return 0;
}

void * process(void * D)
{
    struct RequestBody *currRequest = (struct RequestBody *)D;
    sleep(currRequest->duration);
    LOCK_MEMORY; //locking memory to prevent two threads to change data at the same time
    deallocate(memory, currRequest->alloc_s, currRequest->alloc_s+ currRequest->size);
    UNLOCK_MEMORY;
    free(currRequest);
    pthread_exit(NULL);
    return NULL;
}

void * processSimulator()
{
    struct RequestBody currRequest;
    pthread_t currProcess;
    while(1){
        pthread_mutex_lock(&queueMutex);
        currRequest = dequeue(requestQueue);
        if (currRequest.valid == 0){
            pthread_cond_wait(&queueCond, &queueMutex); // waiting until there is a incoming process
            currRequest = dequeue(requestQueue);
        }
        pthread_mutex_unlock(&queueMutex);
        if(currRequest.valid == 1){  // a unprocessed process has been chosen
            int allocStatus; // to keep a track if the allocation was done
            while(1){
                LOCK_MEMORY;
                allocStatus = allocate(currRequest.size, memory, constants.p, constants.allocationType);
                UNLOCK_MEMORY;
                if(allocStatus!=-1){ // if memory space is available
                    currRequest.alloc_s = allocStatus;
                    break;
                }      
            }
            updateTurnAround(metrics, currRequest.time_s, clock());
            int fd = pthread_create(&currProcess, NULL, process, (void *)heapRequest(currRequest));
        }
    }
}

void * processGenerator(void * r)
{
    int R = *(int *) r;
    int processArrivalRate;
    while(1){
        processArrivalRate = generateRandom(0.1*R, 1.2*R, 1);
        for(int i= 0;i<processArrivalRate;i++){
            struct RequestBody newRequest = generateRandomRequest(constants.m, constants.t);
            pthread_mutex_lock(&queueMutex);
            enqueue(requestQueue, newRequest); //putting request in the queue
            // printf("done");
            pthread_cond_signal(&queueCond); // sending a signal to processSimulator that there is a new 
            pthread_mutex_unlock(&queueMutex);
        }
        sleep(1);
    }
}

