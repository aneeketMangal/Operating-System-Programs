
//utility functions for handling error and warning message
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"

void prompt(const char *msg) { 
        printf("%s\n", msg);
}

void success(const char *msg) { 
        printf(COLOR_GREEN "%s" COLOR_RESET "\n", msg);
}

void error(const char *msg, int close)
{
    if (close == 1){
        printf(COLOR_RED "%s\nExiting..." COLOR_RESET "\n", msg);
        exit(-1);
    }
    else{
            printf(COLOR_RED "%s" COLOR_RESET "\n", msg); 
    }
}


/* 
    utility function to generate a random integer 
    between lowerLimit and UpperLimit (inclusive)
*/
int generateRandom(double lowerLimit, double upperLimit, int round){
    int LL = ceil(lowerLimit/round);
    int UL = floor(upperLimit/round);
    int out = (rand()%(1+UL-LL))+LL;
    return out*round;
}

/*
    Utility function to compare two arrays
    a & b such that it returns: 
        1 when a[i]>= b[i] for all elements 
        0 othrewise
*/
int compare(int * a, int * b, int sizeOfArrays){
    for(int i = 0;i< sizeOfArrays;i++){
        if(a[i]<b[i]) return 0;
    }
    return 1;
}

int max(int X, int Y){
    return (((X) > (Y)) ? (X) : (Y));
}


void printArray(int * array, int size, int maxSize){

    for (int i = 0; i < size; i++)
    {
        printf("%d ", array[i]+1);
        fflush(stdout);
    }
    printf("\n");
}

