#include <stdio.h>
#include <limits.h>
#include "../../source/deallocate.c"

int main(){
    int memory[4] = {1, 1, 1, 1}; // creating a memory of 4 blocks
    deallocate(memory, 1, 3); // applying worst fit algorithm
    if(memory[0] == 1 && memory[1] == 0 && memory[2] == 0 && memory[3] == 1){ 
        //checking if array is {1, 0, 0,, 1}
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}