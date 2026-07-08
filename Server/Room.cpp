#include "Room.h"
#include <QDebug>
#include <QStringList>

Room::Room(QObject *parent)
    : QObject(parent)
{}

void Room::addClientSocket(QTcpSocket *sock)
{
    if (!sock) return;
    m_socketList.append(sock);
    connect(sock, &QTcpSocket::readyRead, this, &Room::onReadData);
    connect(sock, &QTcpSocket::disconnected, this, &Room::onClientDisconnect);
}

void Room::broadcastPacket(const Packet &pkt)
{
    QString data = pkt.toString() + "\n";
    QByteArray byteData = data.toUtf8();
    for (auto sock : m_socketList)
    {
        if (sock->isOpen() && sock->isValid())
            sock->write(byteData);
    }
}

void Room::onReadData()
{
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());
    if (!sock) return;

    QString recvData = sock->readAll();
    Packet pkt = Packet::fromString(recvData);
    handlePacket(sock, pkt);
}

void Room::onClientDisconnect()
{
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());
    if (!sock) return;

    m_userMgr.removeUser(sock);
    m_socketList.removeOne(sock);
    sock->deleteLater();
    qDebug() << "Клиент отключился";
}

void Room::handlePacket(QTcpSocket *sock, const Packet &pkt)
{
    QString cmd = pkt.command();
    QString data = pkt.data();

    // Авторизация
    if (cmd == "LOGIN")
    {
        QStringList parts = data.split(",");
        if (parts.size() < 2) return;

        QString name = parts[0];
        UserType type = (parts[1] == "ACTOR") ? ACTOR : AUDIENCE;
        User user(name, type, sock);

        bool ok = m_userMgr.addUser(user);
        if (ok)
        {
            broadcastPacket(Packet("MSG", name + " вошёл в комнату"));
        }
        else
        {
            sock->write("MSG|Комната заполнена\n");
        }
    }
    // Начать игру: назначить роль и сцену
    else if (cmd == "START")
    {
        QString scene = m_scene.getRandomScene();
        QString role = m_role.getRandomRole();
        broadcastPacket(Packet("START", scene + "|" + role));
        m_userMgr.resetVoteStatus();
    }
    // Передача сообщений чата
    else if (cmd == "CHAT")
    {
        broadcastPacket(Packet("CHAT", data));
    }
    // Голосование зрителей
    else if (cmd == "VOTE")
    {
        QString voteName = data;
        m_userMgr.voteForUser(voteName);

        if (m_userMgr.allAudienceVoted())
        {
            QString winner = m_userMgr.getWinner();
            broadcastPacket(Packet("RESULT", "Лучший актёр: " + winner));
        }
    }
}