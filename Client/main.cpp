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
        
    LoginWindow login(&client);
    LobbyWindow lobby(&client);
    ChatWindow chat(&client);
    VoteWindow vote(&client);

    // Подключение к серверу после ввода IP
    QObject::connect(&login, &LoginWindow::serverIPRequested, [&client](const QString& ip) {
        client.connectServer(ip, SERVER_PORT);
    });

    // Чат → Голосование
    QObject::connect(&chat, &ChatWindow::gotoVote, &vote, &VoteWindow::show);
    
    // Лобби [Старт игры] → открыть чат
    QObject::connect(&lobby, &LobbyWindow::startGame, &chat, &ChatWindow::initUI);
    QObject::connect(&lobby, &LobbyWindow::startGame, &chat, &ChatWindow::show);
    
    // Успешный логин: распределение по ролям
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