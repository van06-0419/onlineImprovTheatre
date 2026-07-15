#include "Room.h"
#include <QDebug>
#include <QStringList>
#include <QRandomGenerator>
#include <algorithm>

Room::Room(QObject *parent)
    : QObject(parent),
      m_gameStarted(false)
{
}

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

            if (m_gameStarted)
            {
                User* newUser = m_userMgr.findUser(sock);
                if (newUser)
                {
                    QString selfRole = (newUser->userType() == ACTOR)
                        ? m_roleMap.value(newUser->userName())
                        : "";

                    QString actorRoleStr;
                    for (auto it = m_roleMap.begin(); it != m_roleMap.end(); ++it)
                    {
                        if (!actorRoleStr.isEmpty()) actorRoleStr += ",";
                        actorRoleStr += it.key() + ":" + it.value();
                    }

                    QString pktData = QString("%1|%2|%3")
                        .arg(m_currentScene)
                        .arg(selfRole)
                        .arg(actorRoleStr);

                    sock->write((Packet("START", pktData).toString() + "\n").toUtf8());
                }
            }
        }
        else
        {
            sock->write("MSG|Комната заполнена\n");
        }
    }
    else if (cmd == "START")
    {
        m_gameStarted = true;
        m_currentScene = m_scene.getRandomScene();
        m_roleMap.clear();

        for (const User& user : m_userMgr.users())
        {
            if (user.userType() == ACTOR)
            {
                QString randRole = m_role.getRandomRole();
                m_roleMap.insert(user.userName(), randRole);
            }
        }

        QString actorRoleStr;
        for (auto it = m_roleMap.begin(); it != m_roleMap.end(); ++it)
        {
            if (!actorRoleStr.isEmpty())
                actorRoleStr += ",";
            actorRoleStr += it.key() + ":" + it.value();
        }

        for (QTcpSocket* s : m_socketList)
        {
            User* user = m_userMgr.findUser(s);
            if (!user) continue;

            QString selfRole = m_roleMap.value(user->userName(), "");
            QString pktData = QString("%1|%2|%3")
                .arg(m_currentScene)
                .arg(selfRole)
                .arg(actorRoleStr);

            Packet p("START", pktData);
            s->write((p.toString() + "\n").toUtf8());
        }

        m_userMgr.resetVoteStatus();
    }
    else if (cmd == "CHAT")
    {
        broadcastPacket(Packet("CHAT", data));
    }
    else if (cmd == "VOICE")
    {
        //  работает ТОЛЬКО с VoiceChat.h/cpp 
        // в клиент и модуль Multimedia в CMakeLists.txt. 
        // Если голосовой чат пока не нужен, удалите этот блок else if.
        broadcastPacket(Packet("VOICE", data));
    }
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