void deallocate(int * memory, int start, int end){
    for(int i = start; i < end; i+=1){
        memory[i] = 0; // deallocating a memory block
    }
}