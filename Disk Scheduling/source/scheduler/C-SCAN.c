void CSCAN(Request* request, int requestCount, Disk* disk, double * timeTrack)
{
    int direction = 1; //direction 1 for right, -1 for left
    disk->track = generateRandomInteger(0, disk->cylinder - 1);
    double timeElapsed = 0;
    int visited[requestCount]; //keeps track of visited requests
    for(int i = 0; i < requestCount; i++){visited[i] = 0;}
    int i = 0;
    for(i = 0; i < requestCount; i++)
    {
        long int start = clock();
        int index = -1;
        if(direction == 1){
            int mini = disk->cylinder;
            for(int j = 0; j < requestCount; j++)
            {
                if (request[j].cylinder  >=disk->track && request[j].cylinder <= mini && visited[j] == 0)
                {
                    mini = request[j].cylinder;
                    index = j;
                }
            }
        }
        else if(direction == -1){
            int maxi = -1;
            for(int j = 0; j < requestCount; j++)
            {
                if (request[j].cylinder  <= disk->track && request[j].cylinder >= maxi && visited[j] == 0)
                {
                    maxi = request[j].cylinder;
                    index = j;
                }
            }
        }
        if(index == -1){
            if(direction == -1){
                timeElapsed+= seek(0, disk);
            }
            else{
                timeElapsed+=seek(disk->cylinder-1, disk);
            }
            direction = -direction;
            i-=1;
            continue;
        }        
        visited[index] = 1;
        long int end = clock();
        timeElapsed += (float)(end - start)/CLOCKS_PER_SEC;
        int accessTime = access(request[index], disk);
        double responseTime = timeElapsed +  accessTime;
        timeElapsed = responseTime;
        timeTrack[i] = responseTime;
    }
}
 
