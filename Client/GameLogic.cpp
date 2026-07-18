#include "GameLogic.h"
#include "../Common/Protocol.h"
#include <QJsonArray>

GameLogic::GameLogic(QObject *parent):QObject(parent)
{
}

void GameLogic::processMessage(const QJsonObject &obj)
{
    int type = obj["type"].toInt();
   
    if(type == Protocol::StartGame)
    {
        QString scene = obj["scene"].toString();
        QString role = obj["role"].toString();
        emit gameStarted(scene, role);
    }

    else if(type == Protocol::StartVote)
    {
        QStringList actors;
        QJsonArray array = obj["actors"].toArray();

        for(const auto &item : array)
        {
            actors.append(item.toString());
        }

        emit voteStarted(actors);
    }

    else if(type == Protocol::VoteResult)
    {
        QString winner = obj["winner"].toString();
        int votes = obj["votes"].toInt();
        emit voteFinished(winner, votes);
    }
}