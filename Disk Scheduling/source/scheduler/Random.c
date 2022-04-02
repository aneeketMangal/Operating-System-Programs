void Random(Request* request, int requestCount, Disk* disk, double * timeTrack)
{
    double timeElapsed = 0;
    int leftRequest = requestCount; //keeps track of count of requests left to visit
    for(int i = 0; i < requestCount; i++)
    {
        long int start = clock();
        //picking a random request
        int index = generateRandomInteger(0, leftRequest-1);
        Request temp = request[index];
        request[index] = request[leftRequest-1];
        request[leftRequest-1] = temp;
        leftRequest-=1;
        
        long int end = clock();
        
        timeElapsed += (float)(end - start)/CLOCKS_PER_SEC;
        int accessTime = access(temp, disk);
        double responseTime = timeElapsed +  accessTime;
        timeElapsed = responseTime;
        timeTrack[i] = responseTime;
    }
}
 
