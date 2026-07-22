#include "GameWindow.h"
#include "ui_GameWindow.h"
#include "../Common/Protocol.h"
#include <QMessageBox>
#include <QDateTime>
#include <QJsonArray>
GameWindow::GameWindow(
        const QString &name,
        const QString &identity,
        const QString &serverIp,
        quint16 serverPort,
        QWidget *parent)
    :
    QMainWindow(parent),
    ui(new Ui::GameWindow),
    m_name(name),
    m_identity(identity),
    m_serverIp(serverIp),
    m_serverPort(serverPort),
    m_network(new NetworkClient(this)),
    m_audio(new AudioManager(this)),
    m_gameLogic(new GameLogic(this)),
    m_hasVoted(false)
{
    ui->setupUi(this);
    ui->identityLabel->setText(identity=="actor" ? "Статус: Актёр" : "Статус: Зритель");
    ui->roleValue->setText(identity=="actor" ? "Ожидание распределения" : "Зрители без роли");
    ui->voteGroup->setVisible(false);
    ui->micButton->setVisible(identity=="actor");
    setupConnections();
    // 使用传入的远程IP端口，不再写死127.0.0.1
    m_network->connectServer(m_serverIp, m_serverPort);
}

GameWindow::~GameWindow()
{
    delete ui;
}
void GameWindow::setupConnections()
{
    connect(m_network, &NetworkClient::connected, this, &GameWindow::onConnected);
    connect(m_network, &NetworkClient::disconnected, this, &GameWindow::onDisconnectFromServer);
    connect(m_network, &NetworkClient::audioReceived, m_audio, &AudioManager::playAudio);
    connect(m_audio, &AudioManager::audioCaptured, m_network, &NetworkClient::sendAudio);
    connect(m_network, &NetworkClient::jsonReceived, this, [this](const QJsonObject &obj)
    {
        int type = obj["type"].toInt();
        if(type == Protocol::LoginResult)
        {
            if(!obj["ok"].toBool())
            {
                QMessageBox::critical(this, "Ошибка входа", obj["message"].toString());
                close();
            }
            else
            {
                appendMessage("Система", "Подключение к серверу театра установлено");
            }
        }
        else if(type == Protocol::UserList)
        {
            ui->actorList->clear();
            ui->audienceList->clear();

            for(auto v : obj["actors"].toArray()) { ui->actorList->addItem("🎭 " + v.toString()); }
            for(auto v : obj["audience"].toArray()) { ui->audienceList->addItem("👁 " + v.toString()); }
        }
        else if(type == Protocol::Chat)
        {
            appendMessage(obj["sender"].toString(), obj["text"].toString());
        }
        else
        {
            m_gameLogic->processMessage(obj);
        }
    });
    connect(m_gameLogic, &GameLogic::gameStarted, this, [this](QString scene, QString role)
    {
        ui->sceneValue->setText(scene);
        if(m_identity == "actor") ui->roleValue->setText(role);
        ui->statusLabel->setText("● Идёт представление");
        appendMessage("Система", "Представление началось!");
    });
    connect(m_gameLogic, &GameLogic::voteStarted, this, [this](QStringList actors)
    {
        ui->actorCombo->clear();
        ui->actorCombo->addItems(actors);
        if(m_identity == "audience") ui->voteGroup->setVisible(true);
        appendMessage("Система", "Представление завершено, началось голосование!");
    });
    connect(m_gameLogic, &GameLogic::voteFinished, this, [this](QString winner, int votes)
    {
        ui->voteGroup->setVisible(false);
        QString text = QString("Лучший актёр: %1 (%2 голосов)").arg(winner).arg(votes);
        QMessageBox::information(this, "Результат голосования", text);
        appendMessage("Система", text);
    });
    connect(ui->sendButton, &QPushButton::clicked, this, &GameWindow::sendMessage);
    connect(ui->messageEdit, &QLineEdit::returnPressed, this, &GameWindow::sendMessage);
    connect(ui->micButton, &QPushButton::clicked, this, &GameWindow::toggleMicrophone);
    connect(ui->voteButton, &QPushButton::clicked, this, &GameWindow::sendVote);
}
void GameWindow::onConnected()
{
    sendJson({ {"type", Protocol::Login}, {"name", m_name}, {"identity", m_identity} });
}
void GameWindow::sendMessage()
{
    QString text = ui->messageEdit->text().trimmed();
    if(text.isEmpty()) return;

    sendJson({ {"type", Protocol::Chat}, {"text", text} });
    ui->messageEdit->clear();
}
void GameWindow::toggleMicrophone()
{
    if(m_audio->isMicrophoneOn())
    {
        m_audio->stopMicrophone();
        ui->micButton->setText("🎤 Включить микрофон");
    }
    else
    {
        m_audio->startMicrophone();
        ui->micButton->setText("🎙 Выключить микрофон");
    }
}
void GameWindow::sendVote()
{
    if(m_hasVoted)
    {
        QMessageBox::information(this, "Уведомление", "Вы уже проголосовали");
        return;
    }

    QString actor = ui->actorCombo->currentText();
    if(actor.isEmpty()) return;

    sendJson({ {"type", Protocol::Vote}, {"actor", actor} });
    m_hasVoted = true;
    ui->voteButton->setEnabled(false);
}
void GameWindow::sendJson(const QJsonObject &obj)
{
    m_network->sendJson(obj);
}
void GameWindow::appendMessage(const QString &sender, const QString &text)
{
    QString time = QDateTime::currentDateTime().toString("HH:mm");
    ui->chatBrowser->append(
        QString(
        "<p><span style='color:#8b95a7'>%1</span> "
        "<b style='color:#e8ecf4'>%2</b><br>"
        "<span style='color:#cbd2df'>%3</span></p>"
        )
        .arg(time, sender.toHtmlEscaped(), text.toHtmlEscaped())
    );
}

void GameWindow::onDisconnectFromServer()
{
    QMessageBox::critical(this, "Потеря соединения", "Отключен от сервера, окно закроется");
    this->close();
}