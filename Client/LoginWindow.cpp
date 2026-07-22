#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include "GameWindow.h"
#include <QMessageBox>
LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    // 默认本地地址，方便测试
    ui->ipEdit->setText("127.0.0.1");
    ui->portEdit->setText("8888");
    connect(ui->loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginClicked);
}
LoginWindow::~LoginWindow()
{
    delete ui;
}
void LoginWindow::onLoginClicked()
{
    QString name = ui->nameEdit->text().trimmed();
    QString ip = ui->ipEdit->text().trimmed();
    QString portStr = ui->portEdit->text().trimmed();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Уведомление", "Введите псевдоним");
        return;
    }
    if (ip.isEmpty() || portStr.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите IP и порт сервера");
        return;
    }
    bool ok;
    quint16 port = portStr.toUShort(&ok);
    if (!ok || port <= 0 || port > 65535) {
        QMessageBox::warning(this, "Ошибка", "Неверный порт");
        return;
    }

    QString identity = ui->actorRadio->isChecked() ? "actor" : "audience";
    // 把远程IP、端口传给GameWindow
    auto *game = new GameWindow(name, identity, ip, port);
    game->setAttribute(Qt::WA_DeleteOnClose);
    game->show();
    close();
}