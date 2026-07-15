#include "VoteWindow.h"
#include "ui_VoteWindow.h"
#include "../Common/Packet.h"
#include "../Common/CommonDef.h"
#include <QMessageBox>

VoteWindow::VoteWindow(Client* client, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VoteWindow)
    , m_client(client)
{
    ui->setupUi(this);
    connect(ui->btnVote, &QPushButton::clicked, this, &VoteWindow::onBtnVoteClicked);
    // 监听网络数据包，获取演员列表
    connect(m_client, &Client::recvPacket, this, [this](Packet pkt){
        if (pkt.command() == "START")
        {
            QStringList parts = pkt.data().split("|");
            if (parts.size() >= 3)
            {
                QString allActor = parts[2];
                QStringList actorRoleList = allActor.split(",");
                m_actorList.clear();
                for (const QString& s : actorRoleList)
                {
                    QString name = s.split(":").first();
                    m_actorList << name;
                }
                // 刷新下拉框
                ui->cboxActor->clear();
                ui->cboxActor->addItems(m_actorList);
            }
        }
    });
}

VoteWindow::~VoteWindow()
{
    delete ui;
}

void VoteWindow::onBtnVoteClicked()
{
    QString selectName = ui->cboxActor->currentText();
    if (selectName.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Выберите актёра!");
        return;
    }
    m_client->sendPacket(Packet("VOTE", selectName));
    QMessageBox::information(this, "Голос принят", "Спасибо за голосование!");
    this->close();
}
