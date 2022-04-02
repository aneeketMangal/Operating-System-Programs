char * AA[10] = {"cos", "sin", "tan", "tanh", "cosh", "sinh", "ceil", "floor", "log", "sqrt"};
char * BB[1] = {"pow"};
/*
    AA: Contains functions with signature (double arg1) and return type double
    BB: Contains functions with signature (double arg1, double arg2) and return type double
*/
int find(char *temp[], int size, char* test){
    for(int i = 0;i<size;i++){
        if(strcmp(test, temp[i]) == 0) return 1;
    }
    return 0;
}

/* This function is meant for loading functions that have function signature
    (double arg1) and return type of double.
*/
double A(struct RequestBody request){
    
    double y = atof(request.func_args[0]);
    void *handle;
    double (*func)(double);
    char *errorC;

    handle = dlopen (request.dll_name, RTLD_LAZY);
    if (!handle){
        error(dlerror(), 0);
        return 0;
    }

    func = dlsym(handle, request.func_name);
    if ((errorC = dlerror()) != NULL) {
        error(errorC, 0);
        return 0;
    }

    double ans =  (*func)(y);
    // printf("Output: %f\n", ans);
    return ans;
}
/* This function is meant for loading functions that have function signature
    (double arg1, double arg2) and return type of double.
*/
double B(struct RequestBody request){
    double y = atof(request.func_args[0]);
    double z = atof(request.func_args[1]);
    void *handle;
    double (*func)(double, double);
    char *errorC;

    handle = dlopen (request.dll_name, RTLD_LAZY);
    if (!handle){
        error(dlerror(), 0);
        return 0;
    }

    func = dlsym(handle, request.func_name);
    if ((errorC = dlerror()) != NULL) {
        error(errorC, 0);
        return 0;
    }
    double ans =  (*func)(y,z);
    // printf("Output: %f\n", ans);
    return ans;
}


double dllInvoker(struct RequestBody request){
    if(strcmp(request.dll_name, "/lib/x86_64-linux-gnu/libm.so.6") == 0){
        if(find(AA, 10,request.func_name)){
            double ans = A(request);
            return ans;
        }
 
        else if(find(BB, 1, request.func_name)){
            double ans = B(request);
            return ans;
        }
    }
    else{
        printf("Not Supported.\n");
    }
}