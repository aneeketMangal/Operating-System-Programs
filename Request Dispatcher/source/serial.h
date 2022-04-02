
//  A utility function to serialise the message from the client
struct RequestBody serialiseMessage(char * buffer){
    struct RequestBody newRequest;
    int noOfArguements = 1;
    for(int i = 0;i<strlen(buffer);i++){
        if(buffer[i] == ',') noOfArguements++;
    }
    if(noOfArguements<2){
        newRequest.dll_name = NULL;
        return newRequest;
    }
    char *token = strtok(buffer, ",");
    newRequest.dll_name = strdup(token);
    token = strtok(NULL, ",");
    newRequest.func_name = strdup(token);
    token = strtok(NULL, ",");
    newRequest.func_args = malloc(sizeof(char*) * noOfArguements-2);
    int temp = 0;

    while (token != NULL) 
    {
        newRequest.func_args[temp] = strdup(token);
        temp+=1;
        token = strtok(NULL, ",");
    }
    return newRequest;
}