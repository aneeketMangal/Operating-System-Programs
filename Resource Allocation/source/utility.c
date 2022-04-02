/* 
    utility function to generate a random integer 
    between lowerLimit and UpperLimit (inclusive)
*/
int generateRandomInteger(int lowerLimit, int upperLimit){
    if(upperLimit<lowerLimit){
        return 0;
    }
    return (rand()%(1+upperLimit-lowerLimit))+lowerLimit;
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

void swap(int * array, int b, int c) {
    int temp = array[b];
    array[b] = array[c];
    array[b] = temp;
}

int rand_comparison(const void *a, const void *b)
{
    (void)a; (void)b;
    return rand() % 2 ? +1 : -1;
}

void printArray(int * array, int size, int maxSize){

    qsort(array, size, sizeof(int), rand_comparison);
    for (int i = 0; i < size; i++)
    {
        printf("%d ", array[i]+1);
        fflush(stdout);
    }
    printf("\n");
}
