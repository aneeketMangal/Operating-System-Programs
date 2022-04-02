//utility functions for handling error and warning message
void prompt(const char *msg) { 
    printf("%s", msg); 
}
void error(const char *msg, int close)
{
    perror(msg);
    if (close == 1){
        exit(-1);
    }
}
