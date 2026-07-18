#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

namespace Protocol {

enum Type {
    Login = 1,
    LoginResult,
    UserList,
    Chat,
    StartGame,
    StartVote,
    Vote,
    VoteResult,
    Audio
};

inline QByteArray packJson(const QJsonObject &obj)
{
    QByteArray payload = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    QByteArray frame;
    quint32 size = payload.size();
    frame.append(char((size >> 24) & 0xff));
    frame.append(char((size >> 16) & 0xff));
    frame.append(char((size >> 8) & 0xff));
    frame.append(char(size & 0xff));
    frame.append(payload);
    return frame;
}

inline QByteArray packBinary(quint8 type, const QByteArray &payload)
{
    QByteArray body;
    body.append(char(type));
    body.append(payload);

    QByteArray frame;
    quint32 size = body.size();
    frame.append(char((size >> 24) & 0xff));
    frame.append(char((size >> 16) & 0xff));
    frame.append(char((size >> 8) & 0xff));
    frame.append(char(size & 0xff));
    frame.append(body);
    return frame;
}

inline quint32 readSize(const QByteArray &b)
{
    return (quint8(b[0]) << 24) |
           (quint8(b[1]) << 16) |
           (quint8(b[2]) << 8) |
           quint8(b[3]);
}

}

#endif
