#include "main.h"

int main(int argc, char *argv[])
{
    if (argc < 5){
        error("usage: [rotational speed]  [sector size] [average seek time] [scheduling algorithm]", 1);    
    } 
    srand(time(0));
    Disk* disk; // object of disk
    disk = malloc(sizeof(Disk));
    int requestCount = 1000;
    disk->r = atoi(argv[1]);
    disk->N = atoi(argv[2]);
    disk->Ts = atoi(argv[3]);
    disk->head = 0;
    disk->track = 0;
    disk->surface = 4;
    disk->cylinder = 25;
    disk->sector = 20;
    Request *requestQueue = malloc(requestCount * sizeof(Request));
    for(int i =0;i<requestCount;i++){
        requestQueue[i] = generateRandomRequest(disk->surface, disk->cylinder, disk->sector);
    }    
    DiskScheduler(requestQueue, requestCount, disk, atoi(argv[4]));
}
