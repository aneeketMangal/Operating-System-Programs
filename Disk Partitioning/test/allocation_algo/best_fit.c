#include <stdio.h>
#include <limits.h>

#include "../../source/allocation_algo/best_fit.c"
#include "../../source/allocation_algo/first_fit.c"
#include "../../source/allocation_algo/next_fit.c"
#include "../../source/allocation_algo/allocate.c"


int main(){
    int memory[10] = {0, 0, 1, 1, 1, 1, 0, 0, 0, 1}; // creating a memory of 10 blocks
    int response = allocate(2, memory, 10, 2); // applying best fit algorithm
    if(response == 0){
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}