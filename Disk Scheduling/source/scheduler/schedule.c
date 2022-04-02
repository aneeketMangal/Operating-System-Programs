

void DiskScheduler(Request* requests, int requestCount, Disk *disk, int policy){
    double temp[requestCount]; // to store the response times to calculate the throughput, response etc,
    switch(policy){
        case 1:
            Random(requests, requestCount, disk, temp);
            break;
        case 2:
            FIFO(requests, requestCount, disk, temp);
            break;
        case 3:
            SSTF(requests, requestCount, disk, temp);
            break;
        case 4:
            SCAN(requests, requestCount, disk, temp);
            break;
        case 5:
            CSCAN(requests, requestCount, disk, temp);
            break;

    }
    double throughput = 0.0;
    double sum = 0.0;
    double mini  = 1e12;
    double maxi = 0.0;
    double avg = 0.0;
    double variance = 0.0;
    double stdDev;
    for(int i = 0; i < requestCount; i++){
        sum += (double)temp[i];
        mini = (double)mini < temp[i] ? mini : temp[i];
        maxi = (double)maxi > temp[i] ? maxi : temp[i];
    }

    avg = sum / requestCount;
    for(int i = 0; i < requestCount; i++){
        variance += ((temp[i] - avg) * (temp[i] - avg));
    }
    variance = variance / requestCount;
    stdDev = sqrt(variance);
    throughput = (double)requestCount / (double)temp[requestCount - 1];
    throughput = throughput * 1000; // to convert into per second
    printf("Throughput: %0.2f\n", throughput);
    printf("Response time:-\n");
    printf("\t1. Average :%0.2f\n", avg);
    printf("\t2. Minimum :%0.2f\n", mini);
    printf("\t3. Maximum :%0.2f\n", maxi);
    printf("\t4. Std.Dev :%0.2f\n", stdDev);
}