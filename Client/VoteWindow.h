#ifndef VOTEWINDOW_H
#define VOTEWINDOW_H

#include <QWidget>
#include "Client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class VoteWindow; }
QT_END_NAMESPACE

class VoteWindow : public QWidget
{
    Q_OBJECT

public:
    explicit VoteWindow(Client* client, QWidget *parent = nullptr);
    ~VoteWindow();

private slots:
    void onBtnVoteClicked();

private:
    Ui::VoteWindow *ui;
    Client* m_client;
    QStringList m_actorList;

};

#endif // VOTEWINDOW_H
