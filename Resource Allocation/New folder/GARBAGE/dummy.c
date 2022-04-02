
void * printing(){
    for(int i = 0;i<50;i++){
        sleep(2);
        printf("available\n");

        for (int i = 0; i < RESOURCE_COUNT; i++)
        {
            printf("%d\t", available[i]);
            fflush(stdout);
        }
        printf("\n");
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            for (int j = 0; j < RESOURCE_COUNT; j++)
            {
                printf("%d\t", allocation[i][j]);
                fflush(stdout);
            }
            printf("\n");
        }

        for (int i = 0; i < THREAD_COUNT; i++)
        {
            
            printf("\n%d-%d-%d\n\n", state[i], currRequest[i].index, currRequest[i].volume);
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