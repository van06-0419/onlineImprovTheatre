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
    client.connectServer("127.0.0.1", SERVER_PORT);

    LoginWindow login(&client);
    LobbyWindow lobby(&client);
    ChatWindow chat(&client);
    VoteWindow vote(&client);

    // 聊天窗 → 投票窗
    QObject::connect(&chat, &ChatWindow::gotoVote, &vote, &VoteWindow::show);

    // 大厅【开始游戏】→ 打开聊天窗口
    QObject::connect(&lobby, &LobbyWindow::startGame, &chat, &ChatWindow::initUI);
    QObject::connect(&lobby, &LobbyWindow::startGame, &chat, &ChatWindow::show);

    // 登录成功：按身份分流
    QObject::connect(&login, &LoginWindow::loginSuccess, [&](){
        if (client.userType() == ACTOR)
        {
            lobby.show(); // 演员进入大厅
        }
        else
        {
            chat.initUI();
            chat.show();  // 观众直接进聊天窗
        }
    });

    login.show();
    return a.exec();
}
