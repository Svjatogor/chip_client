#include "socketworker.h"
#include "tcp_client.h"

SocketWorker::SocketWorker(int s) {
    _sockId = s;
}

void SocketWorker::receivingMessages() {
    // get parsing info
    char message[256];
    readMessage(_sockId, message);
    emit messageReceived(QString(message));
    // get weights info
    readMessage(_sockId, message);
    emit messageReceived(QString(message));
    // load imnage info
    readMessage(_sockId, message);
    emit messageReceived(QString(message));
    // start predict
    readMessage(_sockId, message);
    emit messageReceived(QString(message));
    // predict info
    readMessage(_sockId, message);
    emit messageReceived(QString(message));
    // get image
    emit finished("");
}
