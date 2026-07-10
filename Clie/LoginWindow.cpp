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
        QMessageBox::warning(this, "提示", "Имя не может быть пустым");
        return;
    }

    QString selectText = ui->cboxType->currentText().trimmed();
    qDebug() << "Выбранный текст: |" << selectText << "|";

    UserType type = AUDIENCE;
    // 匹配俄语：Актёр = 演员
    if (selectText == "Актёр")
    {
        type = ACTOR;
        qDebug() << "Определено как Актёр (ACTOR), type = 0";
    }
    else if (selectText == "Зритель")
    {
        type = AUDIENCE;
        qDebug() << "Определено как Зритель (AUDIENCE), type = 1";
    }

    m_client->setUserName(name);
    m_client->setUserType(type);
    qDebug() << "Сохранённый тип в Client: " << m_client->userType();

    QString typeStr = (type == ACTOR) ? "ACTOR" : "AUDIENCE";
    Packet pkt("LOGIN", name + "," + typeStr);
    m_client->sendPacket(pkt);

    emit loginSuccess();
    this->close();
}

