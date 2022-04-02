int bestFit(int size, int* memory, int memorySize){
    int bestBlockIndex = -1;
    int bestBlockSize = INT_MAX;
    int currBlockSize = 0;
    for(int i = 0;i< memorySize;i+=1){
        if(memory[i] == 0){
            currBlockSize+=1;
        }
        else{
            if(bestBlockSize>=currBlockSize && currBlockSize>=size){
                bestBlockSize = currBlockSize;
                bestBlockIndex = i-currBlockSize;
            }
            currBlockSize = 0;
        }
    }
    if(bestBlockSize>=currBlockSize && currBlockSize>=size){
        bestBlockSize = currBlockSize;
        bestBlockIndex = memorySize-currBlockSize;
    }
    return bestBlockIndex;
}