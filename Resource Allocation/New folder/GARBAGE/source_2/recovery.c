

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