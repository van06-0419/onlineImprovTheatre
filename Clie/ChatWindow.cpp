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
    
    // ← ДОБАВЛЕНО: создание VoiceChat и подключение кнопки микрофона
    m_voiceChat = new VoiceChat(m_client, this);
    connect(ui->btnMic, &QPushButton::toggled, this, &ChatWindow::onMicToggled);
    // ← КОНЕЦ ДОБАВЛЕНИЯ
    
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
        QStringList parts = data.split("|");
        if (parts.size() >= 3)
        {
            m_curScene      = parts[0];
            m_curRole       = parts[1];
            m_allActorRole  = parts[2];

            if (m_client->userType() == ACTOR)
            {
                ui->labScene->setText("Сцена：" + m_curScene);
                ui->labRole->setText("Ваша роль：" + m_curRole);
            }
            else
            {
                ui->labScene->setText("Сцена：" + m_curScene);
                ui->labRole->setText("Актёры и их роли：");
                ui->txtChat->clear();
                QStringList actorList = m_allActorRole.split(",");
                for (const QString& item : actorList)
                    ui->txtChat->append(item);
            }
        }
    }
    else if (cmd == "CHAT" || cmd == "MSG")
    {
        ui->txtChat->append(data);
    }
    // ← ДОБАВЛЕНО: обработка голосовых пакетов
    else if (cmd == "VOICE")
    {
        // Формат: <имя>|<base64_pcm>
        int sep = data.indexOf('|');
        if (sep != -1)
        {
            QString sender = data.left(sep);
            QByteArray base64Pcm = data.mid(sep + 1).toUtf8();
            m_voiceChat->handleVoicePacket(sender, base64Pcm);
        }
    }
    // ← КОНЕЦ ДОБАВЛЕНИЯ
}

void ChatWindow::onBtnSendClicked()
{
    QString msg = ui->editMsg->text().trimmed();
    if (msg.isEmpty()) return;
    QString sendContent = m_client->userName() + "：" + msg;
    m_client->sendPacket(Packet("CHAT", sendContent));
    ui->editMsg->clear();
}

void ChatWindow::onBtnVoteClicked()
{
    emit gotoVote();
    this->close();
}

// ← ДОБАВЛЕНО: новый метод для обработки переключения микрофона
void ChatWindow::onMicToggled(bool checked)
{
    if (checked)
    {
        m_voiceChat->startMicrophone();
        ui->btnMic->setText("🎤 Выключить микрофон");
    }
    else
    {
        m_voiceChat->stopMicrophone();
        ui->btnMic->setText("🎤 Включить микрофон");
    }
}
