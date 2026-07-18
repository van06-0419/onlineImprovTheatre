#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include "GameWindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
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
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Уведомление", "Введите псевдоним");
        return;
    }

    QString identity = ui->actorRadio->isChecked() ? "actor" : "audience";
    auto *game = new GameWindow(name, identity);
    game->setAttribute(Qt::WA_DeleteOnClose);
    game->show();
    close();
}
