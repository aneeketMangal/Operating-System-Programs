typedef struct temp {
    int r;  //revolution per minute
    int Ts; // average seek time (ms)
    int N; //sector size (bytes)
    int head; // head position
    int track; //track which is currently being accessed
    int surface;//number of surfaces
    int cylinder; //number of cylinders
    int sector; //number of sectors
} Disk;


// int seek(int finalPosition, Disk* disk){
//     int seekTime = 0;
//     int distance = abs(finalPosition - disk->track);
//     seekTime = distance * disk->Ts;
//     disk->track = finalPosition;
//     return seekTime;
// }

// int rotate(int finalPosition, Disk* disk){ // 1/(2*r)
//     int rotateTime = 0;
//     rotateTime = (60*1000*abs(finalPosition-disk->head))/(2*disk->r);  
//     rotateTime = rotateTime * 1000; //in ms
//     disk->head = finalPosition;
//     return rotateTime;
// }

//functions to simuate actions of a disk
int seek(int finalPosition, Disk* disk){
    int seekTime = 0;
    int distance = abs(finalPosition - disk->track);
    seekTime = distance * disk->Ts*3; //total seek time  = 3*avgseektime
    seekTime = seekTime/disk->cylinder;
    disk->track = finalPosition;
    return seekTime;
}

int rotate(int finalPosition, Disk* disk){ // total number of sectors* time taken to move particular sector
    int rotateTime = 0;
    int distance = abs(finalPosition-disk->head);
    rotateTime = ((distance)*(60*1000))/(disk->sector*disk->r); //in ms
    disk->head = finalPosition;
    return rotateTime;
}
// sector size would get cancelled in the calculation of rotate and transfer time
int transfer(int sectorCount, Disk * disk){ //(b/Nr)
    int transferTime = (sectorCount*60*1000)/(disk->sector*disk->r);//in ms
    disk->head += sectorCount;
    return transferTime;
}

int access(Request request, Disk * disk){
    int seekTime = 0;
    int rotateTime = 0;
    int transferTime = 0;
    int totalTime = 0;
    seekTime = seek(request.cylinder, disk);
    rotateTime = rotate(request.sector, disk);
    transferTime = transfer(request.sector_count, disk);
    totalTime = seekTime + rotateTime + transferTime;
    return totalTime;
}
