void * interruptHandler(void * T){
    int timePeriod = *(int *)T;
    sleep(timePeriod);
    pthread_exit(NULL);
}