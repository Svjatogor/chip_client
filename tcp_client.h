#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
int connectToServer();
void closeSocket(int sock_id);
int readMessage(int sock_id, char* buffer);
int writeMessage(int sock_id, char* buffer);
#endif // TCP_CLIENT_H
