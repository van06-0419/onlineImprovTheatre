#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "../Common/User.h"
#include "../Common/CommonDef.h"
#include <QVector>
#include <QMap>
#include <QTcpSocket>

class UserManager
{
public:
    UserManager();
    bool addUser(const User& user);
    void removeUser(QTcpSocket* sock);
    int actorCount() const;
    int audienceCount() const;
    void voteForUser(const QString& userName);
    bool allAudienceVoted() const;
    QString getWinner() const;
    void resetVoteStatus();
    
    const QVector<User>& users() const;
    User* findUser(QTcpSocket* sock);
    QString userName(QTcpSocket* sock) const;

private:
    QVector<User> m_userList;
    QMap<QString, int> m_voteCount;
};

#endif // USERMANAGER_H