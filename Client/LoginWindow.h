#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void onLoginClicked();

private:
    Ui::LoginWindow *ui;
    // UI新增：IP输入框、端口输入框
    // ui控件名：ipEdit、portEdit
};

#endif