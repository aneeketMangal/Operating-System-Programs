int allocate(int size, int* memory, int memorySize, int allocationType){
    int startIndex; 
    switch(allocationType){
        case 1:
            startIndex = firstFit(size, memory, memorySize);
            break;
        case 2:
            startIndex = bestFit(size, memory, memorySize);
            break;
        case 3:
            startIndex = nextFit(size, memory, memorySize);
            break;

    }
    if(startIndex == -1) return -1;
    for(int i = startIndex;i<startIndex+size;i++){
        memory[i] = 1; // occupying the memory block
    }
    return startIndex;
}