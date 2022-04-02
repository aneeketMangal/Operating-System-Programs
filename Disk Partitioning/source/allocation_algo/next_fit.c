int j = 0;
int nextFit(int size, int* memory, int memorySize){
    int nextBlockIndex = -1;
    int currBlockSize = 0;
    for(int i = j;i< memorySize;i+=1){
        if(memory[i] == 0){
            currBlockSize+=1;
        }
        else{
            if(currBlockSize>=size){
                nextBlockIndex = i-currBlockSize;
                break;
            }
            currBlockSize= 0;
        }
    }
    if(nextBlockIndex == -1 && currBlockSize>=size){
        nextBlockIndex = memorySize-currBlockSize;
    }
    currBlockSize = 0;
    if(nextBlockIndex == -1){
        for(int i = 0;i< j;i+=1){
            if(memory[i] == 0){
                currBlockSize+=1;
            }
            else{
                if(currBlockSize>=size){
                    nextBlockIndex = i-currBlockSize;
                    break;
                }
                currBlockSize = 0;
            }
        }
    
        if(nextBlockIndex == -1 && currBlockSize>=size){
            nextBlockIndex = j-currBlockSize;
        }
    }
    if(nextBlockIndex!=-1){
        j = nextBlockIndex+size;
    }
    return nextBlockIndex;
}