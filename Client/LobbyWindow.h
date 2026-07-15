#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QWidget>
#include "Client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LobbyWindow; }
QT_END_NAMESPACE

class LobbyWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LobbyWindow(Client* client, QWidget *parent = nullptr);
    ~LobbyWindow();

signals:
    void startGame();

private slots:
    void onBtnStartClicked();

private:
    Ui::LobbyWindow *ui;
    Client* m_client;
};

#endif // LOBBYWINDOW_H
