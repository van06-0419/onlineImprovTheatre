#include "NetworkClient.h"
#include "../Common/Protocol.h"
#include <QJsonDocument>

NetworkClient::NetworkClient(QObject *parent)
    : QObject(parent), m_socket(new QTcpSocket(this))
{
    connect(m_socket, &QTcpSocket::connected, this, &NetworkClient::onConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkClient::onReadyRead);
    // 转发底层socket断开信号
    connect(m_socket, &QTcpSocket::disconnected, this, [this](){
        emit disconnected();
    });
}

void NetworkClient::connectServer(const QString &host, quint16 port)
{
    m_socket->connectToHost(host, port);
}

void NetworkClient::onConnected()
{
    emit connected();
}

void NetworkClient::sendJson(const QJsonObject &obj)
{
    m_socket->write(Protocol::packJson(obj));
}

void NetworkClient::sendAudio(const QByteArray &data)
{
    m_socket->write(Protocol::packBinary(Protocol::Audio, data));
}

void NetworkClient::onReadyRead()
{
    m_buffer += m_socket->readAll();
    processFrames();
}

void NetworkClient::processFrames()
{
    while(m_buffer.size() >= 4)
    {
        quint32 size = Protocol::readSize(m_buffer);
        if(m_buffer.size() < 4 + size) return;

        QByteArray body = m_buffer.mid(4, size);
        m_buffer.remove(0, 4 + size);
        if(body.isEmpty()) continue;

        if(quint8(body[0]) == Protocol::Audio)
        {
            emit audioReceived(body.mid(1));
            continue;
        }

        QJsonDocument doc = QJsonDocument::fromJson(body);
        if(doc.isObject()) emit jsonReceived(doc.object());
    }
}