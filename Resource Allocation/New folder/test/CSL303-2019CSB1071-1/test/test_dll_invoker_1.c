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
    sampleRequest.func_name = strdup("pow");
    sampleRequest.func_args = malloc(2*sizeof(char*));
    sampleRequest.func_args[0] = "2.0";
    sampleRequest.func_args[1] = "5.0";
    double result = dllInvoker(sampleRequest);
    double check = pow(2.0, 5.0);
    if(result == check){
        printf("Test 1 passed.\n");
    }
    else{
        printf("Test 1 failed.\n");
    }
}