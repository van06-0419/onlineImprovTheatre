#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include "../Common/Packet.h"
#include "../Common/CommonDef.h"
#include <QMessageBox>
#include <QDebug>

LoginWindow::LoginWindow(Client* client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
    , m_client(client)
{
    ui->setupUi(this);
    connect(ui->btnLogin, &QPushButton::clicked, this, &LoginWindow::onBtnLoginClicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::onBtnLoginClicked()
{
    QString name = ui->editName->text().trimmed();
    if (name.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Name can not be empty");
        return;
    }

    QString selectText = ui->cboxType->currentText().trimmed();
    

    UserType type = AUDIENCE;
    if (selectText == "Актёр")
    {
        type = ACTOR;
        qDebug() << "This is actor (ACTOR), type = 0";
    }
    else if (selectText == "Зритель")
    {
        type = AUDIENCE;
        qDebug() << "This is audience (AUDIENCE), type = 1";
    }

    m_client->setUserName(name);
    m_client->setUserType(type);
    qDebug() << "Saved type in Client: " << m_client->userType();

    QString typeStr = (type == ACTOR) ? "ACTOR" : "AUDIENCE";
    Packet pkt("LOGIN", name + "," + typeStr);
    m_client->sendPacket(pkt);

    emit loginSuccess();
    this->close();
}

