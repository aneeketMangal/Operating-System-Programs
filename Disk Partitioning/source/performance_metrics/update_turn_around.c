void updateTurnAround(Metrics* metrics, long int start, long int end){
    // if(metrics->processServed!= 0)
    // printf("curr: %0.2f\n", metrics->totalTurnAroundTime/(double)metrics->processServed);
    metrics->totalTurnAroundTime += (end-start);
    metrics->processServed+=1;
}