#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QMap>

struct ClientInfo {
    QString name;
    QString identity;
    QByteArray buffer;
};

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    bool start(quint16 port);
    void startGame();
    void startVote();
    void finishVote();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer m_server;
    QHash<QTcpSocket*, ClientInfo> m_clients;
    QMap<QString, int> m_votes;
    bool m_gameStarted = false;
    bool m_voting = false;

    void processFrames(QTcpSocket *socket);
    void processJson(QTcpSocket *socket, const QJsonObject &obj);
    void processAudio(QTcpSocket *sender, const QByteArray &audio);
    void sendJson(QTcpSocket *socket, const QJsonObject &obj);
    void broadcastJson(const QJsonObject &obj);
    void broadcastUserList();
    QStringList actorNames() const;
};

#endif
