//utility functions for handling error and warning message
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"

void prompt(FILE* p, const char *msg, int isTerminal) { 
    if(isTerminal){
        printf("%s\n", msg);
    }
    else{
        fprintf(p, "%s\n", msg); 
    }
}

void success(FILE* p, const char *msg, int isTerminal) { 
    if(isTerminal){
        printf(COLOR_GREEN "%s\n" COLOR_RESET, msg);
    }
    else{
        fprintf(p, "%s\n", msg); 
    }
   
}

void error(FILE* p, const char *msg, int close, int isTerminal)
{
    if (close == 1){
        printf(COLOR_RED "%s\nExiting...\n" COLOR_RESET, msg);
        exit(-1);
    }
    else{
        if(isTerminal){
            printf(COLOR_RED "%s\n" COLOR_RESET, msg);
        }
        else{
            fprintf(p, "%s\n", msg); 
        }
        
    }
}
