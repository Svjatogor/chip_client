//
// Created by root on 04.05.17.
//

#ifndef DARKNET_SOCKET_WORK_H
#define DARKNET_SOCKET_WORK_H

void send_message(const char* message);
void get_message(char* message);
void send_image(char* file_name);
void get_image();
void close_sock();
#endif //DARKNET_SOCKET_WORK_H
