//
// Created by root on 04.05.17.
//
#include "socket_work.h"
#include "darknet.h"
#include "utils.h"
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

extern int sock_id;

void send_message(const char* message) {
    int n = 0;
    int size_message = strlen(message);
    send(sock_id, &size_message, sizeof(size_message), 0);
    n = send(sock_id, message, size_message, 0);
    if (n < 0) {
        error("ERROR message not send");
    }
}

void get_message(char* message) {
    bzero(message, 256);
    int n = 0;
    int size_message = 0;
    n = recv(sock_id, &size_message, sizeof(size_message), 0);
    n = recv(sock_id, message, size_message, 0);
    if (n < 0) {
        error("ERROR message not get");
    }
}

void send_image(const char* file_name) {
    FILE *picture;
    picture = fopen(file_name, "r");
    int size = 0;
    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    fseek(picture, 0, SEEK_SET);
    printf("size image: %d", size);
    // send picture size
    int n = 0;
    n = send(sock_id, &size, sizeof(size), 0);
    if (n < 0) {
        error("ERROR size image not send");
    }

    // send picture as byte array
    char send_buffer[size];
    //while(!feof(picture)) {
        fread(send_buffer, 1, sizeof(send_buffer), picture);
        n = send(sock_id, send_buffer, sizeof(send_buffer), 0);
        if (n < 0) {
            error("ERROR image not send");;
        }
        bzero(send_buffer, sizeof(send_buffer));
    //}
}

void get_image(char *file_name) {
    bzero(file_name, sizeof(file_name));
    strcpy(file_name, "img_for_detect");
    int size = 0;
    int n = 0;
    n = recv(sock_id, &size, sizeof(int), 0);
    if (n < 0) {
        error("ERROR size image not received");
    }
    printf("Size image: %d\nReading image...\n", size);
    char image_bytes[size];
    char* point_image = image_bytes;
    while (size > 0) {
        n = recv(sock_id, point_image, size, 0);
        point_image += n;
        size -= n;
    }
    if (n < 0) {
        error("ERROR image not received");
    }
    printf("Image received\n");
    // convert bytes to file
    FILE *image;
    image = fopen(file_name, "w");
    fwrite(image_bytes, 1, sizeof(image_bytes), image);
    fclose(image);
}

void close_sock() {
    close(sock_id);
}