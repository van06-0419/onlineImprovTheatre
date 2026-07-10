#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include "Room.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    bool startServer(quint16 port);

private slots:
    void onNewConnection();

private:
    QTcpServer m_tcpServer;
    Room m_room;
};

#endif // SERVER_H
