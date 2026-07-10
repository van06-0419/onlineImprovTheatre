#ifndef CHATWINDOW_H
#define CHATWINDOW_H
#include <QWidget>
#include "Client.h"
QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE
class ChatWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWindow(Client* client, QWidget *parent = nullptr);
    ~ChatWindow();
public slots:
    void initUI();
private slots:
    void onRecvPacket(Packet pkt);
    void onBtnSendClicked();
    void onBtnVoteClicked();
signals:
    void gotoVote();
private:
    Ui::ChatWindow *ui;
    Client* m_client;
    QString m_curScene;
    QString m_curRole;
    QString m_allActorRole; // 全体演员角色列表


};
#endif // CHATWINDOW_H
