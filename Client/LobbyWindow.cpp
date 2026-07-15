#include "LobbyWindow.h"
#include "ui_LobbyWindow.h"
#include "../Common/Packet.h"

LobbyWindow::LobbyWindow(Client* client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LobbyWindow)
    , m_client(client)
{
    ui->setupUi(this);
    connect(ui->btnStart, &QPushButton::clicked, this, &LobbyWindow::onBtnStartClicked);
}

LobbyWindow::~LobbyWindow()
{
    delete ui;
}

void LobbyWindow::onBtnStartClicked()
{
    m_client->sendPacket(Packet("START", ""));
    emit startGame();
    this->close();
}
