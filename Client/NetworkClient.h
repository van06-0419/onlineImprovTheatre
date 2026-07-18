#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>

class NetworkClient : public QObject
{
    Q_OBJECT

public:
    explicit NetworkClient(QObject *parent = nullptr);
    void connectServer(const QString &host, quint16 port);
    void sendJson(const QJsonObject &obj);
    void sendAudio(const QByteArray &data);

signals:
    void connected();
    void jsonReceived(const QJsonObject &obj);
    void audioReceived(const QByteArray &data);

private slots:
    void onConnected();
    void onReadyRead();

private:
    QTcpSocket *m_socket;
    QByteArray m_buffer;
    void processFrames();
};

#endif