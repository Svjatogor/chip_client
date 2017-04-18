#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "tcp_client.h"
int connectToServet() {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    portno = atoi("222");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;
    }
    server = gethostbyname("192.168.0.5");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        return -1;
    return sockfd;
}

void closeSocket(int sock_id) {
    close(sock_id);
}

int readMessage(int sock_id, char* buffer) {
    int n = read(sock_id, buffer, 255);
    if (n < 0) {
        return -1;
    }
    return n;
}

int writeMessage(int sock_id, char* buffer) {
    int count = strlen(buffer);
    int n = write(sock_id,buffer, count);
    if (n < 0) {
        return -1;
    }
    return n;
}
