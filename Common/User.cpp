#include "User.h"

User::User()
    : m_type(AUDIENCE), m_socket(nullptr), m_hasVoted(false)
{}

User::User(const QString& name, UserType type, QTcpSocket* sock)
    : m_userName(name), m_type(type), m_socket(sock), m_hasVoted(false)
{}

QString User::userName() const
{
    return m_userName;
}

void User::setUserName(const QString& name)
{
    m_userName = name;
}

UserType User::userType() const
{
    return m_type;
}

void User::setUserType(UserType type)
{
    m_type = type;
}

QTcpSocket* User::socket() const
{
    return m_socket;
}

void User::setSocket(QTcpSocket* sock)
{
    m_socket = sock;
}

bool User::hasVoted() const
{
    return m_hasVoted;
}

void User::setVoted(bool voted)
{
    m_hasVoted = voted;
}
