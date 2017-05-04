#ifndef SOCKETWORKER_H
#define SOCKETWORKER_H

#include <QObject>

class SocketWorker : public QObject
{
    Q_OBJECT
public:
    SocketWorker(int s);

signals:
    void messageReceived(QString message);
    void finished(QString);

public slots:
    void receivingMessages();
private:
    int _sockId;
};

#endif // SOCKETWORKER_H
