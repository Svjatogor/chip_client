#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "parser.h"
#include "utils.h"
#include "cuda.h"
#include "blas.h"
#include "connected_layer.h"
#include "socket_work.h"

extern void run_detector(char *datacfg, char *cfgfile, char *weightfile, char *filename, float thresh, float hier_thresh);
extern int sock_id;

int waiting_clients(char* port) {
    int sockfd, newsockfd;
    int portno;
    socklen_t client;
    struct sockaddr_in serv_addr, cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(port);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,1);
    client = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       &client);
    if (newsockfd < 0)
        error("ERROR on accept");
    close(sockfd);
    return newsockfd;
}

int main(int argc, char **argv)
{
    gpu_index = -1;
    char image_path[256];
    float thresh = .24;
    char command[256] = "";

    printf("\nWaiting client...\n");

    // connection to client
    sock_id = waiting_clients("1245");
    printf("Connection successful\n");
    send_message("Successful");

    while (1) {
        printf("Waiting command...\n");
        get_message(command);
        if (strcmp(command, "yolo") == 0) {
            send_message("ok");
            // getting the image for detection
            printf("Waiting picture...\n");
            get_image(image_path);
            printf("Picture received: %s\n", image_path);
            send_message("Sending of image successful");
            // run detection
            run_detector("cfg/voc.data", "cfg/tiny-yolo-voc.cfg", "weights/tiny-yolo-voc.weights", image_path, thresh, .5);
            printf("\n");
        }
        else if (strcmp(command, "exit") == 0) {
            printf("Client has gone\n");
            break;
        }
        else {
            printf("Incorrect command\n");
        }
    }
    close_sock();
    return 0;
}