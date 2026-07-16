#include <QApplication>
#include "Client.h"
#include "LoginWindow.h"
#include "ChatWindow.h"
#include "LobbyWindow.h"
#include "VoteWindow.h"
#include "../Common/CommonDef.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client client;
    client.connectServer("26.176.5.58", SERVER_PORT);

    LoginWindow login(&client);
    LobbyWindow lobby(&client);
    ChatWindow chat(&client);
    VoteWindow vote(&client);

    QObject::connect(&chat, &ChatWindow::gotoVote, &vote, &VoteWindow::show);

    QObject::connect(&lobby, &LobbyWindow::startGame, &chat, &ChatWindow::initUI);
    QObject::connect(&lobby, &LobbyWindow::startGame, &chat, &ChatWindow::show);

    QObject::connect(&login, &LoginWindow::loginSuccess, [&](){
        if (client.userType() == ACTOR)
        {
            lobby.show(); 
        }
        else
        {
            chat.initUI();
            chat.show(); 
        }
    });

    login.show();
    return a.exec();
}
