#include "Server.h"
#include <QDebug>
#include <QTcpSocket>
#include "../Common/CommonDef.h"
#include "../Common/Packet.h"
#include "../Common/Scene.h"
#include "../Common/Role.h"

Server::Server(QObject *parent)
    : QObject(parent)
{
    connect(&m_tcpServer, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

bool Server::startServer(quint16 port)
{
    if (!m_tcpServer.listen(QHostAddress::Any, port))
    {
        qDebug() << "Ошибка запуска сервера: " << m_tcpServer.errorString();
        return false;
    }
    qDebug() << "Сервер запущен успешно, прослушиваем порт: " << port;
    return true;
}

void Server::onNewConnection()
{
    QTcpSocket* sock = m_tcpServer.nextPendingConnection();
    if (sock)
    {
        qDebug() << "Новый клиент подключен";
        m_room.addClientSocket(sock);
        connect(sock, &QTcpSocket::disconnected, sock, &QTcpSocket::deleteLater);
    }
}
