#include "header.c"
#include "helper.c"
#include "worker.c"
#include "recovery.c"
#include "detection.c"

int main(int argc, char *argv[])
{
    if (argc < 5){
        error("usage [thread_count] [detection interval] [resource count] [heuristic technique index] [max instances of resources]", 1);    
    } 
    
    // CLI input
    THREAD_COUNT = atoi(argv[1]);
    DETECTION_INTERVAL = atoi(argv[2]);
    RESOURCE_COUNT = atoi(argv[3]);
    HEURISTIC_INDEX = atoi(argv[4]);
    REQUEST_INTERVAL = 5000000;
    askForResources = 1;

    threadPool = malloc(THREAD_COUNT*sizeof(pthread_t));
    currAvailable = (int*) malloc(RESOURCE_COUNT * sizeof(int));
    resourceInstances = (int*) malloc(RESOURCE_COUNT * sizeof(int));
    state = (int*) malloc(THREAD_COUNT * sizeof(int));
    for (int i = 0;i<RESOURCE_COUNT;i++){
        currAvailable[i] = atoi(argv[i+5]);
        resourceInstances[i] = currAvailable[i];
    }

    for (int i = 0;i<THREAD_COUNT;i++){ //initialising state of each thread to be 0 
        state[i] = 0;
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
        error("mutex cannot be initialised", 1);


    spawnWorkerThreads();
    spawnDetectionThread();
    int interrupt; scanf("%d", &interrupt); // program will wait for a user input to 
    // TODO: ANALYSIS SOS
    printList(timeLog);
    return 0;
}

