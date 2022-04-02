
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
    prompt("Spawning Worker Threads.........");
    for (int i = 0;i<THREAD_COUNT;i++){
        int *j = malloc(sizeof(*j));
        *j = i;
        int fd = pthread_create(&threadPool[i], NULL, workerThreadLogic, (void *)j);
        if(fd != 0) error("Some worker thread cannot be spawned", 1);
    }
}
