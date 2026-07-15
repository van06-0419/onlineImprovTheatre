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
    UserType userType() const;
    QTcpSocket* socket() const;

    bool hasVoted() const;
    void setVoted(bool voted);

private:
    QString      m_userName;
    UserType     m_userType;
    QTcpSocket*  m_socket;
    bool         m_voted;
};

#endif // USER_H