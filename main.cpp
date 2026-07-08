#include <QCoreApplication>
#include <QDebug>
#include <QTcpSocket>

// Базовые общие классы
#include "Common/Role.h"
#include "Common/Scene.h"
#include "Common/Message.h"
#include "Common/Packet.h"
#include "Common/User.h"
#include "Common/CommonDef.h"

// Новые бизнес-модули
#include "Server/UserManager.h"
#include "Server/Room.h"
#include "Server/Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "\n1. Проверка менеджера пользователей (новый функционал)";
    UserManager userMgr;
    // Имитация актёров и зрителей (нулевой сокет для офлайн-теста)
    User actor1("Детектив", ACTOR, nullptr);
    User actor2("Рыцарь", ACTOR, nullptr);
    User aud1("Зритель Ваня", AUDIENCE, nullptr);
    User aud2("Зритель Лёша", AUDIENCE, nullptr);

    // Добавление пользователей
    userMgr.addUser(actor1);
    userMgr.addUser(actor2);
    userMgr.addUser(aud1);
    userMgr.addUser(aud2);
    qDebug() << "Текущее количество актёров:" << userMgr.actorCount();
    qDebug() << "Текущее количество зрителей:" << userMgr.audienceCount();

    // Проверка логики голосования
    userMgr.voteForUser("Детектив");
    userMgr.voteForUser("Детектив");
    qDebug() << "Все зрители проголосовали:" << userMgr.allAudienceVoted();
    qDebug() << "Актёр с наибольшим количеством голосов:" << userMgr.getWinner();

    // Сброс статуса голосов
    userMgr.resetVoteStatus();
    qDebug() << "После сброса статус голосования всех зрителей:" << userMgr.allAudienceVoted();

    qDebug() << "\n2. Проверка пакетов комнаты";
    Room room;
    Scene scene;
    Role role;
    Packet chatPkt("CHAT", "Начинаем игру!");
    Packet startPkt("START", scene.getRandomScene() + "|" + role.getRandomRole());
    qDebug() << "Пакет широковещательного чата:" << chatPkt.toString();
    qDebug() << "Пакет старта сцены и роли:" << startPkt.toString();

    qDebug() << "\n3. Запуск TCP-сервера полноценное тестирование сети";
    Server server;
    bool startOk = server.startServer(SERVER_PORT);
    if (startOk)
    {
        qDebug() << "Сервер запущен успешно, порт для прослушивания:" << SERVER_PORT;
        qDebug() << "Ожидаю подключения TCP-клиентов, можно тестировать команды через сетевые утилиты";
    }
    else
    {
        qDebug() << "Ошибка запуска сервера, проверьте занятость порта";
    }

    return app.exec();
}