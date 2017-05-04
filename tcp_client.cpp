#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "tcp_client.h"
int connectToServer() {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    portno = atoi("222");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;
    }
    //server = gethostbyname("10.42.0.97");
    server = gethostbyname("localhost");
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

int sendImage(int sock_id, char *file_name) {
    FILE *picture;
    picture = fopen(file_name, "r");
    int size;
    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    fseek(picture, 0, SEEK_SET);

    // send picture size
    int n = write(sock_id, &size, sizeof(size));
    if (n < 0) {
        return -1;
    }

    // send picture as byte array
    char send_buffer[size];
    while(!feof(picture)) {
        fread(send_buffer, 1, sizeof(send_buffer), picture);
        n = write(sock_id, send_buffer, sizeof(send_buffer));
        if (n < 0) {
            return -1;
        }
        bzero(send_buffer, sizeof(send_buffer));
    }

    return n;
}

void get_image(int sock_id, char* file_name) {
    bzero(file_name, sizeof(file_name));
    strcpy(file_name, "predictions.png");
    int size;
    int n = read(sock_id, &size, sizeof(int));
    if (n < 0) {
        return;
    }
    printf("Size image: %d\nReading image...\n", size);
    char image_bytes[size];
    n = 0;
    while (size > 0) {
        n = read(sock_id, image_bytes + n, size);
        if (n < 0) {
            return;
        }
        size -= n;
    }
    printf("n = %d\n", n);
    printf("Image received\n");
    // convert bytes to file
    FILE *image;
    image = fopen(file_name, "w");
    fwrite(image_bytes, 1, sizeof(image_bytes), image);
    fclose(image);
}
