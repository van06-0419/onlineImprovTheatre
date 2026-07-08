#include <QCoreApplication>
#include "Server.h"
#include "../Common/CommonDef.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    server.startServer(SERVER_PORT);

    return a.exec();
}
