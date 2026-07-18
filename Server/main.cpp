#include <QCoreApplication>
#include <QTextStream>
#include <QThread>
#include <QDebug>
#include "Server.h"

class ConsoleThread : public QThread
{
    Q_OBJECT

signals:
    void commandReceived(const QString &command);

protected:
    void run() override
    {
        QTextStream input(stdin);

        while (!isInterruptionRequested()) {
            QString command = input.readLine().trimmed();

            if (!command.isEmpty()) {
                emit commandReceived(command);
            }
        }
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Server server;

    if (!server.start(8888)) {
        qCritical() << "Failed to start server.";
        return 1;
    }

    qInfo() << "=== Online Improv Theatre Server ===";
    qInfo() << "1 - Start game";
    qInfo() << "2 - Start voting";
    qInfo() << "3 - Finish voting";
    qInfo() << "q - Quit";

    ConsoleThread consoleThread;

    QObject::connect(
        &consoleThread,
        &ConsoleThread::commandReceived,
        &app,
        [&](const QString &command)
        {
            qInfo() << "Command received:" << command;

            if (command == "1") {
                server.startGame();
            }
            else if (command == "2") {
                server.startVote();
            }
            else if (command == "3") {
                server.finishVote();
            }
            else if (command == "q") {
                app.quit();
            }
        }
    );

    consoleThread.start();

    int result = app.exec();

    consoleThread.requestInterruption();
    consoleThread.terminate();
    consoleThread.wait();

    return result;
}

#include "main.moc"