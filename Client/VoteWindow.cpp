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
        QMessageBox::warning(this, "Error", "Pleaae, select an actor!");
        return;
    }
    m_client->sendPacket(Packet("VOTE", selectName));
    QMessageBox::information(this, "Vote received", "Thank you for voting!");
    this->close();
}
