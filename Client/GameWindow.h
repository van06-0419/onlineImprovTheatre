#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include <QMainWindow>
#include <QJsonObject>
#include "NetworkClient.h"
#include "AudioManager.h"
#include "GameLogic.h"
QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE
class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    GameWindow(
        const QString &name,
        const QString &identity,
        const QString &serverIp,
        quint16 serverPort,
        QWidget *parent = nullptr
    );
    ~GameWindow();
private slots:
    void onConnected();
    void sendMessage();
    void toggleMicrophone();
    void sendVote();
    void onDisconnectFromServer();
private:
    Ui::GameWindow *ui;
    QString m_name;
    QString m_identity;
    QString m_serverIp;
    quint16 m_serverPort;
    NetworkClient *m_network;
    AudioManager *m_audio;
    GameLogic *m_gameLogic;
    bool m_hasVoted;
private:
    void setupConnections();
    void sendJson(const QJsonObject &obj);
    void appendMessage(const QString &sender, const QString &text);
};
#endif