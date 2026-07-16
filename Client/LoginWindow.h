#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "Client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWindow(Client* client, QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void loginSuccess();
    void serverIPRequested(const QString& ip);

private slots:
    void onBtnLoginClicked();

private:
    Ui::LoginWindow *ui;
    Client* m_client;
};

#endif // LOGINWINDOW_H