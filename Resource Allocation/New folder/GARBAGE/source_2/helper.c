
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




