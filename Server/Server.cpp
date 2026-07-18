#include "Server.h"
#include "../Common/Protocol.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent)
{
    connect(&m_server, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

bool Server::start(quint16 port)
{
    if (!m_server.listen(QHostAddress::Any, port)) return false;
    qInfo() << "Server listening on port" << port;
    return true;
}

void Server::onNewConnection()
{
    while (m_server.hasPendingConnections())
    {
        QTcpSocket *socket = m_server.nextPendingConnection();
        m_clients.insert(socket, ClientInfo{});
        connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &Server::onDisconnected);
        qInfo() << "New connection:" << socket->peerAddress().toString();
    }
}

void Server::onReadyRead()
{
    auto *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket || !m_clients.contains(socket)) return;

    m_clients[socket].buffer += socket->readAll();
    processFrames(socket);
}

void Server::processFrames(QTcpSocket *socket)
{
    QByteArray &buffer = m_clients[socket].buffer;
    while (buffer.size() >= 4)
    {
        quint32 size = Protocol::readSize(buffer);
        if (buffer.size() < 4 + int(size)) return;

        QByteArray body = buffer.mid(4, size);
        buffer.remove(0, 4 + size);
        if (body.isEmpty()) continue;

        if (quint8(body[0]) == Protocol::Audio)
        {
            processAudio(socket, body.mid(1));
            continue;
        }

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(body, &error);
        if (error.error == QJsonParseError::NoError && doc.isObject())
            processJson(socket, doc.object());
    }
}

void Server::processJson(QTcpSocket *socket, const QJsonObject &obj)
{
    int type = obj["type"].toInt();

    if (type == Protocol::Login)
    {
        QString name = obj["name"].toString().trimmed();
        QString identity = obj["identity"].toString();
        int actors = 0, audience = 0;
        bool duplicate = false;

        for (const auto &c : m_clients)
        {
            if (!c.name.isEmpty() && c.name == name) duplicate = true;
            if (c.identity == "actor") actors++;
            if (c.identity == "audience") audience++;
        }

        QString error;
        if (name.isEmpty()) error = "Псевдоним не может быть пустым";
        else if (duplicate) error = "Псевдоним уже занят";
        else if (identity == "actor" && actors >= 5) error = "Достигнут лимит актёров";
        else if (identity == "audience" && audience >= 10) error = "Достигнут лимит зрителей";

        if (!error.isEmpty())
        {
            sendJson(socket, {{"type", Protocol::LoginResult}, {"ok", false}, {"message", error}});
            return;
        }

        m_clients[socket].name = name;
        m_clients[socket].identity = identity;
        sendJson(socket, {{"type", Protocol::LoginResult}, {"ok", true}, {"message", "Вход выполнен успешно"}});
        broadcastUserList();
        broadcastJson({{"type", Protocol::Chat}, {"sender", "Система"}, {"text", name + " вошёл в комнату"}});
    }
    else if (type == Protocol::Chat)
    {
        const ClientInfo &c = m_clients[socket];
        if (c.name.isEmpty()) return;
        broadcastJson({{"type", Protocol::Chat}, {"sender", c.name}, {"text", obj["text"].toString()}});
    }
    else if (type == Protocol::Vote && m_voting)
    {
        const ClientInfo &c = m_clients[socket];
        if (c.identity != "audience") return;
        QString actor = obj["actor"].toString();

        if (actorNames().contains(actor))
        {
            m_votes[actor]++;
            sendJson(socket, {{"type", Protocol::Chat}, {"sender", "Система"}, {"text", "Голос успешно принят"}});
        }
    }
}

void Server::processAudio(QTcpSocket *senderSocket, const QByteArray &audio)
{
    if (!m_clients.contains(senderSocket)) return;
    if (m_clients[senderSocket].identity != "actor") return;

    QByteArray frame = Protocol::packBinary(Protocol::Audio, audio);
    for (QTcpSocket *socket : m_clients.keys())
    {
        if (socket != senderSocket && socket->state() == QAbstractSocket::ConnectedState)
            socket->write(frame);
    }
}

