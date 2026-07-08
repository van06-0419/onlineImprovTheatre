#include "UserManager.h"

UserManager::UserManager()
{}

bool UserManager::addUser(const User& user)
{
    if (user.userType() == ACTOR && actorCount() >= MAX_ACTOR)
        return false;
    if (user.userType() == AUDIENCE && audienceCount() >= MAX_AUDIENCE)
        return false;

    m_userList.append(user);
    return true;
}

void UserManager::removeUser(QTcpSocket* sock)
{
    for (int i = 0; i < m_userList.size(); ++i)
    {
        if (m_userList[i].socket() == sock)
        {
            m_userList.removeAt(i);
            break;
        }
    }
}

int UserManager::actorCount() const
{
    int cnt = 0;
    for (const auto& u : m_userList)
        if (u.userType() == ACTOR) cnt++;
    return cnt;
}

int UserManager::audienceCount() const
{
    int cnt = 0;
    for (const auto& u : m_userList)
        if (u.userType() == AUDIENCE) cnt++;
    return cnt;
}

void UserManager::voteForUser(const QString& userName)
{
    m_voteCount[userName]++;
    for (auto& u : m_userList)
    {
        if (u.userName() == userName)
            u.setVoted(true);
    }
}

bool UserManager::allAudienceVoted() const
{
    for (const auto& u : m_userList)
    {
        if (u.userType() == AUDIENCE && !u.hasVoted())
            return false;
    }
    return true;
}

QString UserManager::getWinner() const
{
    if (m_voteCount.isEmpty()) return "暂无";
    QString winner;
    int maxVote = 0;
    for (auto it = m_voteCount.begin(); it != m_voteCount.end(); ++it)
    {
        if (it.value() > maxVote)
        {
            maxVote = it.value();
            winner = it.key();
        }
    }
    return winner;
}

void UserManager::resetVoteStatus()
{
    m_voteCount.clear();
    for (auto& u : m_userList)
        u.setVoted(false);
}
