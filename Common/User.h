#ifndef USER_H
#define USER_H

#include <QString>
#include <QTcpSocket>
#include "CommonDef.h"

class User
{
public:
    User();
    User(const QString& name, UserType type, QTcpSocket* sock);

    QString userName() const;
    void setUserName(const QString& name);

    UserType userType() const;
    void setUserType(UserType type);

    QTcpSocket* socket() const;
    void setSocket(QTcpSocket* sock);

    bool hasVoted() const;
    void setVoted(bool voted);

private:
    QString     m_userName;
    UserType    m_type;
    QTcpSocket* m_socket;
    bool        m_hasVoted;
};

#endif // USER_H
