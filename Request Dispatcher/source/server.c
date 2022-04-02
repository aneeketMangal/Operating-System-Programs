#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <pthread.h>
#include <dlfcn.h>
#include <errno.h>

//file includes
#include "error.h"
#include "const.h"
#include "request_body.h"
#include "request_queue.h"
#include "display.h"
#include "dll_invoker.h"
#include "serial.h"

int DISPATCHER_THREAD_POOL_SIZE;
int FILE_POOL_SIZE;
int MEMORY_POOL_SIZE;
pthread_t * dispatcherThreadPool;
struct RequestQueue* requestQueue; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conVar = PTHREAD_COND_INITIALIZER;



int createNewSocket(char * socketFile){
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) error("Can not open the socket", 1);

    struct sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = htons(atoi(socketFile));


    // binding to the port
    if (bind(sockFd, (struct sockaddr *) &name, sizeof(name)) < 0) {
        error("Bind failed", 1);
    }
    
    return sockFd;
}


void* threadFunction(int fd) {
    char buffer[SERVER_BUFF_SIZE];
    memset(buffer, '\0', sizeof(buffer));
    int count = read(fd, buffer, sizeof(buffer));
    if (count > 0) {
        printf("Query received from client: %s\n", buffer);
        struct RequestBody newRequest = serialiseMessage(buffer);
        if(newRequest.dll_name == NULL){
            write(fd, "Improper query entered.\n", 255);
            prompt("Improper query.\n");
        }
        else{
            /* if the request is validated enqueue the
               request.
            */
            pthread_mutex_lock(&mutex); // locking to prevent multiple enqueues at same time
            int resEnq = enqueue(requestQueue, newRequest);
            if(resEnq) prompt("Enqueue operation.\n");
            pthread_cond_signal(&conVar); // sending a signal to the thread pool that a enqueue operation has took place
            pthread_mutex_unlock(&mutex);
            write(fd, "Query recieved by Server", 255);
        }
    }
    close(fd);
    pthread_exit(NULL);
}

// function to spawn new threads
void createNewThread(int fd){
    pthread_t threadId;
    int rc = pthread_create(&threadId, NULL, threadFunction, (void*) fd);
    if(rc) error("Failed to create thread.", 1);
    else prompt("Thread created for the new client.\n");
}

void startServer(char * socketFile){

    //following line creates new socket
    int sockFd = createNewSocket(socketFile);
    if(listen(sockFd, MAX_CONNECTS)<0){
        error("Socket is not listening.", 1);
    }
    else{
        prompt("Socket is listening.\n");
    }
    while(1){
        struct sockaddr_in cliAddr;
        int clilen = sizeof(cliAddr);
        prompt("Waiting for connections.\n");
        int clientFd = accept(sockFd, (struct sockaddr *)&cliAddr, &clilen);
        if (clientFd < 0) {
            error("Error on accept.", 1);
            continue;
        }
        createNewThread(clientFd);       
    }
} 

/* the following is a thread process to complete the incoming request.
It keeps looking if there is a unfinished request and as it finds one it completes it.
*/
void* lookForProcessing(){
    while(1){
        struct RequestBody currRequest;
        pthread_mutex_lock(&mutex);
        currRequest = dequeue(requestQueue);
        if (currRequest.dll_name == NULL){
            pthread_cond_wait(&conVar, &mutex);
            currRequest = dequeue(requestQueue);
        }
        pthread_mutex_unlock(&mutex);
        if(currRequest.dll_name!=NULL){
            display(currRequest);
            double ans = dllInvoker(currRequest);
            printf("Output: %f\n", ans);
        }
    }
}

void* dispatcher(){
    for(int i = 0;i<DISPATCHER_THREAD_POOL_SIZE;i++){
        pthread_create(&dispatcherThreadPool[i], NULL, lookForProcessing, NULL);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 5){
       error("usage %s [port] [dispatcher thread pool size] [file pool size] [memory pool size]\n", 1);    
    } 
    DISPATCHER_THREAD_POOL_SIZE = atoi(argv[2]);
    FILE_POOL_SIZE = atoi(argv[3]);
    MEMORY_POOL_SIZE = atoi(argv[4]);
    requestQueue = buildQueue();
    dispatcherThreadPool = malloc(DISPATCHER_THREAD_POOL_SIZE*sizeof(pthread_t));
    pthread_t dispatcherId;

    int dis = pthread_create(&dispatcherId, NULL, dispatcher, NULL);

    if(dis!=0) error("Cannot start the dispatcher", 1);
    else prompt("Dispatcher started.\n");
    startServer(argv[1]);

    return 0;
}
