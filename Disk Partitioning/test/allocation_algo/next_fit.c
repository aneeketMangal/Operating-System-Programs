#include <stdio.h>
#include <limits.h>

#include "../../source/allocation_algo/best_fit.c"
#include "../../source/allocation_algo/first_fit.c"
#include "../../source/allocation_algo/next_fit.c"
#include "../../source/allocation_algo/allocate.c"

int main(){
    int memory[15] = {1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1}; // creating a memory of 10 blocks
    int response = allocate(2, memory, 15, 3); // applying next fit algorithm
    memory[1] = 0;
    memory[2] = 0;
    response = allocate(2, memory, 15, 3);
    if(response == 1){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}