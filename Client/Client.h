#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include "../Common/Packet.h"
#include "../Common/CommonDef.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    bool connectServer(const QString& ip, quint16 port);
    void sendPacket(const Packet& pkt);
    void disconnectServer();

    QString userName() const;
    void setUserName(const QString& name);

    UserType userType() const;
    void setUserType(UserType type);

signals:
    void recvPacket(Packet pkt);
    void connected();
    void disconnected();
    void errorOccur(QString err);

private slots:
    void onReadData();
    void onSocketError(QAbstractSocket::SocketError err);

private:
    QTcpSocket m_socket;
    QString    m_userName;
    UserType   m_userType;
    QString    m_recvBuffer; 
};

#endif // CLIENT_H
