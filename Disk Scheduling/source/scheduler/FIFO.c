void FIFO(Request* request, int requestCount, Disk* disk, double * timeTrack)
{
    double timeElapsed = 0;
    int visited[requestCount]; //keeps track of visited requests
    for(int i = 0; i < requestCount; i++){visited[i] = 0;}
    for(int i = 0; i < requestCount; i++)
    {
        long int start = clock();
        int index = i;
        visited[index] = 1;
        long int end = clock();
        timeElapsed += (float)(end - start)/CLOCKS_PER_SEC;
        int accessTime = access(request[index], disk);
        double responseTime = timeElapsed +  accessTime;
        timeElapsed = responseTime;
        timeTrack[i] = responseTime;
    }
}
 
