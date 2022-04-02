//utility functions for handling error and warning message
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"

void prompt(const char *msg) { 
    printf("%s\n", msg); 
}

void success(const char *msg) { 
    printf(COLOR_GREEN "%s\n" COLOR_RESET, msg); 
}

void error(const char *msg, int close)
{
   
    if (close == 1){
        printf(COLOR_RED "%s\nExiting...\n" COLOR_RESET, msg);
        exit(-1);
    }
    else{
        printf(COLOR_RED "%s\n" COLOR_RESET, msg);
    }
}
