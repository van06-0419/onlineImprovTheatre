#include "ChatWindow.h"
#include "ui_ChatWindow.h"
#include "../Common/Packet.h"
#include "../Common/CommonDef.h"
#include <QDebug>
#include <QStringList>
ChatWindow::ChatWindow(Client* client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWindow)
    , m_client(client)
{
    ui->setupUi(this);
    connect(m_client, &Client::recvPacket, this, &ChatWindow::onRecvPacket);
    connect(ui->btnSend, &QPushButton::clicked, this, &ChatWindow::onBtnSendClicked);
    connect(ui->btnVote, &QPushButton::clicked, this, &ChatWindow::onBtnVoteClicked);
}
ChatWindow::~ChatWindow()
{
    delete ui;
}
void ChatWindow::initUI()
{
    qDebug() << "ChatWindow::initUI 读取用户类型：" << m_client->userType();
    if (m_client->userType() == ACTOR)
    {
        ui->editMsg->setEnabled(true);
        ui->btnSend->setEnabled(true);
        ui->btnVote->setVisible(false);
        qDebug() << "演员：输入框启用，投票按钮隐藏";
    }
    else
    {
        ui->editMsg->setEnabled(false);
        ui->btnSend->setEnabled(false);
        ui->btnVote->setVisible(true);
        qDebug() << "观众：输入框禁用，投票按钮显示";
    }
}
void ChatWindow::onRecvPacket(Packet pkt)
{
    QString cmd = pkt.command();
    QString data = pkt.data();
    if (cmd == "START")
    {
        // 协议：场景 | 本人角色 | 全体演员:角色列表
        QStringList parts = data.split("|");
        if (parts.size() >= 3)
        {
            QString newScene = parts[0];
            QString newSelfRole = parts[1];
            QString newAllActorRole = parts[2];

            m_curScene = newScene;
            m_curRole = newSelfRole;
            m_allActorRole = newAllActorRole;

            // 区分身份展示
            if (m_client->userType() == ACTOR)
            {
                // 演员：显示场景 + 个人角色
                ui->labScene->setText("Сцена：" + m_curScene);
                ui->labRole->setText("Ваша роль：" + m_curRole);
            }
            else
            {
                // 观众：只显示场景，展示所有演员和角色
                ui->labScene->setText("Сцена：" + m_curScene);
                ui->labRole->setText("Актёры и их роли：");

                ui->txtChat->clear();
                QStringList actorList = m_allActorRole.split(",");
                for (const QString& item : actorList)
                {
                    ui->txtChat->append(item);
                }
            }
        }
    }
    else if (cmd == "CHAT" || cmd == "MSG")
    {
        ui->txtChat->append(data);
    }
}

void ChatWindow::onBtnSendClicked()
{
    QString msg = ui->editMsg->text().trimmed();
    if (msg.isEmpty())
        return;
    QString sendContent = m_client->userName() + "：" + msg;
    m_client->sendPacket(Packet("CHAT", sendContent));
    ui->editMsg->clear();
}
void ChatWindow::onBtnVoteClicked()
{
    emit gotoVote();
    this->close();
}
