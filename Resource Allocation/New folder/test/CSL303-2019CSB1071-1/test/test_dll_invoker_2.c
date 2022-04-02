#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dlfcn.h>
#include "../source/error.h"
#include "../source/request_body.h"
#include "../source/dll_invoker.h"

int main(){ 
    struct RequestBody sampleRequest;
    sampleRequest.dll_name = strdup("/lib/x86_64-linux-gnu/libm.so.6");
    sampleRequest.func_name = strdup("floor");
    sampleRequest.func_args = malloc(sizeof(char*));
    sampleRequest.func_args[0] = "2.39";
    double result = dllInvoker(sampleRequest);
    double check = floor(2.39);
    if(result == check){
        printf("Test 2 passed.\n");
    }
    else{
        printf("Test 2 failed.\n");
    }
}