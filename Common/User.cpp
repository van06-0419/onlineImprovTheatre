#include "User.h"

User::User()
    : m_userType(AUDIENCE),
      m_socket(nullptr),
      m_voted(false)
{
}

User::User(const QString& name, UserType type, QTcpSocket* sock)
    : m_userName(name),
      m_userType(type),
      m_socket(sock),
      m_voted(false)
{
}

QString User::userName() const
{
    return m_userName;
}

UserType User::userType() const
{
    return m_userType;
}

QTcpSocket* User::socket() const
{
    return m_socket;
}

bool User::hasVoted() const
{
    return m_voted;
}

void User::setVoted(bool voted)
{
    m_voted = voted;
}