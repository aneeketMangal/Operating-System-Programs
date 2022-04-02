#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "const.h"



void prompt(const char *msg) {printf("%s", msg);}
void error(const char *msg) {perror(msg);exit(-1);}

int main(int argc, char *argv[])
{ 
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    int sockFd, portno, n;
    struct sockaddr_in serAddr;
    struct hostent *server;
    char buffer[CLIENT_BUFF_SIZE];
    
    portno = atoi(argv[2]);
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) 
        error("Error opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) 
        error("No such host.\n");
    
    // configuring the serAddr
    bzero((char *) &serAddr, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serAddr.sin_addr.s_addr,server->h_length);
    serAddr.sin_port = htons(portno);

    if (connect(sockFd,(struct sockaddr *) &serAddr,sizeof(serAddr)) < 0) 
        error("Error connecting to server");
    
    //sending query to the server
    printf("Please enter the query: ");
    bzero(buffer,CLIENT_BUFF_SIZE);
    fgets(buffer,CLIENT_BUFF_SIZE-1,stdin);
    n = write(sockFd,buffer,strlen(buffer));
    if (n < 0) error("Error writing to socket");
    
    //getting response from server
    bzero(buffer,CLIENT_BUFF_SIZE);
    n = read(sockFd,buffer,CLIENT_BUFF_SIZE-1);
    if (n < 0) error("Error reading from socket");
    printf("%s\n",buffer);
    return 0;
}