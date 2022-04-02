#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#include "../../source/request/request_body.c"
#include "../../source/request/request_queue.c"

int main(){
    struct RequestQueue* temp = buildQueue();
    struct RequestBody a;
    a.valid = 1;
    a.size = 100;
    a.duration = 50;
    enqueue(temp, a);
    a.size = 598;
    enqueue(temp, a);
    struct RequestBody b = dequeue(temp);
    b = dequeue(temp);
    if(b.size == 598 && b.duration == 50 && b.valid == 1){ 
        printf("Test Passed\n");
    }
    else{
        printf("Test Failed\n");
    }
}