void Server::sendJson(QTcpSocket *socket, const QJsonObject &obj)
{
    socket->write(Protocol::packJson(obj));
}

void Server::broadcastJson(const QJsonObject &obj)
{
    QByteArray data = Protocol::packJson(obj);
    for (QTcpSocket *socket : m_clients.keys())
    {
        if (socket->state() == QAbstractSocket::ConnectedState)
            socket->write(data);
    }
}

QStringList Server::actorNames() const
{
    QStringList result;
    for (const auto &c : m_clients)
    {
        if (c.identity == "actor" && !c.name.isEmpty()) result << c.name;
    }
    return result;
}

void Server::broadcastUserList()
{
    QJsonArray actors, audience;
    for (const auto &c : m_clients)
    {
        if (c.name.isEmpty()) continue;
        if (c.identity == "actor") actors.append(c.name);
        else audience.append(c.name);
    }
    broadcastJson({{"type", Protocol::UserList}, {"actors", actors}, {"audience", audience}});
}

void Server::startGame()
{
    QStringList actors = actorNames();
    if (actors.isEmpty())
    {
        qInfo() << "No actors online.";
        return;
    }

    QStringList scenes = {
        "На корабле, летящем к Марсу, произошла внезапная поломка",
        "В таинственном ночном ресторане появились странные гости",
        "Все оказались запертыми в обесточенном музее",
        "Перед свадьбой жених внезапно исчез"
    };

    QStringList roles = {
        "Самоуверенный капитан",
        "Врач с тайной",
        "Нервный журналист",
        "Детектив, постоянно неверно понимающий ситуацию",
        "Спокойный, но подозрительный учёный"
    };

    std::shuffle(roles.begin(), roles.end(), *QRandomGenerator::global());
    QString scene = scenes.at(QRandomGenerator::global()->bounded(scenes.size()));

    for (QTcpSocket *socket : m_clients.keys())
    {
        const ClientInfo &c = m_clients[socket];
        QString role;
        if (c.identity == "actor")
        {
            int index = actors.indexOf(c.name);
            role = roles.at(index % roles.size());
        }
        sendJson(socket, {{"type", Protocol::StartGame}, {"scene", scene}, {"role", role}});
    }

    m_gameStarted = true;
    m_voting = false;
    qInfo() << "Performance started.";
}

void Server::startVote()
{
    if (!m_gameStarted)
    {
        qInfo() << "Start the game first.";
        return;
    }

    QJsonArray actors;
    for (const QString &name : actorNames()) actors.append(name);
    m_votes.clear();
    m_voting = true;
    broadcastJson({{"type", Protocol::StartVote}, {"actors", actors}});
    qInfo() << "Voting started.";
}

void Server::finishVote()
{
    if (!m_voting)
    {
        qInfo() << "Voting is not active.";
        return;
    }

    QString winner = "Пока нет";
    int maxVotes = -1;
    for (const QString &actor : actorNames())
    {
        int count = m_votes.value(actor, 0);
        if (count > maxVotes)
        {
            maxVotes = count;
            winner = actor;
        }
    }

    broadcastJson({{"type", Protocol::VoteResult}, {"winner", winner}, {"votes", qMax(0, maxVotes)}});
    m_voting = false;
    m_gameStarted = false;
    qInfo() << "Voting finished. Winner:" << winner;
}

void Server::onDisconnected()
{
    auto *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QString name = m_clients.value(socket).name;
    m_clients.remove(socket);
    socket->deleteLater();

    if (!name.isEmpty())
    {
        broadcastJson({{"type", Protocol::Chat}, {"sender", "Система"}, {"text", name + " покинул комнату"}});
        broadcastUserList();
    }
}