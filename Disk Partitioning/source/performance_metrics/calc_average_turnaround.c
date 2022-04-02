double averageTurnAround(Metrics* metrics){
    return ((double)metrics->totalTurnAroundTime/(double)metrics->processServed)/(double)CLOCKS_PER_SEC;
}