#ifndef ROOM_H
#define ROOM_H

#include "UserManager.h"
#include "../Common/Role.h"
#include "../Common/Scene.h"
#include "../Common/Packet.h"

#include <QObject>
#include <QTcpSocket>
#include <QVector>
#include <QMap>

class Room : public QObject
{
    Q_OBJECT

public:
    explicit Room(QObject *parent = nullptr);
    void addClientSocket(QTcpSocket *sock);
    void broadcastPacket(const Packet &pkt);

private slots:
    void onReadData();
    void onClientDisconnect();

private:
    UserManager m_userMgr;
    Role m_role;
    Scene m_scene;
    QVector<QTcpSocket*> m_socketList;
    bool m_gameStarted;
    QString m_currentScene;
    QMap<QString, QString> m_roleMap;
    void handlePacket(QTcpSocket *sock,
                      const Packet &pkt);
};

#endif // ROOM_H