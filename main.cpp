#include <QCoreApplication>
#include <QDebug>
#include "Common/Role.h"
#include "Common/Scene.h"
#include "Common/Message.h"
#include "Common/Packet.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Role role;
    Scene scene;

    qDebug() << "Role:" << role.getRandomRole();
    qDebug() << "Scene:" << scene.getRandomScene();

    Message msg("Alice", "Hello");
    qDebug() << "Message Sender:" << msg.sender();
    qDebug() << "Message Content:" << msg.content();
    qDebug() << "Message Time:" << msg.time();

    Packet packet("LOGIN", "Alice");
    qDebug() << "Packet:" << packet.toString();

    return 0;


    
}