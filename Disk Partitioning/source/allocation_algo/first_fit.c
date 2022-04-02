int firstFit(int size, int* memory, int memorySize){
    int firstBlockIndex = -1;
    int currBlockSize = 0;
    for(int i = 0;i< memorySize;i+=1){
        if(memory[i] == 0){
            currBlockSize+=1;
        }
        else{
            if(currBlockSize>=size){
                firstBlockIndex = i-currBlockSize;
                break;
            }
            currBlockSize = 0;
        }
    }
    if(firstBlockIndex == -1 && currBlockSize>=size){
        firstBlockIndex = memorySize-currBlockSize;
    }
    return firstBlockIndex;
}