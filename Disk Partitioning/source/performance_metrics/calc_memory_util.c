void calculateMemoryUtilization(int * memory, Metrics * metrics, int memorySize){
    int used = 0;
    for(int i = 0;i<memorySize;i++){
        if(memory[i]) used+=1;
    }
    metrics->memoryUtilization = ((double)used/(double)memorySize)*100;
}