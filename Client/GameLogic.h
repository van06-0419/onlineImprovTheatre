#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include <QObject>
#include <QJsonObject>
#include <QStringList>

class GameLogic : public QObject
{
    Q_OBJECT
public:
    explicit GameLogic(QObject *parent = nullptr);
    void processMessage(const QJsonObject &obj);

signals:
    void gameStarted(const QString &scene, const QString &role);
    void voteStarted(const QStringList &actors);
    void voteFinished(const QString &winner, int votes);
};

#endif