void SSTF(Request* request, int requestCount, Disk* disk, double * timeTrack)
{
    double timeElapsed = 0;
    disk->track = generateRandomInteger(0, disk->cylinder - 1);
    int visited[requestCount]; //keeps track of visited requests
    for(int i = 0; i < requestCount; i++){visited[i] = 0;}
    for(int i = 0; i < requestCount; i++)
    {
        long int start = clock();
        int index = -1;
        int mini = INT_MAX;
        int diff = 0;
        for(int j = 0; j < requestCount; j++)
        {
            diff = abs(disk->track - request[j].cylinder);
            if (!visited[j] && mini >= diff)
            {
                mini = diff;
                index = j;
            }
        }
        visited[index] = 1;
        long int end = clock();
        // timeElapsed += (float)(end - start)/CLOCKS_PER_SEC;
        int accessTime = access(request[index], disk);
        double responseTime = timeElapsed +  accessTime;
        timeElapsed = responseTime;
        timeTrack[i] = responseTime;
    }
}
 
