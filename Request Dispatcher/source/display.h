// A utility function for displaying the request message in a proper format
void display(struct RequestBody newRequest){

    printf("DLL name:%s\n", newRequest.dll_name);
    printf("Function name:%s\n", newRequest.func_name);
    printf("Function arguements: ");
    int arg= -1;
    while(newRequest.func_args[++arg]!= NULL){
        printf("%s ", newRequest.func_args[arg]);
    }
    printf("\n");
}