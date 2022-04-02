void spawnDetectionThread(){
    push(&timeLog, time(NULL)); // to keep track of starting time
    prompt("Spawning detection Thread.........");
    pthread_t detectionThreadId;
    int fd = pthread_create(&detectionThreadId, NULL, deadlockDetection, NULL);
    if(fd != 0) error("Deadlock Detection thread cannot be spawned", 1);
}



void * deadlockDetection(){
    while(1){
        sleep(DETECTION_INTERVAL); // to give time interval detection of deadlock
        LOCK;
        prompt("Looking for deadlock");
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
                    error("Deadlock detected......\nThe deadlocked Threads are", 0);
                    printArray(deadlockedThreads, countDeadlockedThreads);
                    printFlag = 0;
                }
                
                int terminatedThreadIndex = resolve(finish, deadlockedThreads);
                killed[terminatedThreadIndex] = 1;
                deallocate(terminatedThreadIndex);
                continue;
            }
            if(printFlag)
                success("No Deadlock detected.");
            else
                success("Deadlock Resolved.");
            break;
            
        }
        for(int i = 0;i<THREAD_COUNT;i++){
            if(killed[i]) resetThread(i, 0);
        }
        UNLOCK;
    }
}