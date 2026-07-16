#include "Client.h"
#include <QHostAddress>

Client::Client(QObject *parent)
    : QObject(parent), m_userType(AUDIENCE)
{
    connect(&m_socket, &QTcpSocket::readyRead, this, &Client::onReadData);
    connect(&m_socket, &QTcpSocket::connected, this, &Client::connected);
    connect(&m_socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(&m_socket, &QTcpSocket::errorOccurred, this, &Client::onSocketError);
}

bool Client::connectServer(const QString& ip, quint16 port)
{
    if (m_socket.state() == QAbstractSocket::ConnectedState)
        return true;
    m_socket.connectToHost(ip, port);
    return true;
}

void Client::sendPacket(const Packet& pkt)
{
    if (m_socket.state() != QAbstractSocket::ConnectedState)
        return;
    QString data = pkt.toString() + "\n";
    m_socket.write(data.toUtf8());
}

void Client::disconnectServer()
{
    m_socket.disconnectFromHost();
}

QString Client::userName() const
{
    return m_userName;
}

void Client::setUserName(const QString& name)
{
    m_userName = name;
}

UserType Client::userType() const
{
    return m_userType;
}

void Client::setUserType(UserType type)
{
    m_userType = type;
}

void Client::onReadData()
{
    m_recvBuffer += m_socket.readAll();

    while (m_recvBuffer.contains('\n'))
    {
        int idx = m_recvBuffer.indexOf('\n');
        QString line = m_recvBuffer.left(idx).trimmed();
        m_recvBuffer = m_recvBuffer.mid(idx + 1);

        if (line.isEmpty()) continue;

        Packet pkt = Packet::fromString(line);
        emit recvPacket(pkt);
    }
}


void Client::onSocketError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err)
    emit errorOccur(m_socket.errorString());
}
