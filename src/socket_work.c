//
// Created by root on 04.05.17.
//
#include "socket_work.h"
#include "darknet.h"
#include "utils.h"
#include <unistd.h>
#include <string.h>
extern int sock_id;

void send_message(const char* message) {
    int size = strlen(message);
    int n = write(sock_id, message, size);

    if (n < 0) {
        error("ERROR message not send");
    }
}

void get_message(char* message) {
    int n = read(sock_id, message, 255);
    if (n < 0) {
        error("ERROR message not get");
    }
}

void send_image(const char* file_name) {
    FILE *picture;
    picture = fopen(file_name, "r");
    int size;
    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    fseek(picture, 0, SEEK_SET);

    // send picture size
    int n = write(sock_id, &size, sizeof(size));
    if (n < 0) {
        error("ERROR size image not send");
    }

    // send picture as byte array
    char send_buffer[size];
    while(!feof(picture)) {
        fread(send_buffer, 1, sizeof(send_buffer), picture);
        n = write(sock_id, send_buffer, sizeof(send_buffer));
        if (n < 0) {
            error("ERROR image not send");;
        }
        bzero(send_buffer, sizeof(send_buffer));
    }
}

void get_image(char *file_name) {
    bzero(file_name, sizeof(file_name));
    strcpy(file_name, "img_for_detect.png");
    int size;
    int n = read(sock_id, &size, sizeof(int));
    if (n < 0) {
        error("ERROR size image not received");
    }
    printf("Size image: %d\nReading image...\n", size);
    char image_bytes[size];
    n = read(sock_id, image_bytes, size);
    if (n < 0) {
        error("ERROR image not received");
    }
    printf("Image received\n");
    // convert bytes to file
    FILE *image;
    image = fopen(file_name, "w");
    fwrite(image_bytes, 1, sizeof(image_bytes), image);
}

void close_sock() {
    close(sock_id);
}